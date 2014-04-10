#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import <Accelerate/Accelerate.h>


static NSData*		_flipImage(NSData* srcData);
static void			_createDirectoriesIfNeeded(NSString* path);


int
main(int argc, const char* argv[]) {
    NSAutoreleasePool* pool = [NSAutoreleasePool new];
	
	BOOL isOutputArg = NO;
	NSString* outputPath = nil;
	NSString* sourcePath = nil;
	BOOL breaked = NO;
	BOOL isDir1 = NO;
	BOOL isDir2 = NO;
	
	for (NSInteger argi = 1; argi < argc; ++argi) {
		NSString* arg = [NSString stringWithCString:argv[argi] encoding:NSMacOSRomanStringEncoding];
		
		if ([arg isEqualToString:@"-o"]) {
			isOutputArg = YES;
		} else {
			if (isOutputArg) {
				if (!outputPath) {
					[[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir1];
					outputPath = arg;
				}
				isOutputArg = NO;
			} else if (!sourcePath) {
				[[NSFileManager defaultManager] fileExistsAtPath:arg isDirectory:&isDir2];
				sourcePath = arg;
			} else {
				printf("Wrong usage!\n");
				breaked = YES;
				break;
			}
		}
	}
	
	BOOL isFolders = isDir1 && isDir2;
	BOOL isFiles = !isDir1 && !isDir2;
	if (!breaked && (isFolders || isFiles)) {
		if (isFolders) {
			NSArray* fileNames = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:sourcePath error:nil];
			for (NSString* fileName in fileNames) {
				if ([fileName hasPrefix:@"."]) continue; // skip hidden files
				
				NSAutoreleasePool* pool2 = [NSAutoreleasePool new];
				
				NSString* filePath = [sourcePath stringByAppendingPathComponent:fileName];
				NSData* imgData = _flipImage([NSData dataWithContentsOfFile:filePath]);
				NSString* fileExt = [fileName pathExtension];
				fileName = [[[fileName stringByDeletingPathExtension] stringByAppendingString:@"_flipped"] stringByAppendingPathExtension:fileExt];
				[imgData writeToFile:[outputPath stringByAppendingPathComponent:fileName] atomically:YES];
				
				[pool2 drain];
			}
		} else {
			NSString* targetDir = [outputPath stringByDeletingLastPathComponent];
			if (![[NSFileManager defaultManager] fileExistsAtPath:targetDir]) {
				NSError* error = nil;
				if (![[NSFileManager defaultManager] createDirectoryAtPath:targetDir
											   withIntermediateDirectories:YES
																attributes:nil
																	 error:&error])
				{
					[NSAlert alertWithError:error];
				}
			}
			
			_createDirectoriesIfNeeded([outputPath stringByDeletingLastPathComponent]);
			NSData* imgData = _flipImage([NSData dataWithContentsOfFile:sourcePath]);
			[imgData writeToFile:outputPath atomically:YES];
		}
	} else {
		printf("Usage:\n");
		printf("\t imageFlipper -o dst_path src_path\n");
	}
	
    [pool drain];
    return 0;
}


static NSData*
_flipImage(NSData* srcData) {
	NSBitmapImageRep* src = [NSBitmapImageRep imageRepWithData:srcData];
	NSBitmapImageRep* dst = [[src copy] autorelease];
	int8_t* const srcPixels = (int8_t*)[src bitmapData];
	int8_t* const dstPixels = (int8_t*)[dst bitmapData];
		
	const NSInteger widthInPixels = src.pixelsWide; 
	const NSInteger heightInPixels = src.pixelsHigh;
	const NSInteger bpp = src.bytesPerRow / widthInPixels;
	int8_t* srcIt = nil;
	int8_t* dstIt = nil;
	
	for (NSInteger row = 0; row < heightInPixels; ++row) {
		for (NSInteger column = 0; column < widthInPixels; ++column) {
			srcIt = srcPixels;
			dstIt = dstPixels;
			
			srcIt += bpp * (widthInPixels * row + column);
			dstIt += bpp * (widthInPixels * (heightInPixels - row - 1) + column);
			
			memcpy(dstIt, srcIt, bpp);
		}
	}
	
	return dst.TIFFRepresentation;
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
