/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>

#import "ESRenderer.h"

BEGIN_NAMESPACE_IM
class iSysContext;
END_NAMESPACE_IM

@interface ES1Renderer : NSObject <ESRenderer> {
@private
	IM::iContext*		_iContext;
    EAGLContext*		_eaglContext;

    GLint				_backingWidth;
    GLint				_backingHeight;

	GLuint				_defaultFramebuffer;
	GLuint				_depthRenderbuffer;
	GLuint				_colorRenderbuffer;
}

- (ES1Renderer*)initWithOwner:(IM::iSysContext*)sysContext;

- (void)renderScene:(iScene*)scene;
- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer;

@end
