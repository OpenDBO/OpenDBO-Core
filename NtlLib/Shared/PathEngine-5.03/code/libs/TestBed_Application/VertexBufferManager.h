//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/interface/Assert.h"
#include <d3dx8.h>
#include <vector>

const long MAX_TRIS=40000;
const long MAX_LINES=80000;

class cVertexBufferManager
{
    IDirect3DVertexBuffer8* m_tris_buffer;
    IDirect3DVertexBuffer8* m_lines_buffer;
    long m_first_free_tri;
    std::vector<long> m_next_free_tri;
    long m_first_free_line;
    std::vector<long> m_next_free_line;
    long _trisAllocated;
    long _linesAllocated;

public:
    cVertexBufferManager();
    ~cVertexBufferManager();

    long allocateTri()
    {
        _trisAllocated++;
        long result=m_first_free_tri;
        if(result!=-1)
            m_first_free_tri=m_next_free_tri[result];
        return result;
    }
    void freeTri(long index)
    {
        _trisAllocated--;
        assertD(index >= 0 && index < SizeL(m_next_free_tri));
        m_next_free_tri[index]=m_first_free_tri;
        m_first_free_tri=index;
    }

    long allocateLine()
    {
        _linesAllocated++;
        long result=m_first_free_line;
        if(result!=-1)
            m_first_free_line=m_next_free_line[result];
        return result;
    }
    void freeLine(long index)
    {
        _linesAllocated--;
        assertD(index >= 0 && index < SizeL(m_next_free_line));
        m_next_free_line[index]=m_first_free_line;
        m_first_free_line=index;
    }

    IDirect3DVertexBuffer8* getTrisBuffer()
    {
        return m_tris_buffer;
    }
    IDirect3DVertexBuffer8* getLinesBuffer()
    {
        return m_lines_buffer;
    }
};

