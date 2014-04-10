//
//  IMTextureInfo.m
//  resMaker
//
//  Created by Taras Tovchenko on 10/8/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMTextureInfo.h"
#import "IMFlipCmd.h"
#import "IMCopyCmd.h"
#import "IMTEXCmd.h"
#import "IMPVRCmd.h"
#import "IMS565Cmd.h"

NSString*	kTextureFlipVertical = @"texture_flip_vertical";
NSString*	kTextureOutType = @"texture_out_type";
NSString*	kTextureAlphaPremultiplied = @"texture_alpha_premultiplied";
NSString*	kTextureGenerateMipmaps = @"texture_generate_mipmaps";
NSString*	kTextureFormat = @"texture_format";
NSString*	kTextureChannelWeighting = @"texture_channel_weighting";
NSString*	kTextureBPP = @"texture_bits_per_pixel";
NSString*	kTexturePVRDataType = @"texture_pvr_data_type";
NSString*	kTextureGenerateMipmapsPvr = @"texture_generate_mipmaps_pvr";

static NSDictionary*	_sColorFormatRemap = nil;
static NSDictionary*	_sColorFormatReverseRemap = nil;

static NSDictionary*	_colorFormatRemap(BOOL isReverse);

static IMTextureInfo*	_sDefaultTextureInfo = nil;

@implementation IMTextureInfo

@synthesize flipVertical = _flipVertical;
@synthesize outType = _outType;
@synthesize premultipliedAlpha = _premultipliedAlpha;
@synthesize generateMipmaps = _generateMipmaps;
@synthesize format = _format;
@synthesize channelWeighting = _channelWeighting;
@synthesize bpp = _bpp;
@synthesize pvrDataType = _pvrDataType;
@synthesize generateMipmapsPvr = _generateMipmapsPvr;

+ (IMTextureInfo*)defaultTextureInfo {
	if (!_sDefaultTextureInfo) {
		_sDefaultTextureInfo = [self new];
		
		_sDefaultTextureInfo.flipVertical = YES;
		_sDefaultTextureInfo.outType = kiEPVR;
		
		_sDefaultTextureInfo.premultipliedAlpha = YES;
		_sDefaultTextureInfo.generateMipmaps = YES;
		_sDefaultTextureInfo.format = kiEColorFormat_R8G8B8;
		
		_sDefaultTextureInfo.channelWeighting = kiEChannelWeightingPerceptual;
		_sDefaultTextureInfo.bpp = kiEBPP4;
		_sDefaultTextureInfo.pvrDataType = kiEPVRData;
		_sDefaultTextureInfo.generateMipmapsPvr = YES;
	}
	return _sDefaultTextureInfo;
}

+ (NSString*)stringFromColorFormat:(iEColorFormat)fmt {
	return [_colorFormatRemap(YES) objectForKey:[NSNumber numberWithUnsignedInteger:fmt]];
}

+ (iEColorFormat)colorFormatFromString:(NSString*)str {
	return [[_colorFormatRemap(NO) objectForKey:str] unsignedIntegerValue];
}

+ (NSArray*)supportedColorFormats {
	return [_colorFormatRemap(NO) allKeys];
}

#pragma mark -
#pragma mark [isEqual:]

- (BOOL)isEqual:(id)object {
	if (self == object) return YES;
	IMTextureInfo* other = object;
	
	if (other.flipVertical == self.flipVertical &&
		other.outType == self.outType &&
		other.premultipliedAlpha == self.premultipliedAlpha &&
		other.generateMipmaps == self.generateMipmaps &&
		other.format == self.format &&
		other.channelWeighting == self.channelWeighting &&
		other.bpp == self.bpp &&
		other.pvrDataType == self.pvrDataType &&
		other.generateMipmapsPvr == self.generateMipmapsPvr)
	{
		return YES;
	}
	return NO;
}

#pragma mark -
#pragma mark [NSCopying]

- (id)copyWithZone:(NSZone*)zone {
	IMTextureInfo* product = [[IMTextureInfo allocWithZone:zone] init];
	product.flipVertical = self.flipVertical;
	product.outType = self.outType;
	product.premultipliedAlpha = self.premultipliedAlpha;
	product.generateMipmaps = self.generateMipmaps;
	product.format = self.format;
	product.channelWeighting = self.channelWeighting;
	product.bpp = self.bpp;
	product.pvrDataType = self.pvrDataType;
	product.generateMipmapsPvr = self.generateMipmapsPvr;
	
	return product;
}

#pragma mark -
#pragma mark [IMAssetInfo]

- (id)initWithDictionary:(NSDictionary*)info {
	if ((self = [super init])) {
		self.flipVertical = [[info objectForKey:kTextureFlipVertical] boolValue];
		self.outType = [[info objectForKey:kTextureOutType] unsignedIntegerValue];
		self.premultipliedAlpha = [[info objectForKey:kTextureAlphaPremultiplied] boolValue];
		self.generateMipmaps = [[info objectForKey:kTextureGenerateMipmaps] boolValue];
		self.format = [[info objectForKey:kTextureFormat] unsignedIntegerValue];
		self.channelWeighting = [[info objectForKey:kTextureChannelWeighting] unsignedIntegerValue];
		self.bpp = [[info objectForKey:kTextureBPP] unsignedIntegerValue];
		self.pvrDataType = [[info objectForKey:kTexturePVRDataType] unsignedIntegerValue];
		self.generateMipmapsPvr = [[info objectForKey:kTextureGenerateMipmapsPvr] unsignedIntegerValue];
	}
	return self;
}

