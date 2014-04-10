/*
 ** $Id: 2011/03/24, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iATouch_H
#define _iATouch_H


BEGIN_NAMESPACE_IM


class iATouch : public dObject {
CL_CLASS_DECL(iATouch, dObject)
public:
	static _self*	touchWithParams(const dPoint& pos, const dPoint& dPos, dFloat dt, dUInteger tc);
	virtual _self*	initWithParams(const dPoint& pos, const dPoint& dPos, dFloat dt, dUInteger tc);
	
	const dPoint&	position() const;
	const dPoint&	deltaPosition() const;
	dFloat			deltaTime() const;
	dUInteger		tapCount() const;
	
protected:
	iATouch();
	
	dPoint			_position;
	dPoint			_deltaPosition;
	dFloat			_deltaTime;
	dUInteger		_tapCount;
};

END_NAMESPACE_IM

#endif // _iATouch_H

