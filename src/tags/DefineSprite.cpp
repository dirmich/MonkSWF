/*
 *  DefineSprite.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 8/28/10.
 *  Copyright 2010 MP Engineering. All rights reserved.
 *
 */

#include "DefineSprite.h"
#include "mkSWF.h"

namespace MonkSWF {
	bool DefineSpriteTag::read( Reader* reader ) {
		_sprite_id = reader->get<uint16_t>();
		_frame_count = reader->get<uint16_t>();
		
		// get the first tag
		TagHeader *tag_header = new TagHeader();
		tag_header->read( reader );
		
		FrameTagList* frame_tags = new FrameTagList;

		while( tag_header->code() != ENDTAG ) { // while not the end tag 
			
			SWF::TagFactoryFunc tag_factory = _swf->getTagFactory( tag_header->code() );
			ITag* tag = 0;
			
			if( tag_factory ) {
				tag = tag_factory( tag_header );
				int32_t end_pos = reader->getCurrentPos() + tag->length();
				
				if ( tag_header->code() == DEFINESPRITE ) {
					IDefineSpriteTag* sprite = (IDefineSpriteTag*)tag;
					sprite->setSWF( _swf );
					_swf->addSprite( sprite, sprite->code());
				}
				
				tag->read( reader );
				reader->align();
				int32_t dif = end_pos - reader->getCurrentPos();
				if( dif != 0 )
					reader->skip( dif );
				
				
				
				if( tag_header->code() == SHOWFRAME ) {	// ShowFrame
					_frames.push_back( frame_tags );
					frame_tags = new FrameTagList;
					cout << "### SHOWFRAME ###" << endl;
				} else {
					tag->print();
					frame_tags->push_back( tag );
				}

				
			} else {	// no registered factory so skip this tag
				cout << "*** SKIPPING UNKOWN TAG ***" << endl;
				tag_header->print();
				reader->skip( tag_header->length() );
			}
			
			delete tag_header;

			tag_header = new TagHeader();
			tag_header->read( reader );
		}
		
		delete tag_header;
		
		return true;
	}
	void DefineSpriteTag::print() {
		cout << "DEFINESPRITE: id = " << _sprite_id << " frame count = " << _frame_count << endl;
		_header.print();
		
	}
	
	void DefineSpriteTag::draw( const int32_t frame ) {
		
		if ( frame < 0 || frame >= _frame_count ) {
			return;
		}
		
		// build up the display list
		FrameTagList* frame_tags = _frames[ frame ];
		for ( FrameTagList::iterator tag = frame_tags->begin(); tag != frame_tags->end(); tag++ ) {
			switch ( (*tag)->code() ) {
				case PLACEOBJECT2: {
					IPlaceObjectTag* place_obj = (IPlaceObjectTag*)*tag;
					
					if( place_obj->doMove() ) {	// just move the object at the current depth
						IPlaceObjectTag* move_obj = _display_list[ place_obj->depth() ];
						
						// hmmm.  not sure if this is correct need to re-think this
						place_obj->copyNoTransform( move_obj );
						_display_list[ place_obj->depth() ] = place_obj;
//						move_obj->copyTransform( place_obj );
					} else {
						_display_list[ place_obj->depth() ] = place_obj;
					}
					
				} break;
				case REMOVEOBJECT: {
					IRemoveObjectTag* remove_object = (IRemoveObjectTag*)*tag;
					_display_list.erase( remove_object->depth() );
				} break;
	
				default:
					break;
			}
		}
		
		// draw the display list
		for (DisplayListIter iter = _display_list.begin(); iter != _display_list.end(); iter++ ) {
			IPlaceObjectTag *place_obj = iter->second;
			if( place_obj ) {
				place_obj->setOffsetScale( _scale );
				place_obj->setOffsetTranslate( _translate );
				place_obj->draw( _swf );
			}
				
		}
		
	}
	

	ITag* DefineSpriteTag::create( TagHeader* header ) {
		return (ITag*)(new DefineSpriteTag( *header ));
	}
	
}