//**********************************************************************
//
// Copyright (c) 2002-2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SelectString.h"
#include "libs/TestBed_Application/interface/iControlInterface.h"
#include "libs/TestBed_Application/interface/iOSInterface.h"
#include "libs/TestBed_Application/interface/iGraphicsInterface.h"
#include "libs/Geometry/interface/tPoint.h"

long SelectString(  osinterface *ip,
                    iGraphicsInterface *gip,
                    controlinterface *cip,
                    const std::vector<std::string> &aStrings)
{
    long i;
    long x,y;
    tPoint p;

    long max_size=0;
    for(i = 0; i < SizeL(aStrings); i++)
    {
        if(SizeL(aStrings[i]) > max_size)
        {
            max_size = SizeL(aStrings[i]);
        }
    }
    max_size++;

    const long columnSize = ip->getwiny() / 16;

    while(1)
    {
        bool mousedown = (cip->getlmbdown() != 0);


        p=tPoint(cip->getmousex(),cip->getmousey(),10000);
        y = ip->getwiny() - 16;
        x = 0;
        if(aStrings.empty() && mousedown)
            return -1;
        for(i = 0; i < SizeL(aStrings); i++)
        {
            gip->setColour("white");
            if( p.getY()>=y &&
                p.getY()<y+16 &&
                p.getX()>=x &&
                p.getX()<x+(8*max_size)
                )
            {
                if(mousedown)
                    return i;
                gip->setColour("red");
            }
            iRenderGeometry* rg = gip->newText_8By16(x,y,aStrings[i].c_str());
            gip->render_ScreenSpace(rg);
            y -= 16;
            if(y < 0)
            {
                y = ip->getwiny() - 16;
                x+=8*max_size;
            }
        }
        ip->update();
    }
}

