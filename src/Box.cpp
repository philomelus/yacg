#include "precompiled.hpp"
#include <yacg/Box.hpp>
#include <yacg/BoxTheme.hpp>
#include "ThemeImpl.hpp"
#include "utility.hpp"

// BUGBUG: Doesn't respond to style changes after construction!

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace yacg
{
	namespace BoxImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		class Base
		{
		public:
			static const int dirty_frame	= 0x00000001;
			static const int dirty_interior	= 0x00000002;
			static const int dirty_title	= 0x00000004;
			static const int dirty_all		= dirty_frame | dirty_interior | dirty_title;
		
		public:
			Base(Box& o, int x, int y, int w, int h, int f, const char* t);
			virtual ~Base();
			
			int bottom() const;
			
#ifdef	_DEBUG
			void dump() const;
#endif
			
			FONT* font() const;
			void font(FONT* f);
			
			int height() const;
			void height(int h);
			
			int left() const;
			void left(int l);
			
			virtual void paint(BITMAP* bmp) = 0;
			
			int right() const;
			
			virtual void theme(_BoxTheme& t);
			
			void theme_changed();

			const char* title() const;
			void title(const char* t);

			const std::string& title_string() const;
			
			int top() const;
			void top(int t);
			
			virtual void update_from_theme(Theme& t);

			int width() const;
			void width(int w);
		
		protected:
			Box& _owner;
			
			int _left;
			int _right;
			
			int _top;
			int _bottom;
			
			int _width;
			int _height;
	
			std::string _title;

			FONT* _font;
		};
		
//=============================================================================
// class Impl3D declaration
//=============================================================================
		
		class Impl3D : public Base
		{
		public:
			Impl3D(Box& o, int x, int y, int w, int h, int f, const char* t);

#ifdef	_DEBUG
			void dump() const;
#endif

			void paint(BITMAP* bmp);

			void theme(_BoxTheme& t);
						
			void update_from_theme(Theme& t);
		
		private:
			int _frameLightest;
			int _frameLight;
			int _frameDark;
			int _frameDarkest;

			int _background;
			int _foreground;

			int _inactiveBackground;
			int _inactiveForeground;

			int _interior;
		};
		
//=============================================================================
// class ImplFlat declaration
//=============================================================================

		class ImplFlat : public Base
		{
		public:
			ImplFlat(Box& o, int x, int y, int w, int h, int f, const char* t);
			
#ifdef	_DEBUG
			void dump() const;
#endif

			void paint(BITMAP* bmp);

			void theme(_BoxTheme& t);
			
			void update_from_theme(Theme& t);
			
		private:
			int _frame;

			int _background;
			int _foreground;

			int _inactiveBackground;
			int _inactiveForeground;

			int _interior;
		};
		
//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(Box& o, int x, int y, int w, int h, int f, const char* t)
				:
				_owner(o),
				_left(x),
				_right(x + w - 1),
				_top(y),
				_bottom(y + h - 1),
				_width(w),
				_height(h),
				_font(0)
		{
			if (t)
				_title = t;
		}
		
		Base::~Base()
		{
		}
		
		int Base::bottom() const
		{
			return _bottom;
		}

#ifdef	_DEBUG
		void Base::dump() const
		{
			dump_xywh(_left, _top, _width, _top, _right, _bottom);
			if ((_owner.flags() & Box::t_mask) == Box::t_title)
			{
				TRACE(" Title: %s\n", _title.c_str());
				TRACE(" Font: $%08X%s\n", _font, (_font == ::font ?
						" (Allegro Global Font)" : ""));
			}
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
				_owner.dirty(dirty_title);
			}
		}
		
		int Base::height() const
		{
			return _height;
		}
		
		void Base::height(int h)
		{
			_height = h;
			_bottom = _top + _height - 1;
		}

		int Base::left() const
		{
			return _left;
		}
		
		void Base::left(int l)
		{
			_left = l;
			_right = _left + _width - 1;
		}
		
		int Base::right() const
		{
			return _right;
		}
		
		void Base::theme(_BoxTheme& t)
		{
			_font = t.font();
		}
		
		void Base::theme_changed()
		{
			update_from_theme(_owner.manager().theme());
			_owner.dirty(dirty_all);
		}
		
		const char* Base::title() const
		{
			return _title.c_str();
		}
		
		void Base::title(const char* t)
		{
			_title = t;
			_owner.dirty(dirty_title);
		}
		
		const std::string& Base::title_string() const
		{
			return _title;
		}
		
		int Base::top() const
		{
			return _top;
		}
		
		void Base::top(int t)
		{
			_top = t;
			_bottom = _top + _height - 1;
		}
		
		void Base::update_from_theme(Theme& t)
		{
			// Does theme contain a BoxThemeFlat?
			Theme::iterator i = t.find(Theme::TYPE_BOX);
			if (i != t.end())
			{
				// Yup, so initialize colors from type
				_BoxTheme& ti = static_cast<_BoxTheme&>(*i);
				_font = ti.font();
			}
			else
			{
				// Locate common type
				i = t.find(Theme::TYPE_DEFAULT);
				ASSERT(i != t.end());
				_DefaultTheme& ti = static_cast<_DefaultTheme&>(*i);
				_font = ti.font();
			}
		}
		
		int Base::width() const
		{
			return _width;
		}
		
		void Base::width(int w)
		{
			_width = w;
			_right = _left + _width - 1;
		}
		
