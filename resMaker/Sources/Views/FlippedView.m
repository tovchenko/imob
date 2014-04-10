//
//  FlippedView.m
//  resMaker
//
//  Created by Taras Tovchenko on 10/28/10.
//  Copyright 2010 USSR Software. All rights reserved.
//

#import "FlippedView.h"


@implementation FlippedView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

- (BOOL)isFlipped {
	return YES;
}

@end
