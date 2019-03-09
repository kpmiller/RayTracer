//
//  Hitable.hpp
//  RayTracer
//
//  Created by Kent Miller on 3/9/19.
//  Copyright © 2019 Kent Miller. All rights reserved.
//

#ifndef Hitable_hpp
#define Hitable_hpp

#include <stdio.h>
#include "RTDataTypes.hpp"

struct hit_record {
    float t;
    vec3  p;
    vec3  normal;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif /* Hitable_hpp */
