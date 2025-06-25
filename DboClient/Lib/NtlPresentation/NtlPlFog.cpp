#include "precomp_ntlpresentation.h"
#include "NtlPLGlobal.h"
#include "NtlPLRenderState.h"
#include "NtlPLFog.h"
#include "ntlworldcommon.h"
#include "ntlworldinterface.h"
#include "NtlPLEvent.h"
#include "NtlCoreUtil.h"
#include "NtlPLApi.h"
#include "NtlPLWorldState.h"
#include "NtlPLOptionManager.h"

void CNtlPLFog::Initialize(void)
{
    SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
    SetLayer(PLENTITY_LAYER_FOG);
    SetClassType(PLENTITY_FOG);

    m_eType = rwFOGTYPELINEAR;

    m_RGBA.red = 200;   // Reduced from 255 for softer red
    m_RGBA.green = 200; // Reduced from 255 for softer green
    m_RGBA.blue = 200;  // Reduced from 255 for softer blue
    m_RGBA.alpha = 200; // Slightly reduced from 255 for a more translucent fog


    // Increased near plane to start fog further away
    m_Plane[0] = 150.0f; // Changed from 10.0f to push fog further
    m_Plane[1] = 700.0f; // Slightly increased from 456.0f for consistency

    m_bSwitch = TRUE;
    m_RestTime4Change = -999.0f;
}

void CNtlPLFog::HandleEvents(RWS::CMsg& pMsg)
{
    if (CheckEventsWorldEffectChanged(pMsg))
    {
        sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);

        if (m_RGBA.red == pNtlFieldProp->_FogColor.red &&
            m_RGBA.green == pNtlFieldProp->_FogColor.green &&
            m_RGBA.blue == pNtlFieldProp->_FogColor.blue &&
            static_cast<int>(m_Plane[0]) == static_cast<int>(pNtlFieldProp->_FogCamPlane[0]) &&
            static_cast<int>(m_Plane[1]) == static_cast<int>(pNtlFieldProp->_FogCamPlane[1]))
        {
            // Same fog properties, just ignore messages
            return;
        }
        else
        {
            // Increased transition time for smoother change (assuming dFOG_EFFECT_SWITCHING_TIME is 3.0f)
            m_RestTime4Change = dFOG_EFFECT_SWITCHING_TIME; // Suggest defining as 3.0f for smoother transitions

            m_FogRGBA4Change = pNtlFieldProp->_FogColor;
            m_FogPlanes[0] = pNtlFieldProp->_FogCamPlane[0];
            m_FogPlanes[1] = pNtlFieldProp->_FogCamPlane[1];

            m_FogRGBA4ChangeOld = m_RGBA;
            m_FogPlanesOld[0] = m_Plane[0];
            m_FogPlanesOld[1] = m_Plane[1];
        }
    }
}

RwBool CNtlPLFog::Create(const SPLEntityCreateParam* pParam)
{
    NTL_FUNCTION("CNtlPLFog::Create");

    LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
    LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
    LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

    NTL_RETURN(TRUE);
}

void CNtlPLFog::Destroy(void)
{
    RwRenderStateSet(rwRENDERSTATEFOGENABLE, (void*)FALSE);

    UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
    UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
    UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

    return;
}

RwBool CNtlPLFog::Update(RwReal fElapsed)
{
    if (m_RestTime4Change >= 0.0f)
    {
        m_RestTime4Change -= g_GetElapsedTime();
        if (m_RestTime4Change < 0)
        {
            m_RestTime4Change = -999.0f;

            m_RGBA = m_FogRGBA4Change;
            m_Plane[0] = m_FogPlanes[0];
            m_Plane[1] = m_FogPlanes[1];
        }
        else
        {
            // Linear interpolation for smooth color and plane transitions
            m_RGBA.red = RpNtlWorldUInt8PropLerp(m_FogRGBA4ChangeOld.red, m_FogRGBA4Change.red, m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
            m_RGBA.green = RpNtlWorldUInt8PropLerp(m_FogRGBA4ChangeOld.green, m_FogRGBA4Change.green, m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
            m_RGBA.blue = RpNtlWorldUInt8PropLerp(m_FogRGBA4ChangeOld.blue, m_FogRGBA4Change.blue, m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
            m_Plane[0] = RpNtlWorldRealPropLerp(m_FogPlanesOld[0], m_FogPlanes[0], m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
            m_Plane[1] = RpNtlWorldRealPropLerp(m_FogPlanesOld[1], m_FogPlanes[1], m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
        }
    }

    // DN effect
    RwInt32 TFactor = -static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff);
    ::CopyMemory(&m_RGBA4DN, &m_RGBA, sizeof(RwRGBA));
    ::API_PL_8BColorClamp(&m_RGBA4DN.red, TFactor);
    ::API_PL_8BColorClamp(&m_RGBA4DN.green, TFactor);
    ::API_PL_8BColorClamp(&m_RGBA4DN.blue, TFactor);

    return TRUE;
}

RwBool CNtlPLFog::IsWorking()
{
    return m_bSwitch;
}

void CNtlPLFog::Switch()
{
    m_bSwitch = !m_bSwitch;
}

RwBool CNtlPLFog::Render(void)
{
    // Adjusted near plane calculation to ensure fog starts further away
    RwReal fPlaneNear = m_Plane[0] - GetNtlPLOptionManager()->GetTerrainFar() + dFOG_EFFECT_FAR;
    RwReal fPlaneFar = dFOG_EFFECT_FAR;

    // Ensure near plane is at least a reasonable distance from the camera
    fPlaneNear = CNtlPLGlobal::m_RwCamera->nearPlane + (fPlaneNear > dFOG_EFFECT_NEAR ? fPlaneNear : dFOG_EFFECT_NEAR);
    fPlaneFar = CNtlPLGlobal::m_RwCamera->nearPlane + (fPlaneFar < m_Plane[1] ? fPlaneFar : m_Plane[1]);

    // Clamp to prevent fog from starting too close or extending too far
    CLAMP(fPlaneNear, dFOG_EFFECT_NEAR, m_Plane[0]);
    CLAMP(fPlaneFar, dFOG_EFFECT_FAR, m_Plane[1]);

    BegLinearFogRenderState(RWRGBALONG(m_RGBA4DN.red, m_RGBA4DN.green, m_RGBA4DN.blue, 155), fPlaneNear, fPlaneFar);

    if (!m_bSwitch)
    {
        EndLinearFogRenderState();
    }
    return TRUE;
}