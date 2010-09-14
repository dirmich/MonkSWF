/*
 *  DefineShape.cpp
 *  MonkSWF
 *
 *  Created by Micah Pearlman on 3/21/09.
 *  Copyright 2009 MP Engineering. All rights reserved.
 *
 */

// fill style info: http://www.gnashdev.org/doc/apidoc/render_handler_intro.html

#include "DefineShape.h"
#include "mkSWF.h"
#include <list>
#include <iostream>
#include <iterator>
#include <algorithm>

using namespace std;

namespace MonkSWF {
	
	static const VGfloat eps = 0.1f;
	struct IEdge {
		
		IEdge( VGbyte t ) 
			: _type( t ) 
			, _is_reversed( false )
			, _original( 0 )
		{}
		
		
		VGbyte getType() {
			return _type;
		}
		
		IEdge* getOriginal() {
			return _original;
		}
		
		virtual void appendToVGPath( VGPath path ) = 0;
		virtual void reverse() = 0;
		
		virtual VGfloat* getStartPoint() = 0;
		virtual VGfloat* getEndPoint() = 0;
		virtual void print() = 0;
		virtual IEdge* copy( ) = 0;
		
		
		
		bool canConnectToEnd( IEdge* e ) {
			VGfloat *end = this->getEndPoint();
			VGfloat *start = e->getStartPoint();
			if( (fabs( end[0] - start[0] ) < eps) && (fabs( end[1] - start[1] ) < eps) )
//			if( (start[0] == end[0]) && (start[1] == end[1]) )
				return true;
			return false;
		}
		
		bool canConnectToStart( IEdge* e ) {
			VGfloat *end = e->getEndPoint();
			VGfloat *start = this->getStartPoint();
			   if( (fabs( end[0] - start[0] ) < eps) && (fabs( end[1] - start[1] ) < eps) )
				  //			if( (start[0] == end[0]) && (start[1] == end[1]) )
				  return true;
			return false;			
		}
		
		bool _is_reversed;
		VGbyte _type;
		IEdge* _original;	
		
	};
	
	struct StraightEdge : public IEdge {
		
		StraightEdge( VGfloat start[2], VGfloat end[2] ) 
			:	IEdge( VG_LINE_TO | VG_ABSOLUTE ) 
		{
			for ( int i = 0; i < 2; i++ ) {
				_start[i] = start[i];
				_end[i] = end[i];
			}
		}
		
		StraightEdge( StraightEdge* e )
		:	IEdge( VG_LINE_TO | VG_ABSOLUTE ) 
		{
			_original = e;
			for ( int i = 0; i < 2; i++ ) {
				_start[i] = e->_start[i];
				_end[i] = e->_end[i];
			}
		}
		
		
		virtual void reverse() {
			VGfloat tmp[2] = { _start[0], _start[1] };
			_start[0] = _end[0];
			_start[1] = _end[1];
			_end[0] = tmp[0];
			_end[1] = tmp[1];
			_is_reversed = true;
		}
		
		virtual IEdge* copy() {
			return new StraightEdge( this );
		}
		
		virtual void appendToVGPath( VGPath path ) {
			const VGint numSegments = vgGetParameteri( path, VG_PATH_NUM_SEGMENTS );
			if( numSegments == 0 ) {	// no path segments so starting a path 
				VGubyte segments[1] = { VG_MOVE_TO | VG_ABSOLUTE };
				vgAppendPathData( path, 1, segments, _start );
			}
			
			VGubyte segments[1] = { VG_LINE_TO | VG_ABSOLUTE };
			vgAppendPathData( path, 1, segments, _end );
		}
		
		
		virtual VGfloat* getStartPoint() {
			return _start;
		}
		
		virtual VGfloat* getEndPoint() {
			return _end;
		}
		
		virtual void print() {
			cout << "\tStraightEdge: " << _is_reversed << endl;
			cout << "\t\tstart: " << _start[0]*20 << ", " << _start[1]*20 << endl;
			cout << "\t\tend: " << _end[0]*20 << ", " << _end[1]*20 << endl;
		}
		
