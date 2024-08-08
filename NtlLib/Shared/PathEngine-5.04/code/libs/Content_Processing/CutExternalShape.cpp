// **********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
// **********************************************************************

#include "platform_common/Header.h"
#include "CutExternalShape.h"
#include "ExternalShapeTraversal.h"
#include "libs/Mesh3D/interface/tMesh_3D.h"
#include "libs/Mesh_Common/interface/Mesh.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Geometry/interface/tLine.h"
#include "common/Containers/BitVector.h"
#include "common/TemplateMesh/External.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/TemplateMesh/Helper.h"
#include "common/interface/Error.h"
#include "common/TemplateMesh/ValidateMeshConnections.h"

using std::vector;

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;
typedef tVertex_3D_FaceTracked tVertex;

static void
SplitFace_ClearIndices(tMesh& mesh, tEdge e1, tEdge e2)
{
    mesh.splitFace(e1, e2, tMesh::tEdgeData(e1->getZ()), tMesh::tEdgeData(e2->getZ()));
}

static void
InsertVertices(tMesh& mesh, cExternalShapeTraversal& traversal, const cMeshElement<tMesh>& traversalEnd)
{
    cMeshElement<tMesh> nullEl;
    nullEl.clear();
    cMeshElement<tMesh> el;
    do
    {
        el = traversal.advance();
        tPoint p;
        if(traversal.atShapeVertex(p))
        {
            if(el.isFace())
            {
                tMesh::tVertexData vertexData(p.getX(), p.getY());
                tFace toSplit = el.asFace();
                float z = toSplit->heightForF(p);
                tMesh::tEdgeData edgeData[6];
                tEdge edges[3];
                tEdge e = tMesh::beginEdge(toSplit);
                long i;
                for(i = 0; i != 3; ++i)
                {
                    edges[i] = e++;
                    edgeData[i * 2]._z = z;
                    edgeData[i * 2 + 1]._z = e->getZ();
                }
                tVertex v = mesh.splitFaceWithVertex(toSplit, 3, vertexData, edgeData, edges);
                traversal.meshModified(nullEl, v);
            }
            else
            if(el.isEdge())
            {
                tEdge e = el.asEdge();
              // ** note the edge affected flags are not set here because behind portal edges should be able to survive being modified in this way
                tMesh::tVertexData vertexData(p.getX(), p.getY());
                tMesh::tEdgeData edgeData;
                if(e.face()->isExternal())
                {
                    edgeData._z = e.twin().face()->heightForF(p);
                }
                else
                {
                    edgeData._z = e.face()->heightForF(p);
                }
                mesh.splitEdge(e, vertexData, edgeData);
                if(!e.face()->isExternal())
                {
                    SplitFace_ClearIndices(mesh, e.next(), e.prev());
                }
                if(!e.twin().face()->isExternal())
                {
                    SplitFace_ClearIndices(mesh, e.twin(), e.twin().next().next());
                }
                traversal.meshModified(nullEl, e.next().vertex());
            }
        }
    }
    while(el != traversalEnd);
}



static void
DeleteConnectedFaces(tMesh& mesh, tFace startFrom)
{
    assertD(!startFrom->isExternal());
    tEdge e = tMesh::beginEdge(startFrom);
    do
    {
        if(e.twin().valid())
        {
            tFace f = e.twin().face();
            if(f->isExternal())
            {
                mesh.eraseFace(f);
            }
            else
            {
                mesh.breakEdge(e);
                DeleteConnectedFaces(mesh, f);
                if(e.entryIsDead())
                {
                    return;
                }
            }
        }
    }
    while(e.toNextInFace());
    assertD(!startFrom.entryIsDead());
    mesh.eraseFace(startFrom);
}

