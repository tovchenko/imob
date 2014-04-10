/*
 *  TexDefs.c
 *  image2tex
 *
 *  Created by DoctorSet on 8/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import <OpenGL/gl.h>

#include "TexDefs.h"

const char*
glErrorString(unsigned int err) {
	switch (err) {
		case GL_INVALID_ENUM:		return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:		return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:	return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW:		return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW:	return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:		return "GL_OUT_OF_MEMORY";
	}
	return 0;
}