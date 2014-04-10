/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iMeshPart_H
#define _iMeshPart_H


BEGIN_NAMESPACE_IM

class iIndexBuffer;
class iMaterial;
class iContext;

class iMeshPart : public dObject {
CL_CLASS_DECL(iMeshPart, dObject)
public:
	static iMeshPart*	meshPartWithParams(iIndexBuffer* indexBuffer, iMaterial* material, iEDrawMode drawMode, dInteger first, dUInteger count);
	static iMeshPart*	meshPartWithParams(iIndexBuffer* indexBuffer, iEDrawMode drawMode, dInteger first, dUInteger count);
	
	virtual iMeshPart*	initWithParams(iIndexBuffer* indexBuffer, iMaterial* material, iEDrawMode drawMode, dInteger first, dUInteger count);
	virtual iMeshPart*	initWithParams(iIndexBuffer* indexBuffer, iEDrawMode drawMode, dInteger first, dUInteger count);
	
	void				setMaterial(iMaterial* material);
	iMaterial*			material() const;
	
	void				setHidden(bool isHidden);
	bool				hidden() const;
	
	virtual void		renderInContext(iContext* context, iMaterial* defaultMeterial);
	
protected:
	iMeshPart();
	virtual void		dealloc();
	
	iIndexBuffer*		_indexBuffer;
	iMaterial*			_material;
	
	iEDrawMode			_drawMode;
	dInteger			_first;
	dUInteger			_count;
	
	bool				_isHidden;
};

END_NAMESPACE_IM

#endif // _iMeshPart_H