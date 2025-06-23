#include "precomp_ntlsimulation.h"
#include "NtlSobMascotProxy.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"
#include <string.h>
#include <string>
#include <map>

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGlobal.h"
#include "NtlPLSceneManager.h"
#include "NtlPLCharacter.h"
#include "NtlPLCameraRenderTexture.h"
#include "NtlPLHelpers.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLCharacterProperty.h"
#include "NtlPLResourceManager.h"
#include "NtlPLEntityRenderHelpers.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogicDef.h"
#include "NtlSobManager.h"
#include "NtlSobProxyDef.h"
#include "NtlSobAvatar.h"

// mascot
#include "NtlSobMascot.h"
#include "NtlSobMascotAttr.h"
#include "MascotTable.h"

// shared - animation constants
#include "GraphicDataTable.h"

// Define the event handler map
CNtlSobMascotProxy::MapFuncEventHandler CNtlSobMascotProxy::m_mapEventFunc;

// Helper function for replacing mascot textures
struct MascotTextureReplaceData {
	RwTexture* pNewTexture;
	RwBool bReplaced;
};

static RpAtomic* ReplaceMascotTextureCallback(RpAtomic* pAtomic, void* pData)
{
	MascotTextureReplaceData* pReplaceData = (MascotTextureReplaceData*)pData;
	
	RpGeometry* pGeometry = RpAtomicGetGeometry(pAtomic);
	if (pGeometry)
	{
		RwInt32 materialCount = RpGeometryGetNumMaterials(pGeometry);
		for (RwInt32 i = 0; i < materialCount; i++)
		{
			RpMaterial* pMaterial = RpGeometryGetMaterial(pGeometry, i);
			if (pMaterial)
			{
				RpMaterialSetTexture(pMaterial, pReplaceData->pNewTexture);
				pReplaceData->bReplaced = TRUE;
			}
		}
	}
	
	return pAtomic;
}



void CNtlSobMascotProxy::RegisterEventHandler(void)
{
	m_mapEventFunc[g_EventSobCreate.Get_pEventId()] = &CNtlSobMascotProxy::SobCreateEventHandler;
	m_mapEventFunc[g_EventSobDelete.Get_pEventId()] = &CNtlSobMascotProxy::SobDestroyEventHandler;
	m_mapEventFunc[g_EventSobGotFocus.Get_pEventId()] = &CNtlSobMascotProxy::SobGotFocusEventHandler;
	m_mapEventFunc[g_EventSobLostFocus.Get_pEventId()] = &CNtlSobMascotProxy::SobLostFocusEventHandler;
}

CNtlSobMascotProxy::CNtlSobMascotProxy()
{
	m_pCharacter = NULL;
	m_bResLoaded = FALSE;
	m_byAnimBlendNoneOneTime = 0;
	
	// Initialize animation system members
	m_pBaseHierarchy = NULL;
	m_uiCurrentAnimKey = 0;
	
	m_pUIMascotStatusWnd = NULL;
	m_pUIMascotStatusRotate = NULL;
	m_pUIMascotStatusBarCamera = NULL;
	m_bUIMascotStatusWndInitialized = FALSE;
}

CNtlSobMascotProxy::~CNtlSobMascotProxy()
{
	Destroy();
}

RwBool CNtlSobMascotProxy::Create(RwUInt32 uiCompType)
{
	OutputDebugStringA("[MASCOT_PROXY_CREATE_DEBUG] CNtlSobMascotProxy::Create - Entry\n");
	
	if (!CNtlSobProxy::Create(uiCompType))
	{
		OutputDebugStringA("[MASCOT_PROXY_CREATE_DEBUG] Failed to create base proxy\n");
		return FALSE;
	}
	OutputDebugStringA("[MASCOT_PROXY_CREATE_DEBUG] Base proxy created successfully\n");
	
	// Initialize UI rotation
	m_pUIMascotStatusRotate = NTL_NEW SUIMascotStatusRotate;
	memset(m_pUIMascotStatusRotate, 0, sizeof(SUIMascotStatusRotate));
	OutputDebugStringA("[MASCOT_PROXY_CREATE_DEBUG] UI rotation initialized\n");
	
	OutputDebugStringA("[MASCOT_PROXY_CREATE_DEBUG] CNtlSobMascotProxy::Create - Exit (SUCCESS)\n");
	return TRUE;
}

void CNtlSobMascotProxy::Destroy(void)
{
	// Clean up animation system
	if (m_pBaseHierarchy)
	{
		RpHAnimHierarchyDestroy(m_pBaseHierarchy);
		m_pBaseHierarchy = NULL;
	}
	
	m_InstanceAnimTable.Destroy();

	// Clean up UI rotation
	if (m_pUIMascotStatusRotate)
	{
		NTL_DELETE(m_pUIMascotStatusRotate);
		m_pUIMascotStatusRotate = NULL;
	}
	
	CNtlSobProxy::Destroy();
}

void CNtlSobMascotProxy::Update(RwReal fElapsed)
{
	UpdateResourceLoadCheck(fElapsed);
	UpdateGUIUpdateCheck(fElapsed);
	UpdateMascotFollowing(fElapsed);  // Add following behavior

	// Update animation hierarchy if available (following character proxy pattern)
	if (m_pBaseHierarchy)
	{
		RpHAnimHierarchyAddAnimTime(m_pBaseHierarchy, fElapsed);
		RpHAnimHierarchyUpdateMatrices(m_pBaseHierarchy);
	}

	CNtlSobProxy::Update(fElapsed);
	
	// Update UI rotation if needed
	if (m_pUIMascotStatusRotate)
	{
		MascotStatusRotate(fElapsed);
	}
}

void CNtlSobMascotProxy::HandleEvents(RWS::CMsg &pMsg)
{
	CNtlSobProxy::HandleEvents(pMsg);
	
	// Handle mascot-specific events
	MapFuncEventHandler::iterator it = m_mapEventFunc.find(pMsg.Id);
	if (it != m_mapEventFunc.end())
	{
		(this->*(it->second))(pMsg);
	}
}

