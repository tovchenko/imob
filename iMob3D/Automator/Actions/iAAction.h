/*
 ** $Id: 2011/10/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAAction_H
#define _iAAction_H

BEGIN_NAMESPACE_IM

class iANode;
class iAIntervalAction;
class iAActionDelegate;

class iAAction : public dObject {
CL_ABSTRACT_DECL(iAAction, dObject)
friend class iAActionManager; // _update, _stop, _finish
public:
    virtual _self*      copy() const;
    iANode*             target() const;
    
    void                setDelegate(iAActionDelegate* delegate);
    iAActionDelegate*   delegate() const;
    
    void                setTag(dInteger tag);
    dInteger            tag() const;
    
    virtual void        startWithTarget(iANode* target);
    virtual bool        isDone() const;
    
protected:
    iAAction();
    virtual void        dealloc();
    
    virtual void        _stop();
    virtual void        _finish();
    
    virtual void        _update(dFloat dt);
    virtual void        _progressUpdate(dFloat level);
    
    iANode*             _target;
    dInteger            _tag;
    iAActionDelegate*   _delegate;
};


class iAFiniteTimeAction : public iAAction {
CL_ABSTRACT_DECL(iAFiniteTimeAction, iAAction)
public:
    virtual _self*      copy() const;
    
    void                setDuration(dFloat duration);
    dFloat              duration() const;
    
    virtual _self*      reverse() const = 0;
    
protected:
    iAFiniteTimeAction();
    
    dFloat              _duration;
};


class iARepeatForever : public iAAction {
CL_CLASS_DECL(iARepeatForever, iAAction)
public:
    static _self*       actionWithAction(iAIntervalAction* action);
    virtual _self*      initWithAction(iAIntervalAction* action);
    virtual _self*      copy() const;
    
    void                setInnerAction(iAIntervalAction* action);
    iAIntervalAction*   innerAction() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    virtual bool        isDone() const;
    
protected:
    iARepeatForever();
    virtual void        dealloc();
    
    virtual void        _stop();
    
    virtual void        _update(dFloat dt);
    
    iAIntervalAction*   _innerAction;
};


class iASpeed : public iAAction {
CL_CLASS_DECL(iASpeed, iAAction)
public:
    static _self*       actionWithAction(iAIntervalAction* action, dFloat speed);
    virtual _self*      initWithAction(iAIntervalAction* action, dFloat speed);
    virtual _self*      copy() const;
    
    void                setInnerAction(iAIntervalAction* action);
    iAIntervalAction*   innerAction() const;
    
    virtual _self*      reverse() const;
    
    virtual void        startWithTarget(iANode* target);
    virtual bool        isDone() const;
    
protected:
    iASpeed();
    virtual void        dealloc();
    
    virtual void        _stop();
    virtual void        _finish();
    
    virtual void        _update(dFloat dt);
    
    iAIntervalAction*   _innerAction;
    dFloat              _speed;
};

END_NAMESPACE_IM

#endif // _iAAction_H
