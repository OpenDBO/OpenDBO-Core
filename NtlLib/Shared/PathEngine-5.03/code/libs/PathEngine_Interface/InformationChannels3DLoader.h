//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INFORMATION_CHANNELS_3D_LOADER_INCLUDED
#define INFORMATION_CHANNELS_3D_LOADER_INCLUDED

#include "libs/ParseXML/interface/iXMLHandler.h"
#include "common/STL/vector.h"

class cInformationChannels3DLoader : public iXMLHandler
{
    long _facesIn3DMesh;
    std::vector<long>*& _surfaceTypes;
    std::vector<long>*& _sectionIDs;
    std::vector<long>*& _userData;
    long _currentIndex;

public:

    cInformationChannels3DLoader(long facesIn3DMesh,
                    std::vector<long>*& surfaceTypes,
                    std::vector<long>*& sectionIDs,
                    std::vector<long>*& userData
                    ) :
     _surfaceTypes(surfaceTypes),
     _sectionIDs(sectionIDs),
     _userData(userData)
    {
        _facesIn3DMesh = facesIn3DMesh;
        _surfaceTypes = 0;
        _sectionIDs = 0;
        _userData = 0;
    }

    void processFaceAttributes(long faceIndex, const char *const* attributes);

// interface for iXMLHandler

    void init(const char *const* attr);
    iXMLHandler* element(const char* el, const char *const* attributes);
};

#endif
