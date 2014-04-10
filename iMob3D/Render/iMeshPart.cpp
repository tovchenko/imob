/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iMeshPart.h"
#include "iContext.h"
#include "iIndexBuffer.h"
#include "iMaterial.h"

CL_CLASS_DEF(iMeshPart, IM)

iMeshPart::iMeshPart()
: _indexBuffer(nil),
  _material(nil),
  _drawMode(kiEDrawMode_Triangles),
  _first(0),
  _count(0),
  _isHidden(false) {
}

iMeshPart*
iMeshPart::initWithParams(iIndexBuffer* indexBuffer, iMaterial* material, iEDrawMode drawMode, dInteger first, dUInteger count) {
	CL_THROW_IF_NIL(indexBuffer);
	
	_super::init();
	
	_indexBuffer = indexBuffer->retain();
	_material = material ? material->retain() : nil;
	_drawMode = drawMode;
	_first = first;
	_count = (0 == count) ? _indexBuffer->itemsCount() : count;
	
	return this;
}

iMeshPart*
iMeshPart::initWithParams(iIndexBuffer* indexBuffer, iEDrawMode drawMode, dInteger first, dUInteger count) {
	return initWithParams(indexBuffer, nil, drawMode, first, count);
}

void
iMeshPart::dealloc() {
	CL_RELEASE(_indexBuffer);
	CL_RELEASE(_material);
	
	_super::dealloc();
}

iMeshPart*
iMeshPart::meshPartWithParams(iIndexBuffer* indexBuffer, iMaterial* material, iEDrawMode drawMode, dInteger first, dUInteger count) {
	return alloc()->initWithParams(indexBuffer, material, drawMode, first, count)->autorelease();
}

iMeshPart*
iMeshPart::meshPartWithParams(iIndexBuffer* indexBuffer, iEDrawMode drawMode, dInteger first, dUInteger count) {
	return alloc()->initWithParams(indexBuffer, nil, drawMode, first, count)->autorelease();
}

void
iMeshPart::setMaterial(iMaterial* material) {
	CL_SWITCH_LINKS(_material, material);
}

iMaterial*
iMeshPart::material() const {
	return _material;
}

void
iMeshPart::setHidden(bool isHidden) {
	_isHidden = isHidden;
}

bool
iMeshPart::hidden() const {
	return _isHidden;
}

void
iMeshPart::renderInContext(iContext* context, iMaterial* defaultMeterial) {
	if (_isHidden) return;
	
	context->setIndexBuffer(_indexBuffer);
	context->setMaterial(_material ? _material : defaultMeterial);
	context->drawIndexed(_drawMode, _first, _count);
}