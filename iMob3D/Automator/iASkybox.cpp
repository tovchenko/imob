/*
 ** $Id: 2011/03/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iASkybox.h"
#include "iMeshPrimitives.h"
#include "iMeshPart.h"
#include "iMaterial.h"
#include "iTexture.h"
#include "iDrawable.h"
#include "iADrawableComponent.h"
#include "iATransform.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>


#define kDefaultBoxSize		90

CL_CLASS_DEF(iASkybox, IM)

iASkybox::iASkybox() {
}

iASkybox*
iASkybox::initWithTextures(iTexture2D* posX, iTexture2D* negX,
						   iTexture2D* posY, iTexture2D* negY,
						   iTexture2D* posZ, iTexture2D* negZ)
{
	_super::init();
	
	dobj_ptr<iMesh> mesh(iCubeMesh::alloc()->initWithSideSizeAndSegmentsCount(kDefaultBoxSize, 1));
	
	dDictT<iMeshPart*, dString*>* mparts = mesh->parts();
	mparts->objectForKey(_ST("posX"))->setMaterial(iMaterial::materialWithTexture(posX));
	mparts->objectForKey(_ST("negX"))->setMaterial(iMaterial::materialWithTexture(negX));
	mparts->objectForKey(_ST("posY"))->setMaterial(iMaterial::materialWithTexture(posY));
	mparts->objectForKey(_ST("negY"))->setMaterial(iMaterial::materialWithTexture(negY));
	mparts->objectForKey(_ST("posZ"))->setMaterial(iMaterial::materialWithTexture(posZ));
	mparts->objectForKey(_ST("negZ"))->setMaterial(iMaterial::materialWithTexture(negZ));
	
	dobj_ptr<iDrawable> drwbl(iDrawable::alloc()->initWithMesh(mesh.get()));
	dobj_ptr<iADrawableComponent> dc(CL_NEW(iADrawableComponent));
	dc->setDrawable(drwbl.get());
	
	setComponent(kiANodeDrawableComponent, dc.get());
	transform()->setParentTransformUsage(false);
	
	return this;
}

void
iASkybox::update(iAENodeComponentID componentID) {
	if (kiANodeDrawableComponent != componentID)
		return;
	
	GLint fronFace;
	glGetIntegerv(GL_FRONT_FACE, &fronFace);
	_stateFlags.fronFaceIsConterClockWise = (GL_CCW == fronFace);
	if (_stateFlags.fronFaceIsConterClockWise) glFrontFace(GL_CW);
	
	_stateFlags.depthTestWasEnable = glIsEnabled(GL_DEPTH_TEST);
	if (_stateFlags.depthTestWasEnable) glDisable(GL_DEPTH_TEST);
	
    GLboolean isEnabledDM = GL_TRUE;
    glGetBooleanv(GL_DEPTH_WRITEMASK, &isEnabledDM);
	_stateFlags.depthMaskWasEnable = isEnabledDM;
	if (_stateFlags.depthMaskWasEnable) glDepthMask(GL_FALSE);
	
	_stateFlags.lightWasEnable = glIsEnabled(GL_LIGHTING);
	if (_stateFlags.lightWasEnable) glDisable(GL_LIGHTING);
	
	_stateFlags.blendWasEnable = glIsEnabled(GL_BLEND);
	if (_stateFlags.blendWasEnable) glDisable(GL_BLEND);
	
	glSignalIfError();
}

void
iASkybox::postUpdate(iAENodeComponentID componentID) {
	if (kiANodeDrawableComponent != componentID)
		return;
	
	if (_stateFlags.fronFaceIsConterClockWise) glFrontFace(GL_CCW);
	if (_stateFlags.depthTestWasEnable) glEnable(GL_DEPTH_TEST);
	if (_stateFlags.depthMaskWasEnable) glDepthMask(GL_TRUE);
	if (_stateFlags.lightWasEnable) glEnable(GL_LIGHTING);
	if (_stateFlags.blendWasEnable) glEnable(GL_BLEND);
	glSignalIfError();
}