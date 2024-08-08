#include "StdAfx.h"
#include <Math.h>
#include "NtlMovement.h"
#include "NtlVector.h"

#define NTL_SQRT_OF_2		((float)(1.4142135623730950488016887242097))
#define NTL_PI		((float)3.1415926535897932384626433832795)

bool NtlSin(float fX, float fZ, float* pfSin)
{
	if (0.0f == fX && 0.0f == fZ)
		return false;

	*pfSin = fZ / sqrt(fX * fX + fZ * fZ);
	return true;
}

bool NtlCos(float fX, float fZ, float* pfCos)
{
	if (0.0f == fX && 0.0f == fZ)
		return false;

	*pfCos = fX / sqrt(fX * fX + fZ * fZ);
	return true;
}

bool RotateVector45DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = (NTL_SQRT_OF_2 / 2) * fX + (NTL_SQRT_OF_2 / 2) * fZ;
	*pfResultZ = -(NTL_SQRT_OF_2 / 2) * fX + (NTL_SQRT_OF_2 / 2) * fZ;

	return true;
}

bool RotateVector45DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = (NTL_SQRT_OF_2 / 2) * fX - (NTL_SQRT_OF_2 / 2) * fZ;
	*pfResultZ = (NTL_SQRT_OF_2 / 2) * fX + (NTL_SQRT_OF_2 / 2) * fZ;

	return true;
}

bool RotateVector90DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = fZ;
	*pfResultZ = -fX;

	return true;
}

bool RotateVector90DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -fZ;
	*pfResultZ = fX;

	return true;
}

bool RotateVector135DegreeToLeft(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -(NTL_SQRT_OF_2 / 2) * fX + (NTL_SQRT_OF_2 / 2) * fZ;
	*pfResultZ = -(NTL_SQRT_OF_2 / 2) * fX - (NTL_SQRT_OF_2 / 2) * fZ;

	return true;
}

bool RotateVector135DegreeToRight(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -(NTL_SQRT_OF_2 / 2) * fX - (NTL_SQRT_OF_2 / 2) * fZ;
	*pfResultZ = (NTL_SQRT_OF_2 / 2) * fX - (NTL_SQRT_OF_2 / 2) * fZ;

	return true;
}

bool RotateVector180Degree(float fX, float fZ, float* pfResultX, float* pfResultZ)
{
	*pfResultX = -fX;
	*pfResultZ = -fZ;

	return true;
}

bool RotateVector(float fX, float fZ, float fAngleInRadian, float* pfResultX, float* pfResultZ)
{
	*pfResultX = cos(fAngleInRadian) * fX + sin(fAngleInRadian) * fZ;
	*pfResultZ = -sin(fAngleInRadian) * fX + cos(fAngleInRadian) * fZ;

	return true;
}

bool GetVectorWithDegree(int nDegree, float* pfVectorX, float* pfVectorZ)
{
	if (0 <= nDegree)
	{
		nDegree = nDegree % 360;
	}
	else
	{
		nDegree = -((-nDegree) % 360) + 360;
	}

	float fRadian = NTL_PI * nDegree / 180;

	*pfVectorX = sin(fRadian);
	*pfVectorZ = cos(fRadian);

	return true;
}

CNtlVector& RotateVector45DegreeToLeft(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = (NTL_SQRT_OF_2 / 2) * pVector->x + (NTL_SQRT_OF_2 / 2) * pVector->z;
	vResult.y = 0.0f;
	vResult.z = -(NTL_SQRT_OF_2 / 2) * pVector->x + (NTL_SQRT_OF_2 / 2) * pVector->z;

	return vResult;
}

CNtlVector& RotateVector45DegreeToRight(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = (NTL_SQRT_OF_2 / 2) * pVector->x - (NTL_SQRT_OF_2 / 2) * pVector->z;
	vResult.y = 0.0f;
	vResult.z = (NTL_SQRT_OF_2 / 2) * pVector->x + (NTL_SQRT_OF_2 / 2) * pVector->z;

	return vResult;
}

