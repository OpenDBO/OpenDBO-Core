//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

// this didnt make any noticeable difference over default setting (in VC 6)
//#pragma inline_depth(100)

#include "platform_common/Header.h"
#include "interface/PathfindPreprocess.h"
#include "ElementInFaceLookup_Header.h"
#include "AStarHeap.h"
#include "SilhouetteRegion.h"
#include "PathAwayTargets.h"
#include "CombinedCornerProvider.h"
#include "VisibilityGraph.h"
#include "ToTargetStateSpace.h"
#include "AwayStateSpace.h"
#include "SmallConvexSplit.h"
#include "CombinedStaticCircuits.h"
#include "PushPathAroundObstacles.h"
#include "BoundedCombinedTester.h"
#include "ElementLookupCollisionTester.h"
#include "interface/CombinedCollisionTesters.h"
#include "interface/CollisionPreprocess.h"
#include "interface/LineCollision.h"
#include "interface/iCollisionTester.h"
#include "interface/QueryContext.h"
#include "interface/AStarHeapPool.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "interface/CornerProvider.h"
#include "interface/CompoundContextCollisionPreprocess.h"
#include "interface/ContextPathfindPreprocess.h"
#include "interface/ManagedSoftObstacleExpansions.h"
#include "interface/EdgeExpansion.h"
#include "libs/Mesh2D/interface/AddInternalVertices.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_Common.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Mesh_Common/interface/Triangulate.h"
#include "libs/Mesh_Common/interface/JoinFacesAtVertex.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "libs/Instrumentation/interface/MemoryReport.h"
#include "platform_common/PointMultiplication.h"
#include "common/STL_Helpers.h"
#include "common/AttributesBuilder.h"
#include "common/Containers/FlagVector.h"
#include "common/interface/Assert.h"
#include "common/interface/Error.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Attributes.h"
#include "common/STL/algorithm.h"

static void 
AddSilhouetteRegion(
          cMeshCut& region, long stateIndex,
          const cMeshWithTrackedTransform<tMesh>& partitioned,
          std::vector<std::vector<long> >& silhouetteLookup
          )
{
#ifdef ASSERTIONS_ON
    region.validateLinks();
    region.validate();
#endif
    assertD(region.isInternalLoop());

    {
        const iMeshTranslator<tMesh>* asTranslator = &partitioned;
        region.remap(cReversedMeshTranslator<tMesh>(asTranslator));
    }

    const cCircuitElement* e = region.getStartElement();
    long faceIndex = 0;
    do
    {
        tFace f = region.iterateOverFacesCrossed(e, faceIndex);

        long faceIndex = f.index();
		std::vector<long>& lookup = silhouetteLookup[faceIndex];

        if(std::find(lookup.begin(), lookup.end(), stateIndex) == lookup.end())
        {
          // not already in lookup
            lookup.push_back(stateIndex);
        }
    }
    while(e);
}

void
cPathfindPreprocess::processAttributes(
        const char *const* attributes,
        const char *const* collisionPreprocessAttributes
        )
{
    _attributesBuilder = new cAttributesBuilder;
    {
        const char* value = GetValueForAttribute(attributes, "split_with_circumference_below");
        _smallConvexWereSplit = false;
        if(value)
        {
            _smallConvexWereSplit = true;
            _smallConvexMaximumCircumference = ValueAsLong(value);
            _attributesBuilder->addCopies("split_with_circumference_below", value);
        }
        else
        {
//            const char *const* cpAttributes = _cp->getAttributes();
//            const char* value = GetValueForAttribute(cpAttributes, "split_with_circumference_below");
            const char* value = GetValueForAttribute(collisionPreprocessAttributes, "split_with_circumference_below");
            if(value)
            {
                _smallConvexWereSplit = true;
                _smallConvexMaximumCircumference = ValueAsLong(value);
                _attributesBuilder->addCopies("split_with_circumference_below", value);
            }
        }
    }
    _attributesBuilder->finalise();
}

void
cPathfindPreprocess::buildSmallConvexSplitAndCombinedCircuits(
        cQueryContext& qc,
        bool keepPartitioned,
        bool justGeneratePartitioned,
        cCombinedStaticCircuits*& combinedCircuits
        )
{
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess small convex split");
        _smallConvexSplit = 0;
        if(_smallConvexWereSplit)
        {
            _smallConvexSplit = new cSmallConvexSplit(
                    qc,
                    _baseObstacleExpansions->refShapeSet(),
                    _edgeExpansion->refLineTester(),
                    _smallConvexMaximumCircumference
                    );
        }
    }
    if(_smallConvexWereSplit)
    {
        combinedCircuits = new cCombinedStaticCircuits(
                qc,
                _edgeExpansion->refEdgeExpansion(),
                _smallConvexSplit->refNonSmallConvex(),
                keepPartitioned,
                justGeneratePartitioned
                );
    }
    else
    {
        combinedCircuits = new cCombinedStaticCircuits(
                qc,
                _edgeExpansion->refEdgeExpansion(),
                _baseObstacleExpansions->refShapeSet(),
                keepPartitioned,
                justGeneratePartitioned
                );
    }
}

