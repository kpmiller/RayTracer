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
#include <Dispatch/Dispatch.h>


RTimage * CreateRTimage(int width, int height)
{
    RTimage *rt = (RTimage*) malloc(sizeof(RTimage));
    rt->data    = (uint8_t*) calloc(height*width*sizeof(float)*3, 1);
    rt->width   = width;
    rt->height  = height;
    rt->numsamples = 100;

//    float *f = (float*) rt->data;
//    for (int y = 0; y < height; y++)
//    {
//        float yv = (float) y / (float)height;
//        for (int x=0; x< width; x++)
//        {
//            f[0] = (float) x / (float)width;
//            f[1] = yv;
//            f[2] = 0.2f;
//            f+=3;
//        }
//    }
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

hitable * random_scene(void)
{
    int n = 500;
    
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0,-1000,0), 1000, new lambertian(vec3(0.5,0.5,0.5)));
    int i = 1;
    for (int a=-11; a<11; a++) {
        for (int b=-11; b<11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(), 0.2, b+0.9*drand48());
            if ((center - vec3(4,0.2,0)).length() > 0.9) {
                if (choose_mat < 0.8) //diffuse
                {
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(
                                                        drand48()*drand48(),
                                                        drand48()*drand48(),
                                                        drand48()*drand48())));
                }
                else if (choose_mat < 0.95)
                {
                    list[i++] = new sphere(center, 0.2, new metal(vec3(
                                                    0.5* (1+drand48()),
                                                    0.5 * (1+drand48()),
                                                    0.5 * (1+drand48())),
                                                    0.5 * drand48()));

                }
                else
                {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere( vec3(0,1,0), 1.0, new dielectric(1.5));
    list[i++] = new sphere( vec3(-4,1,0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere( vec3(4,1,0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new hitable_list(list, i);
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
    
    hitable *world = random_scene();
    
    vec3    lookfrom(13,2,3);
    vec3    lookat(0,0,0);
    float   dist_to_focus = 10.0;
    float   aperture = 0.1f;
    camera  cam(lookfrom, lookat, vec3(0,1,0), 20, nx/ny, aperture, dist_to_focus);
    
    for (int j = rt->height-1; j >= 0; j--)
    {
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
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
            float *f = &fb[((rt->height-1-j)*rt->width*3)+(i*3)];
            f[0] = sqrtf(col[0]);
            f[1] = sqrtf(col[1]);
            f[2] = sqrtf(col[2]);
            rt->changed = 1;
        }
        });
    }
}
