#ifndef	__YACG_PRIMITIVE_HPP__
#define	__YACG_PRIMITIVE_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

// TODO:  Add fore/back colors as needed to all controls here.

namespace yacg
{
//=============================================================================
// class Arc declaration
//=============================================================================

	class Arc : public _Control
	{
	public:
		Arc(_Manager& m, int x, int y, float s, float e, int r, int c);
		
		void paint(BITMAP* bmp);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _x;
		int _y;
		fixed _start;
		fixed _end;
		int _radius;
		int _color;
	};

//=============================================================================
// class Circle declaration
//=============================================================================

	class Circle : public _Control
	{
	public:
		Circle(_Manager& m, int x, int y, int r, int c, bool f = false);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _x;
		int _y;
		int _radius;
		int _color;
		bool _fill;
	};

//=============================================================================
// class Ellipse declaration
//=============================================================================

	class Ellipse : public _Control
	{
	public:
		Ellipse(_Manager& m, int x, int y, int hr, int vr, int c, bool f = false);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _x;
		int _y;
		int _radiush;
		int _radiusv;
		int _color;
		bool _fill;
	};

//=============================================================================
// class FloodFill declaration
//=============================================================================

	class FloodFill : public _Control
	{
	public:
		FloodFill(_Manager& m, int x, int y, int c);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _x;
		int _y;
		int _color;
	};

//=============================================================================
// class Line declaration
//=============================================================================

	class Line : public _Control
	{
	public:
		Line(_Manager& m, int x1, int y1, int x2, int y2, int c);
		Line(_Manager& m, int x, int y, int xy, int c, bool horizontal = true);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _x1;
		int _y1;
		int _x2;
		int _y2;
		int _color;
	};

//=============================================================================
// class Pixel declaration
//=============================================================================

	class Pixel : public _Control
	{
	public:
		Pixel(_Manager& m, int x, int y, int c);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _x;
		int _y;
		int _color;
	};
	
//=============================================================================
// class Polygon declaration
//=============================================================================

	class Polygon : public _Control
	{
	public:
		Polygon(_Manager& m, int n, const int x[], const int y[], int c, bool f = false);
		Polygon(_Manager& m, int n, const int xy[][2], int c, bool f = false);
		Polygon(_Manager& m, const std::vector<std::pair<int, int> >& xy, int c, bool f = false);
		~Polygon();
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _count;
		int* _points;
		int _color;
		bool _fill;
	};

//=============================================================================
// class Rect declaration
//=============================================================================

	class Rect : public _Control
	{
	public:
		Rect(_Manager& m, int l, int t, int r, int b, int c, bool f = false);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _left;
		int _top;
		int _right;
		int _bottom;
		int _color;
		bool _fill;
	};

//=============================================================================
// class Spline declaration
//=============================================================================

	class Spline : public _Control
	{
	public:
		Spline(_Manager& m, int x1, int y1, int cx1, int cy1, int cx2, int cy2, int x2, int y2, int c);
		Spline(_Manager& m, const int p1[2], const int c1[2], const int c2[2], const int p2[2], int c);
		Spline(_Manager& m, const int xy[2][2], const int cxy[2][2], int c);
		Spline(_Manager& m, const int xyc[4][2], int c);	// 0-1=xy1, 2-3=cxy1 4-5=cxy2 6-7=xy2
		Spline(_Manager& m, const std::vector<std::pair<int, int> >& xyc, int c);
		Spline(_Manager& m, const std::vector<int>& xyc, int c);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _points[8];
		int _color;
	};

//=============================================================================
// class Triangle declaration
//=============================================================================

	class Triangle : public _Control
	{
	public:
		Triangle(_Manager& m, int x1, int y1, int x2, int y2, int x3, int y3, int c, bool f = false);
		Triangle(_Manager& m, const int x[3], const int y[3], int c, bool f = false);
		Triangle(_Manager& m, const int xy[3][2], int c, bool f = false);
		
		void paint(BITMAP* bmp);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _points[3][2];
		int _color;
		bool _fill;
	};

//=============================================================================
// class Arc inline implementation
//=============================================================================

	inline Arc::Arc(_Manager& m, int x, int y, float s, float e, int r, int c)
			:
			_Control(m),
			_x(x),
			_y(y),
			_start(0),
			_end(0),
			_radius(r),
			_color(c)
	{
		const double start = s * double(256) / double(360);
		_start = ftofix(start);
		const double end = e * double(256) / double(360);
		_end = ftofix(end);
	}
	
#ifdef	_DEBUG
	inline void Arc::dump() const
	{
		TRACE("gui::Arc @ $%08X\n", this);
		TRACE(" X: %d\n", _x);
		TRACE(" Y: %d\n", _y);
		TRACE(" Start Angle: %lf\n", fixtof(_start));
		TRACE(" End Angle: %lf\n", fixtof(_end));
		TRACE(" Radius: %d\n", _radius);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
	}
#endif
	
