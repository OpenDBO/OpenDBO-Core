#include "precomp_ntlpresentation.h"
#include "NtlWorldSectorPVS.h"
#include "NtlDebug.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "ntlworldinterface.h"
#include "NtlWorldBrush.h"
#include "NtlWorldBrushLine.h"
#include "NtlPLRenderState.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"

// Static members
RwImage* CNtlWorldSectorPVS::m_pPVSAreaFilter = NULL;
RwInt32 CNtlWorldSectorPVS::m_LockedPVSSector = -999;

// Constants for readability and maintainability
static const RwReal PVS_LAYER_HEIGHT_INCREMENT = 10.0f;
static const RwInt32 PVS_SECTOR_TEST_RANGE = 2; // Reduced range for occluder testing
static const RwReal PVS_CLOSEST_NEIGHBOR_THRESHOLD = 1.41421356f; // sqrt(2) for sector distance

CNtlWorldSectorPVS::CNtlWorldSectorPVS(void)
{
    m_Visibility = TRUE;
    m_pVisibilityNeighbor = NULL;
    m_pPVSAreaFilter = NULL;
}

CNtlWorldSectorPVS::~CNtlWorldSectorPVS(void)
{
    if (m_pPVSAreaFilter)
    {
        RwImageDestroy(m_pPVSAreaFilter);
        m_pPVSAreaFilter = NULL;
    }
    NTL_DELETE(m_pVisibilityNeighbor);
}

VOID CNtlWorldSectorPVS::SetVisibilityNeighbor(BYTE* _VisibilityNeighbor)
{
    NTL_DELETE(m_pVisibilityNeighbor);

    m_pVisibilityNeighbor = NTL_NEW BYTE[dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT];
    if (!m_pVisibilityNeighbor)
    {
        return;
    }

    CopyMemory(m_pVisibilityNeighbor, _VisibilityNeighbor, sizeof(BYTE) * dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT);
}

VOID CNtlWorldSectorPVS::SetVisibility(RwInt32 _SectorIdx, RwBool _Visible)
{
    if (_SectorIdx < 0)
    {
        return;
    }

    CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
   

    RwBool bCulled = !_Visible;

    // Doodads (ornaments)
    RwInt32 NumDoodads = static_cast<RwInt32>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityOrnament.size());
    for (RwInt32 j = 0; j < NumDoodads; ++j)
    {
        CNtlPLObject* pObjTmp = static_cast<CNtlPLObject*>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityOrnament[j]);
        CNtlPLObjectProperty* pProperty = pObjTmp->GetProperty();
        if (!pProperty->GetApplyPVS())
        {
            continue;
        }
        pObjTmp->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
    }

    // Effects
    RwInt32 NumEffects = static_cast<RwInt32>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityEffect.size());
    for (RwInt32 j = 0; j < NumEffects; ++j)
    {
        pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityEffect[j]->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
    }

    // Decals
    RwInt32 NumDecals = static_cast<RwInt32>(pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityDecal.size());
    for (RwInt32 j = 0; j < NumDecals; ++j)
    {
        pFMgr->m_pSectors[_SectorIdx].m_vecNtlPLEntityDecal[j]->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
    }

    // Shoreline
    if (pFMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine)
    {
        pFMgr->m_pSectors[_SectorIdx].m_pNtlPLEntityShoreLine->SetCullFlags(NTL_PLEFLAG_CULLED_PVS, bCulled);
    }

    // Terrain & water
    m_Visibility = _Visible;
}

RwInt32 CNtlWorldSectorPVS::GetVisibilityNeighbor(RwInt32 _NeighborIdx, RwInt32 _iLayer)
{
#ifdef dNTL_WORLD_TOOL_MODE
    if (CNtlPLGlobal::m_iCurrentPVSLayer >= 0)
    {
        _iLayer = CNtlPLGlobal::m_iCurrentPVSLayer;
    }
#endif

    if (_NeighborIdx < 0 || _NeighborIdx >= dPVS_TOT_CELL_CNT || _iLayer < 0 || _iLayer >= dPVS_LAYER_CNT)
    {
        return 0;
    }

    return m_pVisibilityNeighbor[_NeighborIdx + dPVS_TOT_CELL_CNT * _iLayer];
}

