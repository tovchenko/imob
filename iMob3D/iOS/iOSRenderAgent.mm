/*
 ** $Id: 2011/02/03, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iOSRenderAgent.h"
#include "EAGLView.h"
#include "iOSContext.h"

CL_CLASS_DEF(iOSRenderAgent, IM)

iOSRenderAgent::iOSRenderAgent()
: _view(nil) {
}

iOSRenderAgent*
iOSRenderAgent::initWithView(EAGLView* view) {
	_super::init();
	setView(view);
	return this;
}

iOSRenderAgent*
iOSRenderAgent::agentWithView(EAGLView* view) {
	_self* im = alloc()->initWithView(view);
	return im ? im->autorelease() : nil;
}

void
iOSRenderAgent::dealloc() {
	_stop();
	[_view release];
	
	_super::dealloc();
}

void
iOSRenderAgent::setView(EAGLView* view) {
	if (_view == view) return;
	
	bool wasRunning = _view.animating;
	
	_stop();
	[_view release];
	
	_view = [view retain];
	
	dobj_ptr<iScene> scene(CL_NEW(iScene));
	
	scene->setDelegate(this);
	
	_view.context->setScene(scene.get());
	if (wasRunning) _start();
}

EAGLView*
iOSRenderAgent::view() const {
	return _view;
}

void
iOSRenderAgent::_start() {
    _super::_start();
	[_view startAnimation];
}

void
iOSRenderAgent::_stop() {
	[_view stopAnimation];
    _super::_stop();
}