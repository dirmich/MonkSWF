/*
 *  mkHeader.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/19/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#ifndef __mkHeader_h__
#define __mkHeader_h__

#include "mkCommon.h"
#include "mkReader.h"
#include "mkTypes.h"

namespace MonkSWF {
	
	class Header {
	public:
		bool read( Reader* reader );
		void print();
		
		float getFrameWidth() const {
			return ((_frame_size.xmax - _frame_size.xmin)/20.0f);
		}
		float getFrameHeight() const {
			return ((_frame_size.ymax - _frame_size.ymin)/20.0f);
		}		
	
	private:
		uint8_t		_signature[3];
		uint8_t		_version;
		uint32_t	_file_length;
		RECT		_frame_size;
		float		_frame_rate;
		int16_t		_frame_count;
	
	};
}
#endif // __mkHeader_h__
