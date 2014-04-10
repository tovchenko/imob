/*
 ** $Id: 2010/07/18, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iScene_H
#define _iScene_H


BEGIN_NAMESPACE_IM

class iContext;
class iSysContext;
class iSceneDelegate;

class iScene : public dObject {
CL_CLASS_DECL(iScene, dObject)
public:
	void				setDelegate(iSceneDelegate* dlg);
	iSceneDelegate*		delegate() const;
	
	virtual void		setupContext(iContext* context);
	virtual void		renderInContext(iContext* context);
	
protected:
	iScene();
	
	iSceneDelegate*		_delegate;
};


class iSceneDelegate {
public:
	virtual void	renderDrawablesInContext(iScene* scene, iContext* context) = 0;
	virtual void	renderOverlaysInContext(iScene* scene, iContext* context, const dRect& rect) = 0;
};

END_NAMESPACE_IM

#endif // _iScene_H