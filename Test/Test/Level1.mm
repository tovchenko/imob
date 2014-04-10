/*
 ** $Id: 2011/02/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "Level1.h"
#include "Assembly.h"
#include "Motocickle.h"
#include "Cube.h"
#include "Camera.h"

CL_CLASS_DEF(Level1, TEST)

void
Level1::_load() {
    dInteger texIDs[] = {
        kTexturesGui, kTexturesMoto,
        kTexturesSkyPos_X, kTexturesSkyNeg_X,
        kTexturesSkyPos_Y, kTexturesSkyNeg_Y,
        kTexturesSkyPos_Z, kTexturesSkyNeg_Z
    };
	CArrayT<dInteger> txtIDs(texIDs, ITEMS_COUNT_FROM_ARRAY(texIDs));
	
    dInteger mIDs[] = {
        kMeshesMoto
    };
	CArrayT<dInteger> meshIDs(mIDs, ITEMS_COUNT_FROM_ARRAY(mIDs));
	
	iAssetBundle* ab = iADirector::singleton()->assets();
	ab->preloadTextures(txtIDs);
	ab->preloadMeshes(meshIDs);
	
	dobj_ptr<Camera> cam(CL_NEW(Camera));
	dobj_ptr<iASkybox> box(iASkybox::alloc()->initWithTextures(
							ab->texture2D(kTexturesSkyPos_X), ab->texture2D(kTexturesSkyNeg_X),
							ab->texture2D(kTexturesSkyPos_Y), ab->texture2D(kTexturesSkyNeg_Y),
							ab->texture2D(kTexturesSkyPos_Z), ab->texture2D(kTexturesSkyNeg_Z)));
	cam->setSkybox(box.get());
	
	dobj_ptr<Motocickle> moto(CL_NEW(Motocickle));
	dobj_ptr<Motocickle> moto2(CL_NEW(Motocickle));
	dobj_ptr<Motocickle> moto3(CL_NEW(Motocickle));
    dobj_ptr<Motocickle> moto4(CL_NEW(Motocickle));
	dobj_ptr<Cube> cube(CL_NEW(Cube));
	iANode* container = iANode::node();
	iANode* container2 = iANode::node();
	
	moto->transform()->setLocalPosition(vec3f(0, 0, 0));
	moto->transform()->setLocalRotation(quaternionf(M_PI / 4, vec3f::UNIT_Y));
	moto2->transform()->setLocalPosition(vec3f(5, 2, 1));
	moto2->transform()->setLocalRotation(quaternionf(M_PI / 8, vec3f::UNIT_Y));
//	moto3->transform()->setLocalPosition(vec3f(0, -3, 0));
	cube->transform()->setLocalPosition(vec3f(0, -2, 2));
    moto4->transform()->setLocalPosition(vec3f(0, -2.5, 15));
	container->transform()->setLocalPosition(vec3f(0, 0, 5));
	container2->transform()->setLocalPosition(vec3f(0, 0, 10));
	
	dobj_ptr<iANode> root(CL_NEW(iANode));
	
	root->addChild(container);
	container->addChild(container2);
	container2->addChild(cam.get());
	root->addChild(moto.get());
	root->addChild(cube.get());
    root->addChild(moto4.get());
    
//	moto2->addChild(moto3.get());
	moto->addChild(moto2.get());
	setRootNode(root.get());
    
    
    iARotateBy* rotate = iARotateBy::actionWithQuat(5, quaternionf(3 * M_PI_2, vec3f::UNIT_Y));
    iAMoveBy* move = iAMoveBy::actionWithPos(3, vec3f(2, 0, 0));
    iAToggleVisibility* visibility = iAToggleVisibility::action();
    iADelayTime* delay = iADelayTime::actionWithDuration(1);

    iASequence* acts = iASequence::actionWithActions(
                                rotate, move, visibility,
                                delay, visibility, delay,
                                visibility, delay, visibility,
                                move->reverse(), nil);
    
//    iARepeatForever* forv = iARepeatForever::actionWithAction(acts);
    
    iARepeat* rep = iARepeat::actionWithAction(acts, 2);
    iASpeed* speed = iASpeed::actionWithAction(rep, 3);
    moto4->runAction(speed);
}