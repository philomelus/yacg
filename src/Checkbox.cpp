#include "precompiled.hpp"
#include "yacg/Checkbox.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace
{
	inline int states(int s)
	{
		return (s & Checkbox::s_mask);
	}
	
	inline int title_(int t)
	{
		return (t & Checkbox::t_mask);
	}
	
	inline int button(int b)
	{
		return (b & Checkbox::b_mask);
	}
}

namespace yacg
{
	namespace CheckboxImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		class Base
		{
		public:
			static const dirty_checkbox = 0x01;
			static const dirty_title = 0x02;
			
		public:
			Base(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			virtual ~Base();
			
			int bottom() const;

			std::pair<Checkbox::CALLBACK, void*> callback() const;
			void callback(Checkbox::CALLBACK f, void* d);
						
#ifdef	_DEBUG
			void dump() const;
#endif

			int height() const;
			
			int left() const;
			virtual void left(int l) = 0;
			
			bool mouse_down(BITMAP* b, int x, int y, int m);

			int right() const;

			int top() const;
			virtual void top(int t) = 0;
			
			const char* title() const;
			void title(const char* t);
			
			const std::string& title_object() const;
			
			Checkbox::STATE value() const;
			void value(Checkbox::STATE s);
						
			int width() const;

		public:
			virtual void paint(BITMAP* bmp, int hint = Checkbox::all) = 0;

		protected:
			void toggle_state();
			
		protected:
			void paint_check(BITMAP* bmp, int x, int y, int color);
			
		protected:
			Checkbox& _owner;
			
			int _left;
			int _right;
			
			int _top;
			int _bottom;
			
			int _width;
			int _height;
			
			Checkbox::STATE _state;
			
			std::string _title;
			
			int _boxLeft;
			int _boxRight;
			
			int _boxTop;
			int _boxBottom;
			
			Checkbox::CALLBACK _callback;
			void* _data;
		};

//=============================================================================
// class ButtonBase declaration
//=============================================================================

		class ButtonBase : public Base
		{
		public:
			ButtonBase(Checkbox& o, int x, int y, int w, int h, int f, const char* t);

		protected:			
			void toggle_state();
		};
		
//=============================================================================
// class Button declaration
//=============================================================================

		class Button : public ButtonBase
		{
		public:
			Button(Checkbox& o, int x, int y, int w, int h, int f, const char* t);

			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class Button3D declaration
//=============================================================================

		class Button3D : public ButtonBase
		{
		public:
			Button3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class ButtonOutside declaration
//=============================================================================

