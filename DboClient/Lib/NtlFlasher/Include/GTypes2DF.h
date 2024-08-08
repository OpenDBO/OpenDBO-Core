/*****************************************************************

Filename    :   GTypes2DF.h
Content     :   Geometric 2D floating point types
Created     :   January 14, 1999
Authors     :   Michael Antonov, Brendan Iribe, Prasad Silva
Copyright   :   (c) 1999-2007 Scaleform Corp. All Rights Reserved.

Licensees may use this file in accordance with the valid Scaleform
Commercial License Agreement provided with the software.

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING 
THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR ANY PURPOSE.

**********************************************************************/

#ifndef INC_GTYPES2DF_H
#define INC_GTYPES2DF_H

#ifndef GFC_NO_TYPES2D_FLOAT

#include "GTypes.h"
#include "GTLTypes.h"

#ifdef GFC_MATH_H
#include GFC_MATH_H
#else
#include <math.h>
#endif
#include <string.h>

// ****************************************************************************
// Size template class
// 
template <class T>
class GSize
{
public:
    // *** GSize data members
    T   Width;
    T   Height;

    enum BoundsType { Min, Max };

    // Constructors
    GINLINE GSize()                      { }
    GINLINE explicit GSize(T val)        { SetSize(val); }
    GINLINE GSize(T width, T height)     { SetSize(width, height); }
    GINLINE GSize(const GSize<T> &sz)    { SetSize(sz.Width, sz.Height); }
    GINLINE GSize(BoundsType bt)         { SetSize(bt); }            
    
    // Initialization
    GINLINE void SetSize(T val)                 { Width = val; Height = val; }
    GINLINE void SetSize(T w, T h)              { Width = w;   Height = h; }
    GINLINE void SetSize(const GSize<T> &sz)    { SetSize(sz.Width, sz.Height); }
    //GINLINE void SetSize(BoundsType bt);      // this function uses GFC_MAX_T which is not defined
    GINLINE void SetWidth(T w)                  { Width = w; }
    GINLINE void SetHeight(T h)                 { Height= h; }                                                
    GINLINE void Clear()                        { Width=0; Height=0; }
    
    // Swaps the two sizes
    GINLINE void Swap(GSize<T> *psz);
    GINLINE void Transpose()            { GTL::gswap(Width, Height); }

    // Set to a + (b - a) * t
    GINLINE GSize<T>& SetLerp(const GSize<T>& a, const GSize<T>& b, T t);
                                                                         
    // State                                            
    GINLINE bool IsEmpty() const    { return (Width<=1)||(Height<=1); }
    GINLINE bool IsNull() const     { return (Width==0)&&(Height==0); }
    GINLINE bool IsValid() const    { return (Width>=0)&&(Height>=0); }
                                                
    // Center values                        
    GINLINE GSize<T> Center() const { return GSize<T>(HCenter(), VCenter()); }
    GINLINE T HCenter() const       { return Width/2; }
    GINLINE T VCenter() const       { return Height/2; }
                                                       
    // Area
    GINLINE T Area() const  { return Width*Height; }
                                                
    // Expand the size
    GINLINE GSize<T>& Expand(T w, T h)              { Width+=w; Height+=h; return *this; }
    GINLINE GSize<T>& Expand(const GSize<T> &sz)    { return Expand(sz.Width, sz.Height); }
    GINLINE GSize<T>& Expand(T value)               { return Expand(value,value); }
                
    // Contract the size
    GINLINE GSize<T>& Contract(T value)             { Width-=value; Height-=value; return *this; }
    GINLINE GSize<T>& Contract(const GSize<T> &sz)  { return Contract(sz.Width, sz.Height); }
    GINLINE GSize<T>& Contract(T w, T h)            { Width-=w; Height-=h; return *this; }

    // Mul (scale) the size
    GINLINE GSize<T>& Mul(const GSize<T> &sz)   { return Mul(sz.Width, sz.Height); }
    GINLINE GSize<T>& Mul(T value)              { Width*=value; Height*=value; return *this; }  
    GINLINE GSize<T>& Mul(T w, T h)             { Width*=w; Height*=h; return *this; }

    // Divide (scale) the size
    GINLINE GSize<T>& Div(const GSize<T> &sz)   { return Div(sz.Width, sz.Height); }
    GINLINE GSize<T>& Div(T value)              { Width/=value; Height/=value; return *this; }
    GINLINE GSize<T>& Div(T w, T h)             { Width/=w; Height/=h; return *this; }
    
    // Clamp the size
    GINLINE GSize<T>& Clamp(const GSize<T> &sz) { return Clamp(sz.Width, sz.Height); }
    GINLINE GSize<T>& Clamp(T w, T h)           { if (Width>w) Width=w; if (Height>h) Height=h; return *this; }
    GINLINE GSize<T>& ClampWidth(T w)           { if (Width>w) Width=w; return *this; }
    GINLINE GSize<T>& ClampHeight(T h)          { if (Height>h) Height=h; return *this; }

