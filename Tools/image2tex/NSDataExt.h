//
//  NSDataExt.h
//  image2tex
//
//  Created by DoctorSet on 8/18/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSData (NSDataExt)

- (NSData*)compressedData;
- (NSData*)uncompressedData;

@end
