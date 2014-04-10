/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#import "stdafx.h"
#import "EAGLView.h"
#import "ES1Renderer.h"
#import "iOSContext.h"
#import "iOSTouchAgent.h"
#import "iATouch.h"

@implementation EAGLView

@synthesize context = _iOSContext;
@synthesize touchHandler = _iOSTouchHandler;
@synthesize animating = _animating;
@dynamic animationFrameInterval;

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (id)initWithCoder:(NSCoder*)coder {    
    if ((self = [super initWithCoder:coder])) {
        CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;

        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
										kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
										//kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
										nil];
		
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_4_0
		if ([[UIDevice currentDevice].systemVersion compare:@"4.0" options:NSNumericSearch] != NSOrderedAscending)
			eaglLayer.contentsScale = [UIScreen mainScreen].scale;
#endif

		//_iOSContext = IMAY::iOSContext::alloc()->initWithUsingShadersApi(true);
		
        if (!_iOSContext) {
			_iOSContext = IM::iOSContext::alloc()->initWithUsingShadersApi(false);
			
			if (!_iOSContext) {
                [self release];
                return nil;
            }
        }
		
#if __IPHONE_OS_VERSION_MAX_ALLOWED >= __IPHONE_4_0
		if ([[UIDevice currentDevice].systemVersion compare:@"4.0" options:NSNumericSearch] != NSOrderedAscending)
			[_iOSContext->renderer() iContext]->setContentsScale(eaglLayer.contentsScale);
#endif

        _animating = FALSE;
        _displayLinkSupported = FALSE;
        _animationFrameInterval = 1;
        _displayLink = nil;
        _animationTimer = nil;

        // A system version of 3.1 or greater is required to use CADisplayLink. The NSTimer
        // class is used as fallback when it isn't available.
        NSString* reqSysVer = @"3.1";
        NSString* currSysVer = [[UIDevice currentDevice] systemVersion];
        if ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending)
            _displayLinkSupported = TRUE;
    }

    return self;
}

- (void)dealloc {
	CL_RELEASE(_iOSContext);
	
    [super dealloc];
}

- (void)drawView:(id)sender {
	_LocalPool;
    [_iOSContext->renderer() renderScene:_iOSContext->scene()];
}

- (void)layoutSubviews {
    [_iOSContext->renderer() resizeFromLayer:(CAEAGLLayer*)self.layer];
    [self drawView:nil];
}

- (NSInteger)animationFrameInterval {
    return _animationFrameInterval;
}

- (void)setAnimationFrameInterval:(NSInteger)frameInterval {
    // Frame interval defines how many display frames must pass between each time the
    // display link fires. The display link will only fire 30 times a second when the
    // frame internal is two on a display that refreshes 60 times a second. The default
    // frame interval setting of one will fire 60 times a second when the display refreshes
    // at 60 times a second. A frame interval setting of less than one results in undefined
    // behavior.
    if (frameInterval >= 1) {
        _animationFrameInterval = frameInterval;

        if (_animating) {
            [self stopAnimation];
            [self startAnimation];
        }
    }
}

- (void)startAnimation {
    if (!_animating) {
        if (_displayLinkSupported) {
            // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
            // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
            // not be called in system versions earlier than 3.1.

            _displayLink = [NSClassFromString(@"CADisplayLink") displayLinkWithTarget:self selector:@selector(drawView:)];
            [_displayLink setFrameInterval:_animationFrameInterval];
            [_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
        } else {
            _animationTimer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)((1.0 / 60.0) * _animationFrameInterval)
															   target:self
															 selector:@selector(drawView:)
															 userInfo:nil
															  repeats:TRUE];
		}

        _animating = TRUE;
    }
}

- (void)stopAnimation {
    if (_animating) {
        if (_displayLinkSupported) {
            [_displayLink invalidate];
            _displayLink = nil;
        } else {
            [_animationTimer invalidate];
            _animationTimer = nil;
        }

        _animating = FALSE;
    }
}

#pragma mark -
#pragma mark [Touch Handle]

- (dSetT<iATouch*>*)_collectInfoForTouches:(NSSet*)touches {
	dSetT<iATouch*>* result = CL_NEW(dSetT<iATouch*>)->autorelease();
	for (UITouch* t in touches) {
		CGPoint locationPt = [t locationInView:self];
		CGPoint prevLocationPt = [t previousLocationInView:self];
		
		dobj_ptr<iATouch> touch(iATouch::alloc()->initWithParams(
															dMakePoint(locationPt.x, locationPt.y),
															dMakePoint(prevLocationPt.x, prevLocationPt.y),
															t.timestamp,
															t.tapCount));
		result->addObject(touch.get());
	}
	return result;
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
	if (!_iOSTouchHandler) return;
	CGRect bnds = self.bounds;
	_iOSTouchHandler->sendTouches([self _collectInfoForTouches:touches],
								  dMakeRect(bnds.origin.x, bnds.origin.y, bnds.size.width, bnds.size.height),
								  iAETouchPhase_Began);
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
	if (!_iOSTouchHandler) return;
	CGRect bnds = self.bounds;
	_iOSTouchHandler->sendTouches([self _collectInfoForTouches:touches],
								  dMakeRect(bnds.origin.x, bnds.origin.y, bnds.size.width, bnds.size.height),
								  iAETouchPhase_Moved);
}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
	if (!_iOSTouchHandler) return;
	CGRect bnds = self.bounds;
	_iOSTouchHandler->sendTouches([self _collectInfoForTouches:touches],
								  dMakeRect(bnds.origin.x, bnds.origin.y, bnds.size.width, bnds.size.height),
								  iAETouchPhase_Ended);
}

- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event {
	if (!_iOSTouchHandler) return;
	CGRect bnds = self.bounds;
	_iOSTouchHandler->sendTouches([self _collectInfoForTouches:touches],
								  dMakeRect(bnds.origin.x, bnds.origin.y, bnds.size.width, bnds.size.height),
								  iAETouchPhase_Cancelled);
}


@end
