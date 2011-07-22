/*
 *  mkSWF.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/20/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#ifndef __mkSWF_h__
#define __mkSWF_h__

#include "mkReader.h"
#include "mkHeader.h"
#include "mkTag.h"
#include <map> 
#include <vector>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace std;

namespace MonkSWF {
	
	class SWF {
	public:
		
		typedef boost::shared_ptr<SWF> SmartPtr;
		static SWF::SmartPtr create( ) {
			return boost::make_shared<SWF>( );
		}


		// factory function prototype
		typedef ITag* (*TagFactoryFunc)( TagHeader* );
	
		SWF(); 
		
		bool initialize();
		bool read( Reader *reader );
		void print();
		
		void addFactory( uint32_t tag_code, TagFactoryFunc factory ) {
			_tag_factories[ tag_code ] = factory;
		}
		
		TagFactoryFunc getTagFactory( uint32_t tag_code ) {
			TagFactoryMapIter factory = _tag_factories.find( tag_code );
			
			if( factory != _tag_factories.end() && factory->second ) {
				return factory->second;
			}
			
			return 0;
		}
		
		IDefineShapeTag* getShape( uint16_t i ) {		// todo: change to map instead of vector
		
			for ( ShapeDictionaryIter it = _shape_dictionary.begin(); it != _shape_dictionary.end(); it++ ) {
				IDefineShapeTag* shape = *it;
				if( i == shape->shapeId() )
					return shape;
			}
			
			return 0;
		}
		
		int32_t numShapes() const {
			return _shape_dictionary.size();
		}
		
		IDefineShapeTag* shapeAt( int32_t idx ) {
			return _shape_dictionary[idx];
		}
		
		IDefineSpriteTag* spriteAt( int32_t idx ) {
			if ( _sprite_dictionary.size() == 0 || idx >= _sprite_dictionary.size() ) {
				return 0;
			}
			SpriteDictionary::iterator i = _sprite_dictionary.begin();
			advance(i, idx);
			return i->second;
		}
		
		IDefineSpriteTag* getSprite( uint16_t i ) {
			SpriteDictionary::iterator it = _sprite_dictionary.find( i );
			if (it == _sprite_dictionary.end() ) {
				return 0;
			}
			return it->second;
		}
		int32_t numSprites() const {
			return _sprite_dictionary.size();
		}
		
		void addSprite( IDefineSpriteTag* sprite, uint16_t cid ) {
			_sprite_dictionary[cid] = sprite;
		}
		
		int32_t numFrames() const {
			return _frame_list.size();
		}
		
		Reader* reader() const {
			return _reader;
		}
		
		void drawFrame( int32_t frame_idx );
		
		float getFrameWidth() const {
			return _header.getFrameWidth();
		}
		float getFrameHeight() const {
			return _header.getFrameHeight();
		}
		float getFrameRate() const {
			return _header.getFrameRate();
		}
		
		void setOffsetTranslate( const float t[2] ) {
			_offsetTranslate[0] = t[0];
			_offsetTranslate[1] = t[1];
		}
		void getOffsetTranslate( float t[2] ) const  {
			t[0] = _offsetTranslate[0];
			t[1] = _offsetTranslate[1];
		}
		void setOffsetScale( float s ) {
			_offsetScale = s;
		}
		float getOffsetScale() const {
			return _offsetScale;
		}
		
		void setRootTransform( const float t[9] ) {
			for( int i = 0; i < 9; i++ )
				_rootTransform[i] = t[i];
		}
		
		void getRootTransform( float t[9] ) {
			for( int i = 0; i < 9; i++ )
				t[i] = _rootTransform[i];
			
		}
		
		
		
		
	private:
		typedef std::map< uint32_t, SWF::TagFactoryFunc >	TagFactoryMap;
		typedef TagFactoryMap::iterator						TagFactoryMapIter;
		typedef std::vector< IDefineShapeTag* >				ShapeDictionary;
		typedef ShapeDictionary::iterator					ShapeDictionaryIter;
		typedef std::map< uint16_t, IDefineSpriteTag* >		SpriteDictionary;
		typedef std::map< uint16_t, IPlaceObjectTag* >		DisplayList;
		typedef DisplayList::iterator						DisplayListIter;
		typedef std::vector< DisplayList* >					FrameList;
		typedef FrameList::iterator							FrameListIter;
		
		
		ShapeDictionary		_shape_dictionary;
		SpriteDictionary	_sprite_dictionary;
		DisplayList			_display_list;
		FrameList			_frame_list;
		Header				_header;
		TagFactoryMap		_tag_factories;
		Reader*				_reader;
		int32_t				_frame;
		
		// offset
		float				_offsetTranslate[2];
		float				_offsetScale;
		float				_rootTransform[9];
		
		
	
	};
}
#endif // __mkSWF_h__