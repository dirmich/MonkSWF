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
	NSString      *fromFilePath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"fish_idle2.swf"];
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

	vgCreateContextSH( _glview.frame.size.width, _glview.frame.size.height);


	_shapeIdx = 0;
	_shape = _swf->shapeAt( _shapeIdx );
	
	_spriteIdx = 0;
	_frame = 0;
	_sprite = _swf->spriteAt( _spriteIdx );
	
	_lastTime = [NSDate timeIntervalSinceReferenceDate];
	
	
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
}


- (void)preDrawView:(id)sender {
}
- (void)postDrawView:(id)sender {
	
}

- (void)drawView:(id)sender {
	
	static VGfloat angle = 0;
	angle += 0.1;
	if( angle > 360 )
		angle = 0;
	
	float offset[2] = { _glview.frame.size.width*2/2, _glview.frame.size.height*2/2};
	
	NSTimeInterval deltaTime = [NSDate timeIntervalSinceReferenceDate] - _lastTime;
	if ( deltaTime > 1.0f / _swf->getFrameRate() ) {
		_frame++;
		_lastTime = [NSDate timeIntervalSinceReferenceDate];
	}
	if ( _frame >= _swf->numFrames() ) {
		_frame = 0;
	}
	vgLoadIdentity();
	vgRotate( angle );
	_swf->setOffsetTranslate( offset );
	_swf->setOffsetScale( 0.5f );
	_swf->drawFrame( _frame );
	
//	i think the camera is flipped in the wrong direction
//	either flip the camera or do y = height - y
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