		class ButtonOutside : public ButtonBase
		{
		public:
			ButtonOutside(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class ButtonOutside3D declaration
//=============================================================================

		class ButtonOutside3D : public ButtonBase
		{
		public:
			ButtonOutside3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class ButtonOutsideLeft declaration
//=============================================================================

		class ButtonOutsideLeft : public ButtonBase
		{
		public:
			ButtonOutsideLeft(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class ButtonOutsideLeft3D declaration
//=============================================================================

		class ButtonOutsideLeft3D : public ButtonBase
		{
		public:
			ButtonOutsideLeft3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class Left declaration
//=============================================================================

		class Left : public Base
		{
		public:
			Left(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class Left3D declaration
//=============================================================================

		class Left3D : public Base
		{
		public:
			Left3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class Right declaration
//=============================================================================

		class Right : public Base
		{
		public:
			Right(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class Right3D declaration
//=============================================================================

		class Right3D : public Base
		{
		public:
			Right3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t);
			
			void left(int l);
			
			void paint(BITMAP* bmp, int hint);

			void top(int t);
		};

//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
				_owner(o),
				_left(x),
				_right(0),
				_top(y),
				_bottom(0),
				_width(w),
				_height(h),
				_state(Checkbox::unchecked),
				_callback(0)
		{
			// Save title if provided
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
		
		std::pair<Checkbox::CALLBACK, void*> Base::callback() const
		{
			return std::make_pair(_callback, _data);
		}

		void Base::callback(Checkbox::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}
		
#ifdef	_DEBUG
		void Base::dump() const
		{
			dump_xywh(_left, _top, _width, _top, _right, _bottom);
			TRACE("Title: %s\n", _title.c_str());
		}
#endif

		int Base::height() const
		{
			return _height;
		}
		
		int Base::left() const
		{
			return _left;
		}

		void Base::paint_check(BITMAP* bmp, int x, int y, int color)
		{
			vline(bmp, x + 3, y + 5, y + 7, color);
			vline(bmp, x + 4, y + 6, y + 8, color);
			vline(bmp, x + 5, y + 7, y + 9, color);
			vline(bmp, x + 6, y + 6, y + 8, color);
			vline(bmp, x + 7, y + 5, y + 7, color);
			vline(bmp, x + 8, y + 4, y + 6, color);
			vline(bmp, x + 9, y + 3, y + 5, color);
		}

		bool Base::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			// Get out if mouse isn't within us
			if (x < _left || x > _right || y < _top || y > _bottom)
				return false;
			
			// Remember original state in case its cancelled
			Checkbox::STATE originalState = _state;
			toggle_state();
			
			// Update position
			{
				Update u(bmp);
				paint(bmp, dirty_checkbox);
			}
				
			// Wait for button to be released
			Checkbox::STATE toggledState = _state;
			int mx = x;
			int my = y;
			bool oldHit = true;
			bool newHit;
			while ((mouse_b & 1) != 0)
			{
				// If mouse changed position
				if (mouse_x != mx || mouse_y != my)
				{
					// Save new position
					mx = mouse_x;
					my = mouse_y;
					
					// Determine if mouse is still in right arrow
					newHit = (mx >= _left && mx <= _right && my >= _top && my <= _bottom);
					
					// If mouse entered or left right arrow, update image
					if (oldHit != newHit)
					{
						if (newHit)
							_state = toggledState;
						else
							_state = originalState;
						{
							Update u(bmp);
							paint(bmp, dirty_checkbox);
						}
						oldHit = newHit;
					}
				}
			}
			
			// If ended inside us, call action
			if (oldHit && _callback)
				_callback(_owner, _data, bmp);
			
			return true;
		}
		
		int Base::right() const
		{
			return _right;
		}
		
		Checkbox::STATE Base::value() const
		{
			return _state;
		}
		
		void Base::value(Checkbox::STATE s)
		{
			if (_state != s)
			{
				ASSERT((s != Checkbox::gray) || states(_owner.flags()) == Checkbox::s_tristate);
				_state = s;
				_owner.dirty(Checkbox::all);
			}
		}
		
		int Base::top() const
		{
			return _top;
		}

		const char* Base::title() const
		{
			return _title.c_str();
		}
		
		void Base::title(const char* t)
		{
			_title = t;
			_owner.dirty(Checkbox::all);
		}
		
		const std::string& Base::title_object() const
		{
			return _title;
		}
		
		void Base::toggle_state()
		{
			if (states(_owner.flags()) != Checkbox::s_tristate)
			{
				if (_state == Checkbox::checked)
					_state = Checkbox::unchecked;
				else
					_state = Checkbox::checked;
			}
			else
			{
				switch (_state)
				{
				case Checkbox::checked:
					_state = Checkbox::gray;
					break;
				
				case Checkbox::unchecked:
					_state = Checkbox::checked;
					break;
				
				case Checkbox::gray:
					_state = Checkbox::unchecked;
					break;
				}
			}
		}
		
		int Base::width() const
		{
			return _width;
		}

//=============================================================================
// class Button implementation
//=============================================================================

		Button::Button(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			ButtonBase(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 8;
				if (!_title.empty())
					_width += text_length(font, _title.c_str());
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 8 + text_height(font);
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxLeft = _left;
			_boxRight = _right;
			_boxTop = _top;
			_boxBottom = _bottom;
		}

		void Button::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxLeft = _left;
			_boxRight = _right;
		}

		void Button::paint(BITMAP* bmp, int hint)
		{
			if (_state == Checkbox::checked)
			{
				// Paint box
				rectfill(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Foreground);

				// Draw title
				if (!_title.empty())
				{
					Clipper c(bmp, _left + 4, _top + 4, _right - 4, _bottom - 4);
					const int x = _left + (_width / 2);
					const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
					textout_centre_ex(bmp, font, _title.c_str(), x, y, Theme.flat.Background, Theme.flat.Foreground);
				}
			}
			else
			{
				// Paint box
				rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Foreground);
				rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1, Theme.flat.Background);
				
				// Draw title
				if (!_title.empty())
				{
					Clipper c(bmp, _left + 4, _top + 4, _right - 4, _bottom - 4);
					const int x = _left + (_width / 2);
					const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
					textout_centre_ex(bmp, font, _title.c_str(), x, y, Theme.flat.Foreground, Theme.flat.Background);
				}
			}
		}

		void Button::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top;
			_boxBottom = _bottom;
		}
		
//=============================================================================
// class Button3D implementation
//=============================================================================

		Button3D::Button3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			ButtonBase(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 8;
				if (!_title.empty())
					_width += text_length(font, _title.c_str());
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 8 + text_height(font);
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxLeft = _left;
			_boxRight = _right;
			_boxTop = _top;
			_boxBottom = _bottom;
		}

		void Button3D::left(int l)
		{
// BUGBUG:  Doesn't work as desired

			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxLeft = _left;
		}
		
		void Button3D::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			box(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, _state == Checkbox::checked,
					Theme.GRAY);
			
			// Draw title
			if (!_title.empty())
			{
				Clipper c(bmp, _left + 4, _top + 4, _right - 4, _bottom - 4);
				const int x = _left + (_width / 2);
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				if (!_owner.active())
				{
					textout_centre_ex(bmp, font, _title.c_str(), x + 1, y + 1, Theme.WHITE,
							Theme.GRAY);
					textout_centre_ex(bmp, font, _title.c_str(), x, y, Theme.GRAY_DARK, -1);
				}
				else
					textout_centre_ex(bmp, font, _title.c_str(), x, y, Theme.BLACK, Theme.GRAY);
			}
		}

		void Button3D::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top;
		}
				
//=============================================================================
// class ButtonBase implementation
//=============================================================================

		ButtonBase::ButtonBase(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			Base(o, x, y, w, h, f, t)
		{
		}

		void ButtonBase::toggle_state()
		{
			if (_state == Checkbox::checked)
				_state = Checkbox::unchecked;
			else
				_state = Checkbox::checked;
		}
		
//=============================================================================
// class ButtonOutside implementation
//=============================================================================

		ButtonOutside::ButtonOutside(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			ButtonBase(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 17;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 17;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 12;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
		void ButtonOutside::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 12;
		}
		
		void ButtonOutside::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			if (_state == Checkbox::checked)
			{
				rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Frame);
				rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1,
						Theme.flat.Foreground);
			}
			else
			{
				rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Frame);
				rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1,
						Theme.flat.Background);
			}
			
			// Draw title
			if (!_title.empty())
			{
				const int x = _boxRight + 6;
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1)
						- text_height(font)) / 2) + 1;
				textout_ex(bmp, font, _title.c_str(), x, y, Theme.flat.Foreground,
						Theme.flat.Background);
			}
		}

