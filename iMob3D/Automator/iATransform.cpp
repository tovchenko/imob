/*
 ** $Id: 2011/02/10, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iATransform.h"
#include "iANode.h"

void		_enumerate(iANode* node, iAENodeComponentID cmp, dDict* params);

CL_CLASS_DEF(iATransform, IM)

iATransform::iATransform()
: _ownerNode(nil),
  _position(vec3f::ZERO),
  _rotation(quaternionf::IDENTITY),
  _isValidLocalCache(false),
  _isValidGlobalCache(false),
  _cachedLocalMtx(mat4f::IDENTITY),
  _cachedGlobalMtx(mat4f::IDENTITY),
  _isAxesInverted(false),
  _isParentTransformUse(true) {
}

vec3f
iATransform::position() {
	_calcGlobalMatrixIfNeeded();
	
	mat4f inv;
	_cachedGlobalMtx.inverse(inv);
	return vec3f(inv.x(), inv.y(), inv.z());
}

const vec3f&
iATransform::localPosition() const {
	return _position;
}

void
iATransform::setLocalPosition(const vec3f& pos) {
	_position = pos;
	_isValidLocalCache = false;
	
	invalidateCacheHierarchical();
}

quaternionf
iATransform::rotation() {
	_calcGlobalMatrixIfNeeded();
	
	mat4f inv;
	_cachedGlobalMtx.inverse(inv);
	
	mat3f rotMtx;
	inv.get_sub_matrix(rotMtx);
	return quaternionf(rotMtx);
}

const quaternionf&
iATransform::localRotation() const {
	return _rotation;
}

void
iATransform::setLocalRotation(const quaternionf& rot) {
	_rotation = rot;
	_isValidLocalCache = false;

	invalidateCacheHierarchical();
}

const mat4f*
iATransform::resultMatrix() {
	_calcGlobalMatrixIfNeeded();
	return &_cachedGlobalMtx;
}

void
iATransform::invalidateCache() {
	_isValidGlobalCache = false;
}

void
iATransform::invalidateCacheHierarchical() {
	_ownerNode->visit(_enumerate, kiANodeComponentsEndMarker, nil);
}

void
iATransform::setAxesInverted(bool inverted) {
	_isAxesInverted = inverted;
	
	_isValidLocalCache = false;
	invalidateCacheHierarchical();
}

bool
iATransform::isAxesInverted() const {
	return _isAxesInverted;
}

void
iATransform::setParentTransformUsage(bool isUse) {
	_isParentTransformUse = isUse;
	
	_isValidLocalCache = false;
	invalidateCacheHierarchical();
}

bool
iATransform::isParentTransformUse() const {
	return _isParentTransformUse;
}

void
iATransform::_calcLocalMatrixIfNeeded() {
	if (_isValidLocalCache) return;
	
	mat4f rotMtx;
	rotMtx.identity();
	_rotation.get_rotation_matrix(rotMtx);
	
	vec3f realPos = _position;
	if (_isAxesInverted) realPos.negate();
	
	mat4f trsltMtx;
	trsltMtx.identity();
	trsltMtx.set_translation(realPos);
	
	if (_isAxesInverted) _cachedLocalMtx.multiply(rotMtx, trsltMtx);
	else _cachedLocalMtx.multiply(trsltMtx, rotMtx);
	
	_isValidLocalCache = true;
}

void
iATransform::_calcGlobalMatrixIfNeeded() {
	CL_THROW_IF_NIL(_ownerNode);
	if (_isValidGlobalCache) return;
	_calcLocalMatrixIfNeeded();
	
	iANode* parent = _ownerNode->parent();
	if (parent && _isParentTransformUse) {
		if (_isAxesInverted) {
			mat4f inv;
			parent->transform()->resultMatrix()->inverse(inv);
			_cachedGlobalMtx.multiply(_cachedLocalMtx, inv);
		} else {
			_cachedGlobalMtx.multiply(_cachedLocalMtx, *parent->transform()->resultMatrix());
		}
	} else {
		_cachedGlobalMtx = _cachedLocalMtx;
	}
	
	_isValidGlobalCache = true;
}


void
_enumerate(iANode* node, iAENodeComponentID cmp, dDict* params) {
	node->transform()->invalidateCache();
}
