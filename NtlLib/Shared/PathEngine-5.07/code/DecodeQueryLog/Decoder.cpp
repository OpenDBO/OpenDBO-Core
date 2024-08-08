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
#include "Decoder.h"
#include "platform_common/ToAndFromByteBuffer.h"
#include "common/interface/Assert.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/FormatTimeStamp.h"
#include "common/STL/sstream.h"

long
cDecoder::decodeSigned(long tokenSize)
{
    if(_overran)
    {
        return 0;
    }
    if(static_cast<unsigned long>(tokenSize) > _remainingSize)
    {
        _overran = true;
        return 0;
    }
    long result = SignedFromByteBuffer(_bufferPosition, tokenSize);
    _bufferPosition += tokenSize;
    _remainingSize -= tokenSize;
    return result;
}
unsigned long
cDecoder::decodeUnsigned(long tokenSize)
{
    if(_overran)
    {
        return 0;
    }
    if(static_cast<unsigned long>(tokenSize) > _remainingSize)
    {
        _overran = true;
        return 0;
    }
    unsigned long result = UnsignedFromByteBuffer(_bufferPosition, tokenSize);
    _bufferPosition += tokenSize;
    _remainingSize -= tokenSize;
    return result;
}
bool
cDecoder::decodeBool()
{
    long result = decodeSigned(1);
    return result != 0;
}
float
cDecoder::decodeFloat()
{
    if(_overran)
    {
        return 0;
    }
    if(FLOAT_BYTE_SIZE > _remainingSize)
    {
        _overran = true;
        return 0;
    }
    float result = FloatFromByteBuffer(_bufferPosition);
    _bufferPosition += FLOAT_BYTE_SIZE;
    _remainingSize -= FLOAT_BYTE_SIZE;
    return result;
}
std::string
cDecoder::decodeString()
{
    unsigned long len = decodeUnsigned(2);
    if(_overran || len > _remainingSize)
    {
        _overran = true;
        return "";
    }
    std::string result(_bufferPosition, len);
    _bufferPosition += len;
    _remainingSize -= len;
    return result;
}

void
cDecoder::decodeTimeStamp()
{
    if(_overran)
    {
        return;
    }
    if(6 > _remainingSize)
    {
        _overran = true;
        return;
    }
    std::string formatted = FormatTimeStamp(_bufferPosition);
    _bufferPosition += 6;
    _remainingSize -= 6;
    _os.addAttribute("timeStamp", formatted.c_str());
}


void
cDecoder::decodeMethodInfo(long& interfaceIndex, long& methodIndex, bool& methodEntry)
{
    interfaceIndex = decodeSigned(1);
    methodEntry = true;
    if(interfaceIndex < 0)
    {
        interfaceIndex = -1 - interfaceIndex;
        methodEntry = false;
    }
    methodIndex = decodeSigned(1);

    _methodEntry = methodEntry;
    if(_methodEntry)
    {
        _os.openElement("methodEntry");
    }
    else
    {
        _os.openElement("methodExit");
    }

    unsigned long queryIndex = decodeUnsigned(4);
    if(_overran)
    {
        return;
    }
    _os.addAttribute("queryIndex", queryIndex);

    unsigned long thisPointer = decodeUnsigned(4);
    if(_overran)
    {
        return;
    }
    {
        std::ostringstream oss;
        oss.setf(std::ios_base::hex, std::ios_base::basefield);
        oss << "0x" << thisPointer;
        _os.addAttribute("thisPointer", oss.str().c_str());
    }

    decodeTimeStamp();
}

cDecoder::cDecoder(
        const char* buffer, unsigned long bufferLength, iXMLOutputStream& os,
        long& interfaceIndex, long& methodIndex, bool& methodEntry,
        unsigned long& recordSize
        ) : 
 _bufferPosition(buffer),
 _remainingSize(bufferLength),
 _overran(false),
 _os(os)
{
    if(_remainingSize <= 2)
    {
        _overran = true;
        return;
    }
    recordSize = decodeUnsigned(2);
    if(_overran)
    {
        return;
    }
    assertR(recordSize > 2);
    if(_remainingSize < recordSize - 2)
    {
        _overran = true;
        return;
    }
    _remainingSize = recordSize - 2;
    decodeMethodInfo(interfaceIndex, methodIndex, methodEntry);
}

bool
cDecoder::overran() const
{
    return _overran;
}
bool
cDecoder::empty() const
{
    return _remainingSize == 0;
}

void
cDecoder::writeInterfaceName(const char* name)
{
    _os.addAttribute("interface", name);
}
void
cDecoder::writeMethodName(const char* name)
{
    _os.addAttribute("method", name);
}

