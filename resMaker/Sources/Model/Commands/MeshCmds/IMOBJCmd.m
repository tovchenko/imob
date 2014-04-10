//
//  IMOBJCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMOBJCmd.h"


@implementation IMOBJCmd

@synthesize generateRenderType = _generateRenderType;
@synthesize generateColliderType = _generateColiderType;
@synthesize storeNormals = _storeNormals;
@synthesize storeTexCoords = _storeTexCoords;

- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler {
	NSString* cldDst = nil;
	
	if (self.generateColliderType) {
		NSString* srcFolderPath = [_srcPath stringByDeletingLastPathComponent];
		NSArray* srcComponents = [srcFolderPath pathComponents];
		NSArray* dstComponents = [[_dstPath stringByDeletingLastPathComponent] pathComponents];
		
		NSInteger isrc = srcComponents.count - 1;
		NSInteger idst = dstComponents.count - 1;
		while ([[srcComponents objectAtIndex:isrc] isEqualToString:[dstComponents objectAtIndex:idst]]) {
			--isrc;
			--idst;
		}
		
		NSMutableArray* components = [NSMutableArray arrayWithArray:dstComponents];
		[components replaceObjectAtIndex:++idst withObject:@"Colliders"];
		cldDst = [NSString pathWithComponents:components];
		
		if (![[NSFileManager defaultManager] fileExistsAtPath:cldDst]) {
			NSError* error = nil;
			if (![[NSFileManager defaultManager] createDirectoryAtPath:cldDst
										   withIntermediateDirectories:YES
															attributes:nil
																 error:&error])
			{
				[[NSAlert alertWithError:error] runModal];
			}
		}
		
		cldDst = [cldDst stringByAppendingPathComponent:
				  [[[_srcPath lastPathComponent] stringByDeletingPathExtension]
				   stringByAppendingPathExtension:@"cld"]];
	}
	
	NSMutableArray* args = [NSMutableArray arrayWithObjects:
							@"-mesh", @"-n", @"-t",
							@"-o", self.generateRenderType ? self.composedDestinationPath : @"",
							@"-collider",
							@"-o", cldDst ? cldDst : @"",
							_srcPath,
							nil];
	
	if (self.generateRenderType) {
		if (!self.storeNormals) {
			[args removeObjectAtIndex:1];
			if (!self.storeTexCoords)
				[args removeObjectAtIndex:1];
		} else if (!self.storeTexCoords) {
			[args removeObjectAtIndex:2];
		}
	} else {
		if (!self.generateColliderType)
			[NSException raise:@"Exception!" format:@"Can\'t generate anysing"];
		[args removeObjectsAtIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(0, 5)]];
	}
	
	if (!self.generateColliderType) {
		if (!self.generateRenderType)
			[NSException raise:@"Exception!" format:@"Can\'t generate anysing"];
		
		NSInteger location = (self.storeNormals && self.storeTexCoords) 
			? 5 
			: (!self.storeNormals && !self.storeTexCoords)
				? 3
				: 4;
		
		[args removeObjectsAtIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(location, 3)]];
	}
	
	[self.wroteDataPathes removeAllObjects];
	[self.wroteDataPathes addObject:self.composedDestinationPath];
	if (cldDst) [self.wroteDataPathes addObject:cldDst];
	[self runAtPath:[[NSBundle mainBundle] pathForResource:@"obj2mesh" ofType:nil] withArguments:args completionHandler:handler];
}

- (NSString*)composedDestinationPath {
	if (!_cmpDstPath) {
		_cmpDstPath = [[[_dstPath stringByDeletingPathExtension]
						stringByAppendingPathExtension:@"mesh"]
					   retain];
	}
	return _cmpDstPath;
}

- (void)dealloc {
	[_cmpDstPath release];
	[super dealloc];
}

@end