    // Return new adjusted sizes
    GINLINE GSize<T> ExpandedTo(const GSize<T> &sz) const   { return GSize<T>( GTL::gmax(Width, sz.Width), GTL::gmax(Height, sz.Height) ); }
    GINLINE GSize<T> ExpandedTo(T w, T h) const             { return GSize<T>( GTL::gmax<T>(Width, w), GTL::gmax<T>(Height, h) ); }
    GINLINE GSize<T> ExpandedToWidth(T w) const             { return GSize<T>( GTL::gmax<T>(Width, w), Height ); }
    GINLINE GSize<T> ExpandedToHeight(T h) const            { return GSize<T>( Width, GTL::gmax<T>(Height, h) ); }
                                                        
    GINLINE GSize<T> ClampedTo(const GSize<T> &sz) const    { return GSize<T>( GTL::gmin(Width, sz.Width), GTL::gmin(Height, sz.Height) ); }
    GINLINE GSize<T> ClampedTo(T w, T h) const              { return GSize<T>( GTL::gmin<T>(Width, w), GTL::gmin<T>(Height, h) ); }
    GINLINE GSize<T> ClampedToWidth(T w) const              { return GSize<T>( GTL::gmin<T>(Width, w), Height); }
    GINLINE GSize<T> ClampedToHeight(T h) const             { return GSize<T>( Width, GTL::gmin<T>(Height, h) ); }

    // Operator overrides
    GINLINE const GSize<T>& operator =  ( const GSize<T> &sz )  { SetSize(sz); return *this; }
    GINLINE const GSize<T>& operator =  ( T value )             { SetSize(value, value); return *this; }
    GINLINE const GSize<T>& operator =  ( BoundsType bt )       { SetSize(bt); return *this; }

    GINLINE const GSize<T>& operator += ( const GSize<T> &sz )  { return Expand(sz); }
    GINLINE const GSize<T>& operator += ( T value )             { return Expand(value); }
                                                            
    GINLINE const GSize<T>& operator -= ( const GSize<T> &sz )  { return Contract(sz); }
    GINLINE const GSize<T>& operator -= ( T value )             { return Contract(value); }
                                                            
    GINLINE const GSize<T>& operator *= ( const GSize<T> &sz )  { return Mul(sz); }
    GINLINE const GSize<T>& operator *= ( T value )             { return Mul(value); }
                                                            
    GINLINE const GSize<T>& operator /= ( const GSize<T> &sz )  { return Div(sz); }
    GINLINE const GSize<T>& operator /= ( T value )             { return Div(value); }

    GINLINE const GSize<T>& operator |= ( const GSize<T> &sz )  { return (*this = ExpandedTo(sz)); }
    GINLINE const GSize<T>& operator &= ( const GSize<T> &sz )  { return (*this = ClampedTo(sz)); }

    GINLINE bool            operator == ( const GSize<T> &sz ) const  { return (Width == sz.Width) && (Height == sz.Height); }
    GINLINE bool            operator != ( const GSize<T> &sz ) const  { return (Width != sz.Width) || (Height != sz.Height); }

    GINLINE const GSize<T>  operator -  ()                          { return GSize<T>(-Width, -Height); }
    GINLINE const GSize<T>  operator +  ( const GSize<T> &sz)       { return GSize<T>(Width+sz.Width, Height+sz.Height); }
    GINLINE const GSize<T>  operator +  ( T value )                 { return GSize<T>(Width+value, Height+value); }
    GINLINE const GSize<T>  operator -  ( const GSize<T> &sz)       { return GSize<T>(Width-sz.Width, Height-sz.Height); }
    GINLINE const GSize<T>  operator -  ( T value )                 { return GSize<T>(Width-value, Height-value); }
    GINLINE const GSize<T>  operator *  ( const GSize<T> &sz)       { return GSize<T>(Width*sz.Width, Height*sz.Height); }
    GINLINE const GSize<T>  operator *  ( SInt value )              { return GSize<T>(Width*value, Height*value); }
    //GINLINE const GSize<T>    operator *  ( T value, const GSize<T> &sz )                 { return GSize<T>(sz.Width*value, sz.Height*value); }
    GINLINE const GSize<T>  operator /  ( const GSize<T> &sz)       { return GSize<T>(Width/sz.Width, Height/sz.Height); }
    GINLINE const GSize<T>  operator /  ( T value )                 { return GSize<T>(Width/value, Height/value); }
    GINLINE const GSize<T>  operator |  ( const GSize<T> &sz)       { return GSize<T>(GTL::gmax(Width,sz.Width), GTL::gmax(Height,sz.Height)); }
    GINLINE const GSize<T>  operator &  ( const GSize<T> &sz)       { return GSize<T>(GTL::gmin(Width,sz.Width), GTL::gmin(Height,sz.Height)); }
};

/* This function uses GFC_MAX_T which is not defined
// Set the size
template <class T>
GINLINE void GSize<T>::SetSize(BoundsType bt)
{
    switch (bt)
    {
        case Min:   SetSize(0, 0); break;
        case Max:   SetSize(GFC_MAX_T, GFC_MAX_T); break;
    }
}*/


// Set to a + (b - a) * t
template <class T>
GINLINE GSize<T>& GSize<T>::SetLerp(const GSize<T>& a, const GSize<T>& b, T t)
{
    Width   = a.Width + (b.Width - a.Width) * t;
    Height  = a.Height + (b.Height - a.Height) * t;
    return *this;
}


// Swaps the two sizes
template <class T>
GINLINE void GSize<T>::Swap(GSize<T> *psz)
{
    GTL::gswap(Width, psz->Width);
    GTL::gswap(Height, psz->Height);
}

