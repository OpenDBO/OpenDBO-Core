// Minimal d3dx9math.h for x64 compatibility

#ifndef D3DX9MATH_H
#define D3DX9MATH_H

#include <math.h>
#include <d3d9.h>

#ifndef D3DX_PI
#define D3DX_PI 3.14159265358979323846f
#endif
#define D3DXToRadian(degree) ((degree) * (D3DX_PI / 180.0f))
#define D3DXToDegree(radian) ((radian) * (180.0f / D3DX_PI))

#ifdef __cplusplus

// D3DXPLANE
struct D3DXPLANE {
    float a, b, c, d;
};

// D3DXVECTOR2
struct D3DXVECTOR2 {
    float x, y;
    D3DXVECTOR2() : x(0), y(0) {}
    D3DXVECTOR2(float _x, float _y) : x(_x), y(_y) {}
    D3DXVECTOR2(const float* f) : x(f[0]), y(f[1]) {}
};

// D3DXVECTOR3
struct D3DXVECTOR3 {
    float x, y, z;
    D3DXVECTOR3() : x(0), y(0), z(0) {}
    D3DXVECTOR3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    D3DXVECTOR3(const float* f) : x(f[0]), y(f[1]), z(f[2]) {}
    D3DXVECTOR3 operator+(const D3DXVECTOR3& v) const { return D3DXVECTOR3(x+v.x, y+v.y, z+v.z); }
    D3DXVECTOR3 operator-(const D3DXVECTOR3& v) const { return D3DXVECTOR3(x-v.x, y-v.y, z-v.z); }
    D3DXVECTOR3 operator*(float s) const { return D3DXVECTOR3(x*s, y*s, z*s); }
    D3DXVECTOR3 operator/(float s) const { return D3DXVECTOR3(x/s, y/s, z/s); }
    D3DXVECTOR3& operator+=(const D3DXVECTOR3& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
    D3DXVECTOR3& operator-=(const D3DXVECTOR3& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
};

// D3DXVECTOR4
struct D3DXVECTOR4 {
    float x, y, z, w;
    D3DXVECTOR4() : x(0), y(0), z(0), w(0) {}
    D3DXVECTOR4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    D3DXVECTOR4(const float* f) : x(f[0]), y(f[1]), z(f[2]), w(f[3]) {}
};

// D3DXQUATERNION
struct D3DXQUATERNION {
    float x, y, z, w;
    D3DXQUATERNION() : x(0), y(0), z(0), w(1) {}
    D3DXQUATERNION(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
};

// D3DXMATRIX
struct D3DXMATRIX : public D3DMATRIX {
    D3DXMATRIX() { for (int i = 0; i < 16; i++) ((float*)m)[i] = 0; }
};

inline D3DXVECTOR3 operator*(float s, const D3DXVECTOR3& v) { return D3DXVECTOR3(v.x*s, v.y*s, v.z*s); }

typedef __declspec(align(16)) D3DXMATRIX D3DXMATRIXA16, *LPD3DXMATRIXA16;

typedef D3DXVECTOR2 *LPD3DXVECTOR2;
typedef D3DXVECTOR3 *LPD3DXVECTOR3;
typedef D3DXVECTOR4 *LPD3DXVECTOR4;
typedef D3DXQUATERNION *LPD3DXQUATERNION;
typedef D3DXMATRIX *LPD3DXMATRIX;
typedef D3DXPLANE *LPD3DXPLANE;

#else

typedef struct D3DXPLANE { float a, b, c, d; } D3DXPLANE, *LPD3DXPLANE;
typedef struct D3DXVECTOR2 { float x, y; } D3DXVECTOR2, *LPD3DXVECTOR2;
typedef struct D3DXVECTOR3 { float x, y, z; } D3DXVECTOR3, *LPD3DXVECTOR3;
typedef struct D3DXVECTOR4 { float x, y, z, w; } D3DXVECTOR4, *LPD3DXVECTOR4;
typedef struct D3DXQUATERNION { float x, y, z, w; } D3DXQUATERNION, *LPD3DXQUATERNION;
typedef struct D3DXMATRIX { float _11,_12,_13,_14,_21,_22,_23,_24,_31,_32,_33,_34,_41,_42,_43,_44; } D3DXMATRIX, *LPD3DXMATRIX;
typedef D3DXMATRIX D3DXMATRIXA16, *LPD3DXMATRIXA16;

#endif

#ifdef __cplusplus

typedef __declspec(align(16)) D3DXMATRIX D3DXMATRIXA16, *LPD3DXMATRIXA16;

typedef D3DXVECTOR2 *LPD3DXVECTOR2;
typedef D3DXVECTOR3 *LPD3DXVECTOR3;
typedef D3DXVECTOR4 *LPD3DXVECTOR4;
typedef D3DXQUATERNION *LPD3DXQUATERNION;
typedef D3DXMATRIX *LPD3DXMATRIX;
typedef D3DXPLANE *LPD3DXPLANE;

inline float* D3DXMatrixToFloatPtr(const D3DXMATRIX* m) { return (float*)m->m; }

inline D3DXVECTOR3* D3DXVec3Subtract(D3DXVECTOR3* out, const D3DXVECTOR3* v1, const D3DXVECTOR3* v2) {
    out->x = v1->x - v2->x; out->y = v1->y - v2->y; out->z = v1->z - v2->z; return out;
}

inline D3DXVECTOR3* D3DXVec3Add(D3DXVECTOR3* out, const D3DXVECTOR3* v1, const D3DXVECTOR3* v2) {
    out->x = v1->x + v2->x; out->y = v1->y + v2->y; out->z = v1->z + v2->z; return out;
}

inline D3DXVECTOR3* D3DXVec3Scale(D3DXVECTOR3* out, const D3DXVECTOR3* v, float s) {
    out->x = v->x * s; out->y = v->y * s; out->z = v->z * s; return out;
}

inline float D3DXVec3Length(const D3DXVECTOR3* v) { return sqrtf(v->x*v->x + v->y*v->y + v->z*v->z); }
inline float D3DXVec3LengthSq(const D3DXVECTOR3* v) { return v->x*v->x + v->y*v->y + v->z*v->z; }

inline void D3DXVec3Normalize(D3DXVECTOR3* out, const D3DXVECTOR3* v) {
    float l = D3DXVec3Length(v); if (l > 0) { out->x=v->x/l; out->y=v->y/l; out->z=v->z/l; }
}

inline D3DXVECTOR3* D3DXVec3Cross(D3DXVECTOR3* out, const D3DXVECTOR3* v1, const D3DXVECTOR3* v2) {
    out->x = v1->y*v2->z - v1->z*v2->y; out->y = v1->z*v2->x - v1->x*v2->z; out->z = v1->x*v2->y - v1->y*v2->x;
    return out;
}

inline float D3DXVec3Dot(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2) {
    return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

inline D3DXVECTOR4* D3DXVec4Transform(D3DXVECTOR4* out, const D3DXVECTOR4* v, const D3DXMATRIX* m) {
    out->x = m->_11*v->x + m->_21*v->y + m->_31*v->z + m->_41*v->w;
    out->y = m->_12*v->x + m->_22*v->y + m->_32*v->z + m->_42*v->w;
    out->z = m->_13*v->x + m->_23*v->y + m->_33*v->z + m->_43*v->w;
    out->w = m->_14*v->x + m->_24*v->y + m->_34*v->z + m->_44*v->w;
    return out;
}

inline D3DXVECTOR3* D3DXVec3Transform(D3DXVECTOR3* out, const D3DXVECTOR3* v, const D3DXMATRIX* m) {
    out->x = m->_11*v->x + m->_21*v->y + m->_31*v->z + m->_41;
    out->y = m->_12*v->x + m->_22*v->y + m->_32*v->z + m->_42;
    out->z = m->_13*v->x + m->_23*v->y + m->_33*v->z + m->_43;
    return out;
}

inline D3DXVECTOR4* D3DXVec3Transform(D3DXVECTOR4* out, const D3DXVECTOR3* v, const D3DXMATRIX* m) {
    out->x = m->_11*v->x + m->_21*v->y + m->_31*v->z + m->_41;
    out->y = m->_12*v->x + m->_22*v->y + m->_32*v->z + m->_42;
    out->z = m->_13*v->x + m->_23*v->y + m->_33*v->z + m->_43;
    out->w = m->_14*v->x + m->_24*v->y + m->_34*v->z + m->_44;
    return out;
}

inline D3DXVECTOR3* D3DXVec3TransformCoord(D3DXVECTOR3* out, const D3DXVECTOR3* v, const D3DXMATRIX* m) {
    float w = m->_14*v->x + m->_24*v->y + m->_34*v->z + m->_44;
    out->x = (m->_11*v->x + m->_21*v->y + m->_31*v->z + m->_41) / w;
    out->y = (m->_12*v->x + m->_22*v->y + m->_32*v->z + m->_42) / w;
    out->z = (m->_13*v->x + m->_23*v->y + m->_33*v->z + m->_43) / w;
    return out;
}

inline D3DXVECTOR3* D3DXVec3TransformNormal(D3DXVECTOR3* out, const D3DXVECTOR3* v, const D3DXMATRIX* m) {
    out->x = m->_11*v->x + m->_21*v->y + m->_31*v->z;
    out->y = m->_12*v->x + m->_22*v->y + m->_32*v->z;
    out->z = m->_13*v->x + m->_23*v->y + m->_33*v->z;
    return out;
}

inline D3DXVECTOR3* D3DXVec3CatmullRom(D3DXVECTOR3* out, const D3DXVECTOR3* v0, const D3DXVECTOR3* v1, const D3DXVECTOR3* v2, const D3DXVECTOR3* v3, float s) {
    float s2=s*s, s3=s2*s;
    out->x = 0.5f*(2*v1->x+(-v0->x+v2->x)*s+(2*v0->x-5*v1->x+4*v2->x-v3->x)*s2+(-v0->x+3*v1->x-3*v2->x+v3->x)*s3);
    out->y = 0.5f*(2*v1->y+(-v0->y+v2->y)*s+(2*v0->y-5*v1->y+4*v2->y-v3->y)*s2+(-v0->y+3*v1->y-3*v2->y+v3->y)*s3);
    out->z = 0.5f*(2*v1->z+(-v0->z+v2->z)*s+(2*v0->z-5*v1->z+4*v2->z-v3->z)*s2+(-v0->z+3*v1->z-3*v2->z+v3->z)*s3);
    return out;
}

inline D3DXMATRIX* D3DXMatrixIdentity(D3DXMATRIX* out) {
    for (int i = 0; i < 16; i++) ((float*)out)[i] = 0;
    out->_11=out->_22=out->_33=out->_44 = 1;
    return out;
}

inline D3DXMATRIX* D3DXMatrixMultiply(D3DXMATRIX* out, const D3DXMATRIX* a, const D3DXMATRIX* b) {
    D3DXMATRIX t;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++) {
            float s = 0;
            for (int k = 0; k < 4; k++) s += D3DXMatrixToFloatPtr(a)[i*4+k] * D3DXMatrixToFloatPtr(b)[k*4+j];
            D3DXMatrixToFloatPtr(&t)[i*4+j] = s;
        }
    *out = t; return out;
}

inline D3DXMATRIX operator*(const D3DXMATRIX& a, const D3DXMATRIX& b) { D3DXMATRIX r; D3DXMatrixMultiply(&r, &a, &b); return r; }

inline D3DXMATRIX* D3DXMatrixRotationZ(D3DXMATRIX* out, float a) {
    float s=sinf(a), c=cosf(a);
    for (int i = 0; i < 16; i++) ((float*)out)[i] = 0;
    out->_11=c;out->_12=s;out->_21=-s;out->_22=c;out->_33=1;out->_44=1;
    return out;
}

inline D3DXMATRIX* D3DXMatrixRotationX(D3DXMATRIX* out, float a) {
    float s=sinf(a), c=cosf(a);
    for (int i = 0; i < 16; i++) ((float*)out)[i] = 0;
    out->_11=1;out->_22=c;out->_23=s;out->_32=-s;out->_33=c;out->_44=1;
    return out;
}

inline D3DXMATRIX* D3DXMatrixRotationY(D3DXMATRIX* out, float a) {
    float s=sinf(a), c=cosf(a);
    for (int i = 0; i < 16; i++) ((float*)out)[i] = 0;
    out->_11=c;out->_13=-s;out->_22=1;out->_31=s;out->_33=c;out->_44=1;
    return out;
}

inline D3DXMATRIX* D3DXMatrixTranslation(D3DXMATRIX* out, float x, float y, float z) {
    D3DXMatrixIdentity(out); out->_41=x; out->_42=y; out->_43=z;
    return out;
}

inline D3DXMATRIX* D3DXMatrixRotationYawPitchRoll(D3DXMATRIX* out, float yaw, float pitch, float roll) {
    D3DXMATRIX ry, rp, rr;
    D3DXMatrixRotationY(&ry, yaw);
    D3DXMatrixRotationX(&rp, pitch);
    D3DXMatrixRotationZ(&rr, roll);
    D3DXMATRIX temp;
    D3DXMatrixMultiply(&temp, &ry, &rp);
    D3DXMatrixMultiply(out, &temp, &rr);
    return out;
}

inline D3DXMATRIX* D3DXMatrixLookAtLH(D3DXMATRIX* out, const D3DXVECTOR3* eye, const D3DXVECTOR3* at, const D3DXVECTOR3* up) {
    D3DXVECTOR3 za, xa, ya;
    D3DXVec3Subtract(&za, at, eye); D3DXVec3Normalize(&za, &za);
    D3DXVec3Cross(&xa, up, &za); D3DXVec3Normalize(&xa, &xa);
    D3DXVec3Cross(&ya, &za, &xa);
    out->_11=xa.x;out->_12=ya.x;out->_13=za.x;out->_14=0;
    out->_21=xa.y;out->_22=ya.y;out->_23=za.y;out->_24=0;
    out->_31=xa.z;out->_32=ya.z;out->_33=za.z;out->_34=0;
    out->_41=-D3DXVec3Dot(&xa,eye);out->_42=-D3DXVec3Dot(&ya,eye);out->_43=-D3DXVec3Dot(&za,eye);out->_44=1;
    return out;
}

inline D3DXMATRIX* D3DXMatrixPerspectiveFovLH(D3DXMATRIX* out, float fovY, float aspect, float zn, float zf) {
    float y = 1.0f / tanf(fovY/2), x = y/aspect;
    for (int i = 0; i < 16; i++) ((float*)out)[i] = 0;
    out->_11=x;out->_22=y;out->_33=zf/(zf-zn);out->_34=1;out->_43=-zn*zf/(zf-zn);
    return out;
}

inline D3DXQUATERNION* D3DXQuaternionRotationAxis(D3DXQUATERNION* out, const D3DXVECTOR3* axis, float angle) {
    float s = sinf(angle/2); out->x=axis->x*s; out->y=axis->y*s; out->z=axis->z*s; out->w=cosf(angle/2);
    return out;
}

inline D3DXQUATERNION* D3DXQuaternionSlerp(D3DXQUATERNION* out, const D3DXQUATERNION* q1, const D3DXQUATERNION* q2, float t) {
    float cosOmega = q1->x*q2->x+q1->y*q2->y+q1->z*q2->z+q1->w*q2->w;
    float k0, k1;
    if (cosOmega < 0) { cosOmega = -cosOmega; k0 = -1; } else k0 = 1;
    float k = 1 - cosOmega*cosOmega;
    if (k > 0.0001f) {
        float sinOmega = sqrtf(k), omega = atan2f(sinOmega, cosOmega);
        float isinOmega = 1/sinOmega;
        k0 = sinf((1-t)*omega)*isinOmega*k0; k1 = sinf(t*omega)*isinOmega;
    } else { k0 *= (1-t); k1 = t; }
    out->x = q1->x*k0+q2->x*k1; out->y = q1->y*k0+q2->y*k1;
    out->z = q1->z*k0+q2->z*k1; out->w = q1->w*k0+q2->w*k1;
    return out;
}

inline D3DXPLANE* D3DXPlaneNormalize(D3DXPLANE* out, const D3DXPLANE* p) {
    float n = sqrtf(p->a*p->a + p->b*p->b + p->c*p->c);
    if (n) { out->a = p->a/n; out->b = p->b/n; out->c = p->c/n; out->d = p->d/n; }
    return out;
}

inline D3DXPLANE* D3DXPlaneFromPoints(D3DXPLANE* out, const D3DXVECTOR3* v1, const D3DXVECTOR3* v2, const D3DXVECTOR3* v3) {
    D3DXVECTOR3 e1, e2, n;
    D3DXVec3Subtract(&e1, v2, v1); D3DXVec3Subtract(&e2, v3, v1);
    D3DXVec3Cross(&n, &e1, &e2);
    out->a = n.x; out->b = n.y; out->c = n.z;
    out->d = -(n.x*v1->x + n.y*v1->y + n.z*v1->z);
    return out;
}

#endif // __cplusplus (inline functions)

#endif // D3DX9MATH_H
