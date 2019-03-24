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
#include "Sphere.hpp"
#include "HitableList.hpp"
#include "Camera.hpp"
#include "Material.hpp"


RTimage * CreateRTimage(int width, int height)
{
    RTimage *rt = (RTimage*) malloc(sizeof(RTimage));
    rt->data    = (uint8_t*) malloc(height*width*sizeof(float)*3);
    rt->width   = width;
    rt->height  = height;
    rt->numsamples = 100;

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


vec3 color(const ray& r, hitable* world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        if ((depth < 50) && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        }
        else
        {
            return vec3(0,0,0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t) * vec3(1.0f, 1.0f, 1.0f) + t*vec3(0.5f, 0.7f, 1.0f);
    }
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
    
    hitable *list[5];
    float R = cos(M_PI/4.0);
    list[0] = new sphere(vec3(0,0,-1), 0.5, new lambertian(vec3(0.2, 0.3, 0.8)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100.0, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(list, 5);
    camera  cam(vec3(-2,2,1), vec3(0,0,-1), vec3(0,1,0), 30, nx/ny);
    for (int j = rt->height-1; j >= 0; j--)
    {
        float *f = &fb[(rt->height-1-j)*rt->width*3];  //flip
        for (int i = 0; i < rt->width; i++)
        {
            vec3 col(0,0,0);
            for (int s=0; s < rt->numsamples; s++ )
            {
                float u = float(i + drand48()) / nx;
                float v = float(j + drand48()) / ny;
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= (float) rt->numsamples;
            f[0] = sqrtf(col[0]);
            f[1] = sqrtf(col[1]);
            f[2] = sqrtf(col[2]);
            f += 3;
        }
        rt->changed = 1;
    }
}