// ** End Inline Implementation


// ****************************************************************************
// Size template class
// 
template <class T>
class GPoint
{
public:
    // *** GPoint data members
    T   x;
    T   y;

    enum BoundsType { Min, Max };

    GINLINE GPoint()                     { }
    GINLINE explicit GPoint(T value)     { SetPoint(value); }
    GINLINE GPoint(T x2, T y2)           { SetPoint(x2,y2); }
    GINLINE GPoint(const GPoint<T> &pt)  { SetPoint(pt); }
    //GINLINE GPoint<T>(BoundsType bt)      { SetPoint(bt); }
                                                            
    // *** Initialization                                                                                               
    GINLINE void SetPoint(T val)                    { x=y=val; }
    GINLINE void SetPoint(T x2, T y2)           { x=x2; y=y2; }
    GINLINE void SetPoint(const GPoint<T> &pt)  { SetPoint(pt.x, pt.y); }
    GINLINE void SetPoint(BoundsType bt);                                                                
    GINLINE void Clear()                            { x=y=0; }
    GINLINE void Swap(GPoint<T> *ppt);

    // Set to a + (b - a) * t
    GINLINE GPoint<T>& SetLerp(const GPoint<T>& a, const GPoint<T>& b, T t);
                                                            
    // *** State information                                                                                            
    GINLINE bool IsNull() const { return (x==0 && y==0); }
  
    // Offset the position
    GINLINE GPoint<T>& Offset(const GPoint<T> &pt)  { return Offset(pt.x, pt.y); }
    GINLINE GPoint<T>& Offset(const GSize<T> &sz)   { return Offset(sz.Width, sz.Height); }
    GINLINE GPoint<T>& Offset(T val)                { return Offset(val,val); }
    GINLINE GPoint<T>& Offset(T x2, T y2)           { x+=x2; y+=y2; return *this; }

    // Multiply the position
    GINLINE GPoint<T>& Mul(const GPoint<T> &pt) { return Mul(pt.x, pt.y); }
    GINLINE GPoint<T>& Mul(T val)               { return Mul(val,val); }
    GINLINE GPoint<T>& Mul(T x2, T y2)          { x*=x2; y*=y2; return *this; }
    
    // Divide the position                                                      
    GINLINE GPoint<T>& Div(const GPoint<T> &pt) { return Div(pt.x, pt.y); }
    GINLINE GPoint<T>& Div(T val)               { return Div(val,val); }    
    GINLINE GPoint<T>& Div(T x2, T y2)          { x/=x2; y/=y2; return *this; }

    // Dot product
    GINLINE T Dot(T x2,T y2) const              { return (x*x2 + y*y2); }
    GINLINE T Dot(const GPoint<T> &pt) const    { return Dot(pt.x, pt.y); }
    
    // Angle in radians (between this and the passed point)
    GINLINE T Angle(T x2,T y2) const            { return (T)atan2(y2-y, x2-x); }
    GINLINE T Angle(const GPoint<T> &pt) const  { return Angle(pt.x,pt.y); }

    // Distance squared (between this and the passed point)
    GINLINE T DistanceSquared(T x2,T y2) const              { return ((x2-x)*(x2-x)+(y2-y)*(y2-y)); }
    GINLINE T DistanceSquared(const GPoint<T> &pt) const    { return DistanceSquared(pt.x, pt.y); }
    GINLINE T DistanceSquared() const                       { return Dot(*this); }

    // Distance
    GINLINE T Distance(T x2,T y2) const             { return T(sqrt(DistanceSquared(x2,y2))); }
    GINLINE T Distance(const GPoint<T> &pt) const   { return Distance(pt.x, pt.y); }
    GINLINE T Distance() const                      { return T(sqrt(DistanceSquared())); }

    // L1 Distance (Manhattan style)
    // - the equation is: fabs(x1-x2) + fabs(y1-y2)
    GINLINE T DistanceL1(T x2,T y2) const           { return T(fabs(x-x2)+fabs(y-y2)); }
    GINLINE T DistanceL1(const GPoint<T> &pt) const { return DistanceL1(pt.x, pt.y); }
    // L Infinite Distance 
    // - the equation is: max(fabs(x1-x2),fabs(y1-y2))
    GINLINE T DistanceLn(T x2,T y2) const           { return T(GTL::gmax(fabs(x-x2), fabs(y-y2))); }
    GINLINE T DistanceLn(const GPoint<T> &pt) const { return DistanceLn(pt.x, pt.y); }  

    // Clamping                                                                     
    GINLINE void Clamp(T _min, T _max)  { if (x<_min) x=_min; else if (x>_max) x=_max; 
                                          if (y<_min) y=_min; else if (y>_max) y=_max; }
    GINLINE void ClampMin(T _min)       { if (x<_min) x=_min; if (y<_min) y=_min; }
    GINLINE void ClampMax(T _max)       { if (x>_max) x=_max; if (y>_max) y=_max; }                                                 
        
    // Stores the absolute version of the point                         
    GINLINE GPoint<T>& Absolute()   { x=(T)fabs(x); y=(T)fabs(y); return *this; }

    // Bitwise comparison; return true if *this is bitwise identical to p.  
    GINLINE bool BitwiseEqual(const GPoint<T>& p) const { return memcmp(this, &p, sizeof(p)) == 0; }

