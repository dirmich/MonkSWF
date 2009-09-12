//
//  MainView.m
//  MonkSWF-Test-iPhone
//
//  Created by Micah Pearlman on 3/24/09.
//  Copyright Monk Games 2009. All rights reserved.
//

#import "MainView.h"
#include "MonkSWf.h"
#include "VG/openvg.h"
#include "VG/vgu.h"

#include <iostream>
#include <fstream>
using namespace std;
using namespace MonkSWF;


@implementation MainView


- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		_current_frame_idx = 0;
    }
    return self;
}


- (void)drawRect:(CGRect)rect {
	if( _swf ) {
		int32_t num_frames = _swf->getNumFrames();
		if( _current_frame_idx >= num_frames )
			_current_frame_idx = 0;
		_swf->drawFrame( _current_frame_idx );
		_current_frame_idx++;
	}
		
//	// Custom initialization
//	vgCreateContextSH(320,480);
//
//    // Drawing code
//    NSBundle      *mainBundle = [NSBundle mainBundle];
//    NSString      *fromFilePath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"puppy.swf"];
//    NSFileHandle  *fromFile = [NSFileHandle fileHandleForReadingAtPath:fromFilePath];;
//    if (fromFile) {
//        NSData *data = [fromFile readDataToEndOfFile];
//		int length = [data length];
//		char* buffer = new char[length];
//		[data getBytes:buffer];
//		
//		Reader r( buffer, length );
//		
//		SWF *swf = new SWF();
//		swf->initialize();
//		swf->read( &r );
//		swf->print();
//		swf->drawFrame( 0 );
//		
//		delete [] buffer;
//		
//    }
//	
//    [fromFile closeFile];
	
	
}

-(void)setSWF:(MonkSWF::SWF*)swf {
	_swf = swf;
}

- (void)dealloc {
    [super dealloc];
}


@end
