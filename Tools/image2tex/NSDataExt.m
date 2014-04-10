//
//  NSDataExt.m
//  image2tex
//
//  Created by DoctorSet on 8/18/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "NSDataExt.h"
#import <zlib.h>


enum { compressed_signature = 'zdat' };


@implementation NSData (NSDataExt)

- (NSData*)compressedData {
	uLong sourceLen = [self length];
	uLongf destLen = compressBound(sourceLen);
	NSMutableData* data = [NSMutableData dataWithLength:destLen + 8];
	
	if (Z_OK == compress([data mutableBytes], &destLen, [self bytes], sourceLen)) {
		[data setLength:destLen + 8];
		Ptr ptr = [data mutableBytes];
		*(uint32_t*)(ptr + destLen) = CFSwapInt32HostToLittle(sourceLen);
		*(uint32_t*)(ptr + destLen + 4) = CFSwapInt32HostToLittle(compressed_signature);
	} else {
		return nil;
	}
	
	return data;
}

- (NSData*)uncompressedData {
	uLong sourceLen = [self length] - 8;
	uint32_t* signs = (uint32_t*)([self bytes] + sourceLen);
	if (signs[1] != CFSwapInt32HostToLittle(compressed_signature)) return nil;
	
	uLongf destLen = CFSwapInt32HostToLittle(signs[0]);
	NSMutableData* data = [NSMutableData dataWithLength:destLen];
	
	return (Z_OK == uncompress([data mutableBytes], &destLen, [self bytes], sourceLen)) ? data : nil;
}

@end