// Event handlers
void CNtlSobMascotProxy::SobCreateEventHandler(RWS::CMsg &pMsg)
{
	// Handle mascot creation event
	SNtlEventSobBaseCreate* pSobCreate = (SNtlEventSobBaseCreate*)pMsg.pData;
	
	// Create the main mascot entity (not for shading)
	m_pCharacter = CreateMascotEntity(FALSE);
	
	// Create UI character and camera for texture rendering like character proxy does
	CreateUIMascotStatusWndCharacter();
	CreateUIMascotStatusWndCamera();
}

void CNtlSobMascotProxy::SobDestroyEventHandler(RWS::CMsg &pMsg)
{
	// Handle mascot destruction event
	DeleteMascotEntity();
	DeleteUIMascotStatusWndCharacter();
	DeleteUIMascotStatusWndCamera();
}

void CNtlSobMascotProxy::SobGotFocusEventHandler(RWS::CMsg &pMsg)
{
	// Handle focus gained event
	SetFocus();
}

void CNtlSobMascotProxy::SobLostFocusEventHandler(RWS::CMsg &pMsg)
{
	// Handle focus lost event
	ReleaseFocus();
}

// Mascot entity creation following character proxy pattern exactly
CNtlPLCharacter* CNtlSobMascotProxy::CreateMascotEntity(RwBool bNotShading)
{
	NTL_FUNCTION("CNtlSobMascotProxy::CreateMascotEntity");

	CNtlSobMascot* pMascot = (CNtlSobMascot*)m_pSobObj;
	if (!pMascot)
	{
		NTL_RETURN(NULL);
	}

	CNtlSobMascotAttr* pMascotAttr = pMascot->GetMascotAttr();
	if (!pMascotAttr)
	{
		NTL_RETURN(NULL);
	}

	const char* pModelKey = pMascotAttr->GetModelName();
	if (!pModelKey || strlen(pModelKey) == 0)
	{
		OutputDebugStringA("[MASCOT_PROXY_DEBUG] Invalid model name, using default\n");
		pModelKey = "P_EGG_E1"; // Use the actual mascot model
	}
	
	char debugBuffer2[512];
	sprintf_s(debugBuffer2, "[MASCOT_PROXY_DEBUG] Using model key from table: %s\n", pModelKey);
	OutputDebugStringA(debugBuffer2);
	
	// Check if the property exists in the character property container
	CNtlPLProperty* pProperty = CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_CHARACTER, pModelKey);
	sprintf_s(debugBuffer2, "[MASCOT_PROXY_DEBUG] Property lookup result: %s\n", pProperty ? "FOUND" : "NOT_FOUND");
	OutputDebugStringA(debugBuffer2);
	
	if (!pProperty)
	{
		OutputDebugStringA("[MASCOT_PROXY_DEBUG] ERROR: P_EGG_E1 property not found in character property container!\n");
		OutputDebugStringA("[MASCOT_PROXY_DEBUG] This means the property is missing from characterproperty.rdf\n");
	}
	else
	{
		OutputDebugStringA("[MASCOT_PROXY_DEBUG] SUCCESS: P_EGG_E1 property found, proceeding with entity creation\n");
	}

	char debugBuffer[512];
	sprintf_s(debugBuffer, "[MASCOT_PROXY_DEBUG] Creating mascot entity with model: %s\n", pModelKey);
	OutputDebugStringA(debugBuffer);
	
	// Follow character proxy pattern exactly with SPLCharacterCreateParam
	SPLCharacterCreateParam sParam;
	RwV3d vPos = m_pSobObj->GetPosition();
	sParam.pPos = &vPos;
	sParam.uiSerialId = m_pSobObj->GetSerialID();
	sParam.bPcFlag = FALSE; // Mascots are not PCs
	sParam.uiClass = 0;
	sParam.uiRace = 0;
	sParam.uiGender = 0;
	sParam.uiFaceType = 0;
	sParam.uiHeadType = 0;
	sParam.uiHeadColorType = 0;
	sParam.uiSkinColorType = 0;
	sParam.bNotShadingFlag = bNotShading; // Use the parameter properly
	sParam.bIsAdult = FALSE;
	
	// GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pKey, &sParam) like character proxy
	CNtlPLCharacter* pPLCharacter = static_cast<CNtlPLCharacter*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, pModelKey, &sParam));
	if (!pPLCharacter)
	{
		sprintf_s(debugBuffer, "[MASCOT_PROXY_DEBUG] Failed to create mascot entity with model: %s\n", pModelKey);
		OutputDebugStringA(debugBuffer);
		NTL_RETURN(NULL);
	}

	// Set serial ID like character proxy does
	pPLCharacter->SetSerialID(m_pSobObj->GetSerialID());
	
	// Set basic properties
	pPLCharacter->SetScale(0.5f); // Make mascot smaller than characters
	
	RwV3d vNewPos = {0.0f, 0.0f, 0.0f};
	RwV3d vDir = {0.0f, 0.0f, 1.0f};
	pPLCharacter->SetPosition(&vNewPos);
	pPLCharacter->SetDirection(&vDir);

	// ANIMATION SYSTEM INITIALIZATION (following character proxy pattern exactly)
	OutputDebugStringA("[MASCOT_ANIMATION] Initializing animation system\n");
	
	// Get character property for animation table
	CNtlPLCharacterProperty* pCharProperty = (CNtlPLCharacterProperty*)pProperty;
	if (pCharProperty && pCharProperty->GetAnimTable())
	{
		// Initialize instance animation table with property's animation table
		m_InstanceAnimTable.Create(pCharProperty->GetAnimTable());
		OutputDebugStringA("[MASCOT_ANIMATION] Animation table created successfully\n");
		
		sprintf_s(debugBuffer, "[MASCOT_ANIMATION] Animation path: %s\n", pCharProperty->GetAnimTable()->GetAnimPath().c_str());
		OutputDebugStringA(debugBuffer);
		
		// Wait for clump to be loaded, then set up animation hierarchy
		// Note: This will be completed in the update loop when resource is loaded
		OutputDebugStringA("[MASCOT_ANIMATION] Animation system setup queued for after clump loading\n");
	}
	else
	{
		OutputDebugStringA("[MASCOT_ANIMATION] ERROR: Character property or animation table not found\n");
	}

	// CRITICAL: Only add to world if this is NOT a UI mascot (bNotShading = FALSE means main world mascot)
	if (!bNotShading)
	{
		pPLCharacter->AddWorld();
		pPLCharacter->SetVisible(TRUE);
		OutputDebugStringA("[MASCOT_WORLD_CREATE] Main world mascot added to world and made visible\n");
		
		// Position world mascot near player initially
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (pAvatar)
		{
			RwV3d vPlayerPos = pAvatar->GetPosition();
			RwV3d vMascotPos = vPlayerPos;
			vMascotPos.x += 2.0f; // Position to the right of player
			vMascotPos.y += 1.2f; // Add height offset to ensure mascot appears above ground
			vMascotPos.z += 1.0f; // And forward
			
			pPLCharacter->SetPosition(&vMascotPos);
			
			char debugBuffer2[256];
			sprintf_s(debugBuffer2, "[MASCOT_WORLD_CREATE] World mascot positioned near player at (%.3f, %.3f, %.3f)\n", 
				vMascotPos.x, vMascotPos.y, vMascotPos.z);
			OutputDebugStringA(debugBuffer2);
		}
		
		sprintf_s(debugBuffer, "[MASCOT_PROXY_DEBUG] Created WORLD mascot entity successfully with model: %s\n", pModelKey);
		OutputDebugStringA(debugBuffer);
		
		// SIMPLE FIX: Since world mascot is already working and visible, enable following immediately
		m_bResLoaded = TRUE;
		OutputDebugStringA("[MASCOT_WORLD_CREATE] *** FOLLOWING ENABLED - World mascot ready for following! ***\n");
		
		// CRITICAL: Make sure the mascot object is properly registered for updates
		// This ensures the proxy's Update function gets called by the game engine
		if (m_pSobObj)
		{
			// Force the mascot to be active and receive updates
			m_pSobObj->SetFlags(m_pSobObj->GetFlags() | SLFLAG_ADD_UPDATE);
			OutputDebugStringA("[MASCOT_WORLD_CREATE] Mascot object set to ACTIVE for updates\n");
			
			// Debug: Check current SOB object state
			char debugBuffer4[256];
			sprintf_s(debugBuffer4, "[MASCOT_WORLD_CREATE] SOB Object - SerialID: %u, Flags: %u, ClassID: %u\n", 
				m_pSobObj->GetSerialID(), m_pSobObj->GetFlags(), m_pSobObj->GetClassID());
			OutputDebugStringA(debugBuffer4);
			
			// Make sure the object is properly added to SOB manager
			CNtlSob* pExistingSOB = GetNtlSobManager()->GetSobObject(m_pSobObj->GetSerialID());
			if (pExistingSOB)
			{
				OutputDebugStringA("[MASCOT_WORLD_CREATE] SOB object found in manager - should receive updates\n");
			}
			else
			{
				OutputDebugStringA("[MASCOT_WORLD_CREATE] WARNING: SOB object NOT found in manager - this is the problem!\n");
				OutputDebugStringA("[MASCOT_WORLD_CREATE] ATTEMPTING TO FIX: Adding to SOB manager\n");
				
				// Check if serial ID is already valid (assigned by GUI)
				SERIAL_HANDLE hCurrentSerial = m_pSobObj->GetSerialID();
				if (hCurrentSerial == INVALID_SERIAL_ID)
				{
					// Fallback: assign new serial ID if still invalid
					SERIAL_HANDLE hNewSerial = (SERIAL_HANDLE)GetTickCount64();
					if (hNewSerial == INVALID_SERIAL_ID)
						hNewSerial = 1000000;
					
					m_pSobObj->SetSerialID(hNewSerial);
					hCurrentSerial = hNewSerial;
					
					char debugBuffer5[256];
					sprintf_s(debugBuffer5, "[MASCOT_WORLD_CREATE] Assigned fallback SerialID: %u\n", hNewSerial);
					OutputDebugStringA(debugBuffer5);
				}
				else
				{
					char debugBuffer5[256];
					sprintf_s(debugBuffer5, "[MASCOT_WORLD_CREATE] Using existing valid SerialID: %u\n", hCurrentSerial);
					OutputDebugStringA(debugBuffer5);
				}
				
				// Now add to SOB manager with valid serial ID
				GetNtlSobManager()->AddObject(m_pSobObj->GetClassID(), m_pSobObj);
				GetNtlSobManager()->AddUpdate(m_pSobObj);
				
				// Verify it's now registered
				CNtlSob* pVerifySOB = GetNtlSobManager()->GetSobObject(hCurrentSerial);
				if (pVerifySOB)
				{
					OutputDebugStringA("[MASCOT_WORLD_CREATE] SUCCESS: Mascot now found in SOB manager with valid serial ID!\n");
				}
				else
				{
					OutputDebugStringA("[MASCOT_WORLD_CREATE] ERROR: Still not found in SOB manager after manual add\n");
				}
			}
		}
	}
	else
	{
		// UI mascot should NOT be added to world - only used for texture rendering
		pPLCharacter->SetVisible(TRUE);
		
		// Position UI mascot at origin (0,0,0) for camera rendering
		RwV3d vUIPos = {0.0f, 0.0f, 0.0f};
		pPLCharacter->SetPosition(&vUIPos);
		
		OutputDebugStringA("[MASCOT_UI_CREATE] UI mascot created for texture rendering only (NOT added to world)\n");
		sprintf_s(debugBuffer, "[MASCOT_PROXY_DEBUG] Created UI mascot entity successfully with model: %s\n", pModelKey);
		OutputDebugStringA(debugBuffer);
	}

	NTL_RETURN(pPLCharacter);
}

