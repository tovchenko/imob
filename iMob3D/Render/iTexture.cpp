/*
 ** $Id: 2010/06/28, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iTexture.h"
#include "iTextureData.h"
#include "iContext.h"

CL_ABSTRACT_DEF(iTexture, IM)

iTexture::iTexture()
: _isFiltersValid(false),
  _magFilter(kiETextureFilter_Nearest),
  _minFilter(kiETextureFilter_Nearest),
  _textureID(0) {
}

iTexture*
iTexture::init() {
	_super::init();
	
	_magFilter = kiETextureFilter_Linear;
	_minFilter = kiETextureFilter_Linear;
	_isFiltersValid = true;
	
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_textureID);
	glBindTexture(_textureTarget(), _textureID);
	glSignalIfError();
	
	return this;
}

void
iTexture::dealloc() {
	if (_textureID) {
		glDeleteTextures(1, &_textureID);
		glSignalIfError();
	}
	_super::dealloc();
}

void
iTexture::bind() {
	iETextureTarget target = _textureTarget();
	
	glBindTexture(target, _textureID);
	if (_isFiltersValid) {
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, _magFilter);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, _minFilter);
		_isFiltersValid = false;
	}
	glSignalIfError();
}

void
iTexture::unbindAll() {
	iTexture2D::unbind();
	
	if (iContext::currentContext()->useShadersApi()) {
		iTextureCube::unbind();
	} else {
		glDisable(GL_TEXTURE_2D);
		glSignalIfError();
	}
}

void
iTexture::_unbind(iETextureTarget target) {
	glBindTexture(target, 0);
	glSignalIfError();
}

void
iTexture::setFilters(iETextureFilter minFilter, iETextureFilter magFilter) {
	if (minFilter != _minFilter || magFilter != _magFilter) {
		_magFilter = magFilter;
		_minFilter = minFilter;
		_isFiltersValid = true;
	}
}


CL_CLASS_DEF(iTexture2D, IM)

iTexture2D::iTexture2D()
: _width(0),
  _height(0) {
}

iTexture2D*
iTexture2D::initWithTextureData(iTextureData* textureData) {
	CL_THROW_IF_NIL(textureData);
	
	_super::init();
	
	_width = textureData->width();
	_height = textureData->height();
	
	if (textureData->mipmapsCount() > 1) {
		_minFilter = kiETextureFilter_LinearMipmapNearest;
		_isFiltersValid = true;
	}
	
	textureData->createTextureImageByTarget(_textureTarget());	
	iContext::currentContext()->resetTextureAtUnitIndex(0);
	
	return this;
}

void
iTexture2D::updateWithTextureData(iTextureData* textureData) {
	CL_THROW_IF_NIL(textureData);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(_textureTarget(), _textureID);
	glSignalIfError();
	
	textureData->createTextureImageByTarget(_textureTarget());
	iContext::currentContext()->resetTextureAtUnitIndex(0);
}

dUInteger
iTexture2D::width() const {
	return _width;
}

dUInteger
iTexture2D::height() const {
	return _height;
}

iETextureTarget
iTexture2D::_textureTarget() {
	return kiETextureTarget_2D;
}

void
iTexture2D::unbind() {
	_unbind(kiETextureTarget_2D);
}


CL_CLASS_DEF(iTextureCube, IM)

iTextureCube*
iTextureCube::initWithTextureDataArray(dArrayT<iTextureData*>* txtDataArray) {
	CL_THROW_IF_TRUE(!txtDataArray || 6 != txtDataArray->count());
	
	_super::init();
	
	const iETextureTarget faces[] = {
		kiETextureTarget_CubeMapPositiveX,
		kiETextureTarget_CubeMapNegativeX,
		kiETextureTarget_CubeMapPositiveY,
		kiETextureTarget_CubeMapNegativeY,
		kiETextureTarget_CubeMapPositiveZ,
		kiETextureTarget_CubeMapNegativeZ
	};
	
	iTextureData* txtData = txtDataArray->objectAtIndex(0);
	if (txtData->mipmapsCount() > 1) {
		_minFilter = kiETextureFilter_LinearMipmapNearest;
		_isFiltersValid = true;
	}
	
	for (dUInteger i = 0; i < 6; ++i) {
		iTextureData* txtData = txtDataArray->objectAtIndex(i);
		txtData->createTextureImageByTarget(faces[i]);
	}
	
	iContext::currentContext()->resetTextureAtUnitIndex(0);
	
	return this;
}

iETextureTarget
iTextureCube::_textureTarget() {
	return kiETextureTarget_CubeMap;
}

void
iTextureCube::unbind() {
	_unbind(kiETextureTarget_CubeMap);
}