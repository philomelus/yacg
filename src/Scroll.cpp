#include "precompiled.hpp"
#include "yacg/Scroll.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

// TODO:
//	Make thumb size relative to page size instead of relative to line size
//	Update draw for (2) versions, one that is Windows XP like, and 1 that is SPace Empires IV like
//	In order to SE4 style, I need to watch cursor position ANYTIME it enters us, not just when clicked
//	Doesn't support keyboard manipulation
//	Previous will require changes for focus, which there is no concept of currently
//	Thumb size should be based on page size rather than total scroll range
//	When clicking in scroll area, +/- continues even when thumb is now beyond where mouse is down

//=============================================================================
// local objects
//=============================================================================

namespace
{
	// dump thumb start/end coordinates
#ifdef	_DEBUG
	void dumperh(const std::pair<int, int>& i)
	{
		TRACE("  Left: % 5d Right: % 5d\n", i.first, i.second);
	}

	void dumperv(const std::pair<int, int>& i)
	{
		TRACE("  Top: % 5d Bottom: % 5d\n", i.first, i.second);
	}
#endif

	// Tracks the mouse
	template <class T>
	bool track_mouse(T& t, BITMAP* bmp, int omx, int omy, int x, int y, int r, int b, int id = 0, int rd = 0)
	{
		// Draw initial hit
		t.paint(bmp, true);
			
		// Loop until mouse button is released
		bool oldHit = true;
		bool newHit = true;
		int mx = omx;
		int my = omy;
		int lastDelay = retrace_count;
		bool repeating = false;
		while ((mouse_b & 1) != 0)
		{
			// If mouse changed position
			if (mouse_y != my || mouse_x != mx)
			{
				// Save new position
				my = mouse_y;
				mx = mouse_x;
				
				// Determine if mouse is still in right arrow
				newHit = (mx > x && mx < r && my >= y && my <= b);
				
				// If mouse entered or left right arrow, update image
				if (oldHit != newHit)
				{
					t.paint(bmp, newHit);
					oldHit = newHit;
				}
			}
			
			// If we are hitting, check for repeat
			if (oldHit)
			{
				// If already repeating, just check repeat delay
				if (repeating)
				{
					if (retrace_count - lastDelay >= rd)
					{
						t.hit(bmp);
						t.update(bmp);
						lastDelay = retrace_count;
					}
				}
				else if (id)
				{
					if (retrace_count - lastDelay >= id)
					{
						repeating = true;
						t.hit(bmp);
						t.update(bmp);
						lastDelay = retrace_count;
					}
				}
			}
		}
		
		// If we ended as a hit, scroll
		if (!repeating && oldHit)
			t.hit(bmp);

		// Make sure right arrow and scroll are correct
		t.paint(bmp, false);
		t.done(bmp);
		
		// Return whether it was a hit at the end
		return oldHit;
	}
}

namespace yacg
{
	namespace ScrollImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		class Base
		{
		public:
			enum state
			{
				up,
				down,
				inactive
			};
			typedef std::vector<std::pair<int, int> > container;
			
		public:
			class tracker
			{
			public:
				virtual void done(BITMAP* bmp) = 0;
				virtual void hit(BITMAP* bmp) = 0;
				virtual void paint(BITMAP* bmp, bool hit) = 0;
				virtual void update(BITMAP* bmp) = 0;
			};
		
		public:
			Base(Scroll& c, int x, int y, int wh, int r, int s);
			virtual ~Base();
			
			std::pair<Scroll::CALLBACK, void*> callback() const;
			void callback(Scroll::CALLBACK f, void* d);

			std::pair<int, int> delay() const;
			void delay(int i, int r);
			
			int left() const;
			void left(int l);

			int right() const;
			
			int top() const;
			void top(int t);
			int bottom() const;

			int length() const;
			void length(int l);
								
			int height() const;
			int width() const;

			int line() const;
			void line(int l);

			int page() const;
			void page(int p);

			int range() const;
			void range(int r);

			int setting() const;
			void setting(int s);

			int thumb_start() const
			{
				const int index = _setting / _line;
				const int size = int(_thumbs.size());
				return _thumbs[index < size ? index : size].first;
			}
			
			int thumb_end() const
			{
				const int index = _setting / _line;
				const int size = int(_thumbs.size());
				return _thumbs[index < size ? index : size].second;
			}
			
			virtual void paint_arrow1(BITMAP* bmp, state s) = 0;
			virtual void paint_arrow2(BITMAP* bmp, state s) = 0;
			virtual void paint_scroll1(BITMAP* bmp, state s) = 0;
			virtual void paint_scroll2(BITMAP* bmp, state s) = 0;
			virtual void paint_thumb(BITMAP* bmp, state s) = 0;

			virtual bool mouse_down(BITMAP* bmp, int x, int y, int m) = 0;

			virtual void recalc() = 0;
			virtual void recalc_br() = 0;
			
#ifdef	_DEBUG
			virtual void dump() const = 0;
#endif

		protected:
			Scroll& _s;
			
			BITMAP* _altPattern;

			container _thumbs;	

			int _top;
			int _bottom;

			int _left;
			int _right;

			int _length;				// w if horiontal, h if vertical (the opposite is always 20)
			
			int _scrollStart;			// Left or Top of scroll area
			int _scrollEnd;				// Right or Bottom of scroll area

			int _range;

			int _setting;

			int _line;					// Amount to advance per arrow

			int _page;					// Amount to advance per page (area between thumb/scroll)

			Scroll::CALLBACK _callback;
			void* _data;

			int _range_max;

			int _initialDelay;
			int _repeatDelay;
		};

//=============================================================================
// class HorizontalBase declaration
//=============================================================================

		class HorizontalBase : public Base
		{
		public:
			HorizontalBase(Scroll& c, int x, int y, int wh, int r, int s);
			bool mouse_down(BITMAP* bmp, int x, int y, int m);
			void recalc();
			void recalc_br();

#ifdef	_DEBUG
			void dump() const;
#endif

		private:
			// Common tracker base
			class tracker_base : public tracker
			{
			public:
				tracker_base(HorizontalBase& h);
				void done(BITMAP* bmp);
				
			public:
				HorizontalBase& _h;
			};
			
			// Arrow tracker classes
			class left_arrow : public tracker_base
			{
			public:
				left_arrow(HorizontalBase& h);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};
			class right_arrow : public tracker_base
			{
			public:
				right_arrow(HorizontalBase& h);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};

			// Scroll tracker classes
			class left_scroll : public tracker_base
			{
			public:
				left_scroll(HorizontalBase& h);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};
			class right_scroll : public tracker_base
			{
			public:
				right_scroll(HorizontalBase& h);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};
					
		private:
			left_arrow _la;
			left_scroll _ls;
			right_arrow _ra;
			right_scroll _rs;
		};

//=============================================================================
// class Horizontal declaration
//=============================================================================

		class Horizontal : public HorizontalBase
		{
		public:
			Horizontal(Scroll& c, int x, int y, int wh, int r, int s);
			void paint_arrow1(BITMAP* bmp, state s);
			void paint_arrow2(BITMAP* bmp, state s);
			void paint_scroll1(BITMAP* bmp, state s);
			void paint_scroll2(BITMAP* bmp, state s);
			void paint_thumb(BITMAP* bmp, state s);
		};
		
//=============================================================================
// class Horizontal3D declaration
//=============================================================================

		class Horizontal3D : public HorizontalBase
		{
		public:
			Horizontal3D(Scroll& c, int x, int y, int wh, int r, int s);
			void paint_arrow1(BITMAP* bmp, state s);
			void paint_arrow2(BITMAP* bmp, state s);
			void paint_scroll1(BITMAP* bmp, state s);
			void paint_scroll2(BITMAP* bmp, state s);
			void paint_thumb(BITMAP* bmp, state s);
		};

//=============================================================================
// class VerticalBase declaration
//=============================================================================
		
		class VerticalBase : public Base
		{
		public:
			VerticalBase(Scroll& c, int x, int y, int wh, int r, int s);
			bool mouse_down(BITMAP* bmp, int x, int y, int m);
			void recalc();
			void recalc_br();

#ifdef	_DEBUG
			void dump() const;
#endif