void
cPathfindPreprocess::loadSmallConvexSplit(
        cQueryContext& qc,
        iXMLInputStream& is
        )
{
    const char* nextElement = is.peekNextElement();
    _smallConvexSplit = 0;
    if(strcmp(nextElement, "smallConvexSplit") == 0)
    {
        assertD(_smallConvexWereSplit);
        is.openNextElement();
//        _smallConvexSplit = new cSmallConvexSplit(is, qc, _baseObstacleExpansions->refShapeSet());
        _smallConvexSplit = new cSmallConvexSplit(is, qc, _baseObstacleExpansions->refShapeSet());
        is.closeElement();
    }
    else
    {
        assertD(!_smallConvexWereSplit);
    }
}

void
cPathfindPreprocess::setupCollisionTesters(cQueryContext& qc)
{
    _baseLineTester_NonSmallConvex = new cCombinedLineCollisionTester;
    if(_smallConvexWereSplit)
    {
//        _baseLineTester_NonSmallConvex->setTargets(&&_edgeExpansion->refLineTester(), &_smallConvexSplit->refNonSmallConvex());
        _baseLineTester_NonSmallConvex->setTargets(&_edgeExpansion->refLineTester(), &_smallConvexSplit->refNonSmallConvex());
    }
    else
    {
//        _baseLineTester_NonSmallConvex->setTargets(&&_edgeExpansion->refLineTester(), &_baseObstacleExpansions->refShapeSet());
        _baseLineTester_NonSmallConvex->setTargets(&_edgeExpansion->refLineTester(), &_baseObstacleExpansions->refShapeSet());
    }
    if(_smallConvexWereSplit)
    {
        _basePointTester_NonSmallConvex = new cCombinedPointCollisionTester(_edgeExpansion->refPointTester(), _smallConvexSplit->refNonSmallConvex());
    }
    else
    {
        _basePointTester_NonSmallConvex = new cCombinedPointCollisionTester(_edgeExpansion->refPointTester(), _baseObstacleExpansions->refShapeSet());
    }
}

void
cPathfindPreprocess::generateSilhouetteRegions(
                        cQueryContext& qc,
                        bStateSpace<cCornerProvider>& fixedStates,
                        const cCombinedStaticCircuits& combinedCircuits,
                        bool keepPartitioned,
                        bool justGeneratePartitioned
                        )
{
    const tMeshCircuits& circuits = *combinedCircuits.getCircuits_NotThreadSafe();

    tMesh& mesh = qc.refMesh();

    _partitioned = new cMeshWithTrackedTransform<tMesh>(&mesh);
    if(qc.meshHasInternalVertices())
    {
        std::vector<tVertex> toRemove;
        tVertex v = _partitioned->beginVertex();
        do
        {
            if(VertexIsExternal(tMesh::firstEdge(v)))
            {
                continue;
            }
            tPoint p = GetVertexPoint(v);
            tFace f = TranslateToOwned_SI<tMesh>(v, p);
            f = mesh.faceAt(f.index());
            if(refPointTester_NonSmallConvex().testPointCollision(qc, f, p))
            {
                continue;
            }
            toRemove.push_back(v);
        }
        while((++v).valid());
    
        long i;
        for(i = 0; i != SizeL(toRemove); ++i)
        {
            JoinFacesAtVertex_KeepConvex(*_partitioned, toRemove[i]);
        }
    }
    else
    {
        AddInternalVerticesInsideObstructions(*_partitioned, circuits, 30);
    }
    _partitioned->consolidate();

    if(justGeneratePartitioned)
    {
        return;
    }

    cMeshWithTrackedTransform<tMesh>& partitioned = *_partitioned;

    tMeshCircuits partitionedCircuits;
    CopyCircuits(circuits, partitionedCircuits);
    RemapCircuits(partitionedCircuits, partitioned);

    IndexElements(partitionedCircuits, 0);

    std::vector<std::vector<cElementInFaceEntry> >* elementLookup =
        ConstructAndBuildElementsInFaceLookup(partitionedCircuits, partitioned.faces());

    std::vector<std::vector<cCornerInFaceEntry> > cornerLookup;
    cornerLookup.resize(partitioned.faces());

    std::vector<const cCircuitElement*> cornerOnPartitioned(combinedCircuits.numberOfCornerElements());
    {
    // the element corners need remaps
        long i;
        for(i = 0; i < _staticCorners->size(); i++)
        {
            long originalI = i;
            if(originalI >= combinedCircuits.numberOfCornerElements())
            {
                continue;
            }
            const cCircuitElement& el = combinedCircuits.refCornerElement(originalI);
            tFace f = el.getFaceCrossed(0);
            tFace fOnPartitioned = TranslateToPartitioned(partitioned, f, el.cornerPoint());
            long fIndex = fOnPartitioned.index();
            cornerOnPartitioned[originalI] = FindMatchingCorner((*elementLookup)[fIndex], el);
            cornerLookup[fIndex].push_back(cCornerInFaceEntry(cornerOnPartitioned[originalI], i));
        }
    }

	assertD(_silhouetteLookup.empty());
    _silhouetteLookup.resize(mesh.faces());

    cFlagVector faceReferencedBuffers(partitioned.faces());

    {
//        cFlagVector& elementFlags = qc.lockElementFlagVector();
        cFlagVector elementFlags(qc.getMaxElementCount());

        long state;
        for(state = 0; state < fixedStates.states(); state++)
        {
            long point = fixedStates.pointForState(state);
            long index;
            bStateSpace<cCornerProvider>::ePointType pointType =
                fixedStates.getPointTypeAndSubIndex(point, index);
            if(pointType == bStateSpace<cCornerProvider>::CORNER)
            {
                eSilhouetteZone zone = fixedStates.decodeApproachZone(state);
                cMeshCut region;
                long originalIndex = index;
                if(originalIndex < _numberOfElementCorners)
                {
                  // base circuits 'hard' corner
                    const cCircuitElement& cornerElement = *cornerOnPartitioned[originalIndex];
                    nSilhouetteRegion::build(
                                    *elementLookup,
                                    elementFlags,
                                    cornerLookup,
                                    cornerElement, zone,
                                    faceReferencedBuffers,
                                    region);
                }
                else
                {
                  // soft obstacle corner

                    cInternalPosition ip = fixedStates.positionForState(state);
                    cInternalPosition ipOnPartitioned = TranslateToPartitioned(partitioned, ip);
                    tPoint axisBefore(_staticCorners->axisBefore(index));
                    tPoint axisAfter(_staticCorners->axisAfter(index));

                    nSilhouetteRegion::build_Unanchored(
                                    *elementLookup,
                                    elementFlags,
                                    cornerLookup,
                                    ipOnPartitioned, axisBefore, axisAfter, zone,
                                    faceReferencedBuffers,
                                    region);
                }
                AddSilhouetteRegion(region, state, partitioned, _silhouetteLookup);
            }
            else
            if(pointType == bStateSpace<cCornerProvider>::END_POINT)
            {
                bool isDeparting = fixedStates.decodeIsDeparting(state);
                if(isDeparting && !_endPointObstructed[index])
                {
                    cInternalPosition endPointOnOriginal = _offMeshConnections.refEndPoints()[index];
                    cInternalPosition endPointOnPartitioned = TranslateToPartitioned(partitioned, _offMeshConnections.refEndPoints()[index]);
                    cMeshCut region;
                    nSilhouetteRegion::build_360(
                            *elementLookup,
                            elementFlags,
                            cornerLookup,
                            endPointOnPartitioned,
                            faceReferencedBuffers,
                            region
                            );
                    AddSilhouetteRegion(region, state, partitioned, _silhouetteLookup);
                }
            }
            else
            {
                assertD(pointType == bStateSpace<cCornerProvider>::START || pointType == bStateSpace<cCornerProvider>::GOAL);
            }
        }

//        qc.releaseFlagVector();
    }

    if(!keepPartitioned)
    {
        delete _partitioned;
        _partitioned = 0;
    }

    DestroyElementsInFaceLookup(elementLookup);
}

