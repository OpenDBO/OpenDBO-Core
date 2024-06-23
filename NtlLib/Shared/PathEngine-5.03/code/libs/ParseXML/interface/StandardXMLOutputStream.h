//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef STANDARDXMLOUTPUTSTREAM_HAS_BEEN_INCLUDED
#define STANDARDXMLOUTPUTSTREAM_HAS_BEEN_INCLUDED

#include "common/interface/Assert.h"
#include "common/interface/iXMLOutputStream.h"

class iOutputStream;

class cStandardXMLOutputStream : public iXMLOutputStream
{
    iOutputStream& _os;
    long _indent;
    bool _hasNoChildren;

public:

    cStandardXMLOutputStream(iOutputStream& os) :
      _os(os)
    {
        _indent = 0;
        _hasNoChildren = false;
    }
    ~cStandardXMLOutputStream()
    {
        assertD(_indent == 0);
    }

// iXMLOutputStream

    void openElement(const char* element);
    void addAttribute(const char* attribute, const char* value);
    void addAttribute(const char* attribute, long value);
    void addAttribute(const char* attribute, unsigned long value);
    void closeElement(const char* element);
};

#endif
