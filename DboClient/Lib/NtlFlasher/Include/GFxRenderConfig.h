/**********************************************************************

Filename    :   GFxRenderConfig.h
Content     :   GFxRenderConfig - 2D rendering configuration state.
Created     :   January 14, 2007
Authors     :   Michael Antonov

Notes       :   Also includes GFxRenderStats used for tracking
                rendering statistics.

Copyright   :   (c) 2005-2006 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GFXRENDERERINFO_H
#define INC_GFXRENDERERINFO_H

#include "GFxLoader.h"

#include "GRenderer.h"


// Forward declarations
class GRenderer;


// ***** GFxRenderInfo - renderer state


// Combines Renderer and tessellation control.
class GFxRenderConfig : public GFxState
{
protected:
    // In the next version, 2D renderer will wrap/hide tesselator, so it
    // would make sense for Curve Tolerance to be one of its properties.
    // For now, this structure is used in place of a 2D renderer.

    GPtr<GRenderer> pRenderer;
    Float           MaxCurvePixelError;
    UInt32          RenderFlags;
    // Specifies largest blended line width in pixels for which StrokerAA is used.
    // Using StrokerAA is faster, but it will produce artifacts at intersections.
    Float           StrokerAAWidth;
    // Cap flags initialized from renderer - GRenderer::RenderCapBits
    UInt32          RendererCapBits;
    UInt32          RendererVtxFmts;
   
public:

    // Various options that affect 2D rendering.
    enum RenderFlagType
    {       
        // These flags specify what kind of stroking mechanism the player uses.
        RF_StrokeCorrect        = 0x00,         // Flash compatible stroking, expensive to scale/rotate.
        RF_StrokeNormal         = 0x01,         // Fast GFx anisotropic stroking for Normal strokes.
        RF_StrokeHairline       = 0x02,         // Outline only.
        RF_StrokeMask           = 0x03,

        // Whether to enable EdgeAA
        RF_EdgeAA               = 0x10,

        RF_OptimizeTriangles    = 0x20,

        // Disables view frame culling.
        RF_NoViewCull           = 0x100
    };

  
    GFxRenderConfig();
    // Initialize GRendererInfo with GRenderer and flags.
    GFxRenderConfig(GRenderer* prenderer, UInt rendererFlags = 0);
    GFxRenderConfig(const GFxRenderConfig &src);

    GFxRenderConfig& operator = (const GFxRenderConfig &src);
    

    inline bool         IsUsingEdgeAA() const               { return (RenderFlags & RF_EdgeAA) && (RendererCapBits & GRenderer::Cap_FillGouraud); }
    inline bool         IsEdgeAATextured() const            { return (RendererCapBits & GRenderer::Cap_FillGouraudTex) != 0; }
    inline bool         IsOptimizingTriangles() const       { return (RenderFlags & RF_OptimizeTriangles) != 0; }
    inline bool         HasCxformAddAlpha() const           { return (RendererCapBits & GRenderer::Cap_CxformAdd) != 0; }
    inline bool         HasVertexFormat(int v) const        { return (RendererVtxFmts & (1 << v)) != 0; }

    inline RenderFlagType  GetStrokeRenderFlags() const     { return (RenderFlagType) (RenderFlags & RF_StrokeMask); }

    // Obtain RenderCap bits obtained from the renderer.
    inline  UInt32      GetRendererCapBits() const              { return RendererCapBits;  }


    // Sets a renderer, Addref-ing to it. This needs to be done before
    // the movie can be displayed.
    void                SetRenderer(GRenderer* prenderer);
    inline  GRenderer*  GetRenderer() const                     { return pRenderer.GetPtr();  }

    // Set rendering option flags; GRendererInfo::RenderFlagType
    void                SetRenderFlags(UInt32 flags)            { RenderFlags = flags;  }
    inline  UInt32      GetRenderFlags() const                  { return RenderFlags; }

    // Use this to control how finely curves are subdivided.
    // 1.0 is the default.  Larger values result in coarser,
    // more angular curves with fewer vertices.
    inline  void        SetMaxCurvePixelError(Float pixelError) { MaxCurvePixelError = GTL::gclamp<Float>(pixelError, 1e-6f, 1e6f); }
    inline  Float       GetMaxCurvePixelError() const           { return MaxCurvePixelError; }

    // Specifies largest blended line width in pixels for which StrokerAA is used.
    inline  Float       GetStrokerAAWidth() const               {  return StrokerAAWidth; }
    inline  void        SetStrokerAAWidth(Float aawidth)        {  StrokerAAWidth = aawidth; }
};


// *** External class inlines dependent on us

inline void GFxSharedState::SetRenderConfig(GFxRenderConfig* pri)
{
    SetState(GFxState::State_RenderConfig, pri);
}
inline GPtr<GFxRenderConfig>  GFxSharedState::GetRenderConfig() const
{
    return *(GFxRenderConfig*) GetStateAddRef(GFxState::State_RenderConfig);
}

inline void GFxSharedState::SetRenderConfig(GRenderer *prenderer, Float maxError, UInt rendererFlags)
{
    GPtr<GFxRenderConfig> pi = *new GFxRenderConfig(prenderer, rendererFlags);
    if (pi)    
        pi->SetMaxCurvePixelError(maxError);    
    SetState(GFxState::State_RenderConfig, pi.GetPtr());
}

inline GPtr<GRenderer> GFxSharedState::GetRenderer() const
{
    GPtr<GFxRenderConfig> p = GetRenderConfig();
    return p ? p->GetRenderer() : 0;
}




// ***** GFxRenderStats - renderer statistics

// Keeps track of 2D rendering statistics.
class GFxRenderStats : public GFxState
{
protected:
   
    // Generated tessellation/stroke triangles since last clear
    UInt32  TessTriangles;

public:

    GFxRenderStats() : GFxState(State_RenderStats)
    {
        TessTriangles = 0;
    }
    // Initialize GRendererInfo with GRenderer and flags.    
    GFxRenderStats(const GFxRenderStats &src) : GFxState(State_RenderStats)
    {
        TessTriangles = src.TessTriangles;
    }
    GFxRenderStats& operator = (const GFxRenderStats &src)
    {
        TessTriangles = src.TessTriangles;
        return *this;
    }

    // Return tessellation statistics for profiling.
    // Resets statistics counter to zero after query.
    inline  UInt        GetTessStatistics()
    {
        UInt triCount = TessTriangles; TessTriangles = 0;
        return triCount;
    }

    void    AddTessTriangles(UInt count)
    {
        TessTriangles += count;
    } 
};


inline void GFxSharedState::SetRenderStats(GFxRenderStats* prs)
{
    SetState(GFxState::State_RenderStats, prs);
}
inline GPtr<GFxRenderStats>  GFxSharedState::GetRenderStats() const
{
    return *(GFxRenderStats*) GetStateAddRef(GFxState::State_RenderStats);
}



#endif
