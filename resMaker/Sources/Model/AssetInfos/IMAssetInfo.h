//
//  IMAssetInfo.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/8/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol IMCmd;

@protocol IMAssetInfo <NSObject>
@required
- (id)initWithDictionary:(NSDictionary*)info;
- (NSDictionary*)storeInfoToDictionary;

- (id<IMCmd>)buildCommandFromPath:(NSString*)srcPath toPath:(NSString*)dstPath;

@end