void CNtlSobMascotProxy::DeleteMascotEntity(void)
{
	if (m_pCharacter)
	{
		m_pCharacter->RemoveWorld();
		GetSceneManager()->DeleteEntity(m_pCharacter);
		m_pCharacter = NULL;
	}
}

// UI mascot status window creation and deletion
void CNtlSobMascotProxy::CreateUIMascotStatusWndCharacter(void)
{
	OutputDebugStringA("[MASCOT_PROXY_DEBUG] CreateUIMascotStatusWndCharacter - Entry\n");
	
	// Create the UI character structure
	m_pUIMascotStatusWnd = NTL_NEW SUICharacter;
	
	// FINAL SOLUTION: Use SYNCHRONOUS loading for UI mascot entities
	// Bypass the async scheduling system which has timing issues with character entities
	OutputDebugStringA("[MASCOT_SYNC_LOAD] Using SYNCHRONOUS loading for UI mascot (like tools do)\n");
	
	// First create the entity structure
	m_pUIMascotStatusWnd->pPLCharacter = CreateMascotEntity(TRUE);
	if (!m_pUIMascotStatusWnd->pPLCharacter)
	{
		OutputDebugStringA("[MASCOT_SYNC_LOAD] Failed to create mascot entity\n");
		NTL_DELETE(m_pUIMascotStatusWnd);
		m_pUIMascotStatusWnd = NULL;
		return;
	}
	
	OutputDebugStringA("[MASCOT_SYNC_LOAD] Mascot entity created, now loading resources synchronously\n");
	
	// Get the model information for synchronous loading
	CNtlSobMascot* pMascot = (CNtlSobMascot*)m_pSobObj;
	const char* pModelKey = pMascot->GetMascotAttr()->GetModelName();
	
	// Get character property to build file path
	CNtlPLProperty* pProperty = CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_CHARACTER, pModelKey);
	CNtlPLCharacterProperty* pCharProperty = (CNtlPLCharacterProperty*)pProperty;
	
	// Build clump file path
	std::string strClumpFileName = pCharProperty->GetBaseMeshFilePath();
	strClumpFileName += pCharProperty->GetBaseMeshFileName();
	
	// Use the same texture paths as character entities
	std::string strTexPath = ".\\TEXTURE\\CHARACTER\\;";
	strTexPath += ".\\TEXTURE\\ITEM\\";
	
	char debugBuffer[512];
	sprintf_s(debugBuffer, "[MASCOT_SYNC_LOAD] Loading synchronously: %s\n", strClumpFileName.c_str());
	OutputDebugStringA(debugBuffer);
	sprintf_s(debugBuffer, "[MASCOT_SYNC_LOAD] Texture paths: %s\n", strTexPath.c_str());
	OutputDebugStringA(debugBuffer);
	
	// CRITICAL: Load the clump resource SYNCHRONOUSLY like tools do
	CNtlPLResource* pResource = CNtlPLResourceManager::GetInstance()->LoadClump(strClumpFileName.c_str(), strTexPath.c_str());
	
	if (pResource)
	{
		OutputDebugStringA("[MASCOT_SYNC_LOAD] Synchronous loading successful, calling CallSchedulingResource\n");
	
		// Manually call CallSchedulingResource to set up the entity
		m_pUIMascotStatusWnd->pPLCharacter->CallSchedulingResource(pResource);
	
		// Verify the resource was set correctly
		RpClump* pClump = m_pUIMascotStatusWnd->pPLCharacter->GetClump();
		RwBool bLoadComplete = m_pUIMascotStatusWnd->pPLCharacter->GetCharScheduleResInfo()->bLoadComplete;
		
		sprintf_s(debugBuffer, "[MASCOT_SYNC_LOAD] After CallSchedulingResource - LoadComplete: %s, Clump: %s\n", 
			bLoadComplete ? "TRUE" : "FALSE", pClump ? "VALID" : "NULL");
		OutputDebugStringA(debugBuffer);
		
		if (bLoadComplete && pClump)
		{
			OutputDebugStringA("[MASCOT_SYNC_LOAD] SUCCESS: Synchronous loading completed successfully!\n");
		}
		else
		{
			OutputDebugStringA("[MASCOT_SYNC_LOAD] WARNING: CallSchedulingResource did not complete setup properly\n");
		}
	}
	else
	{
		sprintf_s(debugBuffer, "[MASCOT_SYNC_LOAD] ERROR: Failed to load clump resource: %s\n", strClumpFileName.c_str());
		OutputDebugStringA(debugBuffer);
	}
		
	// Set character properties like the character proxy does for UI (EXACT SAME PATTERN)
	if (m_pUIMascotStatusWnd && m_pUIMascotStatusWnd->pPLCharacter)
	{
		m_pUIMascotStatusWnd->pPLCharacter->SetVisible(TRUE);
		
		RwUInt32 uiPLFlags = m_pUIMascotStatusWnd->pPLCharacter->GetFlags();
		m_pUIMascotStatusWnd->pPLCharacter->SetFlags(uiPLFlags | NTL_PLEFLAG_DUMMY);
		
		RwV3d vUIPos;
		CNtlMath::MathRwV3dAssign(&vUIPos, 0.0f, 0.0f, 0.0f);
		m_pUIMascotStatusWnd->pPLCharacter->SetPosition(&vUIPos);
		
		// CRITICAL: Set up animation system for UI mascot as well
		RpClump* pUIClump = m_pUIMascotStatusWnd->pPLCharacter->GetClump();
		if (pUIClump)
		{
			OutputDebugStringA("[MASCOT_UI_ANIMATION] Setting up animation system for UI mascot\n");
		
			// Get character property for animation table setup
			CNtlSobMascot* pMascot = (CNtlSobMascot*)m_pSobObj;
			const char* pModelKey = pMascot->GetMascotAttr()->GetModelName();
			CNtlPLProperty* pProperty = CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_CHARACTER, pModelKey);
			CNtlPLCharacterProperty* pCharProperty = (CNtlPLCharacterProperty*)pProperty;
			
			if (pCharProperty && pCharProperty->GetAnimTable())
				{
				// Debug animation table info
				std::string animPath = pCharProperty->GetAnimTable()->GetAnimPath();
				char debugBuffer[512];
				sprintf_s(debugBuffer, "[MASCOT_UI_ANIMATION] Animation table path: %s\n", animPath.c_str());
				OutputDebugStringA(debugBuffer);
				
				// Check if the specific animation exists
				RwBool bAnimExists = m_pUIMascotStatusWnd->pPLCharacter->IsExistAnim(NML_IDLE_LOOP);
				sprintf_s(debugBuffer, "[MASCOT_UI_ANIMATION] NML_IDLE_LOOP animation exists: %s\n", bAnimExists ? "YES" : "NO");
				OutputDebugStringA(debugBuffer);
				
				// The UI character entity should handle its own animation system
				// Just call SetBaseAnimation and let the character handle animation loading
				RwBool bAnimSet = m_pUIMascotStatusWnd->pPLCharacter->SetBaseAnimation(NML_IDLE_LOOP, 0.0f, TRUE);
				sprintf_s(debugBuffer, "[MASCOT_UI_ANIMATION] SetBaseAnimation result: %s\n", bAnimSet ? "SUCCESS" : "FAILED");
				OutputDebugStringA(debugBuffer);
				
				if (bAnimSet)
								{
					OutputDebugStringA("[MASCOT_UI_ANIMATION] UI mascot animation set - should now be animated!\n");
							}
							else
							{
					OutputDebugStringA("[MASCOT_UI_ANIMATION] ERROR: Failed to set base animation - animation files might be missing\n");
					
					// Try to check what animations are available
					OutputDebugStringA("[MASCOT_UI_ANIMATION] Animation troubleshooting:\n");
					sprintf_s(debugBuffer, "[MASCOT_UI_ANIMATION] - Animation table path: %s\n", animPath.c_str());
					OutputDebugStringA(debugBuffer);
					sprintf_s(debugBuffer, "[MASCOT_UI_ANIMATION] - Looking for NML_IDLE_LOOP (%u) animation file\n", NML_IDLE_LOOP);
					OutputDebugStringA(debugBuffer);
					OutputDebugStringA("[MASCOT_UI_ANIMATION] - Check if animation files exist in the path above\n");
				}
		}
		else
		{
				OutputDebugStringA("[MASCOT_UI_ANIMATION] ERROR: Could not get character property for UI animation\n");
		}
	}
	else
	{
			OutputDebugStringA("[MASCOT_UI_ANIMATION] ERROR: UI clump not available for animation setup\n");
		}
		
		OutputDebugStringA("[MASCOT_SYNC_LOAD] Character properties set successfully\n");
	}
	
	m_bUIMascotStatusWndInitialized = FALSE;
	
	OutputDebugStringA("[MASCOT_PROXY_DEBUG] CreateUIMascotStatusWndCharacter - Exit\n");
}

