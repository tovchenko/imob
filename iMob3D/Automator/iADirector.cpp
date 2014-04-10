/*
 ** $Id: 2011/02/03, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iADirector.h"
#include "iAScene.h"
#include "iAAgent.h"

CL_CLASS_DEF_AS_SINGLETON(iADirector, IM)

iADirector::iADirector()
: _scenes(nil),
  _agents(nil),
  _assets(nil) {
}

void
iADirector::dealloc() {
	_setEnableAgents(false);
	
	CL_RELEASE(_scenes);
	CL_RELEASE(_agents);
	CL_RELEASE(_assets);
	_super::dealloc();
}

void
iADirector::setAssets(iAssetBundle* manager) {
	CL_SWITCH_LINKS(_assets, manager);
}

iAssetBundle*
iADirector::assets() const {
	return _assets;
}

void
iADirector::pushScene(iAScene* scene) {
	CL_THROW_IF_NIL(scene);
	if (!_scenes) _scenes = CL_NEW(dArrayT<iAScene*>);
	
	_setEnableAgents(false);
	_scenes->addObject(scene);
	scene->_load();
	_setEnableAgents(true);
}

void
iADirector::popScene() {
	if (_scenes && 0 != _scenes->count()) {
		_setEnableAgents(false);
		_scenes->lastObject()->_unload();
		_scenes->removeLastObject();
		
		if (0 != _scenes->count())
			_setEnableAgents(true);
	}
}

void
iADirector::replaceScene(iAScene* scene) {
	CL_THROW_IF_NIL(scene);
	if (_scenes && 0 != _scenes->count()) {
		_setEnableAgents(false);
		_scenes->lastObject()->_unload();
		_scenes->replaceObjectAtIndexWithObject(_scenes->count() - 1, scene);
		scene->_load();
		_setEnableAgents(true);
	}
}

iAScene*
iADirector::runningScene() const {
	return (_scenes && 0 != _scenes->count()) ? _scenes->lastObject() : nil;
}

dDictT<iAAgent*, dString*>*
iADirector::agents() {
	if (!_agents)
		_agents = (dDictT<iAAgent*, dString*>*)dDictT<iAAgent*, dString*>::alloc()->init();
	return _agents;
}

void
iADirector::_setEnableAgents(bool enabled) {
	dDictT<iAAgent*, dString*>::Iterator it(_agents);
	if (enabled) while (it.next()) it.object()->_start();
	else while (it.next()) it.object()->_stop();
}