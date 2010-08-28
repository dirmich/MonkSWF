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
		
		while( tag_header->code() != ENDTAG ) { // while not the end tag 
			
			SWF::TagFactoryFunc tag_factory = _swf->getTagFactory( tag_header->code() );
			ITag* tag = 0;
			FrameTagList* frame_tags = new FrameTagList;
			
			if( tag_factory ) {
				tag = tag_factory( tag_header );
				int32_t end_pos = reader->getCurrentPos() + tag->length();
				tag->read( reader );
				int32_t dif = (end_pos - reader->getCurrentPos()) - 1;
				if( dif > 0 )
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
	
	void DefineSpriteTag::draw() {
		
	}
	

	ITag* DefineSpriteTag::create( TagHeader* header ) {
		return (ITag*)(new DefineSpriteTag( *header ));
	}
	
}