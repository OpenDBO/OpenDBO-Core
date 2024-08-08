//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef MESH_ELEMENT_INCLUDED
#define MESH_ELEMENT_INCLUDED

#include "common/interface/Assert.h"
#include "common/TemplateMesh/External.h"

class bMeshElement
{
public:

    enum eType
    {
        FACE,
        EDGE,
        VERTEX,
        UNSET // for internal use only
    };
};

template <class T>
class cMeshElement : public bMeshElement
{
public:
    
    typedef cMeshElement<T> tThis;
    typedef T tMesh;
    typedef typename T::edge tEdge;
    typedef typename T::vertex tVertex;
    typedef typename T::face tFace;

private:

    union
    {
        tEdge _edge;
        tVertex _vertex;
        tFace _face;
    };
    eType _type;

public:

// allow construction uninitialised
    cMeshElement()
    {
    }

// straightforward construct as element
    cMeshElement(tFace face) : _face(face), _type(FACE) {}
    cMeshElement(tEdge edge) : _edge(edge), _type(EDGE) {}
    cMeshElement(tVertex vertex) : _vertex(vertex), _type(VERTEX) {}

    void setInvalid() {_type = UNSET;}
    void clear() {_type = UNSET;}
    bool valid() const {return _type != UNSET;}

    eType type() const
    {
        assertD(_type != UNSET);
        return _type;
    }

    bool operator==(const cMeshElement<T> &rhs) const
    {
        if(type() != rhs.type())
        {
            return false;
        }
        switch(type())
        {
        default:
            invalid();
        case UNSET:
            return true;
        case FACE:
            return _face == rhs._face;
        case EDGE:
            return _edge == rhs._edge;
        case VERTEX:
            return _vertex == rhs._vertex;
        }
    }
    bool operator!=(const cMeshElement<T> &rhs) const
    {
        return !(*this==rhs);
    }

    bool isVertex() const
    {
        return _type == VERTEX;
    }
    bool isEdge() const
    {
        return _type == EDGE;
    }
    bool isFace() const
    {
        return _type == FACE;
    }

    bool isExternalVertex() const
    {
        return type() == VERTEX && VertexIsExternal(tMesh::firstEdge(_vertex));
    }
    bool isExternalEdge() const
    {
        return type() == EDGE && (_edge.face()->isExternal() || _edge.twin().face()->isExternal());
    }
    bool isExternalFace() const
    {
        return type() == FACE && _face->isExternal();
    }


    tFace asFace() const
    {
        assertD(_type == FACE);
        return _face;
    }
    tEdge asEdge() const
    {
        assertD(_type == EDGE);
        return _edge;
    }
    tVertex asVertex() const
    {
        assertD(_type == VERTEX);
        return _vertex;
    }

    tEdge asEdgeInFace(tFace f) const
    {
        assertD(_type == EDGE);
        if(_edge.face() == f)
        {
            return _edge;
        }
        assertD(_edge.twin().face() == f);
        return _edge.twin();
    }
    tEdge asEdgeFromVertex(tVertex v) const
    {
        assertD(_type == EDGE);
        if(_edge.vertex() == v)
        {
            return _edge;
        }
        assertD(_edge.twin().vertex() == v);
        return _edge.twin();
    }
    tEdge asEdgeFromVertex(const tThis& v) const
    {
        return asEdgeFromVertex(v.asVertex());
    }

    tFace getOverlappingFace() const
    {
        switch(type())
        {
        default:
            invalid();
        case FACE:
            return _face;
        case EDGE:
            if(_edge.face()->isExternal())
            {
                return _edge.twin().face();
            }
            return _edge.face();            
        case VERTEX:
            tEdge e = tMesh::firstEdge(_vertex);
            if(e.face()->isExternal())
            {
                return e.twin().face();
            }
            return e.face();
        }
    }

    bool isPositionallyEquivalent(const cMeshElement<T>& rhs) const
    {
        if(type() != rhs.type())
            return false;
        switch(type())
        {
        default:
            invalid();
        case FACE:
            return _face == rhs._face;
        case EDGE:
            return _edge == rhs._edge || (_edge.twin().valid() && _edge.twin() == rhs._edge);
        case VERTEX:
            return _vertex == rhs._vertex;
        }
    }
};


#endif
