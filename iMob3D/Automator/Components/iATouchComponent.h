/*
 ** $Id: 2011/03/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iATouchComponent_H
#define _iATouchComponent_H

#include "iAComponent.h"

BEGIN_NAMESPACE_IM

class iATouchComponentDelegate;

class iATouchComponent : public iAComponent {
CL_CLASS_DECL(iATouchComponent, iAComponent)
public:
	void						setDelegate(iATouchComponentDelegate* dlg);
	iATouchComponentDelegate*	delegate() const;
	
	virtual void				process(iANode* owner, dDict* params);
	
protected:
	iATouchComponent();
	
	iATouchComponentDelegate*	_delegate;
};

END_NAMESPACE_IM

#endif // _iATouchComponent_H