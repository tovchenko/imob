//
//  IMFileObject.m
//  resMaker
//
//  Created by Taras Tovchenko on 10/5/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "IMFileObject.h"
#import "IMFileObjectLoadVisitor.h"

static NSString* kRelativePathKey = @"_relativePath";
static NSString* kParentKey = @"_parent";
static NSString* kChildrenKey = @"_children";
static NSString* kInfoKey = @"_info";

static IMFileObject*	_sRootObject = nil;
static NSMutableArray*	_sLeafObject = nil;

@interface IMFileObject ()
- (id)_initWithPathComponent:(NSString*)path parent:(IMFileObject*)parent;
- (NSMutableArray*)_childrenObjects;
- (NSMutableArray*)_updatedChildren;
- (void)_reload:(id<IMFileObjectLoadVisitor>)visitor forHierarchy:(BOOL)allHierarchy;
- (BOOL)_canAdd:(IMFileObject*)object;
@end


@implementation IMFileObject

@synthesize parent = _parent;
@synthesize info = _info;

@dynamic filters;
@dynamic isLeaf;
@dynamic hierarchyInfo;

#pragma mark -
#pragma mark [NSCoding]

- (void)encodeWithCoder:(NSCoder*)aCoder {
	NSParameterAssert([aCoder allowsKeyedCoding]);
	[aCoder encodeObject:_relativePath forKey:kRelativePathKey];
	[aCoder encodeConditionalObject:_parent forKey:kParentKey];
	[aCoder encodeObject:_children forKey:kChildrenKey];
	[aCoder encodeObject:_info forKey:kInfoKey];
}

- (id)initWithCoder:(NSCoder*)aDecoder {
	if ((self = [super init])) {
		_relativePath = [[aDecoder decodeObjectForKey:kRelativePathKey] retain];
		_parent = [aDecoder decodeObjectForKey:kParentKey];
		_children = [[aDecoder decodeObjectForKey:kChildrenKey] retain];
		_info = [[aDecoder decodeObjectForKey:kInfoKey] retain];
	}
	return self;
}

+ (void)initialize {
	if (self == [IMFileObject class]) {
		_sLeafObject = [[NSMutableArray alloc] init];
	}
}

+ (IMFileObject*)rootObject {
	if (!_sRootObject)
		_sRootObject = [[IMFileObject alloc] _initWithPathComponent:@"/" parent:nil];
	
	return _sRootObject;
}

- (void)setHierarchyInfo:(NSData*)archivedInfo {
	IMFileObject* localRootObj = [NSKeyedUnarchiver unarchiveObjectWithData:archivedInfo];
	
	for (NSInteger i = 0, cnt = [self numberOfChildren]; i < cnt; ++i) {
		IMFileObject* child = [self childAtIndex:i];
		if ([[localRootObj relativePath] isEqualToString:[child relativePath]]) {
			localRootObj->_parent = self;
			[_children replaceObjectAtIndex:i withObject:localRootObj];
			break;
		}
	}
}

- (NSData*)hierarchyInfo {
	return [NSKeyedArchiver archivedDataWithRootObject:self];
}

- (BOOL)isLeaf {
	NSFileManager* fileManager = [NSFileManager defaultManager];
	BOOL isDir, valid;
	valid = [fileManager fileExistsAtPath:[self fullPath] isDirectory:&isDir];
	
	if (valid && !isDir)
		return YES;
	return NO;
		
}

- (id)_initWithPathComponent:(NSString*)path parent:(IMFileObject*)parent {
	if ((self = [super init])) {
		_relativePath = [[path lastPathComponent] copy];
		_parent = parent;
	}
	return self;
}

- (void)dealloc {
	if (_children != _sLeafObject)
		[_children release];
	[_relativePath release];
	
	self.info = nil;
	[_filters release];
	
	[super dealloc];
}

- (NSArray*)filters {
	if (!_filters)
		return self.parent.filters;
	return _filters;
}

- (void)setFilters:(NSArray*)flts {
	[flts retain];
	[_filters release];
	_filters = flts;
}

- (NSMutableArray*)_childrenObjects {
	if (!_children)
		[self reload];
	return _children;
}