		private:
			// Common tracker base
			class tracker_base : public tracker
			{
			public:
				tracker_base(VerticalBase& v);
				void done(BITMAP* bmp);
			
			public:
				VerticalBase& _v;
			};
			
			// Arrow tracker classes
			class down_arrow : public tracker_base
			{
			public:
				down_arrow(VerticalBase& v);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};
			class up_arrow : public tracker_base
			{
			public:
				up_arrow(VerticalBase& v);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};

			// Scroll tracker classes
			class down_scroll : public tracker_base
			{
			public:
				down_scroll(VerticalBase& v);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};
			class up_scroll : public tracker_base
			{
			public:
				up_scroll(VerticalBase& v);
				void hit(BITMAP* bmp);
				void paint(BITMAP* bmp, bool hit);
				void update(BITMAP* bmp);
			};
					
		private:
			down_arrow _da;
			down_scroll _ds;
			up_arrow _ua;
			up_scroll _us;
		};

//=============================================================================
// class Vertical declaration
//=============================================================================

		class Vertical : public VerticalBase
		{
		public:
			Vertical(Scroll& c, int x, int y, int wh, int r, int s);
			void paint_arrow1(BITMAP* bmp, state s);
			void paint_arrow2(BITMAP* bmp, state s);
			void paint_scroll1(BITMAP* bmp, state s);
			void paint_scroll2(BITMAP* bmp, state s);
			void paint_thumb(BITMAP* bmp, state s);
		};

//=============================================================================
// class Vertical3D declaration
//=============================================================================

		class Vertical3D : public VerticalBase
		{
		public:
			Vertical3D(Scroll& c, int x, int y, int wh, int r, int s);
			void paint_arrow1(BITMAP* bmp, state s);
			void paint_arrow2(BITMAP* bmp, state s);
			void paint_scroll1(BITMAP* bmp, state s);
			void paint_scroll2(BITMAP* bmp, state s);
			void paint_thumb(BITMAP* bmp, state s);
		};

//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(Scroll& c, int x, int y, int wh, int r, int s)
				:
				_s(c),
				_altPattern(0),
				_top(y),
				_bottom(0),
				_left(x),
				_right(0),
				_length(wh),
				_scrollStart(0),
				_scrollEnd(0),
				_range(r),
				_setting(s),
				_line(1),
				_page(1),
				_callback(0),
				_initialDelay(30),
				_repeatDelay(6)
		{
			_altPattern = create_bitmap(2, 2);
			putpixel(_altPattern, 0, 0, Theme.GRAY);
			putpixel(_altPattern, 0, 1, Theme.WHITE);
			putpixel(_altPattern, 1, 0, Theme.WHITE);
			putpixel(_altPattern, 1, 1, Theme.GRAY);
		}

		Base::~Base()
		{
			destroy_bitmap(_altPattern);
		}

		int Base::bottom() const
		{
			return _bottom;
		}

		std::pair<Scroll::CALLBACK, void*> Base::callback() const
		{
			return std::make_pair(_callback, _data);
		}

		void Base::callback(Scroll::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}

		std::pair<int, int> Base::delay() const
		{
			return std::make_pair(_initialDelay, _repeatDelay);
		}
		
		void Base::delay(int i, int r)
		{
			_initialDelay = i;
			_repeatDelay = r;
		}
		
		int Base::height() const
		{
			return _bottom - _top + 1;
		}
		
		int Base::left() const
		{
			return _left;
		}

		void Base::left(int l)
		{
			if (_left != l)
			{
				_left = l;
				_s.dirty(Scroll::all);
				recalc_br();
				recalc();
			}
		}
		
		int Base::length() const
		{
			return _length;
		}
		
		void Base::length(int l)
		{
			if (_length != l)
			{
				_length = l;
				_s.dirty(Scroll::all);
				recalc_br();
				recalc();
			}
		}
		
		int Base::line() const
		{
			return _line;
		}

		void Base::line(int l)
		{
			if (_line != l)
			{
				_line = l;
				_s.dirty(Scroll::all);
				recalc();
			}
		}

		int Base::page() const
		{
			return _page;
		}

		void Base::page(int p)
		{
			if (_page != p)
			{
				_page = p;
				_s.dirty(Scroll::all);
				recalc();
			}
		}

		int Base::range() const
		{
			return _range;
		}

		void Base::range(int r)
		{
			if (r > 0)
			{
				if (_range != r)
				{
					_range = r;
					_s.dirty(Scroll::all);
					recalc();
				}
			}
			else if (_range != 1)
			{
				_range = 1;
				_s.dirty(Scroll::all);
				recalc();
			}
		}

		int Base::right() const
		{
			return _right;
		}

		int Base::setting() const
		{
			return _setting;
		}

		void Base::setting(int s)
		{
			if (_setting != s)
			{
				_setting = s;
				_s.dirty(Scroll::all);
			}
		}

		int Base::top() const
		{
			return _top;
		}

		void Base::top(int t)
		{
			if (_top != t)
			{
				_top = t;
				_s.dirty(Scroll::all);
				recalc_br();
				recalc();
			}
		}
		
		int Base::width() const
		{
			return _right - _left + 1;
		}
		
//=============================================================================
// class Horizontal implementation
//=============================================================================

		Horizontal::Horizontal(Scroll& c, int x, int y, int wh, int r, int s)
				:
				HorizontalBase(c, x, y, wh, r, s)
		{
		}

		void Horizontal::paint_arrow1(BITMAP* bmp, state s)
		{
			const int x = _left;
			const int y = _top;

			// Frame
			rect(bmp, x, y, x + 15, y + 15, Theme.flat.Frame);
			
			// Interior
			rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.flat.Background);
				
