#include "precompiled.hpp"
#include "yacg/CheckedBox.hpp"
#include "yacg/Checkbox.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace
{
	inline int alignh(int f)
	{
		return (f & CheckedBox::h_mask);
	}
	
	inline int alignv(int f)
	{
		return (f & CheckedBox::v_mask);
	}
	
	inline int button(int f)
	{
		return (f & CheckedBox::b_mask);
	}
}

namespace yacg
{
	namespace CheckedBoxImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		class Base
		{
		public:
			typedef std::vector<_Control*> container;
			
		public:
			Base(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);
			virtual ~Base();
			
			CheckedBox::iterator begin();
			CheckedBox::const_iterator begin() const;

			int bottom() const;

			int bottom_controls() const;

			std::pair<CheckedBox::CALLBACK, void*> callback() const;
			void callback(CheckedBox::CALLBACK f, void* d);

#ifdef	_DEBUG
			void dump() const;
#endif

			CheckedBox::iterator end();
			CheckedBox::const_iterator end() const;

			bool empty() const;
			
			void erase(CheckedBox::iterator i);
			
			int height() const;

			int height_controls() const;

			CheckedBox::iterator insert(_Control& c);

			int left() const;

			int left_controls() const;

			void paint(BITMAP* bmp);

			bool mouse_down(BITMAP* b, int x, int y, int m);

			int right() const;
			
			int right_controls() const;
			
			int size() const;

			bool state() const;
			void state(bool s);

			const char* title() const;
			void title(const char* t);
			
			const std::string& title_object() const;

			int top() const;
			
			int top_controls() const;
			
			int width() const;

			int width_controls() const;

		public:
			virtual void paint_box(BITMAP* bmp) = 0;

		protected:
			static void checkbox_changed(Checkbox& c, void* d, BITMAP* bmp);
			
		protected:
			CheckedBox& _owner;
			
			int _left;
			int _leftControls;
			int _right;
			int _rightControls;
			
			int _top;
			int _topControls;
			int _bottom;
			int _bottomControls;
			
			int _width;
			int _height;
			
			CheckedBox::CALLBACK _callback;
			void* _data;
			
			bool _state;
			
			container _controls;
			
			std::auto_ptr<Checkbox> _checkbox;
		};
		
//=============================================================================
// class BottomLeft declaration
//=============================================================================

		class BottomLeft : public Base
		{
		public:
			BottomLeft(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);
			
			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class BottomLeft3D declaration
//=============================================================================

		class BottomLeft3D : public Base
		{
		public:
			BottomLeft3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);

			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class BottomRight declaration
//=============================================================================

		class BottomRight : public Base
		{
		public:
			BottomRight(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);
			
			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class BottomRight3D declaration
//=============================================================================

		class BottomRight3D : public Base
		{
		public:
			BottomRight3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);

			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class TopLeft declaration
//=============================================================================

		class TopLeft : public Base
		{
		public:
			TopLeft(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);
			
			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class TopLeft3D declaration
//=============================================================================

		class TopLeft3D : public Base
		{
		public:
			TopLeft3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);

			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class TopRight declaration
//=============================================================================

		class TopRight : public Base
		{
		public:
			TopRight(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);
			
			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class TopRight3D declaration
//=============================================================================

		class TopRight3D : public Base
		{
		public:
			TopRight3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t);

			void paint_box(BITMAP* bmp);
		};
		
//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				_owner(o),
				_left(x),
				_right(x + w - 1),
				_top(y),
				_bottom(y + h - 1),
				_width(w),
				_height(h),
				_callback(0),
				_state(false)
		{
		}
		
		Base::~Base()
		{
		}
		
		CheckedBox::iterator Base::begin()
		{
			return _controls.begin();
		}
		
		CheckedBox::const_iterator Base::begin() const
		{
			return _controls.begin();
		}

		int Base::bottom() const
		{
			return _bottom;
		}

