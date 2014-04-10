/*
 ** $Id: 2011/02/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _Camera_H
#define _Camera_H

#include "iMob.h"
USING_NAMESPACE_IM

class Camera : public iACamera,
			   public iATouchComponentDelegate
{
CL_CLASS_DECL(Camera, iACamera)
public:
	virtual _self*		init();
	// iANode
	virtual void		update(iAENodeComponentID componentID);
	// iATouchComponentDelegate
	virtual void		touchBegan(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	virtual void		touchMoved(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	virtual void		touchEnded(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	virtual void		touchCancelled(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	
protected:
	Camera();
	
	bool				_isPressed;
	bool				_toLeftSide;
};

#endif // _Camera_H

