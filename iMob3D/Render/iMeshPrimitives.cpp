/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

#include "stdafx.h"
#include "iMeshPrimitives.h"
#include "iIndexBuffer.h"
#include "iVertexBuffer.h"
#include "iMeshPart.h"

#define kTrianglePerPolygon	2
#define kVertexPerTriangle	3
#define kCubeSideCount		6
#define kDefaultFVF		(iEFVF)(kiEFVF_XYZ | kiEFVF_Tex1)

#define FILL_INDEX_BUFFER(ib_ptr_param, IBType, startIdx, numXSegs_param, numYSegs_param) {\
	IBType* ib_ptr = (IBType*)ib_ptr_param;\
	int i_fill, j_fill;\
	for (j_fill = 0; j_fill < numYSegs_param; ++j_fill) {\
		for (i_fill = 0; i_fill < numXSegs_param; ++i_fill) {\
			*ib_ptr++ = (IBType)(startIdx + (i_fill + j_fill * (numXSegs_param + 1)));\
			*ib_ptr++ = (IBType)(startIdx + (i_fill + 1 + (j_fill + 1) * (numXSegs_param + 1)));\
			*ib_ptr++ = (IBType)(startIdx + (i_fill + (j_fill + 1) * (numXSegs_param + 1)));\
			*ib_ptr++ = (IBType)(startIdx + (i_fill + j_fill * (numXSegs_param + 1)));\
			*ib_ptr++ = (IBType)(startIdx + (i_fill + 1 + j_fill * (numXSegs_param + 1)));\
			*ib_ptr++ = (IBType)(startIdx + (i_fill + 1 + (j_fill + 1) * (numXSegs_param + 1)));\
		}\
	}\
	ib_ptr_param = ib_ptr;\
}

CL_CLASS_DEF(iPlaneMesh, IM)

iPlaneMesh*
iPlaneMesh::initWithRectAndSegmetsCount(dRect rect, dUInteger numXSegs, dUInteger numYSegs) {
	dUInteger idxCount = numXSegs * numYSegs * kTrianglePerPolygon * kVertexPerTriangle;
	dUInteger totalSize = idxCount * iIndexBuffer::itemSizeByDataType(kiEDataType_UnsignedShort);
	
	void* const idxPtr = new dByte[totalSize];
	void* iptr = idxPtr;
	FILL_INDEX_BUFFER(iptr, GLushort, 0, numXSegs, numYSegs);
	dData* data = dData::dataWithBytesNoCopy(idxPtr, totalSize, true);
	
	iIndexBuffer* indexBuffer = iIndexBuffer::bufferWithDataAndType(data, kiEDataType_UnsignedShort);
	iMeshPart* meshPart = iMeshPart::meshPartWithParams(indexBuffer, kiEDrawMode_Triangles, 0, idxCount);

	dUInteger vCount = (numXSegs + 1) * (numYSegs + 1);
	totalSize = vCount * iVertexBuffer::itemSizeByFVF(kDefaultFVF);
	dFloat* const vtxPtr = (dFloat*)new dByte[totalSize];
	dFloat* vptr = vtxPtr;

	for (dUInteger j = 0; j <= numYSegs; ++j) {
		dFloat tv = (dFloat)j / (dFloat)numYSegs;
		dFloat y = rect.origin.y + tv * rect.size.height;
		
		for (dUInteger i = 0; i <= numXSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)numXSegs;
			dFloat x = rect.origin.x + tu * rect.size.width;
			
			// vertex
			*vptr++ = x;
			*vptr++ = y;
			*vptr++ = 0.0f;
			
			// texture coord
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}

	data = dData::dataWithBytesNoCopy(vtxPtr, totalSize, true);
	iVertexBuffer* vertexBuffer = iVertexBuffer::bufferWithDataAndFVF(data, kDefaultFVF);
	
	_super::initWithVertexBufferAndPart(vertexBuffer, meshPart, _ST("Plane"));
	
	return this;
}

iPlaneMesh*
iPlaneMesh::planeWithRectAndSegmetsCount(dRect rect, dUInteger numXSegs, dUInteger numYSegs) {
	_self* plane = alloc()->initWithRectAndSegmetsCount(rect, numXSegs, numXSegs);
	return plane ? plane->autorelease() : nil;
}