    // *** Operators
    GINLINE const GPoint<T>& operator = (const GPoint<T> &pt)  { SetPoint(pt); return *this; }
    GINLINE const GPoint<T>& operator = (T val)          { SetPoint(val); return *this; }
    GINLINE const GPoint<T>& operator = (BoundsType bt)      { SetPoint(bt); return *this; }

    GINLINE const GPoint<T>   operator - () const { return GPoint<T>(-x, -y); }

    GINLINE const GPoint<T>& operator += (const GPoint<T> &pt)  { return Offset(pt); }
    GINLINE const GPoint<T>& operator += (const GSize<T> &sz)   { return Offset(sz); }
    GINLINE const GPoint<T>& operator += (T val)                { return Offset(val); }
     
    GINLINE const GPoint<T>& operator -= (const GPoint<T> &pt)  { return Offset(-pt.x, -pt.y); }
    GINLINE const GPoint<T>& operator -= (const GSize<T> &sz)   { return Offset(-sz.Width, -sz.Height); }
    GINLINE const GPoint<T>& operator -= (T val)                { return Offset(-val); }
     
    GINLINE const GPoint<T>& operator *= (const GPoint<T> &pt)  { return Mul(pt); }
    GINLINE const GPoint<T>& operator *= (T val)                { return Mul(val); }
     
    GINLINE const GPoint<T>& operator /= (const GPoint<T> &pt)  { return Div(pt); }
    GINLINE const GPoint<T>& operator /= (T val)                { return Div(val); }

    GINLINE bool operator == (const GPoint<T> &pt) const    { return ( (x==pt.x)&&(y==pt.y) ); }
    GINLINE bool operator != (const GPoint<T> &pt) const    { return ( (x!=pt.x)||(y!=pt.y) ); }

    GINLINE const GPoint<T>   operator + (const GPoint<T> &pt)  { return GPoint<T>(x+pt.x, y+pt.y); }
    GINLINE const GPoint<T>   operator + (const GSize<T> &sz)   { return GPoint<T>(x+sz.Width, y+sz.Height); }
    GINLINE const GPoint<T>   operator + (T val)                { return GPoint<T>(x+val, y+val); }
    GINLINE const GPoint<T>   operator - (const GPoint<T> &pt)  { return GPoint<T>(x-pt.x, y-pt.y); }
    GINLINE const GPoint<T>   operator - (const GSize<T> &sz)   { return GPoint<T>(x-sz.Width, y-sz.Height); }
    GINLINE const GPoint<T>   operator - (T val)                { return GPoint<T>(x-val, y-val); }
    GINLINE const GPoint<T>   operator * (const GPoint<T> &pt)  { return GPoint<T>(x*pt.x, y*pt.y); }
    GINLINE const GPoint<T>   operator * (T val)                { return GPoint<T>(x*val, y*val); }
    //GINLINE const GPoint<T>   operator * (T val, const GPoint<T> &pt)               { return GPoint<T>(pt.x*val, pt.y*val); }
    GINLINE const GPoint<T>   operator / (const GPoint<T> &pt)  { return GPoint<T>(x/pt.x, y/pt.y); }
    GINLINE const GPoint<T>   operator / (T val)                { return GPoint<T>(x/val, y/val); }
};

// ** Inline Implementation
/* This function uses GFC_MAX_T which is not defined 
template <class T>
GINLINE void GPoint<T>::SetPoint(BoundsType bt)
{
    switch (bt)
    {
    case Min:   SetPoint(0, 0); break;
    case Max:   SetPoint(GFC_MAX_T, GFC_MAX_T); break;
    }
}*/


template <class T>
GINLINE GPoint<T>& GPoint<T>::SetLerp(const GPoint<T>& a, const GPoint<T>& b, T t)
{
    x = a.x + (b.x - a.x) * t;
    y = a.y + (b.y - a.y) * t;
    return *this;
}

// Swaps the two points
template <class T>
GINLINE void GPoint<T>::Swap(GPoint<T> *ppt)
{
    GTL::gswap(x, ppt->x);
    GTL::gswap(y, ppt->y);
}

// ** End Inline Implementation


// ****************************************************************************
// Size template class
// 
template <class T>
class GRect
{
public:
    // *** GRect data members
    T   Left;
    T   Top;
    T   Right;
    T   Bottom;

    enum BoundsType { Min, Max };

    // Constructors
    GINLINE GRect( )                                         { }
    GINLINE explicit GRect(T val)                            { SetRect(val, val); }
    GINLINE GRect(const GRect<T> &rc)                        { SetRect(rc); }
    GINLINE GRect(const GSize<T> &sz)                        { SetRect(sz); }
    GINLINE GRect(T w, T h)                                  { SetRect(w, h); }
    GINLINE GRect(const GPoint<T> &tl, const GPoint<T> &br)  { SetRect(tl, br); }    
    GINLINE GRect(const GPoint<T> &tl, const GSize<T> &sz)   { SetRect(tl, sz); }
    GINLINE GRect(T x, T y, const GSize<T> &sz)              { SetRect(x, y, sz); }  
    GINLINE GRect(T left, T top, T right, T bottom)          { SetRect(left, top, right, bottom); }
    GINLINE GRect(BoundsType bt)                             { SetRect(bt); }

    // *** Utility functions

