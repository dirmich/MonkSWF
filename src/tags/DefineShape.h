/*
 *  DefineShape.h
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/21/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

#ifndef __DefineShape_h__
#define __DefineShape_h__

#include "mkTag.h"
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <vector>
#include <list>
using namespace std;

namespace MonkSWF {

	class Gradient {
	public:
		bool read( Reader* reader );
		
	private:
		uint8_t		_spread_mode;			// 0 = Pad mode 1 = Reflect mode 2 = Repeat mode
		uint8_t		_interpolation_mode;	// 0 = Normal RGB mode interpolation 1 = Linear RGB mode interpolation
		uint8_t		_num_gradients;
		
		struct Record {
			uint8_t	_ratio;
			RGBA	_color;
		};
		
		typedef std::vector< Gradient::Record > GradientRecordArray;
		typedef GradientRecordArray::iterator GradientRecordArrayIter;
		
		GradientRecordArray		_gradient_records;
	};
	
	class FillStyle {
	public:
		FillStyle()
		:	_type(-1)
		,	_paint(0)
		{}
		
		
		bool read( Reader* reader, bool support_32bit_color );
		
		inline VGPaint getPaint() {
			return _paint;
		}
		
		void print() {
			cout << "FillStyle: " << int(_type) << endl;
			cout << "\tColor: " << int(_color[0] * 255) << ", " << int(_color[1] * 255) << ", " << int(_color[2] * 255) << ", " << int(_color[3] * 255) << endl;
		}
	
	private:
		uint8_t		_type;
		VGfloat		_color[4];
		MATRIX		_gradient_matrix;
		Gradient	_gradient;
		VGPaint		_paint;
		// GRADIENT or FOCAL GRADIENT
		// bitmap id
		// bitmap matrix
		
	};
	
	class LineStyle {
	public:
		
		LineStyle()
		:	_paint(0)
		{}
		
		bool read( Reader* reader, bool support_32bit_color );

		inline VGPaint getPaint() {
			return _paint;
		}
		
		inline VGfloat getWidth() {
			return _width;
		}
		
		void print() {
			cout << "LineStyle: " << endl;
			cout << "\tWidth: " << _width << endl;
			cout << "\tColor: " << int(_color[0] * 255) << ", " << int(_color[1] * 255) << ", " << int(_color[2] * 255) << ", " << int(_color[3] * 255) << endl;
		}
		
	private:
		VGfloat		_width;
		VGfloat		_color[4];
		VGPaint		_paint;		
	};

	struct OpenVGPath {
		
		VGPath			_vgpath;
		FillStyle		_fill_style;
		LineStyle		_line_style;
	};
	
	class DefineShapeTag;
	class ShapeWithStyle {
	public:
		bool read( Reader* reader, DefineShapeTag* define_shape_tag );
		void draw();
		
		void addVGPath( VGPath vgpath, const FillStyle& fill, const LineStyle& line ) {
			OpenVGPath path;
			path._vgpath = vgpath;
			path._fill_style = fill;
			path._line_style = line;
				
			_paths.push_back( path );
				
		}
		
	private:
		
		
		typedef std::list<OpenVGPath> OpenVGPathArray;
		typedef OpenVGPathArray::iterator OpenVGPathArrayIter;

		OpenVGPathArray		_paths;
		DefineShapeTag*		_define_shape_tag;
	};
	
	class DefineShapeTag : public IDefineShapeTag {
	public:
	
		DefineShapeTag( TagHeader& h )
			:	IDefineShapeTag( h )
		{}
		
		virtual ~DefineShapeTag() {
			
		}
		
		virtual bool read( Reader* reader );
		virtual void print();
		
		virtual void draw();
		
		// regular DefineShape tag == 2
		static ITag* create( TagHeader* header );
		
	private:
		RECT				_bounds;
		ShapeWithStyle		_shape_with_style;
	};
	
	
}
#endif // __DefineShape_h__