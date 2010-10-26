/*
 *  PlaceObject.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/26/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#include "PlaceObject.h"
#include "mkSWF.h"
#include <iostream>
using namespace std;

namespace MonkSWF {
	
	bool PlaceObject2Tag::read( Reader* reader ) {
	
		if( reader->getBitPos() )
			cout << "UNALIGNED!" << endl;
		uint8_t has_clip_actions = reader->getbits( 1 );
		uint8_t has_clip_depth = reader->getbits( 1 );
		uint8_t has_name = reader->getbits( 1 );
		uint8_t has_ratio = reader->getbits( 1 );
		uint8_t has_color_transform = reader->getbits( 1 );
		uint8_t has_matrix = reader->getbits( 1 );
		_has_character = reader->getbits( 1 );
		_do_move = reader->getbits( 1 );
		
		if( has_clip_actions || has_clip_depth )
			assert( 0 );
			
		_depth = reader->get<uint16_t>();
		if( _has_character )
			_character_id = reader->get<uint16_t>();
			
			
		if( has_matrix ) {
			MATRIX m;
			reader->getMatrix( m );
			
			//	OpenVG:
			//	sx	shx	tx
			//	shy	sy	ty
			//	0	0	1
			
			// also see MATRIX in swf file format spec
			
			// translation
			_transform[0][2] = m.tx/20.0f;
			_transform[1][2] = m.ty/20.0f;
			
			// scale
			_transform[0][0] = m.sx/65536.0f;
			_transform[1][1] = m.sy/65536.0f;
			
			// rotate and skew
			_transform[0][1] = m.r0/65536.0f;
			_transform[1][0] = m.r1/65536.0f;
//			_translation[0] = m.tx/20.0f;
//			_translation[1] = m.ty/20.0f;
//			_scale[0] = m.sx/65536.0f;
//			_scale[1] = m.sy/65536.0f;
//			_rotation = m.r0/65536.0f; 
		}
		
		if ( has_color_transform ) {
		
			const uint8_t has_add_terms = reader->getbits( 1 );
			const uint8_t has_mult_terms = reader->getbits( 1 );
			const uint8_t nbits = reader->getbits( 4 );
			if( has_mult_terms ) {
				coord_t red = reader->getsignedbits( nbits );
				coord_t green = reader->getsignedbits( nbits );
				coord_t blue = reader->getsignedbits( nbits );
				coord_t alpha = reader->getsignedbits( nbits );

			}
			
			if( has_add_terms ) {
				coord_t red = reader->getsignedbits( nbits );
				coord_t green = reader->getsignedbits( nbits );
				coord_t blue = reader->getsignedbits( nbits );
				coord_t alpha = reader->getsignedbits( nbits );
			}
		}
		
		if ( has_ratio ) {
			reader->get<uint16_t>();
		}
		if( has_name ) {
			_name = reader->getString();
		}
		if( has_clip_depth ) {
			reader->get<uint16_t>();
		}
		if( has_clip_actions ) {
			assert( 0 );
		}
		
		return true;
	}
	
	void PlaceObject2Tag::copyTransform( IPlaceObjectTag* o ) {
		//copyNoTransform( o );
		PlaceObject2Tag* other = (PlaceObject2Tag*)o;
		for ( int i = 0; i < 3; i++ ) {
			for ( int p = 0; p < 3; p++ ) {
				_transform[i][p] = other->_transform[i][p];
			}
		}
//		_translation[0] = other->_translation[0];
//		_translation[1] = other->_translation[1];
//		_scale[0] = other->_scale[0];
//		_scale[1] = other->_scale[1];
//		_rotation = other->_rotation; //todo 
	}
	
//	static inline float degrees (float radians) {return radians * (180.0f/3.14159f);}	
	void PlaceObject2Tag::draw( SWF* swf ) {
		vgLoadIdentity();
		vgLoadMatrix( (VGfloat*)&_transform[0][0] );
		vgTranslate( _offsetTranslate[0], _offsetTranslate[1] );
		vgScale( _offsetScale, _offsetScale );
//		vgRotate( degrees( _rotation ) );
		
		IDefineShapeTag* shape = swf->getShape( _character_id );
		if( shape )
			shape->draw();
		else {
			IDefineSpriteTag* sprite = swf->getSprite( _character_id );
			if ( sprite ) {
				sprite->draw(0);		// TOOD: have swf store the current frame
			}
		}

	}
	
	void PlaceObject2Tag::print() {
		cout << "PLACEOBJECT2: id = " << _character_id << " depth = " << _depth << " DoMove = " << doMove() << endl;
		_header.print();
	}

//	PlaceFlagHasClipActions UB[1] SWF 5 and later: has
//	clip actions (sprite
//				  characters only)
//Otherwise: always 0
//	PlaceFlagHasClipDepth UB[1] Has clip depth
//	PlaceFlagHasName UB[1] Has name
//	PlaceFlagHasRatio UB[1] Has ratio
//	PlaceFlagHasColorTransform UB[1] Has color transform
//	PlaceFlagHasMatrix UB[1] Has matrix
//	PlaceFlagHasCharacter UB[1] Places a character
//	PlaceFlagMove UB[1] Defines a character to
//	be moved
//	Depth UI16 Depth of character
//	CharacterId If PlaceFlagHasCharacter
//	UI16
//	ID of character to place	
}