//=============================================================================
// class Impl3D implementation
//=============================================================================

		Impl3D::Impl3D(Box& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			update_from_theme(o.manager().theme());
		}
		
#ifdef	_DEBUG
		void Impl3D::dump() const
		{
			// Call base first
			Base::dump();
			
			// Dump our color theme
			TRACE(" Frame Lightest Color: %d %d %d\n", getr(_frameLightest),
					getg(_frameLightest), getb(_frameLightest));
			TRACE(" Frame Light Color: %d %d %d\n", getr(_frameLight),
					getg(_frameLight), getb(_frameLight));
			TRACE(" Frame Dark Color: %d %d %d\n", getr(_frameDark), getg(_frameDark),
					getb(_frameDark));
			TRACE(" Frame Darkest Color: %d %d %d\n", getr(_frameDarkest),
					getg(_frameDarkest), getb(_frameDarkest));
			TRACE(" Text Foreground Color: %d %d %d\n", getr(_foreground),
					getg(_foreground), getb(_foreground));
			TRACE(" Text Background Color: %d %d %d\n", getr(_background),
					getg(_background), getb(_background));
			TRACE(" Text Foreground Color (Inactive): %d %d %d\n", getr(_inactiveForeground),
					getg(_inactiveForeground), getb(_inactiveForeground));
			TRACE(" Text Foreground Color (Inactive): %d %d %d\n", getr(_inactiveBackground),
					getg(_inactiveBackground), getb(_inactiveBackground));
			TRACE(" Interior Color: %d %d %d\n", getr(_interior), getg(_interior),
					getb(_interior));
		}
