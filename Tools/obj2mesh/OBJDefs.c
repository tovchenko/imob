/*
 *  Utils.c
 *  obj2mesh
 *
 *  Created by DoctorSet on 8/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <math.h>

#include "OBJDefs.h"

void
vec3_normalize(Vec3* a) {
	float x = a->x;
	float y = a->y;
	float z = a->z;
	float one = 1.0f;
	
	float l = x * x;
	l = l + y * y;
	l = l + z * z;
	
	l = one / sqrt(l);
	
	x *= l;
	y *= l;
	z *= l;
	
	a->x = x;
	a->y = y;
	a->z = z;
}

void
vec3_cross(Vec3* a, const Vec3* v1, const Vec3* v2) {
	a->x = (v1->y*v2->z) - v1->z*v2->y;
	a->y = (v1->z*v2->x) - v1->x*v2->z;
	a->z = (v1->x*v2->y) - v1->y*v2->x;
}