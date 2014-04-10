/*
 ** $Id: 2011/01/29, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iANode_H
#define _iANode_H


BEGIN_NAMESPACE_IM

class iAComponent;
class iATransform;
class iAAction;

enum {
	kiANodeDrawableComponent,
	kiANodeGUIComponent,
	kiANodeCameraComponent,
	kiANodeTouchComponent,
	// -------------------------
	kiANodeComponentsEndMarker
};
typedef dUInteger	iAENodeComponentID;

class iANode : public dObject,
			   public CTreeNodeImplT<
						dArrayT<iAComponent*>*,
						dArrayT<iAComponent*>*,
						CDataOwnershipCocoaPolicyT,
						CNodeOwnershipCocoaPolicyT,
						iANode>
{
CL_CLASS_DECL(iANode, dObject)
public:
	typedef	void (*iANodeCB)(iANode* node, iAENodeComponentID cmp, dDict* params);
	// create
	static _self*		node();
	// for remove component - simply pass nil to 'component' param
	void				setComponent(iAENodeComponentID  componentID, iAComponent* component);
	iAComponent*		component(iAENodeComponentID  componentID); // use macros imCOMPONENT
    // transform
	void				setTransform(iATransform* trnsf);
	iATransform*		transform();
    // event scheduling
    void                schedule(dSEL sel,
                                 double interval,
                                 dUInteger repeatsCount,
                                 double delay = 0.0,
                                 bool paused = false);
    
    void                unschedule(dSEL sel);
    // actions
    iAAction*           runAction(iAAction* action);
	// override
	virtual void		update(iAENodeComponentID componentID);
	virtual void		postUpdate(iAENodeComponentID componentID);
	// this method created for calling from agents,
	// it enumerate all nodes and call 'udtate', 'process' and 'postUpdate'
	// for given component in each node.
	void				visitComponents(iAENodeComponentID componentID, dDict* params);
	void				visit(iANodeCB callback, iAENodeComponentID cmp, dDict* params);

protected:
	iANode();
	virtual void		dealloc();
    
    virtual void        _didDetach();
							
	iATransform*		_transform;
};

// select type for component

#define imCOMPONENT(_node_, _cmp_) ((TypeSelect<_cmp_>::Result)_node_->component(_cmp_))

template <int v>
struct TypeSelect {
    typedef dInteger Result;
};
class iADrawableComponent;
template < >
struct TypeSelect <kiANodeDrawableComponent> {
    typedef iADrawableComponent* Result;
};
class iAGUIComponent;
template < >
struct TypeSelect <kiANodeGUIComponent> {
    typedef iAGUIComponent* Result;
};
class iACameraComponent;
template < >
struct TypeSelect <kiANodeCameraComponent> {
    typedef iACameraComponent* Result;
};
class iATouchComponent;
template < >
struct TypeSelect <kiANodeTouchComponent> {
    typedef iATouchComponent* Result;
};

END_NAMESPACE_IM

#endif // _iANode_H