CL_CLASS_DEF(iSphereMesh, IM)

iSphereMesh*
iSphereMesh::initWithRadiusAndSegments(dFloat radius, dUInteger numSegs, dUInteger numStacks) {
	dUInteger idxCount = numSegs * numStacks * kTrianglePerPolygon * kVertexPerTriangle;
	dUInteger totalSize = idxCount * iIndexBuffer::itemSizeByDataType(kiEDataType_UnsignedShort);
	
	void* const idxPtr = new dByte[totalSize];
	void* iptr = idxPtr;
	FILL_INDEX_BUFFER(iptr, GLushort, 0, numSegs, numStacks);
	dData* data = dData::dataWithBytesNoCopy(idxPtr, totalSize, true);
	
	iIndexBuffer* indexBuffer = iIndexBuffer::bufferWithDataAndType(data, kiEDataType_UnsignedShort);
	iMeshPart* meshPart = iMeshPart::meshPartWithParams(indexBuffer, kiEDrawMode_Triangles, 0, idxCount);
	
	dUInteger vCount = (numSegs + 1) * (numStacks + 1);
	totalSize = vCount * iVertexBuffer::itemSizeByFVF(kDefaultFVF);
	
	dFloat* const vtxPtr = (dFloat*)new dByte[totalSize];
	dFloat* vptr = vtxPtr;
	
	for (dUInteger j = 0; j <= numStacks; ++j) {
		dFloat tv = (dFloat)j / (dFloat)numStacks;
		dFloat z = sinf(M_PI * (tv - 0.5f));
		dFloat xy_r = cosf(M_PI * (tv - 0.5f));
		
		for (dUInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)numSegs;
			dFloat x = xy_r * cosf(M_PI * 2.0f * tu);
			dFloat y = xy_r * sinf(M_PI * 2.0f * tu);
			
			// vertex, y axis
			*vptr++ = radius * x;
			*vptr++ = radius * z;
			*vptr++ = -radius * y;
			
			// texture coord
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}
	
	data = dData::dataWithBytesNoCopy(vtxPtr, totalSize, true);
	iVertexBuffer* vertexBuffer = iVertexBuffer::bufferWithDataAndFVF(data, kDefaultFVF);
	
	_super::initWithVertexBufferAndPart(vertexBuffer, meshPart, _ST("Sphere"));
	
	return this;
}

iSphereMesh*
iSphereMesh::sphereWithRadiusAndSegments(dFloat radius, dUInteger numSegs, dUInteger numStacks) {
	_self* sphere = alloc()->initWithRadiusAndSegments(radius, numSegs, numStacks);
	return sphere ? sphere->autorelease() : nil;
}


CL_CLASS_DEF(iCubeMesh, IM)

