/*
 ** $Id: 2011/10/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAIntervalAction.h"
#include "iANode.h"
#include "iATransform.h"

CL_ABSTRACT_DEF(iAIntervalAction, IM)

iAIntervalAction::iAIntervalAction()
: _elapsed(0.0f),
  _isFirstTick(true) {
}

iAIntervalAction*
iAIntervalAction::initWithDuration(dFloat duration) {
    _super::init();
    setDuration(duration);
    return this;
}

iAIntervalAction*
iAIntervalAction::copy() const {
    _self* res = (iAIntervalAction*)_super::copy();
    res->_duration = _duration;
    return res;
}

dFloat
iAIntervalAction::elapsed() const {
    return _elapsed;
}

void
iAIntervalAction::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    _elapsed = 0.0f;
    _isFirstTick = true;
}

bool
iAIntervalAction::isDone() const {
    return _elapsed >= _duration;
}

void
iAIntervalAction::_update(dFloat dt) {
    if (_isFirstTick) {
        _isFirstTick = false;
        _elapsed = 0.0f;
    } else {
        _elapsed += dt;
    }
    
    _progressUpdate(__min(1, _elapsed / __max(_duration, std::numeric_limits<dFloat>::epsilon())));
}

// =============================================================================================================

CL_CLASS_DEF(iASequence, IM)

iASequence::iASequence() {
    ZERO_MEMORY_ARRAY(_actions);
}

iASequence*
iASequence::actionWithArray(dArrayT<iAFiniteTimeAction*>* actions) {
    _self* im = alloc()->initWithArray(actions);
    return im ? im->autorelease() : nil;
}

iASequence*
iASequence::actionWith2(iAFiniteTimeAction* one, iAFiniteTimeAction* two) {
    _self* im = alloc()->initWith2(one, two);
    return im ? im->autorelease() : nil;
}

iASequence*
iASequence::actionWithActions(iAFiniteTimeAction* first, ...) {
    va_list args;
    va_start(args, first);
    _self* im = alloc()->_initWithVAList(first, args);
    va_end(args);
    
    return im ? im->autorelease() : nil;
}

iASequence*
iASequence::initWithArray(dArrayT<iAFiniteTimeAction*>* actions) {
    CL_THROW_IF_NIL(actions);
    
    dobj_ptr<iAFiniteTimeAction> prev(actions->objectAtIndex(0)->retain());
    
    for (dInteger i = 1, cnt = actions->count(); i < cnt; ++i) {
        dobj_ptr<iAFiniteTimeAction> seq(alloc()->initWithActions(prev.get(), actions->objectAtIndex(i)));
        prev = seq;
    }
    
    release();
    return (iASequence*)prev.get()->retain();
}

iASequence*
iASequence::initWith2(iAFiniteTimeAction* one, iAFiniteTimeAction* two) {
    CL_THROW_IF_NIL(one && two);
    
    _super::initWithDuration(one->duration() + two->duration());
    
    CL_SWITCH_LINKS(_actions[0], one);
    CL_SWITCH_LINKS(_actions[1], two);
    
    return this;
}

iASequence*
iASequence::initWithActions(iAFiniteTimeAction* first, ...) {
    va_list args;
    va_start(args, first);
    _self* im = _initWithVAList(first, args);
    va_end(args);
    
    return im;
}

iASequence*
iASequence::_initWithVAList(iAFiniteTimeAction* first, va_list args) {
    dobj_ptr<iAFiniteTimeAction> prev(first->retain());
    
    while (true) {
        iAFiniteTimeAction* now = va_arg(args, iAFiniteTimeAction*);
        if (now) {
            dobj_ptr<iAFiniteTimeAction> tmp(alloc()->initWith2(prev.get(), now));
            prev = tmp;
        }
        else break;
    }
    
    release();
    return (iASequence*)prev.get()->retain();
}

void
iASequence::dealloc() {
    CL_RELEASE(_actions[0]);
    CL_RELEASE(_actions[1]);
    
    _super::dealloc();
}

iASequence*
iASequence::copy() const {
    _self* res = (iASequence*)_super::copy();
    res->_actions[0] = _actions[0]->copy();
    res->_actions[1] = _actions[1]->copy();
    return res;
}

iASequence*
iASequence::reverse() const {
    return actionWithActions(_actions[0]->reverse(), _actions[1]->reverse());
}

void
iASequence::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    _split = _actions[0]->duration() / __max(_duration, std::numeric_limits<dFloat>::epsilon());
    _last = -1;
}

void
iASequence::_stop() {
    _actions[0]->_stop();
    _actions[1]->_stop();
    _super::_stop();
}

void
iASequence::_progressUpdate(dFloat level) {
    CL_THROW_IF_NIL(_target);
    
    dInteger found = 0;
    dFloat t = 0.0f;
    
    if (level >= _split) {
        found = 1;
        if (1 == _split) t = 1;
        else t = (level - _split) / (1 - _split);
    } else {
        found = 0;
        if (0 != _split) t = level / _split;
        else t = 1;
    }
    
    if (-1 == _last && 1 == found) {
        _actions[0]->startWithTarget(_target);
        _actions[0]->_progressUpdate(1.0f);
        _actions[0]->_finish();
        _actions[0]->_stop();
    }
    
    if (_last != found) {
        if (-1 != _last) {
            _actions[_last]->_progressUpdate(1.0f);
            _actions[_last]->_finish();
            _actions[_last]->_stop();
        }
        _actions[found]->startWithTarget(_target);
    }
    
    _actions[found]->_progressUpdate(t);
    _last = found;
}

// =============================================================================================================

CL_CLASS_DEF(iARepeat, IM)

iARepeat::iARepeat()
: _innerAction(nil) {
}

iARepeat*
iARepeat::actionWithAction(iAFiniteTimeAction* action, dUInteger times) {
    _self* im = alloc()->initWithAction(action, times);
    return im ? im->autorelease() : nil;
}

iARepeat*
iARepeat::initWithAction(iAFiniteTimeAction* action, dUInteger times) {
    CL_THROW_IF_NIL(action);
    
    _super::initWithDuration(times * action->duration());
    _times = times;
    setInnerAction(action);
    
    return this;
}

void
iARepeat::dealloc() {
    CL_RELEASE(_innerAction);
    
    _super::dealloc();
}

iARepeat*
iARepeat::copy() const {
    _self* res = (iARepeat*)_super::copy();
    res->_times = _times;
    res->_innerAction = _innerAction ? _innerAction->copy() : nil;
    
    return res;
}

void
iARepeat::setInnerAction(iAFiniteTimeAction* action) {
    CL_SWITCH_LINKS(_innerAction, action);
}

iAFiniteTimeAction*
iARepeat::innerAction() const {
    return _innerAction;
}

iARepeat*
iARepeat::reverse() const {
    return _innerAction ? actionWithAction(_innerAction->reverse(), _times) : nil;
}

void
iARepeat::startWithTarget(iANode* target) {
    CL_THROW_IF_NIL(target);
    if (!_innerAction) return;
    
    _stopCnt = 0;
    _dt += _innerAction->duration() / _duration;
    _super::startWithTarget(target);
    _innerAction->startWithTarget(target);
    _startCnt = 1;
}

bool
iARepeat::isDone() const {
    return _stopCnt == _times;
}

void
iARepeat::_stop() {
    if (_innerAction) _innerAction->_stop();
    _super::_stop();
}

void
iARepeat::_progressUpdate(dFloat level) {
    if (!_innerAction) return;
    
    if (level >= _dt) {
        if (_startCnt > _stopCnt) {
            _innerAction->_progressUpdate(1.0f);
            _innerAction->_finish();
            _innerAction->_stop();
            ++_stopCnt;
        }
        
        if (_stopCnt < _times) {
            _innerAction->startWithTarget(_target);
            _innerAction->_progressUpdate(level - _dt);
            
            ++_startCnt;
            _dt += _innerAction->duration() / _duration;
        }
    } else {
        _innerAction->_progressUpdate(fmodf(level * _times, 1.0f));
    }
}

// =============================================================================================================

CL_CLASS_DEF(iARotateBy, IM)

iARotateBy::iARotateBy() {
}

iARotateBy*
iARotateBy::initWithQuat(dFloat duration, const quaternionf& q) {
    _super::initWithDuration(duration);
    _initRotation = q;
    return this;
}

iARotateBy*
iARotateBy::actionWithQuat(dFloat duration, const quaternionf& q) {
    _self* im = alloc()->initWithQuat(duration, q);
    return im ? im->autorelease() : nil;
}

iARotateBy*
iARotateBy::copy() const {
    _self* res = (iARotateBy*)_super::copy();
    res->_initRotation = _initRotation;
    return res;
}

iARotateBy*
iARotateBy::reverse() const {
    return actionWithQuat(_duration, _initRotation.inverse());
}

void
iARotateBy::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    _beginRotation = target->transform()->localRotation();
    _endRotation = _initRotation * _beginRotation;
}

void
iARotateBy::_progressUpdate(dFloat level) {
    CL_THROW_IF_NIL(_target);
    
    _target->transform()->setLocalRotation(quaternionf::slerp_no_invert(level, _beginRotation, _endRotation));
}

// =============================================================================================================

CL_CLASS_DEF(iARotateTo, IM)

iARotateTo::iARotateTo() {
}

iARotateTo*
iARotateTo::initWithQuat(dFloat duration, const quaternionf& q) {
    _super::initWithDuration(duration);
    _endRotation = q;
    return this;
}

iARotateTo*
iARotateTo::actionWithQuat(dFloat duration, const quaternionf& q) {
    _self* im = alloc()->initWithQuat(duration, q);
    return im ? im->autorelease() : nil;
}

iARotateTo*
iARotateTo::copy() const {
    _self* res = (iARotateTo*)_super::copy();
    res->_endRotation = _endRotation;
    return res;
}

iARotateTo*
iARotateTo::reverse() const {
    return actionWithQuat(_duration, _endRotation.inverse());
}

void
iARotateTo::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    _beginRotation = target->transform()->localRotation();
}

void
iARotateTo::_progressUpdate(dFloat level) {
    CL_THROW_IF_NIL(_target);
    
    _target->transform()->setLocalRotation(quaternionf::slerp_no_invert(level, _beginRotation, _endRotation));
}

// =============================================================================================================

CL_CLASS_DEF(iAMoveTo, IM)

iAMoveTo::iAMoveTo() {
}

iAMoveTo*
iAMoveTo::initWithPos(dFloat duration, const vec3f& pos) {
    _super::initWithDuration(duration);
    _endPos = pos;
    return this;
}

iAMoveTo*
iAMoveTo::actionWithPos(dFloat duration, const vec3f& pos) {
    _self* im = alloc()->initWithPos(duration, pos);
    return im ? im->autorelease() : nil;
}

iAMoveTo*
iAMoveTo::copy() const {
    _self* res = (iAMoveTo*)_super::copy();
    res->_endPos = _endPos;
    return res;
}

iAMoveTo*
iAMoveTo::reverse() const {
    return actionWithPos(_duration, -_endPos);
}

void
iAMoveTo::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    _beginPos = target->transform()->localPosition();
    _delta = _endPos - _beginPos;
}

void
iAMoveTo::_progressUpdate(dFloat level) {
    CL_THROW_IF_NIL(_target);
    
    _target->transform()->setLocalPosition(_beginPos + _delta * level);
}

// =============================================================================================================

CL_CLASS_DEF(iAMoveBy, IM)

iAMoveBy::iAMoveBy() {
}

iAMoveBy*
iAMoveBy::initWithPos(dFloat duration, const vec3f& pos) {
    _super::initWithPos(duration, pos);
    return this;
}

iAMoveBy*
iAMoveBy::actionWithPos(dFloat duration, const vec3f& pos) {
    _self* im = alloc()->initWithPos(duration, pos);
    return im ? im->autorelease() : nil;
}

iAMoveBy*
iAMoveBy::reverse() const {
    return actionWithPos(_duration, -_endPos);
}

void
iAMoveBy::startWithTarget(iANode* target) {
    _super::startWithTarget(target);
    
    _beginPos = target->transform()->localPosition();
    _delta = _endPos;
}

// =============================================================================================================

CL_CLASS_DEF(iADelayTime, IM)

iADelayTime::iADelayTime() {
}

iADelayTime*
iADelayTime::actionWithDuration(dFloat duration) {
    _self* im = (iADelayTime*)alloc()->initWithDuration(duration);
    return im ? im->autorelease() : nil;
}

iADelayTime*
iADelayTime::reverse() const {
    return actionWithDuration(_duration);
}