/*
 ** $Id: 2011/10/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAImmediateAction.h"
#include "iANode.h"
#include "iADrawableComponent.h"
#include "iAGUIComponent.h"

CL_ABSTRACT_DEF(iAImmediateAction, IM)

// =================================================================================

CL_CLASS_DEF(iAShow, IM)

iAShow::iAShow() {
}

iAImmediateAction*
iAShow::reverse() const {
    return CL_NEW(iAHide)->autorelease();
}

iAShow*
iAShow::action() {
    return CL_NEW(iAShow)->autorelease();
}

void
iAShow::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    iADrawableComponent* dc = imCOMPONENT(target, kiANodeDrawableComponent);
    if (dc) dc->setIsHidden(false);
    iAGUIComponent* gc = imCOMPONENT(target, kiANodeGUIComponent);
    if (gc) gc->setIsHidden(false);
}

// ==================================================================================

CL_CLASS_DEF(iAHide, IM)

iAHide::iAHide() {
}

iAImmediateAction*
iAHide::reverse() const {
    return CL_NEW(iAShow)->autorelease();
}

iAHide*
iAHide::action() {
    return CL_NEW(iAHide)->autorelease();
}

void
iAHide::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    iADrawableComponent* dc = imCOMPONENT(target, kiANodeDrawableComponent);
    if (dc) dc->setIsHidden(true);
    iAGUIComponent* gc = imCOMPONENT(target, kiANodeGUIComponent);
    if (gc) gc->setIsHidden(true);
}

// ==================================================================================

CL_CLASS_DEF(iAToggleVisibility, IM)

iAToggleVisibility::iAToggleVisibility() {
}

iAToggleVisibility*
iAToggleVisibility::reverse() const {
    return CL_NEW(iAToggleVisibility)->autorelease();
}

iAToggleVisibility*
iAToggleVisibility::action() {
    return CL_NEW(iAToggleVisibility)->autorelease();
}

void
iAToggleVisibility::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    iADrawableComponent* dc = imCOMPONENT(target, kiANodeDrawableComponent);
    if (dc) dc->setIsHidden(!dc->isHidden());
    iAGUIComponent* gc = imCOMPONENT(target, kiANodeGUIComponent);
    if (gc) gc->setIsHidden(!gc->isHidden());
}