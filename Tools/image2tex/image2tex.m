#import <Foundation/Foundation.h>

#import "TexDefs.h"
#import "NSDataExt.h"

static iEColorFormat		colorFormatByString(NSString* formatName);
static NSDictionary*		colorFormatRemap();
static NSData*				convertedImageAtPath(NSString* path, iEColorFormat format, BOOL generateMipmaps, BOOL alphaNonpremul);
static NSData*				combinedImagesAtPaths(NSString* path1, NSString* path2, BOOL generateMipmaps);
static NSData*				convertBitmap(NSBitmapImageRep* rep, iEColorFormat format, BOOL generateMipmaps, BOOL alphaNonpremul);
static NSBitmapImageRep*	bitmapRGBAFromCombiningRGBWithGrayBitmaps(NSBitmapImageRep* repRGB, NSBitmapImageRep* repGray);
static NSData*				textureDataFromBitmap(NSBitmapImageRep* rep, iEColorFormat format, BOOL generateMipmaps);
static void					premulAlphaIfNeeded(NSBitmapImageRep* rep);
static void					glColorFormatFrom_iEColorFormat(iEColorFormat iFormat, GLenum* oFmt, GLenum* oType);
static NSInteger			pixelByteSizeFrom_iEColorFormat(iEColorFormat iFormat);
static GLenum				internalTextureFormatFrom_iEColorFormat(iEColorFormat iFormat);
static void					sourceFormatFromBitmap(NSBitmapImageRep* rep, GLenum* oFmt, GLenum* oType);
static NSInteger			mipmapsCount(NSInteger width, NSInteger height);
static void					_createDirectoriesIfNeeded(NSString* path);

static NSDictionary*		_sColorFormatRemap = nil;

