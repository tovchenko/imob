//
//  TestAppDelegate.h
//  Test
//
//  Created by Taras Tovchenko on 8/24/11.
//  Copyright 2011 USSR Software. All rights reserved.
//

#import <UIKit/UIKit.h>

@class TestViewController;

@interface TestAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet TestViewController *viewController;

@end
