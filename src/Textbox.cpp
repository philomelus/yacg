#include "precompiled.hpp"
#include "yacg/Textbox.hpp"
#include "yacg/Scroll.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace
{
	void dumper(const std::string& s)
	{
		TRACE("\"%s\"\n", s.c_str());
	}
	
	void max_length(const std::string& s, const FONT* f, int* l)
	{
		const int length = text_length(f, s.c_str());
		if (length > *l)
			*l = length;
	}
}

//=============================================================================
// class Base declaration
//=============================================================================

namespace yacg
{
	namespace TextboxImpl
	{
		class Base
		{
		public:
			// dirty() hints
			static const int dirty_frame = 0x00000001;
			static const int dirty_text = 0x00000002;
			static const int dirty_hscroll = 0x00000004;
			static const int dirty_vscroll = 0x00000008;
			static const int dirty_margins = 0x00000010;
			
		public:
			Base(Textbox& o, int x, int y, int w, int h, int f, const char* t);
			virtual ~Base();
			
			const char* operator[](int i) const;

			int background() const;
			void background(int c);
			
			int bottom() const;
			
			int count() const;

			FONT* font() const;
			void font(FONT *pFont);

			int foreground() const;
			void foreground(int c);
			
			int height() const;

			Scroll& hscroll() const;
			Scroll& vscroll() const;
						
			int left() const;

			std::pair<int, int> margin_h() const;
			void margin_h(int l, int r);
					
			std::pair<int, int> margin_v() const;
			void margin_v(int t, int b);

			virtual void paint_frame(BITMAP* b) = 0;
			virtual void paint_margins(BITMAP* b) = 0;
			virtual void paint_text(BITMAP* b) = 0;
			
			int right() const;

			void text(const char* t);

			int top() const;
			
			int width() const;

#ifdef	_DEBUG
			void dump() const;
#endif

		protected:
			static void callback_h(Scroll& s, void* d, BITMAP* b, int prev, int cur);
			static void callback_v(Scroll& s, void* d, BITMAP* b, int prev, int cur);
			void recalc_scrolls();

			virtual void recalc_boundary() = 0;

		private:
			typedef std::vector<std::string> container;
		
		protected:
			Textbox& _owner;
			
			int _top;
			int _bottom;

			int _height;

			int _left;
			int _right;

			int _width;

			FONT* _font;

			container _lines;
			
			std::auto_ptr<Scroll> _hscroll;
			std::auto_ptr<Scroll> _vscroll;
			
			int _margin_l;
			int _margin_r;

			int _margin_t;
			int _margin_b;

			int _textHeight;	// Maximum height of text
			int _textWidth;		// Maximum length of text
						
			int _background;
			int _foreground;
			
			int _text_left;
			int _text_right;

			int _text_top;
			int _text_bottom;
		};

//=============================================================================
// class Impl implementation
//=============================================================================

		class Impl : public Base
		{
		public:
			Impl(Textbox& o, int x, int y, int w, int h, int f, const char* t);

			void paint_frame(BITMAP* b);
			void paint_margins(BITMAP* b);
			void paint_text(BITMAP* b);

		protected:
			void recalc_boundary();
		};

//=============================================================================
// class Impl3D implementation
//=============================================================================

		class Impl3D : public Base
		{
		public:
			Impl3D(Textbox& o, int x, int y, int w, int h, int f, const char* t);

			void paint_frame(BITMAP* b);
			void paint_margins(BITMAP* b);
			void paint_text(BITMAP* b);

		protected:
			void recalc_boundary();
		};

//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(Textbox& o, int x, int y, int w, int h, int f, const char* t)
				:
				_owner(o),
				_top(y),
				_bottom(y + h - 1),
				_height(h),
				_left(x),
				_right(x + w - 1),
				_width(w),
				_font(0),
				_margin_l(0),
				_margin_r(0),
				_margin_t(0),
				_margin_b(0),
				_background(-1),
				_foreground(-1)
		{
		}
		
		Base::~Base()
		{
		}
		
		const char* Base::operator[](int i) const
		{
			return _lines[i].c_str();
		}
		
		int Base::background() const
		{
			return _background;
		}
		
		void Base::background(int c)
		{
			if (_background != c)
			{
				_background = c;
				_owner.dirty(dirty_text | dirty_margins);
			}
		}

		int Base::bottom() const
		{
			return _bottom;
		}
		
