//
//  RayTracer.hpp
//  RayTracer
//
//  Created by Kent Miller on 3/3/19.
//  Copyright © 2019 Kent Miller. All rights reserved.
//

#ifndef RayTracer_hpp
#define RayTracer_hpp

#include <stdio.h>
#include <stdint.h>


#ifdef   __cplusplus
extern "C" {
#endif

typedef struct {
    int     changed;
    int     width;
    int     height;
    uint8_t *data;
} RTimage;

RTimage * CreateRTimage(int width, int height);

void Trace(RTimage *rt);

#ifdef   __cplusplus
};
#endif

#endif /* RayTracer_hpp */
