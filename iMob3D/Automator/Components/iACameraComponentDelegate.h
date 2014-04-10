/*
 ** $Id: 2011/02/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iACameraComponentDelegate_H
#define _iACameraComponentDelegate_H

BEGIN_NAMESPACE_IM

class iContext;
class iACameraComponent;

class iACameraComponentDelegate {
public:
	virtual void	applyCameraInContext(iACameraComponent* component, iContext* context) = 0;
};

END_NAMESPACE_IM

#endif // _iACameraComponentDelegate_H