		void Base::callback_h(Scroll& s, void* d, BITMAP* b, int prev, int cur)
		{
#define	_this	reinterpret_cast<Base*>(d)
			_this->paint_text(b);
#undef	_this
		}
		
		void Base::callback_v(Scroll& s, void* d, BITMAP* b, int prev, int cur)
		{
#define	_this	reinterpret_cast<Base*>(d)
			_this->paint_text(b);
#undef	_this
		}
		
		int Base::count() const
		{
			return int(_lines.size());
		}
		
#ifdef	_DEBUG
		void Base::dump() const
		{
			dump_xywh(_left, _top, _width, _height, _right, _bottom);
			TRACE(" Font: ");
			if (_font)
				TRACE("$%08X\n", _font);
			else
				TRACE("Allegro Global Font\n");
			TRACE(" Lines: %d\n", _lines.size());
			TRACE(">>>>>>>>>>\n");
			std::for_each(_lines.begin(), _lines.end(), dumper);
			TRACE("<<<<<<<<<<\n");
		}
#endif

		FONT* Base::font() const
		{
			return _font;
		}

		void Base::font(FONT* f)
		{
			if (_font != f)
			{
				_font = f;
				_owner.dirty(dirty_text | dirty_hscroll | dirty_vscroll);
				recalc_scrolls();
			}
		}

		int Base::foreground() const
		{
			return _foreground;
		}
		
		void Base::foreground(int c)
		{
			if (_foreground != c)
			{
				_foreground = c;
				_owner.dirty(dirty_text);
			}
		}

		int Base::height() const
		{
			return _height;
		}
		
		Scroll& Base::hscroll() const
		{
			return *_hscroll.get();
		}
		
		int Base::left() const
		{
			return _left;
		}

		std::pair<int, int> Base::margin_h() const
		{
			return std::make_pair(_margin_l, _margin_r);
		}
		
		void Base::margin_h(int l, int r)
		{
			if (_margin_l != l || _margin_r != r)
			{
				_margin_l = l;
				_margin_r = r;
				_owner.dirty(dirty_text | dirty_margins);
				recalc_boundary();
			}
		}
			
		std::pair<int, int> Base::margin_v() const
		{
			return std::make_pair(_margin_t, _margin_b);
		}
		
		void Base::margin_v(int t, int b)
		{
			if (_margin_t != t || _margin_b != b)
			{
				_margin_t = t;
				_margin_b = b;
				_owner.dirty(dirty_text | dirty_margins);
				recalc_boundary();
			}
		}

		void Base::recalc_scrolls()
		{
			const FONT* f = _font ? _font : ::font;
			const int flags = _owner.flags();

			// Calculate text width and height
			_textHeight = text_height(f);
			_textWidth = 0;
			std::for_each(_lines.begin(), _lines.end(), bind(max_length, _1, f, &_textWidth));
			
			// Calculate vertical scroll settings if needed
			if ((flags & Textbox::s_vertical) != 0)
			{
				_vscroll->range(int(_lines.size()) * _textHeight);
				_vscroll->line(_textHeight);
				_vscroll->page(int(_vscroll->height() / _textHeight) * _textHeight);
			}
			
			// Calculate horizontal scroll settings if needed
			if ((flags & Textbox::s_horizontal) != 0)
			{
				const int avg_length = (text_length(f, "AaBbCcDdEeFfGgHhIiJjKkLlMmNn"
						"OoPpQqRrSsTtUuVvWwXxYyZz1234567890") / 62);
				_hscroll->range(_textWidth);
				_hscroll->line(avg_length);
				_hscroll->page(_hscroll->width());
			}
		}

		int Base::right() const
		{
			return _right;
		}

		void Base::text(const char* t)
		{
			_lines.clear();
			if (t)
			{
				// Calculate lines
				int i = 0;
				int l = 0;
				int s = 0;
				while (t[i])
				{
					// Find end of line
					while (t[i] && t[i] != '\n')
					{
						++i;
						++l;
					}
					if (t[i])
						++i;
					
					// Get past any other eol chars
					if (t[i] == '\r')
						++i;

					// Add this line to list
					_lines.push_back(std::string(&t[s], l));
					s = i;
					l = 0;

				}
			}
			recalc_boundary();
			recalc_scrolls();
			_owner.dirty(dirty_text | dirty_hscroll | dirty_vscroll);
		}

		int Base::top() const
		{
			return _top;
		}
		
		Scroll& Base::vscroll() const
		{
			return *_vscroll.get();
		}
		
