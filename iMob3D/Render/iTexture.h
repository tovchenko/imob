/*
 ** $Id: 2010/06/28, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iTexture_H
#define _iTexture_H

BEGIN_NAMESPACE_IM

class iTextureData;

class iTexture : public dObject {
CL_ABSTRACT_DECL(iTexture, dObject)
public:
	void					bind();
	static void				unbindAll();
	
	void					setFilters(iETextureFilter minFilter, iETextureFilter magFilter);
	
protected:
	iTexture();
	virtual iTexture*		init();
	virtual void			dealloc();
	
	virtual iETextureTarget	_textureTarget() = 0;
	static void				_unbind(iETextureTarget target);
	
	bool					_isFiltersValid;
	iETextureFilter			_magFilter;
	iETextureFilter			_minFilter;
	
	dUInteger				_textureID;
};


class iTexture2D : public iTexture {
CL_CLASS_DECL(iTexture2D, iTexture)
public:
	virtual iTexture2D*		initWithTextureData(iTextureData* textureData);
	void					updateWithTextureData(iTextureData* textureData);
	
	static void				unbind();
	
	dUInteger				width() const;
	dUInteger				height() const;
	
protected:
	iTexture2D();
	
	virtual iETextureTarget	_textureTarget();
	
	dUInteger				_width;
	dUInteger				_height;
};


class iTextureCube : public iTexture {
CL_CLASS_DECL(iTextureCube, iTexture)
public:
	virtual iTextureCube*	initWithTextureDataArray(dArrayT<iTextureData*>* txtDataArray);	// must be array of 6 iTextureData in order: xPos, xNeg, yPos, yNeg, zPos, zNeg
	
	static void				unbind();
	
protected:
	virtual iETextureTarget	_textureTarget();
};

END_NAMESPACE_IM

#endif // _iTexture_H