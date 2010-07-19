/*
 *  DoABC.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 5/22/10.
 *  Copyright 2010 Zero Vision. All rights reserved.
 *
 */

#ifndef __DoABC_h__
#define __DoABC_h__
#include "mkTag.h"

namespace MonkSWF {
	
	class DoABC	: public IDoABC {
	public:
		DoABC( TagHeader& h ) 
		: IDoABC( h )
		{}
		
		virtual ~DoABC() {
			
		}
		
		virtual bool read( Reader* reader ); 
		virtual void print() {
			
		}
		
		static ITag* create( TagHeader* header ) {
			return (ITag*)(new DoABC( *header ));
		}				
	};
}


#endif