		int Base::width() const
		{
			return _width;
		}

//=============================================================================
// class Impl implementation
//=============================================================================

		Impl::Impl(Textbox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			std::auto_ptr<Scroll> hscroll;
			std::auto_ptr<Scroll> vscroll;

			// If framed, set margin
			const bool isFrame = ((f & Textbox::f_mask) != Textbox::f_none);
			if (isFrame)
			{
				_margin_l = 2;
				_margin_r = 2;
				_margin_t = 2;
				_margin_b = 2;
			}
			const int frame_size = (isFrame ? 1 : 0);
			
			// Initialize vertical scroll if desired
			const bool isHScroll = ((f & Textbox::s_horizontal) != 0);
			const bool isVScroll = ((f & Textbox::s_vertical) != 0);
			if (isVScroll)
			{
				// Create scroll
				vscroll = std::auto_ptr<Scroll>(new Scroll(o.manager(),
						_right - Scroll::h_size + 1, _top,
						_height - (isHScroll ? Scroll::v_size - 1: 0),
						Scroll::t_vertical));
				vscroll->callback(callback_v, this);
			}

			// Initialize horizontal scroll if desired
			if (isHScroll)
			{
				// Create scroll
				hscroll = std::auto_ptr<Scroll>(new Scroll(o.manager(),
						_left, _bottom - Scroll::v_size + 1,
						_width - (isVScroll ? Scroll::h_size - 1 : 0),
						Scroll::t_horizontal));
				hscroll->callback(callback_h, this);
			}

			// Save objects
			_hscroll = hscroll;
			_vscroll = vscroll;

			// Set text
			text(t);
		}
		
		void Impl::paint_frame(BITMAP* bmp)
		{
			const int flags = _owner.flags();

			// Draw disired frame
			if ((flags & Textbox::f_mask) != Textbox::f_none)
			{
				Update u(bmp);
				rect(bmp, _left, _top, _right, _bottom, Theme.flat.Frame);
			}

			// If both vertical and horizontal scrolls, update intersection
			if ((flags & Textbox::s_mask) == Textbox::s_both)
			{
				rectfill(bmp, _right - Scroll::h_size + 2, _bottom - Scroll::v_size + 2,
						_right - 1, _bottom - 1, (_background == -1 ? Theme.flat.Background
						: _background));
			}
		}
		
		void Impl::paint_margins(BITMAP* bmp)
		{
			const int color = (_background == -1 ? Theme.flat.Background : _background);

			Update u(bmp);
						
			// Update left margin
			if (_margin_l)
			{
				for (int n = _text_left - _margin_l; n < _text_left; ++n)
				{
					vline(bmp, n, _text_top - _margin_t, _text_bottom + _margin_b,
							color);
				}
			}

			// Update top margin
			if (_margin_t)
			{
				for (int n = _text_top - _margin_t; n < _text_top; ++n)
				{
					hline(bmp, _text_left - _margin_l, n, _text_right + _margin_r,
							color);
				}
			}

			// Update right margin
			if (_margin_r)
			{
				for (int n = _text_right + _margin_r; n > _text_right; --n)
				{
					vline(bmp, n, _text_top - _margin_t, _text_bottom + _margin_b,
							color);
				}
			}

			// Update bottom margin
			if (_margin_b)
			{
				for (int n = _text_bottom + _margin_b; n > _text_bottom; --n)
				{
					hline(bmp, _text_left - _margin_l, n, _text_right + _margin_r,
							color);
				}
			}
		}
		
		void Impl::paint_text(BITMAP* bmp)
		{
			const int flags = _owner.flags();

			// The font height will determine the number of viewable lines
			const FONT* f = _font ? _font : ::font;

			// Determine line to start drawing
			container::size_type i = 0;
			if ((flags & Textbox::s_vertical) != 0)
				i = _vscroll->setting() / _textHeight;
			
			// Determine collumn to start drawing
			int left_start = 0;
			if ((flags & Textbox::s_horizontal) != 0)
				left_start = _hscroll->setting();
			
			// Determine active background and foreground colors
			const int bcolor = (_background == -1 ? Theme.flat.Background : _background);
			const int fcolor = (_foreground == -1 ? Theme.flat.Foreground : _foreground);
			
			// Create line bitmap
			BITMAP* line = create_bitmap(_textWidth * 2, _textHeight);

			// Draw the text			
			{
				// Prevent updates to anywhere else on user bitmap				
				Clipper c(bmp, _text_left, _text_top, _text_right, _text_bottom);
				Update u(bmp);
				
				// Acquire the bitmaps to speed things up
				Acquire a1(line);
				Acquire a2(bmp);
								
				// Display the lines
				const container::size_type count = _lines.size();
				int t = _text_top;
				while (t < _text_bottom)
				{
					// Clear the bitmap
					clear_to_color(line, bcolor);
					
					// Print the line if needed
					if (i < count)
						textout_ex(line, f, _lines[i].c_str(), 0, 0, fcolor, bcolor);

					// Copy line to user bitmap
					blit(line, bmp, left_start, 0, _text_left, t, _textWidth, _textHeight);
					
					// Next line
					t += _textHeight;
					++i;
				}
			}
			
			// Let go of line bitmap
			destroy_bitmap(line);
		}

