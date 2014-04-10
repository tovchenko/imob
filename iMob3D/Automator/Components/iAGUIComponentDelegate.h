/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAGUIComponentDelegate_H
#define _iAGUIComponentDelegate_H

BEGIN_NAMESPACE_IM

class iAGUIComponent;

class iAGUIComponentDelegate {
public:
	virtual void	layout(iAGUIComponent* object, const dRect& viewport, dRect& outSrcFrame, dRect& outDstFrame) = 0;
};

END_NAMESPACE_IM

#endif // _iAGUIComponentDelegate_H