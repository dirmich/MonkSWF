/*
 *  mkTag.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/20/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#include "mkTag.h"
#include <iostream>
using namespace std;

namespace MonkSWF {
	
	bool TagHeader::read( Reader* reader ) {
		uint16_t tagcode_and_length = reader->get<uint16_t>();
		
		_code = (tagcode_and_length >> 6 );
		_length = (tagcode_and_length & 0x3F );
		if( _length == 0x3f )	// if long tag read an additional 32 bit length value
			_length = reader->get<uint32_t>();
				
		return true;
	}
	
	void TagHeader::print() {
		cout << "code: "	<< _code << endl;
		cout << "length: "	<< _length << endl;
	}
}