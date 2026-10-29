#include "stdafx.h"
#include "BotAiAction_EscortFollow.h"
#include "BotPathFinder.h"


// the escorted npc walks after its target like a summoned pet: it stops close to the target and moves again
// once the target walked off (the following state alone never moved the npc for the client)
#define ESCORT_FOLLOW_END_DISTANCE		2.0f
#define ESCORT_FOLLOW_BEGIN_DISTANCE	4.0f


CBotAiAction_EscortFollow::CBotAiAction_EscortFollow(CNpc* pBot, CNtlVector& rLoc, float fRadius, bool bRunMode, DWORD eventID)
:CBotAiAction(pBot, BOTCONTROL_ACTION_ESCORT_FOLLOW, "BOTCONTROL_ACTION_ESCORT_FOLLOW")
{
	m_dwFollowTime = 0;
	m_dwArriveWaitTime = 0;
	m_dwStopTime = 0;
	m_hTarget = INVALID_HOBJECT;
	m_vDestLoc = rLoc;
	m_fRadius = fRadius;
	m_bRunMode = bRunMode;
	m_eventID = eventID;
	m_bArrived = false;
	m_bMoving = false;
	m_bIsPathFindingUsed = false;
}

CBotAiAction_EscortFollow::~CBotAiAction_EscortFollow()
{
}


void CBotAiAction_EscortFollow::OnEnter()
{
	m_dwArriveWaitTime = 0;
	m_dwStopTime = 0;
	m_dwFollowTime = CNpc::CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS; // move on the first update
}

void CBotAiAction_EscortFollow::OnExit()
{
	GetBot()->SendCharStateStanding();

	if (m_bIsPathFindingUsed)
		GetBot()->GetPathFinder()->Reset();
}

void CBotAiAction_EscortFollow::Stop()
{
	if (m_bMoving)
		GetBot()->SendCharStateStanding();

	m_bMoving = false;
	m_bIsPathFindingUsed = false;
}

bool CBotAiAction_EscortFollow::MoveTowards(CCharacter* pTarget)
{
	m_hTarget = pTarget->GetID();
	m_bMoving = true;

	// same movement as CBotAiAction_PetFollow
	int nPathFindingResult = 0;
	BYTE nextStateId = GetBot()->ConsiderPathMovement(pTarget, &nPathFindingResult);

	if (nextStateId == CHARSTATE_STANDING)
	{
		if (nPathFindingResult)
		{
			GetBot()->SendCharStateStanding();
		}
		else
		{
			CNtlVector rvNewDir;
			CNtlVector rvNewLoc;
			CNtlVector rvNewDestLoc;
			float rfActualSpeed = 0.0f;
			std::list<sVECTOR3> rlistConsumedDestLoc;

			pTarget->PredictMovement(CNpc::CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS, rvNewDir, rvNewLoc, rvNewDestLoc, &rlistConsumedDestLoc, &rfActualSpeed);

			sVECTOR3 destLoc;
			rvNewLoc.CopyTo(destLoc);
			GetBot()->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, -1, 1, &destLoc, m_bRunMode);
		}

		m_bIsPathFindingUsed = false;
	}
	else if (nextStateId == CHARSTATE_DESTMOVE)
	{
		if (nPathFindingResult == 2)
		{
			sVECTOR3 avDestLoc[DBO_MAX_NEXT_DEST_LOC_COUNT];
			BYTE byNextLocCount = GetBot()->GetPathFinder()->GetAllNextNavLoc(avDestLoc);

			GetBot()->SendCharStateDestMove(false, CNtlVector::INVALID_XZ, -1, byNextLocCount, avDestLoc, m_bRunMode);
		}
		else if (nPathFindingResult != 1)
			return false;

		m_bIsPathFindingUsed = true;
	}
	else if (nextStateId == CHARSTATE_FOLLOWING)
	{
		if (GetBot()->GetCharStateID() != CHARSTATE_FOLLOWING)
		{
			sVECTOR3 destLoc;
			pTarget->GetCurLoc().CopyTo(destLoc);
			GetBot()->SendCharStateFollowing(pTarget->GetID(), ESCORT_FOLLOW_END_DISTANCE, DBO_MOVE_FOLLOW_FRIENDLY, destLoc, m_bRunMode);
		}

		m_bIsPathFindingUsed = false;
	}

	return true;
}

int CBotAiAction_EscortFollow::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	CCharacter* pTarget = GetBot()->GetEscortManager()->GetCurrentTarget();
	if (pTarget)
	{
		m_dwStopTime = 0;
		if (m_bArrived)
		{
			m_dwArriveWaitTime = UnsignedSafeIncrease<DWORD>(m_dwArriveWaitTime, dwTickDiff);
			if (GetBot()->GetEscortManager()->GetTargetCount() == 0)
			{
				m_status = COMPLETED;
				return m_status;
			}

			if (m_dwArriveWaitTime >= 60000)
			{
				GetBot()->GetBotController()->ChangeControlState_Leave();
				m_status = FAILED;
				return m_status;
			}

		}
		else
		{
			if (GetBot()->IsInRange(m_vDestLoc, m_fRadius))
			{
				GetBot()->GetEscortManager()->SendEvent(TS_EVENT_SEND_TYPE_PARTY, 0, m_eventID); // 0 = TS_TYPE_QUEST_CS
				GetBot()->SendCharStateStanding();
				m_bArrived = true;
				m_bMoving = false;
			}
			else
			{
				m_dwFollowTime = UnsignedSafeIncrease<DWORD>(m_dwFollowTime, dwTickDiff);

				if (m_dwFollowTime >= CNpc::CONSIDERING_NEXT_MOVEMENT_INTERVAL_IN_MILLISECS && pTarget->GetCurWorld())
				{
					m_dwFollowTime = 0;

					if (GetBot()->IsInRange(pTarget, ESCORT_FOLLOW_END_DISTANCE))
						Stop();
					else if (m_bMoving || m_hTarget != pTarget->GetID() || GetBot()->IsInRange(pTarget, ESCORT_FOLLOW_BEGIN_DISTANCE) == false)
						MoveTowards(pTarget);
				}
			}
		}

		if (UpdateSubControlList(dwTickDiff, fMultiple) == CHANGED)
			m_status = INACTIVATED;
	}
	else
	{

		if (!m_bArrived)
			GetBot()->GetBotController()->ChangeControlState_Leave();

		m_status = COMPLETED;
	}

	return m_status;
}
