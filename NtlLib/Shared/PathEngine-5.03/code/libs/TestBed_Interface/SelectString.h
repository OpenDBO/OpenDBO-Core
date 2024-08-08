//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "common/STL/vector.h"
#include "common/STL/string.h"

class osinterface;
class iGraphicsInterface;
class controlinterface;

long SelectString(  osinterface *ip,
                    iGraphicsInterface *gip,
                    controlinterface *cip,
                    const std::vector<std::string> &aStrings);
