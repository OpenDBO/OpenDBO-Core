//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SQUARE_ROOT_INCLUDED
#define SQUARE_ROOT_INCLUDED

inline __declspec(naked) float __fastcall
SquareRoot(float value)
{
    __asm
    {
        fld   DWORD PTR [esp+4]
        fsqrt
        ret 4
    }
}

//inline __declspec(naked) float __fastcall
//InverseSquareRoot_NotZero(float a)
//{
//    __asm
//    {
//        mov   eax,0be6eb508h
//        mov   DWORD PTR [esp-12],03fc00000h // 1.5 on the stack
//        sub   eax, DWORD PTR [esp+4]        // a
//        sub   DWORD PTR [esp+4],800000h     // a/2 a=Y0
//        shr   eax,1                         // first approx. in eax=R0
//        mov   DWORD PTR [esp-8],eax
//
//        fld   DWORD PTR [esp-8]             // r
//        fmul  st,st                         // r*r
//        fld   DWORD PTR [esp-8]             // r
//        fxch  st(1)
//        fmul  DWORD PTR [esp+4]             // a ; r*r*y0
//        fld   DWORD PTR [esp-12]            // load 1.5
//        fld   st(0)
//        fsub  st,st(2)                      // r1 = 1.5 - y1
//                                            // x1 = st(3)
//                                            // y1 = st(2)
//                                            // 1.5 = st(1)
//                                            // r1 = st(0)
//        fld   st(1)
//        fxch  st(1)
//        fmul  st(3),st                      // y2=y1*r1*...
//        fmul  st(3),st                      // y2=y1*r1*r1
//        fmulp st(4),st                      // x2=x1*r1
//        fsub  st,st(2)                      // r2=1.5-y2
//                                            // x2=st(3)
//                                            // y2=st(2)
//                                            // 1.5=st(1)
//                                            // r2 = st(0)
//        fmul  st(2),st                      // y3=y2*r2*...
//        fmul  st(3),st                      // x3=x2*r2
//        fmulp st(2),st                      // y3=y2*r2*r2
//        fxch  st(1)
//        fsubp st(1),st                      // r3= 1.5 - y3
//                                            // x3 = st(1)
//                                            // r3 = st(0)
//        fmulp  st(1),st
//        ret 4
//    }
//}
//
//
////inline float sqrtInverse2(float n) { // Appears slower than ASM above.
////
////  float halfn = .5f*n;
////
////  int i = *(int *)&n;
////
////  i = 0x5f375a86 - (i>>1); // 0x5f3759df: original number.
////
////  n = *(float *)&i;
////
////  n = n*(1.5f-halfn*n*n);
////
////  return n;
////
////} // sqrtInverse2
//
//inline float
//InverseSquareRoot(float n)
//{
//    if(n > 0.f)
//    {
//        return InverseSquareRoot_NotZero(n);
//    }
//    else
//    {
//        return 0.f;
//    }
//}
//inline float
//SquareRoot(float n)
//{
//    if(n > 0.f)
//    {
//        return n * InverseSquareRoot_NotZero(n);
//    }
//    else
//    {
//        return 0.f;
//    }
//}

#endif