void CNtlSobMascotProxy::RecreateUIMascotStatusWndCharacter(void)
{
	NTL_FUNCTION("CNtlSobMascotProxy::RecreateUIMascotStatusWndCharacter");

	// Delete existing UI character
	DeleteUIMascotStatusWndCharacter();

	// Create new UI character with updated model
	CreateUIMascotStatusWndCharacter();

	NTL_RETURNVOID();
}

void CNtlSobMascotProxy::DeleteUIMascotStatusWndCharacter(void)
{
	if (m_pUIMascotStatusWnd)
	{
		if (m_pUIMascotStatusWnd->pPLCharacter)
		{
			m_pUIMascotStatusWnd->pPLCharacter->RemoveWorld();
			GetSceneManager()->DeleteEntity(m_pUIMascotStatusWnd->pPLCharacter);
			m_pUIMascotStatusWnd->pPLCharacter = NULL;
		}
		
		NTL_DELETE(m_pUIMascotStatusWnd);
		m_pUIMascotStatusWnd = NULL;
	}
	
	m_bUIMascotStatusWndInitialized = FALSE;
}

void CNtlSobMascotProxy::CreateUIMascotStatusWndCamera(void)
{
	if (!m_pUIMascotStatusWnd || !m_pUIMascotStatusWnd->pPLCharacter)
		return;
	
	// Create the camera using the same approach as character proxy
	m_pUIMascotStatusBarCamera = NTL_NEW CNtlPLCameraRenderTexture;
	m_pUIMascotStatusBarCamera->Create(128, 128, 128, 128);
	
	// MASCOT-SPECIFIC CAMERA SETUP: Mascots are smaller than characters, need different camera positioning
	RwReal fHeight = m_pUIMascotStatusWnd->pPLCharacter->GetHeight();
	RwReal fWidth = m_pUIMascotStatusWnd->pPLCharacter->GetWidth();
	RwReal fDepth = m_pUIMascotStatusWnd->pPLCharacter->GetDepth();
	
	// Position mascot at origin and scale appropriately for full visibility
	RwV3d vMascotPos = {0.0f, 0.0f, 0.0f};
	m_pUIMascotStatusWnd->pPLCharacter->SetPosition(&vMascotPos);
	m_pUIMascotStatusWnd->pPLCharacter->SetScale(1.5f); // Slightly smaller for full visibility
	
	// Set up camera to show the full mascot (positioned to see entire model)
	RwV3d vLookAt = {0.0f, fHeight * 0.4f, 0.0f}; // Look slightly lower than center
	RwV3d vCamPos = {0.0f, fHeight * 0.4f, fHeight * 3.0f}; // Move camera further back
	
	m_pUIMascotStatusBarCamera->SetCameraPosition(&vCamPos, &vLookAt);
}

