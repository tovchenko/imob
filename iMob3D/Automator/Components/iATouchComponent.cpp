/*
 ** $Id: 2011/03/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iATouchComponent.h"
#include "iATouchComponentDelegate.h"
#include "iATouchAgent.h"

CL_CLASS_DEF(iATouchComponent, IM)

iATouchComponent::iATouchComponent()
: _delegate(nil) {
}

void
iATouchComponent::setDelegate(iATouchComponentDelegate* dlg) {
	_delegate = dlg;
}

iATouchComponentDelegate*
iATouchComponent::delegate() const {
	return _delegate;
}

void
iATouchComponent::process(iANode* owner, dDict* params) {
	if (!_delegate) return;
	
	dSetT<iATouch*>* touches = (dSetT<iATouch*>*)params->objectForKey(iATouchAgent::kTouchesKey);
	iAETouchPhase phase = ((dValue*)params->objectForKey(iATouchAgent::kPhaseKey))->intValue();
	dRect bounds = ((dValue*)params->objectForKey(iATouchAgent::kBoundsKey))->rectValue();
	
	switch (phase) {
		case iAETouchPhase_Began:
			_delegate->touchBegan(this, touches, bounds);
			break;
			
		case iAETouchPhase_Moved:
			_delegate->touchMoved(this, touches, bounds);
			break;
			
		case iAETouchPhase_Ended:
			_delegate->touchEnded(this, touches, bounds);
			break;
			
		case iAETouchPhase_Cancelled:
			_delegate->touchCancelled(this, touches, bounds);
			break;
			
		default:
			throw new ExAssert;
			break;
	}
}