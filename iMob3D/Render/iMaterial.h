/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iMaterial_H
#define _iMaterial_H


BEGIN_NAMESPACE_IM

class iTexture;
class iContext;
class iTextureUnit;

class iMaterial : public dObject {
friend class iTextureUnit; // _updated
CL_CLASS_DECL(iMaterial, dObject)
public:
	static iMaterial*			material();
	static iMaterial*			materialWithTexture(iTexture* texture);
	
	virtual iMaterial*			initWithTexture(iTexture* texture);
	
	iTextureUnit*				textureUnitAtIndex(dUInteger index);
	
	void						bind();
	static void					unbind();
	
	bool						updated() const;
	
protected:
	iMaterial();
	virtual void				dealloc();
	
	dArrayT<iTextureUnit*>*		_textureUnits;
	bool						_updated;
};

// don't create instance of this class
// call textureUnitAtIndex from iMaterial
// and it create iTextureUnit and return it.
class iTextureUnit : public dObject {
friend class iMaterial; // _initWithOwner
CL_CLASS_DECL(iTextureUnit, dObject)
public:
	void					setTexture(iTexture* txt);
	iTexture*				texture() const;
	void					setLODBias(dFloat lod);
	dFloat					lodBias() const;
	
protected:
	iTextureUnit();
	virtual iTextureUnit*	_initWithOwner(iMaterial* owner);
	virtual void			dealloc();
	
	iMaterial*				_owner;
	
	iTexture*				_texture;
	dFloat					_lodBias;
};

END_NAMESPACE_IM

#endif // _iMaterial_H