/*
 ** $Id: 2011/03/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iATouchComponentDelegate_H
#define _iATouchComponentDelegate_H

BEGIN_NAMESPACE_IM

class iATouchComponent;
class iATouch;

class iATouchComponentDelegate {
public:
	virtual void	touchBegan(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) = 0;
	virtual void	touchMoved(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) = 0;
	virtual void	touchEnded(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) = 0;
	virtual void	touchCancelled(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) = 0;
};

END_NAMESPACE_IM

#endif // _iATouchComponentDelegate_H