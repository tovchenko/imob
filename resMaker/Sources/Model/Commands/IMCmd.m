//
//  IMCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMCmd.h"


@implementation IMCmd

@dynamic composedDestinationPath;
@dynamic wroteDataPathes;

+ (id)commandWithSourcePath:(NSString*)srcPath destinationPath:(NSString*)dstPath {
	return [[[self alloc] initWithSourcePath:srcPath destinationPath:dstPath] autorelease];
}

- (id)initWithSourcePath:(NSString*)srcPath destinationPath:(NSString*)dstPath {
	if ((self = [super init])) {
		_srcPath = [srcPath copy];
		_dstPath = [dstPath copy];
	}
	return self;
}

- (void)dealloc {
	[_srcPath release];
	[_dstPath release];
	
	[_wroteDataPathes release];
	
	[super dealloc];
}

- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler {
	[self doesNotRecognizeSelector:_cmd];
}

- (NSString*)composedDestinationPath {
	[self doesNotRecognizeSelector:_cmd];
	return nil;
}

- (NSMutableArray*)wroteDataPathes {
	if (!_wroteDataPathes)
		_wroteDataPathes = [NSMutableArray new];
	return _wroteDataPathes;
}

@end
