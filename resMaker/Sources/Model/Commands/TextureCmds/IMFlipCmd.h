//
//  IMFlipCmd.h
//  resMaker
//
//  Created by Taras Tovchenko on 12/5/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMCmdLineToolCmd.h"


@protocol IMCmd;

@interface IMFlipCmd : IMCmdLineToolCmd {
	id<IMCmd>	_nextCmd;
	
@private
	NSString*	_cmpDstPath;
}

@property(retain) id<IMCmd>		nextCommand;

@end
