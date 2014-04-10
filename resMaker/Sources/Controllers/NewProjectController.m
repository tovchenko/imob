//
//  NewProjectController.m
//  resMaker
//
//  Created by DoctorSet on 9/2/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NewProjectController.h"


@implementation NewProjectController

@synthesize projectNameField = _projectNameField;
@synthesize pathField = _pathField;

+ (id)controller {
	return [[[self alloc] initWithWindowNibName:@"NewProject"] autorelease];
}

- (void)windowDidLoad {
	[super windowDidLoad];
	
	[self.projectNameField setStringValue:NSLocalizedString(@"Untitled", nil)];
	[self.pathField setStringValue:[NSSearchPathForDirectoriesInDomains(NSDesktopDirectory, NSUserDomainMask, YES) objectAtIndex:0]];
}

- (void)beginModalWithCompletionHandler:(void(^)(BOOL isDone, NSString* path))handler {
	if ([NSApp runModalForWindow:self.window])
		handler(YES, [[self.pathField stringValue] stringByAppendingPathComponent:[self.projectNameField stringValue]]);
	else
		handler(NO, nil);
}

- (IBAction)choosePathPressed:(id)sender {
	NSOpenPanel* op = [[NSOpenPanel openPanel] retain];
	op.canChooseFiles = NO;
	op.canChooseDirectories = YES;
	
	[op beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
		if (NSFileHandlingPanelOKButton == result) {
			[self.pathField setStringValue:[[[op URLs] objectAtIndex:0] path]];
		}
		[op release];
	}];
}

- (IBAction)createPressed:(id)sender {
	[NSApp stopModalWithCode:1];
}

- (IBAction)cancelPressed:(id)sender {
	[NSApp stopModalWithCode:0];
}

@end
