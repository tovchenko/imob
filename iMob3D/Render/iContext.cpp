/*
 ** $Id: 2010/05/12, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iContext.h"
#include "iScene.h"
#include "iVertexBuffer.h"
#include "iIndexBuffer.h"
#include "iMaterial.h"
#include "iTexture.h"
#include "iMeshPrimitives.h"

#define M_2PI		6.28318530717958
#define M_3PI_2		4.71238898038469

dFloat  _clampRadians(dFloat rad);

iContext* iContext::_sCurrentContext = nil;
mat4f iContext::_sOrientationLandscapeLeftMtx;
mat4f iContext::_sOrientationLandscapeRightMtx;
mat4f iContext::_sOrientationPortraitUpsideDownMtx;

CL_CLASS_DEF_WITH_STATIC_CTORDTOR(iContext, IM, &iContext::initialize, nil)

void
iContext::initialize() {
	vec3f zVec(0, 0, 1);
	
	_sOrientationLandscapeLeftMtx.rotate(-M_PI_2, zVec);
	_sOrientationLandscapeRightMtx.rotate(M_PI_2, zVec);
	_sOrientationPortraitUpsideDownMtx.rotate(M_PI, zVec);
}

iContext::iContext()
: _sysContext(nil),
  _maxTextureSize(0),
  _maxTextureUnitsCount(0),
  _vertexBuffer(nil),
  _prevVertexBuffer(nil),
  _indexBuffer(nil),
  _prevIndexBuffer(nil),
  _material(nil),
  _prevMaterial(nil),
  _contentsScale(1),
  _orientation(kiEContextOrientationUnknown),
  _orientationAngle(0),
  _modelMtx(&mat4f::IDENTITY),
  _viewMtx(&mat4f::IDENTITY),
  _orientationMtx(mat4f::IDENTITY),
  _projectionMtx(&mat4f::IDENTITY),
  _textureMtx(&mat4f::IDENTITY),
  _textureMtxFor2D(mat4f::IDENTITY),
  _modelMtxFor2D(mat4f::IDENTITY),
  _2DMesh(nil) {
}

iContext*
iContext::initWithSysContext(iSysContext* sysContext) {
	CL_THROW_IF_NIL(sysContext);
	
	_super::init();
	
	_sysContext = sysContext;
	
	ZERO_MEMORY_STRUCT(_contextFlags);
	
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &_maxTextureSize);
	if (useShadersApi()) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnitsCount);
	else glGetIntegerv(GL_MAX_TEXTURE_UNITS, &_maxTextureUnitsCount);
	_contextFlags.drawTextureOESSupported = glCheckExtension("GL_OES_draw_texture", glGetString(GL_EXTENSIONS));
	
	for (dUInteger i = 0; i < _maxTextureUnitsCount; ++i) {
		_textures.addObject(nil);
		_lodBiases.addObject(0);
		_prevTextures.addObject(nil);
		_prevLODBiases.addObject(0);
		_disableTexCoords.addObject(false);
	}
		
	return this;
}

void
iContext::dealloc() {
	makeCurrentContext();
	resetCurrentContext();
	
	CL_RELEASE(_2DMesh);
	_super::dealloc();
}

bool
iContext::useShadersApi() const {
	return _sysContext->useShadersApi();
}

void
iContext::setViewport(dRect rect) {
	glViewport(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
	glSignalIfError();
	_viewport = rect;
}

const dRect&
iContext::viewport() const {
	return _viewport;
}

void
iContext::setContentsScale(dFloat scaleFactor) {
	_contentsScale = scaleFactor;
}

void
iContext::setVertexBuffer(iVertexBuffer* vertexBuffer) {
	_vertexBuffer = vertexBuffer;
}

void
iContext::setIndexBuffer(iIndexBuffer* indexBuffer) {
	_indexBuffer = indexBuffer;
}

void
iContext::setMaterial(iMaterial* material) {
	_material = material;
}

void
iContext::setTextureUnits(const dArrayT<iTextureUnit*>* textureUnits) {
	dUInteger texUnitCount = textureUnits->count();
	if (texUnitCount > _maxTextureUnitsCount)
		throw new iExMaxTextureUnit;
	
	clearTextureUnits();
	
	for (dUInteger i = 0; i < texUnitCount; ++i) {
		iTextureUnit* unit = textureUnits->objectAtIndex(i);
		_textures.replaceObjectAtIndexWithObject(i, unit->texture());
		_lodBiases.replaceObjectAtIndexWithObject(i, unit->lodBias());
	}
}

void
iContext::clearTextureUnits() {
	for (dUInteger i = 0; i < _maxTextureUnitsCount; ++i) {
		_textures.replaceObjectAtIndexWithObject(i, nil);
		_lodBiases.replaceObjectAtIndexWithObject(i, 0);
	}
}

void
iContext::resetTextureAtUnitIndex(dUInteger index) {
	if (index < _prevTextures.count()) {
		_prevTextures.replaceObjectAtIndexWithObject(index, nil);
		glActiveTexture(GL_TEXTURE0 + index);
		glSignalIfError();
		
		iTexture::unbindAll();
	} else if (index >= _maxTextureUnitsCount) {
		throw new iExMaxTextureUnit;
	}
}

void
iContext::resetIndexBuffer() {
	_prevIndexBuffer = nil;
	iIndexBuffer::unbind();
}

void
iContext::resetVertexBuffer() {
	_prevVertexBuffer = nil;
	iVertexBuffer::unbind();
}

void
iContext::setModelMatrix(const mat4f* mtx) {
	if (mtx) {
		_modelMtx = mtx;
		_contextFlags.isModelViewMtxValid = false;
	} else if (_modelMtx != &mat4f::IDENTITY) {
		_modelMtx = &mat4f::IDENTITY;
		_contextFlags.isModelViewMtxValid = false;
	}
}

void
iContext::setViewMatrix(const mat4f* mtx) {
	if (mtx) {
		_viewMtx = mtx;
		_contextFlags.isModelViewMtxValid = false;
	} else if (_viewMtx != &mat4f::IDENTITY) {
		_viewMtx = &mat4f::IDENTITY;
		_contextFlags.isModelViewMtxValid = false;
	}
}

void
iContext::setProjectionMatrix(const mat4f* mtx) {
	if (mtx) {
		_projectionMtx = mtx;
		_contextFlags.isProjectionMtxValid = false;
	} else if (_projectionMtx != &mat4f::IDENTITY) {
		_projectionMtx = &mat4f::IDENTITY;
		_contextFlags.isProjectionMtxValid = false;
	}
}

void
iContext::setTextureMatrix(const mat4f* mtx) {
	if (mtx) {
		_textureMtx = mtx;
		_contextFlags.isTextureMtxValid = false;
	} else if (_textureMtx != &mat4f::IDENTITY) {
		_textureMtx = &mat4f::IDENTITY;
		_contextFlags.isTextureMtxValid = false;
	}
}

void
iContext::setOrientation(iEContextOrientation orientation) {
	if (_orientation == orientation)
		return;
	
	_orientation = orientation;
	_contextFlags.isModelViewMtxValid = false;
	_contextFlags.isOrientationMtxValid = false;
	_contextFlags.isOrientationAngleValid = false;
}

void
iContext::setDynamicOrientation(bool isDynamic) {
	_contextFlags.isDynamicOrientation = isDynamic;
}

void
iContext::updateDynamicOrientationAngle() {
	if (_contextFlags.isOrientationAngleValid || !_contextFlags.isDynamicOrientation)
		return;
	
	dFloat orientationAngle = 0;
	dFloat diff1, diff2, diff3;
	
	switch (_orientation) {
		case kiEContextOrientationUnknown:
		case kiEContextOrientationPortrait:
			diff1 = fabsf(0 - _orientationAngle);
			diff2 = fabsf(M_2PI - _orientationAngle);
			diff3 = fabsf(-M_2PI - _orientationAngle);
			orientationAngle = (diff1 < diff2) ? 0 : (diff2 < diff3) ? M_2PI : -M_2PI;
			break;

		case kiEContextOrientationLandscapeLeft:
			diff1 = fabsf(-M_PI_2 - _orientationAngle);
			diff2 = fabsf(M_3PI_2 - _orientationAngle);
			orientationAngle = (diff1 < diff2) ? -M_PI_2 : M_3PI_2;
			break;

		case kiEContextOrientationLandscapeRight:
			diff1 = fabsf(M_PI_2 - _orientationAngle);
			diff2 = fabsf(-M_3PI_2 - _orientationAngle);
			orientationAngle = (diff1 < diff2) ? M_PI_2 : -M_3PI_2;
			break;
			
		case kiEContextOrientationPortraitUpsideDown:
			diff1 = fabsf(M_PI - _orientationAngle);
			diff2 = fabsf(-M_PI - _orientationAngle);
			orientationAngle = (diff1 < diff2) ? M_PI : -M_PI;
			break;
	}
	
	dFloat diff = orientationAngle - _orientationAngle;
	if (fabsf(diff) < 0.01f) {
		_orientationAngle = orientationAngle;
		_contextFlags.isOrientationAngleValid = true;
	} else {
		_orientationAngle += 0.1f * diff;
	}
	
	_orientationAngle = _clampRadians(_orientationAngle + M_PI) - M_PI;
}

dRect
iContext::bounds() const {
	dRect result = _viewport;
	dFloat factor = 1.0f / _contentsScale;
	
	result.origin.x *= factor;
	result.origin.y *= factor;
	result.size.width *= factor;
	result.size.height *= factor;
	
	if (kiEContextOrientationLandscapeLeft == _orientation ||
		kiEContextOrientationLandscapeRight == _orientation)
	{
		dFloat temp = result.origin.x;
		result.origin.x = result.origin.y;
		result.origin.y = temp;
		
		temp = result.size.width;
		result.size.width = result.size.height;
		result.size.height = temp;
	}
	
	return result;
}

void
iContext::enter2DMode() {
	if (_contextFlags.isIn2DMode) return;
	
	_viewProjMtxFor2D = mat4f::IDENTITY;
	_viewProjMtxFor2D(0, 3) = -1.0f;
	_viewProjMtxFor2D(1, 3) = -1.0f;
	_viewProjMtxFor2D(0, 0) = 2.0f;
	_viewProjMtxFor2D(1, 1) = 2.0f;
	
	switch (_orientation) {
		case kiEContextOrientationLandscapeLeft:
			_viewProjMtxFor2D = _sOrientationLandscapeLeftMtx * _viewProjMtxFor2D;
			break;
		case kiEContextOrientationLandscapeRight:
			_viewProjMtxFor2D = _sOrientationLandscapeRightMtx * _viewProjMtxFor2D;
			break;
		case kiEContextOrientationPortraitUpsideDown:
			_viewProjMtxFor2D = _sOrientationPortraitUpsideDownMtx * _viewProjMtxFor2D;
			break;
        default:
            break;
	}
	
	if (!useShadersApi()) {
		glActiveTexture(GL_TEXTURE0);
		glMatrixMode(GL_TEXTURE);
		glPushMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
	}
	
	GLboolean depthWasEnableBefore2DMode;
	glGetBooleanv(GL_DEPTH_TEST, &depthWasEnableBefore2DMode);
	_contextFlags.depthWasEnableBefore2DMode = depthWasEnableBefore2DMode;
	if (depthWasEnableBefore2DMode) glDisable(GL_DEPTH_TEST);
	
	glSignalIfError();
	_contextFlags.isIn2DMode = true;
}

void
iContext::exit2DMode() {
	if (!_contextFlags.isIn2DMode) return;
	
	if (!useShadersApi()) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		
		glActiveTexture(GL_TEXTURE0);
		glMatrixMode(GL_TEXTURE);
		glPopMatrix();
	}
	
	if (_contextFlags.depthWasEnableBefore2DMode) glEnable(GL_DEPTH_TEST);
	_contextFlags.isIn2DMode = false;
}

void
iContext::drawIndexed(iEDrawMode drawMode, dInteger first, dUInteger count) {
	_bindContextObjects();
	_indexBuffer->draw(drawMode, first, count);
}

void
iContext::drawNonIndexed(iEDrawMode drawMode, dInteger first, dUInteger count) {
	_bindContextObjects();
	_vertexBuffer->draw(drawMode, first, count);
}

void
iContext::drawTexture(iTexture2D* texture, dRect srcRect, dRect dstRect) {
#ifdef IM_USE_DRAW_TEX_OES
	if (_contextFlags.drawTextureOESSupported) {
		dstRect = dMakeRect(
						dstRect.origin.x * _contentsScale, dstRect.origin.y * _contentsScale,
						dstRect.size.width * _contentsScale, dstRect.size.height * _contentsScale);
		
		_material = nil;
		_bindMaterialIfNeeded();
		
		_textures.replaceObjectAtIndexWithObject(0, texture);
		_bindTexturesIfNeeded();
		
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, (const GLfloat*)&srcRect);
		glDrawTexfOES(dstRect.origin.x, dstRect.origin.y, 0, dstRect.size.width, dstRect.size.height);
		glSignalIfError();
	} else
#endif
	{
		enter2DMode();
		_create2DMeshIfNeeded();
		_2DMesh->material()->textureUnitAtIndex(0)->setTexture(texture);
		
		dRect rect = bounds();
		vec2f srcFactor = vec2f(1.0f / texture->width(), 1.0f / texture->height());
		vec2f dstFactor = vec2f(1.0f / rect.size.width, 1.0f / rect.size.height);
		
		_textureMtxFor2D(0, 3) = srcRect.origin.x * srcFactor.x();
		_textureMtxFor2D(1, 3) = srcRect.origin.y * srcFactor.y();
		_textureMtxFor2D(0, 0) = srcRect.size.width * srcFactor.x();
		_textureMtxFor2D(1, 1) = srcRect.size.height * srcFactor.y();
		
		_modelMtxFor2D(0, 3) = dstRect.origin.x * dstFactor.x();
		_modelMtxFor2D(1, 3) = dstRect.origin.y * dstFactor.y();
		_modelMtxFor2D(0, 0) = dstRect.size.width * dstFactor.x();
		_modelMtxFor2D(1, 1) = dstRect.size.height * dstFactor.y();
		
		_modelViewProjMtxFor2D.multiply(_viewProjMtxFor2D, _modelMtxFor2D);
		
		if (!useShadersApi()) {
			glMatrixMode(GL_TEXTURE);
			glLoadMatrixf(_textureMtxFor2D);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadMatrixf(_modelViewProjMtxFor2D);
			
			glSignalIfError();
		}
		
		_2DMesh->renderInContext(this);
		
		exit2DMode();
	}
}

void
iContext::makeCurrentContext() {
	_sysContext->makeCurrentContext();
	_sCurrentContext = this;
}

void
iContext::resetCurrentContext() {
	_sCurrentContext = nil;
	_sysContext->resetCurrentContext();
}

iContext*
iContext::currentContext() {
	return _sCurrentContext;
}

bool
iContext::_bindVertexBufferIfNeeded() {
	if (_vertexBuffer == _prevVertexBuffer)
		return false;
	
	if (_vertexBuffer) _vertexBuffer->bind();
	else iVertexBuffer::unbind();
	_prevVertexBuffer = _vertexBuffer;
	return true;
}

bool
iContext::_bindIndexBufferIfNeeded() {
	if (_indexBuffer == _prevIndexBuffer)
		return false;
	
	if (_indexBuffer) _indexBuffer->bind();
	else iIndexBuffer::unbind();
	_prevIndexBuffer = _indexBuffer;
	return true;
}

bool
iContext::_bindTexturesIfNeeded() {
	bool wereChanged = false;
	
	for (dUInteger i = 0, cnt = _textures.count(); i < cnt; ++i) {
		bool textureUnitActivated = false;
		
		iTexture* currentTexture = _textures.objectAtIndex(i);
		iTexture* prevTexture = _prevTextures.objectAtIndex(i);
		
		if (currentTexture != prevTexture) {
			glActiveTexture(GL_TEXTURE0 + i);
			glSignalIfError();
			
			if (currentTexture) {
				if (!useShadersApi() && !prevTexture) {
					glEnable(GL_TEXTURE_2D);
					glSignalIfError();
				}
				currentTexture->bind();
			} else {
				iTexture::unbindAll();
			}
			
			_prevTextures.replaceObjectAtIndexWithObject(i, currentTexture);
			
			textureUnitActivated = true;
			wereChanged = true;
		}
		
		
		dFloat currentLODBias = _lodBiases.objectAtIndex(i);
		
		if (currentLODBias != _prevLODBiases.objectAtIndex(i)) {
			if (!useShadersApi()) {
				if (!textureUnitActivated)
					glActiveTexture(GL_TEXTURE0 + i);
				
				glTexEnvf(GL_TEXTURE_FILTER_CONTROL_EXT, GL_TEXTURE_LOD_BIAS_EXT, currentLODBias);
				glSignalIfError();
			}
			
			_prevLODBiases.replaceObjectAtIndexWithObject(i, currentLODBias);
			
			wereChanged = true;
		}
	}
	
	return wereChanged;
}

bool
iContext::_bindMaterialIfNeeded() {
	if (_material == _prevMaterial && !(_material ? _material->updated() : false))
		return false;
	
	if (_material) _material->bind();
	else iMaterial::unbind();
	_prevMaterial = _material;
	return true;
}

void
iContext::_bindContextObjects() {
	_bindMaterialIfNeeded();
	_bindIndexBufferIfNeeded();
	bool vbWasChanged = _bindVertexBufferIfNeeded();
	/*bool texturesWereChanged =*/ _bindTexturesIfNeeded();
	
	if (useShadersApi()) {		
	} else {
		if (vbWasChanged) _bindPointersForFFPipeline();
		_applyTransformsForFFPipelineIfNeeded();
	}
}

