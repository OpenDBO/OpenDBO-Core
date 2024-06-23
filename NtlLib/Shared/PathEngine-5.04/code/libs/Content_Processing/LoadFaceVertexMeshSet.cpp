//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/LoadFaceVertexMeshSet.h"
#include "LoadedFaceVertexMesh.h"
#include "i_pathengine.h"
#include "common/CLib/string.h"

cFaceVertexMeshSetLoader::~cFaceVertexMeshSetLoader()
{
    long i;
    for(i = 0; i != SizeL(_result); ++i)
    {
        delete _result[i];
    }
}

iXMLHandler*
cFaceVertexMeshSetLoader::element(const char* el, const char *const* attr)
{
    if(strcmp(el, "mesh") == 0)
    {
        cLoadedFaceVertexMesh* m = new cLoadedFaceVertexMesh;
        _result.push_back(m);
        return m;
    }
    return 0;
}
