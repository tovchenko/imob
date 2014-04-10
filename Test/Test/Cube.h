/*
 ** $Id: 2011/03/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _Cube_H
#define _Cube_H

#include "iMob.h"
USING_NAMESPACE_IM

class Cube : public iANode {
CL_CLASS_DECL(Cube, iANode)
public:
	virtual _self*		init();
	
	virtual void		update(iAENodeComponentID componentID);
    
    CL_METHOD_DECL(onDirectionChange);
    CL_METHOD_DECL(onCancel);
    
protected:
    Cube();
    
    dInteger            _dir;
};


#endif // _Cube_H