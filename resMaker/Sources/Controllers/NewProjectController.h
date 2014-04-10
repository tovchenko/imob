//
//  NewProjectController.h
//  resMaker
//
//  Created by DoctorSet on 9/2/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NewProjectController : NSWindowController {
	NSTextField*						_projectNameField;
	NSTextField*						_pathField;
}

@property(assign) IBOutlet NSTextField*				projectNameField;
@property(assign) IBOutlet NSTextField*				pathField;

+ (id)controller;

- (void)beginModalWithCompletionHandler:(void(^)(BOOL isDone, NSString* path))handler;

- (IBAction)choosePathPressed:(id)sender;
- (IBAction)createPressed:(id)sender;
- (IBAction)cancelPressed:(id)sender;

@end