	inline void Arc::paint(BITMAP* bmp)
	{
		arc(bmp, _x, _y, _start, _end, _radius, _color);
	}

//=============================================================================
// class Circle inline implementation
//=============================================================================

	inline Circle::Circle(_Manager& m, int x, int y, int r, int c, bool f)
			:
			_Control(m),
			_x(x),
			_y(y),
			_radius(r),
			_color(c),
			_fill(f)
	{
	}
	
#ifdef	_DEBUG
	inline void Circle::dump() const
	{
		TRACE("gui::Circle @ $%08X\n", this);
		TRACE(" X: %d\n", _x);
		TRACE(" Y: %d\n", _y);
		TRACE(" Radius: %d\n", _radius);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
		TRACE(" Fill: %s\n", _fill ? "Yes" : "No");
	}
#endif

	inline void Circle::paint(BITMAP* bmp)
	{
		if (_fill)
			circlefill(bmp, _x, _y, _radius, _color);
		else
			circle(bmp, _x, _y, _radius, _color);
	}

//=============================================================================
// class Ellipse inline implementation
//=============================================================================

	inline Ellipse::Ellipse(_Manager& m, int x, int y, int hr, int vr, int c, bool f)
			:
			_Control(m),
			_x(x),
			_y(y),
			_radiush(hr),
			_radiusv(vr),
			_color(c),
			_fill(f)
	{
	}
	
#ifdef	_DEBUG
	inline void Ellipse::dump() const
	{
		TRACE("gui::Ellipse @ $%08X\n", this);
		TRACE(" X: %d\n", _x);
		TRACE(" Y: %d\n", _y);
		TRACE(" Horizontal Radius: %d\n", _radiush);
		TRACE(" Vertical Radius: %d\n", _radiusv);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
		TRACE(" Fill: %s\n", _fill ? "Yes" : "No");
	}
#endif

	inline void Ellipse::paint(BITMAP* bmp)
	{
		if (_fill)
			ellipsefill(bmp, _x, _y, _radiush, _radiusv, _color);
		else
			ellipse(bmp, _x, _y, _radiush, _radiusv, _color);
	}

//=============================================================================
// class FloodFill inline implementation
//=============================================================================

	inline FloodFill::FloodFill(_Manager& m, int x, int y, int c)
			:
			_Control(m),
			_x(x),
			_y(y),
			_color(c)
	{
	}
	
#ifdef	_DEBUG
	inline void FloodFill::dump() const
	{
		TRACE("gui::FloodFill @ $%08X\n", this);
		TRACE(" X: %d\n", _x);
		TRACE(" Y: %d\n", _y);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
	}
#endif

	inline void FloodFill::paint(BITMAP* bmp)
	{
		floodfill(bmp, _x, _y, _color);
	}

//=============================================================================
// class Line inline implementation
//=============================================================================

	inline Line::Line(_Manager& m, int x1, int y1, int x2, int y2, int c)
			:
			_Control(m),
			_x1(x1),
			_y1(y1),
			_x2(x2),
			_y2(y2),
			_color(c)
	{
	}

	inline Line::Line(_Manager& m, int x, int y, int xy, int c, bool horizontal)
			:
			_Control(m),
			_x1(x),
			_y1(y),
			_x2(horizontal ? xy : x),
			_y2(horizontal ? y : xy),
			_color(c)
	{
	}
	
#ifdef	_DEBUG
	inline void Line::dump() const
	{
		TRACE("gui::Line @ $%08X\n", this);
		TRACE(" Start: %d, %d\n", _x1, _y1);
		TRACE(" End: %d, %d\n", _x2, _y2);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
	}
#endif

	inline void Line::paint(BITMAP* bmp)
	{
		line(bmp, _x1, _y1, _x2, _y2, _color);
	}

//=============================================================================
// class Pixel inline implementation
//=============================================================================

	inline Pixel::Pixel(_Manager& m, int x, int y, int c)
			:
			_Control(m),
			_x(x),
			_y(y),
			_color(c)
	{
	}
	
#ifdef	_DEBUG
	inline void Pixel::dump() const
	{
		TRACE("gui::Pixel @ $%08X\n", this);
		TRACE(" X: %d\n", _x);
		TRACE(" Y: %d\n", _y);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
	}
#endif
	
	inline void Pixel::paint(BITMAP* bmp)
	{
		putpixel(bmp, _x, _y, _color);
	}
	
//=============================================================================
// class Polygon inline implementation
//=============================================================================

//		int _count;
//		int* _points;
//		int _color;
//		bool _fill;