iCubeMesh*
iCubeMesh::initWithSideSizeAndSegmentsCount(dFloat size, dUInteger numSegs) {
	dobj_ptr<dArrayT<iMeshPart*> > meshParts(CL_NEW(dArrayT<iMeshPart*>));
	const dUInteger vertexPerSide = (numSegs + 1) * (numSegs + 1);
	dUInteger idxCount = numSegs * numSegs * kTrianglePerPolygon * kVertexPerTriangle;
	dUInteger totalSize = idxCount * iIndexBuffer::itemSizeByDataType(kiEDataType_UnsignedShort);
	
	dUInteger idxVal = 0;
	
	for (dUInteger i = 0; i < kCubeSideCount; ++i) {
		void* const idxPtr = new dByte[totalSize];
		void* iptr = idxPtr;
		
		FILL_INDEX_BUFFER(iptr, GLushort, idxVal, numSegs, numSegs);
		idxVal += vertexPerSide;
	
		dobj_ptr<dData> data(dData::alloc()->initWithBytesNoCopy(idxPtr, totalSize, true));
		dobj_ptr<iIndexBuffer> indexBuffer(iIndexBuffer::alloc()->initWithDataAndType(data.get(), kiEDataType_UnsignedShort));
		dobj_ptr<iMeshPart> meshPart(iMeshPart::alloc()->initWithParams(indexBuffer.get(), kiEDrawMode_Triangles, 0, idxCount));
		meshParts->addObject(meshPart.get());
	}
	
	dUInteger vCount = vertexPerSide * kCubeSideCount;
	totalSize = vCount * iVertexBuffer::itemSizeByFVF(kDefaultFVF);
	
	dFloat* const vtxPtr = (dFloat*)new dByte[totalSize];
	dFloat* vptr = vtxPtr;
	
	for (dInteger k = 0; k <= numSegs; ++k) {
		dFloat tv = (dFloat)k / (dFloat)numSegs;
		dFloat y = tv * size - 0.5f * size;
		
		for (dInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)(numSegs);
			dFloat x = tu * size - 0.5f * size;
			
			*vptr++ = x;
			*vptr++ = y;
			*vptr++ = 0.5f * size;
			
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}
	
	for (dInteger k = 0; k <= numSegs; ++k) {
		dFloat tv = (dFloat)k / (dFloat)numSegs;
		dFloat y = tv * size - 0.5f * size;
		
		for (dInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)(numSegs);
			dFloat z = -tu * size + 0.5f * size;
			
			*vptr++ = 0.5f * size;
			*vptr++ = y;
			*vptr++ = z;
			
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}
	
	for (dInteger k = 0; k <= numSegs; ++k) {
		dFloat tv = (dFloat)k / (dFloat)numSegs;
		dFloat y = tv * size - 0.5f * size;
		
		for (dInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)(numSegs);
			dFloat x = -tu * size + 0.5f * size;
			
			*vptr++ = x;
			*vptr++ = y;
			*vptr++ = -0.5f * size;
			
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}
	
	for (dInteger k = 0; k <= numSegs; ++k) {
		dFloat tv = (dFloat)k / (dFloat)numSegs;
		dFloat y = tv * size - 0.5f * size;
		
		for (dInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)(numSegs);
			dFloat z = tu * size - 0.5f * size;
			
			*vptr++ = -0.5f * size;
			*vptr++ = y;
			*vptr++ = z;
			
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}

	for (dInteger k = 0; k <= numSegs; ++k) {
		dFloat tv = (dFloat)k / (dFloat)numSegs;
		dFloat z = -tv * size + 0.5f * size;
		
		for (dInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)(numSegs);
			dFloat x = tu * size - 0.5f * size;
			
			*vptr++ = x;
			*vptr++ = 0.5f * size;
			*vptr++ = z;
			
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}
	
	for (dInteger k = 0; k <= numSegs; ++k) {
		dFloat tv = (dFloat)k / (dFloat)numSegs;
		dFloat z = tv * size - 0.5f * size;
		
		for (dInteger i = 0; i <= numSegs; ++i) {
			dFloat tu = (dFloat)i / (dFloat)(numSegs);
			dFloat x = tu * size - 0.5f * size;
			
			*vptr++ = x;
			*vptr++ = -0.5f * size;
			*vptr++ = z;
			
			*vptr++ = tu;
			*vptr++ = tv;
		}
	}
	
	dData* data = dData::dataWithBytesNoCopy(vtxPtr, totalSize, true);
	iVertexBuffer* vertexBuffer = iVertexBuffer::bufferWithDataAndFVF(data, kDefaultFVF);
	
	typedef dDictT<iMeshPart*, dString*> MeshPartDict;
	dobj_ptr<MeshPartDict> mparts(CL_NEW(MeshPartDict));
	
	mparts->setObjectForKey(meshParts->objectAtIndex(0), _ST("posZ"));
	mparts->setObjectForKey(meshParts->objectAtIndex(1), _ST("posX"));
	mparts->setObjectForKey(meshParts->objectAtIndex(2), _ST("negZ"));
	mparts->setObjectForKey(meshParts->objectAtIndex(3), _ST("negX"));
	mparts->setObjectForKey(meshParts->objectAtIndex(4), _ST("posY"));
	mparts->setObjectForKey(meshParts->objectAtIndex(5), _ST("negY"));
	
	_super::initWithVertexBufferAndParts(vertexBuffer, mparts.get());
	
	return this;
}

iCubeMesh*
iCubeMesh::cubeWithSideSizeAndSegmentsCount(dFloat size, dUInteger numSegs) {
	_self* cube = alloc()->initWithSideSizeAndSegmentsCount(size, numSegs);
	return cube ? cube->autorelease() : nil;
}