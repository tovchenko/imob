//
//  ProjectWindowController.m
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "ProjectWindowController.h"
#import "TexturePaneViewController.h"
#import "MeshPaneViewController.h"
#import "IMFileObject.h"
#import "IMFileObjectHelper.h"
#import "IMBuilder.h"
#import "AssetPane.h"

#import "UKKQueue.h"

NSString* const kUntitledFolderName = @"Untitled";

static NSDictionary*	_sSupportedFileExt = nil;
static NSArray*			_supportedFileExtensions(NSString* paneStr);

static NSArray*
_supportedFileExtensions(NSString* paneStr) {
	if (!_sSupportedFileExt) {
		_sSupportedFileExt = [[NSDictionary alloc] initWithObjectsAndKeys:
							  [NSArray arrayWithObjects:@"png", @"jpg", @"jpeg", @"tiff", @"gif", nil], kTextureAssetID,
							  [NSArray arrayWithObjects:@"obj", nil], kMeshAssetID,
							  nil];
	}
	return [_sSupportedFileExt objectForKey:paneStr];
}

@interface ProjectWindowController ()
@property(nonatomic, readonly) IMBuilder*	builder;

- (NSDictionary*)IM_panes;
- (IMFileObject*)IM_projectPathFileSysItem;
- (IMFileObject*)IM_activePathFileSysItem;
- (void)IM_gotoPane:(NSString*)paneKey;
- (void)IM_reloadAll;
- (void)IM_saveSelectedAsset;
@end


@implementation ProjectWindowController

@synthesize popUp = _popUp;
@synthesize filesView = _filesView;
@synthesize scrollView = _scrollView;

@dynamic fileWatcher;
@dynamic builder;
@dynamic state;
@synthesize path = _path;


- (id)initNewProjectByPath:(NSString*)path {
	if ((self = [super initWithWindowNibName:@"ProjectWindow"])) {
		_path = [path copy];
		
		NSFileManager* defaultManager = [NSFileManager defaultManager];
		NSError* error = nil;
		
		if (![defaultManager createDirectoryAtPath:_path
					   withIntermediateDirectories:NO
										attributes:nil
											 error:&error])
		{
			[[NSAlert alertWithError:error] runModal];
			[self release];
			return nil;
		}
		
		[[self IM_panes] enumerateKeysAndObjectsUsingBlock:^(id key, id object, BOOL* stop) {
			NSError* error = nil;
			if (![defaultManager createDirectoryAtPath:[_path stringByAppendingPathComponent:key]
						   withIntermediateDirectories:NO
											attributes:nil
												 error:&error])
			{
				[[NSAlert alertWithError:error] runModal];
			}
		}];
	}
	return self;
}

- (id)initOpenProjectByPath:(NSString*)path {
	if ((self = [super initWithWindowNibName:@"ProjectWindow"])) {
		_path = [[path stringByDeletingLastPathComponent] copy];
		self.state = [NSData dataWithContentsOfFile:path];
	}
	return self;
}

- (void)dealloc {
	[_panesDictionary release];
	[_currentPaneKey release];
	[_path release];
	[_projectPathItem release];
	[_fileWatcher release];
	[_builder release];
	
	[super dealloc];
}

- (void)windowDidLoad {
	[super windowDidLoad];
	
	self.window.title = [NSString stringWithFormat:@"resMaker - %@", [_path lastPathComponent]];
	[_popUp addItemsWithTitles:[NSArray arrayWithObjects:
								kTextureAssetID,
								kMeshAssetID,
								nil]];
	
	[[self IM_panes] enumerateKeysAndObjectsUsingBlock:^(id key, id obj, BOOL* stop) {
		[obj loadView];
	}];
	
	// default pane
	[self IM_gotoPane:kTextureAssetID];
}

- (UKKQueue*)fileWatcher {
	if (!_fileWatcher) {
		_fileWatcher = [[UKKQueue alloc] init];
		[_fileWatcher setDelegate:self];
	}
	return _fileWatcher;
}

- (IMBuilder*)builder {
	if (!_builder)
		_builder = [IMBuilder new];
	return _builder;
}

- (NSData*)state {
	[self IM_reloadAll];
	return [self IM_projectPathFileSysItem].hierarchyInfo;
}

- (void)setState:(NSData*)st {
	[self IM_projectPathFileSysItem].parent.hierarchyInfo = st;
	[_projectPathItem release];
	_projectPathItem = nil;
	
	[self IM_reloadAll];
}

#pragma mark -
#pragma mark [Private]

