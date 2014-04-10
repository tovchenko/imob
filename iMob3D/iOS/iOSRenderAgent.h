/*
 ** $Id: 2011/02/03, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iOSRenderAgent_H
#define _iOSRenderAgent_H

#include "iARenderAgent.h"

@class EAGLView;


BEGIN_NAMESPACE_IM

class iOSRenderAgent : public iARenderAgent {
CL_CLASS_DECL(iOSRenderAgent, iARenderAgent)
public:
	virtual _self*			initWithView(EAGLView* view);
	static _self*			agentWithView(EAGLView* view);
	
	void					setView(EAGLView* view);
	EAGLView*				view() const;
	
protected:
	iOSRenderAgent();
	virtual void			dealloc();
	// iAAgent overrides
	virtual void			_start();
	virtual void			_stop();
	
	EAGLView*				_view;
};

END_NAMESPACE_IM

#endif // _iOSRenderAgent_H