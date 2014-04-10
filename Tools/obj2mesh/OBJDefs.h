/*
 *  Utils.h
 *  obj2mesh
 *
 *  Created by DoctorSet on 8/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdint.h>


enum { meshSignature = 'msh!' };
enum { physSignature = 'cld!' };

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

typedef struct _iSPhysHeader {
	uint32_t	headerLength;
	uint32_t	vbDataSize;
	uint32_t	ibTotalDataSize;
	uint32_t	tag;
} iSPhysHeader;

// Flexible Vertex Format (from D3D)
typedef enum _iEFVF {
	kiEFVF_Reserved0		= 0x001,
	kiEFVF_PositionMask		= 0x400E,
	kiEFVF_XYZ				= 0x002,
	//	kFVF_XYZRHW				0x004,
	//	kFVF_XYZB1				0x006,
	//	kFVF_XYZB2				0x008,
	//	kFVF_XYZB3				0x00A,
	//	kFVF_XYZB4				0x00C,
	//	kFVF_XYZB5				0x00E,
	//	kFVF_XYZW				0x4002,
	kiEFVF_Normal			= 0x010,
	//	kFVF_PSIZE				0x020,
	kiEFVF_Diffuse			= 0x040,
	//	kiEFVF_Specular			= 0x080,
	
	kiEFVF_TexCountMask		= 0xF00,
	kiEFVF_TexCountShift	= 8,
	kiEFVF_Tex0				= 0x000,
	kiEFVF_Tex1				= 0x100,
	kiEFVF_Tex2				= 0x200,
	kiEFVF_Tex3				= 0x300,
	kiEFVF_Tex4				= 0x400,
	kiEFVF_Tex5				= 0x500,
	kiEFVF_Tex6				= 0x600,
	kiEFVF_Tex7				= 0x700,
	kiEFVF_Tex8				= 0x800,
	
	//	kFVF_LASTBETA_UBYTE4	0x1000,
	//	kFVF_LASTBETA_COLOR		0x8000,
	
	//	kiEFVFReserved2			0x6000,  // 2 reserved bits
	
	kiEFVF_TextureFormat1	= 3,	// one floating point value
	kiEFVF_TextureFormat2	= 0,	// two floating point values
	kiEFVF_TextureFormat3	= 1,	// three floating point values
	kiEFVF_TextureFormat4	= 2		// four floating point values
} iEFVF;


#define iEFVF_TEXCOORDSIZEN(coordIndex) 
#define iEFVF_TEXCOORDSIZE1(coordIndex)	(kiEFVF_TextureFormat1 << (2 * (coordIndex) + 16)) 
#define iEFVF_TEXCOORDSIZE2(coordIndex)	(kiEFVF_TextureFormat2) 
#define iEFVF_TEXCOORDSIZE3(coordIndex)	(kiEFVF_TextureFormat3 << (2 * (coordIndex) + 16)) 
#define iEFVF_TEXCOORDSIZE4(coordIndex)	(kiEFVF_TextureFormat4 << (2 * (coordIndex) + 16))

#define iEFVF_GET_TEXCOORDSIZE(fvf, coordIndex)	((fvf & iEFVF_TEXCOORDSIZE1(coordIndex)) ? 1 :	\
												((fvf & iEFVF_TEXCOORDSIZE3(coordIndex)) ? 3 :	\
												((fvf & iEFVF_TEXCOORDSIZE4(coordIndex)) ? 4 : 2 )))

typedef struct {
	float	x;
	float	y;
} Vec2;

typedef struct {
	float	x;
	float	y;
	float	z;
} Vec3;

void	vec3_normalize(Vec3* a);
void	vec3_cross(Vec3* a, const Vec3* v1, const Vec3* v2);