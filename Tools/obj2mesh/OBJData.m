//
//  OBJData.m
//  obj2mesh
//
//  Created by DoctorSet on 8/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

/*
 *	.mesh file format can be contains only one FVF for all mesh parts,
 *	but .obj file format can contains FVF for each mesh part.
 *	This parser will be choose most simple FVF from .obj and than
 *	apply it to .mesh
 */

#import "OBJData.h"
#import "NSScannerExt.h"
#import "VertexIndex.h"

static NSString*	_uniqueKeyForIndex(VertexIndex* idx, BOOL useNormal, BOOL useTexCoord);
static iEFVF		_fvfForIndex(VertexIndex* idx, BOOL useNormal, BOOL useTexCoord);

@interface OBJData ()
- (void)_parseObjFileIfNeeded;
- (void)_parseFile:(NSString*)path fileLineScannerCallback:(SEL)callback;
- (void)_setCurrentObject:(NSString*)objectName;
- (void)_setDefaultName;
- (void)_parseLineWithScanner:(NSScanner*)scanner;
@end



@implementation OBJData

+ (id)dataWithPath:(NSString*)path {
	return [[[self alloc] initWithPath:path] autorelease];
}

- (id)initWithPath:(NSString*)path {
	if ((self = [super init])) {
		_objFilePath = [path retain];
	}
	
	return self;
}

- (void)dealloc {
	[_meshParts release];
	
	[_vertexBuffer release];
	[_tCoordBuffer release];
	[_normalBuffer release];
	
	[_objFilePath release];
	
	[super dealloc];
}

- (NSData*)composedRenderGeometryWithNormals:(BOOL)storeNormals textureCoords:(BOOL)storeTexCoords {
	[self _parseObjFileIfNeeded];
	
	NSMutableData* resIB = [NSMutableData data];
	NSMutableData* resVB = [NSMutableData data];
	NSMutableData* resObjs = [NSMutableData data];
	NSMutableDictionary* indicesRemap = [NSMutableDictionary dictionary];
	__block iEFVF fvf = -1; // mask 0x11111111
	__block ushort currentIndex = 0;
	
	[_meshParts enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL* stop) {
		NSString* objName = key;
		NSArray* indices = obj;
		
		ushort first = resIB.length / sizeof(unsigned short);
		ushort count = indices.count;

		const Vec3* vb = _vertexBuffer.bytes;
		const Vec3* nb = _normalBuffer.bytes;
		const Vec2* tb = _tCoordBuffer.bytes;
		
		for (VertexIndex* vi in indices) {
			NSString* key = _uniqueKeyForIndex(vi, storeTexCoords, storeNormals);
			fvf &= _fvfForIndex(vi, storeNormals, storeTexCoords);
			
			NSNumber* number = [indicesRemap objectForKey:key];
			ushort index = 0;
			
			if (number) {
				index = [number integerValue];
			} else {
				Vec3 v = vb[vi.vertex];
				[resVB appendBytes:&v length:sizeof(v)];
				
				if (storeNormals && vi.hasNormal) {
					Vec3 n = nb[vi.normal];
					vec3_normalize(&n);
					[resVB appendBytes:&n length:sizeof(n)];
				}
				
				if (storeTexCoords && vi.hasTexCoord) {
					Vec2 t = tb[vi.texCoord];
					[resVB appendBytes:&t length:sizeof(t)];
					
					if (storeNormals && vi.hasNormal) {
						Vec3 n = nb[vi.normal];
						Vec3 tangent, tmp = { n.x + 1.0f, n.y, n.z };
						vec3_cross(&tangent, &tmp, &n);
						vec3_normalize(&tangent);
						
						[resVB appendBytes:&tangent length:sizeof(tangent)];
					}
				}
				
				index = currentIndex++;
				[indicesRemap setObject:[NSNumber numberWithInteger:index] forKey:key];
			}
			
			[resIB appendBytes:&index length:sizeof(index)];
		}
		
		ushort nameLength = [objName length];
		iSMeshPartInfo info = { first, count, nameLength };
		[resObjs appendBytes:&info length:sizeof(info)];
		[resObjs appendBytes:[objName cStringUsingEncoding:NSUnicodeStringEncoding] length:2 * sizeof(char) * nameLength];
	}];
	
	iSMeshHeader h;
	h.headerLength = CFSwapInt32HostToLittle(sizeof(h));
	h.fvf = CFSwapInt32HostToLittle(fvf);
	h.ibDataSize = CFSwapInt32HostToLittle(resIB.length);
	h.vbDataSize = CFSwapInt32HostToLittle(resVB.length);
	h.meshPartsDataSize = CFSwapInt32HostToLittle(resObjs.length);
	h.tag = CFSwapInt32HostToLittle(meshSignature);
	
	NSMutableData* result = [NSMutableData data];
	[result appendBytes:&h length:sizeof(h)];
	[result appendData:resIB];
	[result appendData:resVB];
	[result appendData:resObjs];
	
	printf("=== Render Geometry ===\n");
	printf("Verteces count = %d\n", (int)(resVB.length / (sizeof(Vec3) + sizeof(Vec2))));
	printf("Indices count = %d\n", (int)(resIB.length / sizeof(ushort)));
	printf("Faces count = %d\n", (int)(resIB.length / 3 * sizeof(ushort)));
	printf("=======================\n\n");
	
	return result;
}

