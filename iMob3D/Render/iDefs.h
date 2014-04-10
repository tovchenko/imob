/*
 ** $Id: 2010/05/12, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iDefs_H
#define _iDefs_H

#include <dCocoa.h>

#include <vmmlib/vmmlib.hpp>
using namespace vmml;

#ifndef USE_IM_NAMES
	#define USE_IM_NAMES 1
#endif
#if USE_IM_NAMES
	#define BEGIN_NAMESPACE_IM		USING_NAMESPACE_CL namespace IM {
	#define END_NAMESPACE_IM		}
	#define USING_NAMESPACE_IM		using namespace IM;
	#define IM						iMob
#else
	#define BEGIN_NAMESPACE_IM
	#define END_NAMESPACE_IM
	#define USING_NAMESPACE_IM
	#define IM
#endif

typedef struct _iSTexHeader {
	uint32_t headerLength;
	uint32_t height;
	uint32_t width;
	uint32_t numMipmaps;
	uint32_t format;
	uint32_t dataLength;
	uint32_t tag;
} iSTexHeader;

typedef enum _iEDataType {
	kiEDataType_Byte			= 0x1400, // GL_BYTE
	kiEDataType_UnsignedByte	= 0x1401, // GL_UNSIGNED_BYTE
	kiEDataType_Short			= 0x1402, // GL_SHORT
	kiEDataType_UnsignedShort	= 0x1403, // GL_UNSIGNED_SHORT
	kiEDataType_Int				= 0x1404, // GL_INT
	kiEDataType_UnsignedInt		= 0x1405, // GL_UNSIGNED_INT
	kiEDataType_Float			= 0x1406, // GL_FLOAT
	kiEDataType_Fixed			= 0x140C  // GL_FIXED
} iEDataType;

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


typedef enum _iEColorFormat {
	kiEColorFormat_Unknown		= 0,

	kiEColorFormat_R8G8B8		= 20,
	kiEColorFormat_R8G8B8A8		= 21,
	kiEColorFormat_R5G6B5		= 23,
	kiEColorFormat_R5G5B5A1		= 25,
	kiEColorFormat_R4G4B4A4		= 26,
	kiEColorFormat_A8			= 28,

	kiEColorFormat_L8           = 50,
	kiEColorFormat_L8A8			= 51,

	kiEColorFormat_ForceDword	= 0x7FFFFFFF
} iEColorFormat;

typedef enum _iEContextOrientation {
	kiEContextOrientationUnknown,
	kiEContextOrientationPortrait,
	kiEContextOrientationLandscapeLeft,
	kiEContextOrientationLandscapeRight,
	kiEContextOrientationPortraitUpsideDown
} iEContextOrientation;

typedef enum _iEBufferType {
	kiEBufferType_Array			= 0x8892, // GL_ARRAY_BUFFER,
	kiEBufferType_ElementArray	= 0x8893  // GL_ELEMENT_ARRAY_BUFFER
} iEBufferType;

typedef enum _iETextureTarget {
	kiETextureTarget_2D					= 0x0DE1, // GL_TEXTURE_2D,
	kiETextureTarget_Texture			= 0x1702, // GL_TEXTURE,
	kiETextureTarget_CubeMap			= 0x8513, // GL_TEXTURE_CUBE_MAP,
	kiETextureTarget_BindingCubeMap		= 0x8514, // GL_TEXTURE_BINDING_CUBE_MAP,
	kiETextureTarget_CubeMapPositiveX	= 0x8516, // GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	kiETextureTarget_CubeMapNegativeX	= 0x8516, // GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	kiETextureTarget_CubeMapPositiveY	= 0x8517, // GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	kiETextureTarget_CubeMapNegativeY	= 0x8518, // GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	kiETextureTarget_CubeMapPositiveZ	= 0x8519, // GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	kiETextureTarget_CubeMapNegativeZ	= 0x851A, // GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	kiETextureTarget_MaxCubeMapSize		= 0x851C  // GL_MAX_CUBE_MAP_TEXTURE_SIZE
} iETextureTarget;

typedef enum _iEDrawMode {
	kiEDrawMode_Points			= 0x0000, // GL_POINTS,
	kiEDrawMode_Lines			= 0x0001, // GL_LINES,
	kiEDrawMode_LineLoop		= 0x0002, // GL_LINE_LOOP,
	kiEDrawMode_LineStrip		= 0x0003, // GL_LINE_STRIP,
	kiEDrawMode_Triangles		= 0x0004, // GL_TRIANGLES,
	kiEDrawMode_TriangleStrip	= 0x0005, // GL_TRIANGLE_STRIP,
	kiEDrawMode_TriangleFan		= 0x0006  // GL_TRIANGLE_FAN
} iEDrawMode;

typedef enum _iETextureFilter {
	kiETextureFilter_Nearest				= 0x2600, // GL_NEAREST,
	kiETextureFilter_Linear					= 0x2601, // GL_LINEAR,
	kiETextureFilter_NearestMipmapNearest	= 0x2700, // GL_NEAREST_MIPMAP_NEAREST,
	kiETextureFilter_LinearMipmapNearest	= 0x2701, // GL_LINEAR_MIPMAP_NEAREST,
	kiETextureFilter_NearestMipmapLinear	= 0x2702, // GL_NEAREST_MIPMAP_LINEAR,
	kiETextureFilter_LinearMipmapLinear		= 0x2703  // GL_LINEAR_MIPMAP_LINEAR
} iETextureFilter;

#endif // _iDefs_H