void
cDecoder::decode_bool(const char* name)
{
    long valueAsLong = decodeSigned(1);
    _os.openElement("bool");
    _os.addAttribute("name", name);
    if(valueAsLong)
    {
        _os.addAttribute("value", "true");
    }
    else
    {
        _os.addAttribute("value", "false");
    }
    _os.closeElement("bool");
}
void
cDecoder::decode_long(const char* name)
{
    _os.openElement("long");
    _os.addAttribute("name", name);
    long value = decodeSigned(4);
    _os.addAttribute("value", value);
    _os.closeElement("long");
}
void
cDecoder::decode_unsigned_long(const char* name)
{
    _os.openElement("unsigned_long");
    _os.addAttribute("name", name);
    unsigned long value = decodeSigned(4);
    _os.addAttribute("value", value);
    _os.closeElement("unsigned long");
}
void
cDecoder::decode_float(const char* name)
{
    _os.openElement("float");
    _os.addAttribute("name", name);
    float value = decodeFloat();
    std::ostringstream os;
    os << value;
    _os.addAttribute("value", os.str().c_str());
    _os.closeElement("float");
}
void
cDecoder::decode_const_char_star(const char* name)
{
    _os.openElement("string");
    _os.addAttribute("name", name);
    std::string value = decodeString();
    _os.addAttribute("value", value.c_str());
    _os.closeElement("string");
}
void
cDecoder::decode_const_char__starconst_star(const char* name)
{
    _os.openElement("attributes");
    _os.addAttribute("name", name);
    while(1)
    {
        std::string attribute = decodeString();
        if(attribute.empty())
        {
            break;
        }
        std::string value = decodeString();
        std::string attributeWithPrefix = "_" + attribute;
        _os.addAttribute(attributeWithPrefix.c_str(), value.c_str());
    }
    _os.closeElement("attributes");
}
void
cDecoder::decode_const_long_star(const char* name)
{
    _os.openElement("point3L");
    _os.addAttribute("name", name);
    _os.addAttribute("x", decodeSigned(4));
    _os.addAttribute("y", decodeSigned(4));
    _os.addAttribute("z", decodeSigned(4));
    _os.closeElement("point3L");
}
void
cDecoder::decode_const_float_star(const char* name)
{
    _os.openElement("point3F");
    _os.addAttribute("name", name);
    {
        float value = decodeFloat();
        std::ostringstream os;
        os << value;
        _os.addAttribute("x", os.str().c_str());
    }
    {
        float value = decodeFloat();
        std::ostringstream os;
        os << value;
        _os.addAttribute("y", os.str().c_str());
    }
    {
        float value = decodeFloat();
        std::ostringstream os;
        os << value;
        _os.addAttribute("z", os.str().c_str());
    }
    _os.closeElement("point3F");
}
void
cDecoder::decode_void_star(const char* name)
{
    _os.openElement("voidPointer");
    _os.addAttribute("name", name);
    _os.addAttribute("value", decodeUnsigned(4));
    _os.closeElement("voidPointer");
}
void
cDecoder::decode_cPosition(const char* name)
{
    _os.openElement("cPosition");
    _os.addAttribute("name", name);
    _os.addAttribute("cell", decodeSigned(4));
    _os.addAttribute("x", decodeSigned(4));
    _os.addAttribute("y", decodeSigned(4));
    _os.closeElement("cPosition");
}
void
cDecoder::decode_cHorizontalRange(const char* name)
{
    _os.openElement("cHorizontalRange");
    _os.addAttribute("name", name);
    _os.addAttribute("minX", decodeSigned(4));
    _os.addAttribute("minY", decodeSigned(4));
    _os.addAttribute("maxX", decodeSigned(4));
    _os.addAttribute("maxY", decodeSigned(4));
    _os.closeElement("cHorizontalRange");
}

