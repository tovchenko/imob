/*
 ** $Id: 2010/06/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iTextureData.h"


enum { kiTextureSignature = 'tex!' };


BEGIN_NAMESPACE_IM

class _iTextureImp {
public:
	_iTextureImp();
	
	dUInteger		width() const { return _width; }
	dUInteger		height() const { return _height; }
	dUInteger		mipmapsCount() const { return _mipmapsCount; }
	
	virtual void	createTextureImageByTarget(iETextureTarget target) = 0;
	
protected:
	dUInteger	_width;
	dUInteger	_height;
	dUInteger	_mipmapsCount;
};

_iTextureImp::_iTextureImp()
: _width(0),
  _height(0),
  _mipmapsCount(1) {
}

//------------------------------------------------
// iTexture-Data implementation
//------------------------------------------------
class _iTextureDataImp : public _iTextureImp {
public:
	_iTextureDataImp(const dData* data);
	virtual ~_iTextureDataImp();
	
	virtual void	createTextureImageByTarget(iETextureTarget target);
	
protected:
	void			_glColorFormatFrom_iEColorFormat(iEColorFormat iFormat, GLenum* oFmt, GLenum* oType);
	dUInteger		_pixelByteSizeFrom_iEColorFormat(iEColorFormat iFormat);
	
	dData*			_data;
};

_iTextureDataImp::_iTextureDataImp(const dData* data) {
	CL_THROW_IF_NIL(data);
	
	iSTexHeader* header = (iSTexHeader*)data->bytes();
	assert(dSwapLittleIntToHost(header->tag) == kiTextureSignature);
	assert(dSwapLittleIntToHost(header->headerLength) + dSwapLittleIntToHost(header->dataLength) == data->length());
	
	_data = const_cast<dData*>(data)->retain();
	_width = dSwapLittleIntToHost(header->width);
	_height = dSwapLittleIntToHost(header->height);
	_mipmapsCount = dSwapLittleIntToHost(header->numMipmaps);
}

_iTextureDataImp::~_iTextureDataImp() {
	CL_RELEASE(_data);
}

void
_iTextureDataImp::createTextureImageByTarget(iETextureTarget target) {
	iSTexHeader* header = (iSTexHeader*)_data->bytes();
	iEColorFormat fmt = (iEColorFormat)dSwapLittleIntToHost(header->format);
	dUInteger headerLength = dSwapLittleIntToHost(header->headerLength);
	
	dUInteger pixelSz = _pixelByteSizeFrom_iEColorFormat(fmt);
	const dByte* pixels = (const dByte*)header + headerLength;
	GLenum glSrcFmt, glSrcType;
	_glColorFormatFrom_iEColorFormat(fmt, &glSrcFmt, &glSrcType);
	
	dInteger level = 0;
	dUInteger width = _width;
	dUInteger height = _height;
	
	while (level < _mipmapsCount) {
		glTexImage2D(target, level, glSrcFmt, width, height, 0, glSrcFmt, glSrcType, pixels);
		glSignalIfError();
		
		pixels += width * height * pixelSz;
		width = __max(width >> 1, 1);
		height = __max(height >> 1, 1);
		++level;
	}
}

void
_iTextureDataImp::_glColorFormatFrom_iEColorFormat(iEColorFormat iFormat, GLenum* oFmt, GLenum* oType) {
	switch (iFormat) {
		case kiEColorFormat_R8G8B8:		*oFmt = GL_RGB;				*oType = GL_UNSIGNED_BYTE;			break;
		case kiEColorFormat_R8G8B8A8:	*oFmt = GL_RGBA;			*oType = GL_UNSIGNED_BYTE;			break;
		case kiEColorFormat_R5G6B5:		*oFmt = GL_RGB;				*oType = GL_UNSIGNED_SHORT_5_6_5;	break;
		case kiEColorFormat_R5G5B5A1:	*oFmt = GL_RGBA;			*oType = GL_UNSIGNED_SHORT_5_5_5_1; break;
		case kiEColorFormat_R4G4B4A4:	*oFmt = GL_RGBA;			*oType = GL_UNSIGNED_SHORT_4_4_4_4; break;
		case kiEColorFormat_A8:			*oFmt = GL_ALPHA;			*oType = GL_UNSIGNED_BYTE;			break;
		case kiEColorFormat_L8:			*oFmt = GL_LUMINANCE;		*oType = GL_UNSIGNED_BYTE;			break;
		case kiEColorFormat_L8A8:		*oFmt = GL_LUMINANCE_ALPHA; *oType = GL_UNSIGNED_BYTE;			break;
			
		default:
			*oFmt = 0;
			*oType = 0;
			throw new ExAssert;
			break;
	}
}

dUInteger
_iTextureDataImp::_pixelByteSizeFrom_iEColorFormat(iEColorFormat iFormat) {
	switch (iFormat) {
		case kiEColorFormat_R8G8B8:		return 3;
		case kiEColorFormat_R8G8B8A8:	return 4;
		case kiEColorFormat_R5G6B5:		return 2;
		case kiEColorFormat_R5G5B5A1:	return 2;
		case kiEColorFormat_R4G4B4A4:	return 2;
		case kiEColorFormat_A8:			return 1;
		case kiEColorFormat_L8:			return 1;
		case kiEColorFormat_L8A8:		return 2;
			
		default:
			throw new ExAssert;
			break;
	}
	return 0;
}

//------------------------------------------------
// iTexture-CGImage implementation
//------------------------------------------------
class _iTextureCGImageImp : public _iTextureImp {
public:
	_iTextureCGImageImp(CGImageRef image, bool isFlipped = false);
	virtual ~_iTextureCGImageImp();
	
	virtual void	createTextureImageByTarget(iETextureTarget target);
	
protected:
	CGImageRef		_cgImage;
	bool			_isFlipped;	
};

_iTextureCGImageImp::_iTextureCGImageImp(CGImageRef image, bool isFlipped) {
	CL_THROW_IF_NIL(image);
	
	_isFlipped = isFlipped;
	_cgImage = CGImageRetain(image);
	_width = CGImageGetWidth(_cgImage);
	_height = CGImageGetHeight(_cgImage);
}

_iTextureCGImageImp::~_iTextureCGImageImp() {
	CGImageRelease(_cgImage);
}

void
_iTextureCGImageImp::createTextureImageByTarget(iETextureTarget target) {
	size_t bpp = CGImageGetBitsPerPixel(_cgImage);
	size_t bpc = CGImageGetBitsPerComponent(_cgImage);
	CGColorSpaceRef colorSpace = CGImageGetColorSpace(_cgImage);
	
	GLenum fmt = GL_RGBA;
	size_t bytesPerPixel = 4;
	CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast;
	
	if (bpp / bpc < 3) {
		fmt = GL_ALPHA;
		bytesPerPixel = 1;
		bitmapInfo = kCGImageAlphaNone;
	}
	
	dUInteger size = _width * _height * bytesPerPixel;
	void* data = malloc(size);
	
	CGContextRef context = CGBitmapContextCreate(data, _width, _height, 8, _width * bytesPerPixel, colorSpace, bitmapInfo);
	if (_isFlipped) {
		CGContextTranslateCTM(context, 0, _height);
		CGContextScaleCTM(context, 1, -1);
	}
	
	CGRect rect = CGRectMake(0, 0, _width, _height);
	CGContextClearRect(context, rect);
	CGContextDrawImage(context, rect, _cgImage);
	CGContextRelease(context);
	
	glTexImage2D(target, 0, fmt, _width, _height, 0, fmt, GL_UNSIGNED_BYTE, data);
	glSignalIfError();
	
	free(data);
}

//-----------------------------------------------
// iTexture-PVR implementation
//-----------------------------------------------
#define kiPVRTextureFlagTypeMask	0xFF

static const char kPVRTexIdentifier[4] = {'P', 'V', 'R', '!'};

enum {
	kiPVRTextureFlagTypePVRTC_2 = 24,
	kiPVRTextureFlagTypePVRTC_4
};

typedef struct _iSPVRTexHeader {
	uint32_t headerLength;
	uint32_t height;
	uint32_t width;
	uint32_t numMipmaps;
	uint32_t flags;
	uint32_t dataLength;
	uint32_t bpp;
	uint32_t bitmaskRed;
	uint32_t bitmaskGreen;
	uint32_t bitmaskBlue;
	uint32_t bitmaskAlpha;
	uint32_t pvrTag;
	uint32_t numSurfs;
} iSPVRTexHeader;

class _iTexturePVRImp : public _iTextureImp {
public:
	_iTexturePVRImp(const dData* data);
	virtual ~_iTexturePVRImp();
	
	virtual void				createTextureImageByTarget(iETextureTarget target);
	
protected:
	bool						_unpackPVRData();
	
	dData*						_data;
	dArrayT<dData*>*			_imageData;
	
	bool						_hasAlpha;
	GLenum						_internalFmt;
};

_iTexturePVRImp::_iTexturePVRImp(const dData* data) {
	CL_THROW_IF_NIL(data);
	
	_data = const_cast<dData*>(data)->retain();
	_internalFmt = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
	_imageData = CL_NEW(dArrayT<dData*>);
	
	if (!_unpackPVRData()) {
		CL_RELEASE(_data);
		CL_RELEASE(_imageData);
		throw new ExAssert;
	}
}

_iTexturePVRImp::~_iTexturePVRImp() {
	CL_RELEASE(_data);
	CL_RELEASE(_imageData);
}

void
_iTexturePVRImp::createTextureImageByTarget(iETextureTarget target) {
	dUInteger width = _width;
	dUInteger height = _height;
	
	for (dUInteger i = 0, cnt = _imageData->count(); i < cnt; ++i) {
		dData* data = _imageData->objectAtIndex(i);
		dUInteger size = data->length();
		
		glCompressedTexImage2D(target, i, _internalFmt, width, height, 0, size, data->bytes());
		glSignalIfError();
		
		width = __max(width >> 1, 1);
		height = __max(height >> 1, 1);
	}
}

bool
_iTexturePVRImp::_unpackPVRData() {
	iSPVRTexHeader* header = (iSPVRTexHeader*)_data->bytes();
	uint32_t pvrTag = dSwapLittleIntToHost(header->pvrTag);
	
	if (kPVRTexIdentifier[0] != ((pvrTag >> 0) & 0xFF) ||
		kPVRTexIdentifier[1] != ((pvrTag >> 8) & 0xFF) ||
		kPVRTexIdentifier[2] != ((pvrTag >> 16) & 0xFF) ||
		kPVRTexIdentifier[3] != ((pvrTag >> 24) & 0xFF))
	{
		return false;
	}
	
	uint32_t flags = dSwapLittleIntToHost(header->flags);
	uint32_t fmtFlags = kiPVRTextureFlagTypeMask & flags;
	
	if (kiPVRTextureFlagTypePVRTC_2 == fmtFlags || kiPVRTextureFlagTypePVRTC_4 == fmtFlags) {
		_imageData->removeAllObjects();
		
		if (dSwapLittleIntToHost(header->bitmaskAlpha)) _hasAlpha = true;
		else _hasAlpha = false;
		
		if (kiPVRTextureFlagTypePVRTC_4 == fmtFlags) {
			_internalFmt = _hasAlpha ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
		} else if (kiPVRTextureFlagTypePVRTC_2 == fmtFlags) {
			_internalFmt = _hasAlpha ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
		}
		
		uint32_t width = _width = dSwapLittleIntToHost(header->width);
		uint32_t height = _height = dSwapLittleIntToHost(header->height);
		uint32_t dataLength = dSwapLittleIntToHost(header->dataLength);
		uint8_t* bytes = (uint8_t*)_data->bytes() + sizeof(iSPVRTexHeader);
		
		uint32_t dataOffset = 0;
		uint32_t blockSize = 0, widthBlocks = 0, heightBlocks = 0, bpp = 0;
		
		while (dataOffset < dataLength) {
			if (kiPVRTextureFlagTypePVRTC_4 == fmtFlags) {
				blockSize = 4 * 4;
				widthBlocks = width / 4;
				heightBlocks = height / 4;
				bpp = 4;
			} else if (kiPVRTextureFlagTypePVRTC_2 == fmtFlags) {
				blockSize = 8 * 4;
				widthBlocks = width / 8;
				heightBlocks = height / 4;
				bpp = 2;
			}
			
			if (widthBlocks < 2) widthBlocks = 2;
			if (heightBlocks < 2) heightBlocks = 2;
			
			uint32_t dataSize = widthBlocks * heightBlocks * ((blockSize * bpp) / 8);
			_imageData->addObject(dData::dataWithBytesNoCopy(bytes + dataOffset, dataSize, false));
			dataOffset += dataSize;
			
			width = __max(width >> 1, 1);
			height = __max(height >> 1, 1);
		}
		
		_mipmapsCount = _imageData->count();
		
		return true;
	}
	return false;
}

//------------------------------------------------
// iTexture-RGB565 implementation
//------------------------------------------------
class _iTextureRGB565Imp : public _iTextureImp {
public:
	_iTextureRGB565Imp(const dData* data);
	virtual ~_iTextureRGB565Imp();
	
	virtual void	createTextureImageByTarget(iETextureTarget target);
	
protected:
	dData*			_data;
	unsigned short	_bpr;
};

_iTextureRGB565Imp::_iTextureRGB565Imp(const dData* data) {
	CL_THROW_IF_NIL(data);
	
	_data = const_cast<dData*>(data)->retain();
	const void* bytes = data->bytes();
	
	_bpr = dSwapLittleShortToHost(*(unsigned short*)bytes);
	_width = _bpr / sizeof(_bpr);
	_height = (data->length() - sizeof(_bpr)) / _bpr;
}

_iTextureRGB565Imp::~_iTextureRGB565Imp() {
	CL_RELEASE(_data);
}

void
_iTextureRGB565Imp::createTextureImageByTarget(iETextureTarget target) {
	const void* bytes = (dByte*)_data->bytes() + sizeof(_bpr);
	
	glTexImage2D(target, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, bytes);
	glSignalIfError();
}

//-----------------------------------------------
// iTexture-PVRTC implementation
//-----------------------------------------------
class _iTexturePVRTCImp : public _iTextureImp {
public:
	_iTexturePVRTCImp(const dData* data, dUInteger bpp, bool hasAlpha, dUInteger width, dUInteger height);
	virtual ~_iTexturePVRTCImp();
	
	virtual void	createTextureImageByTarget(iETextureTarget target);
	
protected:
	dData*			_data;
	dUInteger		_bpp;
	bool			_hasAlpha;
};

_iTexturePVRTCImp::_iTexturePVRTCImp(const dData* data, dUInteger bpp, bool hasAlpha, dUInteger width, dUInteger height) {
	CL_THROW_IF_NIL(data);
	
	_data = const_cast<dData*>(data)->retain();
	_bpp = bpp;
	_width = width;
	_height = height;
	_hasAlpha = hasAlpha;
}

_iTexturePVRTCImp::~_iTexturePVRTCImp() {
	CL_RELEASE(_data);
}

void
_iTexturePVRTCImp::createTextureImageByTarget(iETextureTarget target) {
	GLenum fmt;
	dUInteger size = _width * _height * _bpp / 8;
	
	if (_hasAlpha) {
		fmt = (4 == _bpp) ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	} else {
		fmt = (4 == _bpp) ? GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
	}
	
	if (size < 32) size = 32;
	
	glCompressedTexImage2D(target, 0, fmt, _width, _height, 0, size, _data->bytes());
	glSignalIfError();
}

//-------------------------------------------
// iTextureData implementation
//-------------------------------------------

END_NAMESPACE_IM


CL_CLASS_DEF(iTextureData, IM)

iTextureData::iTextureData()
: _impl(nil) {
}

iTextureData*
iTextureData::initWithData(const dData* data) {
	_super::init();
	_impl = new _iTextureDataImp(data);
	return this;
}

iTextureData*
iTextureData::initWithCGImage(CGImageRef img, bool isFlipped) {
	_super::init();
	_impl = new _iTextureCGImageImp(img, isFlipped);
	return this;
}

iTextureData*
iTextureData::initWithPVRData(const dData* data) {
	_super::init();
	_impl = new _iTexturePVRImp(data);
	return this;
}

iTextureData*
iTextureData::initWithRGB565Data(const dData* data) {
	_super::init();
	_impl = new _iTextureRGB565Imp(data);
	return this;
}

iTextureData*
iTextureData::initWithPVRTCData(const dData* data, const dDictT<dValue*, dString*>* params) {
	_super::init();
	
	dUInteger bpp = params->objectForKey(_ST("bpp"))->intValue();
	dUInteger width = params->objectForKey(_ST("width"))->intValue();
	dUInteger height = params->objectForKey(_ST("height"))->intValue();
	bool hasAlpha = params->objectForKey(_ST("hasAlpha"))->boolValue();
	
	_impl = new _iTexturePVRTCImp(data, bpp, hasAlpha, width, height);
	return this;
}

iTextureData*
iTextureData::initWithPVRTCData(const dData* data, dUInteger bpp, bool hasAlpha, dUInteger width, dUInteger height) {
	_super::init();
	_impl = new _iTexturePVRTCImp(data, bpp, hasAlpha, width, height);
	return this;
}

void
iTextureData::dealloc() {
	SAFE_DELETE(_impl);
	_super::dealloc();
}

dUInteger
iTextureData::width() const {
	return _impl->width();
}

dUInteger
iTextureData::height() const {
	return _impl->height();
}

dUInteger
iTextureData::mipmapsCount() const {
	return _impl->mipmapsCount();
}

void
iTextureData::createTextureImageByTarget(iETextureTarget target) {
	return _impl->createTextureImageByTarget(target);
}
