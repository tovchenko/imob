//
//  MeshPaneViewController.h
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AssetPane.h"

@class IMMeshInfo;

@interface MeshPaneViewController : AssetPane {
	NSButton*		_generateRenderTypeButton;
	NSButton*		_generateColiderTypeButton;
	NSButton*		_storeNormalsButton;
	NSButton*		_storeTexCoordsButton;
	
	IMMeshInfo*		_meshInfo;
}

@property(assign) IBOutlet NSButton*	generateRenderTypeButton;
@property(assign) IBOutlet NSButton*	generateColiderTypeButton;
@property(assign) IBOutlet NSButton*	storeNormalsButton;
@property(assign) IBOutlet NSButton*	storeTexCoordsButton;

+ (id)controller;

- (IBAction)generateRenderTypePressed:(id)sender;

@end
