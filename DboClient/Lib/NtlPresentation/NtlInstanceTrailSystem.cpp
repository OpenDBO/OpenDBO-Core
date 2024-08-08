#include "precomp_ntlpresentation.h"
#include "NtlInstanceTrailSystem.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLRenderState.h"

void* CNtlInstanceTrailSystem::operator new(size_t size)
{
    NTL_FUNCTION(__FUNCTION__);

    NTL_RETURN(CNtlEffectSystemFreeList::Alloc(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL));
}

void CNtlInstanceTrailSystem::operator delete(void *pObj)
{
    CNtlEffectSystemFreeList::Free(CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_TRAIL, pObj);
}

CNtlInstanceTrailSystem::CNtlInstanceTrailSystem(void)
{
    Init();
}

CNtlInstanceTrailSystem::~CNtlInstanceTrailSystem(void)
{
    Delete();

}

void CNtlInstanceTrailSystem::Reset() 
{
    Delete();
}

void CNtlInstanceTrailSystem::Init() 
{
    CNtlInstanceTraceSystem::Init();

    m_pEmitterTrail = NULL;
    m_pAttachComponent = NULL;
    ZeroMemory(&m_vPrevTrailPoint, sizeof(RwV3d));

    m_uiMemoryUseSize = sizeof(CNtlInstanceTrailSystem);
}

void CNtlInstanceTrailSystem::Delete() 
{
    CNtlInstanceTraceSystem::Delete();
}

/**
 * Trail 이펙트를 생성한다.
 * \param pComponentSystem Trail이 붙을 부모 InstanceComponentSystem 객체
 * \param pEmitterTrail Trail 생성 정보를 가지고 있는 프로퍼티 객체
 * return 성공 유무
 */
RwBool CNtlInstanceTrailSystem::Create(CNtlInstanceComponentSystem* pComponentSystem, SNtlPrtStdEmitterTrail* pEmitterTrail)
{
    if(!pEmitterTrail || !pComponentSystem)
        return FALSE;

    m_pEmitterTrail = pEmitterTrail;
    m_pAttachComponent = pComponentSystem;

    // 이벤트 객체를 생성하여, 값을 넣는다. (상위 클래스의 메소드들을 그대로 활용할수 있기 때문이다)
    m_pEventTrace = NTL_NEW SEventTrace();    
    m_pEventTrace->fLifeTime     = 999999;                      ///< Trail에는 LifeTime이 없다.
    m_pEventTrace->fEdgeLifeTime = pEmitterTrail->fEdgeLifeTime;
    m_pEventTrace->fMaxLength    = pEmitterTrail->fMaxLength;
    m_pEventTrace->fEdgeGap      = pEmitterTrail->fEdgeGap;
    m_pEventTrace->nSplinePointCount = pEmitterTrail->nSplinePointCount;
    m_pEventTrace->eSrcBlend     = pEmitterTrail->eSrcBlend;
    m_pEventTrace->eDestBlend    = pEmitterTrail->eDestBlend;
    m_pEventTrace->colStartColor = pEmitterTrail->colStartColor;
    m_pEventTrace->nMaxEdgeCount = pEmitterTrail->nMaxEdgeCount;
    m_pEventTrace->colEndColor   = pEmitterTrail->colEndColor;  
    m_uiMemoryUseSize += sizeof(SEventTrace);

    if(CNtlInstanceEffect::GetLowSpecEnable())
    {
        m_pEventTrace->fEdgeGap *= 1.0f / CNtlInstanceEffect::GetLowSpecRatio();
    }

    // Vertex Buffer 생성    
    m_pVertices = NTL_NEW RwIm3DVertex[pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount * 2];
    if(!m_pVertices)
        return FALSE;
    ZeroMemory(m_pVertices, sizeof(RwIm3DVertex) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount * 2);
    m_uiMemoryUseSize += sizeof(RwIm3DVertex) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount * 2;

    // Pool을 생성한다.
    m_poolTraceEdge = NTL_NEW STraceEdge[pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount];
    if(!m_poolTraceEdge)
        return FALSE;
    ZeroMemory(m_poolTraceEdge, sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount);
    m_uiMemoryUseSize += sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount * pEmitterTrail->nSplinePointCount;

    m_poolControlPoint = NTL_NEW STraceEdge[pEmitterTrail->nMaxEdgeCount];
    if(!m_poolControlPoint)
        return FALSE;
    ZeroMemory(m_poolControlPoint, sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount);
    m_uiMemoryUseSize += sizeof(STraceEdge) * pEmitterTrail->nMaxEdgeCount;
    
    // 텍스쳐 설정
    std::string strTexture = pEmitterTrail->strTexture;
    m_pCurrentTexture = m_pStandardTexture = CreateTexture(strTexture);

    m_vPrevTrailPoint = *RwMatrixGetPos(&m_pAttachComponent->m_matRender);

    return TRUE;
}

