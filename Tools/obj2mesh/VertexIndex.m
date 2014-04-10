//
//  VertexIndex.m
//  obj2mesh
//
//  Created by Taras Tovchenko on 11/24/10.
//  Copyright 2010 Gameprom. All rights reserved.
//

#import "VertexIndex.h"


@implementation VertexIndex

@synthesize vertex = _vertex;
@synthesize texCoord = _texCoord;
@synthesize normal = _normal;

@synthesize hasTexCoord = _hasTexCoord;
@synthesize hasNormal = _hasNormal;


+ (id)indexWithVertex:(ushort)vertex texCoord:(ushort)texCoord normal:(ushort)normal {
	return [[[self alloc] initWithVertex:vertex texCoord:texCoord normal:normal] autorelease];
}

+ (id)indexWithVertex:(ushort)vertex texCoord:(ushort)texCoord {
	return [[[self alloc] initWithVertex:vertex texCoord:texCoord] autorelease];
}

+ (id)indexWithVertex:(ushort)vertex normal:(ushort)normal {
	return [[[self alloc] initWithVertex:vertex normal:normal] autorelease];
}

+ (id)indexWithVertex:(ushort)vertex {
	return [[[self alloc] initWithVertex:vertex] autorelease];
}

- (id)initWithVertex:(ushort)vertex texCoord:(ushort)texCoord normal:(ushort)normal {
	if ((self = [super init])) {
		_vertex = vertex;
		_texCoord = texCoord;
		_normal = normal;
		
		_hasTexCoord = YES;
		_hasNormal = YES;
	}
	return self;
}

- (id)initWithVertex:(ushort)vertex texCoord:(ushort)texCoord {
	if ((self = [super init])) {
		_vertex = vertex;
		_texCoord = texCoord;
		
		_hasTexCoord = YES;
	}
	return self;
}

- (id)initWithVertex:(ushort)vertex normal:(ushort)normal {
	if ((self = [super init])) {
		_vertex = vertex;
		_normal = normal;
		
		_hasNormal = YES;
	}
	return self;
}

- (id)initWithVertex:(ushort)vertex {
	if ((self = [super init])) {
		_vertex = vertex;
	}
	return self;
}

@end