		void ButtonOutside::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
//=============================================================================
// class ButtonOutside3D implementation
//=============================================================================

		ButtonOutside3D::ButtonOutside3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			ButtonBase(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 19;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 19;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 12;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}

		void ButtonOutside3D::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 12;
		}
		
		void ButtonOutside3D::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			box(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, _state == Checkbox::checked,
					Theme.GRAY);
			
			// Draw title
			if (!_title.empty())
			{
				const int x = _boxRight + 6;
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				if (!_owner.active())
				{
					textout_ex(bmp, font, _title.c_str(), x + 1, y + 1, Theme.WHITE,
							Theme.GRAY);
					textout_ex(bmp, font, _title.c_str(), x, y, Theme.GRAY_DARK, -1);
				}
				else
					textout_ex(bmp, font, _title.c_str(), x, y, Theme.BLACK, Theme.GRAY);
			}
		}

		void ButtonOutside3D::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
//=============================================================================
// class ButtonOutsideLeft implementation
//=============================================================================

		ButtonOutsideLeft::ButtonOutsideLeft(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			ButtonBase(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 17;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 17;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 10;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 10;
		}
		
		void ButtonOutsideLeft::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Calculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 10;
		}
		
		void ButtonOutsideLeft::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			if (_state == Checkbox::checked)
			{
				rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Frame);
				rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1,
						Theme.flat.Foreground);
			}
			else
			{
				rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Frame);
				rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1,
						Theme.flat.Background);
			}
			
			// Draw title
			if (!_title.empty())
			{
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				textout_ex(bmp, font, _title.c_str(), _left + 3, y, Theme.flat.Foreground,
						Theme.flat.Background);
			}
		}

		void ButtonOutsideLeft::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 10;
		}
		
