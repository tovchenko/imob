//
//  OBJData.h
//  obj2mesh
//
//  Created by DoctorSet on 8/13/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface OBJData : NSObject {
	NSString*				_objFilePath;
	
	NSMutableData*			_vertexBuffer;
	NSMutableData*			_tCoordBuffer;
	NSMutableData*			_normalBuffer;
	
	NSMutableDictionary*	_meshParts;
	NSMutableArray*			_currentIndices;
}

+ (id)dataWithPath:(NSString*)path;
- (id)initWithPath:(NSString*)path;

- (NSData*)composedRenderGeometryWithNormals:(BOOL)storeNormals textureCoords:(BOOL)storeTexCoords;
- (NSData*)composedPhysGeometry;

@end
