/*
 *  ShowFrame.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 5/6/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#ifndef __ShowFrame_h__
#define __ShowFrame_h__

#include "mkTag.h"

namespace MonkSWF {
	
	class ShowFrameTag : public IShowFrameTag {
	public:
		ShowFrameTag( TagHeader& h ) 
		: IShowFrameTag( h )
		{}
		
		virtual ~ShowFrameTag() {
			
		}
		
		virtual bool read( Reader* reader ) {
			return true;
		}
		virtual void print() {
			
		}
		
		static ITag* create( TagHeader* header ) {
			return (ITag*)(new ShowFrameTag( *header ));
		}				
	};
}

#endif	// __ShowFrame_h__