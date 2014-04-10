//
//  IMFileObjectHelper.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/18/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMFileObjectHelper.h"
#import "IMFileObject.h"
#import "IMTextureInfo.h"
#import "IMMeshInfo.h"

NSString* const kTextureAssetID = @"Textures";
NSString* const kMeshAssetID = @"Meshes";

static IMFileObjectHelper*	_sSharedHelper = nil;

@implementation IMFileObjectHelper

+ (id)sharedHelper {
	if (!_sSharedHelper)
		_sSharedHelper = [self new];
	return _sSharedHelper;
}

- (void)dealloc {
	[_assetInfos release];
	
	[super dealloc];
}

- (id<IMAssetInfo>)assetInfoByFileObject:(IMFileObject*)fo atProjectPath:(NSString*)path {
	NSString* fileObjPath = [[[fo fullPath] copy] autorelease];
	if ([fileObjPath hasPrefix:path]) {
		fileObjPath = [fileObjPath substringFromIndex:path.length];
		fileObjPath = [[fileObjPath pathComponents] objectAtIndex:1];
		
		if ([fileObjPath isEqualToString:kTextureAssetID]) {
			if (fo.info) return [[[IMTextureInfo alloc] initWithDictionary:fo.info] autorelease];
			return [IMTextureInfo defaultTextureInfo];
		} else if ([fileObjPath isEqualToString:kMeshAssetID]) {
			if (fo.info) return [[[IMMeshInfo alloc] initWithDictionary:fo.info] autorelease];
			return [IMMeshInfo defaultMeshInfo];
		}
	}
	
	[NSException raise:@"Exeption!" format:@"Can\'t determine asset type of IMFileObject"];
	return nil;
}

@end
