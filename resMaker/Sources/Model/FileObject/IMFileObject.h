//
//  IMFileObject.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/5/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol IMFileObjectLoadVisitor;


@interface IMFileObject : NSObject <NSCoding> {
	NSString*					_relativePath;
	IMFileObject*				_parent;
	NSMutableArray*				_children;
	
	NSDictionary*				_info;
	NSArray*					_filters;
}

@property(readonly) IMFileObject*			parent;
@property(readonly) BOOL					isLeaf;
@property(retain) NSDictionary*				info;

@property(retain) NSArray*					filters;
@property(assign) NSData*					hierarchyInfo;

+ (IMFileObject*)rootObject;

- (NSInteger)numberOfChildren; // -1 for leaf nodes
- (IMFileObject*)childAtIndex:(NSUInteger)index; // invalid to call on leaf nodes

- (NSString*)fullPath;
- (NSString*)relativePath;

- (IMFileObject*)searchObjectByPath:(NSString*)path;

- (void)reloadWithVisitor:(id<IMFileObjectLoadVisitor>)visitor;
- (void)reloadHierarchyWithVisitor:(id<IMFileObjectLoadVisitor>)visitor;
- (void)reload;
- (void)reloadHierarchy;

@end
