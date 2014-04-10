/*
 ** $Id: 2011/01/29, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iARenderAgent_H
#define _iARenderAgent_H

#include "iScene.h"
#include "iAAgent.h"

BEGIN_NAMESPACE_IM

class iARenderAgent : public iAAgent,
					  public iSceneDelegate {
CL_ABSTRACT_DECL(iARenderAgent, iAAgent)
public:
	// keys
	static dString* const	kContextKey;
	static dString* const	kOverlayRectKey;
	// iSceneDelegate overrides				  
	virtual void			renderDrawablesInContext(iScene* scene, iContext* context);
	virtual void			renderOverlaysInContext(iScene* scene, iContext* context, const CL::dRect& rect);

protected:
    // iAAgent overrides
    virtual void			_start();
    virtual void			_stop();
};

END_NAMESPACE_IM

#endif // _iARenderAgent_H