			switch (s)
			{
			case up:
				// Arrow
				putpixel(bmp, x + 5, y + 7, Theme.flat.Foreground);
				putpixel(bmp, x + 6, y + 6, Theme.flat.Foreground);
				putpixel(bmp, x + 6, y + 8, Theme.flat.Foreground);
				putpixel(bmp, x + 7, y + 5, Theme.flat.Foreground);
				putpixel(bmp, x + 7, y + 9, Theme.flat.Foreground);
				vline(bmp, x + 8, y + 4, y + 10, Theme.flat.Foreground);
				break;

			case down:
				{
					const int color = color_plus_delta(Theme.flat.Foreground, 64);
					
					// Down frame				
					rect(bmp, x + 2, y + 2, x + 13, y + 13, color);
					
					// Arrow
					putpixel(bmp, x + 5, y + 7, color);
					vline(bmp, x + 6, y + 6, y + 8, color);
					vline(bmp, x + 7, y + 5, y + 9, color);
					vline(bmp, x + 8, y + 4, y + 10, color);
				}
				break;
			
			case inactive:
				{
					// Arrow
					const int color = color_minus_delta(Theme.flat.Foreground, 48);
					putpixel(bmp, x + 5, y + 7, color);
					putpixel(bmp, x + 6, y + 6, color);
					putpixel(bmp, x + 6, y + 8, color);
					putpixel(bmp, x + 7, y + 5, color);
					putpixel(bmp, x + 7, y + 9, color);
					vline(bmp, x + 8, y + 4, y + 10, color);
				}
				break;
			}
		}
		
		void Horizontal::paint_arrow2(BITMAP* bmp, state s)
		{
			const int x = _right - 15;
			const int y = _top;
			
			// Frame
			rect(bmp, x, y, x + 15, y + 15, Theme.flat.Frame);
			
			// Interior
			rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.flat.Background);
				
			switch (s)
			{
			case up:
				// Arrow
				vline(bmp, x + 6, y + 4, y + 10, Theme.flat.Foreground);
				putpixel(bmp, x + 7, y + 5, Theme.flat.Foreground);
				putpixel(bmp, x + 7, y + 9, Theme.flat.Foreground);
				putpixel(bmp, x + 8, y + 6, Theme.flat.Foreground);
				putpixel(bmp, x + 8, y + 8, Theme.flat.Foreground);
				putpixel(bmp, x + 9, y + 7, Theme.flat.Foreground);
				break;

			
			case down:
				{
					const int color = color_plus_delta(Theme.flat.Foreground, 64);

					// Down frame
					rect(bmp, x + 2, y + 2, x + 13, y + 13, color);
					
					// Arrow
					vline(bmp, x + 6, y + 4, y + 10, color);
					vline(bmp, x + 7, y + 5, y + 9, color);
					vline(bmp, x + 8, y + 6, y + 8, color);
					putpixel(bmp, x + 9, y + 7, color);
				}
				break;
			
			case inactive:
				{
					// Arrow
					const int color = color_minus_delta(Theme.flat.Foreground, 48);
					vline(bmp, x + 6, y + 4, y + 10, color);
					putpixel(bmp, x + 7, y + 5, color);
					putpixel(bmp, x + 7, y + 9, color);
					putpixel(bmp, x + 8, y + 6, color);
					putpixel(bmp, x + 8, y + 8, color);
					putpixel(bmp, x + 9, y + 7, color);
				}
				break;
			}
		}
		
		void Horizontal::paint_scroll1(BITMAP* bmp, state s)
		{
			if (_setting > 0)
			{
				const int end = thumb_start() - 1;
				hline(bmp, _scrollStart, _top, end, Theme.flat.Frame);
				hline(bmp, _scrollStart, _bottom, end, Theme.flat.Frame);
				rectfill(bmp, _scrollStart, _top + 1, end, _bottom - 1,
						s == down ? color_plus_delta(Theme.flat.Background, 64)
						: Theme.flat.Background);
			}
		}
		
		void Horizontal::paint_scroll2(BITMAP* bmp, state s)
		{
			if (_setting < _range_max)
			{
				const int start = thumb_end() + 1;
				hline(bmp, start, _top, _scrollEnd, Theme.flat.Frame);
				hline(bmp, start, _bottom, _scrollEnd, Theme.flat.Frame);
				rectfill(bmp, start, _top + 1, _scrollEnd, _bottom - 1,
						s == down ? color_plus_delta(Theme.flat.Background, 64)
						: Theme.flat.Background);
			}
		}
		
		void Horizontal::paint_thumb(BITMAP* bmp, state s)
		{
			if (s == inactive)
			{
				hline(bmp, _scrollStart, _top, _scrollEnd, Theme.flat.Frame);
				hline(bmp, _scrollStart, _top, _scrollEnd, Theme.flat.Frame);
				rectfill(bmp, _scrollStart, _top + 1, _scrollEnd, _bottom - 1,
						Theme.flat.Background);
			}
			else
			{
				rect(bmp, thumb_start(), _top + 2, thumb_end(), _bottom - 2,
						Theme.flat.Foreground);
				rectfill(bmp, thumb_start() + 1, _top + 3, thumb_end() - 1, _bottom - 3,
						Theme.flat.Background);
			}
		}

//=============================================================================
// class Horizontal3D implementation
//=============================================================================

		Horizontal3D::Horizontal3D(Scroll& c, int x, int y, int wh, int r, int s)
				:
				HorizontalBase(c, x, y, wh, r, s)
		{
		}

		void Horizontal3D::paint_arrow1(BITMAP* bmp, state s)
		{
			const int x = _left;
			const int y = _top;
			switch (s)
			{
			case up:
				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);
				
				// Arrow
				putpixel(bmp, x + 5, y + 7, Theme.BLACK);		
				vline(bmp, x + 6, y + 6, y + 8, Theme.BLACK);
				vline(bmp, x + 7, y + 5, y + 9, Theme.BLACK);
				vline(bmp, x + 8, y + 4, y + 10, Theme.BLACK);
				break;
			
			case down:
				// Interior
				rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.GRAY);

				// Outside
				rect(bmp, x, y, x + 15, y + 15, Theme.GRAY_DARK);
				
				// Arrow
				putpixel(bmp, x + 6, y + 8, Theme.BLACK);		
				vline(bmp, x + 7, y + 7, y + 9, Theme.BLACK);
				vline(bmp, x + 8, y + 6, y + 10, Theme.BLACK);
				vline(bmp, x + 9, y + 5, y + 11, Theme.BLACK);
				break;
			
			case inactive:
				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);
				
				// Arrow
				putpixel(bmp, x + 5, y + 7, Theme.GRAY_DARK);		
				vline(bmp, x + 6, y + 6, y + 8, Theme.GRAY_DARK);
				vline(bmp, x + 7, y + 5, y + 9, Theme.GRAY_DARK);
				vline(bmp, x + 8, y + 4, y + 10, Theme.GRAY_DARK);
				vline(bmp, x + 9, y + 5, y + 11, Theme.WHITE);
				break;
			}
		}
		
		void Horizontal3D::paint_arrow2(BITMAP* bmp, state s)
		{
			const int x = _right - 15;
			const int y = _top;
			switch (s)
			{
			case up:
				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);
				
				// Arrow
				vline(bmp, x + 6, y + 4, y + 10, Theme.BLACK);
				vline(bmp, x + 7, y + 5, y + 9, Theme.BLACK);
				vline(bmp, x + 8, y + 6, y + 8, Theme.BLACK);
				putpixel(bmp, x + 9, y + 7, Theme.BLACK);
				break;
			
			case down:
				// Interior
				rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.GRAY);

				// Outside
				rect(bmp, x, y, x + 15, y + 15, Theme.GRAY_DARK);
				
				// Arrow
				vline(bmp, x + 7, y + 5, y + 11, Theme.BLACK);
				vline(bmp, x + 8, y + 6, y + 10, Theme.BLACK);
				vline(bmp, x + 9, y + 7, y + 9, Theme.BLACK);
				putpixel(bmp, x + 10, y + 8, Theme.BLACK);
				break;
			
			case inactive:
				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);
				
				// Arrow
				vline(bmp, x + 6, y + 4, y + 10, Theme.GRAY_DARK);
				vline(bmp, x + 7, y + 5, y + 9, Theme.GRAY_DARK);
				vline(bmp, x + 8, y + 6, y + 8, Theme.GRAY_DARK);
				putpixel(bmp, x + 9, y + 7, Theme.GRAY_DARK);
				putpixel(bmp, x + 9, y + 8, Theme.WHITE);
				putpixel(bmp, x + 10, y + 8, Theme.WHITE);
				putpixel(bmp, x + 8, y + 9, Theme.WHITE);
				putpixel(bmp, x + 9, y + 9, Theme.WHITE);
				putpixel(bmp, x + 7, y + 10, Theme.WHITE);
				putpixel(bmp, x + 8, y + 10, Theme.WHITE);
				putpixel(bmp, x + 7, y + 11, Theme.WHITE);
				break;
			}
		}
		
		void Horizontal3D::paint_scroll1(BITMAP* bmp, state s)
		{
			if (_setting > 0)
			{
				if (s != down)
					drawing_mode(DRAW_MODE_COPY_PATTERN, _altPattern, 0, 0);
				else
					solid_mode();
				rectfill(bmp, _scrollStart, _top, thumb_start() - 1, _bottom,
						s == down ? Theme.BLACK : Theme.GRAY);
				solid_mode();
			}
		}
		
		void Horizontal3D::paint_scroll2(BITMAP* bmp, state s)
		{
			if (_setting < _range_max)
			{
				if (s != down)
					drawing_mode(DRAW_MODE_COPY_PATTERN, _altPattern, 0, 0);
				else
					solid_mode();
				rectfill(bmp, thumb_end() + 1, _top, _scrollEnd, _bottom,
						s == down ? Theme.BLACK : Theme.GRAY);
				solid_mode();
			}
		}
		
		void Horizontal3D::paint_thumb(BITMAP* bmp, state s)
		{
			if (s == inactive)
			{
				drawing_mode(DRAW_MODE_COPY_PATTERN, _altPattern, 0, 0);
				rectfill(bmp, _scrollStart, _top, _scrollEnd, _bottom,
						Theme.GRAY);
				solid_mode();
			}
			else
			{
				const int l = thumb_start();
				const int t = _top;
				const int r = thumb_end();
				const int b = _bottom;
				
				solid_mode();
				
				// Thumb Interior
				if (r - l > 3)
					rectfill(bmp, l + 2, t + 2, r - 2, b - 2, Theme.GRAY);

				// Thumb top/left
				hline(bmp, l, t, r - 1, Theme.GRAY);
				vline(bmp, l, t + 1, b - 1, Theme.GRAY);

				// Thumb bottom/right
				hline(bmp, l, b, r, Theme.GRAY_VERY_DARK);
				vline(bmp, r, t, b - 1, Theme.GRAY_VERY_DARK);

				// Thumb inset top/left
				hline(bmp, l + 1, t + 1, r - 2, Theme.WHITE);
				vline(bmp, l + 1, t + 2, b - 2, Theme.WHITE);

				// Thumb inset bottom/right
				hline(bmp, l + 1, b - 1, r - 1, Theme.GRAY_DARK);
				vline(bmp, r - 1, t + 1, b - 2, Theme.GRAY_DARK);
			}
		}

