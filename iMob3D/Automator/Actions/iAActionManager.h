/*
 ** $Id: 2011/10/27, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAActionManager_H
#define _iAActionManager_H

class _ActionEntry;

BEGIN_NAMESPACE_IM

class iAAction;
class iANode;

class iAActionManager : public dObject {
CL_CLASS_DECL_AS_SINGLETON(iAActionManager, dObject)
friend class iATime; // _tick
public:
    void                                addAction(iAAction* action, iANode* target, bool paused = false);
    void                                remove(iAAction* action);
    void                                removeAllActionsForTarget(iANode* target);
    void                                removeAllActions();
    
    iAAction*                           actionByTag(dInteger tag, iANode* target);
    void                                removeActionByTag(dInteger tag, iANode* target);
    
    dUInteger                           numberOfRunningActionsForTarget(iANode* target);
    
    void                                pauseTarget(iANode* target);
    void                                resumeTarget(iANode* target);
    bool                                isTargetPaused(iANode* target);
    
protected:
    iAActionManager();
    virtual void                        dealloc();
    
    void                                _tick(dFloat dt);
    dDictT<_ActionEntry*, iANode*>*     _actions();
    
    void                                _removeActionAtIndex(dInteger idx, _ActionEntry* entry);
    
    dDictT<_ActionEntry*, iANode*>*     _targetActions;
    _ActionEntry*                       _currEnt;
    bool                                _currEntSalvaged;
};

END_NAMESPACE_IM

#endif // _iAActionManager_H
