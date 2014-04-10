//
//  MeshPaneViewController.m
//  resMaker
//
//  Created by DoctorSet on 9/1/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "MeshPaneViewController.h"
#import "IMFileObject.h"
#import "IMMeshInfo.h"


@implementation MeshPaneViewController

@synthesize generateRenderTypeButton = _generateRenderTypeButton;
@synthesize generateColiderTypeButton = _generateColiderTypeButton;
@synthesize storeNormalsButton = _storeNormalsButton;
@synthesize storeTexCoordsButton = _storeTexCoordsButton;

+ (id)controller {
	return [[[self alloc] initWithNibName:@"MeshPane" bundle:nil] autorelease];
}

- (void)dealloc {
	[_meshInfo release];
	[super dealloc];
}

- (IBAction)generateRenderTypePressed:(id)sender {
	[self.storeNormalsButton setEnabled:self.generateRenderTypeButton.state == NSOnState];
	[self.storeTexCoordsButton setEnabled:self.generateRenderTypeButton.state == NSOnState];
}

- (void)willHide {
	_meshInfo.generateRenderType = (NSOnState == self.generateRenderTypeButton.state) ? YES : NO;
	_meshInfo.generateColiderType = (NSOnState == self.generateColiderTypeButton.state) ? YES : NO;
	_meshInfo.storeNormals = (NSOnState == self.storeNormalsButton.state) ? YES : NO;
	_meshInfo.storeTexCoords = (NSOnState == self.storeTexCoordsButton.state) ? YES : NO;
	
	if ([_meshInfo isEqual:[IMMeshInfo defaultMeshInfo]]) self.asset.info = nil;
	else self.asset.info = [_meshInfo storeInfoToDictionary];
}

- (void)didShown {
	[_meshInfo release];
	
	if (self.asset.info) _meshInfo = [[IMMeshInfo alloc] initWithDictionary:self.asset.info];
	else _meshInfo = [[IMMeshInfo defaultMeshInfo] copy];
	
	self.generateRenderTypeButton.state = (_meshInfo.generateRenderType) ? NSOnState : NSOffState;
	self.generateColiderTypeButton.state = (_meshInfo.generateColiderType) ? NSOnState : NSOffState;
	
	[self.storeNormalsButton setEnabled:_meshInfo.generateRenderType];
	[self.storeTexCoordsButton setEnabled:_meshInfo.generateRenderType];
	
	if (self.storeNormalsButton.isEnabled)
		self.storeNormalsButton.state = (_meshInfo.storeNormals) ? NSOnState : NSOffState;
	
	if (self.storeTexCoordsButton.isEnabled)
		self.storeTexCoordsButton.state = (_meshInfo.storeTexCoords) ? NSOnState : NSOffState;
}

@end
