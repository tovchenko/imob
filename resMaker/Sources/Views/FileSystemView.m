//
//  FileSystemView.m
//  resMaker
//
//  Created by Taras Tovchenko on 9/24/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "FileSystemView.h"
#import "IMFileObject.h"

@implementation FileSystemView

@synthesize folderMenu = _folderMenu;
@synthesize fileMenu = _fileMenu;

- (NSMenu*)menuForEvent:(NSEvent*)event {
	if (NSRightMouseDown == event.type) {
		NSIndexSet* selectedRowIndexes = [self selectedRowIndexes];
		NSPoint mousePoint = [self convertPoint:event.locationInWindow fromView:nil];
		NSInteger row = [self rowAtPoint:mousePoint];
		
		if (![selectedRowIndexes containsIndex:row])
			[self selectRowIndexes:[NSIndexSet indexSetWithIndex:row] byExtendingSelection:NO];
		
		IMFileObject* selectedItem = [self itemAtRow:row];
		if (-1 != [selectedItem numberOfChildren])
			return self.folderMenu;
		else
			return self.fileMenu;
	}
	
	return [super menuForEvent:event];
}

- (void)dealloc {
	self.folderMenu = nil;
	self.fileMenu = nil;
	
	[super dealloc];
}

@end
