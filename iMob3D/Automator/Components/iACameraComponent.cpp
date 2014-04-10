/*
 ** $Id: 2011/02/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iACameraComponent.h"
#include "iACameraComponentDelegate.h"
#include "iARenderAgent.h"

CL_CLASS_DEF(iACameraComponent, IM)

iACameraComponent::iACameraComponent()
: _delegate(nil) {
}

void
iACameraComponent::setDelegate(iACameraComponentDelegate* dlg) {
	_delegate = dlg;
}

iACameraComponentDelegate*
iACameraComponent::delegate() const {
	return _delegate;
}

void
iACameraComponent::process(iANode* owner, dDict* params) {
	if (!_delegate) return;
	
	iContext* context = (iContext*)params->objectForKey(iARenderAgent::kContextKey);
	_delegate->applyCameraInContext(this, context);
}