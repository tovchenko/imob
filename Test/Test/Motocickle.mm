/*
 ** $Id: 2011/02/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "Motocickle.h"
#include "Assembly.h"

CL_CLASS_DEF(Motocickle, TEST)

Motocickle*
Motocickle::init() {
	_super::init();
	
	iAssetBundle* assets = iADirector::singleton()->assets();
	
	// drawable
	dobj_ptr<iADrawableComponent> dc(CL_NEW(iADrawableComponent));
	iMesh* mesh = assets->mesh(kMeshesMoto);
	iTexture2D* tex = assets->texture2D(kTexturesMoto);	
	dobj_ptr<iDrawable> drwbl(iDrawable::alloc()->initWithMesh(mesh, iMaterial::materialWithTexture(tex)));
	dc->setDrawable(drwbl.get());
	setComponent(kiANodeDrawableComponent, dc.get());
	
	// gui
	dobj_ptr<iAGUIComponent> gc(CL_NEW(iAGUIComponent));
	gc->setDelegate(this);
	iTexture2D* texture = assets->texture2D(kTexturesGui);
	gc->setOverlay(texture);
	gc->setSrcFrame(dMakeRect(0, 0, texture->width(), texture->height()));
	gc->setDstFrame(dMakeRect(10, 266, 128, 256));
	setComponent(kiANodeGUIComponent, gc.get());
	
	return this;
}

void
Motocickle::layout(iAGUIComponent* object, const dRect& viewport, dRect& outSrcFrame, dRect& outDstFrame) {
	outSrcFrame = object->srcFrame();
	
	dRect dstFrame = object->dstFrame();
	outDstFrame = dMakeRect(viewport.origin.x + dstFrame.origin.x,
							viewport.size.height - dstFrame.origin.y,
							dstFrame.size.width,
							dstFrame.size.height);
}