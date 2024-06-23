//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef XMLHANDLER_INCLUDED
#define XMLHANDLER_INCLUDED

#include "common/interface/Assert.h"

class iXMLHandler
{
    bool _ignore;
    long _ignoreBelow;
    long _currentLine;

protected:

    iXMLHandler* _parent;
    long _depth;

    long getDepth() const
    {
        return _depth;
    }

public:

    iXMLHandler()
    {
        _parent = 0;
        _depth = 0;
        _ignore = false;
        _currentLine = -1;
    }
    virtual ~iXMLHandler()
    {
    }

    void setCurrentLine(long value)
    {
        _currentLine = value;
    }
    long getCurrentLine()
    {
        return _currentLine;
    }

    virtual void init(const char *const* attr)
    {
    }
    virtual iXMLHandler* element(const char* el, const char *const* attr)
    {
        return 0;
    }
    virtual void elementCompleted(iXMLHandler*)
    {
    }
//    virtual void text(const char* string, int length)
//    {
//    }
    virtual void completed()
    {
    }

    virtual iXMLHandler* start(const char* el, const char *const* attr)
    {
        iXMLHandler* child;
        if(_ignore)
        {
            _depth++;
            return this;
        }
        child = element(el, attr);
        if(child == 0)
        {
            _ignore = true;
            _ignoreBelow = _depth;
            _depth++;
            return this;
        }
        if(child == this)
        {
            _depth++;
            return this;
        }
        child->_parent = this;
        return child;
    }
    virtual iXMLHandler* end()
    {
        iXMLHandler* parent;
        if(_ignore)
        {
            assertR(_depth);
            _depth--;
            if(_depth == _ignoreBelow)
            {
                _ignore = false;
            }
            return this;
        }
        if(_depth)
        {
            _depth--;
            parent = this;
        }
        else
        {
            completed();
            parent = _parent;
        }
        parent->elementCompleted(this);
        return parent;
    }
};

#endif

