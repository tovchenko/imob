/*
 ** $Id: 2011/03/24, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iOSTouchAgent.h"
#include "EAGLView.h"

CL_CLASS_DEF(iOSTouchAgent, IM)

iOSTouchAgent::iOSTouchAgent()
: _view(nil) {
}

iOSTouchAgent*
iOSTouchAgent::initWithView(EAGLView* view) {
	_super::init();
	setView(view);
	return this;
}

iOSTouchAgent*
iOSTouchAgent::agentWithView(EAGLView* view) {
	_self* im = alloc()->initWithView(view);
	return im ? im->autorelease() : nil;
}

void
iOSTouchAgent::dealloc() {
	[_view release];
	
	_super::dealloc();
}

void
iOSTouchAgent::setView(EAGLView* view) {
	if (_view == view) return;
	
	[_view release];
	_view = [view retain];
	
	view.touchHandler = this;
}

EAGLView*
iOSTouchAgent::view() const {
	return _view;
}

void
iOSTouchAgent::_start() {
}

void
iOSTouchAgent::_stop() {
}