#endif

		void Impl3D::paint(BITMAP* bmp)
		{
			const int dirty = _owner.dirty();
			const bool isFrameDirty = ((dirty & dirty_frame) != 0);
			const bool isInteriorDirty = ((dirty & dirty_interior) != 0);
			const bool isTitleDirty = ((dirty & dirty_title) != 0);
			
			const int flags = _owner.flags();
			
			const int font_height = text_height(_font);

			const bool isTitle = ((flags & Box::t_mask) == Box::t_title);
			const int vertical = (flags & Box::v_mask);
			const int options = (flags & Box::o_mask);
			
			// Determine correct top and bottom lines
			int t = _top;
			int b = _bottom;
			if (isTitle)
			{
				const bool isTop = (vertical == Box::v_top);
				const bool isBottom = (vertical == Box::v_bottom);
				
				const bool isAbove = (options == Box::o_titleabove);
				const bool isCentered = (options == Box::o_titlecentered);
				const bool isBelow = (options == Box::o_titlebelow);
				
				// Adjust for top or bottom title plus options
				if (isTop)
				{
					if (isAbove)
						t += (font_height);
					else if (isCentered)
						t += ((font_height - 2) / 2);
				}
				else if (isBottom)
				{
					if (isCentered)
						b -= (font_height / 2);
					else if (isBelow)
						b -= (font_height + 1);
				}
			}

			// Paint frame
			const int frame = (flags & Box::f_mask);
			const bool isIn = (frame == Box::f_in);
			const bool isFlat = (frame == Box::f_flat);
			const bool isFilled = ((flags & Box::i_mask) == Box::i_filled);
			if (isFlat)
			{
				if (isFrameDirty)
				{
					rect(bmp, _left, t, _right - 1, b - 1, _frameDark);
					hline(bmp, _left + 1, t + 1, _right - 2, _frameLightest);
					vline(bmp, _left + 1, t + 2, b - 2, _frameLightest);
					hline(bmp, _left, b, _right, _frameLightest);
					vline(bmp, _right, t, b - 1, _frameLightest);
				}
				if (isFilled && isInteriorDirty)
					rectfill(bmp, _left + 2, t + 2, _right - 2, b - 2, _interior);
			}
			else
			{
				if (isFrameDirty)
				{
					int topLeft;
					int bottomRight;
					int insetTopLeft;
					int insetBottomRight;
					if (isIn)
					{
						topLeft = _frameDarkest;
						bottomRight = _frameLightest;
						insetTopLeft = _frameDark;
						insetBottomRight = _frameLight;
					}
					else
					{
						topLeft = _frameLight;
						bottomRight = _frameDarkest;
						insetTopLeft = _frameLightest;
						insetBottomRight = _frameDark;
					}
					
					// Top/Left
					hline(bmp, _left, t, _right - 1, topLeft);
					vline(bmp, _left, t + 1, b - 1, topLeft);
					
					// Inset top/left
					hline(bmp, _left + 1, t + 1, _right - 2, insetTopLeft);
					vline(bmp, _left + 1, t + 2, b - 2, insetTopLeft);
					
					// Bottom/Right
					hline(bmp, _left, b, _right, bottomRight);
					vline(bmp, _right, t, b - 1, bottomRight);

					// Inset bottom/right
					hline(bmp, _left + 1, b - 1, _right - 1, insetBottomRight);
					vline(bmp, _right - 1, t + 1, b - 2, insetBottomRight);
				}
				
				// Fill interior if desired
				if (isFilled && (dirty & dirty_interior) != 0)
					rectfill(bmp, _left + 2, t + 2, _right - 2, b - 2, _interior);
			}
			
			// Title if desired
			if (isTitle && isTitleDirty)
			{
				// Determine left of title
				int x;
				switch (flags & Box::h_mask)
				{
				case Box::h_left:
					x = _left + 5;
					break;
					
				case Box::h_right:
					x = _right - 5 - text_length(_font, _title.c_str());
					break;
					
				case Box::h_centered:
					x = _left + ((_width - text_length(_font, _title.c_str())) / 2);
					break;
				}
				
				// Determine top of title
				int y;
				switch (vertical)
				{
				case Box::v_top:
					y = _top;
					break;
					
				case Box::v_bottom:
					y = _bottom - font_height;
					break;
					
				case Box::v_centered:
					y = _top + ((_height - font_height) / 2);
					break;
				}
				
				// Adjust top for option's
				switch (options)
				{
				case Box::o_titlecentered:
					break;
					
				case Box::o_titleabove:
					if (vertical == Box::v_bottom)
						y -= 1;
					break;
				
				case Box::o_titlebelow:
					if (vertical == Box::v_top)
						y += 3;
					else if (vertical == Box::v_bottom)
						y += 1;
					break;
				}

				// Display title
				const bool active = _owner.active();
				const int fore = active ? _foreground : _inactiveForeground;
				const int back = active ? _background : _inactiveBackground;
				textout_ex(bmp, _font, _title.c_str(), x, y, fore, back);
			}
		}

		void Impl3D::theme(_BoxTheme& t)
		{
			Base::theme(t);
			BoxTheme3D& ti = static_cast<BoxTheme3D&>(t);
			_frameLightest = ti.frame_lightest();
			_frameLight = ti.frame_light();
			_frameDark = ti.frame_dark();
			_frameDarkest = ti.frame_darkest();
			_background = ti.interior();
			_foreground = ti.text_foreground();
			_inactiveBackground = ti.interior();
			_inactiveForeground = ti.inactive_text_background();
			_interior = ti.interior();
		}
		
		void Impl3D::update_from_theme(Theme& t)
		{
			// NOTE:  Assumes STYLE_3D is still active style for t.
			
			// Let base do common stuff
			Base::update_from_theme(t);
			
			// Does theme contain a BoxThemeFlat?
			Theme::iterator i = t.find(Theme::TYPE_BOX);
			if (i != t.end())
			{
				// Yup, so initialize colors from type
				BoxTheme3D& ti = static_cast<BoxTheme3D&>(*i);
				_frameLightest = ti.frame_lightest();
				_frameLight = ti.frame_light();
				_frameDark = ti.frame_dark();
				_frameDarkest = ti.frame_darkest();
				_background = ti.interior();
				_foreground = ti.text_foreground();
				_inactiveBackground = ti.interior();
				_inactiveForeground = ti.inactive_text_background();
				_interior = ti.interior();
			}
			else
			{
				// Locate flat type
				i = t.find(Theme::TYPE_DEFAULT);
				DefaultTheme3D& ti = static_cast<DefaultTheme3D&>(*i);
				ASSERT(i != t.end());
				_frameLightest = ti.frame_lightest();
				_frameLight = ti.frame_light();
				_frameDark = ti.frame_dark();
				_frameDarkest = ti.frame_darkest();
				_background = ti.interior();
				_foreground = ti.text_foreground();
				_inactiveBackground = ti.interior();
				_inactiveForeground = ti.inactive_text_foreground();
				_interior = ti.interior();
			}
		}

