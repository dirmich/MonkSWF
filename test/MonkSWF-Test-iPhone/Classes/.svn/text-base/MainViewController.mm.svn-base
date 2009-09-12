//
//  MainViewController.m
//  MonkSWF-Test-iPhone
//
//  Created by Micah Pearlman on 3/24/09.
//  Copyright Monk Games 2009. All rights reserved.
//

#include "MonkSWf.h"
#include "VG/openvg.h"
#include "VG/vgu.h"
using namespace MonkSWF;

#import "MainViewController.h"
#import "MainView.h"

#include <iostream>
#include <fstream>
using namespace std;

@implementation MainViewController


- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
		vgCreateContextSH(320,480);
		
		// Drawing code
		NSBundle      *mainBundle = [NSBundle mainBundle];
		NSString      *fromFilePath = [[mainBundle resourcePath] stringByAppendingPathComponent:@"motorcycle.swf"];
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
		
    }
    return self;
}



// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	[self.view setSWF:_swf];
	[NSTimer scheduledTimerWithTimeInterval:(1.0 / 12.0 ) target:self selector:@selector(renderScene) userInfo:nil repeats:YES];	
}

// Renders one scene of the game
- (void)renderScene {
	[self.view setNeedsDisplay];
}




/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning]; // Releases the view if it doesn't have a superview
    // Release anything that's not essential, such as cached data
}


- (void)dealloc {
    [super dealloc];
}


@end
