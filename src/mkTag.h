/*
 *  mkTag.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/20/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#ifndef __mkTag_h__
#define __mkTag_h__

#include "mkReader.h"

namespace MonkSWF {
	
	class SWF;
	
	class TagHeader {
	public:
		
		inline uint32_t getCode() const {
			return _code;
		}
		
		inline uint32_t getLength() const {
			return _length;
		}
		
		bool read( Reader *reader );
		void print();
		
		
	private:
	
		uint32_t _code;	
		uint32_t _length;
	};
	
	class ITag {

	public:
		
		
		virtual bool read( Reader* reader ) = 0;
		virtual void print() = 0;
		
		const TagHeader& getHeader() {
			return _header;
		}
		
		uint32_t getCode() const {
			return _header.getCode();
		}
		
		int32_t getLength() const {
			return _header.getLength();
		}
		
	protected:

		ITag( TagHeader& header ) 
		:	_header( header ) 
		{}
		
		virtual ~ITag() {
			
		}
		
		TagHeader	_header;
	};
	
#define DEFINESHAPE 2
#define DEFINESHAPE2 22
#define DEFINESHAPE3 32
#define DEFINESHAPE4 83	
	class IDefineShapeTag : public ITag {
	public:
	
			virtual void draw() = 0;
			
			uint16_t getShapeId() const {
				return _shape_id;
			}
		
	
	protected:
	
		IDefineShapeTag( TagHeader& header ) 
		:	ITag( header )
		,	_shape_id( 0 )
		{}
		
		virtual ~IDefineShapeTag() {
			
		}
		
		uint16_t			_shape_id;
		
	
	};
#define PLACEOBJECT2 0x1A 
	class IPlaceObjectTag : public ITag {
	public:
		
		virtual void draw( SWF* swf ) = 0;
		virtual void copyTransform( IPlaceObjectTag* other ) = 0;
		virtual void copyNoTransform( IPlaceObjectTag* other ) { // copies everything except transform 
			_depth = other->_depth;
			_character_id = other->_character_id;
			_has_character = other->_has_character;
			_do_move = other->_do_move;
		}	
		uint16_t getDepth() const {
			return _depth;
		}
		
		static bool compare( IPlaceObjectTag* a, IPlaceObjectTag* b ) {
			return a->getDepth() < b->getDepth();
		}
		
		uint16_t getCharacterId() const {
			return _character_id;
		}
		
		bool doMove() {
			return !_has_character && _do_move;
		}
		
		
	protected:
		
		IPlaceObjectTag( TagHeader& header ) 
		:	ITag( header )
		,	_depth( 0 )
		,	_character_id( 0 )
		{}
		
		virtual ~IPlaceObjectTag() {
			
		}
		
		uint16_t	_depth;
		uint16_t	_character_id;
		uint8_t		_has_character;
		uint8_t		_do_move;
	};
	
#define SHOWFRAME 1	
	class IShowFrameTag : public ITag {
	protected:
		
		IShowFrameTag( TagHeader& header ) 
		:	ITag( header )
		{}
		
		virtual ~IShowFrameTag() {
			
		}
	};

#define REMOVEOBJECT	5
#define REMOVEOBJECT2	28
	class IRemoveObjectTag : public ITag {
	public:	
		uint16_t getCharacterId() const {
			return _character_id;
		}
		uint16_t getDepth() const {
			return _depth;
		}
	protected:
		
		IRemoveObjectTag( TagHeader& header ) 
		:	ITag( header )
		,	_character_id( 0 )
		{}
		
		virtual ~IRemoveObjectTag() {
			
		}
		
	protected:
	
		uint16_t	_character_id;
		uint16_t	_depth;
		
	};

#define DOABC	82
	class IDoABC : public ITag {
	public:
		
		uint32_t getFlags() const {
			return _flags;
		}
		char* getName() const {
			return _name;
		}
		uint8_t* getBytecode() const {
			return _bytecode;
		}
		
	protected:
		IDoABC( TagHeader& header )
		:	ITag( header )
		,	_flags( 0 )
		,	_name( 0 )
		,	_bytecode( 0 )
		{}
		virtual ~IDoABC() {
			if ( _name ) {
				delete [] _name;
				_name = 0;
			}
			if ( _bytecode ) {
				delete [] _bytecode;
				_bytecode = 0;
			}
		}
	protected:
		uint32_t	_flags;
		char*		_name;
		uint8_t*	_bytecode;
	};
	
}	// namespace MonkSWF

#endif // __mkTag_h__