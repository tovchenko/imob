/*
 ** $Id: 2010/05/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iVertexBuffer.h"
#include "iContext.h"

CL_CLASS_DEF(iVertexBuffer, IM)

iVertexBuffer::iVertexBuffer() {
}

iVertexBuffer*
iVertexBuffer::initWithDataAndFVF(const dData* data, iEFVF fvf) {
	_super::initWithData(data, _self::itemSizeByFVF(fvf));
	
	_fvf = fvf;
	
	iContext::currentContext()->resetVertexBuffer();
	
	return this;
}

iVertexBuffer*
iVertexBuffer::bufferWithDataAndFVF(const dData* data, iEFVF fvf) {
	return alloc()->initWithDataAndFVF(data, fvf)->autorelease();
}

iEBufferType
iVertexBuffer::_bufferType() {
	return kiEBufferType_Array;
}

void
iVertexBuffer::unbind() {
	_unbind(kiEBufferType_Array);
}

iEFVF
iVertexBuffer::fvf() const {
	return _fvf;
}

dUInteger
iVertexBuffer::itemSizeByFVF(iEFVF fvf) {
	dUInteger res = 0;
	if (kiEFVF_XYZ & fvf) res += 3 * sizeof(GLfloat);
	if (kiEFVF_Normal & fvf) res += 3 * sizeof(GLfloat);
	if (kiEFVF_Diffuse & fvf) res += 4 * sizeof(GLubyte);
	if (kiEFVF_TexCountMask & fvf) {
		dInteger texCnt = (kiEFVF_TexCountMask & fvf) >> kiEFVF_TexCountShift;
		for (dInteger i = 0; i < texCnt; ++i)
			res += iEFVF_GET_TEXCOORDSIZE(fvf, i) * sizeof(GLfloat);
	}
	return res;
}

void
iVertexBuffer::draw(iEDrawMode mode, dInteger first, dUInteger count) {
	glDrawArrays(mode, first, count);
	glSignalIfError();
}