//
//  RenderTexture.h
//  MonkSWF-Test-GLES-iOS
//
//  Created by Micah Pearlman on 10/26/10.
//  Copyright 2010 Zero Vision. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>

#import "Texture2D.h"

@interface RenderTexture : NSObject {
@private
	Texture2D*		_texture;
	GLuint			_frameBuffer;
	GLint			_oldFrameBuffer;
}

@property (readonly) Texture2D* texture;

- (id) initWithSize:(CGSize)size;
- (void) startDraw;
- (void) endDraw;



@end