RwBool CNtlWorldSectorPVS::TestPVS(RwInt32 _SrcSecIdx, RwInt32 _DstSecIdx, RwInt32 _CurArrayIdx, RwReal fEpsilsonHeight)
{
    CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
    if (!pFMgr || _SrcSecIdx < 0 || _DstSecIdx < 0)
    {
        return FALSE;
    }

    // Sector midpoints and starting positions
    RwV3d srcMidPos, dstMidPos;
    CNtlMath::MathRwV3dAssign(&srcMidPos, pFMgr->m_pSectors[_SrcSecIdx].DatumPoint.x,
        pFMgr->m_pSectors[_SrcSecIdx].DatumPoint.y,
        pFMgr->m_pSectors[_SrcSecIdx].DatumPoint.z);
    CNtlMath::MathRwV3dAssign(&dstMidPos, pFMgr->m_pSectors[_DstSecIdx].DatumPoint.x,
        pFMgr->m_pSectors[_DstSecIdx].DatumPoint.y,
        pFMgr->m_pSectors[_DstSecIdx].DatumPoint.z);

    RwV3d srcStartPos, dstStartPos;
    srcStartPos.x = srcMidPos.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
    srcStartPos.z = srcMidPos.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
    dstStartPos.x = dstMidPos.x - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
    dstStartPos.z = dstMidPos.z - dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;

    // Sample only key points (center and corners) for performance
    const float sectorSize = dGET_WORLD_PARAM()->WorldSectorSize;
    const float halfSize = sectorSize / 2.0f;
    const RwV3d srcPoints[] = {
        {srcMidPos.x, srcMidPos.y, srcMidPos.z},              // Center
        {srcStartPos.x, srcMidPos.y, srcStartPos.z},          // Bottom-left
        {srcStartPos.x + sectorSize, srcMidPos.y, srcStartPos.z}, // Bottom-right
        {srcStartPos.x, srcMidPos.y, srcStartPos.z + sectorSize}, // Top-left
        {srcStartPos.x + sectorSize, srcMidPos.y, srcStartPos.z + sectorSize} // Top-right
    };
    const RwV3d dstPoints[] = {
        {dstMidPos.x, dstMidPos.y, dstMidPos.z},              // Center
        {dstStartPos.x, dstMidPos.y, dstStartPos.z},          // Bottom-left
        {dstStartPos.x + sectorSize, dstMidPos.y, dstStartPos.z}, // Bottom-right
        {dstStartPos.x, dstMidPos.y, dstStartPos.z + sectorSize}, // Top-left
        {dstStartPos.x + sectorSize, dstMidPos.y, dstStartPos.z + sectorSize} // Top-right
    };

    // Layer-specific height offsets and results
    RwReal layerEpsilonHeights[dPVS_LAYER_CNT];
    RwBool layerTestResults[dPVS_LAYER_CNT];
    for (RwInt32 iLayerCnt = dPVS_LAYER_CNT - 1; iLayerCnt >= 0; --iLayerCnt)
    {
        layerEpsilonHeights[iLayerCnt] = fEpsilsonHeight + (iLayerCnt * PVS_LAYER_HEIGHT_INCREMENT);
        layerTestResults[iLayerCnt] = TRUE;
    }

    // Test visibility between sampled points
    for (int srcIdx = 0; srcIdx < 5; ++srcIdx)
    {
        RwV3d curSrcVert = srcPoints[srcIdx];
        pFMgr->GetHeight(curSrcVert);
        RwInt32 curSrcSecIdx = pFMgr->GetSectorIdx(curSrcVert);
        if (curSrcSecIdx == -1)
            continue;

        for (int dstIdx = 0; dstIdx < 5; ++dstIdx)
        {
            RwV3d curDstVert = dstPoints[dstIdx];
            pFMgr->GetHeight(curDstVert);
            RwInt32 curDstSecIdx = pFMgr->GetSectorIdx(curDstVert);
            if (curDstSecIdx == -1)
                continue;

            RwV3d curDir;
            RwV3dSubMacro(&curDir, &curDstVert, &curSrcVert);
            RwReal finDist = RwV3dLength(&curDir);
            RwV3dNormalize(&curDir, &curDir);

            RwReal curDist = 0.0f;
            RwInt32 curTestCnt = 1;

            RwBool layerOccluded[dPVS_LAYER_CNT] = { FALSE };
            RwBool allClear = TRUE;

            // Line-of-sight test
            while (curDist < finDist)
            {
                RwV3d curPosVert = curSrcVert;
                RwV3dIncrementScaled(&curPosVert, &curDir, static_cast<RwReal>(dPVS_LINE_TEST_EXTENT) * curTestCnt);
                curDist = RwV3dLength(&(curPosVert - curSrcVert));

                pFMgr->GetHeight(curPosVert);
                RwInt32 curPosSecIdx = pFMgr->GetSectorIdx(curPosVert);

                if (curPosSecIdx == curSrcSecIdx || curPosSecIdx == curDstSecIdx)
                {
                    ++curTestCnt;
                    continue;
                }

                RwBool lastTestPVS = FALSE;
                allClear = TRUE;
                for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
                {
                    if (layerTestResults[iLayerCnt] && !layerOccluded[iLayerCnt])
                    {
                        if (lastTestPVS)
                        {
                            layerOccluded[iLayerCnt] = lastTestPVS;
                        }
                        else if (curPosVert.y > curDstVert.y + layerEpsilonHeights[iLayerCnt] &&
                            curPosVert.y > curSrcVert.y + fEpsilsonHeight + layerEpsilonHeights[iLayerCnt])
                        {
                            layerOccluded[iLayerCnt] = lastTestPVS = TRUE;
                        }
                        else
                        {
                            allClear = FALSE;
                        }
                    }
                }

                if (allClear)
                    break;

                ++curTestCnt;
            }

            // Occluder test for nearby sectors only
            RwBool lastTestOcc = FALSE;
            for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
            {
                if (lastTestOcc)
                {
                    layerOccluded[iLayerCnt] = lastTestOcc;
                    continue;
                }

                if (layerTestResults[iLayerCnt] && !layerOccluded[iLayerCnt])
                {
                    RwV3d curSrcVertOcc = curSrcVert;
                    curSrcVertOcc.y += layerEpsilonHeights[iLayerCnt];
                    RwV3d curDstVertOcc = curDstVert;
                    curDstVertOcc.y += layerEpsilonHeights[iLayerCnt];

                    for (RwInt32 i = -PVS_SECTOR_TEST_RANGE; i <= PVS_SECTOR_TEST_RANGE; ++i)
                    {
                        for (RwInt32 j = -PVS_SECTOR_TEST_RANGE; j <= PVS_SECTOR_TEST_RANGE; ++j)
                        {
                            RwV3d vTemp;
                            vTemp.x = srcMidPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
                            vTemp.z = srcMidPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
                            RwInt32 iIndex = pFMgr->GetSectorIdx(vTemp);

                            if (iIndex == -1 || iIndex == curSrcSecIdx || iIndex == curDstSecIdx)
                                continue;

                            CNtlWorldSector* pSector = &pFMgr->m_pSectors[iIndex];
                            for (int k = 0; k < static_cast<int>(pSector->m_vecNtlPLEntityOccluder_Quad.size()); ++k)
                            {
                                CNtlPLOccluder_Base* pOccluder = static_cast<CNtlPLOccluder_Base*>(pSector->m_vecNtlPLEntityOccluder_Quad[k]);
                                if (pOccluder->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS) &&
                                    pOccluder->PVSTest(&curSrcVertOcc, &curDstVertOcc))
                                {
                                    layerOccluded[iLayerCnt] = lastTestOcc = TRUE;
                                    break;
                                }
                            }
                            if (lastTestOcc)
                                break;
                        }
                        if (lastTestOcc)
                            break;
                    }
                }
            }

            // Update layer test results
            RwBool allFailed = TRUE;
            for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
            {
                if (!layerOccluded[iLayerCnt])
                {
                    layerTestResults[iLayerCnt] = FALSE;
                }
                else
                {
                    allFailed = FALSE;
                }
            }
            if (allFailed)
                return FALSE;
        }
    }

    // Update visibility neighbor array
    for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
    {
        m_pVisibilityNeighbor[_CurArrayIdx + (iLayerCnt * dPVS_TOT_CELL_CNT)] = layerTestResults[iLayerCnt] ? 0 : 1;
    }

    return TRUE;
}

