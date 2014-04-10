/*
 ** $Id: 2011/10/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAAction.h"
#include "iAIntervalAction.h"
#include "iAActionDelegate.h"

CL_ABSTRACT_DEF(iAAction, IM)

iAAction::iAAction()
: _target(nil),
  _tag(-1),
  _delegate(nil) {
}

void
iAAction::dealloc() {
    CL_RELEASE(_target);
    
    _super::dealloc();
}

iAAction*
iAAction::copy() const {
    _self* res = dobj_cast<_self>(runtimeClass()->alloc()->init());
    res->setTag(_tag);
    return res;
}

iANode*
iAAction::target() const {
    return _target;
}

void
iAAction::setTag(dInteger tag) {
    _tag = tag;
}

dInteger
iAAction::tag() const {
    return _tag;
}

void
iAAction::setDelegate(iAActionDelegate* delegate) {
    _delegate = delegate;
}

iAActionDelegate*
iAAction::delegate() const {
    return _delegate;
}

void
iAAction::startWithTarget(iANode* target) {
    CL_SWITCH_LINKS(_target, target);
}

bool
iAAction::isDone() const {
    return true;
}

void
iAAction::_stop() {
    if (_delegate) _delegate->actionDidStop(this);
    CL_RELEASE(_target);
}

void
iAAction::_finish() {
    if (_delegate) _delegate->actionDidFinish(this);
}

void
iAAction::_update(dFloat dt) {
}

void
iAAction::_progressUpdate(dFloat level) {
}

// =======================================================================

CL_ABSTRACT_DEF(iAFiniteTimeAction, IM)

iAFiniteTimeAction::iAFiniteTimeAction()
: _duration(0.0f) {
}

iAFiniteTimeAction*
iAFiniteTimeAction::copy() const {
    return (iAFiniteTimeAction*)_super::copy();
}

void
iAFiniteTimeAction::setDuration(dFloat duration) {
    _duration = (0.0f == duration) ? std::numeric_limits<dFloat>::epsilon() : duration;
}

dFloat
iAFiniteTimeAction::duration() const {
    return _duration;
}

// =======================================================================

CL_CLASS_DEF(iARepeatForever, IM)

iARepeatForever::iARepeatForever()
: _innerAction(nil) {
}

iARepeatForever*
iARepeatForever::actionWithAction(iAIntervalAction* action) {
    _self* im = alloc()->initWithAction(action);
    return im ? im->autorelease() : nil;
}

iARepeatForever*
iARepeatForever::initWithAction(iAIntervalAction* action) {
    _super::init();
    setInnerAction(action);
    return this;
}

void
iARepeatForever::dealloc() {
    CL_RELEASE(_innerAction);
    
    _super::dealloc();
}

iARepeatForever*
iARepeatForever::copy() const {
    _self* res = (iARepeatForever*)_super::copy();
    res->_innerAction = _innerAction ? (iAIntervalAction*)_innerAction->copy() : nil;
    
    return res;
}

void
iARepeatForever::setInnerAction(iAIntervalAction* action) {
    CL_SWITCH_LINKS(_innerAction, action);
}

iAIntervalAction*
iARepeatForever::innerAction() const {
    return _innerAction;
}

iARepeatForever*
iARepeatForever::reverse() const {
    return _innerAction ? actionWithAction(_innerAction->reverse()) : nil;
}

void
iARepeatForever::startWithTarget(iANode* target) {
    if (!_innerAction) return;
    _super::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

bool
iARepeatForever::isDone() const {
    return false;
}

void
iARepeatForever::_stop() {
    if (_innerAction) _innerAction->_stop();
    _super::_stop();
}

void
iARepeatForever::_update(dFloat dt) {
    if (!_innerAction) return;
    
    _innerAction->_update(dt);
    if (_innerAction->isDone()) {
        dFloat diff = _innerAction->_elapsed - _innerAction->_duration;
        _innerAction->startWithTarget(_target);
        
        _innerAction->_update(0.0f);
        _innerAction->_update(diff);
    }
}

// =======================================================================

CL_CLASS_DEF(iASpeed, IM)

iASpeed::iASpeed()
: _innerAction(nil),
  _speed(1.0f) {
}

iASpeed*
iASpeed::actionWithAction(iAIntervalAction* action, dFloat speed) {
    _self* im = alloc()->initWithAction(action, speed);
    return im ? im->autorelease() : nil;
}

iASpeed*
iASpeed::initWithAction(iAIntervalAction* action, dFloat speed) {
    _super::init();
    setInnerAction(action);
    _speed = speed;
    return this;
}

iASpeed*
iASpeed::copy() const {
    _self* res = (iASpeed*)_super::copy();
    res->_speed = _speed;
    res->_innerAction = _innerAction ? _innerAction->copy() : nil;
    
    return res;
}

void
iASpeed::dealloc() {
    CL_RELEASE(_innerAction);
    _super::dealloc();
}

void
iASpeed::setInnerAction(iAIntervalAction* action) {
    CL_SWITCH_LINKS(_innerAction, action);
}

iAIntervalAction*
iASpeed::innerAction() const {
    return _innerAction;
}

iASpeed*
iASpeed::reverse() const {
    if (!_innerAction) return nil;
    return actionWithAction(_innerAction->reverse(), _speed);
}

void
iASpeed::startWithTarget(iANode* target) {
    if (!_innerAction) return;
    
    _super::startWithTarget(target);
    _innerAction->startWithTarget(target);
}

bool
iASpeed::isDone() const {
    if (!_innerAction) return true;
    return _innerAction->isDone();
}

void
iASpeed::_stop() {
    if (_innerAction) _innerAction->_stop();
    _super::_stop();
}

void
iASpeed::_finish() {
    if (_innerAction) _innerAction->_finish();
    _super::_finish();
}

void
iASpeed::_update(dFloat dt) {
    if (_innerAction)
        _innerAction->_update(_speed * dt);
}
