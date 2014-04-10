/*
 ** $Id: 2011/02/03, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iADirector_H
#define _iADirector_H


BEGIN_NAMESPACE_IM

class iAssetBundle;
class iAScene;
class iAAgent;

class iADirector : public dObject {
CL_CLASS_DECL_AS_SINGLETON(iADirector, dObject)
public:
	void						setAssets(iAssetBundle* manager);
	iAssetBundle*				assets() const;
	// scene management
	void						pushScene(iAScene* scene);
	void						popScene();
	void						replaceScene(iAScene* scene);
	iAScene*					runningScene() const;
	// agents
	dDictT<iAAgent*, dString*>*	agents();
	
protected:
	iADirector();
	virtual void				dealloc();
	
	void						_setEnableAgents(bool enabled);
	
	iAssetBundle*				_assets;
	dArrayT<iAScene*>*			_scenes;
	dDictT<iAAgent*, dString*>*	_agents;
};

END_NAMESPACE_IM

#endif // _iADirector_H