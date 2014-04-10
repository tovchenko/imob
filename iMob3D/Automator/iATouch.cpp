/*
 ** $Id: 2011/03/24, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iATouch.h"

CL_CLASS_DEF(iATouch, IM)

iATouch::iATouch() {
}

iATouch*
iATouch::touchWithParams(const dPoint& pos, const dPoint& dPos, dFloat dt, dUInteger tc) {
	_self* im = alloc()->initWithParams(pos, dPos, dt, tc);
	return im ? im->autorelease() : nil;
}

iATouch*
iATouch::initWithParams(const dPoint& pos, const dPoint& dPos, dFloat dt, dUInteger tc) {
	_super::init();
	
	_position = pos;
	_deltaPosition = dPos;
	_deltaTime = dt;
	_tapCount = tc;
	
	return this;
}

const dPoint&
iATouch::position() const {
	return _position;
}

const dPoint&
iATouch::deltaPosition() const {
	return _deltaPosition;
}

dFloat
iATouch::deltaTime() const {
	return _deltaTime;
}

dUInteger
iATouch::tapCount() const {
	return _tapCount;
}