//=============================================================================
// class HorizontalBase implementation
//=============================================================================

		HorizontalBase::HorizontalBase(Scroll& c, int x, int y, int wh, int r, int s)
				:
				Base(c, x, y, wh, r, s),
#pragma warning(disable:4355)
				_la(*this),
				_ls(*this),
				_ra(*this),
				_rs(*this)
#pragma warning(default:4355)
		{
			recalc_br();
		}

#ifdef	_DEBUG
		void HorizontalBase::dump() const
		{
			TRACE(" Left: %d\n", _left);
			TRACE(" Top: %d\n", _top);
			TRACE(" Width: %d\n", _length);
			TRACE(" Range: 0 - %d\n", _range - 1);
			TRACE(" Current: %d\n", _setting);
			TRACE(" Line amount: %d\n", _line);
			TRACE(" Page Amount: %d\n", _page);
			TRACE(" Callback: %s\n", _callback ? "Provided" : "Not Specified");
			TRACE(" Data: $%08X\n", _data);
			TRACE(" Scroll Start: %d\n", _scrollStart);
			TRACE(" Scroll End: %d\n", _scrollEnd);
			TRACE(" Thumbs: %d\n", int(_thumbs.size()));
			TRACE(">>>>>>>>>>\n");
			std::for_each(_thumbs.begin(), _thumbs.end(), dumperh);
			TRACE("<<<<<<<<<<\n");
		}
#endif

		bool HorizontalBase::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			// Don't waste cycles checking for ranges that don't scroll
			if (_range <= 1)
				return false;

			// If it wasn't within our control to begin with	
			if (x < _left || x > _right || y < _top || y > _bottom)
				return false;

			const int thumbStart = thumb_start();
			const int thumbEnd = thumb_end();

			// If its in left arrow
			if (x < _scrollStart)
			{
				// Ignore left arrow if setting is at start already
				if (_setting == 0)
					return false;

				// Track mouse
				track_mouse(_la, bmp, x, y, _left, _top, _scrollStart,
						_bottom, _initialDelay, _repeatDelay);
				
				// Let manager know we consumed the mouse
				return true;
			}
			// If its in right arrow
			else if (x > _scrollEnd)
			{
				// Ignore right arrow if setting is at end already
				if (_setting >= _range_max)
					return false;

				// Track mouse
				track_mouse(_ra, bmp, x, y, _scrollEnd, _top, _right,
						_bottom, _initialDelay, _repeatDelay);
				
				// Let manager know we consumed mouse
				return true;
			}
			// If its in thumb
			else if (x >= thumbStart && x <= thumbEnd)
			{
				// Track while mouse is down
				bool oldHit = true;
				bool newHit = true;
				int mx = x;
				int my = y;
				int setting = _setting;
				while ((mouse_b & 1) != 0)
				{
					// If mouse changed position
					if (mouse_x != mx || mouse_y != my)
					{
						// Save new position
						mx = mouse_x;
						my = mouse_y;

						// Redraw thumb in correct new position
						if (mx < _left || mx > _right || my < _top || my > _bottom)
						{
							// Not in control, so draw in original location
							begin_update(bmp);
							{
								paint_scroll1(bmp, up);
								paint_thumb(bmp, up);
								paint_scroll2(bmp, up);
							}
							end_update(bmp);
						}
						else
						{
							// Calculate new setting
							const container::size_type size = _thumbs.size();
							for (container::size_type i = 0; i < size; ++i)
							{
								if (mx >= _thumbs[i].first && mx <= _thumbs[i].second)
								{
									setting = int(i) * _line;
									break;
								}
							}
							
							// Update position
							const int temp = _setting;
							_setting = setting;
							begin_update(bmp);
							{
								paint_scroll1(bmp, up);
								paint_thumb(bmp, up);
								paint_scroll2(bmp, up);
							}
							end_update(bmp);
							_setting = temp;
						}
					}
				}
				
				// If mouse was released within control (as far as I can tell), then update setting
				if (mx >= _left && mx <= _right && my >= _top && my <= _bottom)
					_setting = setting;
				
				// Make sure state of scroll is correct
				begin_update(bmp);
				{
					paint_scroll1(bmp, up);
					paint_thumb(bmp, up);
					paint_scroll2(bmp, up);
				}
				end_update(bmp);
				return true;
			}
			// If its in left scroll area
			else if (x >= _scrollStart && x < thumbStart)
			{
				// Track mouse
				track_mouse(_ls, bmp, x, y, _scrollStart - 1, _top, thumbStart,
						_bottom, _initialDelay, _repeatDelay);

				// Let manager know we consumed mouse
				return true;
			}
			// If its in right scroll area
			else if (x > thumbEnd && x <= _scrollEnd)
			{
				// Track mouse
				track_mouse(_rs, bmp, x, y, thumbEnd, _top, _scrollEnd + 1,
						_bottom, _initialDelay, _repeatDelay);
				
				// Let manager know we consumed mouse
				return true;
			}

			return false;
		}

		void HorizontalBase::recalc()
		{
			_thumbs.clear();

			const int scroll_width = _length - (2 * Scroll::h_size);					// Width of scroll area in pixels

			const int lines = ((_range / _line) + (_range % _line > 0 ? 1 : 0));		// Number of lines in scroll width

			const int line_width = scroll_width / lines;								// Width of line
			const int line_modulus = scroll_width - (lines * line_width);				// Extra line width to distribute
			
			int start;
			int end;
			for (int i = 0; i < lines - 1; ++i)
			{
				const int remainder = (line_modulus - i) > 0 ? 1 : 0;					// 0 if this is after modulas, 1 if during modulas
				const int my_width = ((line_width < 4) ? 4 : line_width + remainder);	// actual width of _this_ thumb
				start = _scrollStart + (i * line_width);							// actual start of thumb
				if (i > line_modulus)
					start += line_modulus;
				else
					start += i;
				end = start + my_width - 1;
				if (end >= _scrollEnd)
					end = _scrollEnd;
				_thumbs.push_back(std::make_pair(start, end));
			}
			start = _scrollStart + scroll_width - line_width - 1;
			end = _scrollStart + scroll_width - 1;
			if (end - start < 3)
				start = end - 3;
			_thumbs.push_back(std::make_pair(start, end));
			_range_max = int((_range - 1) / _line) * _line;
		}

		void HorizontalBase::recalc_br()
		{
			_right = _left + _length - 1;
			_bottom = _top + 15;
			_scrollStart = _left + 16;
			_scrollEnd = _right - 16;
		}
		
//=============================================================================
// class HorizontalBase::left_arrow implementation
//=============================================================================

		HorizontalBase::left_arrow::left_arrow(HorizontalBase& h)
				:
				tracker_base(h)
		{
		}
		
		void HorizontalBase::left_arrow::hit(BITMAP* bmp)
		{
			const int old = _h._setting;
			int cur = old - _h._line;
			if (cur < 0)
				cur = 0;
			_h._setting = cur;
			if (_h._callback)
				_h._callback(_h._s, _h._data, bmp, old, cur);
		}
		
		void HorizontalBase::left_arrow::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_h.paint_arrow1(bmp, hit ? Base::down : Base::up);
			end_update(bmp);
		}

		void HorizontalBase::left_arrow::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_h.paint_scroll1(bmp, Horizontal3D::up);
			_h.paint_thumb(bmp, Horizontal3D::up);
			_h.paint_scroll2(bmp, Horizontal3D::up);
			end_update(bmp);
		}
		
