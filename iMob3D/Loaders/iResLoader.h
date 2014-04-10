/*
 ** $Id: 2010/08/17, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iResLoader_H
#define _iResLoader_H

BEGIN_NAMESPACE_IM

class iMesh;
class iTexture2D;
class iTextureCube;

class iResLoader : public dObject {
CL_CLASS_DECL_AS_SINGLETON(iResLoader, dObject)
public:
	iMesh*			loadMesh(dString* filePath); // .mesh
	iTexture2D*		loadTexture2D(dString* filePath, const dDictT<dValue*, dString*>* pvrtcParams = nil); // .pvr .pvrtc .tex .565
	iTextureCube*	loadTextureCube(dArrayT<dString*>* filePaths, const dArrayT<dDictT<dValue*, dString*>*>* pvrtcParams = nil);
};

END_NAMESPACE_IM

#endif // _iResLoader_H