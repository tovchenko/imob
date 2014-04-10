/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iMaterial.h"
#include "iTexture.h"
#include "iContext.h"

CL_CLASS_DEF(iMaterial, IM)

iMaterial::iMaterial()
: _textureUnits(nil),
  _updated(false) {
}

iMaterial*
iMaterial::initWithTexture(iTexture* texture) {
	_super::init();
	
	textureUnitAtIndex(0)->setTexture(texture);
	
	return this;
}

void
iMaterial::dealloc() {
	CL_RELEASE(_textureUnits);
	
	_super::dealloc();
}

iMaterial*
iMaterial::material() {
	return (iMaterial*)alloc()->init()->autorelease();
}

iMaterial*
iMaterial::materialWithTexture(iTexture* texture) {
	return alloc()->initWithTexture(texture)->autorelease();
}

iTextureUnit*
iMaterial::textureUnitAtIndex(dUInteger index) {
	if (!_textureUnits)
		_textureUnits = CL_NEW(dArrayT<iTextureUnit*>);
	
	if (index >= _textureUnits->count()) {
		for (dUInteger i = 0, cnt = index + 1 - _textureUnits->count(); i < cnt; ++i) {
			dobj_ptr<iTextureUnit> unit(iTextureUnit::alloc()->_initWithOwner(this));
			_textureUnits->addObject(unit.get());
		}
	}
	
	return _textureUnits->objectAtIndex(index);
}

void
iMaterial::bind() {
	iContext::currentContext()->setTextureUnits(_textureUnits);
	_updated = false;
}

void
iMaterial::unbind() {
	iContext::currentContext()->clearTextureUnits();
}

bool
iMaterial::updated() const {
	return _updated;
}

CL_CLASS_DEF(iTextureUnit, IM)

iTextureUnit::iTextureUnit()
: _texture(nil),
  _lodBias(0) {
}

iTextureUnit*
iTextureUnit::_initWithOwner(iMaterial* owner) {
	CL_THROW_IF_NIL(owner);
	
	_super::init();	
	_owner = owner;
	return this;
}

void
iTextureUnit::dealloc() {
	CL_RELEASE(_texture);
	_super::dealloc();
}

void
iTextureUnit::setTexture(iTexture* txt) {
	if (txt != _texture) {
		CL_RETAIN(txt);
		CL_RELEASE(_texture);
		_texture = txt;
		
		_owner->_updated = true;
	}
}

iTexture*
iTextureUnit::texture() const {
	return _texture;
}

void
iTextureUnit::setLODBias(dFloat lod) {
	if (lod != _lodBias) {
		_lodBias = lod;
		
		_owner->_updated = true;
	}
}

dFloat
iTextureUnit::lodBias() const {
	return _lodBias;
}