static tPoint
IntersectionApproximatedToLeft(const tLine& line1, const tLine& line2)
{
    tPoint pointArray[4];
    long points;
    tIntersection intersection(line1, line2);
    intersection.approximatePoints_IsOnAxis(points, pointArray);
    if(points == 1)
    {
        return pointArray[0];
    }
    if(points == 2)
    {
        eSide side = line1.side(pointArray[0]);
        if(side == SIDE_LEFT)
        {
            return pointArray[0];
        }
        if(side == SIDE_RIGHT)
        {
            return pointArray[1];
        }
        eDirection direction = line1.axis().direction_KnownToBeInline(pointArray[1] - pointArray[0]);
        if(direction == DIRECTION_FORWARD)
        {
            return pointArray[0];
        }
        assertD(direction == DIRECTION_FORWARD);
        return pointArray[1];
    }
  // actually returns the first approximation on the left of the line
  // (note that the points are guaranteed to be ordered clockwise in the case of 4 points)
    long i;
    for(i = 0; i != points; ++i)
    {
        if(line1.side(pointArray[i]) == SIDE_RIGHT)
        {
            break;
        }
    }
    assertD(i != points);
    long j;
    for(j = 0; j != points; ++j)
    {
        ++i;
        if(i == points)
        {
            i = 0;
        }
        if(line1.side(pointArray[i]) != SIDE_RIGHT)
        {
            return pointArray[i];
        }
    }
    invalid();
    return pointArray[0];
}

static void
DoSplitEdge(tMesh& mesh, tEdge e, const tLine& splitLine, cGeometryOverlay<tMesh>& overlay)
{
    tLine edgeLine = overlay.getEdgeLine(e);
    tPoint p = IntersectionApproximatedToLeft(splitLine, edgeLine);
    tMesh::tVertexData vertexData(p.getX(), p.getY());
    tMesh::tEdgeData edgeData;
    edgeData._z = e.face()->heightForF(p);
    mesh.splitEdge(e, vertexData, edgeData);
    overlay.pushBack(e, edgeLine);
    overlay.pushBack(e.next(), edgeLine);
    edgeLine.reverse();
    overlay.pushBack(e.twin(), edgeLine);
    overlay.pushBack(e.next().twin(), edgeLine);
}

static void
CutExternalShape(tMesh& mesh, tEdge ePortal, tEdge eShape)
{
    assertF(ValidateMeshConnections(mesh));

    cGeometryOverlay<tMesh> overlay;

    {
        cExternalShapeTraversal traversal(overlay, ePortal, ePortal.vertex(), eShape.next());
        InsertVertices(mesh, traversal, ePortal.vertex());
    }

//    assertF(ValidateMeshConnections(mesh));

    std::vector<tEdge> edgesToBreak;
    {
        cExternalShapeTraversal traversal(overlay, ePortal, ePortal.vertex(), eShape.next());
        cMeshElement<tMesh> el1(ePortal.vertex());
        do
        {
            tLine splitLine = traversal.getCurrentLine();
            cMeshElement<tMesh> el2 = traversal.advance();
            cMeshElement<tMesh> el3 = traversal.advance();
            if(el2.isEdge())
            {
                tEdge e = el2.asEdgeFromVertex(el1.asVertex());
                if(!e.face()->isExternal())
                {
                    edgesToBreak.push_back(e);
                }
                el1 = el3;
                continue;
            }
            if(el2.isExternalFace())
            {
                assertD(el1.isVertex());
                el1 = el3;
                continue;
            }

            tEdge eVertex;
            if(!el1.isEdge())
            {
                eVertex = EdgeInFaceForVertex<tMesh>(el2.asFace(), el1.asVertex());
            }
            else
            {
                eVertex = el1.asEdgeInFace(el2.asFace());
            }

            assertF(ValidateMeshConnections(mesh));

            vector<tEdge> edgesToSplit;
            while(el3.isEdge() && !el3.asEdgeInFace(el2.asFace()).twin().face()->isExternal())
            {
                edgesToSplit.push_back(el3.asEdgeInFace(el2.asFace()));
                el2 = traversal.advance();
                el3 = traversal.advance();
            }

            if(el1.isEdge())
            {
                DoSplitEdge(mesh, eVertex, splitLine, overlay);
                ++eVertex;
            }

            assertF(ValidateMeshConnections(mesh));

            long i;
            for(i = 0; i != edgesToSplit.size(); ++i)
            {
                DoSplitEdge(mesh, edgesToSplit[i], splitLine, overlay);
                SplitFace_ClearIndices(mesh, eVertex, edgesToSplit[i].next());
                edgesToBreak.push_back(eVertex.prev().twin());
                overlay.pushBack(eVertex.prev(), splitLine.reversed());
                overlay.pushBack(eVertex.prev().twin(), splitLine);
                eVertex = edgesToSplit[i].twin();
            }

            assertF(ValidateMeshConnections(mesh));

            tEdge eLastVertex;
            if(el3.isEdge())
            {
                eLastVertex = el3.asEdgeInFace(el2.asFace());
                DoSplitEdge(mesh, eLastVertex, splitLine, overlay);
                ++eLastVertex;
            }
            else
            {
                eLastVertex = EdgeInFaceForVertex<tMesh>(el2.asFace(), el3.asVertex());
            }

            SplitFace_ClearIndices(mesh, eVertex, eLastVertex);
            edgesToBreak.push_back(eVertex.prev().twin());
            overlay.pushBack(eVertex.prev(), splitLine.reversed());
            overlay.pushBack(eLastVertex.prev(), splitLine);

            traversal.meshModified(eLastVertex.prev(), eLastVertex.vertex());
            el1 = eLastVertex.vertex();
        }
        while(el1 != ePortal.next().vertex());
    }

//    assertF(ValidateMeshConnections(mesh));

    long i;
    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        mesh.breakEdge(edgesToBreak[i]);
    }