CNtlVector& RotateVector90DegreeToLeft(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = pVector->z;
	vResult.y = 0.0f;
	vResult.z = -(pVector->x);

	return vResult;
}

CNtlVector& RotateVector90DegreeToRight(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -(pVector->z);
	vResult.y = 0.0f;
	vResult.z = pVector->x;

	return vResult;
}

CNtlVector& RotateVector135DegreeToLeft(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -(NTL_SQRT_OF_2 / 2) * pVector->x + (NTL_SQRT_OF_2 / 2) * pVector->z;
	vResult.y = 0.0f;
	vResult.z = -(NTL_SQRT_OF_2 / 2) * pVector->x - (NTL_SQRT_OF_2 / 2) * pVector->z;

	return vResult;
}

CNtlVector& RotateVector135DegreeToRight(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -(NTL_SQRT_OF_2 / 2) * pVector->x - (NTL_SQRT_OF_2 / 2) * pVector->z;
	vResult.y = 0.0f;
	vResult.z = (NTL_SQRT_OF_2 / 2) * pVector->x - (NTL_SQRT_OF_2 / 2) * pVector->z;

	return vResult;
}

CNtlVector& RotateVector180Degree(CNtlVector* pVector)
{
	static CNtlVector vResult;

	vResult.x = -(pVector->x);
	vResult.y = 0.0f;
	vResult.z = -(pVector->z);

	return vResult;
}

CNtlVector& RotateVector(CNtlVector* pVector, float fAngleInRadian)
{
	static CNtlVector vResult;

	vResult.x = cos(fAngleInRadian) * pVector->x + sin(fAngleInRadian) * pVector->z;
	vResult.y = 0.0f;
	vResult.z = -sin(fAngleInRadian) * pVector->x + cos(fAngleInRadian) * pVector->z;

	return vResult;
}