		int Base::bottom_controls() const
		{
			return _bottomControls;
		}

		void Base::checkbox_changed(Checkbox& c, void* d, BITMAP* bmp)
		{
#define	_this	reinterpret_cast<Base*>(d)
			// Update enablement of controls
			if (c != Checkbox::unchecked)
				std::for_each(_this->_controls.begin(), _this->_controls.end(), control_enable);
			else
				std::for_each(_this->_controls.begin(), _this->_controls.end(), control_disable);
			
			// Notify caller if needed
			if (_this->_callback)
				_this->_callback(_this->_owner, bmp, _this->_data);
#undef	_this
		}

		std::pair<CheckedBox::CALLBACK, void*> Base::callback() const
		{
			return std::make_pair(_callback, _data);
		}
		
		void Base::callback(CheckedBox::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}

#ifdef	_DEBUG
		void Base::dump() const
		{
			dump_xywh(_left, _top, _width, _top, _right, _bottom);
			TRACE(" Controls Left: %d\n", _leftControls);
			TRACE(" Controls Top: %d\n", _topControls);
			TRACE(" Controls Width (right): %d (%d)\n", width_controls(), _rightControls);
			TRACE(" Controls Height (bottom): %d (%d)\n", height_controls(), _bottomControls);
			_checkbox->dump();
		}
#endif

		CheckedBox::iterator Base::end()
		{
			return _controls.end();
		}
		
		CheckedBox::const_iterator Base::end() const
		{
			return _controls.end();
		}

		bool Base::empty() const
		{
			return _controls.empty();
		}
		
		void Base::erase(CheckedBox::iterator i)
		{
			_controls.erase(i);
		}
		
		int Base::height() const
		{
			return _height;
		}

		int Base::height_controls() const
		{
			return ((_bottomControls - _topControls) + 1);
		}

		CheckedBox::iterator Base::insert(_Control& c)
		{
			c.active(*_checkbox != Checkbox::unchecked);
			return _controls.insert(_controls.end(), &c);
		}
		
		int Base::left() const
		{
			return _left;
		}

		int Base::left_controls() const
		{
			return _leftControls;
		}

		void Base::paint(BITMAP* bmp)
		{
			paint_box(bmp);
			_checkbox->paint(bmp);
		}

		bool Base::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			return _checkbox->mouse_down(bmp, x, y, m);
		}

		int Base::right() const
		{
			return _right;
		}
		
		int Base::right_controls() const
		{
			return _rightControls;
		}
		
		int Base::size() const
		{
			return int(_controls.size());
		}
		
		bool Base::state() const
		{
			return _state;
		}
		
		void Base::state(bool s)
		{
			if (_state != s)
			{
				_state = s;
				_owner.dirty(CheckedBox::all);
			}
		}

		const char* Base::title() const
		{
			return _checkbox->title();
		}
		
		void Base::title(const char* t)
		{
			_checkbox->title(t);
		}
		
		const std::string& Base::title_object() const
		{
			return _checkbox->title_object();
		}

		int Base::top() const
		{
			return _top;
		}
		
		int Base::top_controls() const
		{
			return _topControls;
		}
		
		int Base::width() const
		{
			return _width;
		}

		int Base::width_controls() const
		{
			return ((_rightControls - _leftControls) + 1);
		}

//=============================================================================
// class BottomLeft implementation
//=============================================================================

		BottomLeft::BottomLeft(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = 0;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left + 10, _bottom,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			_checkbox->top(_bottom - _checkbox->height() + 1);
			
			// Calculate control boundaries
			_topControls = _top + 1;
			_bottomControls = _checkbox->top() - 1;
			_leftControls = _left + 1;
			_rightControls = _right - 1;
		}
			
		void BottomLeft::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int bottom = (_bottom - (_checkbox->height() / 2)) + 1;
			
			// Top
			hline(bmp, _left, _top, _right, Theme.flat.Frame);
			
