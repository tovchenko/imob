/*
 ** $Id: 2010/05/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iIndexBuffer.h"
#include "iContext.h"

CL_CLASS_DEF(iIndexBuffer, IM)

iIndexBuffer*
iIndexBuffer::bufferWithDataAndType(const dData* data, iEDataType type) {
	return alloc()->initWithDataAndType(data, type)->autorelease();
}

iIndexBuffer*
iIndexBuffer::initWithDataAndType(const dData* data, iEDataType type) {
	_super::initWithData(data, itemSizeByDataType(type));
	_dataType = type;
	
	iContext::currentContext()->resetIndexBuffer();
	
	return this;
}

iEBufferType
iIndexBuffer::_bufferType() {
	return kiEBufferType_ElementArray;
}

void
iIndexBuffer::unbind() {
	_unbind(kiEBufferType_ElementArray);
}

dUInteger
iIndexBuffer::itemSizeByDataType(iEDataType type) {
	switch (type) {
		case kiEDataType_UnsignedByte: return sizeof(GLubyte);
		case kiEDataType_UnsignedShort: return sizeof(GLushort);	
		case kiEDataType_UnsignedInt: return sizeof(GLuint);
		default: throw new ExAssert; return 0;
	}
}

iEDataType
iIndexBuffer::dataType() const {
	return _dataType;
}

void
iIndexBuffer::draw(iEDrawMode mode, dInteger first, dUInteger count) {
	glDrawElements(mode, count, _dataType, _boundPointer + first * _itemSize);
	glSignalIfError();
}