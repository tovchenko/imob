/*
 ** $Id: 2010/05/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iBuffer_H
#define _iBuffer_H

BEGIN_NAMESPACE_IM

class iBuffer : public dObject {
CL_ABSTRACT_DECL(iBuffer, dObject)
public:
	virtual iBuffer*		initWithData(const dData* data, dUInteger itemSize);
	
	void					bind();

	dUInteger				itemsCount() const;
	dUInteger				itemSize() const;
	const dByte*			boundPointer() const;
	
	virtual void			draw(iEDrawMode mode, dInteger first, dUInteger count) = 0;
	
protected:
	iBuffer();
	virtual void			dealloc();
	
	virtual iEBufferType	_bufferType() = 0;
	static void				_unbind(iEBufferType bufferType);
	
	dUInteger				_itemsCount;
	dUInteger				_itemSize;
	dByte*					_boundPointer;
	
	dUInteger				_bufferID;
};

END_NAMESPACE_IM

#endif // _iBuffer_H