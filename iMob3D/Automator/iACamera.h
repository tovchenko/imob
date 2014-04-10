/*
 ** $Id: 2011/02/15, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iACamera_H
#define _iACamera_H

#include "iANode.h"
#include "iACameraComponentDelegate.h"

BEGIN_NAMESPACE_IM

class iCamera;
class iASkybox;

class iACamera : public iANode,
				 public iACameraComponentDelegate
{
CL_CLASS_DECL(iACamera, iANode)
public:
	virtual _self*		init();
	// iACameraComponentDelegate
	virtual void		applyCameraInContext(iACameraComponent* component, iContext* context);
	// camera parameters
	void				setFOV(dFloat fofv);
	dFloat				fov() const;
	void				setNearPlane(dFloat near);
	dFloat				nearPlane() const;
	void				setFarPlane(dFloat far);
	dFloat				farPlane() const;
	
	void				setLookAt(const vec3f& at, const vec3f& up = vec3f::UP);
	// skybox
	void				setSkybox(iASkybox* box);
	iASkybox*			skybox() const;
	// overrided methods
	virtual void		update(iAENodeComponentID componentID);
	
protected:
	iACamera();
	virtual void		dealloc();
		
	iCamera*			_projection;
	iASkybox*			_skybox;
};

END_NAMESPACE_IM

#endif // _iACamera_H