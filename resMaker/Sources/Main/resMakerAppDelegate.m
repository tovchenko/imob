//
//  resMakerAppDelegate.m
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "resMakerAppDelegate.h"
#import "ProjectWindowController.h"
#import "NewProjectController.h"

static NSString* kProjectExtension = @"resproj";

@implementation resMakerAppDelegate

- (void)dealloc {
	[_projectWindowController release];
	
	[super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification {
	// Insert code here to initialize your application 
}

- (IBAction)newProject:(id)sender {
	NewProjectController* controller = [NewProjectController controller];
	
	[controller beginModalWithCompletionHandler:^(BOOL isDone, NSString* path) {
		if (isDone) {
			[_projectWindowController release];
			_projectWindowController = [[ProjectWindowController alloc] initNewProjectByPath:path];
			
			[self saveProject:sender];
			
			[_projectWindowController showWindow:nil];
		}
	}];
}

- (IBAction)openProject:(id)sender {
	NSOpenPanel* op = [[NSOpenPanel openPanel] retain];
	op.canChooseFiles = YES;
	op.canChooseDirectories = NO;
	op.allowedFileTypes = [NSArray arrayWithObject:kProjectExtension];
	
	[op beginWithCompletionHandler:^(NSInteger result) {
		if (NSOKButton == result) {
			NSString* path = [[[op URLs] objectAtIndex:0] path];
			
			[_projectWindowController close];
			[_projectWindowController release];
			_projectWindowController = [[ProjectWindowController alloc] initOpenProjectByPath:path];
			[_projectWindowController showWindow:nil];
		}
		[op release];
	}];
}

- (IBAction)closeProject:(id)sender {
	
}

- (IBAction)saveProject:(id)sender {
	NSString* path = _projectWindowController.path;
	path = [[path stringByAppendingPathComponent: [path lastPathComponent]] stringByAppendingPathExtension:kProjectExtension];
	
	[_projectWindowController.state writeToFile:path atomically:YES];
}

- (IBAction)saveAsProject:(id)sender {
}

@end