//=============================================================================
// class HorizontalBase::left_scroll implementation
//=============================================================================

		HorizontalBase::left_scroll::left_scroll(HorizontalBase& h)
				:
				tracker_base(h)
		{
		}
		
		void HorizontalBase::left_scroll::hit(BITMAP* bmp)
		{
			const int old = _h._setting;
			int cur = old - _h._page;
			if (cur < 0)
				cur = 0;
			_h._setting = cur;
			if (_h._callback)
				_h._callback(_h._s, _h._data, bmp, old, cur);
		}
		
		void HorizontalBase::left_scroll::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_h.paint_scroll1(bmp, hit ? Vertical3D::down : Vertical3D::up);
			_h.paint_thumb(bmp, Vertical3D::up);
			_h.paint_scroll2(bmp, Vertical3D::up);
			end_update(bmp);
		}
		
		void HorizontalBase::left_scroll::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_h.paint_scroll1(bmp, Horizontal3D::down);
			_h.paint_thumb(bmp, Horizontal3D::up);
			_h.paint_scroll2(bmp, Horizontal3D::up);
			end_update(bmp);
		}
		
//=============================================================================
// class HorizontalBase::right_arrow implementation
//=============================================================================

		HorizontalBase::right_arrow::right_arrow(HorizontalBase& h)
				:
				tracker_base(h)
		{
		}
		
		void HorizontalBase::right_arrow::hit(BITMAP* bmp)
		{
			const int old = _h._setting;
			int cur = old + _h._line;
			const int r = _h._range;
			if (cur > _h._range_max)
				cur = _h._range_max;
			_h._setting = cur;
			if (_h._callback)
				_h._callback(_h._s, _h._data, bmp, old, cur);
		}
		
		void HorizontalBase::right_arrow::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_h.paint_arrow2(bmp, hit ? Base::down : Base::up);
			end_update(bmp);
		}
		
		void HorizontalBase::right_arrow::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_h.paint_scroll1(bmp, Horizontal3D::up);
			_h.paint_thumb(bmp, Horizontal3D::up);
			_h.paint_scroll2(bmp, Horizontal3D::up);
			end_update(bmp);
		}
		
//=============================================================================
// class HorizontalBase::right_scroll implementation
//=============================================================================

		HorizontalBase::right_scroll::right_scroll(HorizontalBase& h)
				:
				tracker_base(h)
		{
		}
		
		void HorizontalBase::right_scroll::hit(BITMAP* bmp)
		{
			const int old = _h._setting;
			int cur = old + _h._page;
			const int r = _h._range;
			if (cur > _h._range_max)
				cur = _h._range_max;
			_h._setting = cur;
			if (_h._callback)
				_h._callback(_h._s, _h._data, bmp, old, cur);
		}
		
		void HorizontalBase::right_scroll::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_h.paint_scroll1(bmp, Vertical3D::up);
			_h.paint_thumb(bmp, Vertical3D::up);
			_h.paint_scroll2(bmp, hit ? Vertical3D::down : Vertical3D::up);
			end_update(bmp);
		}
		
		void HorizontalBase::right_scroll::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_h.paint_scroll1(bmp, Horizontal3D::up);
			_h.paint_thumb(bmp, Horizontal3D::up);
			_h.paint_scroll2(bmp, Horizontal3D::down);
			end_update(bmp);
		}
		
//=============================================================================
// class HorizontalBase::tracker_base implementation
//=============================================================================

		HorizontalBase::tracker_base::tracker_base(HorizontalBase& h)
				:
				_h(h)
		{
		}
		
		void HorizontalBase::tracker_base::done(BITMAP* bmp)
		{
			begin_update(bmp);
			_h._s.paint(bmp);
			end_update(bmp);
		}

//=============================================================================
// class Vertical implementation
//=============================================================================

		Vertical::Vertical(Scroll& c, int x, int y, int wh, int r, int s)
				:
				VerticalBase(c, x, y, wh, r, s)
		{
		}
		
		void Vertical::paint_arrow1(BITMAP* bmp, state s)
		{
			const int x = _left;
			const int y = _top;
			
			// Frame
			rect(bmp, x, y, x + 15, y + 15, Theme.flat.Frame);
			
			// Interior
			rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.flat.Background);
			
			switch (s)
			{
			case up:
				// Arrow
				putpixel(bmp, x + 7, y + 6, Theme.flat.Foreground);
				putpixel(bmp, x + 6, y + 7, Theme.flat.Foreground);
				putpixel(bmp, x + 8, y + 7, Theme.flat.Foreground);
				putpixel(bmp, x + 5, y + 8, Theme.flat.Foreground);
				putpixel(bmp, x + 9, y + 8, Theme.flat.Foreground);
				hline(bmp, x + 4, y + 9, x + 10, Theme.flat.Foreground);
				break;

			case down:
				{
					const int color = color_plus_delta(Theme.flat.Foreground, 64);
					
					// Down frame
					rect(bmp, x + 2, y + 2, x + 13, y + 13, color);

					// Arrow
					putpixel(bmp, x + 7, y + 6, color);
					hline(bmp, x + 6, y + 7, x + 8, color);
					hline(bmp, x + 5, y + 8, x + 9, color);
					hline(bmp, x + 4, y + 9, x + 10, color);
				}
				break;
			
			case inactive:
				{
					// Arrow
					const int color = color_minus_delta(Theme.flat.Foreground, 48);
					putpixel(bmp, x + 7, y + 6, color);
					putpixel(bmp, x + 6, y + 7, color);
					putpixel(bmp, x + 8, y + 7, color);
					putpixel(bmp, x + 5, y + 8, color);
					putpixel(bmp, x + 9, y + 8, color);
					hline(bmp, x + 4, y + 9, x + 10, color);
				}
				break;
			}
		}
		
		void Vertical::paint_arrow2(BITMAP* bmp, state s)
		{
			const int x = _left;
			const int y = _bottom - 15;

			// Frame
			rect(bmp, x, y, x + 15, y + 15, Theme.flat.Frame);
			
			// Interior
			rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.flat.Background);
			
			switch (s)
			{
			case up:		
				// Arrow
				hline(bmp, x + 4, y + 6, x + 10, Theme.flat.Foreground);
				putpixel(bmp, x + 5, y + 7, Theme.flat.Foreground);
				putpixel(bmp, x + 9, y + 7, Theme.flat.Foreground);
				putpixel(bmp, x + 6, y + 8, Theme.flat.Foreground);
				putpixel(bmp, x + 8, y + 8, Theme.flat.Foreground);
				putpixel(bmp, x + 7, y + 9, Theme.flat.Foreground);		
				break;
			
			case down:
				{
					const int color = color_plus_delta(Theme.flat.Foreground, 64);
					
					// Down frame
					rect(bmp, x + 2, y + 2, x + 13, y + 13, color);

					// Arrow
					hline(bmp, x + 4, y + 6, x + 10, color);
					hline(bmp, x + 5, y + 7, x + 9, color);
					hline(bmp, x + 6, y + 8, x + 8, color);
					putpixel(bmp, x + 7, y + 9, color);
				}
				break;
			
			case inactive:
				{
					// Arrow
					const int color = color_minus_delta(Theme.flat.Foreground, 48);
					hline(bmp, x + 4, y + 6, x + 10, color);
					putpixel(bmp, x + 5, y + 7, color);
					putpixel(bmp, x + 9, y + 7, color);
					putpixel(bmp, x + 6, y + 8, color);
					putpixel(bmp, x + 8, y + 8, color);
					putpixel(bmp, x + 7, y + 9, color);
				}
				break;
			}
		}
		
		void Vertical::paint_scroll1(BITMAP* bmp, state s)
		{
			if (_setting > 0)
			{
				const int end = thumb_start() - 1;
				vline(bmp, _left, _scrollStart, end, Theme.flat.Frame);
				vline(bmp, _right, _scrollStart, end, Theme.flat.Frame);
				rectfill(bmp, _left + 1, _scrollStart, _right - 1, end,
						s == down ? color_plus_delta(Theme.flat.Background, 64)
						: Theme.flat.Background);
			}
		}
		
		void Vertical::paint_scroll2(BITMAP* bmp, state s)
		{
			if (_setting < _range_max)
			{
				const int start = thumb_end() + 1;
				vline(bmp, _left, start, _scrollEnd, Theme.flat.Frame);
				vline(bmp, _right, start, _scrollEnd, Theme.flat.Frame);
				rectfill(bmp, _left + 1, start, _right - 1, _scrollEnd,
						s == down ? color_plus_delta(Theme.flat.Background, 64)
						: Theme.flat.Background);
			}
		}
		
		void Vertical::paint_thumb(BITMAP* bmp, state s)
		{
			if (s == inactive)
			{
				vline(bmp, _left, _scrollStart, _scrollEnd, Theme.flat.Frame);
				vline(bmp, _left, _scrollStart, _scrollEnd, Theme.flat.Frame);
				rectfill(bmp, _left + 1, _scrollStart, _right - 1, _scrollEnd,
						Theme.flat.Background);
			}
			else
			{
				rect(bmp, _left + 2, thumb_start(), _right - 2, thumb_end(),
						Theme.flat.Foreground);
				rectfill(bmp, _left + 3, thumb_start() + 1, _right - 3, thumb_end() - 1,
						Theme.flat.Background);
			}
		}

