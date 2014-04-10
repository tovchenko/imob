/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iMesh.h"
#include "iContext.h"
#include "iVertexBuffer.h"
#include "iMeshPart.h"

CL_CLASS_DEF(iMesh, IM)

iMesh::iMesh()
: _vertexBuffer(nil),
  _defaultMaterial(nil),
  _parts(nil) {
}

iMesh*
iMesh::initWithVertexBuffer(iVertexBuffer* vertexBuffer) {
	CL_THROW_IF_NIL(_vertexBuffer);
	
	_super::init();
	_vertexBuffer = vertexBuffer->retain();
	return this;
}

iMesh*
iMesh::initWithVertexBufferAndPart(iVertexBuffer* vertexBuffer, iMeshPart* part, dString* name) {
	CL_THROW_IF_TRUE(!vertexBuffer || !part);
	
	_super::init();
	
	_vertexBuffer = vertexBuffer->retain();
	parts()->setObjectForKey(part, name);
	
	return this;
}

iMesh*
iMesh::initWithVertexBufferAndParts(iVertexBuffer* vertexBuffer, dDictT<iMeshPart*, dString*>* meshParts) {
	CL_THROW_IF_TRUE(!vertexBuffer || !meshParts);
	
	_super::init();
	
	_vertexBuffer = vertexBuffer->retain();
	parts()->addEntriesFromDictionary(meshParts);
	
	return this;
}

void
iMesh::dealloc() {
	CL_RELEASE(_vertexBuffer);
	CL_RELEASE(_defaultMaterial);
	CL_RELEASE(_parts);
	
	_super::dealloc();
}

iMesh*
iMesh::meshWithVertexBuffer(iVertexBuffer* vertexBuffer) {
	return alloc()->initWithVertexBuffer(vertexBuffer)->autorelease();
}

iMesh*
iMesh::meshWithVertexBufferAndPart(iVertexBuffer* vertexBuffer, iMeshPart* part, dString* name) {
	return alloc()->initWithVertexBufferAndPart(vertexBuffer, part, name)->autorelease();
}

iMesh*
iMesh::meshWithVertexBufferAndParts(iVertexBuffer* vertexBuffer, dDictT<iMeshPart*, dString*>* meshParts) {
	return alloc()->initWithVertexBufferAndParts(vertexBuffer, meshParts)->autorelease();
}

void
iMesh::setMaterial(iMaterial* material) {
	CL_SWITCH_LINKS(_defaultMaterial, material);
}

iMaterial*
iMesh::material() const {
	return _defaultMaterial;
}

dDictT<iMeshPart*, dString*>*
iMesh::parts() {
	if (!_parts) _parts = (dDictT<iMeshPart*, dString*>*)
							dDictT<iMeshPart*, dString*>::alloc()->init();
	return _parts;
}

void
iMesh::renderInContext(iContext* context) {
	context->setVertexBuffer(_vertexBuffer);
	
	dDictT<iMeshPart*, dString*>::Iterator it(parts());
	while (it.next()) it.object()->renderInContext(context, _defaultMaterial);
}

void
iMesh::renderPartByNameInContext(dString* name, iContext* context) {
	context->setVertexBuffer(_vertexBuffer);
	parts()->objectForKey(name)->renderInContext(context, _defaultMaterial);
}