VOID CNtlWorldSectorPVS::CreatePVS(RwInt32 _SectorIdx, RwReal fEpsilsonHeight)
{
    CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
    if (!pFMgr || _SectorIdx < 0)
    {
        return;
    }

    DestroyPVS(_SectorIdx);

    m_Visibility = TRUE;
    m_pVisibilityNeighbor = NTL_NEW BYTE[dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT];
    if (!m_pVisibilityNeighbor)
    {
        return;
    }

    // Initialize all neighbors as visible
    RwInt32 tmpCnt = dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT;
    while (tmpCnt--)
    {
        m_pVisibilityNeighbor[tmpCnt] = 1;
    }

    RwV3d pos;
    CNtlMath::MathRwV3dAssign(&pos, pFMgr->m_pSectors[_SectorIdx].DatumPoint.x,
        pFMgr->m_pSectors[_SectorIdx].DatumPoint.y,
        pFMgr->m_pSectors[_SectorIdx].DatumPoint.z);

    RwInt32 cellCnt = dPVS_SECTOR_EXTENT_CNT * 2 + 1;
    RwV3d startPos;
    startPos.x = pos.x - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);
    startPos.z = pos.z - (dPVS_SECTOR_EXTENT_CNT * dGET_WORLD_PARAM()->WorldSectorSize);

    RwReal closestDistThreshold = (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f) * PVS_CLOSEST_NEIGHBOR_THRESHOLD;

    for (RwInt32 i = 0; i < cellCnt; ++i)
    {
        for (RwInt32 j = 0; j < cellCnt; ++j)
        {
            RwV3d curSectorPos;
            curSectorPos.x = startPos.x + j * dGET_WORLD_PARAM()->WorldSectorSize;
            curSectorPos.z = startPos.z + i * dGET_WORLD_PARAM()->WorldSectorSize;
            RwInt32 curSectorIdx = pFMgr->GetSectorIdx(curSectorPos);
            RwInt32 curArrayIdx = i * cellCnt + j;

            if (curSectorIdx == -1 || curSectorIdx == _SectorIdx)
            {
                for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
                {
                    m_pVisibilityNeighbor[curArrayIdx + (iLayerCnt * dPVS_TOT_CELL_CNT)] = 1;
                }
                continue;
            }

            RwV3d curTmp;
            CNtlMath::MathRwV3dAssign(&curTmp, pFMgr->m_pSectors[curSectorIdx].DatumPoint.x,
                pFMgr->m_pSectors[curSectorIdx].DatumPoint.y,
                pFMgr->m_pSectors[curSectorIdx].DatumPoint.z);
            RwReal curNeighborSecDist = RwV3dLength(&(pos - curTmp));
            if (curNeighborSecDist < closestDistThreshold)
            {
                for (RwInt32 iLayerCnt = 0; iLayerCnt < dPVS_LAYER_CNT; ++iLayerCnt)
                {
                    m_pVisibilityNeighbor[curArrayIdx + (iLayerCnt * dPVS_TOT_CELL_CNT)] = 1;
                }
                continue;
            }

            TestPVS(_SectorIdx, curSectorIdx, curArrayIdx, fEpsilsonHeight);
        }
    }
}