- (NSDictionary*)IM_panes {
	if (!_panesDictionary)
		_panesDictionary = [[NSDictionary alloc] initWithObjectsAndKeys:
							[TexturePaneViewController controller], kTextureAssetID,
							[MeshPaneViewController controller], kMeshAssetID,
							nil];
	
	return _panesDictionary;
}

- (IMFileObject*)IM_projectPathFileSysItem {
	if (!_projectPathItem) {
		_projectPathItem = [IMFileObject rootObject];
		
		for (NSString* component in [_path pathComponents]) {
			IMFileObject* nextItem = nil;
			
			for (NSInteger i = 0, cnt = [_projectPathItem numberOfChildren]; i < cnt; ++i) {
				nextItem = [_projectPathItem childAtIndex:i];
				if ([[nextItem relativePath] isEqualToString:component]) {
					_projectPathItem = nextItem;
					break;
				}
			}
		}
		[_projectPathItem retain];
	}
	
	return _projectPathItem;
}

- (IMFileObject*)IM_activePathFileSysItem {
	IMFileObject* baseItem = [self IM_projectPathFileSysItem];
	
	for (NSInteger i = 0, cnt = [baseItem numberOfChildren]; i < cnt; ++i) {
		IMFileObject* currentItem = [baseItem childAtIndex:i];
		if ([_currentPaneKey isEqualToString:[currentItem relativePath]]) {
			return currentItem;
		}
	}
	
	return nil;
}

- (void)IM_gotoPane:(NSString*)paneKey {
	if (![_currentPaneKey isEqualToString:paneKey]) {
		AssetPane* prevPane = [[self IM_panes] objectForKey:_currentPaneKey];
		[prevPane writeAssetData];
		prevPane.asset = nil;
		
		[_currentPaneKey release];
		_currentPaneKey = [paneKey retain];
		
		AssetPane* currentPane = [[self IM_panes] objectForKey:_currentPaneKey];
		
		[self.fileWatcher removeAllPathsFromQueue];
		
		[[self IM_projectPathFileSysItem] reloadWithVisitor:self];
		IMFileObject* activeFileObject = [self IM_activePathFileSysItem];
		activeFileObject.filters = _supportedFileExtensions(_currentPaneKey);
		[activeFileObject reloadHierarchyWithVisitor:self];
		
		self.scrollView.documentView = nil;
		[self.filesView deselectAll:nil];
		
		[self.filesView reloadData];
		[currentPane readAssetData];
	}
}

- (void)IM_reloadAll {
	[self IM_saveSelectedAsset];
	
	IMFileObject* projectFileObj = [self IM_projectPathFileSysItem];
	NSMutableArray* paneKeys = [NSMutableArray arrayWithArray:[[self IM_panes] allKeys]];
	
	for (NSInteger i = 0, cnt = [projectFileObj numberOfChildren]; i < cnt; ++i) {
		IMFileObject* child = [projectFileObj childAtIndex:i];
		
		for (NSString* key in paneKeys) {
			if ([key isEqualToString:[child relativePath]]) {
				child.filters = _supportedFileExtensions(key);
				[paneKeys removeObject:key];
				break;
			}
		}
	}
	
	[projectFileObj reloadHierarchyWithVisitor:self];
}

- (void)IM_saveSelectedAsset {
	IMFileObject* selectedItem = [_filesView itemAtRow:[_filesView selectedRow]];
	if (selectedItem.isLeaf) [[[self IM_panes] objectForKey:_currentPaneKey] writeAssetData];
}

#pragma mark -
#pragma mark [Actions]

- (IBAction)popUpClicked:(id)sender {
	[self IM_gotoPane:[sender titleOfSelectedItem]];
}

- (IBAction)makeNewFolderItem:(id)sender {
	IMFileObject* parentFolder = [_filesView itemAtRow:[_filesView selectedRow]];
	if (!parentFolder) parentFolder = [self IM_activePathFileSysItem];
	
	NSFileManager* defaultManager = [NSFileManager defaultManager];
	NSError* error = nil;
	
	if (![defaultManager createDirectoryAtPath:[[parentFolder fullPath] stringByAppendingPathComponent:kUntitledFolderName]
				   withIntermediateDirectories:NO
									attributes:nil
									 	 error:&error])
	{
		[[NSAlert alertWithError:error] runModal];
		return;
	}
	
	_itemWasAddedFromApp = YES;
}