#include <stdio.h>
bool NtlGetDestination(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
						float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
						float fDestinationX, float fDestinationY, float fDestinationZ,
						BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs,
						float fAttackDistance,
						float* pfNewHeadingVectorX, float* pfNewHeadingVectorZ,
						float* pfDestinationX, float* pfDestinationY, float* pfDestinationZ,
						float fTurningSpeedRatio)
{
	if (0 == fCurrentHeadingVectorX && 0 == fCurrentHeadingVectorZ)
		return false;

	*pfNewHeadingVectorX = fCurrentHeadingVectorX;
	*pfNewHeadingVectorZ = fCurrentHeadingVectorZ;
	*pfDestinationX = fCurrentPositionX;
	*pfDestinationY = fCurrentPositionY;
	*pfDestinationZ = fCurrentPositionZ;

	float fDistanceInTick = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	switch (byMoveDirection)
	{
	case NTL_MOVE_NONE :
		break;

	case NTL_MOVE_F :
	case NTL_MOVE_B :
	// DON'T delete these lines permanently!
	// 완전히 삭제하지 마시오!
	// by YOSHIKI(2006-09-22)
//	case NTL_MOVE_L :
//	case NTL_MOVE_R :
//	case NTL_MOVE_F_L :
//	case NTL_MOVE_F_R :
//	case NTL_MOVE_B_L :
//	case NTL_MOVE_B_R :
		{
			float fMovementVectorX = 0.0f;
			float fMovementVectorZ = 0.0f;

			if (NTL_MOVE_F == byMoveDirection)
			{
				fMovementVectorX = fCurrentHeadingVectorX;
				fMovementVectorZ = fCurrentHeadingVectorZ;
			}
			else if (NTL_MOVE_B == byMoveDirection)
			{
				RotateVector180Degree(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			// DON'T delete these lines permanently!
			// 완전히 삭제하지 마시오!
			// by YOSHIKI(2006-09-22)
/*			else if (NTL_MOVE_L == byMoveDirection)
			{
				RotateVector90DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_R == byMoveDirection)
			{
				RotateVector90DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_F_L == byMoveDirection)
			{
				RotateVector45DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_F_R == byMoveDirection)
			{
				RotateVector45DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_B_L == byMoveDirection)
			{
				RotateVector135DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_B_R == byMoveDirection)
			{
				RotateVector135DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}*/

			float fSin = 0.0f;
			float fCos = 0.0f;

			if (false == NtlSin(fMovementVectorX, fMovementVectorZ, &fSin))
				return true;
			if (false == NtlCos(fMovementVectorX, fMovementVectorZ, &fCos))
				return true;

			float fDeltaX = fDistanceInTick * fCos;
			float fDeltaZ = fDistanceInTick * fSin;
			*pfDestinationX = fCurrentPositionX + fDeltaX;
			*pfDestinationZ = fCurrentPositionZ + fDeltaZ;
		}
		break;

	case NTL_MOVE_TURN_L :
	case NTL_MOVE_TURN_R :
	case NTL_MOVE_F_TURN_L :
	case NTL_MOVE_F_TURN_R :
	case NTL_MOVE_B_TURN_L :
	case NTL_MOVE_B_TURN_R :
		{
			float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
			fRadian *= fTurningSpeedRatio;

			if (NTL_MOVE_TURN_L == byMoveDirection ||
				NTL_MOVE_F_TURN_L == byMoveDirection ||
				NTL_MOVE_B_TURN_L == byMoveDirection)
			{
				RotateVector(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, fRadian, pfNewHeadingVectorX, pfNewHeadingVectorZ);
			}
			else if (NTL_MOVE_TURN_R == byMoveDirection ||
						NTL_MOVE_F_TURN_R == byMoveDirection ||
						NTL_MOVE_B_TURN_R == byMoveDirection)
			{
				RotateVector(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, -fRadian, pfNewHeadingVectorX, pfNewHeadingVectorZ);
			}

			if (NTL_MOVE_TURN_L == byMoveDirection || NTL_MOVE_TURN_R == byMoveDirection)
			{
				// The position doesn't change.
			}
			else if (NTL_MOVE_F_TURN_L == byMoveDirection || NTL_MOVE_F_TURN_R == byMoveDirection ||
						NTL_MOVE_B_TURN_L == byMoveDirection || NTL_MOVE_B_TURN_R == byMoveDirection)
			{
				float fRadius = 0.0f;
				fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

				float fTempX = fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);
				float fTempZ = fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);

				if (NTL_MOVE_F_TURN_L == byMoveDirection)
				{
					*pfDestinationX = fCurrentPositionX + fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
				else if (NTL_MOVE_F_TURN_R == byMoveDirection)
				{
					*pfDestinationX = fCurrentPositionX + fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
				else if (NTL_MOVE_B_TURN_L == byMoveDirection)
				{
					*pfDestinationX = fCurrentPositionX - fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
				else if (NTL_MOVE_B_TURN_R == byMoveDirection)
				{
					*pfDestinationX = fCurrentPositionX - fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
			}
		}
		break;

	case NTL_MOVE_MOUSE_MOVEMENT :
		{
			float fDeltaX = fDestinationX - fCurrentPositionX;
			float fDeltaZ = fDestinationZ - fCurrentPositionZ;

			//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
			if ( ( fabs( fDeltaX ) < 0.001f ) && ( fabs( fDeltaZ ) < 0.001f ) )
			{
				*pfNewHeadingVectorX = fCurrentHeadingVectorX;
				*pfNewHeadingVectorZ = fCurrentHeadingVectorZ;
				*pfDestinationX = fDestinationX;
				*pfDestinationY = fDestinationY;
				*pfDestinationZ = fDestinationZ;

				return true;
			}

			CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
			float fDeltaLength = vDelta.Length();

			vDelta.Normalize(fDeltaLength);

			*pfNewHeadingVectorX = vDelta.x;
			*pfNewHeadingVectorZ = vDelta.z;

			if (fDeltaLength <= fDistanceInTick)
			{
				*pfDestinationX = fDestinationX;
				*pfDestinationY = fDestinationY;
				*pfDestinationZ = fDestinationZ;
			}
			else
			{
				vDelta *= fDistanceInTick;

				*pfDestinationX = fCurrentPositionX + vDelta.x;
				*pfDestinationY = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceInTick / fDeltaLength;
				*pfDestinationZ = fCurrentPositionZ + vDelta.z;
			}
		}
		break;

	case NTL_MOVE_FOLLOW_MOVEMENT :
		{
			float fDeltaX = fDestinationX - fCurrentPositionX;
			float fDeltaZ = fDestinationZ - fCurrentPositionZ;

			//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
			if ( ( fabs( fDeltaX ) < 0.001f ) && ( fabs( fDeltaZ ) < 0.001f ) )
			{
				*pfNewHeadingVectorX = fCurrentHeadingVectorX;
				*pfNewHeadingVectorZ = fCurrentHeadingVectorZ;
				*pfDestinationX = fCurrentPositionX;
				*pfDestinationY = fCurrentPositionY;
				*pfDestinationZ = fCurrentPositionZ;

				return true;
			}

			CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
			float fDeltaLength = vDelta.Length();

			vDelta.Normalize(fDeltaLength);

			*pfNewHeadingVectorX = vDelta.x;
			*pfNewHeadingVectorZ = vDelta.z;

			if ( fDeltaLength <= fAttackDistance )
			{
				*pfDestinationX = fCurrentPositionX;
				*pfDestinationY = fCurrentPositionY;
				*pfDestinationZ = fCurrentPositionZ;
			}
			else if ( fDeltaLength > fAttackDistance && fDeltaLength < fAttackDistance + fDistanceInTick)
			{
				vDelta *= ( fDeltaLength - fAttackDistance * 0.99f );

				*pfDestinationX = fCurrentPositionX + vDelta.x;
				*pfDestinationY = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * (fDeltaLength - fAttackDistance * 0.99f) / fDeltaLength;
				*pfDestinationY = fDestinationY;
				*pfDestinationZ = fCurrentPositionZ + vDelta.z;
			}
			else
			{
				vDelta *= fDistanceInTick;

				*pfDestinationX = fCurrentPositionX + vDelta.x;
				*pfDestinationY = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceInTick / fDeltaLength;
				*pfDestinationZ = fCurrentPositionZ + vDelta.z;
			}
		}
		break;

	default :
		return false;
		break;
	}

	return true;
}

void NtlGetDestination_Keyboard(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	typedef void (*MOVEMENT_KEYBOARD_FUNC)(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

	static bool bIsInitialized = false;
	static MOVEMENT_KEYBOARD_FUNC g_pMovementKeyboardFunc[NTL_MOVE_COUNT];

	if (false == bIsInitialized)
	{
		g_pMovementKeyboardFunc[NTL_MOVE_NONE] = NtlGetDestination_Keyboard_NONE;

		g_pMovementKeyboardFunc[NTL_MOVE_F] = NtlGetDestination_Keyboard_F;
		g_pMovementKeyboardFunc[NTL_MOVE_B] = NtlGetDestination_Keyboard_B;
		// DON'T delete these lines permanently!
		// 완전히 삭제하지 마시오!
		// by YOSHIKI(2006-09-22)
//		g_pMovementKeyboardFunc[NTL_MOVE_L] = NtlGetDestination_Keyboard_L;
//		g_pMovementKeyboardFunc[NTL_MOVE_R] = NtlGetDestination_Keyboard_R;

//		g_pMovementKeyboardFunc[NTL_MOVE_F_L] = NtlGetDestination_Keyboard_F_L;
//		g_pMovementKeyboardFunc[NTL_MOVE_F_R] = NtlGetDestination_Keyboard_F_R;
//		g_pMovementKeyboardFunc[NTL_MOVE_B_L] = NtlGetDestination_Keyboard_B_L;
//		g_pMovementKeyboardFunc[NTL_MOVE_B_R] = NtlGetDestination_Keyboard_B_R;

		g_pMovementKeyboardFunc[NTL_MOVE_TURN_L] = NtlGetDestination_Keyboard_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_TURN_R] = NtlGetDestination_Keyboard_TURN_R;

		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_L] = NtlGetDestination_Keyboard_F_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_R] = NtlGetDestination_Keyboard_F_TURN_R;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_L] = NtlGetDestination_Keyboard_B_TURN_L;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_R] = NtlGetDestination_Keyboard_B_TURN_R;

		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_L_JUMP] = NtlGetDestination_Keyboard_F;
		g_pMovementKeyboardFunc[NTL_MOVE_F_TURN_R_JUMP] = NtlGetDestination_Keyboard_F;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_L_JUMP] = NtlGetDestination_Keyboard_B;
		g_pMovementKeyboardFunc[NTL_MOVE_B_TURN_R_JUMP] = NtlGetDestination_Keyboard_B;

		bIsInitialized = true;
	}

	if(g_pMovementKeyboardFunc[byMoveDirection] == NULL)
	{
		_ASSERTE(0);
		return;
	}

	g_pMovementKeyboardFunc[byMoveDirection](fCurrentHeadingVectorX, fCurrentHeadingVectorZ, fSpeedInSecs,
												fCurrentPositionX, fCurrentPositionY, fCurrentPositionZ,
												dwDeltaTimeInMillisecs, fTurningSpeedRatio,
												pNewHeadingVector, pDestination);
}

