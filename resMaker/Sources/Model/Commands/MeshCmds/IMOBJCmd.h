//
//  IMOBJCmd.h
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMCmdLineToolCmd.h"


@interface IMOBJCmd : IMCmdLineToolCmd {
	BOOL		_generateRenderType;
	BOOL		_generateColiderType;
	BOOL		_storeNormals;
	BOOL		_storeTexCoords;
	
@private
	NSString*	_cmpDstPath;
}

@property(assign) BOOL		generateRenderType;
@property(assign) BOOL		generateColliderType;
@property(assign) BOOL		storeNormals;
@property(assign) BOOL		storeTexCoords;

@end