//=============================================================================
// class Vertical3D implementation
//=============================================================================

		Vertical3D::Vertical3D(Scroll& c, int x, int y, int wh, int r, int s)
				:
				VerticalBase(c, x, y, wh, r, s)
		{
		}

		void Vertical3D::paint_arrow1(BITMAP* bmp, state s)
		{
			const int x = _left;
			const int y = _top;
			switch (s)
			{
			case up:
				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);

				// Arrow
				putpixel(bmp, x + 7, y + 6, Theme.BLACK);		
				hline(bmp, x + 6, y + 7, x + 8, Theme.BLACK);
				hline(bmp, x + 5, y + 8, x + 9, Theme.BLACK);
				hline(bmp, x + 4, y + 9, x + 10, Theme.BLACK);
				break;
			
			case down:
				// Interior
				rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.GRAY);

				// Outside
				rect(bmp, x, y, x + 15, y + 15, Theme.GRAY_DARK);

				// Arrow
				putpixel(bmp, x + 8, y + 7, Theme.BLACK);		
				hline(bmp, x + 7, y + 8, x + 9, Theme.BLACK);
				hline(bmp, x + 6, y + 9, x + 10, Theme.BLACK);
				hline(bmp, x + 5, y + 10, x + 11, Theme.BLACK);
				break;
			
			case inactive:
				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);

				// Arrow
				putpixel(bmp, x + 7, y + 6, Theme.GRAY_DARK);		
				hline(bmp, x + 6, y + 7, x + 8, Theme.GRAY_DARK);
				hline(bmp, x + 5, y + 8, x + 9, Theme.GRAY_DARK);
				hline(bmp, x + 4, y + 9, x + 10, Theme.GRAY_DARK);
				hline(bmp, x + 5, y + 10, x + 11, Theme.WHITE);
				break;
			}
		}
		
		void Vertical3D::paint_arrow2(BITMAP* bmp, state s)
		{
			const int x = _left;
			const int y = _bottom - 15;
			switch (s)
			{
			case up:		
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);

				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Arrow
				hline(bmp, x + 4, y + 6, x + 10, Theme.BLACK);
				hline(bmp, x + 5, y + 7, x + 9, Theme.BLACK);
				hline(bmp, x + 6, y + 8, x + 8, Theme.BLACK);
				putpixel(bmp, x + 7, y + 9, Theme.BLACK);		
				break;
			
			case down:
				// Interior
				rectfill(bmp, x + 1, y + 1, x + 14, y + 14, Theme.GRAY);

				// Outside
				rect(bmp, x, y, x + 15, y + 15, Theme.GRAY_DARK);
				
				// Arrow
				hline(bmp, x + 5, y + 7, x + 11, Theme.BLACK);
				hline(bmp, x + 6, y + 8, x + 10, Theme.BLACK);
				hline(bmp, x + 7, y + 9, x + 9, Theme.BLACK);
				putpixel(bmp, x + 8, y + 10, Theme.BLACK);
				break;
			
			case inactive:
				// Interior
				rectfill(bmp, x + 2, y + 2, x + 13, y + 13, Theme.GRAY);

				// Top left
				hline(bmp, x, y, x + 14, Theme.GRAY);
				vline(bmp, x, y + 1, y + 14, Theme.GRAY);
				
				// Bottom right
				hline(bmp, x, y + 15, x + 15, Theme.GRAY_VERY_DARK);
				vline(bmp, x + 15, y, y + 14, Theme.GRAY_VERY_DARK);
				
				// Inset top left
				hline(bmp, x + 1, y + 1, x + 13, Theme.WHITE);
				vline(bmp, x + 1, y + 2, y + 13, Theme.WHITE);
				
				// Inset bottom right
				hline(bmp, x + 1, y + 14, x + 14, Theme.GRAY_DARK);
				vline(bmp, x + 14, y + 1, y + 13, Theme.GRAY_DARK);
				
				// Arrow
				hline(bmp, x + 4, y + 6, x + 10, Theme.GRAY_DARK);
				hline(bmp, x + 5, y + 7, x + 9, Theme.GRAY_DARK);
				hline(bmp, x + 6, y + 8, x + 8, Theme.GRAY_DARK);
				putpixel(bmp, x + 7, y + 9, Theme.GRAY_DARK);
				putpixel(bmp, x + 10, y + 7, Theme.WHITE);
				putpixel(bmp, x + 11, y + 7, Theme.WHITE);
				putpixel(bmp, x + 9, y + 8, Theme.WHITE);
				putpixel(bmp, x + 10, y + 8, Theme.WHITE);
				putpixel(bmp, x + 8, y + 9, Theme.WHITE);
				putpixel(bmp, x + 9, y + 9, Theme.WHITE);
				putpixel(bmp, x + 8, y + 10, Theme.WHITE);
				break;
			}
		}
		
		void Vertical3D::paint_scroll1(BITMAP* bmp, state s)
		{
			if (_setting > 0)
			{
				if (s != down)
					drawing_mode(DRAW_MODE_COPY_PATTERN, _altPattern, 0, 0);
				else
					solid_mode();
				rectfill(bmp, _left, _scrollStart, _right, thumb_start() - 1,
						s == down ? Theme.BLACK : Theme.GRAY);
				solid_mode();
			}
		}
		
		void Vertical3D::paint_scroll2(BITMAP* bmp, state s)
		{
			if (_setting < _range_max)
			{
				if (s != down)
					drawing_mode(DRAW_MODE_COPY_PATTERN, _altPattern, 0, 0);
				else
					solid_mode();
				rectfill(bmp, _left, thumb_end() + 1, _right, _scrollEnd,
						s == down ? Theme.BLACK : Theme.GRAY);
				solid_mode();
			}
		}
		
		void Vertical3D::paint_thumb(BITMAP* bmp, state s)
		{
			if (s == inactive)
			{
				drawing_mode(DRAW_MODE_COPY_PATTERN, _altPattern, 0, 0);
				rectfill(bmp, _left, _scrollStart, _right, _scrollEnd,
						Theme.GRAY);
				solid_mode();
			}
			else
			{
				const int l = _left;
				const int t = thumb_start();
				const int r = _right;
				const int b = thumb_end();

				solid_mode();
				
				// Thumb interior
				if (b - t > 3)
					rectfill(bmp, l + 2, t + 2, r - 2, b - 2, Theme.GRAY);

				// Thumb top/left
				hline(bmp, l, t, r - 1, Theme.GRAY);
				vline(bmp, l, t + 1, b - 1, Theme.GRAY);

				// Thumb bottom/right
				hline(bmp, l, b, r, Theme.GRAY_VERY_DARK);
				vline(bmp, r, t, b - 1, Theme.GRAY_VERY_DARK);

				// Thumb inset top/left
				hline(bmp, l + 1, t + 1, r - 2, Theme.WHITE);
				vline(bmp, l + 1, t + 2, b - 1, Theme.WHITE);

				// Thumb inset bottom/right
				hline(bmp, l + 1, b - 1, r - 1, Theme.GRAY_DARK);
				vline(bmp, r - 1, t + 1, b - 1, Theme.GRAY_DARK);
			}
		}