void
cPathfindPreprocess::createCostCalculator()
{
    _staticCostCalculator.setTarget1(&_softObstacleExpansions->refCostCalculator());
    _staticCostCalculator.setTarget2Vector(&_staticMesh3DCostCalculators);
}

void
cPathfindPreprocess::createStaticCorners(
        cQueryContext& qc,
        const cCornerProvider& mesh3DCorners,
        const cCombinedStaticCircuits& combinedCircuits
        )
{
    long i;
    _staticCorners = new cCornerProvider(&qc.refMesh());
    for(i = 0; i < combinedCircuits.numberOfCornerElements(); ++i)
    {
        const cCircuitElement& el = combinedCircuits.refCornerElement(i);
        _staticCorners->push_back(
                    el.getFaceCrossed(0),
                    el.cornerPoint(), el.getPrev()->refLine().axis(), el.refLine().axis());
    }
    _numberOfElementCorners = _staticCorners->size();
    _softObstacleExpansions->addCornersTo(qc, *_staticCorners);

    _staticCorners->add(mesh3DCorners);
    _staticCorners->shrinkToFit();
}

void
cPathfindPreprocess::saveElementCorners(iXMLOutputStream& os) const
{
    _staticCorners->save(_numberOfElementCorners, os);
}

void
cPathfindPreprocess::createStaticCornersFromPersistence(
        cQueryContext& qc,
        iXMLInputStream& is,
        const cCornerProvider& mesh3DCorners
        )
{
    _staticCorners = new cCornerProvider(&qc.refMesh());
    is.openElement("elementCorners");
    _staticCorners->load(is);
    is.closeElement();
    _numberOfElementCorners = _staticCorners->size();
    _softObstacleExpansions->addCornersTo(qc, *_staticCorners);
    _staticCorners->add(mesh3DCorners);
    _staticCorners->shrinkToFit();
}

void
cPathfindPreprocess::buildEndPointObstructed(cQueryContext& qc)
{
    assertD(SizeL(_endPointObstructed) == _offMeshConnections.numberOfEndPoints());
    long i;
    for(i = 0; i != _offMeshConnections.numberOfEndPoints(); ++i)
    {
        cInternalPosition endPointOnOriginal = _offMeshConnections.refEndPoints()[i];
        _endPointObstructed.setBit(
                i,
//                _cp->refPointTester_All().testPointCollision(qc, endPointOnOriginal.f, endPointOnOriginal.p)
                _edgeExpansion->refPointTester().testPointCollision(qc, endPointOnOriginal.f, endPointOnOriginal.p)
                 || _baseObstacleExpansions->refShapeSet().testPointCollision(qc, endPointOnOriginal.f, endPointOnOriginal.p)
                );
    }
}