void
iContext::_bindPointersForFFPipeline() {
	bool disablePosition = _contextFlags.disablePosition;
	bool disableNormal = _contextFlags.disableNormal;
	bool disableDiffuse = _contextFlags.disableDiffuse;
	CArrayT<bool, bool> disableTexCoords(_disableTexCoords);
	
	if (_prevVertexBuffer) {
		iEFVF fvf = _prevVertexBuffer->fvf();
		const dByte* ptr = _prevVertexBuffer->boundPointer();
		dUInteger itemSize = _prevVertexBuffer->itemSize();
		
		if (fvf & kiEFVF_XYZ) {
			if (!disablePosition) {
				glEnableClientState(GL_VERTEX_ARRAY);
				_contextFlags.disablePosition = true;
			} else {
				disablePosition = false;
			}
			glVertexPointer(3, GL_FLOAT, itemSize, ptr);
			ptr += 3 * sizeof(GLfloat);
		}
		
		if (fvf & kiEFVF_Normal) {
			if (!disableNormal) {
				glEnableClientState(GL_NORMAL_ARRAY);
				_contextFlags.disableNormal = true;
			} else {
				disableNormal = false;
			}
			glNormalPointer(GL_FLOAT, itemSize, ptr);
			ptr += 3 * sizeof(GLfloat);
		}
		
		if (fvf & kiEFVF_Diffuse) {
			if (!disableDiffuse) {
				glEnableClientState(GL_COLOR_ARRAY);
				_contextFlags.disableDiffuse = true;
			} else {
				disableDiffuse = false;
			}
			glColorPointer(4, GL_UNSIGNED_BYTE, itemSize, ptr);
			ptr += 4 * sizeof(GLbyte);
		}
		
		if (fvf & kiEFVF_TexCountMask) {
			dUInteger texCount = (fvf & kiEFVF_TexCountMask) >> kiEFVF_TexCountShift;
			for (dUInteger i = 0; i < texCount; ++i) {
				dUInteger texCoordSize = iEFVF_GET_TEXCOORDSIZE(fvf, i);
				
				if (i < _maxTextureUnitsCount) {
					glClientActiveTexture(GL_TEXTURE0 + i);
					if (!disableTexCoords.objectAtIndex(i)) {
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						_disableTexCoords.replaceObjectAtIndexWithObject(i, true);
					} else {
						disableTexCoords.replaceObjectAtIndexWithObject(i, false);
					}
					glTexCoordPointer(texCoordSize, GL_FLOAT, itemSize, ptr);
				}
				
				ptr += texCoordSize * sizeof(GLfloat);
			}
		}
	}
	
	if (disablePosition) {
		glDisableClientState(GL_VERTEX_ARRAY);
		_contextFlags.disablePosition = false;
	}
	
	if (disableNormal) {
		glDisableClientState(GL_NORMAL_ARRAY);
		_contextFlags.disableNormal = false;
	}
	
	if (disableDiffuse) {
		glDisableClientState(GL_COLOR_ARRAY);
		_contextFlags.disableDiffuse = false;
	}
	
	for (dUInteger i = 0; i < _maxTextureUnitsCount; ++i) {
		if (disableTexCoords.objectAtIndex(i)) {
			glClientActiveTexture(GL_TEXTURE0 + i);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			_disableTexCoords.replaceObjectAtIndexWithObject(i, false);
		}
	}
	
	glSignalIfError();
}