		void Impl::recalc_boundary()
		{
			const int flags = _owner.flags();
			
			// Initialize text boundary
			_text_left = _left + _margin_l;
			_text_top = _top + _margin_t;
			_text_right = _right - _margin_r;
			_text_bottom = _bottom - _margin_b;

			// Update for frames
			const bool isFrame = ((flags & Textbox::f_mask) != Textbox::f_none);
			if (isFrame)
			{
				_text_left += 1;
				_text_top += 1;
				_text_right -= 1;
				_text_bottom -= 1;
			}
			
			// Update text boundary for horizontal scroll
			if ((flags & Textbox::s_horizontal) != 0)
				_text_bottom -= (Scroll::v_size - (isFrame ? 1 : 0));
			
			// Update text boundary for vertical scroll
			if ((flags & Textbox::s_vertical) != 0)
				_text_right -= (Scroll::h_size - (isFrame ? 1 : 0));
		}

//=============================================================================
// class Impl3D implementation
//=============================================================================

		Impl3D::Impl3D(Textbox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			std::auto_ptr<Scroll> hscroll;
			std::auto_ptr<Scroll> vscroll;

			// If framed, set margin	
			const bool isFrame = ((f & Textbox::f_mask) != Textbox::f_none);
			if (isFrame)
			{
				_margin_l = 2;
				_margin_r = 2;
				_margin_t = 2;
				_margin_b = 2;
			}
			const int frame_size = (isFrame ? 2 : 0);
			
			// If framed, and out, and scrolls, the scrolls overlap the edge as it
			// looks much better.  For in, the scrolls are contained within edge.
			const bool isOut = ((f & Textbox::f_mask) == Textbox::f_out);
			
			// Initialize vertical scroll if desired
			const bool isHScroll = ((f & Textbox::s_horizontal) != 0);
			const bool isVScroll = ((f & Textbox::s_vertical) != 0);
			if (isVScroll)
			{
				// Create scroll
				vscroll = std::auto_ptr<Scroll>(new Scroll(o.manager(),
						_right - Scroll::h_size + 1 - (isOut ? 0 : frame_size),
						_top + (isOut ? 0 : frame_size),
						_height - (isHScroll ? Scroll::v_size - 1 : 0) - (isOut ? 0 : (2 * frame_size)),
						Scroll::t_vertical));
				vscroll->callback(callback_v, this);
			}

			// Initialize horizontal scroll if desired
			if (isHScroll)
			{
				// Create scroll
				hscroll = std::auto_ptr<Scroll>(new Scroll(o.manager(),
						_left + (isOut ? 0 : frame_size),
						_bottom - Scroll::v_size + 1 - (isOut ? 0 : frame_size),
						_width - (isVScroll ? Scroll::h_size - 1 : 0) - (isOut ? 0 : (2 * frame_size)),
						Scroll::t_horizontal));
				hscroll->callback(callback_h, this);
			}

			// Save objects
			_hscroll = hscroll;
			_vscroll = vscroll;

			// Set text
			text(t);
		}

		void Impl3D::paint_frame(BITMAP* bmp)
		{
			const int flags = _owner.flags();

			// Draw disired frame
			if ((flags & Textbox::f_mask) != Textbox::f_none)
			{
				if (flags == Textbox::f_out)
				{
					Update u(bmp);
					box(bmp, _left, _top, _right, _bottom, false, -1);
				}
				else
				{
					Update u(bmp);
					box(bmp, _left, _top, _right, _bottom, true, -1);
				}
			}

			// If both vertical and horizontal scrolls, update intersection
			if ((flags & Textbox::s_mask) == Textbox::s_both)
			{
				if ((flags & Textbox::f_mask) == Textbox::f_in)
				{
					rectfill(bmp, _right - Scroll::h_size, _bottom - Scroll::v_size,
							_right - 2, _bottom - 2, Theme.GRAY);
				}
				else
				{
					rectfill(bmp, _right - Scroll::h_size + 2, _bottom - Scroll::v_size + 2,
							_right - 2, _bottom - 2, Theme.GRAY);
				}
			}
		}
		
