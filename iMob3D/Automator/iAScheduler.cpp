/*
 ** $Id: 2011/09/22, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAScheduler.h"
#include "iATime.h"


class _LightTimer : public dObject {
CL_CLASS_DECL(_LightTimer, dObject)
public:
    static _self*       timerWithParams(
                            dObject* target,
                            dSEL sel,
                            dFloat interval,
                            dUInteger repeatsCount = kIMRepeatForever,
                            dFloat delay = 0.0f);
    
    virtual _self*      initWithParams(
                            dObject* target,
                            dSEL sel,
                            dFloat interval,
                            dUInteger repeatsCount = kIMRepeatForever,
                            dFloat delay = 0.0f);
    
    dSEL                selector() const;
    void                setInterval(dFloat interval);
    dFloat              interval() const;
    
    void                update(dFloat dt);
    
protected:
    _LightTimer();
    virtual void        dealloc();
    
    dInvocation*        _invocation;
    
    dFloat              _elapsed;
    dFloat              _interval;
    
    bool                _runForever;
    bool                _useDelay;
    dUInteger           _nTimesExecuted;
    dUInteger           _repeatsCount;
    double              _delay;
};

CL_CLASS_DEF(_LightTimer, IM)

_LightTimer::_LightTimer()
: _invocation(nil),
  _elapsed(-1.0f),
  _interval(0.0f),
  _runForever(false),
  _useDelay(false),
  _nTimesExecuted(0),
  _repeatsCount(kIMRepeatForever),
  _delay(0.0f) {
}

_LightTimer*
_LightTimer::timerWithParams(dObject* target, dSEL sel, dFloat interval, dUInteger repeatsCount, dFloat delay) {
    _self* im = _self::alloc()->initWithParams(target, sel, interval, repeatsCount, delay);
    return im ? im->autorelease() : nil;
}

_LightTimer*
_LightTimer::initWithParams(dObject* target, dSEL sel, dFloat interval, dUInteger repeatsCount, dFloat delay) {
    _super::init();
    
    _invocation = dInvocation::alloc()->initWithParams(target, sel, nil);
    _interval = interval;
    _repeatsCount = repeatsCount;
    _delay = delay;
    _useDelay = (delay > 0) ? true : false;
    _runForever = (kIMRepeatForever == repeatsCount) ? true : false;
    
    return this;
}

void
_LightTimer::dealloc() {
    CL_RELEASE(_invocation);
    
    _super::dealloc();
}

dSEL
_LightTimer::selector() const {
    return _invocation->selector();
}

void
_LightTimer::setInterval(dFloat interval) {
    _interval = interval;
}

dFloat
_LightTimer::interval() const {
    return _interval;
}

void
_LightTimer::update(dFloat dt) {
    if (-1.0f == _elapsed) {
        _elapsed = 0.0f;
        _nTimesExecuted = 0;
    } else {
        if (_runForever && !_useDelay) {
            _elapsed += dt;
            if (_elapsed >= _interval) {
                _invocation->invoke();
                _elapsed = 0.0f;
            }
        } else {
            _elapsed += dt;
            if (_useDelay) {
                if (_elapsed >= _delay) {
                    _invocation->invoke();
                    _elapsed -= _delay;
                    ++_nTimesExecuted;
                    _useDelay = false;
                }
            } else if (_elapsed >= _interval) {
                _invocation->invoke();
                _elapsed = 0.0f;
                ++_nTimesExecuted;
            }
            
            if (_nTimesExecuted >= _repeatsCount) {
                iAScheduler::singleton()->unschedule(_invocation->selector(), _invocation->target());
            }
        }
    }
}

// =========================================================================================

class _SelectorEntry : public dObject {
CL_CLASS_DECL(_SelectorEntry, dObject)
public:
    dArrayT<_LightTimer*>*      timers();
    dUInteger                   timerIndex;
    _LightTimer*                currentTimer;
    bool                        currentTimerSalvaged;
    bool                        paused;
    
protected:
    _SelectorEntry();
    virtual void                dealloc();
    dArrayT<_LightTimer*>*      _timers;
};

CL_CLASS_DEF(_SelectorEntry, IM)

_SelectorEntry::_SelectorEntry()
: _timers(nil),
  timerIndex(0),
  currentTimer(nil),
  currentTimerSalvaged(false),
  paused(false) {
}

void
_SelectorEntry::dealloc() {
    CL_RELEASE(_timers);
    
    _super::dealloc();
}

dArrayT<_LightTimer*>*
_SelectorEntry::timers() {
    if (!_timers) _timers = CL_NEW(dArrayT<_LightTimer*>);
    return _timers;
}

// ==========================================================================================

CL_CLASS_DEF_AS_SINGLETON(iAScheduler, IM)

iAScheduler::iAScheduler()
: _targetEntries(nil),
  _currEnt(nil),
  _currEntSalvaged(false) {
}

void
iAScheduler::dealloc() {
    CL_RELEASE(_targetEntries);
    
    _super::dealloc();
}

void
iAScheduler::schedule(dSEL sel, dObject* target, dFloat interval, dUInteger repeatsCount, dFloat delay, bool paused) {
    CL_THROW_IF_NIL(sel);
    CL_THROW_IF_NIL(target);
    
    _SelectorEntry* entry = _entries()->objectForKeyNoException(target);
    if (!entry) {
        dobj_ptr<_SelectorEntry> ptrEntry(CL_NEW(_SelectorEntry));
        entry = ptrEntry.get();
        _entries()->setObjectForKey(entry, target);
        entry->paused = paused;
    }
    
    for (dUInteger i = 0; i < entry->timers()->count(); ++i) {
        _LightTimer* timer = entry->timers()->objectAtIndex(i);
        if (timer->selector() == sel) {
            timer->setInterval(interval);
            return;
        }
    }
    
    dobj_ptr<_LightTimer> timer(_LightTimer::alloc()->initWithParams(target, sel, interval, repeatsCount, delay));
    entry->timers()->addObject(timer.get());
}

void
iAScheduler::unschedule(dSEL sel, dObject* target) {
    CL_THROW_IF_NIL(sel);
    
    _SelectorEntry* entry = _entries()->objectForKeyNoException(target);
    if (!entry) return;
    
    for (dUInteger i = 0; i < entry->timers()->count(); ++i) {
        _LightTimer* timer = entry->timers()->objectAtIndex(i);
        
        if (timer->selector() == sel) {
            if (timer == entry->currentTimer && !entry->currentTimerSalvaged) {
                CL_RETAIN(timer);
                entry->currentTimerSalvaged = true;
            }
            
            entry->timers()->removeObjectAtIndex(i);

            if (entry->timerIndex >= i)
                --entry->timerIndex;
            
            if (0 == entry->timers()->count()) {
                if (_currEnt == entry) _currEntSalvaged = true;
                else _entries()->removeObjectForKey(target);
            }
            
            return;
        }
    }
}

void
iAScheduler::unscheduleAllSelectorsForTarget(dObject* target) {
    if (!target) return;
    
    _SelectorEntry* entry = _entries()->objectForKeyNoException(target);
    if (entry) {
        if (entry->timers()->containsObject(entry->currentTimer) && !entry->currentTimerSalvaged) {
            CL_RETAIN(entry->currentTimer);
            entry->currentTimerSalvaged = true;
        }
        
        entry->timers()->removeAllObjects();
        if (_currEnt == entry) _currEntSalvaged = true;
        else _entries()->removeObjectForKey(target);
    }
}

void
iAScheduler::pauseTarget(dObject* target) {
    CL_THROW_IF_NIL(target);
    
    _SelectorEntry* entry = _entries()->objectForKeyNoException(target);
    if (entry) entry->paused = true;
}

void
iAScheduler::resumeTarget(dObject* target) {
    CL_THROW_IF_NIL(target);
    
    _SelectorEntry* entry = _entries()->objectForKeyNoException(target);
    if (entry) entry->paused = false;
}

bool
iAScheduler::isTargetPaused(dObject* target) {
    CL_THROW_IF_NIL(target);
    
    _SelectorEntry* entry = _entries()->objectForKeyNoException(target);
    if (entry) return entry->paused;
    return false;
}

void
iAScheduler::_tick(dFloat dt) {
    dDictT<_SelectorEntry*, dObject*>::Iterator it(_entries());
    while (it.next()) {
        _currEnt = it.object();
        _currEntSalvaged = false;
        
        if (!_currEnt->paused) {
            for (_currEnt->timerIndex = 0; _currEnt->timerIndex < _currEnt->timers()->count(); ++_currEnt->timerIndex) {
                _currEnt->currentTimer = _currEnt->timers()->objectAtIndex(_currEnt->timerIndex);
                _currEnt->currentTimerSalvaged = false;
                
                _currEnt->currentTimer->update(dt);
                
                if (_currEnt->currentTimerSalvaged)
                    _currEnt->currentTimer->release();
                _currEnt->currentTimer = nil;
            }
        }
        
        if (_currEntSalvaged && 0 == _currEnt->timers()->count())
            _entries()->removeObjectForKey(it.key());
    }
    
    _currEnt = nil;
}

dDictT<_SelectorEntry*, dObject*>*
iAScheduler::_entries() {
    if (!_targetEntries)
        _targetEntries = (dDictT<_SelectorEntry*, dObject*>*)dDictT<_SelectorEntry*, dObject*>::alloc()->init();
    return _targetEntries;
}