cPathfindPreprocess::cPathfindPreprocess(
        cQueryContext& qc,
//        cHandle<cCollisionPreprocess> cp,
        cHandle<cEdgeExpansion> edgeExpansion,
        cHandle<cManagedShapeSet> baseObstacleExpansions,
        const char *const* collisionPreprocessAttributes,
        const cOffMeshConnections& offMeshConnections,
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions,
        const cCornerProvider& mesh3DCorners,
        const std::vector<iMovementCostCalculator*>& mesh3DCostCalculators,
        const char *const* attributes,
        bool keepPartitioned,
        bool justGeneratePartitioned
        ) :
// _cp(cp),
 _edgeExpansion(edgeExpansion),
 _baseObstacleExpansions(baseObstacleExpansions),
 _staticMesh3DCostCalculators(mesh3DCostCalculators),
 _offMeshConnections(offMeshConnections),
 _endPointObstructed(offMeshConnections.numberOfEndPoints())
{
    _softObstacleExpansions = softObstacleExpansions;

    buildEndPointObstructed(qc);
    _keptCombinedCircuits = 0;

    processAttributes(attributes, collisionPreprocessAttributes);
    cCombinedStaticCircuits* combinedCircuits;
    bool keepCircuitCombinationPartitioned = (GetValueForAttribute(attributes, "keep_circuit_combination_partitioned") != 0);
    bool justGenerateCircuitCombinationPartitioned = (GetValueForAttribute(attributes, "just_generate_circuit_combination_partitioned") != 0);
    buildSmallConvexSplitAndCombinedCircuits(qc, keepCircuitCombinationPartitioned, justGenerateCircuitCombinationPartitioned, combinedCircuits);
    if(keepCircuitCombinationPartitioned)
    {
        _keptCombinedCircuits = combinedCircuits;
    }
    if(justGenerateCircuitCombinationPartitioned)
    {
        if(!keepCircuitCombinationPartitioned)
        {
            delete combinedCircuits;
        }
        _graph = 0;
        return;
    }
    setupCollisionTesters(qc);

    qc.addElementCountRequirement(combinedCircuits->elementCount());

    createCostCalculator();
    createStaticCorners(qc, mesh3DCorners, *combinedCircuits);

    cFixedStates<
            cCornerProvider,
            cCombinedLineCollisionTester,
            tCombinedCostCalculator
            > fixedStates(qc, _offMeshConnections, *_staticCorners, refLineTester_NonSmallConvex(), &_staticCostCalculator);

    generateSilhouetteRegions(qc, fixedStates, *combinedCircuits, keepPartitioned, justGeneratePartitioned);

    if(!keepCircuitCombinationPartitioned)
    {
        delete combinedCircuits;
    }

    if(justGeneratePartitioned)
    {
        _graph = 0;
        return;
    }
    _graph = new cVisibilityGraph(qc, fixedStates, _silhouetteLookup, _offMeshConnections, _endPointObstructed);

    ShrinkToFit_Compound(_silhouetteLookup);

#ifdef ASSERTIONS_ON
    // check that each connection in the graph has a reverse connection
    long sourceState;
    for(sourceState = 0; sourceState < _graph->size(); sourceState++)
    {
        if(!fixedStates.stateIsCorner(sourceState) && fixedStates.decodeIsDeparting(sourceState))
        {
          // off mesh connections do not have to go both ways
            continue;
        }
        long sourceReversed = fixedStates.reverseState(sourceState);
        long j;
        for(j = 0; j < _graph->connectionsFrom(sourceState); j++)
        {
            long targetState = _graph->connectionFrom(sourceState, j);
            long targetReversed = fixedStates.reverseState(targetState);
            bool found = false;
            long k;
            for(k = 0; k < _graph->connectionsFrom(targetReversed); k++)
            {
                if(_graph->connectionFrom(targetReversed, k) == sourceReversed)
                {
                    assertD(!found);
                    found = true;
                }
            }
            assertD(found);
        }
    }
#endif
}