		void Impl3D::paint_margins(BITMAP* bmp)
		{
			const int color = (_background == -1 ? Theme.GRAY : _background);

			Update u(bmp);
						
			// Update left margin
			if (_margin_l)
			{
				for (int n = _text_left - _margin_l; n < _text_left; ++n)
				{
					vline(bmp, n, _text_top - _margin_t, _text_bottom + _margin_b,
							color);
				}
			}

			// Update top margin
			if (_margin_t)
			{
				for (int n = _text_top - _margin_t; n < _text_top; ++n)
				{
					hline(bmp, _text_left - _margin_l, n, _text_right + _margin_r,
							color);
				}
			}

			// Update right margin
			if (_margin_r)
			{
				for (int n = _text_right + _margin_r; n > _text_right; --n)
				{
					vline(bmp, n, _text_top - _margin_t, _text_bottom + _margin_b,
							color);
				}
			}

			// Update bottom margin
			if (_margin_b)
			{
				for (int n = _text_bottom + _margin_b; n > _text_bottom; --n)
				{
					hline(bmp, _text_left - _margin_l, n, _text_right + _margin_r,
							color);
				}
			}
		}
		
		void Impl3D::paint_text(BITMAP* bmp)
		{
			const int flags = _owner.flags();

			// The font height will determine the number of viewable lines
			const FONT* f = _font ? _font : ::font;

			// Determine line to start drawing
			container::size_type i = 0;
			if ((flags & Textbox::s_vertical) != 0)
				i = _vscroll->setting() / _textHeight;
			
			// Determine collumn to start drawing
			int left_start = 0;
			if ((flags & Textbox::s_horizontal) != 0)
				left_start = _hscroll->setting();
			
			// Determine active background and foreground colors
			const int bcolor = (_background == -1 ? Theme.GRAY : _background);
			const int fcolor = (_foreground == -1 ? Theme.BLACK : _foreground);
			
			// Create line bitmap
			BITMAP* line = create_bitmap(_textWidth * 2, _textHeight);

			// Draw the text			
			{
				// Prevent updates to anywhere else on user bitmap				
				Clipper c(bmp, _text_left, _text_top, _text_right, _text_bottom);
				Update u(bmp);
				
				// Acquire the bitmaps to speed things up
				Acquire a1(line);
				Acquire a2(bmp);
								
				// Display the lines
				const container::size_type count = _lines.size();
				int t = _text_top;
				while (t < _text_bottom)
				{
					// Clear the bitmap
					clear_to_color(line, bcolor);
					
					// Print the line if needed
					if (i < count)
						textout_ex(line, f, _lines[i].c_str(), 0, 0, fcolor, bcolor);

					// Copy line to user bitmap
					blit(line, bmp, left_start, 0, _text_left, t, _textWidth, _textHeight);
					
					// Next line
					t += _textHeight;
					++i;
				}
			}
			
			// Let go of line bitmap
			destroy_bitmap(line);
		}

		void Impl3D::recalc_boundary()
		{
			const int flags = _owner.flags();
			
			// Initialize text boundary
			_text_left = _left + _margin_l;
			_text_top = _top + _margin_t;
			_text_right = _right - _margin_r;
			_text_bottom = _bottom - _margin_b;

			// Update for frames
			if ((flags & Textbox::f_mask) != Textbox::f_none)
			{
				_text_left += 2;
				_text_top += 2;
				_text_right -= 2;
				_text_bottom -= 2;
			}
			
			// Update text boundary for horizontal scroll
			if ((flags & Textbox::s_horizontal) != 0)
				_text_bottom -= Scroll::v_size;
			
			// Update text boundary for vertical scroll
			if ((flags & Textbox::s_vertical) != 0)
				_text_right -= Scroll::h_size;
		}
	}
}

//=============================================================================
// class Textbox implementation
//=============================================================================

