/*
 *  RemoveObject.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 5/7/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#ifndef __RemoveObject_h__
#define __RemoveObject_h__
#include "mkTag.h"
#include <iostream>
using namespace std;

namespace MonkSWF {
	class RemoveObjectTag : public IRemoveObjectTag {
	public:
		RemoveObjectTag( TagHeader& h ) 
		: IRemoveObjectTag( h )
		{}
		
		virtual ~RemoveObjectTag() {
			
		}
		
		virtual bool read( Reader* reader ) {
			if( code() == REMOVEOBJECT )
				_character_id = reader->get<uint16_t>();
				
			_depth = reader->get<uint16_t>();
			
			return true;
		}
		
		virtual void print() {
			if( code() == REMOVEOBJECT )
				cout << "REMOVEOBJECT id = " << _character_id << " depth = " << _depth << endl;
			else
				cout << "REMOVEOBJECT2 depth = " << _depth << endl;
		}
		
		static ITag* create( TagHeader* header ) {
			return (ITag*)(new RemoveObjectTag( *header ));
		}				
	};
	
}	
#endif	// __RemoveObject_h__