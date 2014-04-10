/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iADrawableComponent.h"
#include "iDrawable.h"
#include "iARenderAgent.h"
#include "iANode.h"
#include "iATransform.h"

CL_CLASS_DEF(iADrawableComponent, IM)

iADrawableComponent::iADrawableComponent()
: _drawable(nil),
  _hidden(false) {
}

void
iADrawableComponent::dealloc() {
	CL_RELEASE(_drawable);
	_super::dealloc();
}

void
iADrawableComponent::setDrawable(iDrawable* drawbl) {
	CL_THROW_IF_NIL(drawbl);
	CL_SWITCH_LINKS(_drawable, drawbl);
}

iDrawable*
iADrawableComponent::drawable() const {
	return _drawable;
}

void
iADrawableComponent::setIsHidden(bool hidden) {
    _hidden = hidden;
}

bool
iADrawableComponent::isHidden() const {
    return _hidden;
}

void
iADrawableComponent::process(iANode* owner, dDict* params) {
    if (_hidden) return;
    
	_drawable->setMatrix(owner->transform()->resultMatrix());
	
	iContext* context = (iContext*)params->objectForKey(iARenderAgent::kContextKey);
	_drawable->renderInContext(context);
}