/*
 ** $Id: 2011/04/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAGUIButton.h"
#include "iASprite.h"

CL_CLASS_DEF(iAGUIButton, IM)

iAGUIButton::iAGUIButton() {
}

iAGUIButton*
iAGUIButton::initWithSprites(iASprite* defaultSprite, iASprite* selectedSprite) {
    _super::init();
    
    addChild(defaultSprite);
    addChild(selectedSprite);
    
    return this;
}

void
iAGUIButton::touchBegan(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
}

void
iAGUIButton::touchMoved(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
}

void
iAGUIButton::touchEnded(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
}

void
iAGUIButton::touchCancelled(iATouchComponent* component, dSetT<iATouch*>* touches, const dRect& bounds) {
}