//=============================================================================
// class ButtonOutsideLeft3D implementation
//=============================================================================

		ButtonOutsideLeft3D::ButtonOutsideLeft3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			ButtonBase(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 19;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 19;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 12;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
		void ButtonOutsideLeft3D::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 12;
		}
		
		void ButtonOutsideLeft3D::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			box(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, _state == Checkbox::checked,
					Theme.GRAY);
			
			// Draw title
			if (!_title.empty())
			{
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				if (!_owner.active())
				{
					textout_ex(bmp, font, _title.c_str(), _left + 4, y + 1, Theme.WHITE,
							Theme.GRAY);
					textout_ex(bmp, font, _title.c_str(), _left + 3, y, Theme.GRAY_DARK, -1);
				}
				else
					textout_ex(bmp, font, _title.c_str(), _left + 3, y, Theme.BLACK, Theme.GRAY);
			}
		}

		void ButtonOutsideLeft3D::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
//=============================================================================
// class Left implementation
//=============================================================================

		Left::Left(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			Base(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 17;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 17;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 10;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 10;
		}
		
		void Left::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 10;
		}
		
		void Left::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Frame);
			rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1,
					_state == Checkbox::gray ? Theme.flat.Foreground : Theme.flat.Background);
			
			// Draw check if needed
			if (_state != Checkbox::unchecked)
			{
				paint_check(bmp, _boxLeft - 1, _boxTop - 1, _state == Checkbox::gray
						? Theme.flat.Background : Theme.flat.Foreground);
			}
			
			// Draw title
			if (!_title.empty())
			{
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font))
						/ 2) + 1;
				textout_ex(bmp, font, _title.c_str(), _left + 3, y, Theme.flat.Foreground,
						Theme.flat.Background);
			}
		}

		void Left::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 10;
		}
		
