/*
 ** $Id: 2010/12/16, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#ifndef _iAssetBundle_H
#define _iAssetBundle_H


BEGIN_NAMESPACE_IM

class iMesh;
class iTexture2D;
class iTextureCube;

class iAssetBundle : public dObject {
CL_CLASS_DECL(iAssetBundle, dObject)
public:
	virtual iAssetBundle*			initWithPath(dString* path);
	
	iMesh*							mesh(dInteger resID);
	iTexture2D*						texture2D(dInteger resID);
	iTextureCube*					textureCube(CArrayT<dInteger>& txtIDArray);	// must be array of 6 texture IDs in order: xPos, xNeg, yPos, yNeg, zPos, zNeg
	
	void							preloadMeshes(CArrayT<dInteger>& resIDs);
	void							preloadTextures(CArrayT<dInteger>& resIDs);
	void							preloadTexturesCube(CArrayT<CArrayT<dInteger> >& resIDs);
	void							unloadAll();
	
protected:
	iAssetBundle();
	virtual void					dealloc();
	
	dString*						_absolutePathByResID(dInteger resID);
	dArrayT<dString*>*				_readPathsFile();
	
	dString*						_absolutePath;
	dArrayT<dString*>*				_resPaths;
	
	dDictT<iMesh*, dValue*>*		_loadedMeshes;
	dDictT<iTexture2D*, dValue*>*	_loaded2DTextures;
	dDictT<iTextureCube*, dData*>*	_loadedCubeTextures;
};

END_NAMESPACE_IM

#endif // _iAssetBundle_H