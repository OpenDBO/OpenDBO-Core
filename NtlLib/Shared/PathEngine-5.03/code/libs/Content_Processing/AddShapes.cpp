//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "AddShapes.h"
#include "common/interface/Error.h"
#include "common/AttributesBuilder.h"
#include "i_pathengine.h"

iShape*
ResolveShape(iPathEngine& pathEngine, const iAnchorsAndPinnedShapes& anchorsAndShapes, long i)
{
    long vertices = anchorsAndShapes.pinnedShape_NumberOfVertices(i);
    std::vector<long> coords(vertices * 2);
    long j;
    for(j = 0; j != vertices; ++j)
    {
        anchorsAndShapes.pinnedShape_Vertex(i, j, coords[j * 2], coords[j * 2 + 1]);
    }
    if(pathEngine.shapeIsValid(vertices, &coords.front()))
    {
        return pathEngine.newShape(vertices, &coords.front());
    }
    // so try with coords in reverse order
    for(j = 0; j != vertices; ++j)
    {
        anchorsAndShapes.pinnedShape_Vertex(i, vertices - 1 - j, coords[j * 2], coords[j * 2 + 1]);
    }
    if(pathEngine.shapeIsValid(vertices, &coords.front()))
    {
        return pathEngine.newShape(vertices, &coords.front());
    }
    return 0;
}

bool
PrevalidatePinnedShapes(
        iPathEngine& pathEngine,
        const iAnchorsAndPinnedShapes& anchorsAndShapes
        )
{
    for(long i = 0; i < anchorsAndShapes.numberOfPinnedShapes(); i++)
    {
        iShape* shape = ResolveShape(pathEngine, anchorsAndShapes, i);
        delete shape;
        if(!shape)
        {
            cAttributesBuilder builder;
            builder.add_CopyString("shapeName", anchorsAndShapes.pinnedShape_Name(i));
            Error("NonFatal", "Pinned shape is not valid.", builder.get());
            return false;
        }
    }
    return true;
}

void
AddPinnedShapes(
        iPathEngine& pathEngine,
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        const std::vector<cPosition>& anchorPositions,
        iMesh* mesh
        )
{
    iCollisionContext* context = mesh->newContext();
    for(long i = 0; i < anchorsAndShapes.numberOfPinnedShapes(); i++)
    {
        cPosition p = anchorPositions[anchorsAndShapes.pinnedShape_AnchorIndex(i)];
        if(p.cell == -1)
        {
        // already emitted an error when resolving anchors
            continue;
        }
        iShape* shape = ResolveShape(pathEngine, anchorsAndShapes, i);
        if(!shape)
        {
            cAttributesBuilder builder;
            builder.add_CopyString("shapeName", anchorsAndShapes.pinnedShape_Name(i));
            builder.add("_type", anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type));
            Error("NonFatal", "Pinned shape is not valid. Obstacle will not be placed.", builder.get());
            continue;
        }
        iAgent* agent = mesh->placeAgent(shape, p);
        assertC(PE_PinnedShapeAttribute_Top == 0);
        assertC(PE_PinnedShapeType_Top == 1);
        if(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_BurntIn)
        {
            context->addAgent(agent);
        }
        else
        {
            assertD(anchorsAndShapes.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) == PE_PinnedShapeType_NamedObstacle);
            mesh->storeNamedObstacle(anchorsAndShapes.pinnedShape_Name(i), agent);
            delete agent;
        }
    }

    mesh->burnContextIntoMesh(context);

    while(context->getNumberOfAgents())
    {
        delete context->getAgent(0);
    }
    delete context;
}
