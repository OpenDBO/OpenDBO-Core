#pragma once

#include "NtlVector.h"

#define NTL_MAX_RADIUS_OF_VISIBLE_AREA		(100.0f)
#define NTL_MAX_NUMBER_OF_PLAYERS_IN_VISIBLE_AREA		(100)
#define NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS		(3000)
#define NTL_DEFAULT_MOVEMENT_SPEED		(7.0f)
#define NTL_BACKWARD_MOVEMENT_SPEED_RATE		(0.5f)		// 50% of forward movement speed
#define NTL_DEFAULT_MOVEMENT_SPEED_B		(NTL_DEFAULT_MOVEMENT_SPEED * NTL_BACKWARD_MOVEMENT_SPEED_RATE)

#define NTL_DASH_DISTANCE_F		(15.0f)
#define NTL_DASH_DISTANCE_B		(10.0f)
#define NTL_DASH_DISTANCE_L		(10.0f)
#define NTL_DASH_DISTANCE_R		(10.0f)
#define NTL_DASH_SPEED			(25.0f)
#define NTL_MAX_DASH_DISTANCE_FOR_SKILL		(25.0f)
#define NTL_ACTIVE_DASH_SPEED				(40.0f)
#define DBO_DASH_INVERVAL_WITHOUT_EP_LOSS_IN_MILLISECS						(4000)

#define DBO_VEHICLE_TURNING_RATIO		(1.0f)		// no turning speed change when pc is on the vehicle
#define DBO_SWIMMING_SPEED_RATIO		(0.5f)		// 50% of movement speed on land.
#define DBO_FALLING_SPEED_RATIO			(0.5f)		// 50% of movement speed on land.

#define DBO_DISTANCE_CHECK_TOLERANCE		(10.0f)

const BYTE DBO_MAX_NEXT_DEST_LOC_COUNT = 10;

enum ENtlMovementDirection
{
	NTL_MOVE_NONE = 0,

	NTL_MOVE_F,		// Forward
	NTL_MOVE_B,		// Backward
	// DON'T delete these lines permanently!
	// 완전히 삭제하지 마시오!
	// by YOSHIKI(2006-09-22)
//	NTL_MOVE_L,		// Left
//	NTL_MOVE_R,		// Right

//	NTL_MOVE_F_L,		// Forward + Left
//	NTL_MOVE_F_R,		// Forward + Right
//	NTL_MOVE_B_L,		// Backward + Left
//	NTL_MOVE_B_R,		// Backward + Right

	NTL_MOVE_TURN_L,		// Turning Left
	NTL_MOVE_TURN_R,		// Turning Right

	NTL_MOVE_F_TURN_L,		// Forward + Turning Left
	NTL_MOVE_F_TURN_R,		// Forward + Turning Right
	NTL_MOVE_B_TURN_L,		// Backward + Turning Left
	NTL_MOVE_B_TURN_R,		// Backward + Turning Right

	NTL_MOVE_F_TURN_L_JUMP,		// Forward + Turning Left + Jump
	NTL_MOVE_F_TURN_R_JUMP,		// Forward + Turning Right + Jump
	NTL_MOVE_B_TURN_L_JUMP,		// Backward + Turning Left + Jump
	NTL_MOVE_B_TURN_R_JUMP,		// Backward + Turning Right + Jump

	NTL_MOVE_MOUSE_MOVEMENT,		// Mouse Movement
	NTL_MOVE_FOLLOW_MOVEMENT,		// Follow Movement

	NTL_MOVE_DASH_F,				// Dash + Forward
	NTL_MOVE_DASH_B,				// Dash + Backward
	NTL_MOVE_DASH_L,				// Dash + Left
	NTL_MOVE_DASH_R,				// Dash + Right

	NTL_MOVE_COUNT,

	NTL_MOVE_UNKNOWN = 0xFF,

	NTL_MOVE_KEYBOARD_FIRST = NTL_MOVE_F,
	NTL_MOVE_KEYBOARD_LAST = NTL_MOVE_B_TURN_R_JUMP,

	NTL_MOVE_DASH_PASSIVE_FIRST = NTL_MOVE_DASH_F,
	NTL_MOVE_DASH_PASSIVE_LAST = NTL_MOVE_DASH_R,

	NTL_MOVE_FIRST = NTL_MOVE_NONE,
	NTL_MOVE_LAST = NTL_MOVE_DASH_R
};

enum ENtlMovementFlag
{
	NTL_MOVE_FLAG_RUN = 0x01 << 0
};

enum ENtlMovementStatus
{
	NTL_MOVE_STATUS_NONE = 0,

	NTL_MOVE_STATUS_JUMP = 1,			// JUMPING
	NTL_MOVE_STATUS_FALLING = 2,		// FALLING

	NTL_MOVE_STATUS_UNKNOWN = 0xFFui8,

	NTL_MOVE_STATUS_FIRST = NTL_MOVE_STATUS_NONE,
	NTL_MOVE_STATUS_LAST = NTL_MOVE_STATUS_FALLING
};

bool NtlSin(float fX, float fZ, float* pfSin);
bool NtlCos(float fX, float fZ, float* pfCos);

bool RotateVector45DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector45DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector90DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector90DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector135DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector135DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector180Degree(float fX, float fZ, float* pfResultX, float* pfResultZ);
bool RotateVector(float fX, float fZ, float fAngleInRadian, float* pfResultX, float* pfResultZ);
bool GetVectorWithDegree(int nDegree, float* pfVectorX, float* pfVectorZ);

CNtlVector& RotateVector45DegreeToLeft(CNtlVector* pVector);
CNtlVector& RotateVector45DegreeToRight(CNtlVector* pVector);
CNtlVector& RotateVector90DegreeToLeft(CNtlVector* pVector);
CNtlVector& RotateVector90DegreeToRight(CNtlVector* pVector);
CNtlVector& RotateVector135DegreeToLeft(CNtlVector* pVector);
CNtlVector& RotateVector135DegreeToRight(CNtlVector* pVector);
CNtlVector& RotateVector180Degree(CNtlVector* pVector);
CNtlVector& RotateVector(CNtlVector* pVector, float fAngleInRadian);

bool NtlGetDestination(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
						float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
						float fDestinationX, float fDestinationY, float fDestinationZ,
						BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs,
						float fAttackDistance,
						float* pfNewHeadingVectorX, float* pfNewHeadingVectorZ,
						float* pfDestinationX, float* pfDestinationY, float* pfDestinationZ,
						float fTurningSpeedRatio);

// It's assumed that byMoveDirecton is valid. Please check if it has valid value before calling this function.
// by YOSHIKI(2006-07-06)
void NtlGetDestination_Keyboard(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Keyboard_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Jump(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fJumpDirectionX, float fJumpDirectionZ,
							BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
							CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fJumpDirectionX, float fJumpDirectionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fJumpDirectionX, float fJumpDirectionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fJumpDirectionX, float fJumpDirectionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);
void NtlGetDestination_Jump_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Mouse(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Follow(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								float fTargetDistance,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

void NtlGetDestination_Dash(float fCurrentMoveVectorX, float fCurrentMoveVectorZ, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fDestinationX, float fDestinationY, float fDestinationZ,
							DWORD dwDeltaTimeInMillisecs,
							CNtlVector* pDestination);

float NtlGetDistance(float fPositionX1, float fPositionZ1, float fPositionX2, float fPositionZ2);
float NtlGetRandomPosition(float fPosition1, float fPosition2);
bool NtlIsInsideOrNot(float fPositionX,float fPositionZ, float fDestX, float fDestZ, float fInputX, float fInputZ);