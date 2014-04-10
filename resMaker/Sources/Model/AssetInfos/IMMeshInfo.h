//
//  IMMeshInfo.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/8/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMAssetInfo.h"


@interface IMMeshInfo : NSObject <NSCopying, IMAssetInfo> {
	BOOL	_generateRenderType;
	BOOL	_generateColiderType;
	BOOL	_storeNormals;
	BOOL	_storeTexCoords;
}

@property(assign) BOOL		generateRenderType;
@property(assign) BOOL		generateColiderType;
@property(assign) BOOL		storeNormals;
@property(assign) BOOL		storeTexCoords;

+ (IMMeshInfo*)defaultMeshInfo;

@end
