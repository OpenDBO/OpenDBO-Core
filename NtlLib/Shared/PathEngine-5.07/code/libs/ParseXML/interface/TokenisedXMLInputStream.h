//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef TOKENISED_XML_INPUT_STREAM_INCLUDED
#define TOKENISED_XML_INPUT_STREAM_INCLUDED

#include "common/interface/iXMLInputStream.h"
#include "common/AttributesBuilder.h"
#include <vector>

class cTokenisedXMLInputStream : public iXMLInputStream
{
    const char* _buffer;
    unsigned long _bufferSize;
    unsigned long _bufferPosition;

    std::vector<const char*> _elements;
    std::vector<const char*> _attributes;
    std::vector<int> _attributeTypes;
    long _elementSize;
    long _attributeSize;

    long _nestedLevel;
    bool _bad;
    cAttributesBuilder _ab;

    bool skipString(bool& wasEmpty);
    bool readToken(long tokenSize, unsigned long &result);
    bool peekToken(long tokenSize, unsigned long &result);
    bool readValue(long& result);
    bool readValue(short& result);
    bool readValue(signed char& result);
    bool readValue(unsigned long& result);
    bool readValue(unsigned short& result);
    bool readValue(unsigned char& result);
    bool skipValue(unsigned long size);

    bool skipAttributes();
    bool skipToCloseElement();

public:

    cTokenisedXMLInputStream(const char* buffer, unsigned long bufferSize);

// iXMLInputStream

    const char *const* openElement(const char* element);
    void closeElement();
    const char* peekNextElement();
    const char *const* openNextElement();
    void skipNextElement();
    bool good();
    bool atEnd();
};

#endif
