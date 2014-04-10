/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iADrawableComponent_H
#define _iADrawableComponent_H

#include "iAComponent.h"


BEGIN_NAMESPACE_IM

class iDrawable;

class iADrawableComponent : public iAComponent {
CL_CLASS_DECL(iADrawableComponent, iAComponent)
public:
	void				setDrawable(iDrawable* drawbl);
	iDrawable*			drawable() const;
    
    void                setIsHidden(bool hidden);
    bool                isHidden() const;
	
	virtual void		process(iANode* owner, dDict* params);
	
protected:
	iADrawableComponent();
	virtual void		dealloc();
	
	iDrawable*			_drawable;
    bool                _hidden;
};

END_NAMESPACE_IM

#endif // _iADrawableComponent_H