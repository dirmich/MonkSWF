//
//  RenderTexture.m
//  MonkSWF-Test-GLES-iOS
//
//  Created by Micah Pearlman on 10/26/10.
//  Copyright 2010 Zero Vision. All rights reserved.
//

#import "RenderTexture.h"
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>


@implementation RenderTexture

@synthesize texture = _texture;

- (id) initWithSize:(CGSize)size {
	if ( self = [super init] ) {
		// create the draw to texture
		void* drawBufferData = malloc(size.width*size.height*4);
		memset(drawBufferData, 0xff000000, size.width*size.height*4);
		_texture = [[Texture2D alloc] initWithData:drawBufferData 
									   pixelFormat:kTexture2DPixelFormat_RGBA8888 
										pixelsWide:size.width
										pixelsHigh:size.height
									   contentSize:size];
		
		free(drawBufferData);
		drawBufferData = 0;
		
		// get the old frame buffer
		glGetIntegerv( GL_FRAMEBUFFER_BINDING_OES, &_oldFrameBuffer );

		// create framebuffer
		glGenFramebuffersOES(1, &_frameBuffer);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, _frameBuffer);
		
		// attach renderbuffer
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, _texture.name, 0);
		
		if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES)
			NSLog(@"incomplete");
		
		// unbind frame buffer
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, _oldFrameBuffer);
		
	}
	
	return self;
}

- (void) startDraw {
	// get the old frame buffer
	glGetIntegerv( GL_FRAMEBUFFER_BINDING_OES, &_oldFrameBuffer );

	// bind the draw to texture frame buffer
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _frameBuffer );
	
	// todo: let user set clear color
	glClearColor(226.0/255.0, 219.0f/255.0f, 207.0f/255.0f, 1.0);
	//glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	
}

- (void) endDraw {
	glFlush();
	// unbind
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _oldFrameBuffer );
}


- (void) dealloc {
	[_texture release];
	_texture = nil;
	
	glDeleteFramebuffers( 1, &_frameBuffer );
	_frameBuffer = 0;
	
	[super dealloc];
}
@end
