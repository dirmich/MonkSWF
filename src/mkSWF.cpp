/*
 *  mkSWF.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/20/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#include "mkSWF.h"
#include "tags/DefineShape.h"
#include "tags/PlaceObject.h"
#include "tags/ShowFrame.h"
#include "tags/RemoveObject.h"
#include "tags/DefineSprite.h"

namespace MonkSWF {
	
	SWF::SWF()
	:	_offsetScale( 1 )
	{
		_offsetTranslate[0] = 0;
		_offsetTranslate[1] = 0;
		
		for ( int i = 0; i < 9; i++ ) {
			_rootTransform[i] = 0;
		}
		// set root tranform to identity
		_rootTransform[0] = 1;
		_rootTransform[4] = 1;
		_rootTransform[8] = 1;
		
	}
	bool SWF::initialize() {
		
		// setup the factories
		addFactory( ENDTAG, EndTag::create );
		
		addFactory( DEFINESHAPE, DefineShapeTag::create );		// DefineShape
		addFactory( DEFINESHAPE2, DefineShapeTag::create );		// DefineShape2
		addFactory( DEFINESHAPE3, DefineShapeTag::create );		// DefineShape3
		addFactory( DEFINESHAPE4, DefineShapeTag::create );		// DefineShape4

		addFactory( DEFINESPRITE, DefineSpriteTag::create );
		addFactory( PLACEOBJECT2, PlaceObject2Tag::create );
		addFactory( REMOVEOBJECT2, RemoveObjectTag::create );
		addFactory( SHOWFRAME, ShowFrameTag::create );
		
		
		return true;
	}
	
	bool SWF::read( Reader* reader ) {
		_reader = reader;
		_header.read( reader );
		
		// get the first tag
		TagHeader *tag_header = new TagHeader();
		tag_header->read( reader );
				
		// get all tags and build display and frame lists
		DisplayList* display_list = new DisplayList();
		while( tag_header->code() != 0 ) { // while not the end tag 
		
			TagFactoryMapIter factory = _tag_factories.find( tag_header->code() );
			ITag* tag = 0;
			
			if( factory != _tag_factories.end() && factory->second ) {
				tag = factory->second( tag_header );
				int32_t end_pos = reader->getCurrentPos() + tag->length();
				
				if( tag_header->code() == DEFINESHAPE || tag_header->code() == DEFINESHAPE2 
				   || tag_header->code() == DEFINESHAPE3 || tag_header->code() == DEFINESHAPE4 ) {
					_shape_dictionary.push_back( (IDefineShapeTag*)tag );
				}
				
				if ( tag_header->code() == DEFINESPRITE ) {		// tell the sprite about the SWF...
					IDefineSpriteTag* sprite = (IDefineSpriteTag*)tag;
					sprite->setSWF(this);
				}
				
				tag->read( reader );
				reader->align();
				int32_t dif = end_pos - reader->getCurrentPos();
				if( dif != 0 ) {
					cout << "WARNING: tag not read correctly. trying to skip. " << dif << endl;
					cout << "\t\t"; tag->print();
					reader->skip( dif );
				}
					

				if ( tag_header->code() == DEFINESPRITE ) {
					IDefineSpriteTag* sprite = (IDefineSpriteTag*)tag;
					addSprite( sprite, sprite->spriteId() );
				}
				
				
				if( tag_header->code() == PLACEOBJECT2 ) {	//PlaceObject2
					IPlaceObjectTag* place_obj = (IPlaceObjectTag*)tag;
					
					if ( place_obj->doMove() == false && place_obj->hasCharacter() == true ) {
						// A new character (with ID of CharacterId) is placed on the display list at the specified
						// depth. Other fields set the attributes of this new character.
						IPlaceObjectTag* current_obj = (*display_list)[ place_obj->depth() ];
						// copy over the previous matrix if the new character doesn't have one
						if ( current_obj && place_obj->hasMatrix() == false ) {
							place_obj->copyTransform( current_obj );
						}
						(*display_list)[ place_obj->depth() ] = place_obj;
						
					} else if ( place_obj->doMove() == true && place_obj->hasCharacter() == false ) {
						// The character at the specified depth is modified. Other fields modify the attributes of this
						// character. Because any given depth can have only one character, no CharacterId is required.
						IPlaceObjectTag* current_obj = (*display_list)[ place_obj->depth() ];
						if ( current_obj ) {
							place_obj->setCharacterId( current_obj->characterId() );
							(*display_list)[ place_obj->depth() ] = place_obj;
						}
					} else if ( place_obj->doMove() == true && place_obj->hasCharacter() == true ) {
						// The character at the specified Depth is removed, and a new character (with ID of CharacterId) 
						// is placed at that depth. Other fields set the attributes of this new character.
						(*display_list)[ place_obj->depth() ] = place_obj;
					}
					
				}
				
				if( tag_header->code() == SHOWFRAME ) {	// ShowFrame
					_frame_list.push_back( display_list );
					DisplayList *new_display_list = new DisplayList( display_list->begin(), display_list->end() );
					
					display_list = new_display_list;
					
					//cout << "### SHOWFRAME ###" << endl;
				}
				
				if( tag_header->code() == REMOVEOBJECT2 ) {
					IRemoveObjectTag* remove_object = (IRemoveObjectTag*)tag;
					display_list->erase( remove_object->depth() );
				}
				
				//tag->print();
					
			} else {	// no registered factory so skip this tag
				cout << "*** SKIPPING UNKOWN TAG ***" << endl;
				tag_header->print();
				reader->skip( tag_header->length() );
				delete tag_header;
			}

			
			tag_header = new TagHeader();
			tag_header->read( reader );
		}
		
		return true;
	}
	
	void SWF::print() {
		_header.print();
	}
	
	void SWF::drawFrame( int32_t frame_idx ) {
		// make sure we use even odd fill rule
		vgSeti( VG_FILL_RULE, VG_EVEN_ODD );
		// clear the screen
		VGfloat clearColor[] = {1,1,1,1};
		vgSetfv(VG_CLEAR_COLOR, 4, clearColor);
		vgClear(0,0,_header.getFrameWidth(), _header.getFrameHeight() );
		
		vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
		VGfloat oldMatrix[9];
		vgGetMatrix( oldMatrix );
		vgMultMatrix( _rootTransform );
		
		DisplayList *display_list = _frame_list[ frame_idx ];
		for (DisplayListIter iter = display_list->begin(); iter != display_list->end(); iter++ ) {
			IPlaceObjectTag *place_obj = iter->second;
			if( place_obj ) {
				VGfloat offset[2];
				getOffsetTranslate( offset );
				place_obj->setOffsetTranslate( offset );
				place_obj->setOffsetScale( getOffsetScale() );
				place_obj->draw( this );
			}
		}
		// restore old matrix
		vgLoadMatrix( oldMatrix );

	}
}