- (IBAction)removeFileSystemItem:(id)sender {
	IMFileObject* item = [_filesView itemAtRow:[_filesView selectedRow]];
	
	NSError* error = nil;
	if (![[NSFileManager defaultManager] removeItemAtPath:[item fullPath] error:&error]) {
		[[NSAlert alertWithError:error] runModal];
		return;
	}
}

- (IBAction)addNewFileSystemItem:(id)sender {
	NSOpenPanel* op = [NSOpenPanel openPanel];
	op.canChooseFiles = YES;
	op.canChooseDirectories = YES;
	
	[op beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
		if (NSFileHandlingPanelOKButton == result) {
		}
	}];
}

- (IBAction)buildToFolder:(id)sender {
	NSOpenPanel* op = [NSOpenPanel openPanel];
	op.canChooseFiles = NO;
	op.canChooseDirectories = YES;
	
	[op beginSheetModalForWindow:self.window completionHandler:^(NSInteger result) {
		if (NSFileHandlingPanelOKButton == result) {
			[self IM_saveSelectedAsset];
			
			self.builder.destinationDirectory = [[[op URLs] objectAtIndex:0] path];
			[self.builder buildWithFileObject:[self IM_projectPathFileSysItem]];
		}
	}];
}

#pragma mark -
#pragma mark [OutlineView DataSource]

- (id)outlineView:(NSOutlineView*)outlineView child:(NSInteger)index ofItem:(id)item {
	if (item) return [(IMFileObject*)item childAtIndex:index];
	return [[self IM_activePathFileSysItem] childAtIndex:index];
}

- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item {
	return item ? (-1 != [item numberOfChildren]) : YES;
}

- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item {
	if (!_currentPaneKey) return 0; 
	return item ? [item numberOfChildren] : [[self IM_activePathFileSysItem] numberOfChildren];
}

- (id)outlineView:(NSOutlineView*)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
	return item ? [item relativePath] : @"";
}

- (void)outlineView:(NSOutlineView*)outlineView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn byItem:(id)item {
	NSError* error = nil;
	NSString* newPath = [[[item fullPath] stringByDeletingLastPathComponent] stringByAppendingPathComponent:object];
	
	if (![[NSFileManager defaultManager] moveItemAtPath:[item fullPath] toPath:newPath error:&error]) {
		[[NSAlert alertWithError:error] runModal];
		return;
	}
}

#pragma mark -
#pragma mark [OutlineView Delegate]

- (void)outlineViewSelectionDidChange:(NSNotification*)notification {
	IMFileObject* selectedItem = [_filesView itemAtRow:[_filesView selectedRow]];
	
	if (selectedItem.isLeaf) {
		AssetPane* currentPane = [[self IM_panes] objectForKey:_currentPaneKey];
	
		[currentPane writeAssetData];
		currentPane.asset = selectedItem;
		[currentPane readAssetData];
		
		self.scrollView.documentView = currentPane.view;
	} else {
		self.scrollView.documentView = nil;
	}
}

#pragma mark -
#pragma mark [UKKQueueDelegate]

- (void)watcher:(id<UKFileWatcher>)kq receivedNotification:(NSString*)nm forPath:(NSString*)fpath {
	if ([nm isEqualToString:UKFileWatcherWriteNotification]) {
		IMFileObject* parent = [[self IM_activePathFileSysItem] searchObjectByPath:fpath];
		
		[parent reloadWithVisitor:self];
		[_filesView expandItem:parent];
		[_filesView reloadData];
		
		if (_itemWasAddedFromApp) {
			IMFileObject* newFolder = nil;
			for (NSInteger i = 0, cnt = [parent numberOfChildren]; i < cnt; ++i) {
				IMFileObject* nextItem = [parent childAtIndex:i];
				if ([kUntitledFolderName isEqualToString:[nextItem relativePath]]) {
					newFolder = nextItem;
					break;
				}
			}

			[_filesView editColumn:0 row:[_filesView rowForItem:newFolder] withEvent:nil select:YES];
			_itemWasAddedFromApp = NO;
		}
	} else if ([nm isEqualToString:UKFileWatcherRenameNotification]) {
		IMFileObject* item = [[self IM_activePathFileSysItem] searchObjectByPath:fpath];
		[item.parent reloadWithVisitor:self];
		[_filesView reloadData];
	}
}

#pragma mark -
#pragma mark [IMFileObjectLoadVisitor]

- (void)fileObjectDidUpdate:(IMFileObject*)object {
	[self.fileWatcher addPathToQueue:[object fullPath]];
}

- (void)fileObjectWillDelete:(IMFileObject*)object {
	[self.fileWatcher removePathFromQueue:[object fullPath]];
}

@end