//=============================================================================
// class ImplFlat implementation
//=============================================================================

		ImplFlat::ImplFlat(Box& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			update_from_theme(o.manager().theme());
		}

#ifdef	_DEBUG
		void ImplFlat::dump() const
		{
			// Call base first
			Base::dump();
			
			// Dump our color theme
			TRACE(" Frame Color: %d %d %d\n", getr(_frame), getg(_frame), getb(_frame));
			TRACE(" Text Foreground Color: %d %d %d\n", getr(_foreground),
					getg(_foreground), getb(_foreground));
			TRACE(" Text Background Color: %d %d %d\n", getr(_background),
					getg(_background), getb(_background));
			TRACE(" Text Foreground Color (Inactive): %d %d %d\n", getr(_inactiveForeground),
					getg(_inactiveForeground), getb(_inactiveForeground));
			TRACE(" Text Foreground Color (Inactive): %d %d %d\n", getr(_inactiveBackground),
					getg(_inactiveBackground), getb(_inactiveBackground));
			TRACE(" Interior Color: %d %d %d\n", getr(_interior), getg(_interior),
					getb(_interior));
		}
#endif

		void ImplFlat::paint(BITMAP* bmp)
		{
			const int flags = _owner.flags();
			
			const int font_height = text_height(_font);
			
			const bool isTitle = ((flags & Box::t_mask) == Box::t_title);
			const int vertical = (flags & Box::v_mask);
			const int options = (flags & Box::o_mask);
			
			// Frame
			if (isTitle)
			{
				const bool isTop = (vertical == Box::v_top);
				const bool isBottom = (vertical == Box::v_bottom);
				
				const bool isAbove = (options == Box::o_titleabove);
				const bool isCentered = (options == Box::o_titlecentered);
				const bool isBelow = (options == Box::o_titlebelow);
				
				int t = _top;
				int b = _bottom;
				
				// Adjust for top or bottom title plus options
				if (isTop)
				{
					if (isAbove)
						t += font_height + 1;
					else if (isCentered)
						t += (font_height / 2);
				}
				else if (isBottom)
				{
					if (isCentered)
						b -= (font_height / 2);
					else if (isBelow)
						b -= (font_height + 1);
				}
				
				// Frame it
				rect(bmp, _left, t, _right, b, _frame);
			}
			else
				rect(bmp, _left, _top, _right, _bottom, _frame);
			
			// Interior if desired
			if ((flags & Box::i_mask) == Box::i_filled)
			{
				rectfill(bmp, _left + 1, _top + 1, _right - 1, _bottom - 1,
						_interior);
			}
			
			// Title if desired
			if (isTitle)
			{
				// Determine left of title
				int x;
				switch (flags & Box::h_mask)
				{
				case Box::h_left:
					x = _left + 5;
					break;
					
				case Box::h_right:
					x = _right - 5 - text_length(_font, _title.c_str());
					break;
					
				case Box::h_centered:
					x = _left + ((_width - text_length(_font, _title.c_str())) / 2);
					break;
				}
				
				// Determine top of title
				int y;
				switch (vertical)
				{
				case Box::v_top:
					y = _top;
					break;
					
				case Box::v_bottom:
					y = _bottom - font_height;
					break;
					
				case Box::v_centered:
					y = _top + ((_height - font_height) / 2);
					break;
				}
				
				// Adjust top for option's
				switch (options)
				{
				case Box::o_titlecentered:
					// Nothing to do...
					break;
					
				case Box::o_titleabove:
					if (vertical == Box::v_centered)
						y -= (font_height / 2);
					break;
				
				case Box::o_titlebelow:
					if (vertical == Box::v_centered)
						y += (font_height / 2);
					else
						y += 2;
					break;
				}

				// Display title
				const bool active = _owner.active();
				const int fore = active ? _foreground : _inactiveForeground;
				const int back = active ? _background : _inactiveBackground;
				textout_ex(bmp, _font, _title.c_str(), x, y, fore, back);
			}
		}

		void ImplFlat::theme(_BoxTheme& t)
		{
			Base::theme(t);
			BoxThemeFlat& ti = static_cast<BoxThemeFlat&>(t);
			_frame = ti.frame();
			_background = ti.interior();
			_foreground = ti.text_foreground();
			_inactiveBackground = ti.interior();
			_inactiveForeground = ti.inactive_text_background();
			_interior = ti.interior();
		}
		
		void ImplFlat::update_from_theme(Theme& t)
		{
			// NOTE:  Assumes STYLE_FLAT is still active style for t.
			
			// Let base do common stuff
			Base::update_from_theme(t);
			
			// Does theme contain a BoxThemeFlat?
			Theme::iterator i = t.find(Theme::TYPE_BOX);
			if (i != t.end())
			{
				// Yup, so initialize colors from type
				BoxThemeFlat& ti = static_cast<BoxThemeFlat&>(*i);
				_frame = ti.frame();
				_background = ti.interior();
				_foreground = ti.text_foreground();
				_inactiveBackground = ti.interior();
				_inactiveForeground = ti.inactive_text_background();
				_interior = ti.interior();
			}
			else
			{
				// Locate flat type
				i = t.find(Theme::TYPE_DEFAULT);
				DefaultThemeFlat& ti = static_cast<DefaultThemeFlat&>(*i);
				ASSERT(i != t.end());
				_frame = ti.frame();
				_background = ti.interior();
				_foreground = ti.text_foreground();
				_inactiveBackground = ti.interior();
				_inactiveForeground = ti.inactive_text_foreground();
				_interior = ti.interior();
			}
		}
	}
}

