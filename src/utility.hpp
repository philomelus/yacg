#ifndef	__UTILITY_HPP__
#define	__UTILITY_HPP__

#ifndef	__STD_VECTOR__
#define __STD_VECTOR__
#include <vector>
#endif

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

#ifndef	__YACG_THEME_HPP__
#include "yacg/Theme.hpp"
#endif

namespace utility
{
//=============================================================================
// class Acquire delcaration
//=============================================================================

	class Acquire
	{
	public:
		Acquire(BITMAP* bmp);
		virtual ~Acquire();
	
	private:
		BITMAP* _bitmap;
	};
	
//=============================================================================
// class auto_bitmap delcaration
//=============================================================================

	class auto_bitmap
	{
	public:
		explicit auto_bitmap(BITMAP* bmp = 0);
		auto_bitmap(auto_bitmap& r);
		auto_bitmap& operator=(auto_bitmap& r);
		~auto_bitmap();

		operator BITMAP*() const;
		
		BITMAP* operator->() const;

		BITMAP* get() const;

		BITMAP* release();

		void reset(BITMAP* bmp = 0);

	private:
		BITMAP* _bitmap;
	};
	
//=============================================================================
// class Clipper delcaration
//=============================================================================

	class Clipper
	{
	public:
		Clipper(BITMAP* bmp, int l, int t, int r, int b);
		virtual ~Clipper();
	
	private:
		BITMAP* _bitmap;
		int _oldL;
		int _oldT;
		int _oldR;
		int _oldB;
		int _oldS;
	};
	
//=============================================================================
// class Update delcaration
//=============================================================================

	class Update
	{
	public:
		Update(BITMAP* bmp);
		virtual ~Update();
	
	private:
		BITMAP* _bitmap;
	};
	
//=============================================================================
// function inline implementation
//=============================================================================

	inline void begin_update(BITMAP* b)
	{
		show_mouse(0);
		acquire_bitmap(b);
	}

	inline void end_update(BITMAP* b)
	{
		release_bitmap(b);
		show_mouse(b);
	}

	inline void box(BITMAP* bmp, int x, int y, int r, int b, bool inset = false, int interior = -1)
	{
		int tlc;
		int itlc;
		int ibrc;
		int brc;
		if (inset)
		{
			tlc = yacg::GRAY_VERY_DARK();
			brc = yacg::WHITE();
			itlc = yacg::GRAY_DARK();
			ibrc = yacg::GRAY();
		}
		else
		{
			tlc = yacg::GRAY();
			brc = yacg::GRAY_VERY_DARK();
			itlc = yacg::WHITE();
			ibrc = yacg::GRAY_DARK();
		}
		
		// Top/left
		hline(bmp, x, y, r - 1, tlc);
		vline(bmp, x, y + 1, b - 1, tlc);

		// Bottom/right
		hline(bmp, x, b, r, brc);
		vline(bmp, r, y, b - 1, brc);
		
		// Inset top/left
		hline(bmp, x + 1, y + 1, r - 2, itlc);
		vline(bmp, x + 1, y + 2, b - 2, itlc);

		// Inset bottom/right
		hline(bmp, x + 1, b - 1, r - 1, ibrc);
		vline(bmp, r - 1, y + 1, b - 2, ibrc);

		// Fill interior if desired
		if (interior != -1)
			rectfill(bmp, x + 2, y + 2, r - 2, b - 2, interior);
	}

	inline int color_plus_delta(int c, int p)
	{
		int r = getr(c);
		int g = getg(c);
		int b = getb(c);
		int m = r;
		if (g > m)
			m = g;
		if (b > m)
			m = b;
		if (m + p > 255)
			p = 255 - m;
		return makecol(r + p, g + p, b + p);
	}

	inline int color_minus_delta(int c, int p)
	{
		int r = getr(c);
		int g = getg(c);
		int b = getb(c);
		int m = r;
		if (g < m)
			m = g;
		if (b < m)
			m = b;
		if (m - p < 0)
			p = m;
		return makecol(r - p, g - p, b - p);
	}

	// std::for_each helpers
	
	inline void control_dirty(yacg::_Control* c)
	{
		c->dirty(yacg::_Control::dirty_all);
	}
	
#ifdef	_DEBUG
	inline void control_dump(const yacg::_Control* c)
	{
		c->dump();
	}
#endif

	inline void control_enable(yacg::_Control* c)
	{
		c->active(true);
	}
	
