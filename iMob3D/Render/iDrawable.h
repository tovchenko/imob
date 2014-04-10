/*
 ** $Id: 2010/05/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iDrawable_H
#define	_iDrawable_H


BEGIN_NAMESPACE_IM

class iMesh;
class iContext;
class iMaterial;

class iDrawable : public dObject {
CL_CLASS_DECL(iDrawable, dObject)
public:
	static iDrawable*	drawableWithMesh(iMesh* mesh, iMaterial* material = nil);
	virtual iDrawable*	initWithMesh(iMesh* mesh, iMaterial* material = nil);
	
	void				setMesh(iMesh* mesh);
	iMesh*				mesh() const;
	void				setMatrix(const mat4f* mtx);
	const mat4f*		matrix() const;
	void				setMaterial(iMaterial* material);
	iMaterial*			material() const;
	
	virtual void		renderInContext(iContext* context);
	virtual void		renderMeshPartByNameInContext(dString* name, iContext* context);
	
protected:
	iDrawable();
	virtual void		dealloc();
	
	iMesh*				_mesh;
	const mat4f*		_matrix;
	iMaterial*			_material;
};

END_NAMESPACE_IM

#endif // _iDrawable_H