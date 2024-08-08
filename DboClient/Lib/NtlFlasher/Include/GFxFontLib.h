/**********************************************************************

Filename    :   GFxFontLib.h
Content     :   Implementation of font sharing and lookup through
                a list of registed GFxMovieDef's.
Created     :   July 9, 2007
Authors     :   Michael Antonov

Copyright   :   (c) 2001-2007 Scaleform Corp. All Rights Reserved.

Notes       :   

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFxFontLib_H
#define INC_GFxFontLib_H

#include "GFxLoader.h"


// ***** GFxFontLib

// An user-managed state used to register MovieDefs so that they can 
// be searched for fonts when creating other movies. Fonts in registered
// files form a library, which is searched before GFxFontProvider.
// Unlike GFxFontProvider, GFxFontLib does not require system font support
// and is thus portable to consoles.

class GFxFontResource;
class GFxMovieDef;



class GFxFontLib : public GFxState
{
    class GFxFontLibImpl *pImpl;
public:
    GFxFontLib();
    virtual ~GFxFontLib();

    // Registers a GFxMovieDef that will be used as a source for fonts.
    // By default, the the movie is pinned in the loader so that its
    // textures don't get unloaded.
    void                AddFontsFrom(GFxMovieDef* md, bool pin = 1);

    // A structure filled in if LookupFont succeeds. Combines a font
    // resource with MovieDef which it came from. It is important
    // for users to keep a reference to both for as long as font is used.
    class FontResult
    {
        friend class GFxFontLib;
        // These should be GPtrs, but they are not for compiler compatibility.
        GFxMovieDef*       pMovieDef;
        GFxFontResource*   pFontResource;
        void               SetResult(GFxMovieDef* pmovieDef, GFxFontResource* pfont);
    public:
        FontResult() { pMovieDef = 0; pFontResource = 0; }
        ~FontResult();                   

        GFxMovieDef*     GetMovieDef() const { return pMovieDef; }
        GFxFontResource* GetFontResource() const { return pFontResource; }
    };

    // Finds or creates a font of specified style by searching all of the
    // registered GFxMovieDef objects. The font resource is created to be compatible
    // with puserDef's binding states and utilizing psharedStates for non-binding
    // state values.
    // fontFlags style as described by GFxFont::FontFlags
    virtual bool        FindFont(FontResult *presult,
                                 const char* name, UInt fontFlags,
                                 GFxMovieDef* puserDef, GFxSharedState *psharedStates);
};



// *** External class inlines dependent on us

inline void GFxSharedState::SetFontLib(GFxFontLib* pri)
{
    SetState(GFxState::State_FontLib, pri);
}
inline GPtr<GFxFontLib>  GFxSharedState::GetFontLib() const
{
    return *(GFxFontLib*) GetStateAddRef(GFxState::State_FontLib);
}


#endif
