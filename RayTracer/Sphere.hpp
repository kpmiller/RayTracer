//
//  Sphere.hpp
//  RayTracer
//
//  Created by Kent Miller on 3/9/19.
//  Copyright © 2019 Kent Miller. All rights reserved.
//

#ifndef Sphere_h
#define Sphere_h

#include "Hitable.hpp"
#include "Material.hpp"

class sphere: public hitable {
public:
    sphere() {}
    sphere (vec3 cen, float r, material *mat) : center(cen), radius(r), mat_ptr(mat) {};
    virtual bool hit(const ray& r, float t_min, float tmax, hit_record& rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    
    vec3 oc = r.origin() - center;
    float a = dot (r.direction(), r.direction());
    float b = 2.0f * dot(oc, r.direction());
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4.0f*a*c;
    if (discriminant > 0.0f)
    {
        float temp = (-b - sqrt(discriminant)) / (2.0f*a);
        if (temp < t_max && temp > t_min)  // - root
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / (2.0f*a);
        if (temp < t_max && temp > t_min)  // + root
        {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
};

#endif /* Sphere_h */
