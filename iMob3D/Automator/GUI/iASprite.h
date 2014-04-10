/*
 ** $Id: 2011/04/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iASprite_H
#define _iASprite_H

#include "iANode.h"
#include "iAGUIComponentDelegate.h"

BEGIN_NAMESPACE_IM

class iTexture2D;

class iASprite : public iANode,
				 public iAGUIComponentDelegate
{  
CL_CLASS_DECL(iASprite, iANode)
public:
	static _self*		spriteWithSize(iTexture2D* texture, const dSize& size);
	static _self*		spriteWithFrame(iTexture2D* texture, const dRect& frameOnTexture, const dSize& size);
	virtual _self*		initWithSize(iTexture2D* texture, const dSize& size);
	virtual _self*		initWithFrame(iTexture2D* texture, const dRect& frameOnTexture, const dSize& size);
	
	void				setPosition(const vec2f& pos);
	const vec2f&		position() const;
	
	// iAGUIComponentDelegate
	virtual void		layout(iAGUIComponent* object, const dRect& viewport, dRect& outSrcFrame, dRect& outDstFrame);
	
protected:
	iASprite();
	
	vec2f				_position;
	dRect				_frameOnTexture;
	dSize				_size;
};

END_NAMESPACE_IM

#endif // _iASprite_H
