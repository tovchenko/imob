#import <Foundation/Foundation.h>

#import "OBJData.h"

static void		_createDirectoriesIfNeeded(NSString* path);

int
main (int argc, const char* argv[]) {
    NSAutoreleasePool* pool = [NSAutoreleasePool new];
	
	BOOL generateMesh = NO;
	BOOL storeNormals = NO;
	BOOL storeTexCoords = NO;
	BOOL generateCollider = NO;
	BOOL isOutputArg = NO;
	BOOL breaked = NO;
	BOOL isDir1 = NO;
	BOOL isDir2 = NO;
	BOOL isDir3 = NO;
	
	NSString* meshOutputPath = nil;
	NSString* colliderOutputPath = nil;
	NSString* sourcePath = nil;
	
	for (NSInteger argi = 1; argi < argc; ++argi) {
		NSString* arg = [NSString stringWithCString:argv[argi] encoding:NSMacOSRomanStringEncoding];
		
		if ([arg hasPrefix:@"-"]) {
			NSString* option = [arg substringFromIndex:1];
			
			if ([option isEqualToString:@"mesh"]) generateMesh = YES;
			else if ([option isEqualToString:@"n"]) storeNormals = YES;
			else if ([option isEqualToString:@"t"]) storeTexCoords = YES;
			else if ([option isEqualToString:@"collider"]) generateCollider = YES;
			else if ([option isEqualToString:@"o"]) isOutputArg = YES;
		} else {
			if (isOutputArg) {
				isOutputArg = NO;
				
				if ((generateMesh && !meshOutputPath && generateCollider && !colliderOutputPath) ||
					(generateCollider && !colliderOutputPath && generateMesh && !meshOutputPath))
				{
					breaked = YES;
					break;
				}
				
				if (generateMesh && !meshOutputPath) {
					[[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir1];
					meshOutputPath = arg;
				} else if (generateCollider && !colliderOutputPath) {
					[[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir2];
					colliderOutputPath = arg;
				} else {
					breaked = YES;
					break;
				}
			} else if (!sourcePath && [[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir3]) {
				sourcePath = arg;
			} else {
				breaked = YES;
				break;
			}
		}
	}
	
	BOOL isDirectory = ((generateMesh ? isDir1 : YES) && (generateCollider ? isDir2 : YES) && isDir3);
	BOOL isOneFile = ((generateMesh ? !isDir1 : YES) && (generateCollider ? !isDir2 : YES) && !isDir3);
	if (!breaked && sourcePath && (meshOutputPath || colliderOutputPath) &&
		(isDirectory || isOneFile))
	{
		if (isDirectory) {
			NSArray* fileNames = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:sourcePath error:nil];
			
			for (NSString* fileName in fileNames) {
				if ([fileName hasPrefix:@"."]) continue; // skip hidden files
				
				if ([[fileName pathExtension] isEqualToString:@"obj"]) {
					NSAutoreleasePool* pool2 = [NSAutoreleasePool new];
					
					NSString* objPath = [sourcePath stringByAppendingPathComponent:fileName];
					OBJData* objData = [OBJData dataWithPath:objPath];
					NSString* nameWithoutExt = [fileName stringByDeletingPathExtension];
					
					if (generateMesh) {
						NSString* outPath = [[meshOutputPath stringByAppendingPathComponent:nameWithoutExt] stringByAppendingPathExtension:@"mesh"];
						NSData* meshData = [objData composedRenderGeometryWithNormals:storeNormals textureCoords:storeTexCoords];
						[meshData writeToFile:outPath atomically:YES];
					}
					
					if (generateCollider) {
						NSString* outPath = [[colliderOutputPath stringByAppendingPathComponent:nameWithoutExt] stringByAppendingPathExtension:@"cld"];
						NSData* physData = [objData composedPhysGeometry];
						[physData writeToFile:outPath atomically:YES];
					}
					
					[pool2 drain];
				}
			}
		} else {
			OBJData* objData = [OBJData dataWithPath:sourcePath];
			
			if (generateMesh) {
				_createDirectoriesIfNeeded([meshOutputPath stringByDeletingLastPathComponent]);
				NSData* meshData = [objData composedRenderGeometryWithNormals:storeNormals textureCoords:storeTexCoords];
				[meshData writeToFile:meshOutputPath atomically:YES];
			}
			
			if (generateCollider) {
				_createDirectoriesIfNeeded([colliderOutputPath stringByDeletingLastPathComponent]);
				NSData* physData = [objData composedPhysGeometry];
				[physData writeToFile:colliderOutputPath atomically:YES];
			}
		}
	} else {
		printf("Usage:\n");
		printf("\t obj2mesh [-mesh [-n] [-t] -o dst_path1] [-collider -o dst_path2] src_path\n");
		printf("-mesh \t generate mesh for rendering\n");
		printf("-n \t store normals to vertex buffer\n");
		printf("-t \t store texture coords to vertex buffer\n");
		printf("dst_path1 \t path to folder with .obj files\n");
		printf("-collider \t generate physics collider for collision checking\n");
		printf("dst_path2 \t destination for colliders\n");
		printf("src_path \t path to .obj\n");
	}
    
    [pool drain];
    return 0;
}


static void
_createDirectoriesIfNeeded(NSString* path) {
	if (![[NSFileManager defaultManager] fileExistsAtPath:path]) {
		NSError* error = nil;
		if (![[NSFileManager defaultManager] createDirectoryAtPath:path
									   withIntermediateDirectories:YES
														attributes:nil
															 error:&error])
		{
			[NSException raise:@"Exception!" format:@"Can\'t create directory"];
		}
	}
}
