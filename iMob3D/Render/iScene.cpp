/*
 ** $Id: 2010/07/18, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iScene.h"
#include "iCamera.h"
#include "iContext.h"

CL_CLASS_DEF(iScene, IM)

iScene::iScene()
: _delegate(nil) {
}

void
iScene::setDelegate(iSceneDelegate* dlg) {
	_delegate = dlg;
}

iSceneDelegate*
iScene::delegate() const {
	return _delegate;
}

void
iScene::setupContext(iContext* context) {
	CL_THROW_IF_NIL(context);
	context->makeCurrentContext();
	context->setDynamicOrientation(true);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	glDisable(GL_BLEND);
	glDisable(GL_DITHER);
	
	if (context->useShadersApi()) {
	} else {
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_MULTISAMPLE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
}

void
iScene::renderInContext(iContext* context) {
	assert(context);
	
	if (!_delegate) return;
	
	context->updateDynamicOrientationAngle();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glSignalIfError();
	
	_delegate->renderDrawablesInContext(this, context);
	_delegate->renderOverlaysInContext(this, context, context->bounds());
}