	inline Polygon::Polygon(_Manager& m, int n, const int x[], const int y[], int c, bool f)
			:
			_Control(m),
			_count(n),
			_color(c),
			_fill(f)
	{
		_points = new int[_count * 2];
		for (int i = 0; i < _count; ++i)
		{
			_points[i * 2] = x[i];
			_points[(i * 2) + 1] = y[i];
		}
	}

	inline Polygon::Polygon(_Manager& m, int n, const int xy[][2], int c, bool f)
			:
			_Control(m),
			_count(n),
			_color(c),
			_fill(f)
	{
		_points = new int[_count * 2];
		for (int i = 0; i < _count; ++i)
		{
			_points[i * 2] = xy[i][0];
			_points[(i * 2) + 1] = xy[i][1];
		}
	}

	inline Polygon::Polygon(_Manager& m, const std::vector<std::pair<int, int> >& xy, int c, bool f)
			:
			_Control(m),
			_count(numeric_cast<int>(xy.size())),
			_color(c),
			_fill(f)
	{
		_points = new int[xy.size() * 2];
		for (int i = 0; i < _count; ++i)
		{
			_points[i * 2] = xy[i].first;
			_points[(i * 2) + 1] = xy[i].second;
		}
	}

	inline Polygon::~Polygon()
	{
		delete [] _points;
	}
		
#ifdef	_DEBUG
	inline void Polygon::dump() const
	{
		TRACE("gui::Polygon @ $%08X\n", this);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
		TRACE(" Fill: %s\n", _fill ? "Yes" : "No");
		TRACE(" Points: %d\n", _count);
		TRACE(">>>>>>>>>>\n");
		for (int i = 0; i < _count; ++i)
		{
			TRACE("  Point: %d,%d\n", _points[i * 2], _points[(i * 2) + 1]);
		}
		TRACE("<<<<<<<<<<\n");
	}
#endif
	
	inline void Polygon::paint(BITMAP* bmp)
	{
		if (_fill)
			polygon(bmp, _count, _points, _color);
		else
		{
			for (int i = 0; i < _count - 1; ++i)
			{
				line(bmp, _points[i * 2], _points[(i * 2) + 1],
						_points[(i + 1) * 2], _points[((i + 1) * 2) + 1],
						_color);
			}
			line(bmp, _points[(_count - 1) * 2], _points[((_count - 1) * 2) + 1],
					_points[0], _points[1],
					_color);
		}
	}

//=============================================================================
// class Rect inline implementation
//=============================================================================

	inline Rect::Rect(_Manager& m, int l, int t, int r, int b, int c, bool f)
			:
			_Control(m),
			_left(l),
			_top(t),
			_right(r),
			_bottom(b),
			_color(c),
			_fill(f)
	{
	}
	
#ifdef	_DEBUG
	inline void Rect::dump() const
	{
		TRACE("gui::Rect @ $%08X\n", this);
		TRACE(" Left: %d\n", _left);
		TRACE(" Top: %d\n", _top);
		TRACE(" Right: %d\n", _right);
		TRACE(" Bottom: %d\n", _bottom);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
		TRACE(" Fill: %s\n", _fill ? "Yes" : "No");
	}
#endif

	inline void Rect::paint(BITMAP* bmp)
	{
		if (_fill)
			rectfill(bmp, _left, _top, _right, _bottom, _color);
		else
			rect(bmp, _left, _top, _right, _bottom, _color);
	}

//=============================================================================
// class Spline inline implementation
//=============================================================================

	inline Spline::Spline(_Manager& m, int x1, int y1, int cx1, int cy1, int cx2,
			int cy2, int x2, int y2, int c)
			:
			_Control(m),
			_color(c)
	{
		_points[0] = x1;
		_points[1] = y1;
		_points[2] = cx1;
		_points[3] = cy1;
		_points[4] = cx2;
		_points[5] = cy2;
		_points[6] = x2;
		_points[7] = y2;
	}

	inline Spline::Spline(_Manager& m, const int p1[2], const int c1[2],
			const int c2[2], const int p2[2], int c)
			:
			_Control(m),
			_color(c)
	{
		_points[0] = p1[0];
		_points[1] = p1[1];
		_points[2] = c1[0];
		_points[3] = c1[1];
		_points[4] = c2[0];
		_points[5] = c2[1];
		_points[6] = p2[0];
		_points[7] = p2[1];
	}

	inline Spline::Spline(_Manager& m, const int xy[2][2], const int cxy[2][2],
			int c)
			:
			_Control(m),
			_color(c)
	{
		_points[0] = xy[0][0];
		_points[1] = xy[0][1];
		_points[2] = cxy[0][0];
		_points[3] = cxy[0][1];
		_points[4] = cxy[1][0];
		_points[5] = cxy[1][1];
		_points[6] = xy[1][0];
		_points[7] = xy[1][1];
	}

