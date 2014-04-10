/*
 ** $Id: 2011/02/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iACamera.h"
#include "iACameraComponent.h"
#include "iATransform.h"
#include "iCamera.h"
#include "iContext.h"
#include "iASkybox.h"


CL_CLASS_DEF(iACamera, IM)

iACamera::iACamera()
: _projection(nil),
  _skybox(nil) {
}

iACamera*
iACamera::init() {
	_super::init();
	
	dobj_ptr<iACameraComponent> cc(CL_NEW(iACameraComponent));
	cc->setDelegate(this);
	setComponent(kiANodeCameraComponent, cc.get());
	
	_projection = CL_NEW(iCamera);
	transform()->setAxesInverted(true);
	
	return this;
}

void
iACamera::dealloc() {
	CL_RELEASE(_projection);
	CL_RELEASE(_skybox);
	_super::dealloc();
}

void
iACamera::applyCameraInContext(iACameraComponent* component, iContext* context) {
	_projection->applyProjectionMatrixInContext(context);
	context->setViewMatrix(transform()->resultMatrix());
}

void
iACamera::setFOV(dFloat fofv) {
	_projection->setFOV(fofv);
}

dFloat
iACamera::fov() const {
	return _projection->fov();
}

void
iACamera::setNearPlane(dFloat near) {
	_projection->setNearPlane(near);
}

dFloat
iACamera::nearPlane() const {
	return _projection->nearPlane();
}

void
iACamera::setFarPlane(dFloat far) {
	_projection->setFarPlane(far);
}

dFloat
iACamera::farPlane() const {
	return _projection->farPlane();
}

void
iACamera::setLookAt(const vec3f& at, const vec3f& up) {
	mat4f mtx;
	vec3f pos = transform()->localPosition();
	mtx.set_lookAt(pos, at, up);
	transform()->setLocalRotation(quaternionf(mtx));
}

void
iACamera::setSkybox(iASkybox* box) {
	CL_SWITCH_LINKS(_skybox, box);
}

iASkybox*
iACamera::skybox() const {
	return _skybox;
}

void
iACamera::update(iAENodeComponentID componentID) {
	if (kiANodeCameraComponent != componentID)
		return;
	
	iANode* top = topNode();
	iANode* firstObject = top->childAtIndex(0);
	
	if (firstObject != _skybox) {
		if (firstObject->isKindOfClass(iASkybox)) top->removeChildAtIndex(0);
		if (_skybox) top->insertChildAtIndex(_skybox, 0);
	}
	
	if (_skybox) {
		_skybox->transform()->setLocalPosition(transform()->position());
	}
}