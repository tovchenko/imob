//
//  VertexIndex.h
//  obj2mesh
//
//  Created by Taras Tovchenko on 11/24/10.
//  Copyright 2010 Gameprom. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface VertexIndex : NSObject {
	ushort	_vertex;
	ushort	_texCoord;
	ushort	_normal;
	
	BOOL	_hasTexCoord;
	BOOL	_hasNormal;
}

@property(nonatomic, readonly) ushort		vertex;
@property(nonatomic, readonly) ushort		texCoord;
@property(nonatomic, readonly) ushort		normal;

@property(nonatomic, readonly) BOOL			hasTexCoord;
@property(nonatomic, readonly) BOOL			hasNormal;

+ (id)indexWithVertex:(ushort)vertex texCoord:(ushort)texCoord normal:(ushort)normal;
+ (id)indexWithVertex:(ushort)vertex texCoord:(ushort)texCoord;
+ (id)indexWithVertex:(ushort)vertex normal:(ushort)normal;
+ (id)indexWithVertex:(ushort)vertex;

- (id)initWithVertex:(ushort)vertex texCoord:(ushort)texCoord normal:(ushort)normal;
- (id)initWithVertex:(ushort)vertex texCoord:(ushort)texCoord;
- (id)initWithVertex:(ushort)vertex normal:(ushort)normal;
- (id)initWithVertex:(ushort)vertex;

@end