//=============================================================================
// class VerticalBase implementation
//=============================================================================

		VerticalBase::VerticalBase(Scroll& c, int x, int y, int wh, int r, int s)
				:
				Base(c, x, y, wh, r, s),
#pragma warning(disable:4355)
				_da(*this),
				_ds(*this),
				_ua(*this),
				_us(*this)
#pragma warning(default:4355)
		{
			recalc_br();
		}

#ifdef	_DEBUG
		void VerticalBase::dump() const
		{
			TRACE(" Left: %d\n", _left);
			TRACE(" Top: %d\n", _top);
			TRACE(" Height: %d\n", _length);
			TRACE(" Range: 0 - %d\n", _range - 1);
			TRACE(" Current: %d\n", _setting);
			TRACE(" Line amount: %d\n", _line);
			TRACE(" Page Amount: %d\n", _page);
			TRACE(" Callback: %s\n", _callback ? "Provided" : "Not Specified");
			TRACE(" Data: $%08X\n", _data);
			TRACE(" Scroll Start: %d\n", _scrollStart);
			TRACE(" Scroll End: %d\n", _scrollEnd);
			TRACE(" Thumbs: %d\n", int(_thumbs.size()));
			TRACE(">>>>>>>>>>\n");
			std::for_each(_thumbs.begin(), _thumbs.end(), dumperv);
			TRACE("<<<<<<<<<<\n");
		}
#endif

		bool VerticalBase::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			// Don't waste cycles checking for ranges that don't scroll
			if (_range <= 1)
				return false;

			// If it wasn't within our control to begin with	
			if (x < _left || x > _right || y < _top || y > _bottom)
				return false;

			const int thumbStart = thumb_start();
			const int thumbEnd = thumb_end();

			// If its in up arrow
			if (y < _scrollStart)
			{
				// Ignore left arrow if setting is at start already
				if (_setting == 0)
					return false;

				// Track mouse
				track_mouse(_ua, bmp, x, y, _left, _top, _right,
						_scrollStart, _initialDelay, _repeatDelay);
				
				// Let manager know we consumed the mouse
				return true;
			}
			// If its in down arrow
			else if (y > _scrollEnd)
			{
				// Ignore right arrow if setting is at end already
				if (_setting >= _range_max)
					return false;

				// Track mouse
				track_mouse(_da, bmp, x, y, _left, _scrollEnd, _right,
						_bottom + 1, _initialDelay, _repeatDelay);
				
				// Let manager know we consumed mouse
				return true;
			}
			// If its in thumb
			else if (y >= thumbStart && y <= thumbEnd)
			{
				// Track while mouse is down
				bool oldHit = true;
				bool newHit = true;
				int mx = x;
				int my = y;
				int setting = _setting;
				while ((mouse_b & 1) != 0)
				{
					// If mouse changed position
					if (mouse_x != mx || mouse_y != my)
					{
						// Save new position
						mx = mouse_x;
						my = mouse_y;

						// Redraw thumb in correct new position
						if (mx < _left || mx > _right || my < _top || my > _bottom)
						{
							// Not in control, so draw in original location
							begin_update(bmp);
							{
								paint_scroll1(bmp, up);
								paint_thumb(bmp, up);
								paint_scroll2(bmp, up);
							}
							end_update(bmp);
						}
						else
						{
							// Calculate new setting
							const container::size_type size = _thumbs.size();
							for (container::size_type i = 0; i < size; ++i)
							{
								if (my >= _thumbs[i].first && my <= _thumbs[i].second)
								{
									setting = int(i) * _line;
									break;
								}
							}
							
							// Update position
							const int temp = _setting;
							_setting = setting;
							begin_update(bmp);
							{
								paint_scroll1(bmp, up);
								paint_thumb(bmp, up);
								paint_scroll2(bmp, up);
							}
							end_update(bmp);
							_setting = temp;
						}
					}
				}
				
				// If mouse was released within control (as far as I can tell), then update setting
				if (mx >= _left && mx <= _right && my >= _top && my <= _bottom)
					_setting = setting;
				
				// Make sure state of scroll is correct
				begin_update(bmp);
				{
					paint_scroll1(bmp, up);
					paint_thumb(bmp, up);
					paint_scroll2(bmp, up);
				}
				end_update(bmp);
				return true;
			}
			// If its in up scroll area
			else if (y >= _scrollStart && y < thumbStart)
			{
				// Track mouse
				track_mouse(_us, bmp, x, y, _left, _scrollStart - 1,
						_right, thumbStart, _initialDelay, _repeatDelay);

				// Let manager know we consumed mouse
				return true;
			}
			// If its in down scroll area
			else if (y > thumbEnd && y <= _scrollEnd)
			{
				// Track mouse
				track_mouse(_ds, bmp, x, y, _left, thumbEnd, _right,
						_scrollEnd + 1, _initialDelay, _repeatDelay);
				
				// Let manager know we consumed mouse
				return true;
			}
			
			return false;
		}
		
		void VerticalBase::recalc()
		{
			_thumbs.clear();

			const int scroll_height = _length - (2 * Scroll::v_size);					// Height of scroll area in pixels

			const int lines = ((_range / _line) + (_range % _line > 0 ? 1 : 0));		// Number of lines in scroll width
			
			const int line_height = scroll_height / lines;								// Height of line
			const int line_modulus = scroll_height - (lines * line_height);				// Extra line height to distribute
			
			int start;
			int end;
			for (int i = 0; i < lines - 1; ++i)
			{
				const int remainder = (line_modulus - i) > 0 ? 1 : 0;					// 0 if this is after modulas, 1 if during modulas
				const int my_height = ((line_height < 4) ? 4 : line_height + remainder);// Actual height of _this_ thumb
				start = _scrollStart + (i * line_height);							// Actual start of thumb
				if (i > line_modulus)
					start += line_modulus;
				else
					start += i;
				end = start + my_height - 1;
				if (end >= _scrollEnd)
					end = _scrollEnd;
				_thumbs.push_back(std::make_pair(start, end));
			}
			start = _scrollStart + scroll_height - line_height - 1;
			end = _scrollStart + scroll_height - 1;
			if (end - start < 3)
				start = end - 3;
			_thumbs.push_back(std::make_pair(start, end));
			_range_max = int((_range - 1) / _line) * _line;
		}

		void VerticalBase::recalc_br()
		{
			_right = _left + 15;
			_bottom = _top + _length - 1;
			_scrollStart = _top + 16;
			_scrollEnd = _bottom - 16;
		}

//=============================================================================
// class VerticalBase::down_arrow implementation
//=============================================================================

		VerticalBase::down_arrow::down_arrow(VerticalBase& v)
				:
				tracker_base(v)
		{
		}
		
		void VerticalBase::down_arrow::hit(BITMAP* bmp)
		{
			const int old = _v._setting;
			int cur = old + _v._line;
			const int r = _v._range;
			if (cur >= _v._range_max)
				cur = _v._range_max;
			_v._setting = cur;
			if (_v._callback)
				_v._callback(_v._s, _v._data, bmp, old, cur);
		}
		
		void VerticalBase::down_arrow::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_v.paint_arrow2(bmp, hit ? Vertical3D::down : Vertical3D::up);
			end_update(bmp);
		}
		
		void VerticalBase::down_arrow::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_v.paint_scroll1(bmp, Vertical3D::up);
			_v.paint_thumb(bmp, Vertical3D::up);
			_v.paint_scroll2(bmp, Vertical3D::up);
			end_update(bmp);
		}
		