	inline void control_disable(yacg::_Control* c)
	{
		c->active(false);
	}
	
	inline void control_hide(yacg::_Control* c)
	{
		c->visible(false);
	}
	
	inline void control_paint(yacg::_Control* c, BITMAP* bmp)
	{
		c->paint(bmp);
	}
	
	inline void control_show(yacg::_Control* c)
	{
		c->visible(true);
	}

	typedef std::vector<std::pair<int, int> > distribution;
	inline void distribute(int size, int count, distribution& d)
	{
		const int segment = size / count;
		const int modulus = size % count;
		for (int i = 0; i < count; ++i)
		{
			const int remainder = ((modulus - i) > 0) ? 1 : 0;
			const int current = segment + remainder;
			int start = i * segment;
			if (i > modulus)
				start += modulus;
			else
				start += i;
			int end = start + current - 1;
if (end > size - 1)	// Shouldn't, and hasn't yet, be true
	_asm int 3;
			d.push_back(std::make_pair(start, end));
		}
	}

#ifdef	_DEBUG
	inline void dump_color(const char* n, int c)
	{
		TRACE(" %s: %d %d %d\n", n, getr(c), getg(c), getb(c));
	}
#endif

#ifdef	_DEBUG
	inline void dump_font(FONT* f)
	{
		TRACE(" Font: $%08X%s\n", f, (f == ::font ? " (Allegro Global Font)" : ""));
	}
#endif

#ifdef	_DEBUG
	inline void dump_xywh(int x, int y, int w, int h, int r, int b)
	{
		TRACE(" Left: %d\n", x);
		TRACE(" Top: %d\n", y);
		TRACE(" Width (right): %d (%d)\n", w, r);
		TRACE(" Height (bottom): %d (%d)\n", h, b);
	}
#endif

	template <typename T>
	void object_delete(T* o)
	{
		delete o;
	}
	
//=============================================================================
// class Acquire inline implementation
//=============================================================================

	inline Acquire::Acquire(BITMAP* bmp)
			:
			_bitmap(bmp)
	{
		acquire_bitmap(_bitmap);
	}
	
	inline Acquire::~Acquire()
	{
		release_bitmap(_bitmap);
	}

//=============================================================================
// class auto_bitmap delcaration
//=============================================================================

	inline auto_bitmap::auto_bitmap(BITMAP* bmp)
			:
			_bitmap(bmp)
	{
	}

	inline auto_bitmap::auto_bitmap(auto_bitmap& r)
			:
			_bitmap(r.release())
	{
	}

	inline auto_bitmap::operator BITMAP*() const
	{
		return _bitmap;
	}

	inline auto_bitmap& auto_bitmap::operator=(auto_bitmap& r)
	{
		if (this != &r)
			reset(r.release());
		return *this;
	}

	inline auto_bitmap::~auto_bitmap()
	{
		destroy_bitmap(_bitmap);
	}

	inline BITMAP* auto_bitmap::operator->() const
	{
		return _bitmap;
	}

	inline BITMAP* auto_bitmap::get() const
	{
		return _bitmap;
	}

	inline BITMAP* auto_bitmap::release()
	{
		BITMAP* t = _bitmap;
		_bitmap = 0;
		return t;
	}

	inline void auto_bitmap::reset(BITMAP* bmp)
	{
		if (_bitmap != bmp)
			destroy_bitmap(_bitmap);
		_bitmap = bmp;
	}
	
//=============================================================================
// class Clipper inline implementation
//=============================================================================

	inline Clipper::Clipper(BITMAP* bmp, int l, int t, int r, int b)
			:
			_bitmap(bmp)
	{
		get_clip_rect(bmp, &_oldL, &_oldT, &_oldR, &_oldB);
		_oldS = get_clip_state(bmp);
		set_clip_state(bmp, 1);
		set_clip_rect(bmp, l, t, r, b);
	}
	
	inline Clipper::~Clipper()
	{
		set_clip_rect(_bitmap, _oldL, _oldT, _oldR, _oldB);
		set_clip_state(_bitmap, _oldS);
	}

//=============================================================================
// class Update inline implementation
//=============================================================================

	inline Update::Update(BITMAP* bmp)
			:
			_bitmap(bmp)
	{
		begin_update(_bitmap);
	}
	
	inline Update::~Update()
	{
		end_update(_bitmap);
	}
}

#endif