void
cDecoder::decodeAPIObject(const char* name)
{
    _os.openElement("APIObject");
    _os.addAttribute("name", name);
    {
        std::ostringstream oss;
        oss.setf(std::ios_base::hex, std::ios_base::basefield);
        oss << "0x" << decodeUnsigned(4);
        _os.addAttribute("pointer", oss.str().c_str());
    }
    _os.closeElement("APIObject");
}
void
cDecoder::decodeCallBack(const char* name)
{
    _os.openElement("CallBack");
    _os.addAttribute("name", name);
    {
        std::ostringstream oss;
        oss.setf(std::ios_base::hex, std::ios_base::basefield);
        oss << "0x" << decodeUnsigned(4);
        _os.addAttribute("pointer", oss.str().c_str());
    }
    _os.closeElement("CallBack");
}
void
cDecoder::decodeClassCallBack(const char* name)
{
    _os.openElement("ClassCallBack");
    _os.addAttribute("name", name);
    {
        std::ostringstream oss;
        oss.setf(std::ios_base::hex, std::ios_base::basefield);
        oss << "0x" << decodeUnsigned(4);
        _os.addAttribute("pointer", oss.str().c_str());
    }
    _os.closeElement("ClassCallBack");
}
void
cDecoder::decodeCollidingLine()
{
    _os.openElement("CollidingLine");
    std::ostringstream os;
    long i;
    for(i = 0; i != 4; ++i)
    {
        if(i)
        {
            os << ',';
        }
        os << decodeSigned(4);
    }
    _os.addAttribute("coords", os.str().c_str());    
    _os.closeElement("CollidingLine");
}
void
cDecoder::decodeCollisionShape()
{
    _os.openElement("CollisionShape");
    long numberOfPoints = decodeSigned(4);
    std::ostringstream os;
    long i;
    for(i = 0; i != numberOfPoints; ++i)
    {
        if(i)
        {
            os << ',';
        }
        os << decodeSigned(4);
        os << ',';
        os << decodeSigned(4);
    }
    _os.addAttribute("coords", os.str().c_str());    
    _os.closeElement("CollisionShape");
}
void
cDecoder::decodeDataBuffer()
{
    _os.openElement("DataBuffer");
    unsigned long fileIndex = decodeUnsigned(2);
    if(fileIndex)
    {
        _os.addAttribute("fileIndex", fileIndex);
    }
    else
    {
        _os.addAttribute("fileIndex", "notSaved");
    }
    _os.closeElement("DataBuffer");
}
void
cDecoder::decodePathBuffers()
{
    _os.openElement("PathBuffers");
    long pathLength =  decodeSigned(4);
    _os.addAttribute("pathLength", pathLength);
    _os.openElement("positions");
    long i;
    for(i = 0; i != pathLength; ++i)
    {
        _os.openElement("position");
        _os.addAttribute("cell", decodeSigned(4));
        _os.addAttribute("x", decodeSigned(4));
        _os.addAttribute("y", decodeSigned(4));
        _os.closeElement("position");
    }
    _os.closeElement("positions");
    bool connectionIndicesBufferNonZero = decodeBool();
    if(connectionIndicesBufferNonZero)
    {
        _os.openElement("connectionIndices");
        assertD(pathLength > 1);
        for(i = 0; i + 1 < pathLength; ++i)
        {
            _os.openElement("connectionIndex");
            _os.addAttribute("value", decodeSigned(4));
            _os.closeElement("connectionIndex");
        }
        _os.closeElement("connectionIndices");
    }
    _os.closeElement("PathBuffers");
}
void
cDecoder::decodeSourceContent()
{
    _os.openElement("SourceContent");
    unsigned long fileIndex = decodeUnsigned(2);
    if(fileIndex)
    {
        _os.addAttribute("fileIndex", fileIndex);
    }
    else
    {
        _os.addAttribute("fileIndex", "notSaved");
    }
    _os.closeElement("SourceContent");
}
void
cDecoder::decodeAgentVector()
{
    _os.openElement("AgentVector");
    long number = decodeSigned(4);
    long i;
    for(i = 0; i != number; ++i)
    {
        _os.openElement("agent");
        {
            std::ostringstream oss;
            oss.setf(std::ios_base::hex, std::ios_base::basefield);
            oss << "0x" << decodeUnsigned(4);
            _os.addAttribute("pointer", oss.str().c_str());
        }
        _os.closeElement("agent");
    }
    _os.closeElement("AgentVector");
}
void
cDecoder::decodeContentChunkInstanceVector()
{
    _os.openElement("ContentChunkInstanceVector");
    long number = decodeSigned(4);
    long i;
    for(i = 0; i != number; ++i)
    {
        _os.openElement("instance");
        {
            std::ostringstream oss;
            oss.setf(std::ios_base::hex, std::ios_base::basefield);
            oss << "0x" << decodeUnsigned(4);
            _os.addAttribute("pointer", oss.str().c_str());
        }
        _os.closeElement("instance");
    }
    _os.closeElement("ContentChunkInstanceVector");
}

void
cDecoder::decodeLongVector()
{
    _os.openElement("LongVector");
    long number = decodeSigned(4);
    long i;
    for(i = 0; i != number; ++i)
    {
        _os.openElement("entry");
        _os.addAttribute("value", decodeSigned(4));
        _os.closeElement("entry");
    }
    _os.closeElement("LongVector");
}

void
cDecoder::writeRecordEnd()
{
    if(_methodEntry)
    {
        _os.closeElement("methodEntry");
    }
    else
    {
        _os.closeElement("methodExit");
    }
}
