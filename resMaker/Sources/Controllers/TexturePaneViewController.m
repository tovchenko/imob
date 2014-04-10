//
//  TexturePaneViewController.m
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "TexturePaneViewController.h"
#import "IMFileObject.h"
#import "IMTextureInfo.h"


static NSDictionary*		_sOutTypeRemap = nil;
static NSDictionary*		_sOutTypeReverseRemap = nil;

static NSDictionary*		_outTypeRemap(BOOL isReverse);


@interface TexturePaneViewController ()
- (void)_switchToCurrentPane;
- (NSView*)_currentOutTypePane;
@end


@implementation TexturePaneViewController

@synthesize texPane = _texPane;
@synthesize pvrPane = _pvrPane;
@synthesize simple565Pane = _simple565Pane;
@synthesize noChangePane = _noChangePane;

@synthesize convertorTypePopUp = _convertorTypePopUp;
@synthesize flipVerticalButton = _flipVerticalButton;
@synthesize containerView = _containerView;

@synthesize premultipliedAlphaButton = _premultipliedAlphaButton;
@synthesize generateMipmapsButton = _generateMipmapsButton;
@synthesize formatPopUp = _formatPopUp;

@synthesize channelWeightingMatrix = _channelWeightingMatrix;
@synthesize bppMatrix = _bppMatrix;
@synthesize pvrDataTypeMatrix = _pvrDataTypeMatrix;
@synthesize generateMipmapsPvrButton = _generateMipmapsPvrButton;

+ (id)controller {
	return [[[self alloc] initWithNibName:@"TexturePane" bundle:nil] autorelease];
}

- (void)dealloc {
	self.texPane = nil;
	self.pvrPane = nil;
	self.simple565Pane = nil;
	self.noChangePane = nil;
	
	[_texInfo release];
	
	[super dealloc];
}

- (IBAction)outTypePopUpClicked:(id)sender {
	[self _switchToCurrentPane];
}

- (void)awakeFromNib {
	[self.convertorTypePopUp addItemsWithTitles:[_outTypeRemap(NO) allKeys]];
	[self.formatPopUp addItemsWithTitles:[IMTextureInfo supportedColorFormats]];
}

- (void)willHide {
	_texInfo.outType = [[_outTypeRemap(NO) objectForKey:[self.convertorTypePopUp titleOfSelectedItem]] unsignedIntegerValue];
	_texInfo.flipVertical = (NSOnState == self.flipVerticalButton.state) ? YES : NO;
	
	_texInfo.premultipliedAlpha = (NSOnState == self.premultipliedAlphaButton.state) ? YES : NO;
	_texInfo.generateMipmaps = (NSOnState == self.generateMipmapsButton.state) ? YES : NO;
	_texInfo.format = [IMTextureInfo colorFormatFromString:[self.formatPopUp titleOfSelectedItem]];
	
	_texInfo.channelWeighting = (self.channelWeightingMatrix.selectedRow) ? kiEChannelWeightingPerceptual : kiEChannelWeightingLinear;
	_texInfo.bpp = (self.bppMatrix.selectedRow) ? kiEBPP4 : kiEBPP2;
	_texInfo.pvrDataType = (self.pvrDataTypeMatrix.selectedRow) ? kiERawData : kiEPVRData;
	_texInfo.generateMipmapsPvr = (NSOnState == self.generateMipmapsPvrButton.state) ? YES : NO;
	
	if ([_texInfo isEqual:[IMTextureInfo defaultTextureInfo]]) self.asset.info = nil;
	else self.asset.info = [_texInfo storeInfoToDictionary];
}

- (void)didShown {
	[_texInfo release];
	
	if (self.asset.info) _texInfo = [[IMTextureInfo alloc] initWithDictionary:self.asset.info];
	else _texInfo = [[IMTextureInfo defaultTextureInfo] copy];
	
	[self.convertorTypePopUp selectItemWithTitle:[_outTypeRemap(YES) objectForKey:[NSNumber numberWithUnsignedInteger:_texInfo.outType]]];
	self.flipVerticalButton.state = (_texInfo.flipVertical) ? NSOnState : NSOffState;
	
	self.premultipliedAlphaButton.state = (_texInfo.premultipliedAlpha) ? NSOnState : NSOffState;
	self.generateMipmapsButton.state = (_texInfo.generateMipmaps) ? NSOnState : NSOffState;
	[self.formatPopUp selectItemWithTitle:[IMTextureInfo stringFromColorFormat:_texInfo.format]];
	
	[self.channelWeightingMatrix selectCellAtRow:(kiEChannelWeightingLinear == _texInfo.channelWeighting) ? 0 : 1 column:0];
	[self.bppMatrix selectCellAtRow:(kiEBPP2 == _texInfo.bpp) ? 0 : 1 column:0];
	[self.pvrDataTypeMatrix selectCellAtRow:(kiEPVRData == _texInfo.pvrDataType) ? 0 : 1 column:0];
	self.generateMipmapsPvrButton.state = (_texInfo.generateMipmapsPvr) ? NSOnState : NSOffState;
	
	[self _switchToCurrentPane];
}

#pragma mark -
#pragma mark [Private]

- (void)_switchToCurrentPane {
	self.containerView.contentView = [self _currentOutTypePane];
}

- (NSView*)_currentOutTypePane {
	iEOutTextureType type = [[_outTypeRemap(NO) objectForKey:[self.convertorTypePopUp titleOfSelectedItem]] unsignedIntegerValue];
	switch (type) {
		case kiETEX: return self.texPane;
		case kiEPVR: return self.pvrPane;
		case kiES565: return self.simple565Pane;
		case kiENoChange: return self.noChangePane;
	}
	return nil;
}

@end


static NSDictionary*
_outTypeRemap(BOOL isReverse) {
	if (!isReverse) {
		if (!_sOutTypeRemap) {
			_sOutTypeRemap = [[NSDictionary alloc] initWithObjectsAndKeys:
							  [NSNumber numberWithUnsignedInteger:kiETEX],		@"TEX",
							  [NSNumber numberWithUnsignedInteger:kiEPVR],		@"PVR",
							  [NSNumber numberWithUnsignedInteger:kiES565],		@"Simple 565",
							  [NSNumber numberWithUnsignedInteger:kiENoChange], @"No Change",
							  nil];
		}
		return _sOutTypeRemap;
	} else {
		if (!_sOutTypeReverseRemap) {
			_sOutTypeReverseRemap = [[NSDictionary alloc] initWithObjectsAndKeys:
									 @"TEX",		[NSNumber numberWithUnsignedInteger:kiETEX],
									 @"PVR",		[NSNumber numberWithUnsignedInteger:kiEPVR],
									 @"Simple 565",	[NSNumber numberWithUnsignedInteger:kiES565],
									 @"No Change",	[NSNumber numberWithUnsignedInteger:kiENoChange],
									 nil];
		}
		return _sOutTypeReverseRemap;
	}
}
