/*
 ** $Id: 2011/04/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iASprite.h"
#include "iAGUIComponent.h"
#include "iTexture.h"

CL_CLASS_DEF(iASprite, IM)

iASprite::iASprite()
: _position(0, 0) {
}

iASprite*
iASprite::spriteWithSize(iTexture2D* texture, const dSize& size) {
	_self* im = alloc()->initWithSize(texture, size);
	return im ? im->autorelease() : nil;
}

iASprite*
iASprite::spriteWithFrame(iTexture2D* texture, const dRect& frameOnTexture, const dSize& size) {
	_self* im = alloc()->initWithFrame(texture, frameOnTexture, size);
	return im ? im->autorelease() : nil;
}

iASprite*
iASprite::initWithSize(iTexture2D* texture, const dSize& size) {
	return initWithFrame(texture, dMakeRect(0, 0, texture->width(), texture->height()), size);
}

iASprite*
iASprite::initWithFrame(iTexture2D* texture, const dRect& frameOnTexture, const dSize& size) {
	_super::init();
	
	dobj_ptr<iAGUIComponent> gc(CL_NEW(iAGUIComponent));
	gc->setDelegate(this);
	gc->setOverlay(texture);
	setComponent(kiANodeGUIComponent, gc.get());
	
	_frameOnTexture = frameOnTexture;
	_size = size;
	
	return this;
}

void
iASprite::setPosition(const vec2f& pos) {
	_position = pos;
}

const vec2f&
iASprite::position() const {
	return _position;
}

void
iASprite::layout(iAGUIComponent* object, const dRect& viewport, dRect& outSrcFrame, dRect& outDstFrame) {
	outSrcFrame = _frameOnTexture;
	outDstFrame = dMakeRect(
                    _position.x() - 0.5f * _size.width,
                    _position.y() - 0.5f * _size.height,
                    _size.width,
                    _size.height);
}