VOID CNtlWorldSectorPVS::DestroyPVS(RwInt32 _SectorIdx)
{
    m_Visibility = TRUE;
    NTL_DELETE(m_pVisibilityNeighbor);
}

VOID CNtlWorldSectorPVS::SetPVSAreaFilter(RwImage* _pPVSAreaFilter)
{
    if (m_pPVSAreaFilter)
    {
        RwImageDestroy(m_pPVSAreaFilter);
        m_pPVSAreaFilter = NULL;
    }
    if (_pPVSAreaFilter)
    {
        m_pPVSAreaFilter = _pPVSAreaFilter;
    }
}

RwInt32 CNtlWorldSectorPVS::GetPVSAreaTotNum()
{
    if (!m_pPVSAreaFilter)
    {
        return -1;
    }

    RwInt32 texelCnt = m_pPVSAreaFilter->width;
    DWORD* pBits = (DWORD*)RwImageGetPixels(m_pPVSAreaFilter);
    RwInt32 totPVSAreaCnt = 0;

    for (RwInt32 i = 0; i < texelCnt * texelCnt; ++i)
    {
        DWORD texel = pBits[i];
        RwRGBA rgba;
        rgba.red = (RwUInt8)((texel & 0x000000ff) >> 0);
        rgba.green = (RwUInt8)((texel & 0x0000ff00) >> 8);
        rgba.blue = (RwUInt8)((texel & 0x00ff0000) >> 16);
        rgba.alpha = (RwUInt8)((texel & 0xff000000) >> 24);

        if (rgba.red == 0 && rgba.green == 0 && rgba.blue == 0)
        {
            ++totPVSAreaCnt;
        }
    }

    return totPVSAreaCnt;
}

RwInt32 CNtlWorldSectorPVS::GetPVSAppliedNeighborSectorNum(RwInt32 _CurSectorIdx, RwInt32 _iLayer)
{
#ifdef dNTL_WORLD_TOOL_MODE
    if (CNtlPLGlobal::m_iCurrentPVSLayer >= 0)
    {
        _iLayer = CNtlPLGlobal::m_iCurrentPVSLayer;
    }
#endif

    if (_CurSectorIdx < 0 || !m_pVisibilityNeighbor || _iLayer < 0 || _iLayer >= dPVS_LAYER_CNT)
    {
        return 0;
    }

    RwInt32 retCnt = 0;
    for (RwInt32 i = 0; i < dPVS_TOT_CELL_CNT; ++i)
    {
        if (!m_pVisibilityNeighbor[i + (_iLayer * dPVS_TOT_CELL_CNT)])
        {
            ++retCnt;
        }
    }

    return retCnt;
}