void CNtlSobMascotProxy::DeleteUIMascotStatusWndCamera(void)
{
	if (m_pUIMascotStatusBarCamera)
	{
		m_pUIMascotStatusBarCamera->Destroy();
		NTL_DELETE(m_pUIMascotStatusBarCamera);
		m_pUIMascotStatusBarCamera = NULL;
	}
}

// Update functions
void CNtlSobMascotProxy::UpdateResourceLoadCheck(RwReal fElapsed)
{
	if (m_pCharacter && !m_bResLoaded)
	{
		// Check if the character's clump is loaded
		RpClump* pClump = m_pCharacter->GetClump();
		RwBool bLoadComplete = m_pCharacter->GetCharScheduleResInfo()->bLoadComplete;
		
		if (pClump && bLoadComplete)
		{
			
			// Complete animation system setup now that clump is loaded
			if (!m_pBaseHierarchy)
			{
				// Get hierarchy from the clump (like character proxy does)
				m_pBaseHierarchy = Helper_GetHierarchyClump(pClump);
				if (m_pBaseHierarchy)
				{
					// Set hierarchy flags (following character proxy pattern exactly)
					Helper_SetHierarchyClumpSkin(pClump, m_pBaseHierarchy);
					
					RpHAnimHierarchySetFlags(m_pBaseHierarchy,
						(RpHAnimHierarchyFlag)(RpHAnimHierarchyGetFlags(m_pBaseHierarchy) | 
						rpHANIMHIERARCHYUPDATELTMS | 
						rpHANIMHIERARCHYUPDATEMODELLINGMATRICES));
					
					RpHAnimHierarchyAttach(m_pBaseHierarchy);
					RpHAnimUpdateHierarchyMatrices(m_pBaseHierarchy);
					
					// Set default idle animation (like character proxy does)
					SetBaseAnimation(NML_IDLE_LOOP, TRUE, 0.0f);
				}
			}
			
		m_bResLoaded = TRUE;
		ResourceLoadComplete(TRUE);
		}
	}
}