//=============================================================================
// class _BoxTheme implementation
//=============================================================================

//-----------------------------------------------------------------------------

_BoxTheme::_BoxTheme(Theme& t)
	:
	_ThemeItem(none)
{
	// Box theme?
	Theme::iterator i = t.find(Theme::TYPE_BOX);
	if (i != t.end())
		// Yes, so initialize us from it
		*this = static_cast<_BoxTheme&>(*i);
	else
	{
		// No, so get the default theme
		_DefaultTheme& o = t.at<_DefaultTheme>(Theme::TYPE_DEFAULT);
		_font = o.font();
	}
}

_BoxTheme::_BoxTheme(const _BoxTheme& o)
		:
		_ThemeItem(o),
		_font(o._font)
{
}

_BoxTheme& _BoxTheme::operator=(const _BoxTheme& o)
{
	if (this != &o)
	{
		_ThemeItem::operator=(o);
		_font = o._font;
	}
	return *this;
}

#ifdef	_DEBUG
void _BoxTheme::dump() const
{
	_ThemeItem::dump();

	dump_font(_font);
}
#endif

FONT* _BoxTheme::font() const
{
	return _font;
}

void _BoxTheme::font(FONT* f)
{
	_font = f;
}

//=============================================================================
// class Box implementation
//=============================================================================

