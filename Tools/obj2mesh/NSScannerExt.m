//
//  NSScannerExt.m
//  obj2mesh
//
//  Created by DoctorSet on 8/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NSScannerExt.h"
#import "VertexIndex.h"

@implementation NSScanner (NSScannerExt)

- (BOOL)scanVector3:(Vec3*)v {
	return [self scanFloat:&v->x] && [self scanFloat:&v->y] && [self scanFloat:&v->z];
}

- (BOOL)scanVector2:(Vec2*)v {
	return [self scanFloat:&v->x] && [self scanFloat:&v->y];
}

- (VertexIndex*)scanVertexIndex {
	NSInteger v, t, n;
	
	if ([self scanInteger:&v] && [self scanString:@"/" intoString:NULL] &&
		[self scanInteger:&t] && [self scanString:@"/" intoString:NULL] &&
		[self scanInteger:&n])
	{
		return [VertexIndex indexWithVertex:--v texCoord:--t normal:--n];
	} else if ([self scanInteger:&v] &&
			   [self scanString:@"/" intoString:NULL] &&
			   [self scanInteger:&t])
	{
		return [VertexIndex indexWithVertex:--v texCoord:--t];
	} else if ([self scanInteger:&v] &&
			   [self scanString:@"/" intoString:NULL] && [self scanString:@"/" intoString:NULL] &&
			   [self scanInteger:&n])
	{
		return [VertexIndex indexWithVertex:--v normal:--n];
	} else if ([self scanInteger:&v]) {
		return [VertexIndex indexWithVertex:--v];
	}
	
	[NSException raise:@"Exception!" format:@"Wrong face-index format!"];
	return nil;
}

@end
