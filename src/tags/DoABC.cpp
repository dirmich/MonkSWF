/*
 *  DoABC.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 5/22/10.
 *  Copyright 2010 Zero Vision. All rights reserved.
 *
 */

#include "DoABC.h"

namespace MonkSWF {
	bool DoABC::read( Reader* reader ) {
		_flags = reader->get<uint32_t>();
		_name = reader->getString();
		
		return true;
	}
}