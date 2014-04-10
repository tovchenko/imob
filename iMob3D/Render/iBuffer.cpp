/*
 ** $Id: 2010/05/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iBuffer.h"


CL_ABSTRACT_DEF(iBuffer, IM)

iBuffer::iBuffer()
: _bufferID(0),
  _itemsCount(0),
  _itemSize(0),
  _boundPointer(nil) {
}

iBuffer*
iBuffer::initWithData(const dData* data, dUInteger itemSize) {
	CL_THROW_IF_NIL(data);
	dInteger len = data->length();
	CL_THROW_IF_TRUE(0 == len || 0 == itemSize);
	
	_super::init();
	
	_itemsCount = len / itemSize;
	_itemSize = itemSize;
	_boundPointer = nil; // always nil in VBO implementation

	GLenum type = _bufferType();
	
	glGenBuffers(1, &_bufferID);
	glBindBuffer(type, _bufferID);
	glBufferData(type, len, data->bytes(), GL_STATIC_DRAW);
	glBindBuffer(type, 0);
	glSignalIfError();
	
	return this;
}

void
iBuffer::dealloc() {
	if (_bufferID) {
		glDeleteBuffers(1, &_bufferID);
		glSignalIfError();
	}
	_super::dealloc();
}

void
iBuffer::bind() {
	glBindBuffer(_bufferType(), _bufferID);
	glSignalIfError();
}

void
iBuffer::_unbind(iEBufferType bufferType) {
	glBindBuffer(bufferType, 0);
	glSignalIfError();
}

dUInteger
iBuffer::itemsCount() const {
	return _itemsCount;
}

dUInteger
iBuffer::itemSize() const {
	return _itemSize;
}

const dByte*
iBuffer::boundPointer() const {
	return _boundPointer;
}
