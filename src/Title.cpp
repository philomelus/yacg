#include "precompiled.hpp"
#include "yacg/Title.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

namespace yacg
{
	namespace TitleImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		class Base
		{
		public:
			Base(Title& o, int x, int y, int w, int h, int f, const char* t);
			virtual ~Base();
			
			int bottom() const;
			void bottom(int b);

			FONT* font() const;
			void font(FONT *pFont);

			int height() const;
			void height(int h);

			int left() const;
			void left(int l);

			int right() const;
			void right(int r);

			const char* title() const;
			void title(const char* t);

			const std::string& title_object() const;
			
			int top() const;
			void top(int t);
			
			int width() const;
			void width(int w);

		public:
			virtual void paint(BITMAP* bmp) = 0;
					
#ifdef	_DEBUG
			void dump() const;
#endif
			
		private:
			void calc_height();
			void calc_width();
			
		protected:
			Title& _owner;
			
			bool _autoHeight;
			bool _autoWidth;

			int _left;
			int _right;

			int _top;
			int _bottom;

			int _height;
			int _width;

			FONT* _font;

			std::string _title;
		};
		

//=============================================================================
// class Impl declaration
//=============================================================================

		class Impl : public Base
		{
		public:
			Impl(Title& o, int x, int y, int w, int h, int f, const char* t);

			void paint(BITMAP* bmp);
		};
		
//=============================================================================
// class Impl3D declaration
//=============================================================================

		class Impl3D : public Base
		{
		public:
			Impl3D(Title& o, int x, int y, int w, int h, int f, const char* t);

			void paint(BITMAP* bmp);
		};
		
//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(Title& o, int x, int y, int w, int h, int f, const char* t)
				:
				_owner(o),
				_autoHeight(h == Title::calc ? true : false),
				_autoWidth(w == Title::calc ? true : false),
				_bottom(0),
				_height(h),
				_font(0),
				_left(x),
				_right(0),
				_top(y),
				_width(w)
		{
			// Save title if given
			if (t)
				_title = t;

			// Initialize width
			if (_autoWidth)
			{
				calc_width();
				if ((f & Title::o_no_resize_h) == Title::o_no_resize_h)
					_autoWidth = false;
			}
			else
				_right = x + w - 1;
			
			// Save height
			if (_autoHeight)
			{
				calc_height();
				if ((f & Title::o_no_resize_v) == Title::o_no_resize_v)
					_autoHeight = false;
			}
			else
				_bottom = y + h - 1;
		}

		Base::~Base()
		{
		}
		
		int Base::bottom() const
		{
			return _bottom;
		}
		
		void Base::bottom(int b)
		{
			if (_bottom != b)
			{
				_bottom = b;
				_height = _bottom - _top + 1;
				_autoHeight = false;
				_owner.dirty(Title::all);
			}
		}
		
		void Base::calc_height()
		{
			_height = text_height(_font ? _font : ::font) + 6;
			_bottom = _top + _height - 1;
		}