//=============================================================================
// class Left3D implementation
//=============================================================================

		Left3D::Left3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			Base(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 19;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 19;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 12;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
		void Left3D::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxRight = _right - 3;
			_boxLeft = _boxRight - 12;
		}
		
		void Left3D::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			box(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, true,
					_state == Checkbox::gray ? Theme.GRAY : Theme.WHITE);
			
			// Draw check if needed
			bool disabled = (_owner.active() != true);
			if (_state != Checkbox::unchecked)
				paint_check(bmp, _boxLeft, _boxTop, disabled ? Theme.GRAY_DARK : Theme.BLACK);
			
			// Draw title
			if (!_title.empty())
			{
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				if (disabled)
				{
					textout_ex(bmp, font, _title.c_str(), _left + 4, y + 1, Theme.WHITE,
							Theme.GRAY);
					textout_ex(bmp, font, _title.c_str(), _left + 3, y, Theme.GRAY_DARK, -1);
				}
				else
					textout_ex(bmp, font, _title.c_str(), _left + 3, y, Theme.BLACK, Theme.GRAY);
			}
		}

		void Left3D::top(int t)
		{
			_top = t;
			
			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
//=============================================================================
// class Right implementation
//=============================================================================

		Right::Right(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			Base(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 17;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 17;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 10;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 10;
		}
			
		void Right::left(int l)
		{
			_left = l;

			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 10;
		}
		
		void Right::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			rect(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, Theme.flat.Frame);
			rectfill(bmp, _boxLeft + 1, _boxTop + 1, _boxRight - 1, _boxBottom - 1,
					_state == Checkbox::gray ? Theme.flat.Foreground : Theme.flat.Background);
			
			// Draw check if needed
			if (_state != Checkbox::unchecked)
			{
				paint_check(bmp, _boxLeft - 1, _boxTop - 1, _state == Checkbox::gray
						? Theme.flat.Background : Theme.flat.Foreground);
			}
			
			// Draw title
			if (!_title.empty())
			{
				const int x = _boxRight + 6;
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				textout_ex(bmp, font, _title.c_str(), x, y, Theme.flat.Foreground, Theme.flat.Background);
			}
		}

		void Right::top(int t)
		{
			_top = t;

			// Recalculate bottom
			_bottom = _top + _height - 1;
			
			// Recalculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 10;
		}
		
//=============================================================================
// class Right3D implementation
//=============================================================================

		Right3D::Right3D(Checkbox& o, int x, int y, int w, int h, int f, const char* t)
				:
			Base(o, x, y, w, h, f, t)
		{
			// Calculate width if needed
			if (_width == Checkbox::calc)
			{
				_width = 19;
				if (!_title.empty())
					_width += (5 + text_length(font, _title.c_str()));
			}
			
			// Calculate height if needed
			if (_height == Checkbox::calc)
				_height = 19;
			
			// Calculate right/bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 12;
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
		
		void Right3D::left(int l)
		{
			_left = l;
			
			// Recalculate right
			_right = _left + _width - 1;
			
			// Recalculate checkbox boundary
			_boxLeft = _left + 3;
			_boxRight = _boxLeft + 12;
		}
		
		void Right3D::paint(BITMAP* bmp, int hint)
		{
			// Paint box
			box(bmp, _boxLeft, _boxTop, _boxRight, _boxBottom, true,
					_state == Checkbox::gray ? Theme.GRAY : Theme.WHITE);
			
			// Draw check if needed
			bool disabled = (_owner.active() != true);
			if (_state != Checkbox::unchecked)
				paint_check(bmp, _boxLeft, _boxTop, disabled ? Theme.GRAY_DARK : Theme.BLACK);
			
			// Draw title
			if (!_title.empty())
			{
				const int x = _boxRight + 6;
				const int y = _boxTop + ((((_boxBottom - _boxTop) + 1) - text_height(font)) / 2) + 1;
				if (disabled)
				{
					textout_ex(bmp, font, _title.c_str(), x + 1, y + 1, Theme.WHITE,
							Theme.GRAY);
					textout_ex(bmp, font, _title.c_str(), x, y, Theme.GRAY_DARK, -1);
				}
				else
					textout_ex(bmp, font, _title.c_str(), x, y, Theme.BLACK, Theme.GRAY);
			}
		}

		void Right3D::top(int t)
		{
			_top = t;
			
			// Calculate right/bottom
			_bottom = _top + _height - 1;
			
			// Calculate checkbox boundary
			_boxTop = _top + 3;
			_boxBottom = _boxTop + 12;
		}
	}
}

