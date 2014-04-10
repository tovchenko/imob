/*
 ** $Id: 2011/01/29, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iARenderAgent.h"
#include "iADirector.h"
#include "iAScene.h"
#include "iANode.h"
#include "iContext.h"
#include "iATime.h"

static dStaticString _kiAContextArgKey(_T("Context"));
dString* const iARenderAgent::kContextKey = &_kiAContextArgKey;
static dStaticString _kiAOverlayRectKey(_T("OverlayRect"));
dString* const iARenderAgent::kOverlayRectKey = &_kiAOverlayRectKey;

CL_ABSTRACT_DEF(iARenderAgent, IM)

void
iARenderAgent::renderDrawablesInContext(iScene* scene, iContext* context) {
	iAScene* runScene = iADirector::singleton()->runningScene();
	if (!runScene) return;
    
    iATime::singleton()->_calculateDeltaTime();
	
	dobj_ptr<dDict> params(CL_NEW(dDict));
	params->setObjectForKey(context, kContextKey);
	runScene->rootNode()->visitComponents(kiANodeCameraComponent, params.get());
	runScene->rootNode()->visitComponents(kiANodeDrawableComponent, params.get());
}

void
iARenderAgent::renderOverlaysInContext(iScene* scene, iContext* context, const CL::dRect& rect) {
	iAScene* runScene = iADirector::singleton()->runningScene();
	if (!runScene) return;
	
	dobj_ptr<dDict> params(CL_NEW(dDict));
	params->setObjectForKey(context, kContextKey);
	params->setObjectForKey(dValue::valueWithRect(rect), kOverlayRectKey);
	runScene->rootNode()->visitComponents(kiANodeGUIComponent, params.get());
}

void
iARenderAgent::_start() {
    iATime::singleton()->setupNewSequence();
}

void
iARenderAgent::_stop() {
}