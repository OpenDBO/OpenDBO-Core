//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef FIXED_BUFFER_OUTPUT_STREAM
#define FIXED_BUFFER_OUTPUT_STREAM

#include "common/interface/Assert.h"
#include "i_pathengine.h"
#include <memory.h>

class cFixedBufferOutputStream : public iOutputStream
{
    char* m_buffer;
    unsigned long m_buffer_size;
    unsigned long m_position;

public:

    cFixedBufferOutputStream(char* buffer, unsigned long buffer_size)
    {
        m_buffer = buffer;
        m_buffer_size = buffer_size;
        m_position = 0;
    }

    bool completed() const
    {
        return m_position == m_buffer_size;
    }

    virtual void put(const char *data, unsigned long data_size)
    {
        assertD(data_size <= m_buffer_size);
        assertD(m_position <= m_buffer_size - data_size);
        memcpy(m_buffer + m_position, data, data_size);
        m_position += data_size;
    }
};

#endif