void CNtlSobMascotProxy::UpdateGUIUpdateCheck(RwReal fElapsed)
{
	// Update UI rotation if needed
	if (m_pUIMascotStatusRotate)
	{
		if (m_pUIMascotStatusRotate->bLeftRot)
		{
			m_pUIMascotStatusRotate->fAngle -= 90.0f * fElapsed;
		}
		else if (m_pUIMascotStatusRotate->bRightRot)
		{
			m_pUIMascotStatusRotate->fAngle += 90.0f * fElapsed;
		}
	}
}

void CNtlSobMascotProxy::UpdateMascotFollowing(RwReal fElapsed)
{
	// Only update following if the main world mascot exists and resources are loaded
	if (!m_pCharacter || !m_bResLoaded)
		return;
		
	// Get the avatar (player)
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if (!pAvatar)
		return;
	
	// Get current positions
	RwV3d vPlayerPos = pAvatar->GetPosition();
	RwV3d vMascotPos = m_pCharacter->GetPosition();
	
	// Calculate distance between player and mascot
	RwV3d vDistance;
	vDistance.x = vPlayerPos.x - vMascotPos.x;
	vDistance.y = 0.0f; // Ignore Y difference for following
	vDistance.z = vPlayerPos.z - vMascotPos.z;
	RwReal fDistance = sqrtf(vDistance.x * vDistance.x + vDistance.z * vDistance.z);
	
	// Define following parameters
	const RwReal fFollowDistance = 3.0f;      // Desired distance from player
	const RwReal fMaxDistance = 8.0f;         // Max distance before teleporting
	const RwReal fMoveSpeed = 8.0f;           // Movement speed
	
	// If too far, teleport or move closer
	if (fDistance > fMaxDistance)
	{
		// Teleport mascot near player if too far
		RwV3d vNewPos = vPlayerPos;
		vNewPos.x += 2.0f;  // Position to the right
		vNewPos.y += 1.2f;  // Add height offset to ensure mascot appears above ground
		vNewPos.z += 1.0f;  // Position slightly forward
		m_pCharacter->SetPosition(&vNewPos);
		
		char debugBuffer[256];
		sprintf_s(debugBuffer, "[MASCOT_FOLLOW] Teleported to player - distance was %.1f\n", fDistance);
		OutputDebugStringA(debugBuffer);
	}
	else if (fDistance > fFollowDistance)
	{
		// Move mascot toward player smoothly
		RwV3d vDirection;
		RwV3dNormalize(&vDirection, &vDistance);
		
		// Calculate new position (manual vector scaling)
		RwV3d vMoveVector;
		vMoveVector.x = vDirection.x * fMoveSpeed * fElapsed;
		vMoveVector.y = vDirection.y * fMoveSpeed * fElapsed;
		vMoveVector.z = vDirection.z * fMoveSpeed * fElapsed;
		
		RwV3d vNewPos;
		vNewPos.x = vMascotPos.x + vMoveVector.x;
		vNewPos.y = vPlayerPos.y + 1.2f;  // Keep proper height offset (don't fly, but stay above ground)
		vNewPos.z = vMascotPos.z + vMoveVector.z;
		
		m_pCharacter->SetPosition(&vNewPos);
		
		// Make mascot face the direction it's moving
		RwV3d vFaceDirection = vDirection;
		m_pCharacter->SetDirection(&vFaceDirection);
	}
}

void CNtlSobMascotProxy::ResourceLoadComplete(RwBool bVisible)
{
	if (m_pCharacter)
	{
		// CRITICAL: Make sure mascot appears in world when summoned
		m_pCharacter->SetVisible(bVisible);
		
		// Position mascot near player (like pets do)
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		if (pAvatar)
		{
			RwV3d vPlayerPos = pAvatar->GetPosition();
			RwV3d vMascotPos = vPlayerPos;
			vMascotPos.x += 2.0f; // Position mascot 2 units to the right of player
			vMascotPos.y += 1.2f; // Add height offset to ensure mascot appears above ground
			vMascotPos.z += 1.0f; // And 1 unit forward
			
			m_pCharacter->SetPosition(&vMascotPos);
			
			char debugBuffer[256];
			sprintf_s(debugBuffer, "[MASCOT_WORLD_DEBUG] Mascot positioned at (%.3f, %.3f, %.3f)\n", 
				vMascotPos.x, vMascotPos.y, vMascotPos.z);
			OutputDebugStringA(debugBuffer);
		}
		

	}

	if (m_pUIMascotStatusWnd)
	{
		m_pUIMascotStatusWnd->pPLCharacter->SetVisible(bVisible);
	}
}

// UI rendering (following character proxy pattern exactly)
RwTexture* CNtlSobMascotProxy::UIPcStatusBarRender(void)
{
	// Follow character proxy pattern exactly - simple and clean
	if (m_pUIMascotStatusBarCamera == NULL)
		return NULL;

	// Check if UI character exists and has valid clump before rendering
	if (m_pUIMascotStatusWnd == NULL || m_pUIMascotStatusWnd->pPLCharacter == NULL)
		return NULL;
					
	// Check if the character clump is loaded (key fix for mascot texture)
					RpClump* pClump = m_pUIMascotStatusWnd->pPLCharacter->GetClump();
	if (pClump == NULL)
		return NULL;
	m_pUIMascotStatusBarCamera->RenderTexture(m_pUIMascotStatusWnd->pPLCharacter);

	return m_pUIMascotStatusBarCamera->GetTexture();
}