    // Initialization
    GINLINE void SetRect(T l, T t, T r, T b)                        { Left=l; Top=t; Right=r; Bottom=b; }   
    GINLINE void SetRect(const GSize<T> &sz)                        { SetRect(0, 0, sz.Width, sz.Height); }
    GINLINE void SetRect(T w, T h)                                  { SetRect(0, 0, w, h); }
    GINLINE void SetRect(const GRect<T> &rc)                        { SetRect(rc.Left, rc.Top, rc.Right, rc.Bottom); }
    GINLINE void SetRect(const GPoint<T> &tl, const GPoint<T> &br)  { SetRect(tl.x, tl.y, br.x, br.y); }
    GINLINE void SetRect(T x, T y, const GSize<T> &sz)              { SetRect(x, y, x+sz.Width, y+sz.Height); }
    GINLINE void SetRect(const GPoint<T> &tl, const GSize<T> &sz)   { SetRect(tl.x, tl.y, sz); }    
    GINLINE void SetRect(BoundsType bt);     
    
    // Set to a + (b - a) * t
    GINLINE GRect<T>& SetLerp(const GRect<T>& a, const GRect<T>& b, T t); 

    GINLINE void Clear()    { SetRect(GPoint<T>(0,0),GSize<T>(0,0)); }

    GINLINE void Swap(GRect<T> *pr);
    

    // *** Different rect data conversion
    // Point/Size rect (values are stored as a point and a size)
    GINLINE GRect<T>&   FromRectPS(const GRect<T> *prc) { SetRect(prc->Left,prc->Top,GSize<T>(prc->Right,prc->Bottom)); return *this; }
    GINLINE void        ToRectPS(GRect<T> *prc) const   { prc->SetRect(Left,Top,Width(),Height()); }


    //  *** State functions
    // - these functions are for Normal rectangles only
    GINLINE bool IsNull() const     { return (Right == Left) && (Bottom == Top); }
    GINLINE bool IsEmpty() const    { return (Left >= Right) || (Top >= Bottom); }
    GINLINE bool IsNormal() const   { return (Right >= Left) && (Bottom >= Top); }

    // Point properties
    GINLINE T X1() const    { return Left; }
    GINLINE T Y1() const    { return Top; }
    GINLINE T X2() const    { return Right; }
    GINLINE T Y2() const    { return Bottom; }

    // *** Corner position
    GINLINE GPoint<T> TopLeft() const       { return GPoint<T>(Left, Top); }
    GINLINE GPoint<T> TopRight() const      { return GPoint<T>(Right, Top); }
    GINLINE GPoint<T> BottomLeft() const    { return GPoint<T>(Left, Bottom); }
    GINLINE GPoint<T> BottomRight() const   { return GPoint<T>(Right, Bottom); }
                                                            
    GINLINE void SetTopLeft(const GPoint<T> &pt)        { Left  = pt.x; Top     = pt.y; }
    GINLINE void SetTopRight(const GPoint<T> &pt)       { Right = pt.x; Top     = pt.y; }
    GINLINE void SetBottomLeft(const GPoint<T> &pt)     { Left  = pt.x; Bottom  = pt.y; }
    GINLINE void SetBottomRight(const GPoint<T> &pt)    { Right = pt.x; Bottom  = pt.y; }
    
    // *** Side access
    // Returns a new rectangle that 
    GINLINE GRect<T> TopSide(T height = 0.0) const      { return GRect<T>(Left, Top, Right, (Top+height)); }
    GINLINE GRect<T> BottomSide(T height = 0.0) const   { return GRect<T>(Left, (Bottom-height), Right, Bottom); }
    GINLINE GRect<T> LeftSide(T width = 0.0) const      { return GRect<T>(Left, Top, (Left+width), Bottom); }
    GINLINE GRect<T> RightSide(T width = 0.0) const     { return GRect<T>((Right-width), Top, Right, Bottom); }
    
    // *** Center
    GINLINE GPoint<T> Center() const    { return GPoint<T>(HCenter(), VCenter()); }
    GINLINE T HCenter() const           { return (Right+Left)/2; }
    GINLINE T VCenter() const           { return (Bottom+Top)/2; }
                                                                        
    GINLINE void SetCenter(const GPoint<T> &nc);                   
    GINLINE void SetCenter(T x, T y)                { SetCenter(GPoint<T>(x,y)); }
    GINLINE void SetHCenter(T x);                            
    GINLINE void SetVCenter(T y);                            
                                                                        
                                                                        
    // *** Size functions                                                                                                                       
    GINLINE T Width() const     { return Right-Left; }
    GINLINE T Height() const    { return Bottom-Top; }
    // Checked functions return 0 size for non-normal coordinate        
    GINLINE T CheckedWidth() const  { return (Right>=Left) ? Width() : 0; }
    GINLINE T CheckedHeight() const { return (Bottom>=Top) ? Height() : 0; }

    GINLINE GSize<T> Size() const           { return GSize<T>(Width(), Height()); }
    GINLINE GSize<T> CheckedSize() const    { return GSize<T>(CheckedWidth(), CheckedHeight()); }
                                                            
    GINLINE void SetSize(T width, T height)     { SetWidth(width); SetHeight(height); }
    GINLINE void SetSize(const GSize<T> &sz)    { SetSize(sz.Width, sz.Height); }
                                                                        