		void Base::calc_width()
		{
			if (!_title.empty())
				_width = text_length(_font ? _font : ::font, _title.c_str()) + 6;
			else
				_width = 0;
			_right = _left + _width - 1;
		}

#ifdef	_DEBUG
		void Base::dump() const
		{
			dump_xywh(_left, _top, _width, _height, _right, _bottom);
			TRACE(" Auto Width: %s\n", _autoWidth ? "TRUE" : "FALSE");
			TRACE(" Auto Height: %s\n", _autoHeight ? "TRUE" : "FALSE");
			TRACE(" Title: \"%s\"\n", _title.c_str());
			TRACE(" Font: ");
			if (_font)
				TRACE("$%08X\n", _font);
			else
				TRACE("Allegro Global Font\n");
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
				_owner.dirty(Title::all);
				if (_autoHeight)
					calc_height();
				if (_autoWidth)
					calc_width();
			}
		}
	
		int Base::height() const
		{
			return _height;
		}

		void Base::height(int h)
		{
			if (_height != h)
			{
				_height = h;
				_bottom = _top + _height - 1;
				_autoHeight = false;
				_owner.dirty(Title::all);
			}
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
				if (_autoWidth)
					calc_width();
				else
					_right = _left + _width - 1;
				_owner.dirty(Title::all);
			}
		}
		
		int Base::right() const
		{
			return _right;
		}

		void Base::right(int r)
		{
			if (_right != r)
			{
				_right = r;
				_width = _right - _left + 1;
				_autoWidth = false;
				_owner.dirty(Title::all);
			}
		}
		
		const char* Base::title() const
		{
			return _title.c_str();
		}

		void Base::title(const char* t)
		{
			_title = t;
			_owner.dirty(Title::all);
			if (_autoWidth)
				calc_width();
			if (_autoHeight)
				calc_height();
		}

		const std::string& Base::title_object() const
		{
			return _title;
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
				if (_autoHeight)
					calc_height();
				else
					_bottom = _top + _height - 1;
				_owner.dirty(Title::all);
			}
		}
		
		int Base::width() const
		{
			return _width;
		}

		void Base::width(int w)
		{
			if (_width != w)
			{
				_width = w;
				_right = _left + _width - 1;
				_autoWidth = false;
				_owner.dirty(Title::all);
			}
		}
		
//=============================================================================
// class Impl implementation
//=============================================================================

		Impl::Impl(Title& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
		}

		void Impl::paint(BITMAP* b)
		{
			Update u(b);
			
			// Determine font to use
			FONT* f = _font == 0 ? ::font : _font;
			
			// Paint the frame
			const int flags = _owner.flags();
			const int frame = (flags & Title::f_mask);
			int margin;
			switch (frame)
			{
			case Title::f_none:
				// Erase to brackground
				rectfill(b, _left, _top, _right, _bottom, Theme.flat.Background);
				margin = 0;
				break;
				
			case Title::f_in:
			case Title::f_out:
			case Title::f_flat:
				rect(b, _left, _top, _right, _bottom, Theme.flat.Frame);
				rectfill(b, _left + 1, _top + 1, _right - 1, _bottom - 1,
						Theme.flat.Background);
				margin = 1;
				break;
			}

			// Display text if there is any
			if (!_title.empty())
			{
				// Figure left coordinate
				int x;
				const int horiz = (flags & Title::h_mask);
				switch (horiz)
				{
				case Title::h_center:
					x = _left + ((_width - text_length(f, _title.c_str())) / 2);
					break;

				case Title::h_left:
					x = _left + margin + 2;
					break;

				case Title::h_right:
					x = _right - margin - 2 - text_length(f, _title.c_str());
					break;
				}

				// Figure top coordinate
				int y;
				const int vert = (flags & Title::v_mask);
				switch (vert)
				{
				case Title::v_center:
					y = _top + ((_height - text_height(f)) / 2);
					break;
				
				case Title::v_top:
					y = _top + margin + 2;
					break;
					
				case Title::v_bottom:
					y = _bottom - margin - 2 - text_height(f);
					break;
				}

				// Display the text
				text_mode(-1);
				textout_ex(b, f, _title.c_str(), x, y, Theme.flat.Foreground,
						Theme.flat.Background);
			}
		}

