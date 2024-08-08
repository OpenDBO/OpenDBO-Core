//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef CONTENT_PROCESSING_3D__CONTENT_PROCESSING_3D_INCLUDED
#define CONTENT_PROCESSING_3D__CONTENT_PROCESSING_3D_INCLUDED

class iMesh;
class iFaceVertexMesh;
class iProgressCallBack;

iMesh*
BuildMeshFrom3DContent(
        const iFaceVertexMesh *const * meshObjectPointers,
        long numberOfMeshObjects,
        const long* solidObjectsData,
        long solidObjectsDataSize,
        long extrudeHeight,
        long maxStepHeight,
        long maxSlope,
        const char *const* options,
        iProgressCallBack* progressCallBack
        );

#endif