- (NSData*)composedPhysGeometry {
	[self _parseObjFileIfNeeded];
	
	NSMutableData*	resIBs = [NSMutableData data];
	__block NSInteger totalIndicesCount = 0;
	
	[_meshParts enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL* stop) {
		NSArray* indices = obj;
		NSMutableData* ib = [NSMutableData data];
		
		for (VertexIndex* vi in indices) {
			uint32_t vIndex = vi.vertex;
			[ib appendBytes:&vIndex length:sizeof(vIndex)];
		}
		
		uint32_t ibDataSize = ib.length;
		[resIBs appendBytes:&ibDataSize length:sizeof(ibDataSize)];
		[resIBs appendData:ib];
		
		totalIndicesCount += ib.length / sizeof(uint32_t);
	}];
	
	iSPhysHeader h;
	h.headerLength = CFSwapInt32HostToLittle(sizeof(h));
	h.vbDataSize = CFSwapInt32HostToLittle(_vertexBuffer.length);
	h.ibTotalDataSize = CFSwapInt32HostToLittle(resIBs.length);
	h.tag = CFSwapInt32HostToLittle(physSignature);
	
	NSMutableData* result = [NSMutableData data];
	[result appendBytes:&h length:sizeof(h)];
	[result appendData:_vertexBuffer];
	[result appendData:resIBs];
	
	printf("=== Phys Geometry ===\n");
	printf("Verteces count = %d\n", (int)(_vertexBuffer.length / sizeof(Vec3)));
	printf("Total indices count = %d\n", (int)(totalIndicesCount));
	printf("Total faces count = %d\n", (int)(totalIndicesCount / 3));
	printf("=====================\n\n");
	
	return result;
}

#pragma mark -
#pragma mark [Private]

- (void)_parseObjFileIfNeeded {
	if (_vertexBuffer) return;
	_vertexBuffer = [NSMutableData new];
	
	[self _parseFile:_objFilePath fileLineScannerCallback:@selector(_parseLineWithScanner:)];
	
	_currentIndices = nil;
}

- (void)_parseFile:(NSString*)path fileLineScannerCallback:(SEL)callback {
	NSAutoreleasePool* pool = [NSAutoreleasePool new];
	
	NSString* str = [NSString stringWithContentsOfFile:path encoding:NSMacOSRomanStringEncoding error:nil];
	NSArray* lines = [str componentsSeparatedByString:@"\r\n"];
	if (1 == lines.count) lines = [str componentsSeparatedByString:@"\n"];
	if (1 == lines.count) lines = [str componentsSeparatedByString:@"\r"];
	
	for (NSString* line in lines) {
		NSScanner* scanner = [NSScanner scannerWithString:line];
		[self performSelector:callback withObject:scanner];
	}
	
	[pool drain];
}

