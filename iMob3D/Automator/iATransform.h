/*
 ** $Id: 2011/02/10, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iATransform_H
#define _iATransform_H


BEGIN_NAMESPACE_IM

class iANode;

class iATransform : public dObject {
CL_CLASS_DECL(iATransform, dObject)
friend class iANode; // _ownerNode
public:
	vec3f				position(); // global position, read-only
	const vec3f&		localPosition() const;
	void				setLocalPosition(const vec3f& pos);
	quaternionf			rotation(); // global rotation, read-only
	const quaternionf&	localRotation() const;
	void				setLocalRotation(const quaternionf& rot);
	
	const mat4f*		resultMatrix();
	void				invalidateCache();
	void				invalidateCacheHierarchical();
	// used by camera object
	void				setAxesInverted(bool inverted);
	bool				isAxesInverted() const;
	void				setParentTransformUsage(bool isUse);
	bool				isParentTransformUse() const;
	
protected:
	iATransform();
	
	void				_calcLocalMatrixIfNeeded();
	void				_calcGlobalMatrixIfNeeded();
	
	iANode*				_ownerNode;
	
	vec3f				_position;
	quaternionf			_rotation;
	
	bool				_isValidLocalCache;
	bool				_isValidGlobalCache;
	mat4f				_cachedLocalMtx;
	mat4f				_cachedGlobalMtx;
	bool				_isAxesInverted;
	bool				_isParentTransformUse;
};

END_NAMESPACE_IM

#endif // _iATransform_H