			// Left
			vline(bmp, _left, _top + 1, bottom - 1, Theme.flat.Frame);
			
			// Bottom left of checkbox
			hline(bmp, _left + 1, bottom - 1, _checkbox->left() - 1, Theme.flat.Frame);
			
			// Bottom right of checkbox
			hline(bmp, _checkbox->right() + 1, bottom - 1, _right - 1, Theme.flat.Frame);
			
			// Right
			vline(bmp, _right - 1, _top + 1, bottom - 2, Theme.flat.Frame);
		}

//=============================================================================
// class BottomLeft3D implementation
//=============================================================================

		BottomLeft3D::BottomLeft3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = 0;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left + 10, _bottom,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			_checkbox->top(_bottom - _checkbox->height() + 1);
			
			// Calculate control boundaries
			_topControls = _top + 2;
			_bottomControls = _checkbox->top() - 2;
			_leftControls = _left + 2;
			_rightControls = _right - 2;
		}
		
		void BottomLeft3D::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int bottom = (_bottom - (_checkbox->height() / 2)) + 1;
			
			// Top
			hline(bmp, _left, _top, _right, Theme.GRAY_DARK);
			hline(bmp, _left + 1, _top + 1, _right - 1, Theme.WHITE);
			
			// Left
			vline(bmp, _left, _top + 1, bottom - 1, Theme.GRAY_DARK);
			vline(bmp, _left + 1, _top + 2, bottom - 2, Theme.WHITE);
			
			// Bottom left of checkbox
			hline(bmp, _left + 1, bottom - 1, _checkbox->left() - 1, Theme.GRAY_DARK);
			hline(bmp, _left, bottom, _checkbox->left() - 1, Theme.WHITE);
			
			// Bottom right of checkbox
			hline(bmp, _checkbox->right() + 1, bottom - 1, _right - 1, Theme.GRAY_DARK);
			hline(bmp, _checkbox->right() + 1, bottom, _right, Theme.WHITE);
			
			// Right
			vline(bmp, _right - 1, _top + 1, bottom - 2, Theme.GRAY_DARK);
			vline(bmp, _right, _top, bottom - 1, Theme.WHITE);
		}

//=============================================================================
// class BottomRight implementation
//=============================================================================

		BottomRight::BottomRight(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = Checkbox::t_lefttitle;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left, _bottom,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			_checkbox->top(_bottom - _checkbox->height() + 1);
			_checkbox->left(_right - _checkbox->width() - 10);

			// Calculate control boundaries
			_topControls = _top + 1;
			_bottomControls = _checkbox->top() - 1;
			_leftControls = _left + 1;
			_rightControls = _right - 1;
		}
			
		void BottomRight::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int bottom = (_bottom - (_checkbox->height() / 2)) + 1;
			
			// Top
			hline(bmp, _left, _top, _right, Theme.flat.Frame);
			
			// Left
			vline(bmp, _left, _top + 1, bottom - 1, Theme.flat.Frame);
			
			// Bottom left of checkbox
			hline(bmp, _left + 1, bottom - 1, _checkbox->left() - 1, Theme.flat.Frame);
			
			// Bottom right of checkbox
			hline(bmp, _checkbox->right() + 1, bottom - 1, _right - 1, Theme.flat.Frame);
			
			// Right
			vline(bmp, _right - 1, _top + 1, bottom - 2, Theme.flat.Frame);
		}