    GINLINE void SetWidth(T width)      { Right  = (Left+width); }
    GINLINE void SetHeight(T height)    { Bottom = (Top+height); }
                                                                        
    // *** Area                                                                                                                                 
    GINLINE T Area() const  { return Width()*Height(); }                        
                        
    // *** Ranges                                                                                                               
    GINLINE void SetHRange(T l, T r)    { Left = l; Right  = r; }
    GINLINE void SetVRange(T t, T b)    { Left = t; Right  = b; }
    
    // *** Sizing/Movement
    // Offset the rect
    GINLINE GRect<T>& Offset(T x, T y)              { Left+=x; Right+=x; Top+=y; Bottom+=y; return *this; }
    GINLINE GRect<T>& Offset(const GPoint<T> &pt)   { return Offset(pt.x, pt.y); }
    GINLINE GRect<T>& OffsetX(T x)                  { Left+=x; Right+=x; return *this; }
    GINLINE GRect<T>& OffsetY(T y)                  { Top+=y; Bottom+=y; return *this; }
                        
    // Expand (inflate) rect
    GINLINE GRect<T>& Expand(T l, T t, T r, T b)    { Left-=l; Right+=r; Top-=t; Bottom+=b; return *this; }
    GINLINE GRect<T>& Expand(T val)                 { return Expand(val,val,val,val); }
    // Applies the value to both sides (h = Left-h & Right+h)                   
    GINLINE GRect<T>& Expand(T h, T v)  { return Expand(h,v,h,v); }
    GINLINE GRect<T>& HExpand(T h)      { Left-=h; Right+=h; return *this; }
    GINLINE GRect<T>& HExpand(T l, T r) { Left-=l; Right+=r; return *this; }    
    GINLINE GRect<T>& VExpand(T v)      { Top-=v; Bottom+=v; return *this; }
    GINLINE GRect<T>& VExpand(T t, T b) { Top-=t; Bottom+=b; return *this; }    

    // Expand this rectangle to enclose the given point.
    GINLINE GRect<T>& ExpandToPoint(const GPoint<T> &pt)    { return ExpandToPoint(pt.x, pt.y); }
    GINLINE GRect<T>& ExpandToPoint(T x, T y);    
                
    // Contract (deflate) rect          
    GINLINE GRect<T>& Contract(T l, T t, T r, T b)  { Left+=l; Right-=r; Top+=t; Bottom-=b; return *this; } 
    GINLINE GRect<T>& Contract(T val)               { return Contract(val,val,val,val); }
    // Applies the value to both sides (w = Left+w & Right-w)       
    GINLINE GRect<T>& Contract(T h, T v)    { return Contract(h,v,h,v); }
    GINLINE GRect<T>& HContract(T h)        { Left+=h; Right-=h; return *this; }
    GINLINE GRect<T>& HContract(T l, T r)   { Left+=l; Right-=r; return *this; }    
    GINLINE GRect<T>& VContract(T v)        { Top+=v; Bottom-=v; return *this; }
    GINLINE GRect<T>& VContract(T t, T b)   { Top+=t; Bottom-=b; return *this; }
        
    // Clamp rect
    GINLINE GRect<T>& Clamp(const GRect<T> &r)      { return Clamp(r.Left,r.Top,r.Right,r.Bottom); }
    GINLINE GRect<T>& Clamp(T l, T t, T r, T b);
    GINLINE GRect<T>& HClamp(T l, T r);
    GINLINE GRect<T>& HClamp(const GRect<T> &r)     { return HClamp(r.Left, r.Right); } 
    GINLINE GRect<T>& VClamp(T t, T b);
    GINLINE GRect<T>& VClamp(const GRect<T> &r)     { return VClamp(r.Top, r.Bottom); } 

    // Rectangle becomes the sum of this and r
    GINLINE GRect<T>& Union(T l, T t, T r, T b);
    GINLINE GRect<T>& Union(const GRect<T> &r)      { return Union(r.Left,r.Top,r.Right,r.Bottom); }

    // Stores the area inside both rectangles
    // - clears the rectangle if they don’t intersect
    GINLINE GRect<T>& Intersect(T l, T t, T r, T b);
    GINLINE GRect<T>& Intersect(const GRect<T> &r)      { return Intersect(r.Left,r.Top,r.Right,r.Bottom); }

    // Normalize the rectangle
    GINLINE void Normalize();
    GINLINE void NormalizeX();
    GINLINE void NormalizeY();
    // Returns a normalized copy 
    GINLINE GRect<T> Normal() const;

    // *** Area Testing
    // Returns 1 if the passed geometry is completely inside the rectangle
    GINLINE bool Contains(T x, T y) const               { return ((x<=Right)&&(x>=Left)&&(y<=Bottom)&&(y>=Top)); }
    GINLINE bool Contains(const GPoint<T> &pt) const    { return ((pt.x<=Right)&&(pt.x>=Left)&&(pt.y<=Bottom)&&(pt.y>=Top)); }
    GINLINE bool Contains(const GRect<T> &r) const      { return ((Right>=r.Right)&&(Bottom>=r.Bottom)&&(Left<=r.Left)&&(Top<=r.Top)); }

    // Returns 1 if rectangles overlap at all
    // - r maybe completely inside
    GINLINE bool Intersects(const GRect<T> &r) const;
    
