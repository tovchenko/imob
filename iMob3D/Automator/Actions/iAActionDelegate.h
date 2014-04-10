/*
 ** $Id: 2011/11/19, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAActionDelegate_H
#define _iAActionDelegate_H

BEGIN_NAMESPACE_IM

class iAAction;

class iAActionDelegate {
public:
	virtual void	actionDidStop(iAAction* action) = 0;
    virtual void	actionDidFinish(iAAction* action) = 0;
};

END_NAMESPACE_IM

#endif // _iAActionDelegate_H
