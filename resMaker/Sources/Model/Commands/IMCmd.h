//
//  IMCmd.h
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol IMCmd
- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler;
@end


@interface IMCmd : NSObject <IMCmd> {
	NSString*			_srcPath;
	NSString*			_dstPath;
	
	NSMutableArray*		_wroteDataPathes;
}

@property(readonly) NSString*			composedDestinationPath;
@property(readonly) NSMutableArray*		wroteDataPathes;

+ (id)commandWithSourcePath:(NSString*)srcPath destinationPath:(NSString*)dstPath;
- (id)initWithSourcePath:(NSString*)srcPath destinationPath:(NSString*)dstPath;

@end
