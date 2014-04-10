/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAScene.h"
#include "iANode.h"

CL_ABSTRACT_DEF(iAScene, IM)

iAScene::iAScene()
: _rootNode(nil) {
}

void
iAScene::dealloc() {
	CL_RELEASE(_rootNode);
	_super::dealloc();
}

void
iAScene::setRootNode(iANode* node) {
	CL_SWITCH_LINKS(_rootNode, node);
}

iANode*
iAScene::rootNode() const {
	if (!_rootNode)
		_rootNode = CL_NEW(iANode);
	
	return _rootNode;
}

void
iAScene::_unload() {
	// override
}