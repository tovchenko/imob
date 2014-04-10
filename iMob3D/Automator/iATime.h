/*
 ** $Id: 2011/09/22, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iATime_H
#define _iATime_H

#define imDT    iATime::singleton()->deltaTime()

BEGIN_NAMESPACE_IM

class iATime : public dObject {
CL_CLASS_DECL_AS_SINGLETON(iATime, dObject)
friend class iARenderAgent; // _calculateDeltaTime
public:
    void                setupNewSequence();
    dFloat              deltaTime() const;
    
    void                setScale(dFloat scale);
    dFloat              scale() const;
    
protected:
    iATime();
    
    void                _calculateDeltaTime();
    
    struct timeval      _lastUpdate;
    dFloat              _dt;
    bool                _newSequence;
    
    dFloat              _scale;
};

END_NAMESPACE_IM

#endif // _iATime_H