void NtlGetDestination_Keyboard_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( dwDeltaTimeInMillisecs );
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;
}

void NtlGetDestination_Keyboard_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += *pNewHeadingVector * fDistanceToGo;
}

void NtlGetDestination_Keyboard_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = (fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE) * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToLeft(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToRight(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_F_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector45DegreeToLeft(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_F_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector45DegreeToRight(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_B_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = (fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE) * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector135DegreeToLeft(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_B_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fDistanceToGo = (fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE) * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector135DegreeToRight(pNewHeadingVector) * fDistanceToGo;
}

void NtlGetDestination_Keyboard_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);
}

void NtlGetDestination_Keyboard_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);
}

void NtlGetDestination_Keyboard_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	float fRadius = 0.0f;
	fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);
	float fTempZ = fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);

	pDestination->x = fCurrentPositionX + fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian));
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
}

void NtlGetDestination_Keyboard_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	float fRadius = 0.0f;
	fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);
	float fTempZ = fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);

	pDestination->x = fCurrentPositionX + fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian));
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
}

void NtlGetDestination_Keyboard_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	float fRadius = 0.0f;
	fRadius = (fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE) * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);
	float fTempZ = fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);

	pDestination->x = fCurrentPositionX - fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian));
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
}

