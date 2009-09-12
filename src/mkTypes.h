/*
 *  mkSWFTypes.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/19/09.
 *  Copyright 2009 Monk Games. All rights reserved.
 *
 */

#ifndef __mkSWFTypes_h__
#define __mkSWFTypes_h__

namespace MonkSWF {

	typedef         signed long     fixed_t;
	typedef         signed long     coord_t;
	
	#define coord_t_MAX 0x7fffffff
	#define coord_t_MIN -0x80000000
	
	// Basic Structures
	
	struct POINT
	{ 
		coord_t        x;
		coord_t        y;
	};
	
	struct RGBA
	{ 
		uint8_t    a;
		uint8_t    r;
		uint8_t    g;
		uint8_t    b;
	};
	
	struct YUV
	{
		uint8_t	y,u,v;
	};
	
	struct RECT
	{ 
		coord_t        xmin;
		coord_t        ymin;
		coord_t        xmax;
		coord_t        ymax;
	};
	
	struct MATRIX { 
		fixed_t        sx, r1, tx;
		fixed_t        r0, sy, ty;
	};
//	
//	typedef struct _CXFORM
//		{ S16           a0, a1; /* mult, add */
//			S16           r0, r1;
//			S16           g0, g1;
//			S16           b0, b1;
//		} CXFORM;
//	
#define GRADIENT_LINEAR 0x10
#define GRADIENT_RADIAL 0x12
	struct GRADIENT
	{
		int num;
		uint8_t* ratios;
		RGBA* rgba;
	};
//	
//	typedef struct _FILTER
//		{
//			uint8_t type;
//		} FILTER;
//	
//	typedef struct _FILTERLIST
//		{
//			int num;
//			FILTER*filter[8];
//		} FILTERLIST;
//	
//	typedef struct _TAG             // NEVER access a Tag-Struct directly !
//		{ U16           id;
//			uint8_t *          data;
//			U32           memsize;        // to minimize realloc() calls
//			
//			U32         len;            // for Set-Access
//			U32         pos;            // for Get-Access
//			
//			struct _TAG * next;
//			struct _TAG * prev;
//			
//			uint8_t            readBit;        // for Bit-Manipulating Functions [read]
//			uint8_t            writeBit;       // [write]
//			
//		} TAG;
//	
//#define swf_ResetReadBits(tag)   if (tag->readBit)  { tag->pos++; tag->readBit = 0; }
//#define swf_ResetWriteBits(tag)  if (tag->writeBit) { tag->writeBit = 0; }
//	
//	typedef struct _SOUNDINFO 
//		{
//			uint8_t stop;
//			uint8_t nomultiple; //continue playing if already started
//			
//			U32 inpoint;
//			U32 outpoint;
//			
//			U16 loops;
//			uint8_t envelopes;
//			
//			//envelope:
//			U32* pos;
//			U32* left;
//			U32* right;
//		} SOUNDINFO;
	
}
#endif	// __mkSWFTypes_h__