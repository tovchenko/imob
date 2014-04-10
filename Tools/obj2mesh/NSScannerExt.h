//
//  NSScannerExt.h
//  obj2mesh
//
//  Created by DoctorSet on 8/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VertexIndex;

@interface NSScanner (NSScannerExt)

- (BOOL)scanVector2:(Vec2*)v;
- (BOOL)scanVector3:(Vec3*)v;

- (VertexIndex*)scanVertexIndex;

@end
