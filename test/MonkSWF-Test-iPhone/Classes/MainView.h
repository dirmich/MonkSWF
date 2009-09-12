//
//  MainView.h
//  MonkSWF-Test-iPhone
//
//  Created by Micah Pearlman on 3/24/09.
//  Copyright Monk Games 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#include "MonkSWF.h"
@interface MainView : UIView {
	MonkSWF::SWF	*_swf;
	int				_current_frame_idx;
}
-(void)setSWF:(MonkSWF::SWF*)swf;
@end