//=============================================================================
// class BottomRight3D implementation
//=============================================================================

		BottomRight3D::BottomRight3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = Checkbox::t_lefttitle;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left, _bottom,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			_checkbox->top(_bottom - _checkbox->height() + 1);
			_checkbox->left(_right - _checkbox->width() - 10);

			// Calculate control boundaries
			_topControls = _top + 2;
			_bottomControls = _checkbox->top() - 2;
			_leftControls = _left + 2;
			_rightControls = _right - 2;
		}
			
		void BottomRight3D::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int bottom = (_bottom - (_checkbox->height() / 2)) + 1;
			
			// Top
			hline(bmp, _left, _top, _right, Theme.GRAY_DARK);
			hline(bmp, _left + 1, _top + 1, _right - 1, Theme.WHITE);
			
			// Left
			vline(bmp, _left, _top + 1, bottom - 1, Theme.GRAY_DARK);
			vline(bmp, _left + 1, _top + 2, bottom - 2, Theme.WHITE);
			
			// Bottom left of checkbox
			hline(bmp, _left + 1, bottom - 1, _checkbox->left() - 1, Theme.GRAY_DARK);
			hline(bmp, _left, bottom, _checkbox->left() - 1, Theme.WHITE);
			
			// Bottom right of checkbox
			hline(bmp, _checkbox->right() + 1, bottom - 1, _right - 1, Theme.GRAY_DARK);
			hline(bmp, _checkbox->right() + 1, bottom, _right, Theme.WHITE);
			
			// Right
			vline(bmp, _right - 1, _top + 1, bottom - 2, Theme.GRAY_DARK);
			vline(bmp, _right, _top, bottom - 1, Theme.WHITE);
		}

//=============================================================================
// class TopLeft implementation
//=============================================================================

		TopLeft::TopLeft(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = 0;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left + 10, _top,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			
			// Calculate control boundaries
			_topControls = _checkbox->bottom() + 1;
			_bottomControls = _bottom - 1;
			_leftControls = _left + 1;
			_rightControls = _right - 1;
		}
		
		void TopLeft::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int top = _top + (_checkbox->height() / 2);
			
			// Top left of checkbox
			hline(bmp, _left, top, _checkbox->left() - 1, Theme.flat.Frame);
			
			// Top right of checkbox
			hline(bmp, _checkbox->right() + 1, top, _right - 1, Theme.flat.Frame);
			
			// Left
			vline(bmp, _left, top + 1, _bottom - 1, Theme.flat.Frame);
			
			// Bottom
			hline(bmp, _left + 1, _bottom - 1, _right - 1, Theme.flat.Frame);
			
			// Right
			vline(bmp, _right - 1, top + 1, _bottom - 2, Theme.flat.Frame);
		}

//=============================================================================
// class TopLeft3D implementation
//=============================================================================

		TopLeft3D::TopLeft3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = 0;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left + 10, _top,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			
			// Calculate control boundaries
			_topControls = _checkbox->bottom() + 2;
			_bottomControls = _bottom - 2;
			_leftControls = _left + 2;
			_rightControls = _right - 2;
		}
		
		void TopLeft3D::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int top = _top + (_checkbox->height() / 2);
			
			// Top left of checkbox
			hline(bmp, _left, top, _checkbox->left() - 1, Theme.GRAY_DARK);
			hline(bmp, _left + 1, top + 1, _checkbox->left() - 1, Theme.WHITE);
			
			// Top right of checkbox
			hline(bmp, _checkbox->right() + 1, top, _right - 1, Theme.GRAY_DARK);
			hline(bmp, _checkbox->right() + 1, top + 1, _right - 2, Theme.WHITE);
			
			// Left
			vline(bmp, _left, top + 1, _bottom - 1, Theme.GRAY_DARK);
			vline(bmp, _left + 1, top + 2, _bottom - 2, Theme.WHITE);
			
			// Bottom
			hline(bmp, _left + 1, _bottom - 1, _right - 1, Theme.GRAY_DARK);
			hline(bmp, _left, _bottom, _right, Theme.WHITE);
			
			// Right
			vline(bmp, _right - 1, top + 1, _bottom - 2, Theme.GRAY_DARK);
			vline(bmp, _right, top, _bottom - 1, Theme.WHITE);
		}

