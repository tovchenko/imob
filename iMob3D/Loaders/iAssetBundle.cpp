/*
 ** $Id: 2010/12/16, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#include "stdafx.h"
#include "iAssetBundle.h"
#include "iResLoader.h"
#include "iMesh.h"
#include "iTexture.h"

#define kCubeSideCount	6


CL_CLASS_DEF(iAssetBundle, IM)

iAssetBundle::iAssetBundle()
: _absolutePath(nil),
  _resPaths(nil),
  _loadedMeshes(nil),
  _loaded2DTextures(nil),
  _loadedCubeTextures(nil) {
}

iAssetBundle*
iAssetBundle::initWithPath(dString* path) {
	CL_THROW_IF_NIL(path);
	
	_super::init();
	_absolutePath = path->copy();
	return this;
}

void
iAssetBundle::dealloc() {
	CL_RELEASE(_absolutePath);
	CL_RELEASE(_resPaths);
	CL_RELEASE(_loadedMeshes);
	CL_RELEASE(_loaded2DTextures);
	CL_RELEASE(_loadedCubeTextures);
	
	_super::dealloc();
}

iMesh*
iAssetBundle::mesh(dInteger resID) {
	iMesh* result = nil;
	if (_loadedMeshes && (result = _loadedMeshes->objectForKeyNoException(dValue::valueWithInt(resID))))
		return result;
	
	result = iResLoader::singleton()->loadMesh(_absolutePathByResID(resID));
	if (!_loadedMeshes) _loadedMeshes = (dDictT<iMesh*, dValue*>*)dDictT<iMesh*, dValue*>::alloc()->init();
	_loadedMeshes->setObjectForKey(result, dValue::valueWithInt(resID));
	return result;
}

iTexture2D*
iAssetBundle::texture2D(dInteger resID) {
	iTexture2D* result = nil;
	if (_loaded2DTextures && (result = _loaded2DTextures->objectForKeyNoException(dValue::valueWithInt(resID))))
		return result;
	
	result = iResLoader::singleton()->loadTexture2D(_absolutePathByResID(resID));
	if (!_loaded2DTextures) _loaded2DTextures = (dDictT<iTexture2D*, dValue*>*)dDictT<iTexture2D*, dValue*>::alloc()->init();
	_loaded2DTextures->setObjectForKey(result, dValue::valueWithInt(resID));
	return result;
}

iTextureCube*
iAssetBundle::textureCube(CArrayT<dInteger>& txtIDArray) {
	CL_THROW_IF_TRUE(kCubeSideCount != txtIDArray.count());

	const dUInteger infoSize = kCubeSideCount * sizeof(dInteger);
	dInteger* keyInfo = (dInteger*)new dByte[infoSize];
	
	for (dInteger i = 0; i < kCubeSideCount; ++i)
		keyInfo[i] = txtIDArray.objectAtIndex(i);
	
	dobj_ptr<dData> key(dData::alloc()->initWithBytesNoCopy(keyInfo, infoSize, true));
	
	iTextureCube* result = nil;
	if (_loadedCubeTextures && (result = _loadedCubeTextures->objectForKeyNoException(key.get())))
		return result;
	
	dobj_ptr<dArrayT<dString*> > paths(CL_NEW(dArrayT<dString*>));
	for (dUInteger i = 0; i < kCubeSideCount; ++i)
		paths->addObject(_absolutePathByResID(keyInfo[i]));
	
	result = iResLoader::singleton()->loadTextureCube(paths.get());
	if (!_loadedCubeTextures) _loadedCubeTextures = (dDictT<iTextureCube*, dData*>*)dDictT<iTextureCube*, dData*>::alloc()->init();
	_loadedCubeTextures->setObjectForKey(result, key.get());
	return result;
}

void
iAssetBundle::preloadMeshes(CArrayT<dInteger>& resIDs) {
	for (dUInteger i = 0, cnt = resIDs.count(); i < cnt; ++i) {
		dUInteger resID = resIDs.objectAtIndex(i);
		iMesh* result = nil;
		
		if (!_loadedMeshes) _loadedMeshes = (dDictT<iMesh*, dValue*>*)dDictT<iMesh*, dValue*>::alloc()->init();
		if (!(result = _loadedMeshes->objectForKeyNoException(dValue::valueWithInt(resID)))) {
			result = iResLoader::singleton()->loadMesh(_absolutePathByResID(resID));
			_loadedMeshes->setObjectForKey(result, dValue::valueWithInt(resID));
		}
	}
}

void
iAssetBundle::preloadTextures(CArrayT<dInteger>& resIDs) {
	for (dUInteger i = 0, cnt = resIDs.count(); i < cnt; ++i) {
		dUInteger resID = resIDs.objectAtIndex(i);
		iTexture2D* result = nil;
		
		if (!_loaded2DTextures) _loaded2DTextures = (dDictT<iTexture2D*, dValue*>*)dDictT<iTexture2D*, dValue*>::alloc()->init();
		if (!(result = _loaded2DTextures->objectForKeyNoException(dValue::valueWithInt(resID)))) {
			result = iResLoader::singleton()->loadTexture2D(_absolutePathByResID(resID));
			_loaded2DTextures->setObjectForKey(result, dValue::valueWithInt(resID));
		}
	}
}

void
iAssetBundle::preloadTexturesCube(CArrayT<CArrayT<dInteger> >& resIDs) {
}

void
iAssetBundle::unloadAll() {
	if (_loadedMeshes) _loadedMeshes->removeAllObjects();
	if (_loaded2DTextures) _loaded2DTextures->removeAllObjects();
	if (_loadedCubeTextures) _loadedCubeTextures->removeAllObjects();
}

dString*
iAssetBundle::_absolutePathByResID(dInteger resID) {
	if (!_resPaths)
		_resPaths = _readPathsFile()->retain();
	
	dString* path = _resPaths->objectAtIndex(resID);
	if (!path->isAbsolutePath()) {
		path = _absolutePath->stringByAppendingPathComponent(path);
		_resPaths->replaceObjectAtIndexWithObject(resID, path);
	}
	
	return path;
}

dArrayT<dString*>*
iAssetBundle::_readPathsFile() {
	dString* pathToPathsFile = _absolutePath->stringByAppendingPathComponent(_ST("Meta"))
		->stringByAppendingPathComponent(_ST("paths")
		->stringByAppendingPathExtension(_ST("dat")));
	
	dobj_ptr<dData> fileData(dData::alloc()->initWithContentsOfFile(pathToPathsFile));
	dArrayT<dString*>* result = CL_NEW(dArrayT<dString*>)->autorelease();
	const dByte* buffer = (const dByte*)fileData->bytes();
	dUInteger len = fileData->length();
	
	dUInteger it = 0;
	while (it < len) {
		unsigned short len = dSwapLittleShortToHost(*(unsigned short*)(buffer + it));
		it += sizeof(len);
		dobj_ptr<dData> data(dData::alloc()->initWithBytes((const void*)(buffer + it), len));
		it += len;
		
		dobj_ptr<dString> str(dString::alloc()->initWithData(data.get()));
		result->addObject(str.get());
	}
	
	return result;
}