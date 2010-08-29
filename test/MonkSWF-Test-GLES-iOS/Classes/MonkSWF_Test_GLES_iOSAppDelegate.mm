//
//  MonkSWF_Test_GLES_iOSAppDelegate.m
//  MonkSWF-Test-GLES-iOS
//
//  Created by Micah Pearlman on 8/28/10.
//  Copyright Zero Vision 2010. All rights reserved.
//

#import "MonkSWF_Test_GLES_iOSAppDelegate.h"
#import "EAGLView.h"
#import "GLViewController.h"

@implementation MonkSWF_Test_GLES_iOSAppDelegate

@synthesize window;
@synthesize glViewController = _glViewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	[window addSubview:_glViewController.view];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)applicationWillTerminate:(UIApplication *)application
{
}

- (void)dealloc
{
    [window release];
    [_glViewController release];

    [super dealloc];
}

@end