cPathfindPreprocess::cPathfindPreprocess(
        cQueryContext& qc,
//        cHandle<cCollisionPreprocess> cp,
        cHandle<cEdgeExpansion> edgeExpansion,
        cHandle<cManagedShapeSet> baseObstacleExpansions,
        const char *const* collisionPreprocessAttributes,
        const cOffMeshConnections& offMeshConnections,
        cHandle<cManagedSoftObstacleExpansions> softObstacleExpansions,
        const cCornerProvider& mesh3DCorners,
        const std::vector<iMovementCostCalculator*>& mesh3DCostCalculators,
        iXMLInputStream& is,
        const char *const* attributes        
        ) :
 _edgeExpansion(edgeExpansion),
 _baseObstacleExpansions(baseObstacleExpansions),
 _staticMesh3DCostCalculators(mesh3DCostCalculators),
 _offMeshConnections(offMeshConnections),
 _endPointObstructed(offMeshConnections.numberOfEndPoints())
{
    _softObstacleExpansions = softObstacleExpansions;

    buildEndPointObstructed(qc);
    _keptCombinedCircuits = 0;

    processAttributes(attributes, collisionPreprocessAttributes);
    loadSmallConvexSplit(qc, is);
    setupCollisionTesters(qc);

    createCostCalculator();
    createStaticCornersFromPersistence(qc, is, mesh3DCorners);

    _partitioned = 0;
    const char *const* graphAttributes = is.openElement("graph");
    _graph = new cVisibilityGraph(is, graphAttributes);
    is.closeElement();
    const char *const* silhouetteLookupAttributes = is.openElement("silhouetteLookup");
    _silhouetteLookup.resize(ValueAsLong(silhouetteLookupAttributes, "size"));
    long i;
    for(i = 0; i < SizeL(_silhouetteLookup); ++i)
    {
        const char *const* attributes = is.openElement("face");
        _silhouetteLookup[i].resize(ValueAsLong(attributes, "size"));
        long j;
        for(j = 0; j < SizeL(_silhouetteLookup[i]); ++j)
        {
            const char *const* attributes = is.openElement("regionTarget");
            _silhouetteLookup[i][j] = ValueAsLong(attributes, "state");
            is.closeElement();
        }
        is.closeElement();
    }
    is.closeElement();
}

cPathfindPreprocess::~cPathfindPreprocess()
{
    MEMORY_REPORT_SCOPE("cPathfindPreprocess - destructor");
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - attributes builder");
        delete _attributesBuilder;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - small convex split");
        delete _smallConvexSplit;
    }
    delete _keptCombinedCircuits;
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - base collision testers");
        delete _baseLineTester_NonSmallConvex;
        delete _basePointTester_NonSmallConvex;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - visibility graph");
        delete _graph;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - partition");
        delete _partitioned;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - static corners");
        delete _staticCorners;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - static cost calculators");
        long i;
        for(i = 0; i < SizeL(_staticMesh3DCostCalculators); ++i)
        {
            delete _staticMesh3DCostCalculators[i];
        }
    }
#ifdef MEMORY_REPORTS_ON
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - edge expansion");
        _edgeExpansion = 0;
    }
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - base obstacle expansions");
        _baseObstacleExpansions = 0;
    }
    _softObstacleExpansions = 0;
    _endPointObstructed.completelyClear();
    {
        MEMORY_REPORT_SCOPE("cPathfindPreprocess - silhouette lookup");
        CompletelyClear(_silhouetteLookup);
    }
#endif
}

void
cPathfindPreprocess::save(iXMLOutputStream& os) const
{
    if(_smallConvexSplit)
    {
        os.openElement("smallConvexSplit");
        _smallConvexSplit->save(os);
        os.closeElement("smallConvexSplit");
    }
//    os.openElement("combinedStaticCircuits");
//    _combinedCircuits->save(os);
//    os.closeElement("combinedStaticCircuits");

    os.openElement("elementCorners");
    saveElementCorners(os);
    os.closeElement("elementCorners");

    os.openElement("graph");
    _graph->save(os);
    os.closeElement("graph");
    os.openElement("silhouetteLookup");
    os.addAttribute("size", SizeL(_silhouetteLookup));
    long i;
    for(i = 0; i < SizeL(_silhouetteLookup); ++i)
    {
        os.openElement("face");
        os.addAttribute("size", SizeL(_silhouetteLookup[i]));
        long j;
        for(j = 0; j < SizeL(_silhouetteLookup[i]); ++j)
        {
            os.openElement("regionTarget");
            os.addAttribute("state", _silhouetteLookup[i][j]);
            os.closeElement("regionTarget");
        }
        os.closeElement("face");
    }
    os.closeElement("silhouetteLookup");
}

const char *const*
cPathfindPreprocess::getAttributes() const
{
    return _attributesBuilder->get();
}

bool
cPathfindPreprocess::testCollision_SmallConvex(cQueryContext& qc, const cInternalPosition& from, const cInternalPosition& to) const
{
    if(from == to)
    {
        return false;
    }
    if(!_smallConvexWereSplit)
    {
        return false;
    }
    PROFILER_SCOPE("cPathfindPreprocess::testCollision_SmallConvex");
    tFace startF = _smallConvexSplit->translateToPartitioned(from.f, from.p);
    tFace endF = _smallConvexSplit->translateToPartitioned(to.f, to.p);
    return _smallConvexSplit->refSmallConvexOnPartitioned().testCollision(qc, cInternalPosition(startF, from.p), cInternalPosition(endF, to.p));
}
bool
cPathfindPreprocess::testPointCollision_SmallConvex(cQueryContext& qc, tFace face, const tPoint& point) const
{
    if(!_smallConvexWereSplit)
    {
        return false;
    }
    tFace faceOnPartitioned = _smallConvexSplit->translateToPartitioned(face, point);
    return PointCollision_AgainstShapeSet(_smallConvexSplit->refSmallConvexOnPartitioned(), cInternalPosition(faceOnPartitioned, point));
}

const cCombinedLineCollisionTester&
cPathfindPreprocess::refLineTester_NonSmallConvex() const
{
    return *_baseLineTester_NonSmallConvex;
}
const cCombinedPointCollisionTester&
cPathfindPreprocess::refPointTester_NonSmallConvex() const
{
    return *_basePointTester_NonSmallConvex;
}

