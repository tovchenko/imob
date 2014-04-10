/*
 ** $Id: 2011/01/29, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iANode.h"
#include "iAComponent.h"
#include "iATransform.h"
#include "iAScheduler.h"
#include "iAActionManager.h"


static void		_visitCallback(iANode* node, iAENodeComponentID cmp, dDict* params);


CL_CLASS_DEF(iANode, IM)

iANode::iANode()
: _transform(nil) {
	dobj_ptr<dArrayT<iAComponent*> > components(CL_NEW(dArrayT<iAComponent*>));
	for (dUInteger i = 0; i < kiANodeComponentsEndMarker; ++i)
		components->addObject(CL_NULL(iAComponent));
	
	setValue(components.get());
}

void
iANode::dealloc() {
	CL_RELEASE(_transform);
    
	_super::dealloc();
}

iANode*
iANode::node() {
	_self* im = (_self*)alloc()->init();
	return im ? im->autorelease() : nil; 
}

void
iANode::visitComponents(iAENodeComponentID componentID, dDict* params) {
	visit(_visitCallback, componentID, params);
}

void
iANode::visit(iANodeCB callback, iAENodeComponentID cmp, dDict* params) {
	dobj_ptr<dArrayT<_self*> > buffer1(CL_NEW(dArrayT<_self*>));
	dobj_ptr<dArrayT<_self*> > buffer2(CL_NEW(dArrayT<_self*>));
	
	buffer1->addObject(this);
	
	while (0 != buffer1->count()) {
		for (dUInteger i = 0, cnt = buffer1->count(); i < cnt; ++i) {
			_self* child = buffer1->objectAtIndex(i);
			
			callback(child, cmp, params);
			
			for (dUInteger j = 0, jcnt = child->childrenCount(); j < jcnt; ++j)
				buffer2->addObject(child->childAtIndex(j));
		}
		
		buffer1->removeAllObjects();
		dobj_ptr<dArrayT<_self*> > tmp(buffer1);
		buffer1 = buffer2;
		buffer2 = tmp;
	}
}

static void
_visitCallback(iANode* node, iAENodeComponentID cmp, dDict* params) {
	iAComponent* component = node->component(cmp);
	if (component) {
		node->update(cmp);
		component->process(node, params);
		node->postUpdate(cmp);
	}
}

void
iANode::_didDetach() {
    iAScheduler::singleton()->unscheduleAllSelectorsForTarget(this);
    iAActionManager::singleton()->removeAllActionsForTarget(this);
}

void
iANode::setComponent(iAENodeComponentID  componentID, iAComponent* component) {
	// TODO: need check count() for additional adding user defined components
	value()->replaceObjectAtIndexWithObject(componentID, component);
}

iAComponent*
iANode::component(iAENodeComponentID  componentID) {
    iAComponent* res = value()->objectAtIndex(componentID);
	return res != CL_NULL(iAComponent) ? res : nil;
}

void
iANode::setTransform(iATransform* trnsf) {
	if (_transform == trnsf) return;
	CL_RELEASE(_transform);
	
	if (trnsf) {
		_transform = trnsf->retain();
		
		_transform->_ownerNode->setTransform(nil);
		_transform->_ownerNode->transform()->invalidateCacheHierarchical();
		
		_transform->_ownerNode = this;
		_transform->invalidateCacheHierarchical();
	}
}

iATransform*
iANode::transform() {
	if (!_transform) {
		_transform = CL_NEW(iATransform);
		_transform->_ownerNode = this;
	}
	return _transform;
}

void
iANode::schedule(dSEL sel, double interval, dUInteger repeatsCount, double delay, bool paused) {
    iAScheduler::singleton()->schedule(sel, this, interval, repeatsCount, delay, paused);
}

void
iANode::unschedule(dSEL sel) {
    iAScheduler::singleton()->unschedule(sel, this);
}

iAAction*
iANode::runAction(iAAction* action) {
    CL_THROW_IF_NIL(action);
    
    iAActionManager::singleton()->addAction(action, this);
    return action;
}

void
iANode::update(iAENodeComponentID componentID) {
	// override
}

void
iANode::postUpdate(iAENodeComponentID componentID) {
	// override
}