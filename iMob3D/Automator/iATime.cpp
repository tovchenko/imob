/*
 ** $Id: 2011/09/22, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iATime.h"
#include "iAScheduler.h"
#include "iAActionManager.h"

#include <sys/time.h>


CL_CLASS_DEF_AS_SINGLETON(iATime, IM)

iATime::iATime()
: _dt(0.0f),
  _newSequence(true),
  _scale(1.0f) {
    ZERO_MEMORY_STRUCT(_lastUpdate);
}

void
iATime::_calculateDeltaTime() {
    struct timeval current;
    if (0 != gettimeofday(&current, NULL)) {
        CL_THROW_IF_TRUE(true);
        _dt = 0.0f;
        return;
    }
    
    if (_newSequence) {
        _dt = 0.0f;
        _newSequence = false;
    } else {
        _dt = (current.tv_sec - _lastUpdate.tv_sec) + (current.tv_usec - _lastUpdate.tv_usec) / 1000000.0f;
        _dt = __max(0.0f, _dt);
    }
    
    _lastUpdate = current;
    
    iAScheduler::singleton()->_tick(_dt);
    iAActionManager::singleton()->_tick(_dt);
}

void
iATime::setupNewSequence() {
    _newSequence = true;
}

dFloat
iATime::deltaTime() const {
    return _dt * _scale;
}

void
iATime::setScale(dFloat scale) {
    _scale = scale;
}

dFloat
iATime::scale() const {
    return _scale;
}