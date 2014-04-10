/*
 ** $Id: 2010/05/07, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iDrawable.h"
#include "iMesh.h"
#include "iMaterial.h"
#include "iContext.h"

CL_CLASS_DEF(iDrawable, IM)

iDrawable::iDrawable()
: _mesh(nil),
  _matrix(nil),
  _material(nil) {
}

iDrawable*
iDrawable::initWithMesh(iMesh* mesh, iMaterial* material) {
	CL_THROW_IF_NIL(mesh);
	
	_super::init();
	
	_mesh = mesh->retain();
	_material = material ? material->retain() : nil;
	
	return this;
}

void
iDrawable::dealloc() {
	CL_RELEASE(_mesh);
	CL_RELEASE(_material);
	_super::dealloc();
}

iDrawable*
iDrawable::drawableWithMesh(iMesh* mesh, iMaterial* material) {
	return alloc()->initWithMesh(mesh, material)->autorelease();
}

void
iDrawable::setMesh(iMesh* mesh) {
	CL_SWITCH_LINKS(_mesh, mesh);
}

iMesh*
iDrawable::mesh() const {
	return _mesh;
}

void
iDrawable::setMatrix(const mat4f* mtx) {
	_matrix = mtx;
}

const mat4f*
iDrawable::matrix() const {
	return _matrix;
}

void
iDrawable::setMaterial(iMaterial* material) {
	CL_SWITCH_LINKS(_material, material);
}

iMaterial*
iDrawable::material() const {
	return _material;
}

void
iDrawable::renderInContext(iContext* context) {
	context->setModelMatrix(_matrix);
	_mesh->setMaterial(_material);
	_mesh->renderInContext(context);
}

void
iDrawable::renderMeshPartByNameInContext(dString* name, iContext* context) {
	context->setModelMatrix(_matrix);
	_mesh->renderPartByNameInContext(name, context);
}