void
cPathfindPreprocess::pushPathAroundSmallConvex(
        const std::vector<cInternalPosition>& path,
        const std::vector<long>* connectionIndices,
        std::vector<cInternalPosition>& result,
        std::vector<long>* connectionIndicesResult
        ) const
{
    assertD(_smallConvexWereSplit);
    nPushPathAroundObstacles::pushPathAroundSmallConvex(
        _edgeExpansion->refEdgeExpansionLookup(),
        _smallConvexSplit->refNonSmallConvex(),
        _smallConvexSplit->getPartitioned(),
        _smallConvexSplit->refSmallConvexOnPartitioned(),
        0,
        path,
        connectionIndices,
        result,
        connectionIndicesResult
        );
}

void
cPathfindPreprocess::pushPathAroundSmallConvex(
        const std::vector<cInternalPosition>& path,
        const std::vector<long>* connectionIndices,
        std::vector<cInternalPosition>& result,
        std::vector<long>* connectionIndicesResult,
        const cLocalisedConvexShapeSet& dynamicExpansions
        ) const
{
    assertD(_smallConvexWereSplit);
    nPushPathAroundObstacles::pushPathAroundSmallConvex(
        _edgeExpansion->refEdgeExpansionLookup(),
        _smallConvexSplit->refNonSmallConvex(),
        _smallConvexSplit->getPartitioned(),
        _smallConvexSplit->refSmallConvexOnPartitioned(),
        &dynamicExpansions,
        path,
        connectionIndices,
        result,
        connectionIndicesResult
        );
}

void
cPathfindPreprocess::getSmallConvexPotentiallyOverlapped(const cLocalisedConvexShape& shape, std::vector<long>& appendTo) const
{
    if(!_smallConvexWereSplit)
    {
        return;
    }
    GetShapesPotentiallyOverlappedBy(_smallConvexSplit->refSmallConvexOnOriginal(), shape, appendTo);
}


const cLocalisedConvexShapeSet*
cPathfindPreprocess::getSmallConvexOnOriginal() const
{
    return &_smallConvexSplit->refSmallConvexOnOriginal();
}
const cLocalisedConvexShapeSet*
cPathfindPreprocess::getNonSmallConvex() const
{
    return &_smallConvexSplit->refNonSmallConvex();
}
const cMeshWithTrackedTransform<tMesh>*
cPathfindPreprocess::getSmallConvexPartitioned() const
{
    return _smallConvexSplit->getPartitioned();
}
const cMeshWithTrackedTransform<tMesh>&
cPathfindPreprocess::refCircuitCombinationPartitioned()
{
    assertD(_keptCombinedCircuits);
    return _keptCombinedCircuits->refCircuitCombinationPartitioned();
}
bool
cPathfindPreprocess::smallConvexWereSplit() const
{
    return _smallConvexWereSplit;
}

void
cPathfindPreprocess::lookupPositionForBaseCorner(cQueryContext& qc, long stateIndex, cInternalPosition& result) const
{
    bStateSpace<cCornerProvider> fixedStates(_offMeshConnections, *_staticCorners);
    result = fixedStates.positionForState(stateIndex);
}

static void
FilterZeroLengthPathSegments(
        std::vector<cInternalPosition>& result,
        std::vector<long>*& connectionIndices
        )
{
    if(!connectionIndices)
    {
      // note that this early out means that zero length connection from start to goal would not be filtered
        return;
    }
    std::vector<long>& indices = *connectionIndices;
    long i;
    for(i = 0; i + 1 < SizeL(result); ++i)
    {
        if(result[i] == result[i + 1] && indices[i] == -1)
        {
            break;
        }
    }
    if(i + 1 == result.size())
    {
      // no segments need to be filtered
        return;
    }
    long copyTo = i;
    // skip first filtered segment
    ++i;
    // scan for any further segments to be skipped
    for(; i + 1 < SizeL(result); ++i)
    {
        if(result[i] == result[i + 1] && indices[i] == -1)
        {
          // filter this segment
        }
        else
        {
            result[copyTo] = result[i];
            indices[copyTo] = indices[i];
            copyTo++;
        }
    }
    // end point of path
    result[copyTo] = result[i];
    indices.resize(copyTo);
    ++copyTo;
    result.resize(copyTo);
}

template <class STATESPACE, class ASTAR> void
PathFromAStar(
        STATESPACE& stateSpace, ASTAR& aStar, long bestPath,
        std::vector<cInternalPosition>& result,
        std::vector<long>*& connectionIndices
        )
{
    long points = aStar.getPathPointsTo(bestPath);
    result.resize(points);
    long i = points - 1;
    long endState = -1;
    long startState = bestPath;
    while(startState != -1)
    {
        result[i] = stateSpace.positionForState(startState);
        long startPoint = stateSpace.pointForState(startState);
        if(endState != -1)
        {
            long connectionIndex = stateSpace.getConnectionIndex(startState, endState);
            if(connectionIndex != -1)
            {
                if(!connectionIndices)
                {
                    connectionIndices = new std::vector<long>(points - 1, -1);
                }
                (*connectionIndices)[i] = connectionIndex;
            }
        }
        endState = startState;
        i--;
        startState = aStar.getParentPoint(startState);
    }
    FilterZeroLengthPathSegments(result, connectionIndices);
}