//=============================================================================
// class Impl3D implementation
//=============================================================================

		Impl3D::Impl3D(Title& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
		}

		void Impl3D::paint(BITMAP* b)
		{
			Update u(b);
			
			// Determine font to use
			FONT* f = _font == 0 ? ::font : _font;
			
			// Paint the frame
			const int flags = _owner.flags();
			const int frame = (flags & Title::f_mask);
			int margin;
			switch (frame)
			{
			case Title::f_none:
				// Erase to background
				rectfill(b, _left, _top, _right, _bottom, Theme.GRAY);
				margin = 0;
				break;
				
			case Title::f_in:
				box(b, _left, _top, _right, _bottom, true, Theme.GRAY);
				margin = 2;
				break;
				
			case Title::f_out:
				box(b, _left, _top, _right, _bottom, false, Theme.GRAY);
				margin = 2;
				break;
			
			case Title::f_flat:
				rect(b, _left, _top, _right - 1, _bottom - 1, Theme.GRAY_DARK);
				hline(b, _left + 1, _top + 1, _right - 2, Theme.WHITE);
				vline(b, _left + 1, _top + 2, _bottom - 2, Theme.WHITE);
				hline(b, _left, _bottom, _right, Theme.WHITE);
				vline(b, _right, _top, _bottom - 1, Theme.WHITE);
				rectfill(b, _left + 2, _top + 2, _right - 2, _bottom - 2, Theme.GRAY);
				margin = 1;
				break;
			}

			// Display text if there is any
			if (!_title.empty())
			{
				// Figure left coordinate
				int x;
				const int horiz = (flags & Title::h_mask);
				switch (horiz)
				{
				case Title::h_center:
					x = _left + ((_width - text_length(f, _title.c_str())) / 2);
					break;

				case Title::h_left:
					x = _left + margin + 2;
					break;

				case Title::h_right:
					x = _right - margin - 2 - text_length(f, _title.c_str());
					break;
				}

				// Figure top coordinate
				int y;
				const int vert = (flags & Title::v_mask);
				switch (vert)
				{
				case Title::v_center:
					y = _top + ((_height - text_height(f)) / 2);
					break;
				
				case Title::v_top:
					y = _top + margin + 2;
					break;
					
				case Title::v_bottom:
					y = _bottom - margin - 2 - text_height(f);
					break;
				}

				// Display the text
				text_mode(-1);
				textout_ex(b, f, _title.c_str(), x, y, Theme.BLACK, Theme.GRAY);
			}
		}
	}
}

//=============================================================================
// class Title implementation
//=============================================================================

Title::Title(_Manager& m, int x, int y, int w, int h, int f, const char* t)
		:
		_Control(m, f)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new TitleImpl::Impl3D(*this, x, y, w, h, f, t);
	else
		_impl = new TitleImpl::Impl(*this, x, y, w, h, f, t);
}

Title::~Title()
{
	delete _impl;
}

int Title::bottom() const
{
	return _impl->bottom();
}

void Title::bottom(int b)
{
	_impl->bottom(b);
}

#ifdef	_DEBUG
void Title::dump() const
{
	TRACE("gui::Title @ $%08X\n", this);
	TRACE(" Flags:");
	const int flags = _Control::flags();
	switch (flags & h_mask)
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
	switch (flags & v_mask)
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
	switch (flags & f_mask)
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
	switch (flags & o_mask)
	{
	case o_resize_h:
		TRACE(" | o_resize_h | o_resize_v\n");
		break;
		
	case o_no_resize_h:
		TRACE(" | o_no_resize_h | o_resize_v\n");
		break;
		
	case o_no_resize_v:
		TRACE(" | o_resize_h | o_no_resize_v\n");
		break;
		
	case o_no_resize_h | o_no_resize_v:
		TRACE(" | o_no_resize_h | o_no_resize_v\n");
		break;
	}
	_impl->dump();
}
#endif

FONT* Title::font() const
{
	return _impl->font();
}

int Title::height() const
{
	return _impl->height();
}

void Title::height(int h)
{
	_impl->height(h);
}

int Title::left() const
{
	return _impl->left();
}

void Title::left(int l)
{
	_impl->left(l);
}

void Title::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

int Title::right() const
{
	return _impl->right();
}

void Title::right(int r)
{
	_impl->right(r);
}

const char* Title::title() const
{
	return _impl->title();
}

void Title::title(const char* t)
{
	_impl->title(t);
}

const std::string& Title::title_object() const
{
	return _impl->title_object();
}

int Title::top() const
{
	return _impl->top();
}

void Title::top(int t)
{
	_impl->top(t);
}

int Title::width() const
{
	return _impl->width();
}

void Title::width(int w)
{
	_impl->width(w);
}