Box::Box(_Manager& m, int x, int y, int w, int h, int f, const char* t)
		:
		_Control(m, f)
{
	// Only the title gets repainted on active state change
	_activeChangedHint = BoxImpl::Base::dirty_title;
	
	// Create correctly styled Box implementation
	switch (theme().style())
	{
	case Theme::STYLE_3D:
		_impl = new BoxImpl::Impl3D(*this, x, y, w, h, f, t);
		break;
		
	case Theme::STYLE_FLAT:
		_impl = new BoxImpl::ImplFlat(*this, x, y, w, h, f, t);
		break;
		
#ifdef	_DEBUG
	default:
		TRACE("yacg::Box:  Style %d not implemented\n", theme().style());
		break;
#endif
	}
}

Box::~Box()
{
	delete _impl;
}

int Box::bottom() const
{
	return _impl->bottom();
}

#ifdef	_DEBUG
void Box::dump() const
{
	TRACE("gui::Box @ $%08X\n", this);
	TRACE(" Flags:");
	const int flags = _Control::flags();
	switch (flags & f_mask)
	{
	case f_flat:
		TRACE(" f_flat");
		break;
	
	case f_in:
		TRACE(" f_in");
		break;
	
	case f_out:
		TRACE(" f_out");
		break;
	}
	switch (flags & t_mask)
	{
	case t_title:
		TRACE(" | t_title");
		break;
	
	case t_notitle:
		TRACE(" | t_notitle");
		break;
	}
	switch (flags & v_mask)
	{
	case v_top:
		TRACE(" | v_top");
		break;
	
	case v_bottom:
		TRACE(" | v_bottom");
		break;

	case v_centered:
		TRACE(" | v_centered");
		break;
	}
	switch (flags & h_mask)
	{
	case h_left:
		TRACE(" | h_left");
		break;
	
	case h_right:
		TRACE(" | h_right");
		break;

	case h_centered:
		TRACE(" | h_centered");
		break;
	}
	switch (flags & o_mask)
	{
	case o_titlecentered:
		TRACE(" | o_titlecentered");
		break;
	
	case o_titleabove:
		TRACE(" | o_titleabove");
		break;

	case o_titlebelow:
		TRACE(" | o_titlebelow");
		break;
	}
	switch (flags & i_mask)
	{
	case i_unfilled:
		TRACE(" | i_unfilled\n");
		break;
	
	case i_filled:
		TRACE(" | i_filled\n");
		break;
	}
	_impl->dump();
}
#endif

FONT* Box::font() const
{
	return _impl->font();
}

void Box::font(FONT* f)
{
	_impl->font(f);
}

int Box::height() const
{
	return _impl->height();
}

void Box::height(int h)
{
	if (h != _impl->height())
	{
		_impl->height(h);
		dirty(BoxImpl::Base::dirty_all);
	}
}

int Box::left() const
{
	return _impl->left();
}

void Box::left(int l)
{
	if (l != _impl->left())
	{
		_impl->left(l);
		dirty(BoxImpl::Base::dirty_all);
	}
}

void Box::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

int Box::right() const
{
	return _impl->right();
}

Theme& Box::theme() const
{
	return _Control::theme();
}

void Box::theme(_BoxTheme& t)
{
	_impl->theme(t);
	_Control::theme_changed();
}

void Box::theme(Theme& t)
{
	_Control::theme(t);
}

void Box::theme_changed()
{
	_impl->theme_changed();
	_Control::theme_changed();
}

const char* Box::title() const
{
	return _impl->title();
}

void Box::title(const char* t)
{
	_impl->title(t);
}

const std::string& Box::title_string() const
{
	return _impl->title_string();
}

int Box::top() const
{
	return _impl->top();
}

void Box::top(int t)
{
	if (t != _impl->top())
	{
		_impl->top(t);
		dirty(BoxImpl::Base::dirty_all);
	}
}

int Box::width() const
{
	return _impl->width();
}

void Box::width(int w)
{
	if (w != _impl->width())
	{
		_impl->width(w);
		dirty(BoxImpl::Base::dirty_all);
	}
}

//=============================================================================
// class BoxTheme3D implementation
//=============================================================================

//-----------------------------------------------------------------------------

#define	IMPL(var, func)			_THEMEFUNC(BoxTheme3D, var, func)

