//
//  AssetPane.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/7/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class IMFileObject;


@interface AssetPane : NSViewController {
	IMFileObject*	_asset;
}

@property(retain) IMFileObject*		asset;

- (void)writeAssetData;
- (void)readAssetData;

// needs override
- (void)willHide;
- (void)didShown;

@end
