//
//  IMBuilder.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/26/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMFileObjectLoadVisitor.h"


@class IMFileObject;


@interface IMBuilder : NSObject <IMFileObjectLoadVisitor> {
	NSString*			_dstPath;
	IMFileObject*		_rootObject; // week ref
	
	NSMutableSet*		_commands;
	NSMutableArray*		_buildedResPaths;
}

@property(nonatomic, copy) NSString*	destinationDirectory;

- (void)buildWithFileObject:(IMFileObject*)fileObject;

@end
