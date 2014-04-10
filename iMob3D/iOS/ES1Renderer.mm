/*
 ** $Id: 2010/07/23, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#import "stdafx.h"
#import "ES1Renderer.h"
#import "iContext.h"
#import "iScene.h"

@implementation ES1Renderer

- (ES1Renderer*)initWithOwner:(IM::iSysContext*)sysContext {
    if ((self = [super init]) && sysContext) {
        _eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];

        if (!_eaglContext || ![EAGLContext setCurrentContext:_eaglContext]) {
            [self release];
            return nil;
        }

        glGenFramebuffersOES(1, &_defaultFramebuffer);
		glGenRenderbuffersOES(1, &_depthRenderbuffer);
        glGenRenderbuffersOES(1, &_colorRenderbuffer);
        glBindFramebufferOES(GL_FRAMEBUFFER_OES, _defaultFramebuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, _depthRenderbuffer);
        glBindRenderbufferOES(GL_RENDERBUFFER_OES, _colorRenderbuffer);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, _depthRenderbuffer);
        glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _colorRenderbuffer);
		glSignalIfError();
		
		_iContext = IM::iContext::alloc()->initWithSysContext(sysContext);
    }

    return self;
}

- (EAGLContext*)eaglContext {
	return _eaglContext;
}

- (IM::iContext*)iContext {
	return _iContext;
}

- (void)renderScene:(iScene*)scene {
    _iContext->makeCurrentContext();

    glBindFramebufferOES(GL_FRAMEBUFFER_OES, _defaultFramebuffer);
	
	scene->renderInContext(_iContext);

    glBindRenderbufferOES(GL_RENDERBUFFER_OES, _colorRenderbuffer);
    [_eaglContext presentRenderbuffer:GL_RENDERBUFFER_OES];
}

- (BOOL)resizeFromLayer:(CAEAGLLayer*)layer {	
    glBindRenderbufferOES(GL_RENDERBUFFER_OES, _colorRenderbuffer);
    [_eaglContext renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:layer];
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &_backingWidth);
    glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &_backingHeight);
	
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, _backingWidth, _backingHeight);

    if (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES));
        return NO;
    }
	
	_iContext->setViewport(dMakeRect(0, 0, _backingWidth, _backingHeight));

    return YES;
}

- (void)dealloc {
    if (_defaultFramebuffer) {
        glDeleteFramebuffersOES(1, &_defaultFramebuffer);
        _defaultFramebuffer = 0;
    }
	
	if (_depthRenderbuffer) {
		glDeleteRenderbuffersOES(1, &_depthRenderbuffer);
		_depthRenderbuffer = 0;
	}

    if (_colorRenderbuffer) {
        glDeleteRenderbuffersOES(1, &_colorRenderbuffer);
        _colorRenderbuffer = 0;
    }

    if ([EAGLContext currentContext] == _eaglContext)
        [EAGLContext setCurrentContext:nil];

    [_eaglContext release];
    _eaglContext = nil;
	
	CL_RELEASE(_iContext);

    [super dealloc];
}

@end
