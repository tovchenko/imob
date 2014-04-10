/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAGUIComponent.h"
#include "iAGUIComponentDelegate.h"
#include "iARenderAgent.h"
#include "iContext.h"

CL_CLASS_DEF(iAGUIComponent, IM)

iAGUIComponent::iAGUIComponent()
: _delegate(nil),
  _overlay(nil),
  _srcFrame(dZeroRect),
  _dstFrame(dZeroRect),
  _hidden(false) {
}

void
iAGUIComponent::dealloc() {
	CL_RELEASE(_overlay);
	_super::dealloc();
}

void
iAGUIComponent::setDelegate(iAGUIComponentDelegate* dlg) {
	_delegate = dlg;
}

iAGUIComponentDelegate*
iAGUIComponent::delegate() const {
	return _delegate;
}

void
iAGUIComponent::setOverlay(iTexture2D* texture) {
	CL_THROW_IF_NIL(texture);
	CL_SWITCH_LINKS(_overlay, texture);
}

iTexture2D*
iAGUIComponent::overlay() const {
	return _overlay;
}

void
iAGUIComponent::setSrcFrame(const dRect& frame) {
	_srcFrame = frame;
}

const dRect&
iAGUIComponent::srcFrame() const {
	return _srcFrame;
}

void
iAGUIComponent::setDstFrame(const dRect& frame) {
	_dstFrame = frame;
}

const dRect&
iAGUIComponent::dstFrame() const {
	return _dstFrame;
}

void
iAGUIComponent::setIsHidden(bool hidden) {
    _hidden = hidden;
}

bool
iAGUIComponent::isHidden() const {
    return _hidden;
}

void
iAGUIComponent::process(iANode* owner, dDict* params) {
    if (_hidden) return;
    
	iContext* context = (iContext*)params->objectForKey(iARenderAgent::kContextKey);
	dValue* rect = (dValue*)params->objectForKey(iARenderAgent::kOverlayRectKey);
	
	dRect src, dst;
	if (_delegate) _delegate->layout(this, rect->rectValue(), src, dst);
	else {
		src = _srcFrame;
		dst = _dstFrame;
	}
	
	context->drawTexture(_overlay, src, dst);
}
