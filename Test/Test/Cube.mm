/*
 ** $Id: 2011/03/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "Cube.h"
#include "Assembly.h"

CL_CLASS_DEF(Cube, TEST)

Cube::Cube()
: _dir(1) {
}

Cube*
Cube::init() {
	_super::init();
	
	iAssetBundle* assets = iADirector::singleton()->assets();
	
	// drawable
	dobj_ptr<iADrawableComponent> dc(CL_NEW(iADrawableComponent));
	iMesh* mesh = iCubeMesh::cubeWithSideSizeAndSegmentsCount(2, 5);
	iTexture2D* tex = assets->texture2D(kTexturesWall1);	
	dobj_ptr<iDrawable> drwbl(iDrawable::alloc()->initWithMesh(mesh, iMaterial::materialWithTexture(tex)));
	dc->setDrawable(drwbl.get());
	setComponent(kiANodeDrawableComponent, dc.get());
    
    schedule(dselector(onDirectionChange), 3.0, 5, 7.0);
    schedule(dselector(onCancel), 15.0, kIMRepeatForever);
	
	return this;
}

void
Cube::update(iAENodeComponentID componentID) {
	if (kiANodeDrawableComponent != componentID)
		return;
	
	iATransform* tr = transform();
	tr->setLocalRotation(quaternionf(_dir * M_PI / 200, vec3f::UNIT_Y) * tr->localRotation());
	tr->setLocalRotation(quaternionf(M_PI / 300, vec3f::UNIT_X) * tr->localRotation());
}

CL_METHOD_DEF(Cube, onDirectionChange) {
    _dir = -_dir;
    return nil;
}

CL_METHOD_DEF(Cube, onCancel) {
    unschedule(dselector(onCancel));
//    unschedule(dselector(onDirectionChange));
    
    return nil;
}