//=============================================================================
// class TopRight implementation
//=============================================================================

		TopRight::TopRight(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = Checkbox::t_lefttitle;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left, _top,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			_checkbox->left(_right - _checkbox->width() - 10);
			
			// Calculate control boundaries
			_topControls = _checkbox->bottom() + 1;
			_bottomControls = _bottom - 1;
			_leftControls = _left + 1;
			_rightControls = _right - 1;
		}
		
		void TopRight::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int top = _top + (_checkbox->height() / 2);
			
			// Top left of checkbox
			hline(bmp, _left, top, _checkbox->left() - 1, Theme.flat.Frame);
			
			// Top right of checkbox
			hline(bmp, _checkbox->right() + 1, top, _right - 1, Theme.flat.Frame);
			
			// Left
			vline(bmp, _left, top + 1, _bottom - 1, Theme.flat.Frame);
			
			// Bottom
			hline(bmp, _left + 1, _bottom - 1, _right - 1, Theme.flat.Frame);
			
			// Right
			vline(bmp, _right - 1, top + 1, _bottom - 2, Theme.flat.Frame);
		}

//=============================================================================
// class TopRight3D implementation
//=============================================================================

		TopRight3D::TopRight3D(CheckedBox& o, int x, int y, int w, int h, int f, const char* t)
				:
				Base(o, x, y, w, h, f, t)
		{
			// Create checkbox
			int cbf = Checkbox::t_lefttitle;
			if (button(f) == CheckedBox::b_button)
				cbf |= Checkbox::b_buttonoutside;
			_checkbox = std::auto_ptr<Checkbox>(new Checkbox(o.manager(), _left, _top,
					Checkbox::calc, Checkbox::calc, cbf, t));
			_checkbox->callback(checkbox_changed, this);
			_checkbox->left(_right - _checkbox->width() - 10);
			
			// Calculate control boundaries
			_topControls = _checkbox->bottom() + 2;
			_bottomControls = _bottom - 2;
			_leftControls = _left + 2;
			_rightControls = _right - 2;
		}
		
		void TopRight3D::paint_box(BITMAP* bmp)
		{
			// Paint box
			const int top = _top + (_checkbox->height() / 2);
			
			// Top left of checkbox
			hline(bmp, _left, top, _checkbox->left() - 1, Theme.GRAY_DARK);
			hline(bmp, _left + 1, top + 1, _checkbox->left() - 1, Theme.WHITE);
			
			// Top right of checkbox
			hline(bmp, _checkbox->right() + 1, top, _right - 1, Theme.GRAY_DARK);
			hline(bmp, _checkbox->right() + 1, top + 1, _right - 2, Theme.WHITE);
			
			// Left
			vline(bmp, _left, top + 1, _bottom - 1, Theme.GRAY_DARK);
			vline(bmp, _left + 1, top + 2, _bottom - 2, Theme.WHITE);
			
			// Bottom
			hline(bmp, _left + 1, _bottom - 1, _right - 1, Theme.GRAY_DARK);
			hline(bmp, _left, _bottom, _right, Theme.WHITE);
			
			// Right
			vline(bmp, _right - 1, top + 1, _bottom - 2, Theme.GRAY_DARK);
			vline(bmp, _right, top, _bottom - 1, Theme.WHITE);
		}
	}
}

//=============================================================================
// class CheckedBox implementation
//=============================================================================

CheckedBox::CheckedBox(_Manager& m, int x, int y, int w, int h, int f, const char* t)
		:
		_ControlEx(m, f | m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
	{
		if (alignv(f) != v_bottom)
		{
			if (alignh(f) == h_left)
				_impl = new CheckedBoxImpl::TopLeft3D(*this, x, y, w, h, f, t);
			else
				_impl = new CheckedBoxImpl::TopRight3D(*this, x, y, w, h, f, t);
		}
		else
		{
			if (alignh(f) == h_left)
				_impl = new CheckedBoxImpl::BottomLeft3D(*this, x, y, w, h, f, t);
			else
				_impl = new CheckedBoxImpl::BottomRight3D(*this, x, y, w, h, f, t);
		}
	}
	else
	{
		if (alignv(f) != v_bottom)
		{
			if (alignh(f) == h_left)
				_impl = new CheckedBoxImpl::TopLeft(*this, x, y, w, h, f, t);
			else
				_impl = new CheckedBoxImpl::TopRight(*this, x, y, w, h, f, t);
		}
		else
		{
			if (alignh(f) == h_left)
				_impl = new CheckedBoxImpl::BottomLeft(*this, x, y, w, h, f, t);
			else
				_impl = new CheckedBoxImpl::BottomRight(*this, x, y, w, h, f, t);
		}
	}
}

