/*
 ** $Id: 2010/06/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iTextureData_H
#define _iTextureData_H

BEGIN_NAMESPACE_IM

class _iTextureImp;

class iTextureData : public dObject {
CL_CLASS_DECL(iTextureData, dObject)
public:
	virtual iTextureData*	initWithData(const dData* data);
	virtual iTextureData*	initWithCGImage(CGImageRef img, bool isFlipped = false);
	virtual iTextureData*	initWithPVRData(const dData* data);
	virtual iTextureData*	initWithRGB565Data(const dData* data);
	virtual iTextureData*	initWithPVRTCData(const dData* data, const dDictT<dValue*, dString*>* params);
	virtual iTextureData*	initWithPVRTCData(const dData* data, dUInteger bpp, bool hasAlpha, dUInteger width, dUInteger height);
	
	dUInteger				width() const;
	dUInteger				height() const;
	dUInteger				mipmapsCount() const;
	
	void					createTextureImageByTarget(iETextureTarget target);
	
protected:
	iTextureData();
	virtual void			dealloc();
	
	_iTextureImp*			_impl;
};

END_NAMESPACE_IM

#endif // _iTextureData_H