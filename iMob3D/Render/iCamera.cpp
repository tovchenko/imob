/*
 ** $Id: 2010/07/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iCamera.h"
#include "iContext.h"

CL_CLASS_DEF(iCamera, IM)

iCamera::iCamera()
: _zNear(0.1f),
  _zFar(100.0f),
  _fov(45.0f),
  _position(vec3f::BACKWARD),
  _lookAt(vec3f::ZERO),
  _upVector(vec3f::UP),
  _cachedProjMtx(mat4f::IDENTITY),
  _cachedViewMtx(mat4f::IDENTITY),
  _isValidProjMtx(false),
  _isValidViewMtx(false) {
}

void
iCamera::setFOV(dFloat fofv) {
	_fov = fofv;
	_isValidProjMtx = false;
}

dFloat
iCamera::fov() const {
	return _fov;
}

void
iCamera::setNearPlane(dFloat near) {
	_zNear = near;
	_isValidProjMtx = false;
}

dFloat
iCamera::nearPlane() const {
	return _zNear;
}

void
iCamera::setFarPlane(dFloat far) {
	_zFar = far;
	_isValidProjMtx = false;
}

dFloat
iCamera::farPlane() const {
	return _zFar;
}

void
iCamera::setPosition(const vec3f& pos) {
	_position = pos;
	_isValidViewMtx = false;
}

const vec3f&
iCamera::position() const {
	return _position;
}

void
iCamera::setUpVector(const vec3f& up) {
	_upVector = up;
	_isValidViewMtx = false;
}

const vec3f&
iCamera::upVector() const {
	return _upVector;
}

void
iCamera::setLookAt(const vec3f& at) {
	_lookAt = at;
	_isValidViewMtx = false;
}

const vec3f&
iCamera::lookAt() const {
	return _lookAt;
}

void
iCamera::applyCameraInContext(iContext* context) {
	applyProjectionMatrixInContext(context);
	applyViewMatrixInContext(context);
}

void
iCamera::applyProjectionMatrixInContext(iContext* context) {
	if (!_isValidProjMtx) {
		dFloat aspect = context->viewport().size.width / context->viewport().size.height;
		frustumf f;
		f.set_perspective(_fov, aspect, _zNear, _zFar);
		f.compute_matrix(_cachedProjMtx);
		
		_isValidProjMtx = true;
	}
	
	context->setProjectionMatrix(&_cachedProjMtx);
}

void
iCamera::applyViewMatrixInContext(iContext* context) {
	if (!_isValidViewMtx) {
		_cachedViewMtx.set_lookAt(_position, _lookAt, _upVector);
		
		_isValidViewMtx = true;
	}
	
	context->setViewMatrix(&_cachedViewMtx);
}
