/*
 ** $Id: 2010/08/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iResLoader.h"
#include "iIndexBuffer.h"
#include "iVertexBuffer.h"
#include "iMeshPart.h"
#include "iMesh.h"
#include "iTexture.h"
#include "iTextureData.h"

#define kCubeSideCount	6

enum { kiMeshSignature = 'msh!' };

typedef struct _iSMeshHeader {
	uint32_t	headerLength;
	uint32_t	fvf;
	uint32_t	ibDataSize;
	uint32_t	vbDataSize;
	uint32_t	meshPartsDataSize;
	uint32_t	tag;
} iSMeshHeader;

typedef struct _iSMeshPartInfo {
	unsigned short	first;
	unsigned short	count;
	unsigned short	nameLength;
} iSMeshPartInfo;

static iTextureData*		_texDataFromPath(dString* filePath, const dDictT<dValue*, dString*>* pvrtcParams);

CL_CLASS_DEF_AS_SINGLETON(iResLoader, IM)

iMesh*
iResLoader::loadMesh(dString* filePath) {
	CL_THROW_IF_NIL(filePath);
	
	dData* meshData = dData::dataWithContentsOfFile(filePath);
	const dByte* meshBytes = (const dByte*)meshData->bytes();
	
	iSMeshHeader* h = (iSMeshHeader*)meshBytes;
	assert(dSwapLittleIntToHost(h->tag) == kiMeshSignature);
	assert(dSwapLittleIntToHost(h->headerLength) +
		   dSwapLittleIntToHost(h->ibDataSize) +
		   dSwapLittleIntToHost(h->vbDataSize) +
		   dSwapLittleIntToHost(h->meshPartsDataSize) == meshData->length());
	
	meshBytes += dSwapLittleIntToHost(h->headerLength);
	dobj_ptr<dData> ibData(dData::alloc()->initWithBytes(meshBytes, h->ibDataSize));
	dobj_ptr<iIndexBuffer> ib(iIndexBuffer::alloc()->initWithDataAndType(ibData.get(), kiEDataType_UnsignedShort));
	
	meshBytes += dSwapLittleIntToHost(h->ibDataSize);
	dobj_ptr<dData> vbData(dData::alloc()->initWithBytes(meshBytes, h->vbDataSize));
	dobj_ptr<iVertexBuffer> vb(iVertexBuffer::alloc()->initWithDataAndFVF(vbData.get(), (iEFVF)dSwapLittleIntToHost(h->fvf)));
	
	meshBytes += dSwapLittleIntToHost(h->vbDataSize);
	dobj_ptr<dData> mpData(dData::alloc()->initWithBytes(meshBytes, h->meshPartsDataSize));
	const dByte* mpPtr = (const dByte*)mpData->bytes();
	dUInteger dataLength = 0;
	
	dobj_ptr<dDictT<iMeshPart*, dString*> > parts((dDictT<iMeshPart*, dString*>*)
													dDictT<iMeshPart*, dString*>::alloc()->init()); 
	
	while (dataLength < h->meshPartsDataSize) {
		iSMeshPartInfo* partInfo = (iSMeshPartInfo*)mpPtr;
		dobj_ptr<iMeshPart> part(iMeshPart::alloc()->initWithParams(ib.get(), kiEDrawMode_Triangles, partInfo->first, partInfo->count));
		mpPtr += sizeof(iSMeshPartInfo);
		dataLength += sizeof(iSMeshPartInfo);
		
		dobj_ptr<dString> name(dString::alloc()->initWithCharacters((TCHAR*)mpPtr, partInfo->nameLength));
		mpPtr += partInfo->nameLength * sizeof(TCHAR);
		dataLength += partInfo->nameLength * sizeof(TCHAR);
		
		parts->setObjectForKey(part.get(), name.get());
	}
	
	return iMesh::meshWithVertexBufferAndParts(vb.get(), parts.get());
}

iTexture2D*
iResLoader::loadTexture2D(dString* filePath, const dDictT<dValue*, dString*>* pvrtcParams) {
	return iTexture2D::alloc()->initWithTextureData(_texDataFromPath(filePath, pvrtcParams))->autorelease();
}

iTextureCube*
iResLoader::loadTextureCube(dArrayT<dString*>* filePaths, const dArrayT<dDictT<dValue*, dString*>*>* pvrtcParams) {
	CL_THROW_IF_NIL(filePaths);
	CL_THROW_IF_TRUE(filePaths->count() != kCubeSideCount);
	
	typedef dDictT<dValue*, dString*> PvrtcData;
	
	dobj_ptr<dArrayT<iTextureData*> > texData(CL_NEW(dArrayT<iTextureData*>));
	
	for (dUInteger i = 0, cnt = filePaths->count(); i < cnt; ++i) {
		PvrtcData* pvrtc = nil;
		if (pvrtcParams && pvrtcParams->count() > i)
			pvrtc = pvrtcParams->objectAtIndex(i);
		pvrtc = (CL_NULL(PvrtcData) != pvrtc) ? pvrtc : nil;
		
		texData->addObject(_texDataFromPath(filePaths->objectAtIndex(i), pvrtc));
	}
	
	return iTextureCube::alloc()->initWithTextureDataArray(texData.get())->autorelease();
}


static iTextureData*
_texDataFromPath(dString* filePath, const dDictT<dValue*, dString*>* pvrtcParams) {
	CL_THROW_IF_NIL(filePath);
	
	dobj_ptr<dData> data(dData::alloc()->initWithContentsOfFile(filePath));
	iTextureData* texData = nil;
	
	if (filePath->pathExtension()->isEqual(_ST("pvr"))) texData = iTextureData::alloc()->initWithPVRData(data.get());
	else if (filePath->pathExtension()->isEqual(_ST("tex"))) texData = iTextureData::alloc()->initWithData(data->uncompressedData());
	else if (filePath->pathExtension()->isEqual(_ST("pvrtc"))) texData = iTextureData::alloc()->initWithPVRTCData(data.get(), pvrtcParams);
	else if (filePath->pathExtension()->isEqual(_ST("565"))) texData = iTextureData::alloc()->initWithRGB565Data(data.get());
	else throw new ExAssert;
	
	return texData ? texData->autorelease() : nil;
}