// Transform functions
void CNtlSobMascotProxy::SetPosition(const RwV3d *pPos)
{
	if (m_pCharacter)
		m_pCharacter->SetPosition(pPos);
}

void CNtlSobMascotProxy::SetDirection(const RwV3d *pDir)
{
	if (m_pCharacter)
		m_pCharacter->SetDirection(pDir);
}

void CNtlSobMascotProxy::SetAngleY(RwReal fAngle)
{
	if (m_pCharacter)
		m_pCharacter->SetAngleY(fAngle);
}

void CNtlSobMascotProxy::SetDeltaAngleY(RwReal fAngle)
{
	if (m_pCharacter)
		m_pCharacter->SetAngleY(m_pCharacter->GetAngleY() + fAngle);
}

void CNtlSobMascotProxy::SetAngleX(RwReal fAngle)
{
	if (m_pCharacter)
		m_pCharacter->SetAngleX(fAngle);
}

void CNtlSobMascotProxy::SetDeltaAngleX(RwReal fAngle)
{
	if (m_pCharacter)
		m_pCharacter->SetAngleX(m_pCharacter->GetAngleX() + fAngle);
}

void CNtlSobMascotProxy::SetScale(RwReal fScale)
{
	if (m_pCharacter)
		m_pCharacter->SetScale(fScale);
}

// Material functions
void CNtlSobMascotProxy::SetAlpha(RwUInt8 byAlpha)
{
	if (m_pCharacter)
		m_pCharacter->SetAlpha(byAlpha);
}

void CNtlSobMascotProxy::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if (m_pCharacter)
		m_pCharacter->SetColor(byRed, byGreen, byBlue);
}

void CNtlSobMascotProxy::SetInkThicknessWeight(RwReal fWeightValue)
{
	// Ink thickness weight is not directly supported by character class
	// This would need to be implemented differently
}

void CNtlSobMascotProxy::SetInkThickness(RwReal fValue)
{
	if (m_pCharacter)
		m_pCharacter->SetInkThickness(fValue);
}

void CNtlSobMascotProxy::SetInkColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if (m_pCharacter)
		m_pCharacter->SetInkColor(byRed, byGreen, byBlue);
}

// Animation functions - Following character proxy pattern exactly
void CNtlSobMascotProxy::SetBaseAnimation(RwUInt32 uiAnimKey, RwBool bLoop, RwReal fStartTime)
{
	if(m_pCharacter == NULL)
		return;

	// CRITICAL: Update UI mascot animation like character proxy does
	if(m_pUIMascotStatusWnd)
	{
		if(uiAnimKey == NML_IDLE_LOOP || uiAnimKey == NML_STAFF_IDLE_LOOP || uiAnimKey == NML_DGUN_IDLE_LOOP)
		{
			RwUInt32 uiCurrTargetAnimKey = m_pUIMascotStatusWnd->pPLCharacter->GetCurBaseAnimKey();
			if(uiAnimKey != uiCurrTargetAnimKey)
			{
				// Set animation on UI character - this enables automatic animation
				m_pUIMascotStatusWnd->pPLCharacter->SetBaseAnimation(uiAnimKey, 0.0f, TRUE);
			}
		}
	}

	// Set animation on main mascot character (standard approach)
		m_pCharacter->SetBaseAnimation(uiAnimKey, fStartTime, bLoop);
}

RwBool CNtlSobMascotProxy::IsExistBaseAnimation(RwUInt32 uiAnimKey)
{
	if (m_pCharacter)
		return m_pCharacter->IsExistAnim(uiAnimKey);
	return FALSE;
}

RwUInt32 CNtlSobMascotProxy::GetBaseAnimationKey(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetCurBaseAnimKey();
	return 0;
}

RwBool CNtlSobMascotProxy::IsBaseAnimationEnd(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetBaseCurrentAnimEnd();
	return FALSE;
}

void CNtlSobMascotProxy::SetAnimSpeed(RwReal fSpeed)
{
	if (m_pCharacter)
		m_pCharacter->SetAnimSpeed(fSpeed);
}

RwReal CNtlSobMascotProxy::GetAnimSpeed(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetAnimSpeed();
	return 1.0f;
}

void CNtlSobMascotProxy::SetAnimBlendNoneOneTime(void)
{
	m_byAnimBlendNoneOneTime = 1;
}

// Entity functions
CNtlPLEntity* CNtlSobMascotProxy::GetPLMainEntity(void)
{
	return m_pCharacter;
}

RwV3d CNtlSobMascotProxy::GetBonePosition(const RwChar *pBoneName)
{
	if (m_pCharacter)
	{
		RwMatrix* pMat = m_pCharacter->GetBoneMatrix(pBoneName);
		if (pMat)
			return pMat->pos;
	}
	
	RwV3d vPos = {0.0f, 0.0f, 0.0f};
	return vPos;
}

RwReal CNtlSobMascotProxy::GetPLEntityHeight(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetHeight();
	return 0.0f;
}

RwReal CNtlSobMascotProxy::GetPLEntityWidth(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetWidth();
	return 0.0f;
}

RwReal CNtlSobMascotProxy::GetPLEntityDepth(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetDepth();
	return 0.0f;
}

RwReal CNtlSobMascotProxy::GetPLEntityBaseScale(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetBaseScale();
	return 1.0f;
}

RwReal CNtlSobMascotProxy::GetScale(void)
{
	if (m_pCharacter)
		return m_pCharacter->GetScale();
	return 1.0f;
}

void CNtlSobMascotProxy::ChangeModel(CNtlPLCharacter* pCharacter, RwBool bDeleteOrgModel)
{
	if (m_pCharacter && bDeleteOrgModel)
	{
		DeleteMascotEntity();
	}
	
	m_pCharacter = pCharacter;
	
	if (m_pCharacter)
	{
		m_pCharacter->AddWorld();
	}
}