		VGfloat _start[2];
		VGfloat _end[2];
	};
	
	struct CurveEdge : public IEdge {
		
		CurveEdge( VGfloat start[2], VGfloat control1[2], VGfloat control2[2], VGfloat end[2] )
			:	IEdge( VG_CUBIC_TO | VG_ABSOLUTE) 
		{
			for ( int i = 0; i < 2; i++ ) {
				_start[i] = start[i];
				_control1[i] = control1[i];
				_control2[i] = control2[i];
				_end[i] = end[i];
			}
		}
		
		CurveEdge( CurveEdge* e )
		:	IEdge( VG_CUBIC_TO | VG_ABSOLUTE) 
		{		
			_original = e;
			for ( int i = 0; i < 2; i++ ) {
				_start[i] = e->_start[i];
				_control1[i] = e->_control1[i];
				_control2[i] = e->_control2[i];
				_end[i] = e->_end[i];
			}
			
		}
		virtual IEdge* copy( ) {
			return new CurveEdge( this );
		}
		
		
		virtual void reverse() {
			VGfloat tmp[2] = { _start[0], _start[1] };
			_start[0] = _end[0];
			_start[1] = _end[1];
			_end[0] = tmp[0];
			_end[1] = tmp[1];
			tmp[0] = _control1[0];
			tmp[1] = _control1[1];
			_control1[0] = _control2[0];
			_control1[1] = _control2[1];
			_control2[0] = tmp[0];
			_control2[1] = tmp[1];
		}
		
		virtual void appendToVGPath( VGPath path ) {
			const VGint numSegments = vgGetParameteri( path, VG_PATH_NUM_SEGMENTS );
			if( numSegments == 0 ) {	// no path segments so starting a path 
				VGubyte segments[1] = { VG_MOVE_TO | VG_ABSOLUTE };
				vgAppendPathData( path, 1, segments, _start );
			}
			
			VGubyte segments[1] = { VG_CUBIC_TO | VG_ABSOLUTE };
			vgAppendPathData( path, 1, segments, _control1 );
		}
		

		virtual VGfloat* getStartPoint() {
			return _start;
		}
		
		virtual VGfloat* getEndPoint() {
			return _end;
		}
		
		virtual void print() {
			cout << "\tCurvedEdge:" << endl;
			cout << "\t\tstart: " << _start[0]*20 << ", " << _start[1]*20 << endl;
			cout << "\t\tend: " << _end[0]*20 << ", " << _end[1]*20 << endl;
		}
		
		
		VGfloat _start[2];
		VGfloat _control1[2];
		VGfloat _control2[2];
		VGfloat _end[2];
	};
	
	typedef std::list< IEdge* > EdgeArray;
	typedef EdgeArray::iterator EdgeArrayIter;
	
	class Path {
	public:
		Path()
			:	_fill_idx0( -1 )
			,	_fill_idx1( -1 )
			,	_line_idx( -1 )
			,	_is_reversed( false )
		{
			
		}
		
		
		Path( Path* other ) {
			this->copy( other );
		}
		
		~Path() {
			for ( EdgeArrayIter i = _edges.begin(); i != _edges.end(); i++ ) {
				IEdge* edge = *i;
				delete edge;
			}
			
			_edges.clear();
		}
		
		FillStyle& fill() {
			return _fill;
		}
		
		void setFill( const FillStyle& fill ) {
			_fill = fill;
		}
		
		LineStyle& line() {
			return _line;
		}
		
		void setLine( const LineStyle& line ) {
			_line = line;
		}
		
		bool isReversed() {
			return _is_reversed;
		}
		
		void setReversed( bool reversed ) {
			_is_reversed = reversed;
		}
		
		int32_t fillIdx0() {
			return _fill_idx0;
		}
		void setFillIdx0( int32_t i ) {
			_fill_idx0 = i;
		}
		int32_t fillIdx1() {
			return _fill_idx1;
		}
		void setFillIdx1( int32_t i ) {
			_fill_idx1 = i;
		}
		
		int32_t lineIdx() {
			return _line_idx;
		}
		void setLineIdx( int32_t i ) {
			_line_idx = i;
		}
		
