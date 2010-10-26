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
#include <map>

namespace MonkSWF {

	class DefineSpriteTag : public IDefineSpriteTag {
	public:
		DefineSpriteTag( TagHeader& h )
		:	IDefineSpriteTag( h )
		,	_scale(1.0f)
		{
			_translate[0] = _translate[1] = 0.0f;
		}
		
		virtual ~DefineSpriteTag() {
			
		}
		
		virtual bool read( Reader* reader );
		virtual void print();
		
		virtual void draw( const int32_t frame );
		
		virtual void setTranslate( float t[2] ) {
			_translate[0] = t[0];
			_translate[1] = t[1];
		}
		virtual void getTranslate( float t[2] ) const  {
			t[0] = _translate[0];
			t[1] = _translate[1];
		}
		virtual void setScale( float s ) {
			_scale = s;
		}
		virtual float scale() const {
			return _scale;
		}
		
		
		// regular DefineSpriteTag tag == 39
		static ITag* create( TagHeader* header );
		
	private:
		typedef std::map< uint16_t, IPlaceObjectTag* >		DisplayList;
		typedef DisplayList::iterator						DisplayListIter;

		DisplayList			_display_list;
		
		float		_translate[2];
		float		_scale;
		
	};
}
#endif // __DefineSprite_h__
