//
//  AssetPane.m
//  resMaker
//
//  Created by Taras Tovchenko on 10/7/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "AssetPane.h"
#import "IMFileObject.h"


@implementation AssetPane

@synthesize asset = _asset;

- (void)dealloc {
	self.asset = nil;
	[super dealloc];
}

- (void)writeAssetData {
	if (self.asset && self.asset.isLeaf) {
		[self willHide];
	}
}

- (void)readAssetData {
	if (self.asset && self.asset.isLeaf) {
		[self didShown];
	}
}

- (void)willHide {
	[self doesNotRecognizeSelector:_cmd];
}

- (void)didShown {
	[self doesNotRecognizeSelector:_cmd];
}

@end
