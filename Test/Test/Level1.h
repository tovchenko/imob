/*
 ** $Id: 2011/02/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _Level1_H
#define _Level1_H

#include "iMob.h"
USING_NAMESPACE_IM


class Level1 : public iAScene {
CL_CLASS_DECL(Level1, iAScene)
protected:
	virtual void		_load();
};


#endif // _Level1_H