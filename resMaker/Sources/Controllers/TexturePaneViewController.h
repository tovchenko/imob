//
//  TexturePaneViewController.h
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AssetPane.h"

@class IMTextureInfo;

@interface TexturePaneViewController : AssetPane {
	NSView*			_texPane;
	NSView*			_pvrPane;
	NSView*			_simple565Pane;
	NSView*			_noChangePane;
	
	NSPopUpButton*	_convertorTypePopUp;
	NSButton*		_flipVerticalButton;
	NSBox*			_containerView;
	
	NSButton*		_premultipliedAlphaButton;
	NSButton*		_generateMipmapsButton;
	NSPopUpButton*	_formatPopUp;
	
	NSMatrix*		_channelWeightingMatrix;
	NSMatrix*		_bppMatrix;
	NSMatrix*		_pvrDataTypeMatrix;
	NSButton*		_generateMipmapsPvrButton;
	
	IMTextureInfo*	_texInfo;
}

// panes
@property(retain) IBOutlet NSView*			texPane;
@property(retain) IBOutlet NSView*			pvrPane;
@property(retain) IBOutlet NSView*			simple565Pane;
@property(retain) IBOutlet NSView*			noChangePane;
// common
@property(assign) IBOutlet NSPopUpButton*	convertorTypePopUp;
@property(assign) IBOutlet NSButton*		flipVerticalButton;
@property(assign) IBOutlet NSBox*			containerView;
// .tex format
@property(assign) IBOutlet NSButton*		premultipliedAlphaButton;
@property(assign) IBOutlet NSButton*		generateMipmapsButton;
@property(assign) IBOutlet NSPopUpButton*	formatPopUp;
// .pvr format
@property(assign) IBOutlet NSMatrix*		channelWeightingMatrix;
@property(assign) IBOutlet NSMatrix*		bppMatrix;
@property(assign) IBOutlet NSMatrix*		pvrDataTypeMatrix;
@property(assign) IBOutlet NSButton*		generateMipmapsPvrButton;

+ (id)controller;

- (IBAction)outTypePopUpClicked:(id)sender;

@end
