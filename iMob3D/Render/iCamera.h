/*
 ** $Id: 2010/07/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iCamera_H
#define _iCamera_H


BEGIN_NAMESPACE_IM

class iContext;

class iCamera : public dObject {
CL_CLASS_DECL(iCamera, dObject)
public:
	void				setFOV(dFloat fofv);
	dFloat				fov() const;
	void				setNearPlane(dFloat near);
	dFloat				nearPlane() const;
	void				setFarPlane(dFloat far);
	dFloat				farPlane() const;
	
	void				setPosition(const vec3f& pos);
	const vec3f&		position() const;
	void				setUpVector(const vec3f& up);
	const vec3f&		upVector() const;
	void				setLookAt(const vec3f& at);
	const vec3f&		lookAt() const;
	
	virtual void		applyCameraInContext(iContext* context);
	virtual void		applyProjectionMatrixInContext(iContext* context);
	virtual void		applyViewMatrixInContext(iContext* context);
	
protected:
	iCamera();
	
	dFloat				_zNear;
	dFloat				_zFar;
	dFloat				_fov;
	
	vec3f				_position;
	vec3f				_upVector;
	vec3f				_lookAt;
	
	mat4f				_cachedProjMtx;
	mat4f				_cachedViewMtx;
	bool				_isValidProjMtx;
	bool				_isValidViewMtx;
};

END_NAMESPACE_IM

#endif // _iCamera_H