    // Returns 1 if the rectangle touches one of the edges
    // - can optionally adjust the edge width
    GINLINE bool IntersectsEdge(const GRect<T> &r, T ewidth=1) const;
    // Handles separate widths for each sides edge
    GINLINE bool IntersectsEdge(const GRect<T> &r, T leftw, T topw, T rightw, T bottomw) const;

    // *** General calculations
    // - store new rectangle inside pdest

    // Clamps a point to the rectangle
    // - returns a new value that is inside the rect
    GINLINE GPoint<T> ClampPoint(const GPoint<T> &pt) const;

    // Calculates area inside both rectangles
    // Returns 0 if rectangles don’t intersect
    GINLINE bool IntersectRect(GRect<T> *pdest, const GRect<T> &r) const;
    // Calculates the area enclosing both rectangles
    GINLINE void UnionRect(GRect<T> *pdest, const GRect<T> &r) const;
    
    // *** Operator overrides
    GINLINE const GRect<T>& operator =  (const GRect<T> &r) { SetRect(r); return *this; }
    GINLINE const GRect<T>& operator =  (BoundsType bt)     { SetRect(bt); return *this; }
                
    GINLINE const GRect<T>& operator += (const GPoint<T> &pt)   { return Offset(pt); }  
    GINLINE const GRect<T>& operator += (T val)                 { return Offset(val,val); }
          
    GINLINE GRect<T>& operator -= (const GPoint<T> &pt) { return Offset(-pt); } 
    GINLINE GRect<T>& operator -= (T val)                   { return Offset(-val,-val); }
          
    GINLINE const GRect<T>& operator |= (const GRect<T> &r);
    GINLINE const GRect<T>& operator &= (const GRect<T> &r);

    GINLINE bool operator == (const GRect<T> &r) const       { return ((Left==r.Left)&&(Right==r.Right)&&(Top==r.Top)&&(Bottom==r.Bottom) ); }
    GINLINE bool operator != (const GRect<T> &r) const       { return ((Left!=r.Left)||(Right!=r.Right)||(Top!=r.Top)||(Bottom!=r.Bottom) ); }

    GINLINE const GRect<T>    operator -  ()                        { return GRect<T>(-Left,-Top,-Right,-Bottom); }
    GINLINE const GRect<T>    operator +  ( const GPoint<T> &pt)    { return GRect<T>(*this).Offset(pt); }
    GINLINE const GRect<T>    operator +  (T val)                   { return GRect<T>(Left+val, Top+val, Right+val, Bottom+val); }
    GINLINE const GRect<T>    operator -  (const GPoint<T> &pt)     { return GRect<T>(*this).Offset(-pt); }
    GINLINE const GRect<T>    operator -  (T val)                   { return GRect<T>(Left-val, Top-val, Right-val, Bottom-val); }
    GINLINE const GRect<T>    operator +  (const GRect<T> &r)       { GRect<T> dest; UnionRect(&dest, r); return dest; }                                                                                  
    GINLINE const GRect<T>    operator |  (const GRect<T> &r)       { GRect<T> dest; UnionRect(&dest, r); return dest; }
    GINLINE const GRect<T>    operator &  (const GRect<T> &r)       { GRect<T> dest; IntersectRect(&dest, r); return dest; }
};


// ** Inline Implementation
template <class T>
GINLINE void GRect<T>::SetRect(BoundsType bt)
{
    switch (bt)
    {
        case Min:   SetRect(GPoint<T>(GPoint<T>::Min), GSize<T>(GSize<T>::Min)); break;
        case Max:   SetRect(GPoint<T>(GPoint<T>::Min), GSize<T>(GSize<T>::Max)); break;
    }
}

// Set to a + (b - a) * t
template <class T>
GINLINE GRect<T>& GRect<T>::SetLerp(const GRect<T>& a, const GRect<T>& b, T t)
{
    Left    = (b.Left - a.Left) * t + a.Left;
    Top     = (b.Top - a.Top) * t + a.Top;
    Right   = (b.Right - a.Right) * t + a.Right;
    Bottom  = (b.Bottom - a.Bottom) * t + a.Bottom;
    return *this;
}

// Swaps the two rectangles
template <class T>
GINLINE void GRect<T>::Swap(GRect<T> *pr)
{
    GTL::gswap(Left,   pr->Left);
    GTL::gswap(Right,  pr->Right);
    GTL::gswap(Top,   pr->Top);
    GTL::gswap(Bottom, pr->Bottom);
}

// Clamp rect
template <class T>
GINLINE GRect<T>& GRect<T>::Clamp(T left, T top, T right, T bottom)
{
    if (left>Left)      Left    =left;
    if (top>Top)        Top     =top;
    if (right<Right)    Right   =right;
    if (bottom<Bottom)  Bottom  =bottom;
    return *this;
}

template <class T>
GINLINE GRect<T>& GRect<T>::HClamp(T left, T right)
{
    if (left>Left)      Left    =left;
    if (right<Right)    Right   =right;
    return *this;
}

template <class T>
GINLINE GRect<T>& GRect<T>::VClamp(T top, T bottom)
{
    if (top>Top)        Top     =top;
    if (bottom<Bottom)  Bottom  =bottom;
    return *this;
}

