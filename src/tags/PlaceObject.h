/*
 *  PlaceObject.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/26/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#ifndef __PlaceObject_h__
#define __PlaceObject_h__

#include "mkTag.h"
#include <vg/openvg.h>
#include <vg/vgu.h>

namespace MonkSWF {

	class PlaceObject2Tag : public IPlaceObjectTag {
	public:
		PlaceObject2Tag( TagHeader& h ) 
			:	IPlaceObjectTag( h )
			,	_name(0)
		{}
		
		virtual ~PlaceObject2Tag() {
			if ( _name ) {
				delete [] _name;
				_name = 0;
			}
		}
		
		virtual bool read( Reader* reader );
		virtual void print();
		virtual void draw( SWF* swf );
		virtual void copyTransform( IPlaceObjectTag* other );
		
		static ITag* create( TagHeader* header ) {
			return (ITag*)(new PlaceObject2Tag( *header ));
		}
		

	private:
	
		VGfloat		_translation[2];
		VGfloat		_scale[2];
		VGfloat		_rotation;
		char*		_name;
	};
}
#endif // __PlaceObject_h__