void
iContext::_applyTransformsForFFPipelineIfNeeded() {
	if (_contextFlags.isIn2DMode) return;
	
	if (!_contextFlags.isProjectionMtxValid) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(*_projectionMtx);
		_contextFlags.isProjectionMtxValid = true;
	}
	
	if (!_contextFlags.isModelViewMtxValid) {
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(_modelViewMatrix());
		_contextFlags.isModelViewMtxValid = true;
	}
	
	if (!_contextFlags.isTextureMtxValid) {
		glMatrixMode(GL_TEXTURE);
		glLoadMatrixf(*_textureMtx);
		_contextFlags.isTextureMtxValid = true;
	}
	
	glSignalIfError();
}

void
iContext::_bindPointersForShadersPipeline() {
}

mat4f&
iContext::_modelViewMatrix() {
	if (_contextFlags.isModelViewMtxValid)
		return _modelViewMtx;
	
	if (!_contextFlags.isOrientationMtxValid) {
		if (!_contextFlags.isDynamicOrientation) {
			switch (_orientation) {
				case kiEContextOrientationLandscapeLeft:
					_orientationMtx = _sOrientationLandscapeLeftMtx;
					break;
					
				case kiEContextOrientationLandscapeRight:
					_orientationMtx = _sOrientationLandscapeRightMtx;
					break;
					
				case kiEContextOrientationPortraitUpsideDown:
					_orientationMtx = _sOrientationPortraitUpsideDownMtx;
					break;
					
				default:
					_orientationMtx = mat4f::IDENTITY;
					break;
			}
			
			_contextFlags.isOrientationMtxValid = true;
		} else {
			_orientationMtx.rotate(_orientationAngle, vec3f::UNIT_Z);
			
			if (_contextFlags.isOrientationAngleValid)
				_contextFlags.isOrientationMtxValid = true;
		}
	}
	
	mat4f result;
	result.multiply(_orientationMtx, *_viewMtx);
	_modelViewMtx.multiply(result, *_modelMtx);
	_contextFlags.isModelViewMtxValid = true;
	
	return _modelViewMtx;
}

void
iContext::_create2DMeshIfNeeded() {
	if (_2DMesh) return;
	_2DMesh = iPlaneMesh::alloc()->initWithRectAndSegmetsCount(dMakeRect(0, 0, 1, 1), 1, 1);
	_2DMesh->setMaterial(iMaterial::material());
}


CL_ABSTRACT_DEF(iSysContext, IM)

iSysContext::iSysContext()
: _scene(nil),
  _useShadersApi(false) {
}

iSysContext*
iSysContext::init() {
	_super::init();
	
	return this;
}

void
iSysContext::dealloc() {
	CL_RELEASE(_scene);
	
	_super::dealloc();
}

bool
iSysContext::useShadersApi() const {
	return _useShadersApi;
}

void
iSysContext::setScene(iScene* scene) {
	CL_SWITCH_LINKS(_scene, scene);
	setup();
}

iScene*
iSysContext::scene() {
	return _scene;
}

// ===================================================

dFloat
_clampRadians(dFloat rad) {
	while (rad > M_2PI) rad -= M_2PI;
	while (rad < 0) rad += M_2PI;
	
	return rad;
}