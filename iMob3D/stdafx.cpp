/*
 ** $Id: 2010/07/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"

CL_ASSEMBLY_DEF(IM, 1.0)

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
	return nil;
}

bool
glCheckExtension(const char* extName, const unsigned char* extString) {
	bool flag = false;
	char* word;
	char* lookHere;
	char* deleteThis;
	
	if (!extString) return false;
	
	deleteThis = lookHere = (char*)malloc(strlen((const char*)extString) + 1); 
	if (!lookHere) return false;
	/* strtok() will modify string, so copy it somewhere */
	strcpy(lookHere, (const char*)extString);
	
	while ((word = strtok(lookHere, " "))) {
		if (strcmp(word, (const char*)extName) == 0) {
			flag = true;
			break;
		}  
		lookHere = NULL;		/* get next token */
	}
	
	free((void*)deleteThis);
	return flag;
}