CheckedBox::~CheckedBox()
{
	delete _impl;
}

void CheckedBox::active_changed(bool o, bool n)
{
	dirty(all);
	// TODO:
}

CheckedBox::iterator CheckedBox::begin()
{
	return _impl->begin();
}

CheckedBox::const_iterator CheckedBox::begin() const
{
	return _impl->begin();
}

int CheckedBox::bottom() const
{
	return _impl->bottom();
}

int CheckedBox::bottom_controls() const
{
	return _impl->bottom_controls();
}

std::pair<CheckedBox::CALLBACK, void*> CheckedBox::callback() const
{
	return _impl->callback();
}

void CheckedBox::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

#ifdef	_DEBUG
void CheckedBox::dump() const
{
	const int flags = _Control::flags();
	TRACE("gui::CheckedBox @ $%08X\n", this);
	TRACE(" Flags:");
	switch (button(flags))
	{
	case b_box:
		TRACE(" b_box");
		break;
		
	case b_button:
		TRACE(" b_button");
		break;
	}
	switch (alignh(flags))
	{
	case h_left:
		TRACE(" | h_left");
		break;
		
	case h_right:
		TRACE(" | h_right");
		break;
	}
	switch (alignv(flags))
	{
	case v_top:
		TRACE(" | v_top\n");
		break;
	
	case v_bottom:
		TRACE(" | v_bottom\n");
		break;
	}
	_impl->dump();
}
#endif

CheckedBox::iterator CheckedBox::end()
{
	return _impl->end();
}

CheckedBox::const_iterator CheckedBox::end() const
{
	return _impl->end();
}

bool CheckedBox::empty() const
{
	return _impl->empty();
}

void CheckedBox::erase(iterator i)
{
	_impl->erase(i);
}

void CheckedBox::flags_changed(int o, int n)
{
	// TODO:
}

int CheckedBox::height() const
{
	return _impl->height();
}

int CheckedBox::height_controls() const
{
	return _impl->height_controls();
}

CheckedBox::iterator CheckedBox::insert(_Control& c)
{
	return _impl->insert(c);
}

CheckedBox::iterator CheckedBox::insert(_ControlEx& c)
{
	return _impl->insert(c);
}

int CheckedBox::left() const
{
	return _impl->left();
}

int CheckedBox::left_controls() const
{
	return _impl->left_controls();
}

void CheckedBox::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

bool CheckedBox::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	return _impl->mouse_down(bmp, x, y, m);
}

int CheckedBox::right() const
{
	return _impl->right();
}

int CheckedBox::right_controls() const
{
	return _impl->right_controls();
}

int CheckedBox::size() const
{
	return _impl->size();
}

bool CheckedBox::state() const
{
	return _impl->state();
}

void CheckedBox::state(bool s)
{
	_impl->state(s);
}

const char* CheckedBox::title() const
{
	return _impl->title();
}

void CheckedBox::title(const char* t)
{
	_impl->title(t);
}

const std::string& CheckedBox::title_object() const
{
	return _impl->title_object();
}

int CheckedBox::top() const
{
	return _impl->top();
}

int CheckedBox::top_controls() const
{
	return _impl->top_controls();
}

int CheckedBox::width() const
{
	return _impl->width();
}

int CheckedBox::width_controls() const
{
	return _impl->width_controls();
}