void
cPathfindPreprocess::findShortestPath(cQueryContext& qc, const cInternalPosition& start, const cInternalPosition& end,
                std::vector<cInternalPosition>& result,
                std::vector<long>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    assertD(connectionIndices == 0);
    if(start == end)
    {
        result.push_back(start);
        return;
    }
    PROFILER_SCOPE("cPathfindPreprocess::findShortestPath no obstacles, vector");

    cNullLineCollisionTester nullLineTester;
    cNullPointCollisionTester nullPointTester;

    cToTargetStateSpace<
        cCornerProvider,
        iLineCollisionTester,
        tCombinedCostCalculator,
        cNullLineCollisionTester,
        cNullPointCollisionTester,
        cShapesCostCalculator
        > stateSpace(
                qc,
                _offMeshConnections,
                *_staticCorners,
                refLineTester_NonSmallConvex(),
                &_staticCostCalculator,
                nullLineTester,
                nullPointTester,
                0,
                *_graph, _silhouetteLookup,
                queryCallBack
                );
    stateSpace.setStart(start);
    stateSpace.setGoal(end);

    cAStarLock lock(qc._heapPool);
    long bestPath = lock._heap.search(&stateSpace, stateSpace.startState());
    PathFromAStar(stateSpace, lock._heap, bestPath, result, connectionIndices);
}

void
cPathfindPreprocess::findShortestPath(
                cQueryContext& qc, 
                const cInternalPosition& start,
                const cInternalPosition& end,
                const cCompoundContextCollisionPreprocess& cccp,
                const cContextPathfindPreprocess& cpp,
                std::vector<cInternalPosition>& result,
                std::vector<long>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    if(start == end)
    {
        result.push_back(start);
        return;
    }
    if(cpp.boundsValid() && !start.p.insideOrOnBounds(cpp.boundsMin(), cpp.boundsMax()))
    {
        result.push_back(start);
        return;
    }
    PROFILER_SCOPE("cPathfindPreprocess::findShortestPath obstacles, vector");

    cCombinedCornerProvider<cCornerProvider, cCornerProvider>
        combinedCorners(*_staticCorners, cpp.refCornerProvider());

    cBoundedCombinedTester dynamicLineTester(
            &cccp,
            &cpp.refSmallConvexOverlapped(),
            cpp.boundsMin(), cpp.boundsMax(), cpp.boundsValid()
            );

    cCombinedPointCollisionTester combinedPointTester(cccp, cpp.refSmallConvexOverlapped());

    cToTargetStateSpace<
        cCombinedCornerProvider<cCornerProvider, cCornerProvider>,
        iLineCollisionTester,
        tCombinedCostCalculator,
        cBoundedCombinedTester,
        cCombinedPointCollisionTester,
        tCombinedCostCalculator
        >
        stateSpace(
            qc,
            _offMeshConnections,
            combinedCorners,
            refLineTester_NonSmallConvex(),
            &_staticCostCalculator,
            dynamicLineTester,
            combinedPointTester,
            cpp.getCostCalculator(),
            *_graph, _silhouetteLookup,
            queryCallBack
            );
    stateSpace.setStart(start);
    stateSpace.setGoal(end);

    cAStarLock lock(qc._heapPool);
    long bestPath = lock._heap.search(&stateSpace, stateSpace.startState());
    PathFromAStar(stateSpace, lock._heap, bestPath, result, connectionIndices);
}

template <class STATESPACE, class ASTAR> void
PathFromAStar_AwayStateSpace(
        STATESPACE& stateSpace, ASTAR& aStar, long bestPath,
        std::vector<cInternalPosition>& result,
        std::vector<long>*& connectionIndices
        )
{
    long points = aStar.getPathPointsTo(bestPath);
    result.resize(points);
    if(bestPath == -1)
    {
        return;
    }
    long i = points - 1;
    long endPoint = bestPath;
    long startPoint = aStar.getParentPoint(endPoint);
    assertD(startPoint != -1);
    long connectionIndex = stateSpace.getConnectionIndex(startPoint, endPoint);
    if(connectionIndex != -1)
    {
        assertD(!connectionIndices);
        connectionIndices = new std::vector<long>(points - 1, -1);
        (*connectionIndices)[i - 1] = stateSpace.getConnectionIndex(startPoint, endPoint);
        cInternalPosition p = stateSpace.positionForState(endPoint);
        result[i] = stateSpace.positionForState(endPoint);
    }
    else
    {
        tPoint endP = stateSpace.getGoalPoint(endPoint, startPoint);
        cInternalPosition startIP = stateSpace.positionForState(startPoint);
        tLine l(startIP.p, endP);
        tFace endF = FaceAtEndOfLine<tMesh>(startIP.f, l);
        result[i] = cInternalPosition(endF, endP);
    }
    --i;
    endPoint = startPoint;
    do
    {
        startPoint = aStar.getParentPoint(endPoint);
        if(startPoint != -1)
        {
            long connectionIndex = stateSpace.getConnectionIndex(startPoint, endPoint);
            if(connectionIndex != -1)
            {
                if(!connectionIndices)
                {
                    connectionIndices = new std::vector<long>(points - 1, -1);
                }
                (*connectionIndices)[i - 1] = stateSpace.getConnectionIndex(startPoint, endPoint);
            }
        }
        result[i] = stateSpace.positionForState(endPoint);
        --i;
        endPoint = startPoint;
    }
    while(endPoint != -1);
    FilterZeroLengthPathSegments(result, connectionIndices);
}

