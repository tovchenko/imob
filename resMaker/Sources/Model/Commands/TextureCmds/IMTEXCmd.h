//
//  IMTEXCmd.h
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMTextureInfo.h"
#import "IMCmdLineToolCmd.h"


@interface IMTEXCmd : IMCmdLineToolCmd {
	BOOL				_premultipliedAlpha;
	BOOL				_generateMipmaps;
	iEColorFormat		_format;
	
@private
	NSString*			_cmpDstPath;
}

@property(assign) BOOL					premultipliedAlpha;
@property(assign) BOOL					generateMipmaps;
@property(assign) iEColorFormat			format;

@end
