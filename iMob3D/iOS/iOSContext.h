/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iOSContext_H
#define _iOSContext_H

#include "iContext.h"

@protocol ESRenderer;

BEGIN_NAMESPACE_IM


class iOSContext : public iSysContext {
CL_CLASS_DECL(iOSContext, iSysContext)
public:
	virtual iOSContext*	initWithUsingShadersApi(bool useShadersApi);
	
	id<ESRenderer>		renderer();
	
	virtual void		makeCurrentContext();
	virtual void		resetCurrentContext();
	virtual void		setup();
	
protected:
	iOSContext();
	virtual void		dealloc();
	
	id<ESRenderer>		_renderer;
};

END_NAMESPACE_IM

#endif // _iOSContext_H