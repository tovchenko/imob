//
//  IMCopyCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/16/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMCopyCmd.h"


@implementation IMCopyCmd

- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler {
	NSError* error = nil;
	[[NSFileManager defaultManager] copyItemAtPath:_srcPath
											toPath:self.composedDestinationPath
											 error:&error];
	
	if (error) {
		[[NSAlert alertWithError:error] runModal];
		handler(error.code, [NSArray arrayWithObject:self.composedDestinationPath]);
	} else {
		handler(0, [NSArray arrayWithObject:self.composedDestinationPath]);
	}
}

- (NSString*)composedDestinationPath {
	return _dstPath;
}

@end