		EdgeArray& edges() {
			return _edges;
		}
		
		
		void copy( Path* p ) {
			for ( EdgeArrayIter i = p->_edges.begin(); i != p->_edges.end(); i++ ) {
				addEdge( (*i)->copy() );
			}
			
			_fill = p->_fill;
			_line = p->_line;
			_fill_idx0 = p->_fill_idx0;
			_fill_idx1 = p->_fill_idx1;			
			_line_idx = p->_line_idx;
		}
		
		void addEdge( IEdge* e ) {
//			if ( _is_reversed ) {
//				// fill0 type path are reversed
//				IEdge* reversed_edge = e->copy();
//				reversed_edge->reverse();
//				_edges.push_front( reversed_edge );
//			} else {
				_edges.push_back( e );
//			}
		}
		
		void reverse( ) {
			_is_reversed = true;
			for ( EdgeArrayIter i = _edges.begin(); i != _edges.end(); i++ ) {
				IEdge* edge = *i;
				edge->reverse();
			}
			
			std::reverse( _edges.begin(), _edges.end() );
		}
		
		void appendToVGPath( VGPath path ) {
			for ( EdgeArrayIter i = _edges.begin(); i != _edges.end(); i++ ) {
				IEdge* edge = *i;
				edge->appendToVGPath( path );
			}
		}
		
		void addToShapeWithStyle( ShapeWithStyle* sws ) {
		
			VGPath vgpath = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F,1,0,0,0, VG_PATH_CAPABILITY_ALL);
			this->appendToVGPath( vgpath );
			sws->addVGPath( vgpath, _fill, _line );
			
		}
		
		void append( Path* other ) {
			std::copy (other->_edges.begin(), other->_edges.end(), back_inserter( this->_edges ) );
			
		}
		
		void prepend( Path* other ) {
			std::copy (other->_edges.begin(), other->_edges.end(), front_inserter( this->_edges ) );
		}
		
		bool isClosed() {
			IEdge* this_end = _edges.back();
			IEdge* this_start = _edges.front(); 
			
			if( this_start->canConnectToStart( this_end ) )
				return true;
			
			return false;
		}
		
		bool join( Path* other ) {
		
			if( this->isClosed() || other->isClosed() )
				return false;
				
//			if( this->isReversed() != other->isReversed() )
//				return false;
				
			IEdge* this_end = _edges.back();
//			IEdge* this_start = _edges.front(); 
//			IEdge* other_end = other->_edges.back();
			IEdge* other_start = other->_edges.front();
			
			// if it is just a reversed edge of itself skip
//			if( this_end->getOriginal() == other_start->getOriginal() )
//				return false;


			if( this_end->canConnectToEnd( other_start ) ) {
				append( other );
				return true;
			}
			
//			if( this_start->canConnectToStart( other_end ) ) {
//				cout << "\n***PREPEND***" << endl;
//				prepend( other );
//				return true;
//			}
			
			return false;
		}
		
		
		
		void print() {
			cout << "Path: " << endl;
			cout << "FillIdx0: " << _fill_idx0 << endl;
			cout << "FillIdx1: " << _fill_idx1 << endl;
			cout << "LineIdx: " << _line_idx << endl;
			cout << "Closed: " << isClosed() << endl;
			cout << "Reversed: " << isReversed() << endl;
			_fill.print();
			_line.print();
			
			for ( EdgeArrayIter i = _edges.begin(); i != _edges.end(); i++ ) {
				IEdge* edge = *i;
				edge->print();
			}
		}
		
	private:
		
		EdgeArray	_edges;
		
