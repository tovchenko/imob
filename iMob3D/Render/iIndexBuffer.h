/*
 ** $Id: 2010/05/13, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iIndexBuffer_H
#define _iIndexBuffer_H

#include "iBuffer.h"


BEGIN_NAMESPACE_IM

class iIndexBuffer : public iBuffer {
CL_CLASS_DECL(iIndexBuffer, iBuffer)
public:
	static iIndexBuffer*	bufferWithDataAndType(const dData* data, iEDataType type);
	virtual iIndexBuffer*	initWithDataAndType(const dData* data, iEDataType type);
	
	static void				unbind();
	
	static dUInteger		itemSizeByDataType(iEDataType type);
	iEDataType				dataType() const;
	
	virtual void			draw(iEDrawMode mode, dInteger first, dUInteger count);
	
protected:
	virtual iEBufferType	_bufferType();
	
	iEDataType				_dataType;
};

END_NAMESPACE_IM

#endif // _iIndexBuffer_H