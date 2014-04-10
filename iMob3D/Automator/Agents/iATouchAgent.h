/*
 ** $Id: 2011/03/24, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iATouchAgent_H
#define _iATouchAgent_H

#include "iScene.h"
#include "iAAgent.h"

BEGIN_NAMESPACE_IM

enum {
	iAETouchPhase_Began,
	iAETouchPhase_Moved,
	iAETouchPhase_Ended,
	iAETouchPhase_Cancelled
};
typedef dUInteger iAETouchPhase;

class iATouch;

class iATouchAgent : public iAAgent {
CL_ABSTRACT_DECL(iATouchAgent, iAAgent)
public:
	// keys
	static dString* const	kTouchesKey;
	static dString* const	kPhaseKey;
	static dString* const	kBoundsKey;
	// collect touches
	void					sendTouches(dSetT<iATouch*>* touches, const dRect& bounds, iAETouchPhase phase);
};

END_NAMESPACE_IM

#endif // _iATouchAgent_H