int
main (int argc, const char* argv[]) {
    NSAutoreleasePool* pool = [NSAutoreleasePool new];
	
	BOOL generateMipmaps = NO;
	BOOL alphaNonpremul = NO;
	BOOL isOutputArg = NO;
	BOOL isFormatArg = NO;
	BOOL isFirstForm = NO;
	iEColorFormat format = kiEColorFormat_R8G8B8A8;
	NSString* sourcePath = nil;
	NSString* sourceGrayPath = nil;
	NSString* outputPath = nil;
	BOOL isDir1 = NO;
	BOOL isDir2 = NO;
	BOOL breaked = NO;
	
	for (NSInteger argi = 1; argi < argc; ++argi) {
		NSString* arg = [NSString stringWithCString:argv[argi] encoding:NSMacOSRomanStringEncoding];
		
		if ([arg hasPrefix:@"-"]) {
			NSString* option = [arg substringFromIndex:1];
			
			if ([option isEqualToString:@"m"]) generateMipmaps = YES;
			else if ([option isEqualToString:@"nonpremul"]) alphaNonpremul = YES;
			else if ([option isEqualToString:@"o"]) isOutputArg = YES;
			else if ([option isEqualToString:@"f"]) isFormatArg = YES;
		} else {
			if (isOutputArg && isFormatArg) {
				printf("Wrong usage, missed argument!\n");
				breaked = YES;
				break;
			}
			
			if (isOutputArg) {
				if (!sourceGrayPath) isFirstForm = YES;
				if (!outputPath) {
					[[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir2];
					outputPath = arg;
				}
				isOutputArg = NO;
			} else if (isFormatArg) {
				isFirstForm = YES;
				format = colorFormatByString([arg lowercaseString]);
				isFormatArg = NO;
				if (!format) {
					printf("Wrong usage, incorrect color format!\n");
					breaked = YES;
					break;
				}
			} else if (!sourcePath && [[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir1]) {
				sourcePath = arg;
			} else if (!sourceGrayPath && [[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir2]) {
				isFirstForm = NO;
				sourceGrayPath = arg;
			} else {
				printf("Wrong usage!\n");
				breaked = YES;
				break;
			}
		}
	}
	
	BOOL isCorectForm1 = (sourcePath && outputPath) && ((isDir1 && isDir2) || (!isDir1 && !isDir2)) && isFirstForm;
	BOOL isCorectForm2 = (sourcePath && outputPath && sourceGrayPath) && !isDir1 && !isDir2 && !isFirstForm;
	
	if ((isCorectForm1 || isCorectForm2) && !breaked) {
		if (isCorectForm1) {
			if (isDir1 && isDir2) {
				NSArray* fileNames = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:sourcePath error:nil];
				for (NSString* fileName in fileNames) {
					if ([fileName hasPrefix:@"."]) continue; // skip hidden files
					
					NSAutoreleasePool* pool2 = [NSAutoreleasePool new];
					
					NSString* filePath = [sourcePath stringByAppendingPathComponent:fileName];
					NSData* imgData = convertedImageAtPath(filePath, format, generateMipmaps, alphaNonpremul);
					fileName = [[fileName stringByDeletingPathExtension] stringByAppendingPathExtension:@"tex"];
					[imgData writeToFile:[outputPath stringByAppendingPathComponent:fileName] atomically:YES];
					
					[pool2 drain];
				}
			} else if (!isDir1 && !isDir2) {
				_createDirectoriesIfNeeded([outputPath stringByDeletingLastPathComponent]);
				NSData* imgData = convertedImageAtPath(sourcePath, format, generateMipmaps, alphaNonpremul);
				[imgData writeToFile:outputPath atomically:YES];
			}
		} else if (isCorectForm2) {
			_createDirectoriesIfNeeded([outputPath stringByDeletingLastPathComponent]);
			NSData* imgData = combinedImagesAtPaths(sourcePath, sourceGrayPath, generateMipmaps);
			[imgData writeToFile:outputPath atomically:YES];
		}
	} else {
		printf("Usage:\n");
		printf("\t image2tex [-m] [-nonpremul] -o dst_path -f format src_path\n");
		printf("\t image2tex [-m] path1 path2 -o dst_path\n\n");
		printf("-m \t\t generate mipmaps\n");
		printf("-nonpremul \t alpha is not premultiplied, ignored for format without alpha\n");
		printf("format \t possible format values:\n\n");
		
		NSArray* formats = [colorFormatRemap() allKeys];
		for (NSString* name in formats)
			printf("\t\t\t\t %s\n", [name cStringUsingEncoding:NSMacOSRomanStringEncoding]);
		
		printf("\npath \t\t path to image or folder with images\n");
		printf("path1 \t\t path to image in RGB format\n");
		printf("path2 \t\t path to image in Gray format\n\n");
	}

	[_sColorFormatRemap release];
    [pool drain];
    return 0;
}


typedef struct _SColorRGBA {
	UInt8	r;
	UInt8	g;
	UInt8	b;
	UInt8	a;
} SColorRGBA;

typedef struct _SColorRGB {
	UInt8	r;
	UInt8	g;
	UInt8	b;
} SColorRGB;

typedef struct _SColorLA {
	UInt8	l;
	UInt8	a;
} SColorLA;

typedef struct _SColorA {
	UInt8	a;
} SColorA;


static iEColorFormat
colorFormatByString(NSString* formatName) {
	return [[colorFormatRemap() objectForKey:formatName] integerValue];
}

static NSDictionary*
colorFormatRemap() {
	if (!_sColorFormatRemap) {
		_sColorFormatRemap = [[NSDictionary alloc] initWithObjectsAndKeys:
							  [NSNumber numberWithInteger:kiEColorFormat_R8G8B8],	@"rgb8",
							  [NSNumber numberWithInteger:kiEColorFormat_R8G8B8A8], @"rgba8",
							  [NSNumber numberWithInteger:kiEColorFormat_R5G6B5],	@"rgb565",
							  [NSNumber numberWithInteger:kiEColorFormat_R5G5B5A1], @"rgb5a1",
							  [NSNumber numberWithInteger:kiEColorFormat_R4G4B4A4], @"rgba4",
							  [NSNumber numberWithInteger:kiEColorFormat_A8],		@"a8",
							  [NSNumber numberWithInteger:kiEColorFormat_L8],		@"l8",
							  [NSNumber numberWithInteger:kiEColorFormat_L8A8],		@"l8a8",
							  nil];
	}
	
	return _sColorFormatRemap;
}

static NSData*
convertedImageAtPath(NSString* path, iEColorFormat format, BOOL generateMipmaps, BOOL alphaNonpremul) {
	NSData* imageData = [NSData dataWithContentsOfFile:path];
	NSBitmapImageRep* rep = [NSBitmapImageRep imageRepWithData:imageData];
	NSData* outData = nil;
	
	if (rep) {
		printf("Process image at path %s\n", [path cStringUsingEncoding:NSMacOSRomanStringEncoding]);
		outData = convertBitmap(rep, format, generateMipmaps, alphaNonpremul);
		printf("Done.\n");
	}
	
	return outData;
}

static NSData*
combinedImagesAtPaths(NSString* path1, NSString* path2, BOOL generateMipmaps) {
	NSBitmapImageRep* rep1 = [NSBitmapImageRep imageRepWithData:[NSData dataWithContentsOfFile:path1]];
	NSBitmapImageRep* rep2 = [NSBitmapImageRep imageRepWithData:[NSData dataWithContentsOfFile:path2]];
	NSData* outData = nil;
	
	if (rep1 && rep2) {
		printf("Process images:\n\t%s\n\t%s\n",
			   [path1 cStringUsingEncoding:NSMacOSRomanStringEncoding],
			   [path2 cStringUsingEncoding:NSMacOSRomanStringEncoding]);
		
		NSBitmapImageRep* rep = bitmapRGBAFromCombiningRGBWithGrayBitmaps(rep1, rep2);
		outData = convertBitmap(rep, kiEColorFormat_R8G8B8A8, generateMipmaps, YES);
		printf("Done.\n");
	}
	
	return [outData compressedData];
}

static NSData*
convertBitmap(NSBitmapImageRep* rep, iEColorFormat format, BOOL generateMipmaps, BOOL alphaNonpremul) {
	if (!alphaNonpremul) premulAlphaIfNeeded(rep);
	NSData* data = textureDataFromBitmap(rep, format, generateMipmaps);
	return [data compressedData];
}

static NSBitmapImageRep*
bitmapRGBAFromCombiningRGBWithGrayBitmaps(NSBitmapImageRep* repRGB, NSBitmapImageRep* repGray) {
	BOOL res = ![repRGB hasAlpha] && [repRGB samplesPerPixel] == 3 && [repRGB bitsPerSample] == 8;
	if (!res) {
		printf("ERROR: Unsupported RGB bitmap image format.");
		return nil;
	}
	res = ![repGray hasAlpha] && [repGray samplesPerPixel] == 1 && [repGray bitsPerSample] == 8;
	if (!res) {
		printf("ERROR: Unsupported Gray bitmap image format.");
		return nil;
	}
	res = [repRGB pixelsWide] == [repGray pixelsWide] && [repRGB pixelsHigh] == [repGray pixelsHigh];
	if (!res) {
		printf("ERROR: RGB and Gray bitmaps must have the same size.");
		return nil;
	}
	
	NSInteger bps = 8;
	NSInteger spp = 4;
	NSInteger width = [repRGB pixelsWide];
	NSInteger height = [repRGB pixelsHigh];
	
	NSBitmapImageRep* rep = [[[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																	 pixelsWide:width
																	 pixelsHigh:height
																  bitsPerSample:bps
																samplesPerPixel:spp
																	   hasAlpha:YES
																	   isPlanar:NO
																 colorSpaceName:NSDeviceRGBColorSpace
																   bitmapFormat:NSAlphaNonpremultipliedBitmapFormat
																	bytesPerRow:width * spp
																   bitsPerPixel:0] autorelease];
	SColorA* colorA = (SColorA*)[repGray bitmapData];
	SColorRGB* colorRGB = (SColorRGB*)[repRGB bitmapData];
	SColorRGBA* colorRGBA = (SColorRGBA*)[rep bitmapData];
	
	for (NSInteger i = 0, cnt = width * height; i < cnt; ++i, ++colorA, ++colorRGB, ++colorRGBA) {
		colorRGBA->r = colorRGB->r;
		colorRGBA->g = colorRGB->g;
		colorRGBA->b = colorRGB->b;
		colorRGBA->a = colorA->a;
	}
	
	return rep;
}

static NSData*
textureDataFromBitmap(NSBitmapImageRep* rep, iEColorFormat format, BOOL generateMipmaps) {
	NSInteger width = [rep pixelsWide];
	NSInteger height = [rep pixelsHigh];
	
	NSOpenGLPixelFormatAttribute attrs[] = { NSOpenGLPFAAllRenderers, 0 };
	NSOpenGLPixelFormat* glPixelFmt = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attrs] autorelease];
	NSOpenGLContext* glContext = [[[NSOpenGLContext alloc] initWithFormat:glPixelFmt shareContext:nil] autorelease];
	[glContext makeCurrentContext];
	
	GLuint tempTex = 0;
	GLenum texTarget = GL_TEXTURE_2D;
	
	glGenTextures(1, &tempTex);
	glBindTexture(texTarget, tempTex);
	glTexParameteri(texTarget, GL_GENERATE_MIPMAP, generateMipmaps ? GL_TRUE : GL_FALSE);
	glSignalIfError();
	
	GLenum glSrcFmt, glSrcType;
	sourceFormatFromBitmap(rep, &glSrcFmt, &glSrcType);
	GLenum internalFmt = internalTextureFormatFrom_iEColorFormat(format);
	
	glTexImage2D(texTarget, 0, internalFmt, width, height, 0, glSrcFmt, glSrcType, [rep bitmapData]);
	glSignalIfError();
	
	NSInteger level = 0;
	NSInteger w = width;
	NSInteger h = height;
	NSUInteger dataLength = 0;
	GLenum glDstFmt, glDstType;
	glColorFormatFrom_iEColorFormat(format, &glDstFmt, &glDstType);
	NSInteger numMipmaps = generateMipmaps ? mipmapsCount(width, height) : 1;
	NSMutableArray* mipmaps = [NSMutableArray arrayWithCapacity:numMipmaps];
	
	while (level < numMipmaps) {
		NSUInteger length = pixelByteSizeFrom_iEColorFormat(format) * w * h;
		NSMutableData* dstData = [NSMutableData dataWithLength:length];
		[mipmaps addObject:dstData];
		dataLength += length;
		
		glGetTexImage(texTarget, level, glDstFmt, glDstType, [dstData mutableBytes]);
		glSignalIfError();
		
		w = MAX(w >> 1, 1);
		h = MAX(h >> 1, 1);
		++level;
	}
	
	glBindTexture(texTarget, tempTex);
	glDeleteTextures(1, &tempTex);
	glSignalIfError();
	
	[NSOpenGLContext clearCurrentContext];
	
	iSTexHeader th;
	th.headerLength = CFSwapInt32HostToLittle(sizeof(th));
	th.height = CFSwapInt32HostToLittle(height);
	th.width = CFSwapInt32HostToLittle(width);
	th.numMipmaps = CFSwapInt32HostToLittle(numMipmaps);
	th.format = CFSwapInt32HostToLittle(format);
	th.dataLength = CFSwapInt32HostToLittle(dataLength);
	th.tag = CFSwapInt32HostToLittle(kiTextureSignature);
	
	NSMutableData* textureData = [NSMutableData dataWithCapacity:dataLength + sizeof(th)];
	[textureData appendBytes:&th length:sizeof(th)];
	for (NSData* data in mipmaps) [textureData appendData:data];
	
	return textureData;
}

static void
premulAlphaIfNeeded(NSBitmapImageRep* rep) {
	if (![rep hasAlpha]) return;
	NSCAssert(NSAlphaNonpremultipliedBitmapFormat == [rep bitmapFormat], @"ERROR: Unsupported bitmap image format.");
	
	NSInteger width = [rep pixelsWide];
	NSInteger height = [rep pixelsHigh];
	NSInteger spp = [rep samplesPerPixel];
	UInt8* bitmapDataPtr = [rep bitmapData];
	NSInteger count = width * height;
	
	switch (spp) {
		case 4: {
			SColorRGBA* color = (SColorRGBA*)bitmapDataPtr;
			for (NSInteger i = 0; i < count; ++i, ++color) {
				color->r = (color->r / 255.0f) * (color->a / 255.0f) * 255.0f;
				color->g = (color->g / 255.0f) * (color->a / 255.0f) * 255.0f;
				color->b = (color->b / 255.0f) * (color->a / 255.0f) * 255.0f;
			}
			break;
		}
		case 2: {
			SColorLA* color = (SColorLA*)bitmapDataPtr;
			for (NSInteger i = 0; i < count; ++i, ++color) {
				color->l = (color->l / 255.0f) * (color->a / 255.0f) * 255.0f;
			}
			break;
		}
		default:
			printf("ERROR: Unsupported bitmap image format.\n");
			break;
	}
}

static void
glColorFormatFrom_iEColorFormat(iEColorFormat iFormat, GLenum* oFmt, GLenum* oType) {
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
			*oFmt = GL_RGBA;
			*oType = GL_UNSIGNED_BYTE;
			printf("ERROR: Unsupported GL texture format.\n");
			break;
	}
}

