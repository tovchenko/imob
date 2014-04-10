/*
 ** $Id: 2011/03/24, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iOSTouchAgent_H
#define _iOSTouchAgent_H

#include "iATouchAgent.h"

@class EAGLView;


BEGIN_NAMESPACE_IM

class iOSTouchAgent : public iATouchAgent {
CL_CLASS_DECL(iOSTouchAgent, iATouchAgent)
public:
	virtual _self*			initWithView(EAGLView* view);
	static _self*			agentWithView(EAGLView* view);
	
	void					setView(EAGLView* view);
	EAGLView*				view() const;
	
protected:
	iOSTouchAgent();
	virtual void			dealloc();
	// iAAgent overrides
	virtual void			_start();
	virtual void			_stop();
	
	EAGLView*				_view;
};

END_NAMESPACE_IM

#endif // _iOSTouchAgent_H