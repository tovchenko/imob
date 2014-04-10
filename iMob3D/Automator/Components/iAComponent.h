/*
 ** $Id: 2011/01/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAComponent_H
#define _iAComponent_H


BEGIN_NAMESPACE_IM

class iANode;

class iAComponent : public dObject {
CL_ABSTRACT_DECL(iAComponent, dObject)
public:
	virtual void		process(iANode* owner, dDict* params) = 0;
};

END_NAMESPACE_IM

#endif // _iAComponent_H