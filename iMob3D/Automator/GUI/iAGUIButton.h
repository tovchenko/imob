/*
 ** $Id: 2011/04/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAGUIButton_H
#define _iAGUIButton_H

#include "iANode.h"
#include "iATouchComponentDelegate.h"


BEGIN_NAMESPACE_IM

class iASprite;

class iAGUIButton : public iANode,
                    public iATouchComponentDelegate
{
CL_CLASS_DECL(iAGUIButton, iANode)
public:
    virtual _self*  initWithSprites(iASprite* defaultSprite, iASprite* selectedSprite);
    // iATouchComponentDelegate
    virtual void	touchBegan(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	virtual void	touchMoved(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	virtual void	touchEnded(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
	virtual void	touchCancelled(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds);
    
protected:
    iAGUIButton();
};

END_NAMESPACE_IM

#endif // _iAGUIButton_H