void
cPathfindPreprocess::findPathAway(
                cQueryContext& qc, 
                const cInternalPosition& start,
                const cInternalPosition& awayFrom,
                long radius,
                std::vector<cInternalPosition>& result,
                std::vector<long>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    {
        tPoint p = start.p;
        if( p.getX() <= awayFrom.p.getX() - radius
            ||
            p.getX() >= awayFrom.p.getX() + radius
            ||
            p.getY() <= awayFrom.p.getY() - radius
            ||
            p.getY() >= awayFrom.p.getY() + radius)
        {
            result.push_back(start);
            return;
        }
    }

    cPathAwayTargets pathAwayTargets(
            qc, awayFrom, radius,
            _edgeExpansion->refEdgeExpansionLookup(),
            _baseObstacleExpansions->refShapeSet()
            );
    
    if(!pathAwayTargets.positionIsInside(start))
    {
        result.push_back(start);
        return;
    }

    cNullLineCollisionTester nullLineTester;
    cNullPointCollisionTester nullPointTester;

    cAwayStateSpace<
        cCornerProvider, cPathAwayTargets,
        iLineCollisionTester,
        tCombinedCostCalculator,
        cNullLineCollisionTester,
        cNullPointCollisionTester,
        cShapesCostCalculator
        >
        stateSpace( 
            qc,
            _offMeshConnections,
            *_staticCorners, pathAwayTargets,
            refLineTester_NonSmallConvex(),
            &_staticCostCalculator,
            nullLineTester,
            nullPointTester,
            0,
            *_graph, _silhouetteLookup,
            queryCallBack
            );

    stateSpace.setStart(start);
    cAStarLock lock(qc._heapPool);
    cAStarHeap& aStar = lock._heap;
    long bestPath = aStar.search(&stateSpace, stateSpace.startState());
    PathFromAStar_AwayStateSpace(stateSpace, lock._heap, bestPath, result, connectionIndices);
}


void
cPathfindPreprocess::findPathAway(
                cQueryContext& qc, 
                const cInternalPosition& start,
                const cInternalPosition& awayFrom,
                long radius,
                const cCompoundContextCollisionPreprocess& cccp,
                const cContextPathfindPreprocess& cpp,
                std::vector<cInternalPosition>& result,
                std::vector<long>*& connectionIndices,
                iQueryCallBack* queryCallBack
                ) const
{
    assertD(result.empty());
    {
        tPoint p = start.p;
        if( p.getX() <= awayFrom.p.getX() - radius
            ||
            p.getX() >= awayFrom.p.getX() + radius
            ||
            p.getY() <= awayFrom.p.getY() - radius
            ||
            p.getY() >= awayFrom.p.getY() + radius)
        {
            result.push_back(start);
            return;
        }
    }
    if(cpp.boundsValid() && !start.p.insideOrOnBounds(cpp.boundsMin(), cpp.boundsMax()))
    {
        result.push_back(start);
        return;
    }

    cPathAwayTargets pathAwayTargets(
            qc, awayFrom, radius,
            _edgeExpansion->refEdgeExpansionLookup(),
            _baseObstacleExpansions->refShapeSet()
            );
    
    if(!pathAwayTargets.positionIsInside(start))
    {
        result.push_back(start);
        return;
    }

    cCombinedCornerProvider<cCornerProvider, cCornerProvider>
        combinedCorners(*_staticCorners, cpp.refCornerProvider());

    cBoundedCombinedTester dynamicLineTester(
            &cccp,
            &cpp.refSmallConvexOverlapped(),
            cpp.boundsMin(), cpp.boundsMax(), cpp.boundsValid()
            );

    cCombinedPointCollisionTester combinedPointTester(cccp, cpp.refSmallConvexOverlapped());

    cAwayStateSpace<
        cCombinedCornerProvider<cCornerProvider, cCornerProvider>, cPathAwayTargets,
        iLineCollisionTester,
        tCombinedCostCalculator,
        cBoundedCombinedTester,
        cCombinedPointCollisionTester,
        tCombinedCostCalculator
        > stateSpace( 
            qc,
            _offMeshConnections,
            combinedCorners, pathAwayTargets,
            refLineTester_NonSmallConvex(),
            &_staticCostCalculator,
            dynamicLineTester,
            combinedPointTester,
            cpp.getCostCalculator(),
            *_graph, _silhouetteLookup,
            queryCallBack
            );

    stateSpace.setStart(start);
    cAStarLock lock(qc._heapPool);
    cAStarHeap& aStar = lock._heap;
    long bestPath = aStar.search(&stateSpace, stateSpace.startState());
    PathFromAStar_AwayStateSpace(stateSpace, lock._heap, bestPath, result, connectionIndices);
}

long
cPathfindPreprocess::majorVersion()
{
    return 3;
}
long
cPathfindPreprocess::minorVersion()
{
    return 0;
}
