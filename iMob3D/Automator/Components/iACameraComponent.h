/*
 ** $Id: 2011/02/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iACameraComponent_H
#define _iACameraComponent_H

#include "iAComponent.h"

BEGIN_NAMESPACE_IM

class iACameraComponentDelegate;

class iACameraComponent : public iAComponent {
CL_CLASS_DECL(iACameraComponent, iAComponent)
public:
	void						setDelegate(iACameraComponentDelegate* dlg);
	iACameraComponentDelegate*	delegate() const;
	
	virtual void				process(iANode* owner, dDict* params);
	
protected:
	iACameraComponent();
	
	iACameraComponentDelegate*	_delegate;
};

END_NAMESPACE_IM

#endif // _iACameraComponent_H