/*
 ** $Id: 2010/05/12, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iContext_H
#define _iContext_H

BEGIN_NAMESPACE_IM

class iVertexBuffer;
class iIndexBuffer;
class iTexture;
class iTexture2D;
class iMaterial;
class iTextureUnit;
class iScene;
class iSysContext;
class iMesh;

class iContext : public dObject {
CL_CLASS_DECL(iContext, dObject)
public:
	// exceptions
    class iExMaxTextureUnit : public CException { };
	
	virtual iContext*				initWithSysContext(iSysContext* sysContext);
	
	bool							useShadersApi() const;
	
	void							setViewport(dRect rect);
	const dRect&					viewport() const;
	void							setContentsScale(dFloat scaleFactor);
	
	void							setVertexBuffer(iVertexBuffer* vertexBuffer);
	void							setIndexBuffer(iIndexBuffer* indexBuffer);
	void							setMaterial(iMaterial* material);
	
	void							setTextureUnits(const dArrayT<iTextureUnit*>* textureUnits);
	void							clearTextureUnits();
	void							resetTextureAtUnitIndex(dUInteger index);
	void							resetIndexBuffer();
	void							resetVertexBuffer();
	
	void							setModelMatrix(const mat4f* mtx);
	void							setViewMatrix(const mat4f* mtx);
	void							setProjectionMatrix(const mat4f* mtx);
	void							setTextureMatrix(const mat4f* mtx);
	
	void							setOrientation(iEContextOrientation orientation);
	void							setDynamicOrientation(bool isDynamic);
	void							updateDynamicOrientationAngle();
	
	dRect							bounds() const;
	
	void							enter2DMode();
	void							exit2DMode();
	
	virtual	void					drawIndexed(iEDrawMode drawMode, dInteger first, dUInteger count);
	virtual	void					drawNonIndexed(iEDrawMode drawMode, dInteger first, dUInteger count);
	virtual void					drawTexture(iTexture2D* texture, dRect srcRect, dRect dstRect);
	
	virtual void					makeCurrentContext();
	virtual void					resetCurrentContext();
	static iContext*				currentContext();
	
protected:
	static void						initialize();
	iContext();
	virtual void					dealloc();
	
	bool							_bindVertexBufferIfNeeded();
	bool							_bindIndexBufferIfNeeded();
	bool							_bindTexturesIfNeeded();
	bool							_bindMaterialIfNeeded();
	
	void							_bindContextObjects();
	void							_bindPointersForFFPipeline();
	void							_applyTransformsForFFPipelineIfNeeded();
	void							_bindPointersForShadersPipeline();
	
	mat4f&							_modelViewMatrix();
	
	void							_create2DMeshIfNeeded();
	
	iSysContext*					_sysContext;
	dInteger						_maxTextureSize;
	dInteger						_maxTextureUnitsCount;
	bool							_drawTextureOESSupported;
	
	dRect							_viewport;
	
	iVertexBuffer*					_vertexBuffer;
	iVertexBuffer*					_prevVertexBuffer;
	iIndexBuffer*					_indexBuffer;
	iIndexBuffer*					_prevIndexBuffer;
	iMaterial*						_material;
	iMaterial*						_prevMaterial;
	
	CArrayT<iTexture*, iTexture*>	_textures;
	CArrayT<iTexture*, iTexture*>	_prevTextures;
	CArrayT<dFloat, dFloat>			_lodBiases;
	CArrayT<dFloat, dFloat>			_prevLODBiases;
	CArrayT<bool, bool>				_disableTexCoords;
	
	dFloat							_contentsScale;
	iEContextOrientation			_orientation;
	dFloat							_orientationAngle;
	
	mat4f							_modelViewMtx;
	const mat4f*					_modelMtx; // pointer for performans reason
	const mat4f*					_viewMtx; // pointer for performans reason
	mat4f							_orientationMtx;
	const mat4f*					_projectionMtx; // pointer for performans reason
	const mat4f*					_textureMtx; // pointer for performans reason
	
	mat4f							_modelViewProjMtxFor2D;
	mat4f							_viewProjMtxFor2D;
	mat4f							_textureMtxFor2D;
	mat4f							_modelMtxFor2D;
	
	struct {
		dUInteger					isModelViewMtxValid:1;
		dUInteger					isProjectionMtxValid:1;
		dUInteger					isTextureMtxValid:1;
		dUInteger					disablePosition:1;
		dUInteger					disableNormal:1;
		dUInteger					disableDiffuse:1;
		dUInteger					isIn2DMode:1;
		dUInteger					depthWasEnableBefore2DMode:1;
		dUInteger					isOrientationMtxValid:1;
		dUInteger					isOrientationAngleValid:1;
		dUInteger					isDynamicOrientation:1;
		dUInteger					drawTextureOESSupported:1;
	} _contextFlags;
	
	iMesh*							_2DMesh;
	
	static iContext*				_sCurrentContext;
	
	static mat4f					_sOrientationLandscapeLeftMtx;
	static mat4f					_sOrientationLandscapeRightMtx;
	static mat4f					_sOrientationPortraitUpsideDownMtx;
};


class iSysContext : public dObject {
CL_ABSTRACT_DECL(iSysContext, dObject)
public:
	bool					useShadersApi() const;
	
	void					setScene(iScene* scene);
	iScene*					scene();
	
	virtual void			makeCurrentContext() = 0;
	virtual void			resetCurrentContext() = 0;
	virtual void			setup() = 0;
	
protected:
	virtual iSysContext*	init();
	iSysContext();
	virtual void			dealloc();
	
	iScene*					_scene;
	bool					_useShadersApi;
};

END_NAMESPACE_IM

#endif // _iContext_H