/*
 ** $Id: 2010/05/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iVertexBuffer_H
#define _iVertexBuffer_H

#include "iBuffer.h"

BEGIN_NAMESPACE_IM


class iVertexBuffer : public iBuffer {
CL_CLASS_DECL(iVertexBuffer, iBuffer)
public:
	static iVertexBuffer*	bufferWithDataAndFVF(const dData* data, iEFVF fvf);
	virtual iVertexBuffer*	initWithDataAndFVF(const dData* data, iEFVF fvf);
	
	static void				unbind();
	
	iEFVF					fvf() const;
	static dUInteger		itemSizeByFVF(iEFVF fvf);
	
	virtual void			draw(iEDrawMode mode, dInteger first, dUInteger count);
	
protected:
	iVertexBuffer();
	
	virtual iEBufferType	_bufferType();
	
	iEFVF					_fvf;
};

END_NAMESPACE_IM

#endif // _iVertexBuffer_H