#define	IMPL_PROPS()										\
IMPL(_frameDark, frame_dark);								\
IMPL(_frameDarkest, frame_darkest);							\
IMPL(_frameLight, frame_light);								\
IMPL(_frameLightest, frame_lightest);						\
IMPL(_textBackground, text_background);						\
IMPL(_textForeground, text_foreground);						\
IMPL(_inactiveTextBackground, inactive_text_background);	\
IMPL(_inactiveTextForeground, inactive_text_foreground);	\
IMPL(_interior, interior)

//-----------------------------------------------------------------------------

#define	IMPL_RESET(type, num)							\
void BoxTheme3D::reset##num()							\
{														\
	const FORMAT old = format();						\
	if (old != type##)									\
	{													\
		CONVERT_COLOR(_frameDark, type##);				\
		CONVERT_COLOR(_frameDarkest, type##);			\
		CONVERT_COLOR(_frameLight, type##);				\
		CONVERT_COLOR(_frameLightest, type##);			\
		CONVERT_COLOR(_textBackground, type##);			\
		CONVERT_COLOR(_textForeground, type##);			\
		CONVERT_COLOR(_inactiveTextBackground, type##);	\
		CONVERT_COLOR(_inactiveTextForeground, type##);	\
		CONVERT_COLOR(_interior, type##);				\
		format(type##);									\
		modified();										\
	}													\
}

#define	IMPL_RESETS()			\
IMPL_RESET(none,)				\
IMPL_RESET(eight, 8)			\
IMPL_RESET(fifteen, 15)			\
IMPL_RESET(sixteen, 16)			\
IMPL_RESET(twentyfour, 24)		\
IMPL_RESET(thirtytwo, 32)

//-----------------------------------------------------------------------------

BoxTheme3D::BoxTheme3D(Theme& t)
		:
		_BoxTheme(t)
{
	// Box theme?
	Theme::iterator i = t.find(Theme::TYPE_BOX);
	if (i != t.end())
		// Yes, so initialize us from it
		*this = static_cast<BoxTheme3D&>(*i);
	else
	{
		// No, so get the default theme
		DefaultTheme3D& o = t.at<DefaultTheme3D>(Theme::TYPE_DEFAULT);
		_frameDark = o.frame_dark();
		_frameDarkest = o.frame_darkest();
		_frameLight = o.frame_light();
		_frameLightest = o.frame_lightest();
		_textBackground = o.text_background();
		_textForeground = o.text_foreground();
		_inactiveTextBackground = o.inactive_text_background();
		_inactiveTextForeground = o.inactive_text_foreground();
		_interior = o.interior();
	}
}

BoxTheme3D::BoxTheme3D(const BoxTheme3D& o)
		:
		_BoxTheme(o),
		_frameDark(o._frameDark),
		_frameDarkest(o._frameDarkest),
		_frameLight(o._frameLight),
		_frameLightest(o._frameLightest),
		_inactiveTextBackground(o._inactiveTextBackground),
		_inactiveTextForeground(o._inactiveTextForeground),
		_interior(o._interior),
		_textBackground(o._textBackground),
		_textForeground(o._textForeground)
{
}

BoxTheme3D& BoxTheme3D::operator=(const BoxTheme3D& o)
{
	if (this != &o)
	{
		_BoxTheme::operator=(o);
		_frameDark = o._frameDark;
		_frameDarkest = o._frameDarkest;
		_frameLight = o._frameLight;
		_frameLightest = o._frameLightest;
		_inactiveTextBackground = o._inactiveTextBackground;
		_inactiveTextForeground = o._inactiveTextForeground;
		_interior = o._interior;
		_textBackground = o._textBackground;
		_textForeground = o._textForeground;
	}
	return *this;
}

#ifdef	_DEBUG
void BoxTheme3D::dump() const
{
	TRACE("yacg::BoxTheme3D @ %08X\n", this);

	_BoxTheme::dump();

	dump_color("Frame Lightest Color", _frameLightest);
	dump_color("Frame Light Color", _frameLight);
	dump_color("Frame Dark Color", _frameDark);
	dump_color("Frame Darkest Color", _frameDarkest);
	dump_color("Text Foreground Color", _textForeground);
	dump_color("Text Background Color", _textBackground);
	dump_color("Text Foreground Color (Inactive)", _inactiveTextForeground);
	dump_color("Text Foreground Color (Inactive)", _inactiveTextBackground);
	dump_color("Interior Color", _interior);
}
#endif

IMPL_PROPS();

IMPL_RESETS();

#undef	IMPL
#undef	IMPL_PROPS
#undef	IMPL_RESET
#undef	IMPL_RESETS

//=============================================================================
// class BoxThemeFlat implementation
//=============================================================================

//-----------------------------------------------------------------------------

#define	IMPL(var, func)			_THEMEFUNC(BoxThemeFlat, var, func)

#define	IMPL_PROPS()										\
IMPL(_frame, frame);										\
IMPL(_textBackground, text_background);						\
IMPL(_textForeground, text_foreground);						\
IMPL(_inactiveTextBackground, inactive_text_background);	\
IMPL(_inactiveTextForeground, inactive_text_foreground);	\
IMPL(_interior, interior)

//-----------------------------------------------------------------------------

#define	IMPL_RESET(type, num)							\
void BoxThemeFlat::reset##num()							\
{														\
	const FORMAT old = format();						\
	if (old != type##)									\
	{													\
		CONVERT_COLOR(_frame, type##);					\
		CONVERT_COLOR(_textBackground, type##);			\
		CONVERT_COLOR(_textForeground, type##);			\
		CONVERT_COLOR(_inactiveTextBackground, type##);	\
		CONVERT_COLOR(_inactiveTextForeground, type##);	\
		CONVERT_COLOR(_interior, type##);				\
		format(type##);									\
		modified();										\
	}													\
}

#define	IMPL_RESETS()			\
IMPL_RESET(none,)				\
IMPL_RESET(eight, 8)			\
IMPL_RESET(fifteen, 15)			\
IMPL_RESET(sixteen, 16)			\
IMPL_RESET(twentyfour, 24)		\
IMPL_RESET(thirtytwo, 32)

//-----------------------------------------------------------------------------

BoxThemeFlat::BoxThemeFlat(Theme& t)
		:
		_BoxTheme(t)
{
	// Box theme?
	Theme::iterator i = t.find(Theme::TYPE_BOX);
	if (i != t.end())
		// Yes, so initialize us from it
		*this = static_cast<BoxThemeFlat&>(*i);
	else
	{
		// No, so get the default theme
		DefaultThemeFlat& o = t.at<DefaultThemeFlat>(Theme::TYPE_DEFAULT);
		_frame = o.frame();
	}
}

BoxThemeFlat::BoxThemeFlat(const BoxThemeFlat& o)
		:
		_BoxTheme(o),
		_frame(o._frame),
		_inactiveTextBackground(o._inactiveTextBackground),
		_inactiveTextForeground(o._inactiveTextForeground),
		_interior(o._interior),
		_textBackground(o._textBackground),
		_textForeground(o._textForeground)
{
}

BoxThemeFlat& BoxThemeFlat::operator=(const BoxThemeFlat& o)
{
	if (this != &o)
	{
		_ThemeItem::operator=(o);
		_frame = o._frame;
		_inactiveTextBackground = o._inactiveTextBackground;
		_inactiveTextForeground = o._inactiveTextForeground;
		_interior = o._interior;
		_textBackground = o._textBackground;
		_textForeground = o._textForeground;
	}
	return *this;
}

#ifdef	_DEBUG
void BoxThemeFlat::dump() const
{
	TRACE("yacg::BoxThemeFlat @ %08X\n", this);

	_BoxTheme::dump();

	TRACE(" Frame Color: %d %d %d\n", getr(_frame), getg(_frame),
			getb(_frame));
	dump_color("Text Foreground Color", _textForeground);
	dump_color("Text Background Color", _textBackground);
	dump_color("Text Foreground Color (Inactive)", _inactiveTextForeground);
	dump_color("Text Foreground Color (Inactive)", _inactiveTextBackground);
	dump_color("Interior Color", _interior);
}
#endif

IMPL_PROPS();

IMPL_RESETS();

#undef	IMPL
#undef	IMPL_PROPS
#undef	IMPL_RESET
#undef	IMPL_RESETS
