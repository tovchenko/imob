/*
 ** $Id: 2010/08/08, Taras Tovchenko $
 ** See Copyright Notice in iMob.h
 */

#import <UIKit/UIKit.h>
#import <dCocoa.h>

int main(int argc, char *argv[]) {
    _LocalPool;
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
