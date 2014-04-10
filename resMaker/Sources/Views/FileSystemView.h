//
//  FileSystemView.h
//  resMaker
//
//  Created by Taras Tovchenko on 9/24/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface FileSystemView : NSOutlineView {
	NSMenu*		_folderMenu;
	NSMenu*		_fileMenu;
}

@property(retain) IBOutlet NSMenu*	folderMenu;
@property(retain) IBOutlet NSMenu*	fileMenu;

@end
