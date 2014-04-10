//
//  IMFileObjectDelegate.h
//  resMaker
//
//  Created by Taras Tovchenko on 10/6/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class IMFileObject;

@protocol IMFileObjectLoadVisitor <NSObject>
@optional

- (void)fileObjectDidUpdate:(IMFileObject*)object;
- (void)fileObjectWillDelete:(IMFileObject*)object;

@end
