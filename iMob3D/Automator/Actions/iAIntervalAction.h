/*
 ** $Id: 2011/10/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAIntervalActionl_H
#define _iAIntervalActionl_H

#include "iAAction.h"


BEGIN_NAMESPACE_IM

class iAIntervalAction : public iAFiniteTimeAction {
CL_ABSTRACT_DECL(iAIntervalAction, iAFiniteTimeAction)
friend class iARepeatForever; // _update, _stop
friend class iASpeed; // _update, _stop, _finish
public:
    virtual _self*      initWithDuration(dFloat duration);
    virtual _self*      copy() const;
    
    dFloat              elapsed() const;
    
    virtual _self*      reverse() const = 0;
    
    virtual void        startWithTarget(iANode* target);
    virtual bool        isDone() const;
    
protected:
    iAIntervalAction();
    
    virtual void        _update(dFloat dt);
    
    dFloat              _elapsed;
    bool                _isFirstTick;
};


class iASequence : public iAIntervalAction {
CL_CLASS_DECL(iASequence, iAIntervalAction)
public:
    static _self*       actionWithArray(dArrayT<iAFiniteTimeAction*>* actions);
    static _self*       actionWith2(iAFiniteTimeAction* one, iAFiniteTimeAction* two);
    static _self*       actionWithActions(iAFiniteTimeAction* first, ...);
    virtual _self*      initWithArray(dArrayT<iAFiniteTimeAction*>* actions);
    virtual _self*      initWith2(iAFiniteTimeAction* one, iAFiniteTimeAction* two);
    virtual _self*      initWithActions(iAFiniteTimeAction* first, ...);
    virtual _self*      copy() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    
protected:
    iASequence();
    virtual void        dealloc();
    virtual _self*      _initWithVAList(iAFiniteTimeAction* first, va_list args);
    
    virtual void        _stop();
    virtual void        _progressUpdate(dFloat level);
    
    iAFiniteTimeAction* _actions[2];
    dFloat              _split;
    dInteger            _last;
};


class iARepeat : public iAIntervalAction {
CL_CLASS_DECL(iARepeat, iAIntervalAction)
public:
    static _self*       actionWithAction(iAFiniteTimeAction* action, dUInteger times);
    virtual _self*      initWithAction(iAFiniteTimeAction* action, dUInteger times);
    virtual _self*      copy() const;
    
    void                setInnerAction(iAFiniteTimeAction* action);
    iAFiniteTimeAction* innerAction() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    virtual bool        isDone() const;
    
protected:
    iARepeat();
    virtual void        dealloc();
    
    virtual void        _stop();
    virtual void        _progressUpdate(dFloat level);
    
    dUInteger           _times;
    dUInteger           _startCnt;
    dUInteger           _stopCnt;
    dFloat              _dt;
    iAFiniteTimeAction* _innerAction;
};


class iARotateBy : public iAIntervalAction {
CL_CLASS_DECL(iARotateBy, iAIntervalAction)
public:
    static _self*       actionWithQuat(dFloat duration, const quaternionf& q);
    virtual _self*      initWithQuat(dFloat duration, const quaternionf& q);
    virtual _self*      copy() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    
protected:
    iARotateBy();
    
    virtual void        _progressUpdate(dFloat level);
    
    quaternionf         _initRotation; // need for copy() and reverse()
    quaternionf         _beginRotation;
    quaternionf         _endRotation;
};


class iARotateTo : public iAIntervalAction {
CL_CLASS_DECL(iARotateTo, iAIntervalAction)
public:
    static _self*       actionWithQuat(dFloat duration, const quaternionf& q);
    virtual _self*      initWithQuat(dFloat duration, const quaternionf& q);
    virtual _self*      copy() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    
protected:
    iARotateTo();
    
    virtual void        _progressUpdate(dFloat level);
    
    quaternionf         _beginRotation;
    quaternionf         _endRotation;
};


class iAMoveTo : public iAIntervalAction {
CL_CLASS_DECL(iAMoveTo, iAIntervalAction)
public:
    static _self*       actionWithPos(dFloat duration, const vec3f& pos);
    virtual _self*      initWithPos(dFloat duration, const vec3f& pos);
    virtual _self*      copy() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    
protected:
    iAMoveTo();
    
    virtual void        _progressUpdate(dFloat level);
    
    vec3f               _beginPos;
    vec3f               _endPos;
    vec3f               _delta;
};


class iAMoveBy : public iAMoveTo {
CL_CLASS_DECL(iAMoveBy, iAMoveTo)
public:
    static _self*       actionWithPos(dFloat duration, const vec3f& pos);
    virtual _self*      initWithPos(dFloat duration, const vec3f& pos);
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    
protected:
    iAMoveBy();
};


class iADelayTime : public iAIntervalAction {
CL_CLASS_DECL(iADelayTime, iAIntervalAction)
public:
    static _self*       actionWithDuration(dFloat duration);
    virtual _self*      reverse() const;
    
protected:
    iADelayTime();
};

END_NAMESPACE_IM

#endif // _iAIntervalActionl_H