void NtlGetDestination_Keyboard_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
											float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
											DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
											CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	float fRadius = 0.0f;
	fRadius = (fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE) * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

	float fTempX = fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);
	float fTempZ = fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);

	pDestination->x = fCurrentPositionX - fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian));
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
}

void NtlGetDestination_Jump(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fJumpDirectionX, float fJumpDirectionZ,
								BYTE byMoveDirection, DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	typedef void (*MOVEMENT_JUMP_FUNC)(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fJumpDirectionX, float fJumpDirectionZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination);

	static bool bIsInitialized = false;
	static MOVEMENT_JUMP_FUNC g_pMovementJumpFunc[NTL_MOVE_COUNT];

	if (false == bIsInitialized)
	{
		g_pMovementJumpFunc[NTL_MOVE_NONE] = NtlGetDestination_Jump_NONE;

		g_pMovementJumpFunc[NTL_MOVE_F] = NtlGetDestination_Jump_F;
		g_pMovementJumpFunc[NTL_MOVE_B] = NtlGetDestination_Jump_B;
		// DON'T delete these lines permanently!
		// 완전히 삭제하지 마시오!
		// by YOSHIKI(2006-09-22)
//		g_pMovementJumpFunc[NTL_MOVE_L] = NtlGetDestination_Jump_L;
//		g_pMovementJumpFunc[NTL_MOVE_R] = NtlGetDestination_Jump_R;

//		g_pMovementJumpFunc[NTL_MOVE_F_L] = NtlGetDestination_Jump_F_L;
//		g_pMovementJumpFunc[NTL_MOVE_F_R] = NtlGetDestination_Jump_F_R;
//		g_pMovementJumpFunc[NTL_MOVE_B_L] = NtlGetDestination_Jump_B_L;
//		g_pMovementJumpFunc[NTL_MOVE_B_R] = NtlGetDestination_Jump_B_R;

		g_pMovementJumpFunc[NTL_MOVE_TURN_L] = NtlGetDestination_Jump_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_TURN_R] = NtlGetDestination_Jump_TURN_R;

		g_pMovementJumpFunc[NTL_MOVE_F_TURN_L_JUMP] = NtlGetDestination_Jump_F_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_F_TURN_R_JUMP] = NtlGetDestination_Jump_F_TURN_R;
		g_pMovementJumpFunc[NTL_MOVE_B_TURN_L_JUMP] = NtlGetDestination_Jump_B_TURN_L;
		g_pMovementJumpFunc[NTL_MOVE_B_TURN_R_JUMP] = NtlGetDestination_Jump_B_TURN_R;

		bIsInitialized = true;
	}

	if(g_pMovementJumpFunc[byMoveDirection] == NULL)
	{
		//- yoshiki : Find out why this can happen while a character is jumping!
//		_ASSERTE(0);
		return;
	}

	g_pMovementJumpFunc[byMoveDirection](fCurrentHeadingVectorX, fCurrentHeadingVectorZ, fSpeedInSecs,
											fCurrentPositionX, fCurrentPositionY, fCurrentPositionZ,
											fJumpDirectionX, fJumpDirectionZ,
											dwDeltaTimeInMillisecs, fTurningSpeedRatio,
											pNewHeadingVector, pDestination);
}