		FillStyle	_fill;
		LineStyle	_line;
		int32_t		_fill_idx0;
		int32_t		_fill_idx1;
		int32_t		_line_idx;
		bool _is_reversed;
		
	};
	
	typedef std::list< Path* > PathArray;
	typedef PathArray::iterator PathArrayIter;
	typedef PathArray::const_iterator PathArrayConstIter;
	typedef PathArray::reverse_iterator PathArrayReverseIter;
	typedef std::vector< PathArray > FillPathMap;
	typedef FillPathMap::iterator FillPathMapIter;
	
	PathArray get_paths_by_style(const PathArray& path_vec, unsigned int style)
	{
		PathArray paths;
		for (PathArrayConstIter it = path_vec.begin(), end = path_vec.end(); it != end; ++it) {
			Path* cur_path = *it;
			
			if ( cur_path->fillIdx0() == style || cur_path->fillIdx1() == style ) {
				
				paths.push_back( cur_path );
			}
		}
		
		return paths;
	}
		
	
	PathArray normalize_paths(const PathArray &paths)
	{
		PathArray normalized;
		
		for ( PathArrayConstIter it = paths.begin(), end = paths.end();it != end; ++it) {
		
			Path* cur_path = *it;
			
			if (cur_path->edges().empty()) {
				continue;
				
			} else if (cur_path->fillIdx0() != -1 && cur_path->fillIdx1() != -1 ) {     
				
				// Two fill styles; duplicate and then reverse the left-filled one.
				Path* new_path = new Path( cur_path );
				new_path->reverse();
				new_path->setFillIdx1( new_path->fillIdx0() );
				new_path->setFillIdx0( -1 );
				cur_path->setFillIdx1( -1 );
				normalized.push_back(cur_path);
				normalized.push_back(new_path);       
				
				
			} else if ( cur_path->fillIdx0() != -1 ) {
				// Left fill style.
				//Path *newpath = reverse_path(cur_path);
				cur_path->reverse();
				cur_path->setFillIdx1( cur_path->fillIdx0() );
				cur_path->setFillIdx0( -1 );
				
				normalized.push_back(cur_path);
			} else if ( cur_path->fillIdx1() != -1 ) {
				// Right fill style.
				
				normalized.push_back(cur_path);
			} else {
				// No fill styles; copy without modifying.
				normalized.push_back(cur_path);
			}
			
		}
		
		return normalized;
	}
	
	template <class T>
	T next(T x) { return ++x; }

	template <class T>
	T prev(T x) { return --x; }
	
	
	
	
	Path* find_connecting_path2( Path* to_connect, PathArray &paths ) {
	
		if( to_connect->isClosed() )
			return 0;
		
		for ( PathArrayIter it = paths.begin(); it != paths.end(); it++ ) {
			Path* cur_path = *it;
			
			if (cur_path == to_connect || cur_path->isClosed() /*|| (cur_path->getOriginal() == to_connect) || (to_connect->getOriginal() == cur_path)/* || (cur_path->isReversed() != to_connect->isReversed())*/ ) {
				continue;
			}
			
			if( to_connect->join( cur_path ) )
				return cur_path;
			
		}
		
		return 0;
	}
	
	PathArray get_contours2( PathArray& paths ) {
//		PathArray contours;
		PathArray path_refs;

		for (PathArrayIter it = paths.begin(), end = paths.end(); it != end; ++it) {
			Path* cur_path = *it;
			path_refs.push_back( cur_path );
		}
		
		bool found_connection = false;
		for ( PathArrayIter it = path_refs.begin(); it != path_refs.end(); it++ ) {
			
			if( found_connection )
				--it;
			Path* to_connect = *it;
			Path* connected = find_connecting_path2( to_connect, path_refs );
			if( connected != 0 ) {
				path_refs.remove( connected );
				found_connection = true;
			} else {
				found_connection = false;
			}

			
		}
		
		return path_refs;
	}
	
	bool Gradient::read( Reader* reader ) {
		_spread_mode = reader->getbits( 2 );
		_interpolation_mode = reader->getbits( 2 );
		_num_gradients = reader->getbits( 4 );
		
		for ( int i = 0; i < _num_gradients; i++ ) {
			Gradient::Record record;
			record._ratio = reader->get<uint8_t>();
			record._color.r = reader->get<uint8_t>(); 
			record._color.g = reader->get<uint8_t>(); 
			record._color.b = reader->get<uint8_t>(); 
			_gradient_records.push_back( record );
		}
		
		return true;
	}
	
	bool LineStyle::read( Reader* reader, bool support_32bit_color ) {
		// create the openvg paint
		_paint = vgCreatePaint();

		_width = (reader->get<uint16_t>()/20.0f);
		
		_color[0] = (reader->get<uint8_t>()/255.0f);
		_color[1] = (reader->get<uint8_t>()/255.0f);
		_color[2] = (reader->get<uint8_t>()/255.0f);
		if( support_32bit_color )
			_color[3] = (reader->get<uint8_t>()/255.0f);
		else
			_color[3] = 1.0f;
		
		vgSetParameterfv( _paint, VG_PAINT_COLOR, 4, &_color[0] );
		
		return true;
	}

	bool FillStyle::read( Reader* reader, bool support_32bit_color ) {
	
		// create the openvg paint
		_paint = vgCreatePaint();
		
		_type = reader->get<uint8_t>();
		assert( _type == 0 && "unsupported fill style" );
		
		// todo set vg paint mode...
		
		_color[0] = (reader->get<uint8_t>()/255.0f);
		_color[1] = (reader->get<uint8_t>()/255.0f);
		_color[2] = (reader->get<uint8_t>()/255.0f);
		
		if( support_32bit_color )
			_color[3] = (reader->get<uint8_t>()/255.0f);
		else
			_color[3] = 1.0f;
		
		vgSetParameterfv( _paint, VG_PAINT_COLOR, 4, &_color[0] );

		//cout << "\t\tFill Style: " << int(_color[0] * 255) << ", " << int(_color[1] * 255) << ", " << int(_color[2] * 255) << ", " << int(_color[3] * 255) << endl;
		
		if( _type == GRADIENT_LINEAR || _type == GRADIENT_RADIAL ) {
			reader->getMatrix( _gradient_matrix );
			_gradient.read( reader );
			assert(0);
		}
		
		return true;
	}
	
	typedef std::vector<FillStyle> FillStyleArray;
	typedef std::vector<LineStyle> LineStyleArray;

	
	bool ShapeWithStyle::read( Reader* reader, DefineShapeTag* define_shape_tag ) {
		
		_define_shape_tag = define_shape_tag;
		const TagHeader& shape_header = define_shape_tag->header();
		bool support_32bit_color = (shape_header.code() != DEFINESHAPE && shape_header.code() != DEFINESHAPE2);	// all shape definitions except DEFINESHAPE & DEFINESHAPE2 support 32 bit color
		FillStyleArray fill_styles;
		LineStyleArray line_styles;
		
		// get the fill styles
		uint8_t num_fill_styles = reader->get<uint8_t>();
		if( num_fill_styles == 0xff )
			num_fill_styles = reader->get<uint16_t>();
		cout << "\tnum fill styles: " << int(num_fill_styles) << endl;	
		for ( int i = 0; i < num_fill_styles; i++ ) {
			FillStyle fill;
			fill.read( reader, support_32bit_color );
			fill.print();
			fill_styles.push_back( fill );
			
		}
		
		// get the line styles
		uint8_t num_line_styles = reader->get<uint8_t>();
		if( num_line_styles == 0xff )
			num_fill_styles = reader->get<uint16_t>();
		cout << "\tnum line styles: " << int(num_line_styles) << endl;	
		for ( int i = 0; i < num_line_styles; i++ ) {
			LineStyle line;
			line.read( reader, support_32bit_color );
			line.print();
			line_styles.push_back( line );
		}
		
		uint8_t num_fill_bits = reader->getbits( 4 );
		uint8_t num_line_bits = reader->getbits( 4 );

#define SF_MOVETO       0x01
#define SF_FILL0        0x02
#define SF_FILL1        0x04
#define SF_LINE         0x08
#define SF_NEWSTYLE     0x10
		
		uint8_t end = 0;
		VGfloat startxy[2] = { 0.0f, 0.0f };
		int fill_idx0 = -1;
		int fill_idx1 = -1;
		int line_idx = -1;
		Path* path = 0;
		
		PathArray path_array;
		
		while ( !end ) {
		
			// go through the shape records...
			uint8_t type_flag = reader->getbits( 1 );
			
			if( type_flag == 0 ) {	// change or end record
			
				uint8_t flags = reader->getbits( 5 );
			
				if( flags ) {
					
					if ( (flags & SF_MOVETO) || (flags & SF_FILL0) || (flags & SF_FILL1) ) {
						
						if ( path ) {
							path->setLineIdx( line_idx );
							path->setFillIdx0( fill_idx0 );
							path->setFillIdx1( fill_idx1 );
							path_array.push_back( path );
						}
						
						path = new Path();
					}
					
					if( flags & SF_MOVETO ) {
						uint8_t nbits = reader->getbits( 5 );
						startxy[0] = reader->getsignedbits( nbits ) / 20.0f;
						startxy[1] = reader->getsignedbits( nbits ) / 20.0f;
						//cout << "\tMoveTo: " << int(startxy[0] * 20) << ", " << int(startxy[1] * 20) << endl;
					} 
					
					if( flags & SF_FILL0 ) { 
						fill_idx0 = reader->getbits( num_fill_bits ) - 1;
					} 

					if( flags & SF_FILL1 ) {
						fill_idx1 = reader->getbits( num_fill_bits ) - 1;
					} 

					
					if( flags & SF_LINE) { 
						line_idx = reader->getbits( num_line_bits ) - 1;
					} 
					

					if( flags & SF_NEWSTYLE ) { 
						
//						PathArray normalized_array = normalize_paths( path_array );
//						
//						// find the path contours...
//						for (size_t i = 0; i < fill_styles.size(); ++i) {
//							PathArray paths_by_style = get_paths_by_style( normalized_array, i );
//							FillStyle &fill = fill_styles[i];
//							
//							if (!paths_by_style.size()) {
//								continue;
//							}
//							
//							PathArray contours = get_contours2( paths_by_style );
//							int k = 0;
//							for ( PathArrayIter contour_iter = contours.begin(); contour_iter != contours.end(); contour_iter++, k++) {
//								Path* contour = *contour_iter;
//								contour->setFill( fill );
//								if ( contour->lineIdx() != -1 ) {
//									contour->setLine( line_styles[ contour->lineIdx() ] );
//								}
//								
//								contour->print();
//								contour->addToShapeWithStyle( this );
//								
//								if ( !contour->isClosed() ) {
//									cout << "path not closed" << endl;
//								}
//							}
//						}	
//						
//						
//						// TODO: path line styles need to be done as welll
//						
//						// reset the style array
//						fill_styles.clear();
//						line_styles.clear();
//						path_array.clear();
						
						//todo: fix up the lines...
						
						cout << "NEW STYLE" << endl;
						// get the fill styles
						num_fill_styles = reader->get<uint8_t>();
						if( num_fill_styles == 0xff )
							num_fill_styles = reader->get<uint16_t>();
						
						cout << "\tnum fill styles: " << int(num_fill_styles) << endl;
						
						
						for ( int i = 0; i < num_fill_styles; i++ ) {
							FillStyle fill;
							fill.read( reader, support_32bit_color );
							fill.print();
							fill_styles.push_back( fill );
						}
						
						num_line_styles = reader->get<uint8_t>();
						if ( num_line_styles == 0xff )
							num_line_styles = reader->get<uint16_t>();
						
						cout << "\tnum line styles: " << int(num_line_styles) << endl;
						
						for ( int i = 0; i < num_line_styles; i++ ) {
							LineStyle line;
							line.read( reader, support_32bit_color );
							line.print();
							line_styles.push_back( line );
							
						}
						
						num_fill_bits = reader->getbits( 4 );
						num_line_bits = reader->getbits( 4 );
						
					}
					
				} else { // end record
					end = 1;
					if ( path ) {
						path->setLineIdx( line_idx );
						path->setFillIdx0( fill_idx0 );
						path->setFillIdx1( fill_idx1 );
						path_array.push_back( path );
					}
					
//					PathArray normalized_array = normalize_paths( path_array );
//					
//					// find the path contours...
//					for (size_t i = 0; i < fill_styles.size(); ++i) {
//						PathArray paths_by_style = get_paths_by_style( normalized_array, i );
//						FillStyle &fill = fill_styles[i];
//						
//						if (!paths_by_style.size()) {
//							continue;
//						}
//						
//						PathArray contours = get_contours2( paths_by_style );
//						int k = 0;
//						for ( PathArrayIter contour_iter = contours.begin(); contour_iter != contours.end(); contour_iter++, k++) {
//							Path* contour = *contour_iter;
//							contour->setFill( fill );
//							if ( contour->lineIdx() != -1 ) {
//								contour->setLine( line_styles[ contour->lineIdx() ] );
//							}
//							
//							contour->print();
//							contour->addToShapeWithStyle( this );
//							
//							if ( !contour->isClosed() ) {
//								cout << "path not closed" << endl;
//							}
//						}
//					}	
//					
//					// TODO: path line styles need to be done as welll
//					
//					// reset the style array
//					fill_styles.clear();
//					line_styles.clear();
//					path_array.clear();
					
				}	// if( flags )
				
			} else {	// edge type record
			
				uint16_t isLine = reader->getbits( 1 );
				
				if( isLine ) {
				
					const uint16_t nbits = reader->getbits( 4 ) + 2;
					const uint16_t general_line_flag = reader->getbits( 1 );
					
					if( general_line_flag ) {	
					
						VGfloat dxy[2];
						dxy[0] = startxy[0] + (reader->getsignedbits( nbits ) / 20.0f);
						dxy[1] = startxy[1] + (reader->getsignedbits( nbits ) / 20.0f);
						
						IEdge* edge = new StraightEdge( startxy, dxy );
						path->addEdge( edge );

						startxy[0] = dxy[0];
						startxy[1] = dxy[1];
						
					} else {	// either a horizontal or vertical line
					
						uint16_t vert_line_flag = reader->getbits( 1 ); 
						
						if ( vert_line_flag ) {
						
							VGfloat dxy[2];
							dxy[0] = startxy[0];
							dxy[1] = startxy[1] + (reader->getsignedbits( nbits ) / 20.0f);
							
							IEdge* edge = new StraightEdge( startxy, dxy );
							path->addEdge( edge );
							
							startxy[0] = dxy[0];
							startxy[1] = dxy[1];
						} else {	// horizontal line
							VGfloat dxy[2];
							dxy[0] = startxy[0] + (reader->getsignedbits( nbits ) / 20.0f);
							dxy[1] = startxy[1];
							
							IEdge* edge = new StraightEdge( startxy, dxy );
							path->addEdge( edge );							
							
							startxy[0] = dxy[0];
							startxy[1] = dxy[1];
						}
					}

				} else {	// curve
				
					uint16_t nbits = reader->getbits( 4 ) + 2;
					VGfloat cxy[2];
					VGfloat axy1[2] = { startxy[0], startxy[1] };
					VGfloat axy2[2];
					startxy[0] = cxy[0] = startxy[0] + (reader->getsignedbits( nbits ) / 20.0f);
					startxy[1] = cxy[1] = startxy[1] + (reader->getsignedbits( nbits ) / 20.0f);
					axy2[0] = startxy[0] + (reader->getsignedbits( nbits ) / 20.0f);
					axy2[1] = startxy[1] + (reader->getsignedbits( nbits ) / 20.0f);
					
					// convert from quadratic to cubic.  see:  http://fontforge.sourceforge.net/bezier.html
					//	CP1 = QP0 + 2/3 *(QP1-QP0)
					//	CP2 = CP1 + 1/3 *(QP2-QP0)					
					VGfloat cxy1[2], cxy2[2];	// cubic control points
					cxy1[0] = axy1[0] + 2.0f/3.0f * (cxy[0] - axy1[0] );
					cxy1[1] = axy1[1] + 2.0f/3.0f * (cxy[1] - axy1[1] );
					
					cxy2[0] = cxy1[0] + 1.0f/3.0f * (axy2[0] - axy1[0]);
					cxy2[1] = cxy1[1] + 1.0f/3.0f * (axy2[1] - axy1[1]);
					
					IEdge* curve = new CurveEdge( axy1, cxy1, cxy2, axy2 );
					path->addEdge( curve );					
					
					startxy[0] = axy2[0];
					startxy[1] = axy2[1];
				}
			}
		}
		
//		shape.addToShapeWithStyle( this );

//		cout << "START BEFORE NORMALIZE" << endl;
//		for ( PathArrayIter it = path_array.begin(); it != path_array.end(); it++ ) {
//			Path *path = *it;
//			path->print();
//		}
//		cout << "END BEFORE NORMALIZE" << endl;
		
// BUGBUG: this all needs to happen within each SF_NEWSTYLE		
		
		//not needed anymore PathArray normalized_array = normalize_paths( path_array );
		
//		for (size_t i = 0; i < _fill_styles.size(); ++i) {
//			PathArray paths = get_paths_by_style( normalized_array, i );
//			
//			if (!paths.size()) {
//				continue;
//			}
//			
//			PathArray contours = get_contours2( paths );
//			int k = 0;
//			for ( PathArrayIter contour_iter = contours.begin(); contour_iter != contours.end(); contour_iter++, k++) {
//				Path* path = *contour_iter;
////				cout << "K = " << k << endl;
////				path->print();
////				if( /* path->isClosed() */ k == 2 )
////				if( path->isClosed() )
//					path->addToShapeWithStyle( this );
//			}
//
//			
//		}	
		
		PathArray normalized_array = normalize_paths( path_array );
		
		// find the path contours...
		for (size_t i = 0; i < fill_styles.size(); ++i) {
			PathArray paths_by_style = get_paths_by_style( normalized_array, i );
			FillStyle &fill = fill_styles[i];
			
			if (!paths_by_style.size()) {
				continue;
			}
			
			PathArray contours = get_contours2( paths_by_style );
			int k = 0;
			for ( PathArrayIter contour_iter = contours.begin(); contour_iter != contours.end(); contour_iter++, k++) {
				Path* contour = *contour_iter;
				contour->setFill( fill );
				if ( contour->lineIdx() != -1 ) {
					contour->setLine( line_styles[ contour->lineIdx() ] );
				}
				
				contour->print();
				contour->addToShapeWithStyle( this );
				
				if ( !contour->isClosed() ) {
					cout << "path not closed" << endl;
				}
			}
		}	
		
		
		return true;
	}
	
	
	
	
	void ShapeWithStyle::draw() {
		for ( OpenVGPathArrayIter iter = _paths.begin(); iter != _paths.end(); iter++ ) {
			OpenVGPath &path = *iter;
			uint32_t path_style = 0;
			

//			vgSeti( VG_FILL_RULE, VG_EVEN_ODD );

			
			if( path._fill_style.getPaint() ) {	// set up fill style
				path_style |= VG_FILL_PATH;
				vgSetPaint( path._fill_style.getPaint(), VG_FILL_PATH );
			} 
			
			if( path._line_style.getPaint() ) {	// set up stroke style
				path_style |= VG_STROKE_PATH;
				vgSetPaint( path._line_style.getPaint(), VG_STROKE_PATH );
				vgSetf(VG_STROKE_LINE_WIDTH, path._line_style.getWidth() );
			}
				
			vgDrawPath( path._vgpath, path_style );
		}
	}
	
	bool DefineShapeTag::read( Reader* reader ) {
		_shape_id = reader->get<uint16_t>();
		reader->getRectangle( _bounds );
		
		_shape_with_style.read( reader, this );
		
// todo: DEFINESHAPE4!		
		
//		this->print();
		return true;
	}
	
	void DefineShapeTag::print() {
		//_header.print();
		cout << "DEFINESHAPE code = " << code() << " id = " << _shape_id << endl;
//		cout << "shape id: "		<< _shape_id << endl;
//		cout << "frame width: "		<< ((_bounds.xmax - _bounds.xmin)/20.0f) << endl;
//		cout << "frame height: "	<< ((_bounds.ymax - _bounds.ymin)/20.0f) << endl;
		
	}
	
	void DefineShapeTag::draw() {
		_shape_with_style.draw();
	}
	
	ITag* DefineShapeTag::create( TagHeader* header ) {
		return (ITag*)(new DefineShapeTag( *header ));
	}
		
}