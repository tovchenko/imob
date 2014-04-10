//
//  TestViewController.m
//  Test
//
//  Created by Taras Tovchenko on 8/19/11.
//  Copyright 2011 USSR Software. All rights reserved.
//

#import "TestViewController.h"
#import <dCocoa.h>
#import "iMob.h"
#import "iOSiMob.h"
#import "Level1.h"

USING_NAMESPACE_IM

@implementation TestViewController

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	[[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
    
    [super dealloc];
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - View lifecycle


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	[[NSNotificationCenter defaultCenter]
        addObserver:self
        selector:@selector(orientationDidChange:)
        name:UIDeviceOrientationDidChangeNotification
        object:nil];
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	
	// initialize engine
	const char* resPath = [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSUTF32LittleEndianStringEncoding];
	dobj_ptr<iAssetBundle> assets(iAssetBundle::alloc()->initWithPath(dString::stringWithCharacters((TCHAR*)resPath)->stringByAppendingPathComponent(_ST("Assets"))));
	iADirector::singleton()->setAssets(assets.get());
	
	dobj_ptr<iAAgent> renderAgent(iOSRenderAgent::alloc()->initWithView((EAGLView*)self.view));
	iADirector::singleton()->agents()->setObjectForKey(renderAgent.get(), _ST("RenderAgent"));
	dobj_ptr<iAAgent> touchAgent(iOSTouchAgent::alloc()->initWithView((EAGLView*)self.view));
	iADirector::singleton()->agents()->setObjectForKey(touchAgent.get(), _ST("TouchAgent"));
	
	dobj_ptr<Level1> level1(CL_NEW(Level1));
	iADirector::singleton()->pushScene(level1.get());
}

- (void)orientationDidChange:(NSNotification*)ntf {
	iContext* context = [((EAGLView*)self.view).context->renderer() iContext];
	
	switch ([UIDevice currentDevice].orientation) {
		case UIDeviceOrientationUnknown:
		case UIDeviceOrientationPortrait:
			context->setOrientation(kiEContextOrientationPortrait);
			break;
		case UIDeviceOrientationPortraitUpsideDown:
			context->setOrientation(kiEContextOrientationPortraitUpsideDown);
			break;
		case UIDeviceOrientationLandscapeLeft:
			context->setOrientation(kiEContextOrientationLandscapeLeft);
			break;
		case UIDeviceOrientationLandscapeRight:
			context->setOrientation(kiEContextOrientationLandscapeRight);
			break;
	}
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
