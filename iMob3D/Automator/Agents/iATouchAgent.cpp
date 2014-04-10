/*
 ** $Id: 2011/03/24, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iATouchAgent.h"
#include "iADirector.h"
#include "iAScene.h"
#include "iANode.h"

static dStaticString _kiATouchesKey(_T("Touches"));
dString* const iATouchAgent::kTouchesKey = &_kiATouchesKey;
static dStaticString _kiAPhaseKey(_T("Phase"));
dString* const iATouchAgent::kPhaseKey = &_kiAPhaseKey;
static dStaticString _kiABoundsKey(_T("Bounds"));
dString* const iATouchAgent::kBoundsKey = &_kiABoundsKey;

CL_ABSTRACT_DEF(iATouchAgent, IM)

void
iATouchAgent::sendTouches(dSetT<iATouch*>* touches, const dRect& bounds, iAETouchPhase phase) {
	iAScene* runScene = iADirector::singleton()->runningScene();
	if (!runScene) return;
	
	dobj_ptr<dDict> params(CL_NEW(dDict));
	params->setObjectForKey(touches, kTouchesKey);
	params->setObjectForKey(dValue::valueWithInt(phase), kPhaseKey);
	params->setObjectForKey(dValue::valueWithRect(bounds), kBoundsKey);
	runScene->rootNode()->visitComponents(kiANodeTouchComponent, params.get());
}
