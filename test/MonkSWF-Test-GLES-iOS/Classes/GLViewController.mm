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
	NSString      *fromFilePath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"puppy.swf"];
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
	
	vgCreateContextSH( 320, 480 );

	_shapeIdx = 0;
	_shape = _swf->shapeAt( _shapeIdx );
	
	_spriteIdx = 0;
	_frame = 0;
	_sprite = 0;//_swf->spriteAt( _spriteIdx );
	
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

- (void)drawView:(id)sender {
	vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
	vgLoadIdentity();
	vgTranslate(320/2,480/2);
//	vgScale(0.2, 0.2);
	
//	_swf->drawFrame( _frame++ );
//	if ( _frame >= _swf->numFrames() ) {
//		_frame = 0;
//	}
	if ( _sprite ) {
		_sprite->draw( _frame++ );
		if ( _frame >= _sprite->frameCount() ) {
			_frame = 0;
		}
		
	} else if ( _shape ) {
		_shape->draw();
	}
	
}

#pragma mark Actions

- (IBAction) incrementShape:(id)sender {
	int c = _swf->numShapes();

	_shapeIdx++;
	if ( _shapeIdx >= c) {
		_shapeIdx = 0;
	}

	_shape = _swf->shapeAt( _shapeIdx );
	
}
- (IBAction) decrementShape:(id)sender {
	int c = _swf->numShapes();
	
	_shapeIdx--;
	if ( _shapeIdx < 0 ) {
		_shapeIdx = c - 1;
	}
	
	_shape = _swf->shapeAt( _shapeIdx );
	
}


- (void)dealloc {
    [super dealloc];
}


@end
