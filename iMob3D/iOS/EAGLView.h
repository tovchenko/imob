/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>


BEGIN_NAMESPACE_IM
class iOSContext;
class iOSTouchAgent;
END_NAMESPACE_IM

// This class wraps the CAEAGLLayer from CoreAnimation into a convenient UIView subclass.
// The view content is basically an EAGL surface you render your OpenGL scene into.
// Note that setting the view non-opaque will only work if the EAGL surface has an alpha channel.
@interface EAGLView : UIView {
@private
	IM::iOSContext*		_iOSContext;
	IM::iOSTouchAgent*	_iOSTouchHandler;

    BOOL				_animating;
    BOOL				_displayLinkSupported;
    NSInteger			_animationFrameInterval;

    id					_displayLink;
    NSTimer*			_animationTimer;
}

@property(nonatomic, readonly) IM::iOSContext*			context;
@property(nonatomic, assign) IM::iOSTouchAgent*			touchHandler;
@property(readonly, nonatomic, getter=isAnimating) BOOL animating;
@property(nonatomic) NSInteger							animationFrameInterval;

- (void)startAnimation;
- (void)stopAnimation;
- (void)drawView:(id)sender;

@end
