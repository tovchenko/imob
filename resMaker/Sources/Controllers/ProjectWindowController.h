//
//  ProjectWindowController.h
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMFileObjectLoadVisitor.h"

@class IMFileObject;
@class UKKQueue;
@class IMBuilder;

@interface ProjectWindowController : NSWindowController <NSOutlineViewDataSource, NSOutlineViewDelegate, IMFileObjectLoadVisitor> {
	NSPopUpButton*	_popUp;
	NSOutlineView*	_filesView;
	NSScrollView*	_scrollView;
	
	NSDictionary*	_panesDictionary;
	IMFileObject*	_projectPathItem;
	NSString*		_currentPaneKey;
	NSString*		_path;
	
	UKKQueue*		_fileWatcher;
	IMBuilder*		_builder;
	
@private
	BOOL			_itemWasAddedFromApp;
}

@property(assign) IBOutlet NSPopUpButton*	popUp;
@property(assign) IBOutlet NSOutlineView*	filesView;
@property(assign) IBOutlet NSScrollView*	scrollView;

@property(readonly) UKKQueue*				fileWatcher;
@property(assign) NSData*					state;
@property(readonly) NSString*				path;

- (id)initNewProjectByPath:(NSString*)path;
- (id)initOpenProjectByPath:(NSString*)path;

- (IBAction)popUpClicked:(id)sender;
- (IBAction)makeNewFolderItem:(id)sender;
- (IBAction)removeFileSystemItem:(id)sender;
- (IBAction)addNewFileSystemItem:(id)sender;

- (IBAction)buildToFolder:(id)sender;

@end
