//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef INFORMATION_CHANNELS_OUTPUT_STREAM
#define INFORMATION_CHANNELS_OUTPUT_STREAM

#include "common/interface/iXMLOutputStream.h"
#include "common/STL/vector.h"

class cInformationChannelsOutputStream : public iXMLOutputStream
{
    iXMLOutputStream& _base;
    long _depth;
    bool _inMesh3D;
    long _currentIndex;
    std::vector<long>* _surfaceTypes;
    std::vector<long>* _sectionIDs;
    std::vector<long>* _userData;

public:

    cInformationChannelsOutputStream(iXMLOutputStream& base,
                                    std::vector<long>* surfaceTypes,
                                    std::vector<long>* sectionIDs,
                                    std::vector<long>* userData
									) :
      _base(base)
    {
        _depth = 0;
        _inMesh3D = false;
        _surfaceTypes = surfaceTypes;
        _sectionIDs = sectionIDs;
        _userData = userData;
    }

// iXMLOutputStream

    void openElement(const char* element);
    void addAttribute(const char* attribute, const char* value);
    void addAttribute(const char* attribute, long value);
    void addAttribute(const char* attribute, unsigned long value);
    void closeElement(const char* element);
};

#endif