void NtlGetDestination_Jump_NONE(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( dwDeltaTimeInMillisecs );
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorX );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorZ );
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;
}

void NtlGetDestination_Jump_F(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vInitialHeading * fDistanceToGo;
}

void NtlGetDestination_Jump_B(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = (fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE) * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToLeft(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
								DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fTurningSpeedRatio );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector90DegreeToRight(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorX );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorZ );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);
}

void NtlGetDestination_Jump_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
									float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
									float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
									DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
									CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	UNREFERENCED_PARAMETER( fSpeedInSecs );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorX );
	UNREFERENCED_PARAMETER( fInitialHeadingVectorZ );

	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);
}

void NtlGetDestination_Jump_F_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vInitialHeading * fDistanceToGo;
}

void NtlGetDestination_Jump_F_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += vInitialHeading * fDistanceToGo;
}

void NtlGetDestination_Jump_B_TURN_L(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Jump_B_TURN_R(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
										float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
										float fInitialHeadingVectorX, float fInitialHeadingVectorZ,
										DWORD dwDeltaTimeInMillisecs, float fTurningSpeedRatio,
										CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	pNewHeadingVector->x = fCurrentHeadingVectorX;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = fCurrentHeadingVectorZ;

	float fRadian = 2 * NTL_PI * ((float)dwDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);
	fRadian *= fTurningSpeedRatio;

	*pNewHeadingVector = RotateVector(pNewHeadingVector, -fRadian);

	pDestination->x = fCurrentPositionX;
	pDestination->y = fCurrentPositionY;
	pDestination->z = fCurrentPositionZ;

	CNtlVector vInitialHeading(fInitialHeadingVectorX, 0, fInitialHeadingVectorZ);

	float fDistanceToGo = fSpeedInSecs * NTL_BACKWARD_MOVEMENT_SPEED_RATE * (float)dwDeltaTimeInMillisecs / 1000.0f;

	*pDestination += RotateVector180Degree(&vInitialHeading) * fDistanceToGo;
}

void NtlGetDestination_Mouse(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
	if ((fabs(fDeltaX) < 0.001f) && (fabs(fDeltaZ) < 0.001f))
	{
		pNewHeadingVector->x = fCurrentHeadingVectorX;
		pNewHeadingVector->y = 0.0f;
		pNewHeadingVector->z = fCurrentHeadingVectorZ;

		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;

		return;
	}

	CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	vDelta.Normalize(fDeltaLength);

	pNewHeadingVector->x = vDelta.x;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = vDelta.z;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	if (fDeltaLength <= fDistanceToGo)
	{
		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;
	}
	else
	{
		vDelta *= fDistanceToGo;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceToGo / fDeltaLength;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
}

void NtlGetDestination_Follow(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
								float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
								float fDestinationX, float fDestinationY, float fDestinationZ,
								DWORD dwDeltaTimeInMillisecs,
								float fTargetDistance,
								CNtlVector* pNewHeadingVector, CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
	if ((fabs(fDeltaX) < 0.001f) && (fabs(fDeltaZ) < 0.001f))
	{
		pNewHeadingVector->x = fCurrentHeadingVectorX;
		pNewHeadingVector->y = 0.0f;
		pNewHeadingVector->z = fCurrentHeadingVectorZ;

		pDestination->x = fCurrentPositionX;
		pDestination->y = fCurrentPositionY;
		pDestination->z = fCurrentPositionZ;

		return;
	}

	CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	vDelta.Normalize(fDeltaLength);

	pNewHeadingVector->x = vDelta.x;
	pNewHeadingVector->y = 0.0f;
	pNewHeadingVector->z = vDelta.z;

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	if (fDeltaLength <= fTargetDistance)
	{
		pDestination->x = fCurrentPositionX;
		pDestination->y = fCurrentPositionY;
		pDestination->z = fCurrentPositionZ;
	}
	else if (fDeltaLength > fTargetDistance && fDeltaLength < fTargetDistance + fDistanceToGo)
	{
		vDelta *= fDeltaLength - fTargetDistance * 0.99f;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * (fDeltaLength - fTargetDistance * 0.99f) / fDeltaLength;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
	else
	{
		vDelta *= fDistanceToGo;

		pDestination->x = fCurrentPositionX + vDelta.x;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceToGo / fDeltaLength;
		pDestination->z = fCurrentPositionZ + vDelta.z;
	}
}

void NtlGetDestination_Dash(float fCurrentMoveVectorX, float fCurrentMoveVectorZ, float fSpeedInSecs,
							float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
							float fDestinationX, float fDestinationY, float fDestinationZ,
							DWORD dwDeltaTimeInMillisecs,
							CNtlVector* pDestination)
{
	float fDeltaX = fDestinationX - fCurrentPositionX;
	float fDeltaZ = fDestinationZ - fCurrentPositionZ;

	//  [6/21/2006 zeroera] : 수정 : float 비교 오차 범위
	if ((fabs(fDeltaX) < 0.001f) && (fabs(fDeltaZ) < 0.001f))
	{
		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;

		return;
	}

	float fDistanceToGo = fSpeedInSecs * (float)dwDeltaTimeInMillisecs / 1000.0f;

	CNtlVector vDelta(fDeltaX, 0, fDeltaZ);
	float fDeltaLength = vDelta.Length();

	if (fDeltaLength <= fDistanceToGo)
	{
		pDestination->x = fDestinationX;
		pDestination->y = fDestinationY;
		pDestination->z = fDestinationZ;
	}
	else
	{
		pDestination->x = fCurrentPositionX + fCurrentMoveVectorX * fDistanceToGo;
		pDestination->y = fCurrentPositionY + (fDestinationY - fCurrentPositionY) * fDistanceToGo / fDeltaLength;
		pDestination->z = fCurrentPositionZ + fCurrentMoveVectorZ * fDistanceToGo;
	}
}

float NtlGetDistance(float fPositionX1, float fPositionZ1, float fPositionX2, float fPositionZ2)
{
	return sqrt((fPositionX1 - fPositionX2) * (fPositionX1 - fPositionX2) + (fPositionZ1 - fPositionZ2) * (fPositionZ1 - fPositionZ2));
}

bool NtlIsInsideOrNot(float fPositionX,float fPositionZ, float fDestinationX, float fDestinationZ, float fInputX, float fInputZ)
{
	if( ( (fInputX - fPositionX) * (fInputX - fDestinationX) ) <= 0 && 
		( (fInputZ - fPositionZ) * (fInputZ - fDestinationZ) ) <= 0 )
		return true;
	
	return false;
}

#undef NTL_SQRT_OF_2
#undef NTL_PI