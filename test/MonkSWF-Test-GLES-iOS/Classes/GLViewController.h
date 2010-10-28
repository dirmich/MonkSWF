//
//  GLViewController.h
//  MonkSWF-Test-GLES-iOS
//
//  Created by Micah Pearlman on 8/28/10.
//  Copyright 2010 Zero Vision. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "EAGLView.h"
#import "MonkSWF.h"
#import "RenderTexture.h"

@interface GLViewController : UIViewController <EAGLViewDelegate> {
@private
	EAGLView*		_glview;
	MonkSWF::SWF*	_swf;
	MonkSWF::IDefineShapeTag*	_shape;	// current shape
	int							_shapeIdx;
	MonkSWF::IDefineSpriteTag*	_sprite;
	int							_spriteIdx;
	int							_frame;
	
	NSTimeInterval				_lastTime;
	
	RenderTexture*				_renderTexture;
	Texture2D*					_background;

}
@property (nonatomic, retain) IBOutlet EAGLView* glview;

- (IBAction) incrementShape:(id)sender;
- (IBAction) decrementShape:(id)sender;
@end
