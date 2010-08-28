/*
 *  mkTag.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/20/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#ifndef __mkTag_h__
#define __mkTag_h__

#include "mkReader.h"
#include <iostream>
#include <vector>

namespace MonkSWF {
	
	class SWF;
	
	using namespace std;
	
	class TagHeader {
	public:
		
		inline uint32_t code() const {
			return _code;
		}
		
		inline uint32_t length() const {
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
		
		const TagHeader& header() {
			return _header;
		}
		
		uint32_t code() const {
			return _header.code();
		}
		
		int32_t length() const {
			return _header.length();
		}
		
	protected:

		ITag( TagHeader& header ) 
		:	_header( header ) 
		{}
		
		virtual ~ITag() {
			
		}
		
		TagHeader	_header;
	};
	
#define ENDTAG 0
	class EndTag : public ITag {
	public:
		EndTag( TagHeader& header )
		:	ITag( header )
		{}
		
		virtual ~EndTag() {}
		
		virtual bool read( Reader* reader ) { return true; }
		virtual void print() {
			std::cout << "END TAG" << std::endl;
		}
		
		static ITag* create( TagHeader* header ) {
			return (ITag*)(new EndTag( *header ));
		}				
		
		
	};
	
#define DEFINESHAPE 2
#define DEFINESHAPE2 22
#define DEFINESHAPE3 32
#define DEFINESHAPE4 83	
	class IDefineShapeTag : public ITag {
	public:
	
		virtual void draw() = 0;
		
		uint16_t shapeId() const {
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
	
#define DEFINESPRITE 39
	class IDefineSpriteTag : public ITag {
	public:
		
		virtual void draw() = 0;
		
		uint16_t spriteId() const {
			return _sprite_id;
		}
		
		uint16_t frameCount() const {
			return _frame_count;
		}
		
		void setSWF( SWF* swf ) {
			_swf = swf;
		}
		
	protected:
		IDefineSpriteTag( TagHeader& header )
		:	ITag( header )
		,	_sprite_id( 0 )
		{}
		
		virtual ~IDefineSpriteTag()
		{
		}
		
		typedef std::vector<ITag*>		FrameTagList;
		typedef vector<FrameTagList*>	FrameList;
		
		
		uint16_t		_sprite_id;
		uint16_t		_frame_count;
		FrameList		_frames;
		SWF*			_swf;
		
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
		uint16_t depth() const {
			return _depth;
		}
		
		static bool compare( IPlaceObjectTag* a, IPlaceObjectTag* b ) {
			return a->depth() < b->depth();
		}
		
		uint16_t characterId() const {
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
		uint16_t characterId() const {
			return _character_id;
		}
		uint16_t depth() const {
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
	
}
#endif // __mkTag_h__