static NSInteger
pixelByteSizeFrom_iEColorFormat(iEColorFormat iFormat) {
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
			printf("ERROR: Unsupported GL texture format.\n");
			return 4;
	}
}

static GLenum
internalTextureFormatFrom_iEColorFormat(iEColorFormat iFormat) {
	switch (iFormat) {
		case kiEColorFormat_R8G8B8:		return GL_RGB8;
		case kiEColorFormat_R8G8B8A8:	return GL_RGBA8;
		case kiEColorFormat_R5G6B5:		return GL_RGB8;
		case kiEColorFormat_R5G5B5A1:	return GL_RGB5_A1;
		case kiEColorFormat_R4G4B4A4:	return GL_RGBA4;
		case kiEColorFormat_A8:			return GL_ALPHA8;
		case kiEColorFormat_L8:			return GL_LUMINANCE8;
		case kiEColorFormat_L8A8:		return GL_LUMINANCE8_ALPHA8;
			
		default:
			printf("ERROR: Unsupported GL texture format.\n");
			return GL_RGBA8;
	}
}

static void
sourceFormatFromBitmap(NSBitmapImageRep* rep, GLenum* oFmt, GLenum* oType) {
	*oType = GL_UNSIGNED_BYTE;
	
	switch ([rep samplesPerPixel]) {
		case 1:	*oFmt = GL_ALPHA;			break;
		case 2:	*oFmt = GL_LUMINANCE_ALPHA; break;
		case 3:	*oFmt = GL_RGB;				break;
		case 4:	*oFmt = GL_RGBA;			break;
			
		default:
			printf("ERROR: Unsupported bitmap image format.\n");
			*oFmt = GL_RGBA;
			break;
	}
}

static NSInteger
mipmapsCount(NSInteger width, NSInteger height) {
	NSInteger numMipmaps = 1;
	while (width > 1 || height > 1) {
		++numMipmaps;
		width = MAX(width >> 1, 1);
		height = MAX(height >> 1, 1);
	}
	
	return numMipmaps;
}

static void
_createDirectoriesIfNeeded(NSString* path) {
	if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
		NSError* error = nil;
		if (![[NSFileManager defaultManager] createDirectoryAtPath:path
									   withIntermediateDirectories:YES
														attributes:nil
															 error:&error])
		{
			[NSException raise:@"Exception!" format:@"Can\'t create directory"];
		}
	}
}