void CNtlSobMascotProxy::ChangeUIModel(CNtlPLCharacter* pCharacter)
{
	if (m_pUIMascotStatusWnd)
	{
		DeleteUIMascotStatusWndCharacter();
	}
	
	// Create new UI character structure
	m_pUIMascotStatusWnd = NTL_NEW SUICharacter;
	m_pUIMascotStatusWnd->pPLCharacter = pCharacter;
	
	if (m_pUIMascotStatusWnd && m_pUIMascotStatusWnd->pPLCharacter)
	{
		m_pUIMascotStatusWnd->pPLCharacter->AddWorld();
	}
}

// UI rotation functions
void CNtlSobMascotProxy::MascotStatusRotateLeft(void)
{
	if (m_pUIMascotStatusRotate)
	{
		m_pUIMascotStatusRotate->bLeftRot = TRUE;
		m_pUIMascotStatusRotate->bRightRot = FALSE;
	}
}

void CNtlSobMascotProxy::MascotStatusRotateRight(void)
{
	if (m_pUIMascotStatusRotate)
	{
		m_pUIMascotStatusRotate->bLeftRot = FALSE;
		m_pUIMascotStatusRotate->bRightRot = TRUE;
	}
}

void CNtlSobMascotProxy::MascotStatusRotateStop(void)
{
	if (m_pUIMascotStatusRotate)
	{
		m_pUIMascotStatusRotate->bLeftRot = FALSE;
		m_pUIMascotStatusRotate->bRightRot = FALSE;
	}
}

void CNtlSobMascotProxy::MascotStatusRotateReset(void)
{
	if (m_pUIMascotStatusRotate)
	{
		m_pUIMascotStatusRotate->fAngle = 0.0f;
	}
}

void CNtlSobMascotProxy::MascotStatusRotate(RwReal fDelta)
{
	if (m_pUIMascotStatusRotate && m_pUIMascotStatusWnd)
	{
		if (m_pUIMascotStatusRotate->bLeftRot)
		{
			m_pUIMascotStatusRotate->fAngle += fDelta;
		}
		else if (m_pUIMascotStatusRotate->bRightRot)
		{
			m_pUIMascotStatusRotate->fAngle -= fDelta;
		}
		
		m_pUIMascotStatusWnd->pPLCharacter->SetAngleY(m_pUIMascotStatusRotate->fAngle);
	}
}

// World functions
void CNtlSobMascotProxy::AddWorld(void)
{
	if (m_pCharacter)
		m_pCharacter->AddWorld();
}

void CNtlSobMascotProxy::RemoveWorld(void)
{
	if (m_pCharacter)
		m_pCharacter->RemoveWorld();
}

// Visibility functions
void CNtlSobMascotProxy::EnableVisible(RwBool bEnable)
{
	if (m_pCharacter)
		m_pCharacter->SetVisible(bEnable);
}

void CNtlSobMascotProxy::EnablePicking(RwBool bEnable)
{
	if (m_pCharacter)
		m_pCharacter->SetPicking(bEnable);
}

void CNtlSobMascotProxy::EnableShadowRendering(RwBool bEnable)
{
	// Shadow rendering is not directly supported by character class
	// This would need to be implemented differently
}

void CNtlSobMascotProxy::EnableHighlight(RwBool bEnable)
{
	// Highlight is not directly supported by character class
	// This would need to be implemented differently
}

void CNtlSobMascotProxy::CreateElapsedController(RwReal fCtrlTime, RwReal fWeightValue)
{
	CNtlSobProxy::CreateElapsedController(fCtrlTime, fWeightValue);
}

void CNtlSobMascotProxy::DeleteElapsedController(void)
{
	CNtlSobProxy::DeleteElapsedController();
}

// Focus functions
void CNtlSobMascotProxy::SetFocus(void)
{
	// Handle focus gained
	// This would need to be implemented based on the specific requirements
}

void CNtlSobMascotProxy::ReleaseFocus(void)
{
	// Handle focus lost
	// This would need to be implemented based on the specific requirements
}

// Effect creation functions
CNtlPLEntity* CNtlSobMascotProxy::CreatePLChildEffect(const RwChar *pKey, const RwChar *pAttachBoneName, RwV3d vOffset, RwBool bIgnoreVisible)
{
	CNtlPLEntity *pPLEntity = CNtlSobProxy::CreatePLChildEffect(pKey, bIgnoreVisible);
	if (!pPLEntity || !m_pCharacter || !pAttachBoneName)
		return pPLEntity;
	Helper_AttachBone(m_pCharacter, pPLEntity, pAttachBoneName, vOffset);
	return pPLEntity;
}

CNtlPLEntity* CNtlSobMascotProxy::CreatePLChildEffect(const RwChar *pKey, RwV3d vOffset, RwBool bIgnoreVisible, RwBool bApplyRotate)
{
	CNtlPLEntity *pPLEntity = CNtlSobProxy::CreatePLChildEffect(pKey, bIgnoreVisible);
	if (!pPLEntity || !m_pCharacter)
		return pPLEntity;
	Helper_AttachWorldPos(m_pCharacter, pPLEntity, vOffset, bApplyRotate);
	return pPLEntity;
}

// State notification functions
void CNtlSobMascotProxy::NotifyStateEnter(RwUInt32 uiStateId)
{
	// Handle state enter notification
	// This would need to be implemented based on the specific requirements
}

void CNtlSobMascotProxy::NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState)
{
	// Handle state exit notification
	// This would need to be implemented based on the specific requirements
}

// Proxy system visibility
void CNtlSobMascotProxy::EnableProxySystemVisible(RwBool bVisible)
{
	OutputDebugStringA(("[MASCOT_PROXY_DEBUG] EnableProxySystemVisible: " + std::string(bVisible ? "TRUE" : "FALSE") + "\n").c_str());
	
	if (m_pUIMascotStatusWnd)
	{
		m_pUIMascotStatusWnd->pPLCharacter->SetVisible(bVisible);
	}
}



RwBool CNtlSobMascotProxy::IsResourceLoaded(void)
{
	return m_bResLoaded;
} 