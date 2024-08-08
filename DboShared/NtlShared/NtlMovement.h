#pragma once

#include "NtlSharedCommon.h"

#define NTL_MAX_RADIUS_OF_VISIBLE_AREA		(100.0f)
#define NTL_MAX_NUMBER_OF_PLAYERS_IN_VISIBLE_AREA		(100)
#define NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS		(3000)
#define NTL_DEFAULT_MOVEMENT_SPEED_F		(7.0f)
#define NTL_DEFAULT_MOVEMENT_SPEED_B		(3.0f)

enum ENtlMovementDirection
{
	NTL_MOVE_NONE = 0x0000,

	NTL_MOVE_F = 0x0001,		// Forward
	NTL_MOVE_B = 0x0002,		// Backward
	NTL_MOVE_L = 0x0003,		// Left
	NTL_MOVE_R = 0x0004,		// Right

	NTL_MOVE_F_L = 0x0005,		// Forward + Left
	NTL_MOVE_F_R = 0x0006,		// Forward + Right
	NTL_MOVE_B_L = 0x0007,		// Backward + Left
	NTL_MOVE_B_R = 0x0008,		// Backward + Right

	NTL_MOVE_TURN_L = 0x0009,		// Turning Left
	NTL_MOVE_TURN_R = 0x000A,		// Turning Right

	NTL_MOVE_F_TURN_L = 0x000B,		// Forward + Turning Left
	NTL_MOVE_F_TURN_R = 0x000C,		// Forward + Turning Right
	NTL_MOVE_B_TURN_L = 0x000D,		// Backward + Turning Left
	NTL_MOVE_B_TURN_R = 0x000E,		// Backward + Turning Right

	NTL_MOVE_MOUSE_MOVEMENT = 0x000F,		// Mouse Movement
	NTL_MOVE_FOLLOW_MOVEMENT = 0x0010,		// Follow Movement

	NTL_MOVE_UNKNOWN = 0xFFFF,

	NTL_MOVE_FIRST = NTL_MOVE_NONE,
	NTL_MOVE_LAST = NTL_MOVE_FOLLOW_MOVEMENT
};
enum ENtlMovementStatus
{
	NTL_MOVE_STATUS_NONE     = 0x00,

	NTL_MOVE_STATUS_JUMP     = 0x01,		// JUMPING
	NTL_MOVE_STATUS_SITDOWN  = 0x02,		// SITDOWN
	NTL_MOVE_STATUS_SWIMMING = 0x03,		// SWIMMING

	NTL_MOVE_STATUS_UNKNOWN =  0xFF,

	NTL_MOVE_STATUS_FIRST = NTL_MOVE_STATUS_NONE,
	NTL_MOVE_STATUS_LAST = NTL_MOVE_STATUS_SWIMMING
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
bool GetVectorWithDegree(Int32 iDegree, float* pfVectorX, float* pfVectorZ);

bool NtlGetDestination(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
						float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
						float fDestinationX, float fDestinationY, float fDestinationZ,
						ENtlMovementDirection eDirection, UInt32 uiDeltaTimeInMillisecs,
						float fAttackDistance,
						float* pfNewHeadingVectorX, float* pfNewHeadingVectorZ,
						float* pfDestinationX, float* pfDestinationY, float* pfDestinationZ);

float NtlGetDistance(float fPositionX1, float fPositionZ1, float fPositionX2, float fPositionZ2);
float NtlGetRandomPosition(float fPosition1, float fPosition2);
bool NtlIsInsideOrNot(float fPositionX,float fPositionZ, float fDestX, float fDestZ, float fInputX, float fInputZ);
