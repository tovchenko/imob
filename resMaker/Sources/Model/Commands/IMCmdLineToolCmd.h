//
//  IMCmdLineToolCmd.h
//  resMaker
//
//  Created by Taras Tovchenko on 11/22/10.
//  Copyright 2010 Gameprom. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMCmd.h"


@interface IMCmdLineToolCmd : IMCmd {
	NSTask*	_task;
	void (^_completionHandler)(NSUInteger, NSArray*);
}

- (void)runAtPath:(NSString*)cmdLineToolPath
	withArguments:(NSArray*)args
completionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler;

@end
