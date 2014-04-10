/*
 ** $Id: 2011/03/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iASkybox_H
#define _iASkybox_H

#include "iANode.h"

BEGIN_NAMESPACE_IM

class iMaterial;
class iTexture2D;

class iASkybox : public iANode {
CL_CLASS_DECL(iASkybox, iANode)
public:
	virtual _self*		initWithTextures(iTexture2D* posX, iTexture2D* negX,
										 iTexture2D* posY, iTexture2D* negY,
										 iTexture2D* posZ, iTexture2D* negZ);
	// overrided
	virtual void		update(iAENodeComponentID componentID);
	virtual void		postUpdate(iAENodeComponentID componentID);
	
protected:
	iASkybox();
	
	struct {
		dUInteger		fronFaceIsConterClockWise:1;
		dUInteger		depthTestWasEnable:1;
		dUInteger		depthMaskWasEnable:1;
		dUInteger		lightWasEnable:1;
		dUInteger		blendWasEnable:1;
	} _stateFlags;
};

END_NAMESPACE_IM

#endif // _iASkybox_H