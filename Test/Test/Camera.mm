/*
 ** $Id: 2011/02/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "Camera.h"
#include "Assembly.h"

CL_CLASS_DEF(Camera, TEST)

Camera::Camera()
: _isPressed(false),
  _toLeftSide(false) {
}

Camera*
Camera::init() {
	_super::init();
	
	dobj_ptr<iATouchComponent> tc(CL_NEW(iATouchComponent));
	tc->setDelegate(this);
	setComponent(kiANodeTouchComponent, tc.get());
	
	return this;
}

void
Camera::update(iAENodeComponentID componentID) {
	if (kiANodeCameraComponent != componentID)
		return;
	
//	static dFloat angle = 0;
//	dFloat x = 20 * cosf(angle);
//	dFloat z = 20 * sinf(angle);
//	angle += 0.01;
//	if (angle >= 2 * M_PI)
//		angle = 0;
//	transform()->setLocalPosition(vec3f(x, 0.5f, z));
	
//	setLookAt(vec3f(5, 3, 0));
	if (_isPressed) {
		iATransform* tr = transform();
		tr->setLocalRotation(quaternionf(_toLeftSide ? -M_PI / 300 : M_PI / 300, vec3f::UNIT_Y) * tr->localRotation());
	}
//	tr->setLocalRotation(quaternionf(M_PI / 400, vec3f::UNIT_X) * tr->localRotation());
	
	transform()->setLocalPosition(vec3f(0, 0, 10));
//	setLookAt(vec3f(5, 2, -1));
//	transform()->setLocalRotation(quaternionf(M_PI / 32, vec3f::UNIT_Y));
	
	_super::update(componentID);
}

void
Camera::touchBegan(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
	_isPressed = true;
	if (touches->anyObject()->position().x < 0.5f * bounds.size.width) _toLeftSide = true;
	else _toLeftSide = false;
}

void
Camera::touchMoved(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
}

void
Camera::touchEnded(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
	_isPressed = false;
}

void
Camera::touchCancelled(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
}