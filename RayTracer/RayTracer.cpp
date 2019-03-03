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


RTimage * CreateRTimage(int height, int width)
{
    RTimage *rt = (RTimage*) malloc(sizeof(RTimage));
    rt->data    = (uint8_t*) malloc(height*width*sizeof(float)*3);
    rt->width   = width;
    rt->height  = height;

    float *f = (float*) rt->data;
    for (int y = 0; y < width; y++)
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
    return rt;
}
