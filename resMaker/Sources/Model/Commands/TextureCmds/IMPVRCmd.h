//
//  IMPVRCmd.h
//  resMaker
//
//  Created by Taras Tovchenko on 11/17/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IMTextureInfo.h"
#import "IMCmdLineToolCmd.h"

@interface IMPVRCmd : IMCmdLineToolCmd {
	iEChannelWeighting		_channelWeighting;
	iEBPP					_bpp;
	iEPVRDataType			_pvrDataType;
	BOOL					_generateMipmaps;
	
@private
	NSString*				_cmpDstPath;
}

@property(assign) iEChannelWeighting	channelWeighting;
@property(assign) iEBPP					bpp;
@property(assign) iEPVRDataType			pvrDataType;
@property(assign) BOOL					generateMipmaps;

@end
