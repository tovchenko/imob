/*
 ** $Id: 2011/01/29, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAAgent_H
#define _iAAgent_H


BEGIN_NAMESPACE_IM

class iAAgent : public dObject {
CL_ABSTRACT_DECL(iAAgent, dObject)
friend class iADirector; // _start(), _stop()
protected:
	virtual void	_start() = 0;
	virtual void	_stop() = 0;
};

END_NAMESPACE_IM

#endif // _iAAgent_H