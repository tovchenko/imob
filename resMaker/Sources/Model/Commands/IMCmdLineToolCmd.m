//
//  IMCmdLineToolCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 11/22/10.
//  Copyright 2010 Gameprom. All rights reserved.
//

#import "IMCmdLineToolCmd.h"


@implementation IMCmdLineToolCmd

- (void)runAtPath:(NSString*)cmdLineToolPath
	withArguments:(NSArray*)args
completionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler
{
	if (handler != _completionHandler) {
		[_completionHandler release];
		_completionHandler = [handler copy];
	}
	
	_task = [NSTask new];
	_task.launchPath = cmdLineToolPath;
	_task.arguments = args;
	
	[[NSNotificationCenter defaultCenter] addObserver:self
											 selector:@selector(taskWasFinished:)
												 name:NSTaskDidTerminateNotification
											   object:_task];
	[_task launch];
}

- (void)taskWasFinished:(NSNotification*)ntf {
	[[NSNotificationCenter defaultCenter] removeObserver:self
													name:NSTaskDidTerminateNotification
												  object:_task];	
	
	_completionHandler(_task.terminationStatus, self.wroteDataPathes);
	[_completionHandler release];
	_completionHandler = nil;
	
	[_task release];
	_task = nil;
}

- (void)dealloc {
	[[NSNotificationCenter defaultCenter] removeObserver:self];
	
	[_completionHandler release];
	
	if (_task.isRunning) {
		[NSException raise:@"Exception!"
					format:@"Task doesn\'t finish work"];
		[_task terminate];
	}
	[_task release];
	
	[super dealloc];
}

@end
