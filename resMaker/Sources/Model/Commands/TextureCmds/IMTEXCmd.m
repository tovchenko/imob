//
//  IMTEXCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMTEXCmd.h"
#import "IMTextureInfo.h"


@implementation IMTEXCmd

@synthesize premultipliedAlpha = _premultipliedAlpha;
@synthesize generateMipmaps = _generateMipmaps;
@synthesize format = _format;

- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler {
	NSMutableArray* args = [NSMutableArray arrayWithObjects:
							@"-m",
							@"-nonpremul",
							@"-o", self.composedDestinationPath,
							@"-f", [IMTextureInfo stringFromColorFormat:self.format],
							_srcPath,
							nil];
	
	if (!self.generateMipmaps) {
		[args removeObjectAtIndex:0];
		if (self.premultipliedAlpha)
			[args removeObjectAtIndex:0];
	} else if (self.premultipliedAlpha) {
		[args removeObjectAtIndex:1];
	}
	
	[self.wroteDataPathes removeAllObjects];
	[self.wroteDataPathes addObject:self.composedDestinationPath];
	[self runAtPath:[[NSBundle mainBundle] pathForResource:@"image2tex" ofType:nil] withArguments:args completionHandler:handler];
}

- (NSString*)composedDestinationPath {
	if (!_cmpDstPath) {
		_cmpDstPath = [[[_dstPath stringByDeletingPathExtension]
						stringByAppendingPathExtension:@"tex"]
					   retain];
	}
	return _cmpDstPath;
}

- (void)dealloc {
	[_cmpDstPath release];
	[super dealloc];
}

@end
