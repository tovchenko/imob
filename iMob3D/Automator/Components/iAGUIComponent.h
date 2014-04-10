/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAGUIComponent_H
#define _iAGUIComponent_H

#include "iAComponent.h"


BEGIN_NAMESPACE_IM

class iTexture2D;
class iAGUIComponentDelegate;

class iAGUIComponent : public iAComponent {
CL_CLASS_DECL(iAGUIComponent, iAComponent)
public:
	void						setDelegate(iAGUIComponentDelegate* dlg);
	iAGUIComponentDelegate*		delegate() const;
	
	void						setOverlay(iTexture2D* texture);
	iTexture2D*					overlay() const;
	
	void						setSrcFrame(const dRect& frame);
	const dRect&				srcFrame() const;
	void						setDstFrame(const dRect& frame);
	const dRect&				dstFrame() const;
    
    void                        setIsHidden(bool hidden);
    bool                        isHidden() const;
	
	virtual void				process(iANode* owner, dDict* params);

protected:
	iAGUIComponent();
	virtual void				dealloc();
	
	iAGUIComponentDelegate*		_delegate;
	iTexture2D*					_overlay;
	dRect						_srcFrame;
	dRect						_dstFrame;
    bool                        _hidden;
};

END_NAMESPACE_IM

#endif // _iAGUIComponent_H