RwBool CNtlWorldSectorPVS::GetPVSAreaSectorArray(vector<RwInt32>& _vecPVSAreaSectorArray)
{
    if (!m_pPVSAreaFilter)
    {
        return FALSE;
    }

    _vecPVSAreaSectorArray.clear();

    RwInt32 texelCnt = m_pPVSAreaFilter->width;
    DWORD* pBits = (DWORD*)RwImageGetPixels(m_pPVSAreaFilter);
    CNtlWorldFieldManager* pFieldMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
    RwInt32 curFieldSecIdx[4];

    for (RwInt32 i = 0; i < texelCnt * texelCnt; ++i)
    {
        DWORD texel = pBits[i];
        RwRGBA rgba;
        rgba.red = (RwUInt8)((texel & 0x000000ff) >> 0);
        rgba.green = (RwUInt8)((texel & 0x0000ff00) >> 8);
        rgba.blue = (RwUInt8)((texel & 0x00ff0000) >> 16);
        rgba.alpha = (RwUInt8)((texel & 0xff000000) >> 24);

        if (rgba.red || rgba.green || rgba.blue)
        {
            continue;
        }

        pFieldMgr->GetFieldSectorIndices(texelCnt * texelCnt - 1 - i, curFieldSecIdx);
        _vecPVSAreaSectorArray.push_back(curFieldSecIdx[0]);
        _vecPVSAreaSectorArray.push_back(curFieldSecIdx[1]);
        _vecPVSAreaSectorArray.push_back(curFieldSecIdx[2]);
        _vecPVSAreaSectorArray.push_back(curFieldSecIdx[3]);
    }

    return TRUE;
}

VOID CNtlWorldSectorPVS::RenderLockingGuideLine()
{
    if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PVS_LOCKING) ||
        GetSceneManager()->GetActiveWorldType() != AW_HEGITHFIELD)
    {
        return;
    }

    CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
    

    BeginD3DLineState();
    RwD3D9SetFVF(D3DFVF_LINE3D);

    D3DXMATRIXA16 mIdentity;
    D3DXMatrixIdentity(&mIdentity);
    RwD3D9SetTransform(D3DTS_WORLD, &mIdentity);

    D3DXVECTOR3 midPos = pFMgr->m_pSectors[m_LockedPVSSector].DatumPoint;
    RwV3d midPosRw;
    CNtlMath::MathRwV3dAssign(&midPosRw, midPos.x, midPos.y, midPos.z);
    midPos.y = pFMgr->GetWorldSectorHeight(midPosRw);

    sLINE3D brushLine[2];
    const float halfSize = dGET_WORLD_PARAM()->WorldSectorSize / 2.0f;
    const float farPlaneHeight = dGET_WORLD_PARAM()->WorldFarPlane;

    // Draw corner lines (blue)
    const RwV3d offsets[4] = {
        {-halfSize, 0.0f, -halfSize}, // Bottom-left
        {halfSize, 0.0f, -halfSize},  // Bottom-right
        {halfSize, 0.0f, halfSize},   // Top-right
        {-halfSize, 0.0f, halfSize}   // Top-left
    };

    for (int i = 0; i < 4; ++i)
    {
        brushLine[0].Col = 0xff0000ff; // Blue
        brushLine[1].Col = 0xff0000ff;
        brushLine[0].Pos = midPos;
        brushLine[0].Pos.x += offsets[i].x;
        brushLine[0].Pos.z += offsets[i].z;
        RwV3d temp = { brushLine[0].Pos.x, 0.0f, brushLine[0].Pos.z };
        brushLine[0].Pos.y = pFMgr->GetWorldSectorHeight(temp);
        brushLine[1].Pos = brushLine[0].Pos;
        brushLine[1].Pos.y += farPlaneHeight;
        RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, brushLine, sizeof(sLINE3D));
    }

    // Draw center line (red)
    brushLine[0].Col = 0xffff0000; // Red
    brushLine[1].Col = 0xffff0000;
    brushLine[0].Pos = midPos;
    brushLine[1].Pos = brushLine[0].Pos;
    brushLine[1].Pos.y += farPlaneHeight;
    RwD3D9DrawPrimitiveUP(D3DPT_LINELIST, 1, brushLine, sizeof(sLINE3D));

    EndD3DLineState();
}