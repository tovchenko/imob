/*
 ** $Id: 2011/10/27, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAActionManager.h"
#include "iAAction.h"
#include "iANode.h"


class _ActionEntry : public dObject {
CL_CLASS_DECL(_ActionEntry, dObject)
public:
    dArrayT<iAAction*>*         actions();
    dUInteger                   actionIndex;
    iAAction*                   currentAction;
    bool                        currentActionSalvaged;
    bool                        paused;
    
protected:
    _ActionEntry();
    virtual void                dealloc();
    dArrayT<iAAction*>*         _actions;
};

CL_CLASS_DEF(_ActionEntry, IM)

_ActionEntry::_ActionEntry()
: _actions(nil),
  actionIndex(0),
  currentAction(nil),
  currentActionSalvaged(false),
  paused(false) {
}

void
_ActionEntry::dealloc() {
    CL_RELEASE(_actions);
    
    _super::dealloc();
}

dArrayT<iAAction*>*
_ActionEntry::actions() {
    if (!_actions) _actions = CL_NEW(dArrayT<iAAction*>);
    return _actions;
}

// ===========================================================

CL_CLASS_DEF_AS_SINGLETON(iAActionManager, IM)

iAActionManager::iAActionManager()
: _targetActions(nil) {
}

void
iAActionManager::dealloc() {
    CL_RELEASE(_targetActions);
    
    _super::dealloc();
}

void
iAActionManager::addAction(iAAction* action, iANode* target, bool paused) {
    CL_THROW_IF_NIL(action);
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (!entry) {
        dobj_ptr<_ActionEntry> ptrEntry(CL_NEW(_ActionEntry));
        entry = ptrEntry.get();
        _actions()->setObjectForKey(entry, target);
        entry->paused = paused;
    }
    
    entry->actions()->addObject(action);
    action->startWithTarget(target);
}

void
iAActionManager::remove(iAAction* action) {
    if (!action) return;
    iANode* target = action->target();
    if (!target) return;
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    CL_THROW_IF_NIL(entry);
    if (!entry) return;
    
    dInteger idx = entry->actions()->indexOfObject(action);
    if (kNotFound != idx)
        _removeActionAtIndex(idx, entry);
}

void
iAActionManager::removeAllActionsForTarget(iANode* target) {
    if (!target) return;
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (!entry) return;
    
    if (entry->actions()->containsObject(entry->currentAction) && !entry->currentActionSalvaged) {
        CL_RETAIN(entry->currentAction);
        entry->currentActionSalvaged = true;
    }
    
    for (dUInteger i = 0, cnt = entry->actions()->count(); i < cnt; ++i) {
        iAAction* a = entry->actions()->objectAtIndex(i);
        
        if (entry->currentActionSalvaged) {
            if (entry->currentAction != a)
                a->_stop();
        } else {
            a->_stop();
        }
    }
    
    entry->actions()->removeAllObjects();
    
    if (_currEnt == entry) _currEntSalvaged = true;
    else _actions()->removeObjectForKey(target);
}

void
iAActionManager::removeAllActions() {
    dDictT<_ActionEntry*, iANode*>::Iterator it(_actions());
    while (it.next())
        removeAllActionsForTarget(it.key());
}

iAAction*
iAActionManager::actionByTag(dInteger tag, iANode* target) {
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (entry) {
        for (dUInteger i = 0, cnt = entry->actions()->count(); i < cnt; ++i) {
            iAAction* a = entry->actions()->objectAtIndex(i);
            if (a->tag() == tag)
                return a;
        }
    }
    
    return nil;
}

void
iAActionManager::removeActionByTag(dInteger tag, iANode* target) {
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (entry) {
        for (dUInteger i = 0, cnt = entry->actions()->count(); i < cnt; ++i) {
            iAAction* a = entry->actions()->objectAtIndex(i);
            if (a->tag() == tag) {
                _removeActionAtIndex(i, entry);
                break;
            }
        }
    }
}

dUInteger
iAActionManager::numberOfRunningActionsForTarget(iANode* target) {
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (entry)
        return entry->actions()->count();
    return 0;
}

void
iAActionManager::pauseTarget(iANode* target) {
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (entry) entry->paused = true;
}

void
iAActionManager::resumeTarget(iANode* target) {
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (entry) entry->paused = false;
}

bool
iAActionManager::isTargetPaused(iANode* target) {
    CL_THROW_IF_NIL(target);
    
    _ActionEntry* entry = _actions()->objectForKeyNoException(target);
    if (entry) return entry->paused;
    return false;
}

void
iAActionManager::_tick(dFloat dt) {
    dDictT<_ActionEntry*, iANode*>::Iterator it(_actions());
    while (it.next()) {
        _currEnt = it.object();
        _currEntSalvaged = false;
        
        if (!_currEnt->paused) {
            for (_currEnt->actionIndex = 0; _currEnt->actionIndex < _currEnt->actions()->count(); ++_currEnt->actionIndex) {
                _currEnt->currentAction = _currEnt->actions()->objectAtIndex(_currEnt->actionIndex);
                _currEnt->currentActionSalvaged = false;
                
                _currEnt->currentAction->_update(dt);
                
                if (_currEnt->currentActionSalvaged) {
                    _currEnt->currentAction->_stop();
                    _currEnt->currentAction->release();
                } else if (_currEnt->currentAction->isDone()) {
                    iAAction* a = _currEnt->currentAction;
                    a->_finish();
                    // Make currentAction nil to prevent remove() from salvaging it.
                    _currEnt->currentAction = nil;
                    remove(a);
                }
                
                _currEnt->currentAction = nil;
            }
        }
        
        if (_currEntSalvaged && 0 == _currEnt->actions()->count())
            _actions()->removeObjectForKey(it.key());
    }
    
    _currEnt = nil;
}

dDictT<_ActionEntry*, iANode*>*
iAActionManager::_actions() {
    if (!_targetActions)
        _targetActions = (dDictT<_ActionEntry*, iANode*>*)dDictT<_ActionEntry*, iANode*>::alloc()->init();
    return _targetActions;
}

void
iAActionManager::_removeActionAtIndex(dInteger idx, _ActionEntry* entry) {
    iAAction* const action = entry->actions()->objectAtIndex(idx);
    iANode* const target = action->target();
    
    if (action == entry->currentAction && !entry->currentActionSalvaged) {
        CL_RETAIN(entry->currentAction);
        entry->currentActionSalvaged = true;
    } else {
        action->_stop();
    }
    
    entry->actions()->removeObjectAtIndex(idx);
    
    if (entry->actionIndex >= idx)
        --entry->actionIndex;
    
    if (0 == entry->actions()->count()) {
        if (_currEnt == entry) _currEntSalvaged = true;
        else _actions()->removeObjectForKey(target);
    }
}
