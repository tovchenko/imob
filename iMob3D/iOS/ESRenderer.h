/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#import <QuartzCore/QuartzCore.h>

BEGIN_NAMESPACE_IM
class iContext;
class iScene;
END_NAMESPACE_IM

@protocol ESRenderer <NSObject>

- (EAGLContext*)eaglContext;
- (IM::iContext*)iContext;

- (void)renderScene:(IM::iScene*)scene;
- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer;

@end
