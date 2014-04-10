//
//  IMFlipCmd.m
//  resMaker
//
//  Created by Taras Tovchenko on 12/5/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMFlipCmd.h"


@implementation IMFlipCmd

@synthesize nextCommand = _nextCmd;

- (void)executeWithCompletionHandler:(void (^)(NSUInteger code, NSArray* pathes))handler {
	NSMutableArray* args = [NSMutableArray arrayWithObjects:
							@"-o", self.composedDestinationPath,
							_srcPath,
							nil];
	
	[self runAtPath:[[NSBundle mainBundle] pathForResource:@"imageFlipper" ofType:nil]
	  withArguments:args
  completionHandler:^(NSUInteger codeNo, NSArray* dataPathes) {
	  void (^remover)() = ^{
		  NSError* error = nil;
		  if (![[NSFileManager defaultManager] removeItemAtPath:self.composedDestinationPath error:&error]) {
			  [[NSAlert alertWithError:error] runModal];
		  }
	  };
		  
	  if (self.nextCommand) {
		  [self.nextCommand executeWithCompletionHandler:^(NSUInteger codeNo, NSArray* dataPathes) {
			  remover();
			  handler(codeNo, dataPathes);
		  }];
	  } else {
		  remover();
		  handler(codeNo, dataPathes);
	  }
  }];
}

- (NSString*)composedDestinationPath {
	if (!_cmpDstPath) {
		_cmpDstPath = [[[_dstPath stringByDeletingLastPathComponent]
						stringByAppendingPathComponent:
						[@".$" stringByAppendingString:[_dstPath lastPathComponent]]]
					   retain];
	}
	return _cmpDstPath;
}

- (void)dealloc {
	[_cmpDstPath release];
	[super dealloc];
}

@end
