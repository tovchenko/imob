/*
 ** $Id: 2010/07/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _imob_stdafx_H
#define _imob_stdafx_H

#include "iDefs.h"

CL_ASSEMBLY_DECL_NO_EXPORT(IM)

USING_NAMESPACE_CL

BEGIN_NAMESPACE_IM
END_NAMESPACE_IM
USING_NAMESPACE_IM

const char*	glErrorString(unsigned int err);
bool		glCheckExtension(const char* extName, const unsigned char* extString);

#define glSignalIfError()	{ GLenum err=glGetError(); while(err!=GL_NO_ERROR) { printf("GL_ERROR(%s:%d): %s\n", __FILE__, __LINE__, glErrorString(err)); err=glGetError(); } }

#endif // _imob_stdafx_H