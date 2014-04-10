/*
 *  TexDefs.h
 *  image2tex
 *
 *  Created by DoctorSet on 8/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _TexDefs_H
#define _TexDefs_H

#include <stdint.h>

enum { kiTextureSignature = 'tex!' };


typedef struct _iSTexHeader {
	uint32_t headerLength;
	uint32_t height;
	uint32_t width;
	uint32_t numMipmaps;
	uint32_t format;
	uint32_t dataLength;
	uint32_t tag;
} iSTexHeader;

typedef enum _iEColorFormat {
	kiEColorFormat_Unknown		= 0,
	
	kiEColorFormat_R8G8B8		= 20,
	kiEColorFormat_R8G8B8A8		= 21,
	kiEColorFormat_R5G6B5		= 23,
	kiEColorFormat_R5G5B5A1		= 25,
	kiEColorFormat_R4G4B4A4		= 26,
	kiEColorFormat_A8			= 28,
	
	kiEColorFormat_L8           = 50,
	kiEColorFormat_L8A8			= 51,
	
	kiEColorFormat_ForceDword	= 0x7FFFFFFF
} iEColorFormat;


const char*	glErrorString(unsigned int err);
#define glSignalIfError()	{ GLenum err=glGetError(); while(err!=GL_NO_ERROR) { printf("GL_ERROR(%s:%d): %s", __FILE__, __LINE__, glErrorString(err)); err=glGetError(); } }


#endif // _TexDefs_H