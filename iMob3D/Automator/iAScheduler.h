/*
 ** $Id: 2011/09/22, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAScheduler_H
#define _iAScheduler_H

#define kIMRepeatForever    UINT_MAX

class _SelectorEntry;

BEGIN_NAMESPACE_IM

class iAScheduler : public dObject {
CL_CLASS_DECL_AS_SINGLETON(iAScheduler, dObject)
friend class iATime; // _tick
public:
    void                                schedule(
                                            dSEL sel,
                                            dObject* target,
                                            dFloat interval,
                                            dUInteger repeatsCount = kIMRepeatForever,
                                            dFloat delay = 0.0f,
                                            bool paused = false);
    
    void                                unschedule(dSEL sel, dObject* target);
    void                                unscheduleAllSelectorsForTarget(dObject* target);
    
    void                                pauseTarget(dObject* target);
    void                                resumeTarget(dObject* target);
    bool                                isTargetPaused(dObject* target);
    
protected:
    iAScheduler();
    virtual void                        dealloc();
    
    void                                _tick(dFloat dt);
    dDictT<_SelectorEntry*, dObject*>*  _entries();
    
    dDictT<_SelectorEntry*, dObject*>*  _targetEntries;
    _SelectorEntry*                     _currEnt;
    bool                                _currEntSalvaged;
};

END_NAMESPACE_IM

#endif // _iAScheduler_H