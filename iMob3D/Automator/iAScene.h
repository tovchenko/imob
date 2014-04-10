/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAScene_H
#define _iAScene_H


BEGIN_NAMESPACE_IM

class iANode;

class iAScene : public dObject {
CL_ABSTRACT_DECL(iAScene, dObject)
friend class iADirector; // _load(), _unload()
public:
	void						setRootNode(iANode* node);
	iANode*						rootNode() const;
	
protected:
	iAScene();
	virtual void				dealloc();
	
	// override
	virtual void				_load() = 0;
	virtual void				_unload();
	
	mutable iANode*				_rootNode;
};

END_NAMESPACE_IM

#endif // _iAScene_H