- (void)_setCurrentObject:(NSString*)objectName {
	_currentIndices = [_meshParts objectForKey:objectName];
	if (!_currentIndices) {
		_currentIndices = [NSMutableArray array];
		if (!_meshParts) _meshParts = [NSMutableDictionary new];
		[_meshParts setObject:_currentIndices forKey:objectName];
	}
}

- (void)_setDefaultName {
	if (_meshParts) return;
	
	_currentIndices = [NSMutableData data];
	_meshParts = [NSMutableDictionary new];
	[_meshParts setObject:_currentIndices
				   forKey:[[_objFilePath lastPathComponent] stringByDeletingPathExtension]];
}

#pragma mark -
#pragma mark [Scanner Callbacks]

- (void)_parseLineWithScanner:(NSScanner*)scanner {
	if ([scanner scanString:@"v " intoString:NULL]) {
		Vec3 v;
		BOOL res = [scanner scanVector3:&v];
		NSAssert1(res && [scanner isAtEnd], @"Broken vertex: %@", [scanner string]);
		[_vertexBuffer appendBytes:&v length:sizeof(v)];
	} else if ([scanner scanString:@"vt " intoString:NULL]) {
		Vec2 v;
		BOOL res = [scanner scanVector2:&v];
		NSAssert1(res && [scanner isAtEnd], @"Broken texture coord: %@", [scanner string]);
		if (!_tCoordBuffer) _tCoordBuffer = [NSMutableData new];
		[_tCoordBuffer appendBytes:&v length:sizeof(v)];
	} else if ([scanner scanString:@"vn " intoString:NULL]) {
		Vec3 v;
		BOOL res = [scanner scanVector3:&v];
		NSAssert1(res && [scanner isAtEnd], @"Broken normal: %@", [scanner string]);
		if (!_normalBuffer) _normalBuffer = [NSMutableData new];
		[_normalBuffer appendBytes:&v length:sizeof(v)];
	} else if ([scanner scanString:@"g default" intoString:NULL]) {
		// skip
	} else if ([scanner scanString:@"g " intoString:NULL]) {
		NSString* objectName = [[scanner string] substringFromIndex:[scanner scanLocation]];
		[self _setCurrentObject:objectName];
	} else if ([scanner scanString:@"f " intoString:NULL]) {
		if (!_currentIndices && _vertexBuffer)
			[self _setDefaultName];
			
		for (NSInteger i = 0; i < 3; ++i) {
			VertexIndex* idx = [scanner scanVertexIndex];
			NSAssert1(idx, @"Broken face in file %@", [_objFilePath lastPathComponent]);
			[_currentIndices addObject:idx];
		}
		
		NSAssert1([scanner isAtEnd], @"Broken face in file %@", [_objFilePath lastPathComponent]);
	}
}

@end


static NSString*
_uniqueKeyForIndex(VertexIndex* idx, BOOL useNormal, BOOL useTexCoord) {
	NSCAssert(idx, @"VertexIndex == nil");
	useTexCoord = useTexCoord && idx.hasTexCoord;
	useNormal = useNormal && idx.hasNormal;
	
	if (useTexCoord && useNormal)
		return [NSString stringWithFormat:@"%d_%d_%d", idx.vertex, idx.texCoord, idx.normal];
	else if (useTexCoord && !useNormal)
		return [NSString stringWithFormat:@"%d_%d", idx.vertex, idx.texCoord];
	else if (useNormal && !useTexCoord)
		return [NSString stringWithFormat:@"%d_nan_%d", idx.vertex, idx.normal];
	else return [NSString stringWithFormat:@"%d", idx.vertex];
}

static iEFVF
_fvfForIndex(VertexIndex* idx, BOOL useNormal, BOOL useTexCoord) {
	NSCAssert(idx, @"VertexIndex == nil");
	useTexCoord = useTexCoord && idx.hasTexCoord;
	useNormal = useNormal && idx.hasNormal;
	
	iEFVF result = kiEFVF_XYZ;
	if (useNormal) result |= kiEFVF_Normal;
	if (useTexCoord) {
		if (useNormal) result |= (kiEFVF_Tex2 | iEFVF_TEXCOORDSIZE3(1));
		else result |= kiEFVF_Tex1;
	}
	return result;
}