//=============================================================================
// class VerticalBase::down_scroll implementation
//=============================================================================

		VerticalBase::down_scroll::down_scroll(VerticalBase& v)
				:
				tracker_base(v)
		{
		}
		
		void VerticalBase::down_scroll::hit(BITMAP* bmp)
		{
			const int old = _v._setting;
			int cur = old + _v._page;
			if (cur >= _v._range_max)
				cur = _v._range_max;
			_v._setting = cur;
			if (_v._callback)
				_v._callback(_v._s, _v._data, bmp, old, cur);
		}
		
		void VerticalBase::down_scroll::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_v.paint_scroll1(bmp, Vertical3D::up);
			_v.paint_thumb(bmp, Vertical3D::up);
			_v.paint_scroll2(bmp, hit ? Vertical3D::down : Vertical3D::up);
			end_update(bmp);
		}
		
		void VerticalBase::down_scroll::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_v.paint_scroll1(bmp, Vertical3D::up);
			_v.paint_thumb(bmp, Vertical3D::up);
			_v.paint_scroll2(bmp, Vertical3D::down);
			end_update(bmp);
		}
		
//=============================================================================
// class VerticalBase::tracker_base implementation
//=============================================================================

		VerticalBase::tracker_base::tracker_base(VerticalBase& v)
				:
				_v(v)
		{
		}
		
		void VerticalBase::tracker_base::done(BITMAP* bmp)
		{
			begin_update(bmp);
			_v._s.paint(bmp);
			end_update(bmp);
		}
		
//=============================================================================
// class VerticalBase::up_arrow implementation
//=============================================================================

		VerticalBase::up_arrow::up_arrow(VerticalBase& v)
				:
				tracker_base(v)
		{
		}
		
		void VerticalBase::up_arrow::hit(BITMAP* bmp)
		{
			const int old = _v._setting;
			int cur = old - _v._line;
			if (cur < 0)
				cur = 0;
			_v._setting = cur;
			if (_v._callback)
				_v._callback(_v._s, _v._data, bmp, old, _v._setting);
		}
		
		void VerticalBase::up_arrow::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_v.paint_arrow1(bmp, hit ? Vertical3D::down : Vertical3D::up);
			end_update(bmp);
		}
		
		void VerticalBase::up_arrow::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_v.paint_scroll1(bmp, Vertical3D::up);
			_v.paint_thumb(bmp, Vertical3D::up);
			_v.paint_scroll2(bmp, Vertical3D::up);
			end_update(bmp);
		}
		
//=============================================================================
// class VerticalBase::up_scroll implementation
//=============================================================================

		VerticalBase::up_scroll::up_scroll(VerticalBase& v)
				:
				tracker_base(v)
		{
		}
		
		void VerticalBase::up_scroll::hit(BITMAP* bmp)
		{
			const int old = _v._setting;
			int cur = old - _v._page;
			if (cur < 0)
				cur = 0;
			_v._setting = cur;
			if (_v._callback)
				_v._callback(_v._s, _v._data, bmp, old, cur);
		}
		
		void VerticalBase::up_scroll::paint(BITMAP* bmp, bool hit)
		{
			begin_update(bmp);
			_v.paint_scroll1(bmp, hit ? Vertical3D::down : Vertical3D::up);
			_v.paint_thumb(bmp, Vertical3D::up);
			_v.paint_scroll2(bmp, Vertical3D::up);
			end_update(bmp);
		}

		void VerticalBase::up_scroll::update(BITMAP* bmp)
		{
			begin_update(bmp);
			_v.paint_scroll1(bmp, Vertical3D::down);
			_v.paint_thumb(bmp, Vertical3D::up);
			_v.paint_scroll2(bmp, Vertical3D::up);
			end_update(bmp);
		}
	}
}

//=============================================================================
// class Scroll implementation
//=============================================================================

Scroll::Scroll(_Manager& m, int x, int y, int wh, int f, int r, int s)
		:
		_ControlEx(m, f | m_down)
{
	if ((flags() & t_mask) == t_horizontal)
	{
		if (Theme.style == Configuration::STYLE_3D)
			_impl = new ScrollImpl::Horizontal3D(*this, x, y, wh, r, s);
		else
			_impl = new ScrollImpl::Horizontal(*this, x, y, wh, r, s);
	}
	else
	{
		if (Theme.style == Configuration::STYLE_3D)
			_impl = new ScrollImpl::Vertical3D(*this, x, y, wh, r, s);
		else
			_impl = new ScrollImpl::Vertical(*this, x, y, wh, r, s);
	}
}

Scroll::~Scroll()
{
	delete _impl;
}

Scroll::operator int() const
{
	return setting();
}

Scroll& Scroll::operator=(int s)
{
	setting(s);
	return *this;
}

int Scroll::bottom() const
{
	return _impl->bottom();
}

std::pair<Scroll::CALLBACK, void*> Scroll::callback() const
{
	return _impl->callback();
}

void Scroll::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

std::pair<int, int> Scroll::delay() const
{
	return _impl->delay();
}

void Scroll::delay(int initial, int repeat)
{
	_impl->delay(initial, repeat);
}

#ifdef	_DEBUG
void Scroll::dump() const
{
	TRACE("gui::Scroll @ $%08X\n", this);
	TRACE(" Flags:");
	switch (flags() & t_mask)
	{
	case t_horizontal:
		TRACE(" t_horizontal");
		break;
	
	case t_vertical:
		TRACE(" t_vertical");
		break;
	}
	switch (flags() & o_mask)
	{
	case o_frame:
		TRACE(" | o_frame\n");
		break;
	
	case o_noframe:
		TRACE(" | o_norframe\n");
		break;
	}
	_impl->dump();
}
#endif

int Scroll::height() const
{
	return _impl->height();
}

int Scroll::left() const
{
	return _impl->left();
}

void Scroll::left(int l)
{
	_impl->left(l);
}

int Scroll::length() const
{
	return _impl->length();
}

void Scroll::length(int l)
{
	_impl->length(l);
}

int Scroll::line() const
{
	return _impl->line();
}

void Scroll::line(int l)
{
	_impl->line(l);
}

int Scroll::page() const
{
	return _impl->page();
}

void Scroll::page(int p)
{
	_impl->page(p);
}

bool Scroll::mouse_down(BITMAP* b, int x, int y, int m)
{
	return _impl->mouse_down(b, x, y, m);
}

void Scroll::paint(BITMAP* b)
{
	// Determine state
	const ScrollImpl::Base::state s = (active() ? (_impl->range() > 1
			? ScrollImpl::Base::up : ScrollImpl::Base::inactive)
			: ScrollImpl::Base::inactive);
	
	// Frame if desired
	if ((flags() & o_mask) == o_frame)
	{
		begin_update(b);
		{
			switch (Theme.style)
			{
			case Configuration::STYLE_3D:
				rect(b, _impl->left() - 1, _impl->top() - 1, _impl->right() + 1,
						_impl->bottom() + 1, Theme.GRAY_DARK);
				break;
			
			case Configuration::STYLE_FLAT:
				rect(b, _impl->left(), _impl->top(), _impl->right(),
						_impl->bottom(), Theme.flat.Frame);
				break;
			}
		}
		end_update(b);
	}
	else if (Theme.style == Configuration::STYLE_FLAT)
	{
		rect(b, _impl->left(), _impl->top(), _impl->right(), _impl->bottom(),
				Theme.flat.Frame);
	}

	// Paint left/up arrow
	_impl->paint_arrow1(b, s);
	
	// Paint area from arrow to thumb
	_impl->paint_scroll1(b, s);
	
	// Paint thumb
	_impl->paint_thumb(b, s);
	
	// Paint right/down arrow
	_impl->paint_arrow2(b, s);
	
	// Paint area from thumb to arrow
	_impl->paint_scroll2(b, s);
}

int Scroll::range() const
{
	return _impl->range();
}

void Scroll::range(int r)
{
	_impl->range(r);
}

int Scroll::right() const
{
	return _impl->right();
}

int Scroll::setting() const
{
	return _impl->setting();
}

void Scroll::setting(int s)
{
	_impl->setting(s);
}

int Scroll::top() const
{
	return _impl->top();
}

void Scroll::top(int t)
{
	_impl->top(t);
}

int Scroll::width() const
{
	return _impl->width();
}
