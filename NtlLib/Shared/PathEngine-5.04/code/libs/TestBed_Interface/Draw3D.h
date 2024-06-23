//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DRAW3D_INCLUDED
#define DRAW3D_INCLUDED

#include "libs/Geometry/interface/tPoint_Header.h"
//#include "tpoint3_header.h"
#include "libs/Geometry/interface/Vector3F.h"

class iGraphicsInterface;
class cCollisionShape;

void Draw3DLine(iGraphicsInterface* gip, const cVector3F& start, const cVector3F& end);
void Draw3DCross(iGraphicsInterface* gip, const cVector3F& p, float size);

//void DrawAgent(iGraphicsInterface* gip, const tPoint& p, long z, const cCollisionShape& agent, long height);
//void DrawAgentArrow(iGraphicsInterface* gip, const tPoint& p, float direction, long z, float size, long height);
//
//void DrawAgentWithPrecision(iGraphicsInterface* gip, const tPoint& p, long z, const cCollisionShape& agent, long height, float precisionX, float precisionY);
//void DrawAgentArrowWithPrecision(iGraphicsInterface* gip, const tPoint& p, float direction, long z, float size, long height, float precisionX, float precisionY);

class cPosition;
class iShape;
void DrawAgent(iGraphicsInterface* gip, const cPosition& p, long z, const iShape* shape, long height);
void DrawAgentArrow(iGraphicsInterface* gip, const cPosition& p, float direction, long z, float size, long height);
void DrawAgentWithPrecision(iGraphicsInterface* gip, const cPosition& p, long z, const iShape* shape, long height, float precisionX, float precisionY);
void DrawAgentArrowWithPrecision(iGraphicsInterface* gip, const cPosition& p, float direction, long z, float size, long height, float precisionX, float precisionY);

#endif
