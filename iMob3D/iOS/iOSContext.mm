/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#import "stdafx.h"
#import "iOSContext.h"
#import "ES1Renderer.h"
#import "iScene.h"


CL_CLASS_DEF(iOSContext, IM)

iOSContext::iOSContext()
: _renderer(nil) {
}

iOSContext*
iOSContext::initWithUsingShadersApi(bool useShadersApi) {
	_super::init();
	
	_renderer = (!useShadersApi) ? [[ES1Renderer alloc] initWithOwner:this] : nil;
	CL_THROW_IF_NIL(_renderer);
	[_renderer iContext]->makeCurrentContext();
	
	_useShadersApi = useShadersApi;
	
	return this;
}

void
iOSContext::dealloc() {
	[(id<ESRenderer>)_renderer release];
	
	_super::dealloc();
}

id<ESRenderer>
iOSContext::renderer() {
	return _renderer;
}

void
iOSContext::makeCurrentContext() {
	[EAGLContext setCurrentContext:[_renderer eaglContext]];
}

void
iOSContext::resetCurrentContext() {
	[EAGLContext setCurrentContext:nil];
}

void
iOSContext::setup() {
	_scene->setupContext([_renderer iContext]);
}
