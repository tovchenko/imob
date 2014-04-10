//
//  IMFileObjectHelper.h
//  resMaker
//
//  Created by Taras Tovchenko on 11/18/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol IMAssetInfo;
@class IMFileObject;

@interface IMFileObjectHelper : NSObject {
	NSDictionary*	_assetInfos;
}

+ (id)sharedHelper;

- (id<IMAssetInfo>)assetInfoByFileObject:(IMFileObject*)fo atProjectPath:(NSString*)path;

@end

// IDs of avaliable assets
extern NSString* const kTextureAssetID;
extern NSString* const kMeshAssetID;