template <class T>
GINLINE GRect<T>& GRect<T>::ExpandToPoint(T x, T y) 
{
    Left    = GTL::gmin<T>(Left, x);
    Top     = GTL::gmin<T>(Top, y);
    Right   = GTL::gmax<T>(Right, x);
    Bottom  = GTL::gmax<T>(Bottom, y);
    return *this;
}


// Stores the sum of this and r
template <class T>
GINLINE GRect<T>& GRect<T>::Union(T left, T top, T right, T bottom)
{
    GRect<T> ur;
    UnionRect(&ur, GRect<T>(left, top, right, bottom));
    SetRect(ur);
    return *this;
}

template <class T>
GINLINE GRect<T>& GRect<T>::Intersect(T left, T top, T right, T bottom)
{
    GRect<T> ir;
    if (!IntersectRect(&ir, GRect<T>(left, top, right, bottom)))
        Clear();
    else
        SetRect(ir);
    return *this;
}


// Normalize the rectangle
template <class T>
GINLINE void    GRect<T>::Normalize()
{       
    if (Left>Right) GTL::gswap(Left, Right);
    if (Top>Bottom) GTL::gswap(Top, Bottom);
}
template <class T>
GINLINE void    GRect<T>::NormalizeX()
{       
    if (Left>Right) GTL::gswap(Left, Right);    
}
template <class T>
GINLINE void    GRect<T>::NormalizeY()
{       
    if (Top>Bottom) GTL::gswap(Top, Bottom);
}


// Normalize the rectangle
template <class T>
GINLINE GRect<T>  GRect<T>::Normal() const
{       
    GRect<T> r(*this);
    r.Normalize();
    return r;
}

// Sets the center
template <class T>
GINLINE void    GRect<T>::SetCenter(const GPoint<T> &nc)
{
    GPoint<T> oc = Center();
    Offset(nc.x-oc.x, nc.y-oc.y);
}

// Sets the horizontal center
template <class T>
GINLINE void    GRect<T>::SetHCenter(T x)
{ Offset(x-HCenter(), 0); }

// Sets the vertical center
template <class T>
GINLINE void    GRect<T>::SetVCenter(T y)
{ Offset(0, y-VCenter()); }


// Returns 1 if the rectangles overlap
template <class T>
GINLINE bool    GRect<T>::Intersects(const GRect<T> &r) const
{
    if ( (Bottom>=r.Top)&&(r.Bottom>=Top) )
        if ( (r.Right>=Left)&&(Right>=r.Left) )
            return 1;
    return 0;
}

// Returns 1 if the passed rectangle overlaps the edge
template <class T>
GINLINE bool    GRect<T>::IntersectsEdge(const GRect<T> &r, T ew) const
{
    return IntersectsEdge(r, ew, ew, ew, ew);
}

template <class T>
GINLINE bool    GRect<T>::IntersectsEdge(const GRect<T> &r, T lw, T tw, T rw, T bw) const
{
    GRect<T> inside = *this;
    inside.Contract(lw, tw, rw, bw);
    return (Intersects(r) && !inside.Contains(r));
}


// Calculates area inside both GRectangles
// Return 0 if GRectangles don’t intersect
template <class T>
GINLINE bool    GRect<T>::IntersectRect(GRect<T> *pdest, const GRect<T> &r) const
{
    if (!Intersects(r)) return 0;
    pdest->Left     = (Left>r.Left)     ? Left      : r.Left;
    pdest->Right    = (Right>r.Right)   ? r.Right   : Right;
    pdest->Top      = (Top>r.Top)       ? Top       : r.Top;
    pdest->Bottom   = (Bottom>r.Bottom) ? r.Bottom  : Bottom;
    return 1;
}

// Calculates the area enclosing both GRectangles
template <class T>
GINLINE void    GRect<T>::UnionRect(GRect<T> *pdest, const GRect<T> &r) const
{
    pdest->Left     = (Left>r.Left)     ? r.Left    : Left;
    pdest->Right    = (Right>r.Right)   ? Right     : r.Right;
    pdest->Top      = (Top>r.Top)       ? r.Top     : Top;
    pdest->Bottom   = (Bottom>r.Bottom) ? Bottom    : r.Bottom;
}

template <class T>
GINLINE GPoint<T> GRect<T>::ClampPoint(const GPoint<T> &pt) const
{
    GPoint<T> npt;
    npt.x = (Left>=pt.x) ? Left : (Right<=pt.x) ? Right : pt.x;
    npt.y = (Top>=pt.y) ? Top : (Bottom<=pt.y) ? Bottom : pt.y;
    return npt;
}

template <class T>
GINLINE const GRect<T>& GRect<T>::operator |= (const GRect<T> &r)
{
    UnionRect(this, r); 
    return *this;
}

template <class T>
GINLINE const GRect<T>& GRect<T>::operator &= (const GRect<T> &r)
{
    if (!IntersectRect(this, r))
        this->Clear();
    return *this;
}
                                                                                           
// ** End Inline Implementation

// ****************************************************************************
// Backwards compatible typedefs 
// 

// Float structures 
typedef GPoint<Float> GPointF;
typedef GSize<Float> GSizeF;
typedef GRect<Float> GRectF;

// Double structures
typedef GPoint<Double> GPointD;
typedef GSize<Double> GSizeD;
typedef GRect<Double> GRectD;

#endif // GFC_NO_TYPES2D_FLOAT

#endif