/**
 * 새로운 Edge를 리스트의 맨끝에 추가한다. 
 */
void CNtlInstanceTrailSystem::CreateEdge() 
{
    // 최대 Edge 개수를 초과했는지 계산한다.
    if(m_nControlPointIndex >= m_pEventTrace->nMaxEdgeCount)
    {
        // Index가 Pool의 끝에 달하면 처음으로 되돌린다.
        m_nControlPointIndex = 0;        

        m_listControlPoint.erase(m_listControlPoint.begin());
    }
    // 최대 길이를 초과 했는지 계산한다.
    if(m_listTraceEdge.size() > 2)
    {
        RwReal fLength = 0.0f;               

        ListTraceEdge::iterator it = m_listTraceEdge.begin();
        RwV3d  v3dPrevPos = (*it)->vVertices[0].objVertex;
        ++it;

        while(it != m_listTraceEdge.end())
        {            
            RwV3d vLength = (*it)->vVertices[0].objVertex - v3dPrevPos;
            fLength += RwV3dLength(&vLength);

            v3dPrevPos = (*it)->vVertices[0].objVertex;
            ++it;
        }

        if(fLength >= m_pEventTrace->fMaxLength)
        {
            m_listTraceEdge.erase(m_listTraceEdge.begin());            
        }
    }

    // 두개의 Vertex의 위치를 설정한다.
    SetEdgePoint();
    
    // List에 새로운 컨트롤 포인트를 추가한다.
    m_poolControlPoint[m_nControlPointIndex].vVertices[0].objVertex = m_vStartPoint;
    m_poolControlPoint[m_nControlPointIndex].vVertices[1].objVertex = m_vEndPoint;

    m_listControlPoint.push_back(&m_poolControlPoint[m_nControlPointIndex]);
    ++m_nControlPointIndex;

    // Spline Curve Point를 생성하여 리스트에 추가한다.
    CreateSplinePath();  

}

void CNtlInstanceTrailSystem::SetEdgePoint() 
{
    //RwV3d vTrailPoint = *RwMatrixGetPos(&m_pAttachComponent->m_matRender);
    //RwV3d vDir = vTrailPoint - m_vPrevTrailPoint;
    //RwV3dNormalize(&vDir, &vDir);

    //RwV3d vCameraPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
    //
    //RwV3d vAtCamera = vCameraPos - m_vPrevTrailPoint;
    //RwV3dNormalize(&vAtCamera, &vAtCamera);

    //RwV3d vRight;
    //RwV3dCrossProduct(&vRight, &vAtCamera, &vDir);
    //RwV3dNormalize(&vRight, &vRight);

    //m_vStartPoint = m_vPrevTrailPoint + vRight * m_pEmitterTrail->fWidth * 0.5f;
    //m_vEndPoint   = m_vPrevTrailPoint - vRight * m_pEmitterTrail->fWidth * 0.5f;

    //m_vPrevTrailPoint = vTrailPoint;

    RwV3d vTrailPoint = *RwMatrixGetPos(&m_pAttachComponent->m_matRender);
    RwV3d vDir = vTrailPoint - m_vPrevTrailPoint;
    RwV3dNormalize(&vDir, &vDir);

    RwV3d vCameraPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
    RwV3d vAtCamera = vCameraPos - vTrailPoint;
    RwV3dNormalize(&vAtCamera, &vAtCamera);

    RwV3d vRight;
    RwV3dCrossProduct(&vRight, &vAtCamera, &vDir);
    RwV3dNormalize(&vRight, &vRight);

    m_vStartPoint = vTrailPoint + vRight * m_pEmitterTrail->fWidth * 0.5f;
    m_vEndPoint   = vTrailPoint - vRight * m_pEmitterTrail->fWidth * 0.5f;

    // Offset이 설정되어 있으면 적용한다.
    if(RwV3dLength(&m_pEmitterTrail->vOffset) > 0.0f)
    {
        // Matrix에서 회전값만 가져와서 Offset에 적용한 후 Point에 더한다.
        RwMatrix matComponent = m_pAttachComponent->m_matRender;
        *RwMatrixGetPos(&matComponent) = ZeroAxis;

        RwV3d vOffset;
        RwV3dTransformPoint(&vOffset, &m_pEmitterTrail->vOffset, &matComponent);

        m_vStartPoint += vOffset;
        m_vEndPoint += vOffset;
    }

    m_vPrevTrailPoint = vTrailPoint;
}