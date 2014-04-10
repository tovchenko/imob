/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iMesh_H
#define _iMesh_H


BEGIN_NAMESPACE_IM

class iVertexBuffer;
class iMaterial;
class iMeshPart;
class iContext;

class iMesh : public dObject {
CL_CLASS_DECL(iMesh, dObject)
public:
	static iMesh*					meshWithVertexBuffer(iVertexBuffer* vertexBuffer);
	static iMesh*					meshWithVertexBufferAndPart(iVertexBuffer* vertexBuffer, iMeshPart* part, dString* name);
	static iMesh*					meshWithVertexBufferAndParts(iVertexBuffer* vertexBuffer, dDictT<iMeshPart*, dString*>* meshParts);
	
	virtual iMesh*					initWithVertexBuffer(iVertexBuffer* vertexBuffer);
	virtual iMesh*					initWithVertexBufferAndPart(iVertexBuffer* vertexBuffer, iMeshPart* part, dString* name);
	virtual iMesh*					initWithVertexBufferAndParts(iVertexBuffer* vertexBuffer, dDictT<iMeshPart*, dString*>* meshParts);
	
	void							setMaterial(iMaterial* material);
	iMaterial*						material() const;
	
	dDictT<iMeshPart*, dString*>*	parts();
	
	virtual void					renderInContext(iContext* context);
	virtual void					renderPartByNameInContext(dString* name, iContext* context);
	
protected:
	iMesh();
	virtual void					dealloc();
	
	iVertexBuffer*					_vertexBuffer;
	iMaterial*						_defaultMaterial;
	dDictT<iMeshPart*, dString*>*	_parts;
};

END_NAMESPACE_IM

#endif // _iMesh_H