- (NSDictionary*)storeInfoToDictionary {
	return [NSDictionary dictionaryWithObjectsAndKeys:
			[NSNumber numberWithBool:self.flipVertical], kTextureFlipVertical,
			[NSNumber numberWithUnsignedInteger:self.outType], kTextureOutType,
			[NSNumber numberWithBool:self.premultipliedAlpha], kTextureAlphaPremultiplied,
			[NSNumber numberWithBool:self.generateMipmaps], kTextureGenerateMipmaps,
			[NSNumber numberWithUnsignedInteger:self.format], kTextureFormat,
			[NSNumber numberWithUnsignedInteger:self.channelWeighting], kTextureChannelWeighting,
			[NSNumber numberWithUnsignedInteger:self.bpp], kTextureBPP,
			[NSNumber numberWithUnsignedInteger:self.pvrDataType], kTexturePVRDataType,
			[NSNumber numberWithBool:self.generateMipmapsPvr], kTextureGenerateMipmapsPvr,
			nil];
}

- (id<IMCmd>)buildCommandFromPath:(NSString*)srcPath toPath:(NSString*)dstPath {
	IMFlipCmd* flipCmd = nil;
	id<IMCmd> result = nil;
	
	if (self.flipVertical) {
		flipCmd = [IMFlipCmd commandWithSourcePath:srcPath destinationPath:dstPath];
		srcPath = flipCmd.composedDestinationPath;
	}
	
	switch (self.outType) {
		case kiETEX: {
			IMTEXCmd* cmd = [IMTEXCmd commandWithSourcePath:srcPath destinationPath:dstPath];
			cmd.premultipliedAlpha = self.premultipliedAlpha;
			cmd.generateMipmaps = self.generateMipmaps;
			cmd.format = self.format;
			result = cmd;
			break;
		}
		case kiEPVR: {
			IMPVRCmd* cmd = [IMPVRCmd commandWithSourcePath:srcPath destinationPath:dstPath];
			cmd.channelWeighting = self.channelWeighting;
			cmd.bpp = self.bpp;
			cmd.pvrDataType = self.pvrDataType;
			cmd.generateMipmaps = self.generateMipmapsPvr;
			result = cmd;
			break;
		}
		case kiES565: {
			result = [IMS565Cmd commandWithSourcePath:srcPath destinationPath:dstPath];
			break;
		}
		case kiENoChange: {
			result = [IMCopyCmd commandWithSourcePath:srcPath destinationPath:dstPath];
			break;
		}
		default:
			[NSException raise:@"Build texture error: wrong self.outType" format:@"Exception"];
			break;
	}
	
	if (self.flipVertical) {
		flipCmd.nextCommand = result;
		return flipCmd;
	} else {
		return result;
	}
	return nil;
}

@end


static NSDictionary*
_colorFormatRemap(BOOL isReverse) {
	if (!isReverse) {
		if (!_sColorFormatRemap) {
			_sColorFormatRemap = [[NSDictionary alloc] initWithObjectsAndKeys:
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_R8G8B8],	@"rgb8",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_R8G8B8A8], @"rgba8",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_R5G6B5],	@"rgb565",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_R5G5B5A1], @"rgb5a1",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_R4G4B4A4], @"rgba4",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_A8],		@"a8",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_L8],		@"l8",
								  [NSNumber numberWithUnsignedInteger:kiEColorFormat_L8A8],		@"l8a8",
								  nil];
		}
		return _sColorFormatRemap;
	} else {
		if (!_sColorFormatReverseRemap) {
			_sColorFormatReverseRemap = [[NSDictionary alloc] initWithObjectsAndKeys:
										 @"rgb8",	[NSNumber numberWithUnsignedInteger:kiEColorFormat_R8G8B8],
										 @"rgba8",	[NSNumber numberWithUnsignedInteger:kiEColorFormat_R8G8B8A8],
										 @"rgb565",	[NSNumber numberWithUnsignedInteger:kiEColorFormat_R5G6B5],
										 @"rgb5a1",	[NSNumber numberWithUnsignedInteger:kiEColorFormat_R5G5B5A1],
										 @"rgba4",	[NSNumber numberWithUnsignedInteger:kiEColorFormat_R4G4B4A4],
										 @"a8",		[NSNumber numberWithUnsignedInteger:kiEColorFormat_A8],
										 @"l8",		[NSNumber numberWithUnsignedInteger:kiEColorFormat_L8],
										 @"l8a8",	[NSNumber numberWithUnsignedInteger:kiEColorFormat_L8A8],	
										 nil];
		}
		return _sColorFormatReverseRemap;
	}
}