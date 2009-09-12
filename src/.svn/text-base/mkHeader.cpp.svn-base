/*
 *  mkHeader.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/19/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#include "mkHeader.h"

#include <iostream>
using namespace std;

short int endianSwap16(short int val)
{
	long int i = 1;
	const char *p = (const char *) &i;
	if (p[0] == 1)  // Lowest address contains the least significant byte
	{
		return (((val & 0xff00) >> 8) | ((val & 0x00ff) << 8));
	}
	return val;
}


namespace MonkSWF {
	
	bool Header::read( Reader* reader ) {
	
		_signature[0] = reader->get<uint8_t>();
		_signature[1] = reader->get<uint8_t>();
		_signature[2] = reader->get<uint8_t>();
		
		_version = reader->get<uint8_t>();
		
		_file_length = reader->get<uint32_t>();
		
		// get the bound rectangle
		int32_t nbits;
		nbits = (int32_t) reader->getbits( 5 );
		_frame_size.xmin = reader->getsignedbits( nbits );
		_frame_size.xmax = reader->getsignedbits( nbits );
		_frame_size.ymin = reader->getsignedbits( nbits );
		_frame_size.ymax = reader->getsignedbits( nbits );	
		
		uint16_t fr = reader->get<uint16_t>();
		_frame_rate = fr/256.0f;
		
		_frame_count = reader->get<uint16_t>(); 
				
		return true;
	}
	
	void Header::print() {
		cout << "signature: "		<< _signature[0] << _signature[1] << _signature[2] << endl;
		cout << "version: "			<< (int)_version << endl; 
		cout << "file length: "		<< _file_length << endl;
		cout << "frame width: "		<< ((_frame_size.xmax - _frame_size.xmin)/20.0f) << endl;
		cout << "frame height: "	<< ((_frame_size.ymax - _frame_size.ymin)/20.0f) << endl;
		cout << "frame rate: "		<< _frame_rate << endl;
		cout << "frame count: "		<< _frame_count << endl;
	}
	
}