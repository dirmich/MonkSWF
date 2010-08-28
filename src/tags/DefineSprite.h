/*
 *  DefineSprite.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 8/28/10.
 *  Copyright 2010 MP Engineering. All rights reserved.
 *
 */

#ifndef __DefineSprite_h__
#define __DefineSprite_h__

#include "mkTag.h"
#include <VG/openvg.h>

namespace MonkSWF {

	class DefineSpriteTag : public IDefineSpriteTag {
	public:
		DefineSpriteTag( TagHeader& h )
		:	IDefineSpriteTag( h )
		{}
		
		virtual ~DefineSpriteTag() {
			
		}
		
		virtual bool read( Reader* reader );
		virtual void print();
		
		virtual void draw();
		
		// regular DefineSpriteTag tag == 39
		static ITag* create( TagHeader* header );
		
	private:
		
		
	};
}
#endif // __DefineSprite_h__