Textbox::Textbox(_Manager& m, int x, int y, int w, int h, int f, const char* t)
		:
		_ControlEx(m, f | m_down),
		_impl(0)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new TextboxImpl::Impl3D(*this, x, y, w, h, f, t);
	else
		_impl = new TextboxImpl::Impl(*this, x, y, w, h, f, t);
}

Textbox::~Textbox()
{
	delete _impl;
}

#ifdef	_DEBUG
void Textbox::dump() const
{
	TRACE("gui::Textbox @ $%08X\n", this);
	TRACE(" Flags:");
	const int _flags = flags();
	switch (_flags & h_mask)
	{
	case h_center:
		TRACE(" h_center");
		break;
	
	case h_left:
		TRACE(" h_left");
		break;
	
	case h_right:
		TRACE(" h_right");
		break;
	}
	switch (_flags & v_mask)
	{
	case v_center:
		TRACE(" | v_center");
		break;
	
	case v_top:
		TRACE(" | v_top");
		break;
	
	case v_bottom:
		TRACE(" | v_bottom");
		break;
	}
	switch (_flags & f_mask)
	{
	case f_none:
		TRACE(" | f_none");
		break;
	
	case f_out:
		TRACE(" | f_out");
		break;
	
	case f_in:
		TRACE(" | f_in");
		break;
	}
	switch (_flags & s_mask)
	{
	case s_none:
		TRACE("\n");
		break;
		
	case s_horizontal:
		TRACE(" | s_horizontal\n");
		break;
		
	case s_vertical:
		TRACE(" | s_vertical\n");
		break;
		
	case s_both:
		TRACE(" | s_both\n");
		break;
	}
	_impl->dump();
}
#endif

const char* Textbox::operator[](int i) const
{
	return (*_impl)[i];
}

int Textbox::background() const
{
	return _impl->background();
}

void Textbox::background(int c)
{
	_impl->background(c);
}

int Textbox::bottom() const
{
	return _impl->bottom();
}

int Textbox::count() const
{
	return _impl->count();
}

FONT* Textbox::font() const
{
	return _impl->font();
}

void Textbox::font(FONT* f)
{
	_impl->font(f);
}

int Textbox::foreground() const
{
	return _impl->foreground();
}

void Textbox::foreground(int c)
{
	_impl->foreground(c);
}

int Textbox::height() const
{
	return _impl->height();
}

int Textbox::left() const
{
	return _impl->left();
}

std::pair<int, int> Textbox::margin_h() const
{
	return _impl->margin_h();
}

void Textbox::margin_h(int l, int r)
{
	_impl->margin_h(l, r);
}
		
std::pair<int, int> Textbox::margin_v() const
{
	return _impl->margin_v();
}

void Textbox::margin_v(int t, int b)
{
	_impl->margin_v(t, b);
}

void Textbox::paint(BITMAP* bmp)
{
	const int _dirty = dirty();
	const int _flags = flags();
	
	// Update frame if needed
	if ((_dirty & TextboxImpl::Base::dirty_frame) != 0)
		_impl->paint_frame(bmp);
	
	// Update margins if needed
	if ((_dirty & TextboxImpl::Base::dirty_margins) != 0)
		_impl->paint_margins(bmp);
	
	// Update text if needed
	if ((_dirty & TextboxImpl::Base::dirty_text) != 0)
		_impl->paint_text(bmp);

	// Update horizontal scroll if needed
	if (((_flags & s_horizontal) != 0)
			&& (_dirty & TextboxImpl::Base::dirty_hscroll) != 0)
	{
		_impl->hscroll().paint(bmp);
	}
	
	// Update vertical scroll if needed
	if (((_flags & s_vertical) != 0)
			&& (_dirty & TextboxImpl::Base::dirty_vscroll) != 0)
	{
		_impl->vscroll().paint(bmp);
	}
}

bool Textbox::mouse_down(BITMAP* b, int x, int y, int m)
{
	bool handled = false;
	const int _flags = flags();
	if ((_flags & s_horizontal) != 0)
		handled = _impl->hscroll().mouse_down(b, x, y, m);
	if (!handled && (_flags & s_vertical) != 0)
		handled = _impl->vscroll().mouse_down(b, x, y, m);
	return handled;
}

int Textbox::right() const
{
	return _impl->right();
}

void Textbox::text(const char* t)
{
	_impl->text(t);
}

int Textbox::top() const
{
	return _impl->top();
}

int Textbox::width() const
{
	return _impl->width();
}