	inline Spline::Spline(_Manager& m, const int xyc[4][2], int c)
			:
			_Control(m),
			_color(c)
	{
		_points[0] = xyc[0][0];
		_points[1] = xyc[0][1];
		_points[2] = xyc[1][0];
		_points[3] = xyc[1][1];
		_points[4] = xyc[2][0];
		_points[5] = xyc[2][1];
		_points[6] = xyc[3][0];
		_points[7] = xyc[3][1];
	}

	inline Spline::Spline(_Manager& m, const std::vector<std::pair<int, int> >& xyc,
			int c)
			:
			_Control(m),
			_color(c)
	{
		_points[0] = xyc[0].first;
		_points[1] = xyc[0].second;
		_points[2] = xyc[1].first;
		_points[3] = xyc[1].second;
		_points[4] = xyc[2].first;
		_points[5] = xyc[2].second;
		_points[6] = xyc[3].first;
		_points[7] = xyc[3].second;
	}
	
	inline Spline::Spline(_Manager& m, const std::vector<int>& xyc, int c)
			:
			_Control(m),
			_color(c)
	{
		_points[0] = xyc[0];
		_points[1] = xyc[1];
		_points[2] = xyc[2];
		_points[3] = xyc[3];
		_points[4] = xyc[4];
		_points[5] = xyc[5];
		_points[6] = xyc[6];
		_points[7] = xyc[7];
	}

#ifdef	_DEBUG
	inline void Spline::dump() const
	{
		TRACE("gui::Spline @ $%08X\n", this);
		TRACE(" Point 1: %d,%d\n", _points[0], _points[1]);
		TRACE(" Control 1: %d,%d\n", _points[2], _points[3]);
		TRACE(" Control 2: %d,%d\n", _points[4], _points[5]);
		TRACE(" Point 2: %d,%d\n", _points[6], _points[7]);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
	}
#endif

	inline void Spline::paint(BITMAP* bmp)
	{
		spline(bmp, _points, _color);
	}

//=============================================================================
// class Triangle inline implementation
//=============================================================================

	inline Triangle::Triangle(_Manager& m, int x1, int y1, int x2, int y2, int x3, int y3, int c, bool f)
			:
			_Control(m),
			_color(c),
			_fill(f)
	{
		_points[0][0] = x1;
		_points[0][1] = y1;
		_points[1][0] = x2;
		_points[1][1] = y2;
		_points[2][0] = x3;
		_points[2][1] = y3;
	}

	inline Triangle::Triangle(_Manager& m, const int x[3], const int y[3], int c, bool f)
			:
			_Control(m),
			_color(c),
			_fill(f)
	{
		_points[0][0] = x[0];
		_points[0][1] = y[0];
		_points[1][0] = x[1];
		_points[1][1] = y[1];
		_points[2][0] = x[2];
		_points[2][1] = y[2];
	}

	inline Triangle::Triangle(_Manager& m, const int xy[3][2], int c, bool f)
			:
			_Control(m),
			_color(c),
			_fill(f)
	{
		_points[0][0] = xy[0][0];
		_points[0][1] = xy[0][1];
		_points[1][0] = xy[1][0];
		_points[1][1] = xy[1][1];
		_points[2][0] = xy[2][0];
		_points[2][1] = xy[2][1];
	}
	
#ifdef	_DEBUG
	inline void Triangle::dump() const
	{
		TRACE("gui::Triangle @ $%08X\n", this);
		TRACE(" Point 1: %d,%d\n", _points[0][0], _points[0][1]);
		TRACE(" Point 2: %d,%d\n", _points[1][0], _points[1][1]);
		TRACE(" Point 3: %d,%d\n", _points[2][0], _points[2][1]);
		TRACE(" Color: %d %d %d\n", getr(_color), getg(_color), getb(_color));
		TRACE(" Fill: %s\n", _fill ? "Yes" : "No");
	}
#endif

	inline void Triangle::paint(BITMAP* bmp)
	{
		if (_fill)
		{
			triangle(bmp, _points[0][0], _points[0][1], _points[1][0], _points[1][1],
					_points[2][0], _points[2][1], _color);
		}
		else
		{
			// BUGBUG:  This triangle is not the same as that created by Allegro::triangle
			// (used for filled...).  It appears to me that the issue is the polygon
			// algorythm in Allegro, but there is nothing I can do about it (I messed
			// with the corrdinates to attempt to get it, and it didn't work)
			line(bmp, _points[0][0], _points[0][1], _points[1][0], _points[1][1], _color);
			line(bmp, _points[1][0], _points[1][1], _points[2][0], _points[2][1], _color);
			line(bmp, _points[0][0], _points[0][1], _points[2][0], _points[2][1], _color);
		}
	}
}

#endif
