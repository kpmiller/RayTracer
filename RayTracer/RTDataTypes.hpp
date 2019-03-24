//
//  RTDataTypes.hpp
//  RayTracer
//
//  Created by Kent Miller on 3/3/19.
//  Copyright © 2019 Kent Miller. All rights reserved.
//

#ifndef RTDataTypes_hpp
#define RTDataTypes_hpp

#include <stdio.h>
#include <math.h>

class vec3 {
public:
    vec3() {}
    vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2;}
    inline float x() const {return e[0];}
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}
    inline float r() const {return e[0];}
    inline float g() const {return e[1];}
    inline float b() const {return e[2];}
    
    inline const vec3& operator+() const { return *this;}
    inline vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
    inline float operator[] (int i) const { return e[i];}
    inline float& operator[] (int i) {return e[i];}
    
    inline vec3& operator +=(const vec3 &v2);
    inline vec3& operator -=(const vec3 &v2);
    inline vec3& operator *=(const vec3 &v2);
    inline vec3& operator /=(const vec3 &v2);
    inline vec3& operator *=(const float t);
    inline vec3& operator /=(const float t);
    
    inline float length() const {
        return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    inline float squared_length() const {
        return (e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }
    
    inline void make_unit_vector();

    float e[3];


};

inline vec3 operator+(const vec3 &v1, const vec3 &v2) { return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);}
inline vec3 operator-(const vec3 &v1, const vec3 &v2) { return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);}
inline vec3 operator*(const vec3 &v1, const vec3 &v2) { return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);}
inline vec3 operator/(const vec3 &v1, const vec3 &v2) { return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);}

inline vec3 operator*(float t, const vec3 &v) { return vec3(t*v[0], t*v[1], t*v[2]);}
inline vec3 operator/(float t, const vec3 &v) { return vec3(v[0]/t, v[1]/t, v[2]/t);}
inline vec3 operator*(const vec3 &v, float t) { return vec3(t*v[0], t*v[1], t*v[2]);}
inline vec3 operator/(const vec3 &v, float t) { return vec3(v[0]/t, v[1]/t, v[2]/t);}

inline float dot(const vec3 &v1, const vec3 &v2) {return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];}
inline vec3 cross(const vec3 &v1, const vec3 &v2) {
    return vec3(
        (v1.e[1] * v2.e[2] - v1.e[2]*v2.e[1]),
        -(v1.e[0] * v2.e[2] - v1.e[2]*v2.e[0]),
        (v1.e[0] * v2.e[1] - v1.e[1]*v2.e[0])
    );
    }

inline vec3& vec3::operator+=(const vec3 &v) { e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2]; return (*this);}
inline vec3& vec3::operator*=(const vec3 &v) { e[0] *= v.e[0]; e[1] *= v.e[1]; e[2] *= v.e[2]; return (*this);}
inline vec3& vec3::operator/=(const vec3 &v) { e[0] /= v.e[0]; e[1] /= v.e[1]; e[2] /= v.e[2]; return (*this);}
inline vec3& vec3::operator*=(float t) { e[0] *= t; e[1] *= t; e[2] *= t; return (*this);}
inline vec3& vec3::operator/=(float k) { float t=(1.0f/k); e[0] *= t; e[1] *= t; e[2] *= t; return (*this);}

inline vec3 unit_vector(vec3 v) { return v/ v.length();}

inline vec3 reflect(const vec3&v, const vec3&n) {
    return v-2*dot(v,n)*n;
}

inline bool refract(const vec3&v, const vec3&n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv,n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv-n*dt) - n*sqrt(discriminant);
        return true;
    }
    else
        return false;
}

inline float schlick(float cosine, float ref_idx) {
    float r0 = (1.0-ref_idx) / (1.0+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1-cosine), 5);
}


class ray
{
public:
    ray() {}
    ray(const vec3&a, const vec3&b) {A = a; B = b;}
    vec3 origin() const {return A;}
    vec3 direction() const {return B;}
    vec3 point_at_parameter(float t) const {return A + t*B;}
    vec3 A;
    vec3 B;
};

vec3 random_in_unit_sphere();

#endif /* RTDataTypes_hpp */
