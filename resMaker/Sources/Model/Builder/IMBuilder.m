//
//  IMBuilder.m
//  resMaker
//
//  Created by Taras Tovchenko on 10/26/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMBuilder.h"
#import "IMFileObject.h"
#import "IMFileObjectHelper.h"
#import "IMAssetInfo.h"

static NSString*	kBuildDirectoryName = @"Assets";
static NSString*	kMetaDirectoryName = @"Meta";


@interface IMBuilder ()
- (void)_allCommandsDidComplete;
@end



@implementation IMBuilder

@synthesize destinationDirectory = _dstPath;

- (void)dealloc {
	self.destinationDirectory = nil;
	[_commands release];
	[_buildedResPaths release];
	
	[super dealloc];
}

- (void)buildWithFileObject:(IMFileObject*)fileObject {
	if (!_commands) _commands = [NSMutableSet new];
	else [_commands removeAllObjects];
	
	NSString* buildPath = [self.destinationDirectory stringByAppendingPathComponent:kBuildDirectoryName];
	NSFileManager* fm = [NSFileManager defaultManager];
	NSError* error = nil;
	BOOL isDir = NO;
	BOOL isExist = [fm fileExistsAtPath:buildPath isDirectory:&isDir];
	
	if (isExist && isDir) {
		if (![fm removeItemAtPath:buildPath error:&error]) {
			[[NSAlert alertWithError:error] runModal];
		}
	}
	
	if (![fm createDirectoryAtPath:buildPath
	   withIntermediateDirectories:NO
						attributes:nil
							 error:&error])
	{
		[[NSAlert alertWithError:error] runModal];
	}
	
	_rootObject = fileObject;
	[_rootObject reloadHierarchyWithVisitor:self];
	
	if (!_buildedResPaths) _buildedResPaths = [NSMutableArray new];
	else [_buildedResPaths removeAllObjects];
	
	__block NSUInteger leftCommandCount = _commands.count;
	
	[_commands makeObjectsPerformSelector:@selector(executeWithCompletionHandler:)
							   withObject:^(NSUInteger code, NSArray* pathes) {
								   [_buildedResPaths addObjectsFromArray:pathes];
								   
								   if (code) NSLog(@"Operation failed!");
								   else NSLog(@"Operation succeeded!");
								   
								   for (NSString* path in pathes)
									   NSLog(@"Resource Path: %@", path);
								   
								   if (0 == --leftCommandCount)
									   [self _allCommandsDidComplete];
							   }];
}

#pragma mark -
#pragma mark [IMFileObjectLoadVisitor]

- (void)fileObjectDidUpdate:(IMFileObject*)object {
	NSString* buildPath = [self.destinationDirectory stringByAppendingPathComponent:kBuildDirectoryName];
	
	NSString* invertedPath = [NSString string];
	IMFileObject* it = object;
	while (it != _rootObject) {
		invertedPath = [invertedPath stringByAppendingPathComponent:[it relativePath]];
		it = it.parent;
	}
	
	NSArray* invertedPathComponents = [invertedPath pathComponents];
	for (NSInteger i = invertedPathComponents.count - 1; i >= 0; --i)
		buildPath = [buildPath stringByAppendingPathComponent:[invertedPathComponents objectAtIndex:i]];
	
	NSFileManager* fm = [NSFileManager defaultManager];
	if (object.isLeaf) {
		id<IMCmd> cmd = [[[IMFileObjectHelper sharedHelper]
						  assetInfoByFileObject:object
								  atProjectPath:[_rootObject fullPath]]
								buildCommandFromPath:[object fullPath]
											  toPath:buildPath];
		
		if (cmd) [_commands addObject:cmd];
	} else {
		NSError* error = nil;
		if (![fm createDirectoryAtPath:buildPath
		   withIntermediateDirectories:NO
							attributes:nil
								 error:&error])
		{
			[[NSAlert alertWithError:error] runModal];
		}
	}
}

#pragma mark -
#pragma mark [Private]

- (void)_allCommandsDidComplete {
	NSMutableArray* temp = [NSMutableArray arrayWithCapacity:_buildedResPaths.count];
	
	for (NSString* path in _buildedResPaths) {
		if ([path hasPrefix:self.destinationDirectory]) {
			[temp addObject:[path substringFromIndex:(self.destinationDirectory.length + 1) + (kBuildDirectoryName.length + 1)]];
		} else {
			NSLog(@"Wrong path");
			[NSException raise:@"Exception!" format:@"Wrong path"];
		}
	}
	
	NSMutableData* pathsData = [NSMutableData data];
	NSUInteger index = 0;
	NSString* header = @"// Generated by resMaker\n\n#ifndef _Assets_H\n#define _Assets_H\n\n";
	
	for (NSString* path in temp) {
		// write to paths data file
		const char* cStr = [path cStringUsingEncoding:NSUnicodeStringEncoding];
		ushort len = 2 * path.length * sizeof(char);
		[pathsData appendBytes:&len length:sizeof(len)];
		[pathsData appendBytes:cStr length:len];
		
		// write to header file
		NSString* identifier = [NSString stringWithFormat:@"k"];
		NSArray* components = [[[path stringByDeletingPathExtension]
								stringByReplacingOccurrencesOfString:@" " withString:@""]
							   pathComponents];
		
		for (NSUInteger i = 0, cnt = components.count; i < cnt; ++i) {
			NSString* component = [[components objectAtIndex:i] capitalizedString];
			identifier = [identifier stringByAppendingString:component];
		}
			
		header = [header stringByAppendingFormat:@"#define %@    %d\n", identifier, index++];
	}
	
	header = [header stringByAppendingString:@"\n\n#endif // _Assets_H"];
	
	NSError* err = nil;
	NSString* metaDirPath = [[self.destinationDirectory stringByAppendingPathComponent:kBuildDirectoryName]
							 stringByAppendingPathComponent:kMetaDirectoryName];
	
	if (![[NSFileManager defaultManager] createDirectoryAtPath:metaDirPath
	   withIntermediateDirectories:NO
						attributes:nil
							 error:&err])
	{
		[[NSAlert alertWithError:err] runModal];
	}
	
	NSString* copyScriptDstPath = [[metaDirPath stringByAppendingPathComponent:@"copy"] stringByAppendingPathExtension:@"sh"];
	
	if (![[NSFileManager defaultManager] copyItemAtPath:[[NSBundle mainBundle] pathForResource:@"copy" ofType:@"sh"]
												 toPath:copyScriptDstPath
												  error:&err])
	{
		[[NSAlert alertWithError:err] runModal];
	}
	// make file executable
	if (![[NSFileManager defaultManager] setAttributes:[NSDictionary dictionaryWithObject:[NSNumber numberWithUnsignedLong:493] forKey:NSFilePosixPermissions]
										  ofItemAtPath:copyScriptDstPath
												 error:&err])
	{
		[[NSAlert alertWithError:err] runModal];
	}
	
	[pathsData writeToFile:[[metaDirPath stringByAppendingPathComponent:@"paths"] stringByAppendingPathExtension:@"dat"]
				atomically:YES];
	
	if (![header writeToFile:[[metaDirPath stringByAppendingPathComponent:@"assets"] stringByAppendingPathExtension:@"h"]
				  atomically:YES
					encoding:NSUTF8StringEncoding
					   error:&err])
	{
		[[NSAlert alertWithError:err] runModal];
	}
}

@end