- (NSMutableArray*)_updatedChildren {
	NSFileManager* fileManager = [NSFileManager defaultManager];
	NSString* fullPath = [self fullPath];
	NSMutableArray* result = nil;
	BOOL isDir, valid;
	
	valid = [fileManager fileExistsAtPath:fullPath isDirectory:&isDir];
	
	if (valid && isDir) {
		NSArray* files = [fileManager contentsOfDirectoryAtPath:fullPath error:nil];
		result = [NSMutableArray arrayWithCapacity:files.count];
		
		for (NSString* path in files) {
			IMFileObject* newItem = [[[IMFileObject alloc] _initWithPathComponent:path parent:self] autorelease];
			[result addObject:newItem];
		}
	} else {
		result = _sLeafObject;
	}
	
	return result;
}

- (void)_reload:(id<IMFileObjectLoadVisitor>)visitor forHierarchy:(BOOL)allHierarchy {
	NSMutableArray* realItems = [self _updatedChildren];
	if (realItems == _sLeafObject) {
		if (_children != _sLeafObject) {
			[_children release];
			_children = _sLeafObject;
		}
		return;
	}
	
	if (!_children) _children = [NSMutableArray new];	
	NSMutableArray* deletedObjects = [NSMutableArray array];
	
	for (IMFileObject* existItem in _children) {
		BOOL exist = NO;
		
		for (IMFileObject* realItem in realItems) {
			if ([[existItem relativePath] isEqualToString:[realItem relativePath]]) {
				[realItems removeObject:realItem];
				exist = YES;
				break;
			}
		}
		
		if (!exist) [deletedObjects addObject:existItem];
		else {
			if ([visitor respondsToSelector:@selector(fileObjectDidUpdate:)])
				[visitor fileObjectDidUpdate:existItem];
			
			if (allHierarchy) [existItem _reload:visitor forHierarchy:allHierarchy];
		}
	}
	
	for (IMFileObject* item in deletedObjects) {
		if ([visitor respondsToSelector:@selector(fileObjectWillDelete:)])
			[visitor fileObjectWillDelete:item];
		
		[_children removeObject:item];
	}
	
	for (IMFileObject* item in realItems) {
		if ([self _canAdd:item]) {
			[_children addObject:item];
			
			if ([visitor respondsToSelector:@selector(fileObjectDidUpdate:)])
				[visitor fileObjectDidUpdate:item];
			
			if (allHierarchy) [item _reload:visitor forHierarchy:allHierarchy];
		}
	}
}

- (BOOL)_canAdd:(IMFileObject*)object {
	if (!object.isLeaf && ![[object relativePath] hasPrefix:@"."])
		return YES;
	
	NSString* currentExt = [[object relativePath] pathExtension];
	NSArray* supportedExts = self.filters;
	
	for (NSString* suppExt in supportedExts) {
		if (NSOrderedSame == [currentExt caseInsensitiveCompare:suppExt])
			return YES;
	}
	
	return NO;
}

- (NSInteger)numberOfChildren {
	NSMutableArray* children = [self _childrenObjects];
	
	return (children == _sLeafObject)
		? -1
		: children.count;
}

- (IMFileObject*)childAtIndex:(NSUInteger)index {
	return [[self _childrenObjects] objectAtIndex:index];
}

- (NSString*)fullPath {
	if (!_parent) return _relativePath;
	return [[_parent fullPath] stringByAppendingPathComponent:_relativePath];
}

- (NSString*)relativePath {
	return _relativePath;
}

- (IMFileObject*)searchObjectByPath:(NSString*)path {
	if ([[self relativePath] isEqualToString:[path lastPathComponent]]) {
		if ([[self fullPath] isEqualToString:path])
			return self;
	}
	
	for (NSInteger i = 0, cnt = [self numberOfChildren]; i < cnt; ++i) {
		IMFileObject* foundItem = [[self childAtIndex:i] searchObjectByPath:path];
		if (foundItem) return foundItem;
	}
	
	return nil;
}

- (void)reloadWithVisitor:(id<IMFileObjectLoadVisitor>)visitor {
	[self _reload:visitor forHierarchy:NO];
}

- (void)reloadHierarchyWithVisitor:(id<IMFileObjectLoadVisitor>)visitor {
	[self _reload:visitor forHierarchy:YES];
}

- (void)reload {
	[self reloadWithVisitor:nil];
}

- (void)reloadHierarchy {
	[self reloadHierarchyWithVisitor:nil];
}

@end
