/*
 ** $Id: 2011/02/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _Motocickle_H
#define _Motocickle_H

#include "iMob.h"
USING_NAMESPACE_IM

class Motocickle : public iANode,
				   public iAGUIComponentDelegate {
CL_CLASS_DECL(Motocickle, iANode)
public:
	virtual _self*		init();
	// iAGUIComponentDelegate
	virtual void		layout(iAGUIComponent* object, const dRect& viewport, dRect& outSrcFrame, dRect& outDstFrame);
};


#endif // _Motocickle_H