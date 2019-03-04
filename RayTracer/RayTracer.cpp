//
//  RayTracer.cpp
//  RayTracer
//
//  Created by Kent Miller on 3/3/19.
//  Copyright © 2019 Kent Miller. All rights reserved.
//

#include "RayTracer.hpp"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "RTDataTypes.hpp"


RTimage * CreateRTimage(int width, int height)
{
    RTimage *rt = (RTimage*) malloc(sizeof(RTimage));
    rt->data    = (uint8_t*) malloc(height*width*sizeof(float)*3);
    rt->width   = width;
    rt->height  = height;

    float *f = (float*) rt->data;
    for (int y = 0; y < height; y++)
    {
        float yv = (float) y / (float)height;
        for (int x=0; x< width; x++)
        {
            f[0] = (float) x / (float)width;
            f[1] = yv;
            f[2] = 0.2f;
            f+=3;
        }
    }
    rt->changed = 1;
    return rt;
}

float hit_sphere(const vec3& center, float radius, const ray&r)
{
    vec3 oc = r.origin() - center;
    float a = dot (r.direction(), r.direction());
    float b = 2.0f * dot(oc, r.direction());
    float c = dot(oc,oc) - radius*radius;
    float discriminant = b*b - 4.0f*a*c;
    if (discriminant < 0)
        return -1.0f;
    else
        return (-b - sqrt(discriminant)) / (2.0f*a);
}

vec3 color(const ray& r)
{
    float t = hit_sphere(vec3(0.0f,0.0f,-1.0f), 0.5, r);
    if (t > 0.0f)
    {
        vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0.0f, 0.0f, -1.0f));
        return 0.5 * (vec3(N.x() + 1.0f, N.y() + 1.0f, N.z() + 1.0f));
    }
    vec3 unit_direction  = unit_vector(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t) * vec3(1.0,1.0,1.0) + t*vec3(0.5, 0.7, 1.0);
}

void Trace(RTimage *rt)
{
    float nx = (float) rt->width;
    float ny = (float) rt->height;
    
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin (0.0, 0.0, 0.0);
    
    float *fb = (float *) rt->data;

    for (int j = rt->height-1; j >= 0; j--)
    {
        float *f = &fb[(rt->height-1-j)*rt->width*3];  //flip
        float  v = (float) j / ny;
        for (int i = 0; i < rt->width; i++)
        {
            float u = float(i) / nx;
            ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            
            vec3 col = color(r);
            
            f[0] = col[0];
            f[1] = col[1];
            f[2] = col[2];
            f += 3;
        }
        rt->changed = 1;
        usleep(2000*10);
    }
}