//=============================================================================
// class Checkbox implementation
//=============================================================================

Checkbox::Checkbox(_Manager& m, int x, int y, int w, int h, int f, const char* title)
		:
		_ControlEx(m, f | m_down)
{
	const int flags = _Control::flags();
	if (Theme.style == Configuration::STYLE_3D)
	{
		switch (button(flags))
		{
		case b_box:
			switch (title_(flags))
			{
			case t_righttitle:
				_impl = new CheckboxImpl::Right3D(*this, x, y, w, h, f, title);
				break;

			case t_lefttitle:
				_impl = new CheckboxImpl::Left3D(*this, x, y, w, h, f, title);
				break;
			}
			break;
			
		case b_button:
			_impl = new CheckboxImpl::Button3D(*this, x, y, w, h, f, title);
			break;
		
		case b_buttonoutside:
			switch (title_(flags))
			{
			case t_righttitle:
				_impl = new CheckboxImpl::ButtonOutside3D(*this, x, y, w, h, f, title);
				break;
				
			case t_lefttitle:
				_impl = new CheckboxImpl::ButtonOutsideLeft3D(*this, x, y, w, h, f, title);
				break;
			}
			break;
		}
	}
	else
	{
	}
}

Checkbox::~Checkbox()
{
	delete _impl;
}

Checkbox& Checkbox::operator=(STATE v)
{
	value(v);
	return *this;
}

Checkbox& Checkbox::operator=(bool v)
{
	if (v)
		value(checked);
	else
		value(unchecked);
	return *this;
}

Checkbox::operator Checkbox::STATE() const
{
	return _impl->value();
}

bool Checkbox::operator!() const
{
	return (_impl->value() != unchecked);
}

void Checkbox::active_changed(bool o, bool n)
{
}

int Checkbox::bottom() const
{
	return _impl->bottom();
}

std::pair<Checkbox::CALLBACK, void*> Checkbox::callback() const
{
	return _impl->callback();
}

void Checkbox::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

#ifdef	_DEBUG
void Checkbox::dump() const
{
	TRACE("yacg::Checkbox @ $%08X\n", this);
	TRACE(" Flags:");
	const int flags = _Control::flags();
	switch (states(flags))
	{
	case s_boolean:
		TRACE(" s_boolean");
		break;
		
	case s_tristate:
		TRACE(" s_tristate");
		break;
	}
	switch (title_(flags))
	{
	case t_righttitle:
		TRACE(" | t_righttitle");
		break;
		
	case t_lefttitle:
		TRACE(" | t_lefttitle");
		break;
	}
	switch (button(flags))
	{
	case b_box:
		TRACE(" | b_box\n");
		break;

	case b_button:
		TRACE(" | b_button\n");
		break;

	case b_buttonoutside:
		TRACE(" | b_buttonoutside\n");
		break;
	}
	_impl->dump();
}
#endif

void Checkbox::flags_changed(int o, int n)
{
}

int Checkbox::height() const
{
	return _impl->height();
}

int Checkbox::left() const
{
	return _impl->left();
}

void Checkbox::left(int l)
{
	_impl->left(l);
}

void Checkbox::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

bool Checkbox::mouse_down(BITMAP* b, int x, int y, int m)
{
	return _impl->mouse_down(b, x, y, m);
}

int Checkbox::right() const
{
	return _impl->right();
}

Checkbox::STATE Checkbox::value() const
{
	return _impl->value();
}

void Checkbox::value(STATE s)
{
	_impl->value(s);
}

const char* Checkbox::title() const
{
	return _impl->title();
}

void Checkbox::title(const char* t)
{
	_impl->title(t);
}

const std::string& Checkbox::title_object() const
{
	return _impl->title_object();
}

int Checkbox::top() const
{
	return _impl->top();
}

void Checkbox::top(int t)
{
	_impl->top(t);
}

int Checkbox::width() const
{
	return _impl->width();
}
