//
//  IMMeshInfo.m
//  resMaker
//
//  Created by Taras Tovchenko on 10/8/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMMeshInfo.h"
#import "IMOBJCmd.h"

NSString*	kMeshGenerateRenderType = @"mesh_generate_render_type";
NSString*	kMeshGenerateColiderType = @"mesh_generate_colider_type";
NSString*	kMeshStoreNormals = @"mesh_store_normals";
NSString*	kMeshStoreTexCoords = @"mesh_store_texcoords";

static IMMeshInfo*		_sDefaultMeshInfo = nil;

@implementation IMMeshInfo

@synthesize generateRenderType = _generateRenderType;
@synthesize generateColiderType = _generateColiderType;
@synthesize storeNormals = _storeNormals;
@synthesize storeTexCoords = _storeTexCoords;

+ (IMMeshInfo*)defaultMeshInfo {
	if (!_sDefaultMeshInfo) {
		_sDefaultMeshInfo = [self new];
		_sDefaultMeshInfo.generateRenderType = YES;
		_sDefaultMeshInfo.generateColiderType = NO;
		_sDefaultMeshInfo.storeNormals = NO;
		_sDefaultMeshInfo.storeTexCoords = YES;
	}
	return _sDefaultMeshInfo;
}

#pragma mark -
#pragma mark [isEqual:]

- (BOOL)isEqual:(id)object {
	if (self == object) return YES;
	IMMeshInfo* other = object;
	
	if (other.generateRenderType == self.generateRenderType &&
		other.generateColiderType == self.generateColiderType &&
		other.storeNormals == self.storeNormals &&
		other.storeTexCoords == self.storeTexCoords)
	{
		return YES;
	}
	
	return NO;
}

#pragma mark -
#pragma mark [NSCopying]

- (id)copyWithZone:(NSZone*)zone {
	IMMeshInfo* product = [[IMMeshInfo allocWithZone:zone] init];
	product.generateRenderType = self.generateRenderType;
	product.generateColiderType = self.generateColiderType;
	product.storeNormals = self.storeNormals;
	product.storeTexCoords = self.storeTexCoords;
	
	return product;
}

#pragma mark -
#pragma mark [IMAssetInfo]

- (id)initWithDictionary:(NSDictionary*)info {
	if ((self = [super init])) {
		self.generateRenderType = [[info objectForKey:kMeshGenerateRenderType] boolValue];
		self.generateColiderType = [[info objectForKey:kMeshGenerateColiderType] boolValue];
		self.storeNormals = [[info objectForKey:kMeshStoreNormals] boolValue];
		self.storeTexCoords = [[info objectForKey:kMeshStoreTexCoords] boolValue];
	}
	return self;
}

- (NSDictionary*)storeInfoToDictionary {
	return [NSDictionary dictionaryWithObjectsAndKeys:
			[NSNumber numberWithBool:self.generateRenderType], kMeshGenerateRenderType,
			[NSNumber numberWithBool:self.generateColiderType], kMeshGenerateColiderType,
			[NSNumber numberWithBool:self.storeNormals], kMeshStoreNormals,
			[NSNumber numberWithBool:self.storeTexCoords], kMeshStoreTexCoords,
			nil];
}

- (id<IMCmd>)buildCommandFromPath:(NSString*)srcPath toPath:(NSString*)dstPath {
	IMOBJCmd* cmd = [IMOBJCmd commandWithSourcePath:srcPath destinationPath:dstPath];
	cmd.generateRenderType = self.generateRenderType;
	cmd.generateColliderType = self.generateColiderType;
	cmd.storeNormals = self.storeNormals;
	cmd.storeTexCoords = self.storeTexCoords;
	return cmd;
}

@end