//    assertF(ValidateMeshConnections(mesh));

    tEdge eBefore = ePortal.twin().next().twin();
    tEdge eAfter = ePortal.twin().prev().twin();
    DeleteConnectedFaces(mesh, ePortal.face());

//    assertF(ValidateMeshConnections(mesh));

    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        tEdge e = edgesToBreak[i];
        if(!e.valid())
        {
            continue;
        }
        assertD(CountEdgesAround(e) >= 3);
        while(CountEdgesAround(e) != 3)
        {
            SplitFace_ClearIndices(mesh, e, e.prev().prev());
        }
    }

//    assertF(ValidateMeshConnections(mesh));

    for(i = 0; i != SizeL(edgesToBreak); ++i)
    {
        tEdge e = edgesToBreak[i];
        if(GetEdgeLine(e.prev()).side(GetVertexPoint(e.next())) != SIDE_RIGHT)
        {
            tEdge eNext = e.next();
            *eNext.vertex() = *e.vertex();

            assertF(ValidateMeshConnections(mesh));

            mesh.collapseEdge(e);

            assertF(ValidateMeshConnections(mesh));

            if(CountEdgesAround(eNext) == 2)
            {
                if(eNext.twin().valid())
                {
                    mesh.joinFaces(eNext);
                }
                else
                {
                    mesh.eraseFace(eNext.face());
                }
            }
            edgesToBreak[i].clear();
        }
    }

//    assertF(ValidateMeshConnections(mesh));

//    AddExternalFace(mesh, Before);
    RemoveExternalFaces(mesh); //.... looks like this is not necessary
    AddExternalFaces(mesh);
}

static bool
PortalAffected(
        tEdge e,
        const tPoint* points
        )
{
    if(e.entryIsDead())
    {
        return true;
    }
    if(GetVertexPoint(e) != points[0] || GetVertexPoint(e.next()) != points[1])
    {
        return true;
    }
    if(!e.twin().face()->isExternal())
    {
        return true;
    }
    return false;
}

bool
CutExternalShape(tMesh& mesh,
        tFace& shapeExternalFace,
        const std::vector<tEdge>& edgeBehindPortal,
        const std::vector<tPoint>& edgeBehindPortalPoints
        )
{
    bool portalsAffected = false;
    long i;
    for(i = 0; i != edgeBehindPortal.size(); ++i)
    {
        tEdge e = edgeBehindPortal[i];
        if(e.valid())
        {
            if(PortalAffected(e, &edgeBehindPortalPoints[i * 2]))
            {
                portalsAffected = true;
                break;
            }
        }
    }
    tEdge eShape = tMesh::beginEdge(shapeExternalFace);
    for(i = 0; i != edgeBehindPortal.size(); ++i, --eShape)
    {
        tEdge e = edgeBehindPortal[i];
        if(!e.valid())
        {
            continue;
        }
        if(PortalAffected(e, &edgeBehindPortalPoints[i * 2]))
        {
            continue;
        }
        CutExternalShape(mesh, e, eShape);
    }
    return portalsAffected;
}


