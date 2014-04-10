/*
 ** $Id: 2010/07/04, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iMeshPrimitives_H
#define _iMeshPrimitives_H

#include "iMesh.h"

BEGIN_NAMESPACE_IM


class iPlaneMesh : public iMesh {
CL_CLASS_DECL(iPlaneMesh, iMesh)
public:
	static iPlaneMesh*		planeWithRectAndSegmetsCount(dRect rect, dUInteger numXSegs, dUInteger numYSegs);
	virtual iPlaneMesh*		initWithRectAndSegmetsCount(dRect rect, dUInteger numXSegs, dUInteger numYSegs);
};


class iSphereMesh : public iMesh {
CL_CLASS_DECL(iSphereMesh, iMesh)
public:
	static iSphereMesh*		sphereWithRadiusAndSegments(dFloat radius, dUInteger numSegs, dUInteger numStacks);
	virtual iSphereMesh*	initWithRadiusAndSegments(dFloat radius, dUInteger numSegs, dUInteger numStacks);
};

class iCubeMesh : public iMesh {
CL_CLASS_DECL(iCubeMesh, iMesh)
public:
	static iCubeMesh*		cubeWithSideSizeAndSegmentsCount(dFloat size, dUInteger numSegs);
	virtual iCubeMesh*		initWithSideSizeAndSegmentsCount(dFloat size, dUInteger numSegs);
};

END_NAMESPACE_IM

#endif // _iMeshPrimitives_H