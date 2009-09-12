/*
 *  reader.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/18/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#ifndef __reader_h__
#define __reader_h__

#include "mkCommon.h"
#include "mkTypes.h"

namespace MonkSWF {
	
	class Reader {
	public:
	
		Reader( char* data, int32_t sz )
			:	_data( data )
			,	_sz( sz )
			,	_cur( 0 )
			,	_bitpos( 0 )
		{}
		
		template< class T> T get() {
			if( _bitpos > 0 && _bitpos < 8)
				_cur++;
			_bitpos = 0;
			T *ret = (T*)(_data + _cur);
			_cur += sizeof( T );
			return *ret;
		}
		
		inline void skip( int32_t bytes ) {
			_cur += bytes;
		}
		
		inline uint32_t getbit() {

			uint32_t v = (_data[_cur] >> (7 - _bitpos++)) & 1;
			if( _bitpos == 8 )
				get<uint8_t>();
			
			return v;
		}
		
		inline uint32_t getbits( uint8_t nbits ) {
			uint32_t val = 0;
			for( int t = 0; t < nbits; t++ )
			{
				val <<= 1;
				val |= getbit();
			}
			return val;
		}
		
		inline int32_t getsignedbits( uint8_t nbits ) {
			uint32_t res = getbits( nbits );
			if ( res & ( 1 << ( nbits - 1 ) ) ) 
				res |= (0xffffffff << nbits);
				  
			return (int32_t)res;			
		}
		
		inline void getRectangle( RECT& rect ) {
			const int32_t nbits = (int32_t) getbits( 5 );
			rect.xmin = getsignedbits( nbits );
			rect.xmax = getsignedbits( nbits );
			rect.ymin = getsignedbits( nbits );
			rect.ymax = getsignedbits( nbits );	
		}
		
		inline void getMatrix( MATRIX& m ) {
			int32_t nbits;
			if ( getbits( 1 ) )	{	// has scale
				nbits = getbits( 5 );
				m.sx = getsignedbits( nbits );
				m.sy = getsignedbits( nbits );
			} else {
				m.sx = m.sy = 0x10000;
			}

			if ( getbits( 1 ) ) { // has rotation
				nbits = getbits( 5 );
				m.r0 = getsignedbits( nbits );
				m.r1 = getsignedbits( nbits );
			} else {
				m.r0 = m.r1 = 0x0;	
			}

			nbits = getbits( 5 );
			m.tx = getsignedbits( nbits );
			m.ty = getsignedbits( nbits );
		}
		
		inline int32_t getCurrentPos() const {
			return _cur;
		}
		
		inline uint8_t getBitPos() const {
			return _bitpos;
		}
		
	private:
	
		char*		_data;
		int32_t		_sz;		// total size of the buffer
		int32_t		_cur;		// current ptr/idx into buffer
		uint8_t		_bitpos;	// for reading bits
	};
	
//	template<> char Reader::get<char>();
}
#endif // __reader_h__