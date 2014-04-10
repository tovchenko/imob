//
//  IMPVRCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMPVRCmd.h"

NSString* const	kTextureToolPath = @"/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/texturetool";
NSString* const	kChannelWeightingLinear = @"--channel-weighting-linear";
NSString* const	kChannelWeightingPerceptual = @"--channel-weighting-perceptual";
NSString* const	kBitsPerPixel2 = @"--bits-per-pixel-2";
NSString* const	kBitsPerPixel4 = @"--bits-per-pixel-4";

@implementation IMPVRCmd

@synthesize channelWeighting = _channelWeighting;
@synthesize bpp = _bpp;
@synthesize pvrDataType = _pvrDataType;
@synthesize generateMipmaps = _generateMipmaps;

- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler {
	NSMutableArray* args = [NSMutableArray arrayWithObjects:
							@"-m",
							@"-e", @"PVRTC",
							(kiEChannelWeightingLinear == self.channelWeighting) ? kChannelWeightingLinear : kChannelWeightingPerceptual,
							(kiEBPP2 == self.bpp) ? kBitsPerPixel2 : kBitsPerPixel4,
							@"-o", self.composedDestinationPath,
							@"-f", (kiEPVRData == self.pvrDataType) ? @"PVR" : @"Raw",
							_srcPath,
							nil];
	
	if (!self.generateMipmaps)
		[args removeObjectAtIndex:0];
	
	[self.wroteDataPathes removeAllObjects];
	[self.wroteDataPathes addObject:self.composedDestinationPath];
	[self runAtPath:kTextureToolPath withArguments:args completionHandler:handler];
}

- (NSString*)composedDestinationPath {
	if (!_cmpDstPath) {
		_cmpDstPath = [[[_dstPath stringByDeletingPathExtension]
						stringByAppendingPathExtension:(kiEPVRData == self.pvrDataType) ? @"pvr" : @"pvrtc"]
					   retain];
	}
	return _cmpDstPath;
}

- (void)dealloc {
	[_cmpDstPath release];
	[super dealloc];
}

@end
