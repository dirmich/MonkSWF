//
//  GLViewController.mm
//  MonkSWF-Test-GLES-iOS
//
//  Created by Micah Pearlman on 8/28/10.
//  Copyright 2010 Zero Vision. All rights reserved.
//

#import "GLViewController.h"
#include <vg/openvg.h>
#include <vg/vgu.h>

using namespace MonkSWF;

@implementation GLViewController
@synthesize glview = _glview;


 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	NSBundle      *mainBundle = [NSBundle mainBundle];
	NSString      *fromFilePath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"giraffe.swf"];
	NSFileHandle  *fromFile = [NSFileHandle fileHandleForReadingAtPath:fromFilePath];;
	if (fromFile) {
		NSData *data = [fromFile readDataToEndOfFile];
		int length = [data length];
		char* buffer = new char[length];
		[data getBytes:buffer];
		
		Reader r( buffer, length );
		
		_swf = new SWF();
		_swf->initialize();
		_swf->read( &r );
		
		delete [] buffer;
		
	}
	
	[fromFile closeFile];

	_glview.delegate = self;

	vgCreateContextSH( _glview.frame.size.width*2, _glview.frame.size.height*2);
//	if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) {	
//		vgCreateContextSH( self.view.frame.size.width, self.view.frame.size.height );
//	} else { // iPhone
//		vgCreateContextSH( self.view.frame.size.width, self.view.frame.size.height );
//	}


	_shapeIdx = 0;
	_shape = _swf->shapeAt( _shapeIdx );
	
	_spriteIdx = 0;
	_frame = 0;
	_sprite = _swf->spriteAt( _spriteIdx );
	
	_lastTime = [NSDate timeIntervalSinceReferenceDate];
	
	//_background = [[Texture2D alloc] initWithImage:[UIImage imageNamed:@"IMG_1350.JPG"]];
	
	[_glview startAnimation];
	
}

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)postLayoutSubviews:(id)sender {
	_renderTexture =[[RenderTexture alloc] initWithSize: CGSizeMake( _glview.frame.size.width*2, _glview.frame.size.width*2) ];
}


- (void)preDrawView:(id)sender {
	
	if ( _renderTexture == nil ) 
		return;
	
	
	
	[_renderTexture startDraw];

	float offset[2] = { _glview.frame.size.width*2/16, _glview.frame.size.height*2/16};
	
	NSTimeInterval deltaTime = [NSDate timeIntervalSinceReferenceDate] - _lastTime;
	if ( deltaTime > 1.0f / _swf->getFrameRate() ) {
		_frame++;
		_lastTime = [NSDate timeIntervalSinceReferenceDate];
	}
	if ( _frame >= _swf->numFrames() ) {
		_frame = 0;
	}
	
	_swf->setOffsetTranslate( offset );
	_swf->setOffsetScale( 3.0 );
	_swf->drawFrame( _frame );
	

//	if ( _sprite ) {
//		_sprite->setScale( 2.0f );
//		_sprite->setTranslate( offset );
//		_sprite->draw( _frame );
//		
//		
//	} else if ( _shape ) {
//		
//		vgLoadIdentity();
//		vgTranslate( offset[0], offset[1] );
//		vgScale( 2.0f, 2.0f );
//
//		_shape->draw();
//	}
	
	[_renderTexture endDraw];
	
}
- (void)postDrawView:(id)sender {
	
}

- (void)drawView:(id)sender {
	
	if ( _renderTexture == nil ) 
		return;
	
//	[_renderTexture startDraw];
//	
//	if ( _sprite ) {
//		float offset[2] = { _glview.frame.size.width/2, _glview.frame.size.height/2};
//		_sprite->setTranslate( offset );
//		_sprite->draw( _frame );
//		
//		NSTimeInterval deltaTime = [NSDate timeIntervalSinceReferenceDate] - _lastTime;
//		if ( deltaTime > 1.0f / _swf->getFrameRate() ) {
//			_frame++;
//			_lastTime = [NSDate timeIntervalSinceReferenceDate];
//		}
//		if ( _frame >= _sprite->frameCount() ) {
//			_frame = 0;
//		}
//		
//	} else if ( _shape ) {
//		_shape->draw();
//	}
//	
//	[_renderTexture endDraw];
	
	// setup GL projection 
	glViewport(0,0, _glview.frame.size.width, _glview.frame.size.height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrthof(0, _glview.frame.size.width, _glview.frame.size.height, 0, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glDisable( GL_CULL_FACE );
	
	// turn on blending
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	
	
	
	CGRect rect = _glview.frame;
	rect.origin = CGPointMake(0, 0);

	[_renderTexture.texture drawInRect:rect 
//							   srcEnum:GL_ONE
//							   dstEnum:GL_ONE_MINUS_SRC_ALPHA
					textureEnvironment:GL_REPLACE 
								 flipY:NO];
	
	//[_renderTexture.texture drawInRect:rect textureEnvironment:GL_REPLACE flipY:NO];
	
//	vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
//	vgLoadIdentity();
//	vgTranslate(320/2,480/2);
//	vgScale(2.0, 2.0);
	
//	_swf->drawFrame( _frame++ );
//	if ( _frame >= _swf->numFrames() ) {
//		_frame = 0;
//	}
	
//	if ( _sprite ) {
//		float offset[2] = { _glview.frame.size.width/2, _glview.frame.size.height/2};
//		_sprite->setTranslate( offset );
//		_sprite->draw( _frame );
//		
//		NSTimeInterval deltaTime = [NSDate timeIntervalSinceReferenceDate] - _lastTime;
//		if ( deltaTime > 1.0f / _swf->getFrameRate() ) {
//			_frame++;
//			_lastTime = [NSDate timeIntervalSinceReferenceDate];
//		}
//		if ( _frame >= _sprite->frameCount() ) {
//			_frame = 0;
//		}
//		
//	} else if ( _shape ) {
//		_shape->draw();
//	}
	
}

#pragma mark Actions

- (IBAction) incrementShape:(id)sender {

	
	if ( _sprite ) {
		int c = _swf->numSprites();
		_shapeIdx++;
		if ( _shapeIdx >= c ) {
			_shapeIdx = 0;
		}
		_sprite = _swf->spriteAt( _shapeIdx );
	} else {
		int c = _swf->numShapes();
		
		_shapeIdx++;
		if ( _shapeIdx >= c) {
			_shapeIdx = 0;
		}
		
		_shape = _swf->shapeAt( _shapeIdx );
		
	}
	
}
- (IBAction) decrementShape:(id)sender {
	
	if ( _sprite ) {
		int c = _swf->numSprites();
		_shapeIdx--;
		if ( _shapeIdx < 0 ) {
			_shapeIdx = c - 1;
		}
		_sprite = _swf->spriteAt( _shapeIdx );
	} else {
		int c = _swf->numShapes();
		
		_shapeIdx--;
		if ( _shapeIdx < 0 ) {
			_shapeIdx = c - 1;
		}
		
		_shape = _swf->shapeAt( _shapeIdx );
		
	}
}

- (IBAction) incrementSprite:(id)sender {
	
}
- (IBAction) decrementSprite:(id)sender {
	
}



- (void)dealloc {
    [super dealloc];
}


@end
