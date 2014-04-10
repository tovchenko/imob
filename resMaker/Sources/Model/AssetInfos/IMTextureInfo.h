//
//  IMTextureInfo.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/8/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMAssetInfo.h"


enum {
	kiETEX,
	kiEPVR,
	kiES565,
	kiENoChange
};
typedef NSUInteger iEOutTextureType;

enum {
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
};
typedef NSUInteger iEColorFormat;

enum {
	kiEChannelWeightingLinear,
	kiEChannelWeightingPerceptual
};
typedef NSUInteger iEChannelWeighting;

enum {
	kiEBPP2,
	kiEBPP4
};
typedef NSUInteger iEBPP;

enum {
	kiEPVRData,
	kiERawData
}
typedef iEPVRDataType;


@interface IMTextureInfo : NSObject <NSCopying, IMAssetInfo> {
	BOOL				_flipVertical;
	iEOutTextureType	_outType;
	
	BOOL				_premultipliedAlpha;
	BOOL				_generateMipmaps;
	iEColorFormat		_format;
	
	iEChannelWeighting	_channelWeighting;
	iEBPP				_bpp;
	iEPVRDataType		_pvrDataType;
	BOOL				_generateMipmapsPvr;
}
// common
@property(assign) BOOL					flipVertical;
@property(assign) iEOutTextureType		outType;
// .tex
@property(assign) BOOL					premultipliedAlpha;
@property(assign) BOOL					generateMipmaps;
@property(assign) iEColorFormat			format;
// .pvr
@property(assign) iEChannelWeighting	channelWeighting;
@property(assign) iEBPP					bpp;
@property(assign) iEPVRDataType			pvrDataType;
@property(assign) BOOL					generateMipmapsPvr;

+ (IMTextureInfo*)defaultTextureInfo;

+ (NSString*)stringFromColorFormat:(iEColorFormat)fmt;
+ (iEColorFormat)colorFormatFromString:(NSString*)str;
+ (NSArray*)supportedColorFormats;

@end
