//
//  MonkSWF_Test_GLES_iOSAppDelegate.h
//  MonkSWF-Test-GLES-iOS
//
//  Created by Micah Pearlman on 8/28/10.
//  Copyright Zero Vision 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;
@class GLViewController;


@interface MonkSWF_Test_GLES_iOSAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
@private	
	GLViewController*	_glViewController;
	
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet GLViewController* glViewController;

@end

