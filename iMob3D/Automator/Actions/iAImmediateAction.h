/*
 ** $Id: 2011/10/25, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAImmediateAction_H
#define _iAImmediateAction_H

#include "iAAction.h"


BEGIN_NAMESPACE_IM

class iAImmediateAction : public iAFiniteTimeAction {
CL_ABSTRACT_DECL(iAImmediateAction, iAFiniteTimeAction)
public:
    virtual _self*      reverse() const = 0;
};


class iAShow : public iAImmediateAction {
CL_CLASS_DECL(iAShow, iAImmediateAction)
public:
    static _self*               action();
    
    virtual iAImmediateAction*  reverse() const;
    virtual void                startWithTarget(iANode* target);
    
protected:
    iAShow();
};


class iAHide : public iAImmediateAction {
CL_CLASS_DECL(iAHide, iAImmediateAction)
public:
    static _self*               action();
    
    virtual iAImmediateAction*  reverse() const;
    virtual void                startWithTarget(iANode* target);
    
protected:
    iAHide();
};


class iAToggleVisibility : public iAImmediateAction {
CL_CLASS_DECL(iAToggleVisibility, iAImmediateAction)
public:
    static _self*               action();
    
    virtual _self*              reverse() const;
    virtual void                startWithTarget(iANode* target);
    
protected:
    iAToggleVisibility();
};

END_NAMESPACE_IM

#endif // _iAIntervalActionl_H
