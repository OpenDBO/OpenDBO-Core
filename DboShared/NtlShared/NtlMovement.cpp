#include "stdafx.h"
#include "NtlGame.h"
#include <Math.h>
//#include <stdio.h>

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

bool GetVectorWithDegree(Int32 iDegree, float* pfVectorX, float* pfVectorZ)
{
	if (0 <= iDegree)
	{
		iDegree = iDegree % 360;
	}
	else
	{
		iDegree = -((-iDegree) % 360) + 360;
	}

	float fRadian = NTL_PI * iDegree / 180;

	*pfVectorX = sin(fRadian);
	*pfVectorZ = cos(fRadian);

	return true;
}

bool NtlGetDestination(float fCurrentHeadingVectorX, float fCurrentHeadingVectorZ, float fSpeedInSecs,
						float fCurrentPositionX, float fCurrentPositionY, float fCurrentPositionZ,
						float fDestinationX, float fDestinationY, float fDestinationZ,
						ENtlMovementDirection eDirection, UInt32 uiDeltaTimeInMillisecs,
						float fAttackDistance,
						float* pfNewHeadingVectorX, float* pfNewHeadingVectorZ,
						float* pfDestinationX, float* pfDestinationY, float* pfDestinationZ)
{
	if (0 == fCurrentHeadingVectorX && 0 == fCurrentHeadingVectorZ)
		return false;

	*pfNewHeadingVectorX = fCurrentHeadingVectorX;
	*pfNewHeadingVectorZ = fCurrentHeadingVectorZ;
	*pfDestinationX = fCurrentPositionX;
	*pfDestinationY = fCurrentPositionY;
	*pfDestinationZ = fCurrentPositionZ;

	switch (eDirection)
	{
	case NTL_MOVE_NONE :
		break;

	case NTL_MOVE_F :
	case NTL_MOVE_B :
	case NTL_MOVE_L :
	case NTL_MOVE_R :
	case NTL_MOVE_F_L :
	case NTL_MOVE_F_R :
	case NTL_MOVE_B_L :
	case NTL_MOVE_B_R :
		{
			float fMovementVectorX = 0.0f;
			float fMovementVectorZ = 0.0f;

			if (NTL_MOVE_F == eDirection)
			{
				fMovementVectorX = fCurrentHeadingVectorX;
				fMovementVectorZ = fCurrentHeadingVectorZ;
			}
			else if (NTL_MOVE_B == eDirection)
			{
				RotateVector180Degree(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_L == eDirection)
			{
				RotateVector90DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_R == eDirection)
			{
				RotateVector90DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_F_L == eDirection)
			{
				RotateVector45DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_F_R == eDirection)
			{
				RotateVector45DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_B_L == eDirection)
			{
				RotateVector135DegreeToLeft(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}
			else if (NTL_MOVE_B_R == eDirection)
			{
				RotateVector135DegreeToRight(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, &fMovementVectorX, &fMovementVectorZ);
			}

			float fSin = 0.0f;
			float fCos = 0.0f;

			if (false == NtlSin(fMovementVectorX, fMovementVectorZ, &fSin))
				return true;
			if (false == NtlCos(fMovementVectorX, fMovementVectorZ, &fCos))
				return true;

			float fDeltaX = fSpeedInSecs * ((float)uiDeltaTimeInMillisecs / (float)1000) * fCos;
			float fDeltaZ = fSpeedInSecs * ((float)uiDeltaTimeInMillisecs / (float)1000) * fSin;
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
			float fRadian = 2 * NTL_PI * ((float)uiDeltaTimeInMillisecs / (float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS);

			if (NTL_MOVE_TURN_L == eDirection ||
				NTL_MOVE_F_TURN_L == eDirection ||
				NTL_MOVE_B_TURN_L == eDirection)
			{
				RotateVector(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, fRadian, pfNewHeadingVectorX, pfNewHeadingVectorZ);
			}
			else if (NTL_MOVE_TURN_R == eDirection ||
						NTL_MOVE_F_TURN_R == eDirection ||
						NTL_MOVE_B_TURN_R == eDirection)
			{
				RotateVector(fCurrentHeadingVectorX, fCurrentHeadingVectorZ, -fRadian, pfNewHeadingVectorX, pfNewHeadingVectorZ);
			}

			if (NTL_MOVE_TURN_L == eDirection || NTL_MOVE_TURN_R == eDirection)
			{
				// The position doesn't change.
			}
			else if (NTL_MOVE_F_TURN_L == eDirection || NTL_MOVE_F_TURN_R == eDirection ||
						NTL_MOVE_B_TURN_L == eDirection || NTL_MOVE_B_TURN_R == eDirection)
			{
				float fRadius = 0.0f;
				fRadius = fSpeedInSecs * ((float)NTL_REQUIRED_TIME_FOR_COMPLETE_CIRCULAR_MOVEMENT_IN_MILLISECS / (float)1000) / (2 * NTL_PI);

				float fTempX = fCurrentHeadingVectorX * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);
				float fTempZ = fCurrentHeadingVectorZ * fRadius / sqrt(fCurrentHeadingVectorX * fCurrentHeadingVectorX + fCurrentHeadingVectorZ * fCurrentHeadingVectorZ);

				if (NTL_MOVE_F_TURN_L == eDirection)
				{
					*pfDestinationX = fCurrentPositionX + fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
				else if (NTL_MOVE_F_TURN_R == eDirection)
				{
					*pfDestinationX = fCurrentPositionX + fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ + fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
				else if (NTL_MOVE_B_TURN_L == eDirection)
				{
					*pfDestinationX = fCurrentPositionX - fTempX * sin(fRadian) - fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
				else if (NTL_MOVE_B_TURN_R == eDirection)
				{
					*pfDestinationX = fCurrentPositionX - fTempX * sin(fRadian) + fTempZ * (1 - cos(fRadian));
					*pfDestinationZ = fCurrentPositionZ - fTempZ * sin(fRadian) - fTempX * (1 - cos(fRadian));
				}
			}
		}
		break;

	case NTL_MOVE_MOUSE_MOVEMENT :
	case NTL_MOVE_FOLLOW_MOVEMENT :
		{
			float fMovementVectorX = fDestinationX - fCurrentPositionX;
			float fMovementVectorZ = fDestinationZ - fCurrentPositionZ;

			if (0.0f == fMovementVectorX && 0.0f == fMovementVectorZ)
			{
				*pfNewHeadingVectorX = fCurrentHeadingVectorX;
				*pfNewHeadingVectorZ = fCurrentHeadingVectorZ;
				*pfDestinationX = fDestinationX;
				*pfDestinationY = fDestinationY;
				*pfDestinationZ = fDestinationZ;

				return true;
			}

			if (NTL_MOVE_FOLLOW_MOVEMENT == eDirection)
			{
				if (fAttackDistance >= NtlGetDistance(fCurrentPositionX, fCurrentPositionZ, fDestinationX, fDestinationZ))
				{
					*pfNewHeadingVectorX = fMovementVectorX;
					*pfNewHeadingVectorZ = fMovementVectorZ;
					*pfDestinationX = fCurrentPositionX;
					*pfDestinationY = fCurrentPositionY;
					*pfDestinationZ = fCurrentPositionZ;

					return true;
				}

				float fAttackDistanceX = 0.0f;
				float fAttackDistanceZ = 0.0f;
				NtlCos(fDestinationX - fCurrentPositionX, fDestinationZ - fCurrentPositionZ, &fAttackDistanceX);
				NtlSin(fDestinationX - fCurrentPositionX, fDestinationZ - fCurrentPositionZ, &fAttackDistanceZ);
				fAttackDistanceX = fAttackDistance * fAttackDistanceX / 2;
				fAttackDistanceZ = fAttackDistance * fAttackDistanceZ / 2;

				fDestinationX = fDestinationX - fAttackDistanceX;
				fDestinationZ = fDestinationZ - fAttackDistanceZ;
			}

			float fSin = 0.0f;
			float fCos = 0.0f;

			if (false == NtlSin(fMovementVectorX, fMovementVectorZ, &fSin))
				return false;
			if (false == NtlCos(fMovementVectorX, fMovementVectorZ, &fCos))
				return false;

			*pfNewHeadingVectorX = fMovementVectorX;
			*pfNewHeadingVectorZ = fMovementVectorZ;

			float fDistanceToGo = NtlGetDistance(fDestinationX, fDestinationZ, fCurrentPositionX, fCurrentPositionZ);
			if (fDistanceToGo <= fSpeedInSecs * ((float)uiDeltaTimeInMillisecs / 1000))
			{
				*pfDestinationX = fDestinationX;
				*pfDestinationY = fDestinationY;
				*pfDestinationZ = fDestinationZ;
			}
			else
			{
				float fDeltaX = fSpeedInSecs * ((float)uiDeltaTimeInMillisecs / 1000) * fCos;
				float fDeltaZ = fSpeedInSecs * ((float)uiDeltaTimeInMillisecs / 1000) * fSin;

				*pfDestinationX = fCurrentPositionX + fDeltaX;
				*pfDestinationZ = fCurrentPositionZ + fDeltaZ;
			}

//			if (NTL_MOVE_FOLLOW_MOVEMENT == eDirection)
//			{
//				printf("YOSHIKI : NTL_MOVE_FOLLOW_MOVEMENT, *pfDestinationX = %f, *pfDestinationZ = %f\n", *pfDestinationX, *pfDestinationZ);
//			}
		}
		break;

	default :
		return false;
		break;
	}

	return true;
}

float NtlGetDistance(float fPositionX1, float fPositionZ1, float fPositionX2, float fPositionZ2)
{
	return sqrt((fPositionX1 - fPositionX2) * (fPositionX1 - fPositionX2) + (fPositionZ1 - fPositionZ2) * (fPositionZ1 - fPositionZ2));
}

float NtlGetRandomPosition(float fPosition1, float fPosition2)
{
	float fRand = 0; 
	float fDiff = abs(fPosition1 - fPosition2);	
	int iTemp = (int)(fDiff * 100);
	// woody
	//if(iTemp)
//	fRand = (float)((rand() % iTemp)) / 100;
	//else 
	fRand = 0;

	float fRet = (fPosition1 > fPosition2 ? fPosition2 + fRand : fPosition1 + fRand);
	return fRet;
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