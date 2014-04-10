//
//  resMakerAppDelegate.h
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class ProjectWindowController;

@interface resMakerAppDelegate : NSObject <NSApplicationDelegate> {
	ProjectWindowController*	_projectWindowController;
}

- (IBAction)newProject:(id)sender;
- (IBAction)openProject:(id)sender;
- (IBAction)closeProject:(id)sender;
- (IBAction)saveProject:(id)sender;
- (IBAction)saveAsProject:(id)sender;

@end
