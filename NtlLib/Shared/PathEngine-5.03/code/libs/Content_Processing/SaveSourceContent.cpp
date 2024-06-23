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
#include "interface/SaveSourceContent.h"
#include "SaveFaceVertexMesh.h"
#include "SaveAnchorsAndPinnedShapes.h"
#include "libs/PathEngine_Core/interface/Version.h"
#include "common/interface/iXMLOutputStream.h"

void
SaveSourceContent(
        const std::vector<const iFaceVertexMesh*>& meshes,
        const iAnchorsAndPinnedShapes* anchorsAndShapes,
        iXMLOutputStream& xos
        )
{
    xos.openElement("content");
    xos.addAttribute("majorRelease", MajorRelease());
    xos.addAttribute("minorRelease", MinorRelease());
    long i;
    for(i = 0; i != SizeL(meshes); ++i)
    {
        xos.openElement("mesh");
        SaveFaceVertexMesh(*meshes[i], xos);
        xos.closeElement("mesh");
    }
    if(anchorsAndShapes)
    {
        SaveAnchorsAndPinnedShapes(*anchorsAndShapes, xos);
    }
    xos.closeElement("content");
}

