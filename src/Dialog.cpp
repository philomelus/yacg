#include "precompiled.hpp"
#include <yacg/Dialog.hpp>
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace
{
}

namespace yacg
{
	namespace DialogImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		/// \note I was unable to display the dialog in constructor, because
		/// the derived object hasn't completed construction yet, and this
		/// calls virtual functions within that object (namely, \c init()), so
		/// it has to remain a 2 stage implementation.  I DESPISE THAT IDEA!

		class Base
		{
		public:
			Base(_Dialog& d, int x, int y, int w, int h, const char* t, BITMAP* bmp);
			Base(_Dialog& d, int w, int h, const char* t, BITMAP* bmp);

			int bottom() const;

			int bottom_controls() const;
			void bottom_controls(int bc);
		
			std::pair<_Dialog::IDLE, void*> callback() const;
			void callback(_Dialog::IDLE f, void* d);

			void execute();

			int height() const;
			int height_controls() const;
		
			int left() const;

			int left_controls() const;
			void left_controls(int lc);

			int right() const;

			int right_controls() const;
			void right_controls(int rc);
			
			const char* title() const;
			void title(const char* t);

			Title& title_object() const;
			
			int top() const;

			int top_controls() const;
			void top_controls(int tc);

			int width() const;
			int width_controls() const;
			
		public:
			virtual void frame(BITMAP* bmp, int l, int t, int b, int r, int w, int h) = 0;
					
		protected:
			_Dialog& _owner;
			
			int _left;
			int _leftControls;
			
			int _right;
			int _rightControls;
			
			int _top;
			int _topControls;
			
			int _bottom;
			int _bottomControls;
			
			BITMAP* _root;
			
			_Dialog::IDLE _callback;
			void* _data;
			
			std::auto_ptr<Title> _title;
		};

//=============================================================================
// class Impl declaration
//=============================================================================

		class Impl : public Base
		{
		public:
			Impl(_Dialog& d, int x, int y, int w, int h, const char* t, BITMAP* bmp);
			Impl(_Dialog& d, int w, int h, const char* t, BITMAP* bmp);

			void frame(BITMAP* bmp, int l, int t, int r, int b, int w, int h);
		};
		
//=============================================================================
// class Impl3D declaration
//=============================================================================

		class Impl3D : public Base
		{
		public:
			Impl3D(_Dialog& d, int x, int y, int w, int h, const char* t, BITMAP* bmp);
			Impl3D(_Dialog& d, int w, int h, const char* t, BITMAP* bmp);

			void frame(BITMAP* bmp, int l, int t, int r, int b, int w, int h);
		};
		
//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(_Dialog& d, int x, int y, int w, int h, const char* t, BITMAP* bmp)
				:
				_owner(d),
				_left(x),
				_right(x + w - 1),
				_top(y),
				_bottom(y + h - 1),
				_root(bmp),
				_callback(0)
		{
			if (t)
			{
				_title = std::auto_ptr<Title>(new Title(d, _left, _top, w, Title::calc,
						Title::h_center | Title::v_center | Title::f_flat, t));
			}
		}
		
		Base::Base(_Dialog& d, int w, int h, const char* t, BITMAP* bmp)
				:
				_owner(d),
				_root(bmp),
				_callback(0)
		{
			_left = (bmp->w - w) / 2;
			_right = _left + w - 1;
			_top = (bmp->h - h) / 2;
			_bottom = _top + h - 1;
			if (t)
			{
				_title = std::auto_ptr<Title>(new Title(d, _left, _top, w, Title::calc,
						Title::h_center | Title::v_center | Title::f_flat, t));
			}
		}
		
		int Base::bottom() const
		{
			return _bottom;
		}

		int Base::bottom_controls() const
		{
			return _bottomControls;
		}

		void Base::bottom_controls(int bc)
		{
			_bottomControls = bc;
		}

		std::pair<_Dialog::IDLE, void*> Base::callback() const
		{
			return std::make_pair(_callback, _data);
		}

		void Base::callback(_Dialog::IDLE f, void* d)
		{
			_callback = f;
			_data = d;
		}
		
		void Base::execute()
		{
			// NOTE: _left/_right/_top/_bottom must be initialized before this is called
			
			const int w = width();
			const int h = height();

			// Save a copy of bitmap we will modify
			auto_bitmap save(create_bitmap(w, h));
			{
				Update u(_root);
				blit(_root, save, _left, _top, 0, 0, w, h);
			}
			
			{
				Clipper c(_root, _left, _top, _right, _bottom);
			
				// Paint frame and background
				{
					Update u(_root);
					_owner.frame(_root, _left, _top, _right, _bottom, w, h);
				}
				
				// Initialize title
				if (_title.get())
				{
					_title->left(_leftControls);
					_title->top(_topControls);
					_title->width(width_controls());
					_topControls = _title->bottom() + 1;
					_owner.insert(*_title.get());
				}
				
				// Initialize controls
				_owner.init();

				// Enter event loop
				_owner.update();
				std::pair<bool, bool> result = std::make_pair(true, false);
				while (!result.second)
				{
					if (result.first)
						_owner.update();
					result = _owner._Manager::process_events_and_return();
					if (!result.first)
					{
						// Call user callback or give up some cpu time
						if (_callback)
							_callback(_owner, _data);
						else
							rest(1);
					}
				}
				
				// Allow owner to clean up controls
				if (_title.get())
					_owner.erase(*_title.get());
				_owner.uninit();
			}
			
			// Restore bitmap to original state
			{
				Update u(_root);
				blit(save, _root, 0, 0, _left, _top, w, h);
			}
		}

		int Base::height() const
		{
			return (_bottom - _top) + 1;
		}

		int Base::height_controls() const
		{
			return (_bottomControls - _topControls) + 1;
		}

		int Base::left() const
		{
			return _left;
		}

		int Base::left_controls() const
		{
			return _leftControls;
		}

		void Base::left_controls(int lc)
		{
			_leftControls = lc;
		}

		int Base::right() const
		{
			return _right;
		}

		int Base::right_controls() const
		{
			return _rightControls;
		}

		void Base::right_controls(int rc)
		{
			_rightControls = rc;
		}

		const char* Base::title() const
		{
			return _title->title();
		}

		void Base::title(const char* t)
		{
			_title->title(t);
		}

		Title& Base::title_object() const
		{
			return *_title.get();
		}

		int Base::top() const
		{
			return _top;
		}

		int Base::top_controls() const
		{
			return _topControls;
		}

		void Base::top_controls(int tc)
		{
			_topControls = tc;
		}

		int Base::width() const
		{
			return (_right - _left) + 1;
		}

		int Base::width_controls() const
		{
			return (_rightControls - _leftControls) + 1;
		}
		
//=============================================================================
// class Impl implementation
//=============================================================================

		Impl::Impl(_Dialog& d, int x, int y, int w, int h, const char* t, BITMAP* bmp)
				:
				Base(d, x, y, w, h, t, bmp)
		{
		}
		
		Impl::Impl(_Dialog& d, int w, int h, const char* t, BITMAP* bmp)
				:
				Base(d, w, h, t, bmp)
		{
		}

		void Impl::frame(BITMAP* bmp, int l, int t, int r, int b, int w, int h)
		{
			rect(bmp, l, t, r, b, Theme.flat.Frame);
			rect(bmp, l + 1, t + 1, r - 1, b - 1, Theme.flat.Background);
			rect(bmp, l + 2, t + 2, r - 2, b - 2, Theme.flat.Frame);
			rectfill(bmp, l + 3, t + 3, r - 3, b - 3, Theme.flat.Background);
			int offset = 3;
			if (_title.get())
				offset = 2;
			_leftControls = l + offset;
			_rightControls = r - offset;
			_topControls = t + offset;
			_bottomControls = b - 3;
		}
		
//=============================================================================
// class Impl3D implementation
//=============================================================================
		
		Impl3D::Impl3D(_Dialog& d, int x, int y, int w, int h, const char* t, BITMAP* bmp)
				:
				Base(d, x, y, w, h, t, bmp)
		{
		}
		
		Impl3D::Impl3D(_Dialog& d, int w, int h, const char* t, BITMAP* bmp)
				:
				Base(d, w, h, t, bmp)
		{
		}

		void Impl3D::frame(BITMAP* bmp, int l, int t, int r, int b, int w, int h)
		{
			box(bmp, l, t, r, b, false, Theme.GRAY);
			_leftControls = l + 3;
			_rightControls = r - 3;
			_topControls = t + 3;
			_bottomControls = b - 3;
		}
	}
}

//=============================================================================
// class _Dialog implementation
//=============================================================================

_Dialog::_Dialog(int x, int y, int w, int h, const char* t, BITMAP* bmp)
		:
		_Manager(bmp)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new DialogImpl::Impl3D(*this, x, y, w, h, t, bmp);
	else
		_impl = new DialogImpl::Impl(*this, x, y, w, h, t, bmp);
}

_Dialog::_Dialog(int w, int h, const char* t, BITMAP* bmp)
		:
		_Manager(bmp)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new DialogImpl::Impl3D(*this, w, h, t, bmp);
	else
		_impl = new DialogImpl::Impl(*this, w, h, t, bmp);
}

_Dialog::~_Dialog()
{
	delete _impl;
}

int _Dialog::bottom() const
{
	return _impl->bottom();
}

int _Dialog::bottom_controls() const
{
	return _impl->bottom_controls();
}

void _Dialog::bottom_controls(int bc)
{
	_impl->bottom_controls(bc);
}

std::pair<_Dialog::IDLE, void*> _Dialog::callback() const
{
	return _impl->callback();
}

void _Dialog::callback(IDLE f, void* d)
{
	_impl->callback(f, d);
}

void _Dialog::close()
{
	_Manager::abort();
}

void _Dialog::execute()
{
	_Manager::reset();
	_impl->execute();
}

void _Dialog::frame(BITMAP* bmp, int l, int t, int r, int b, int w, int h)
{
	return _impl->frame(bmp, l, t, r, b, w, h);
}

int _Dialog::height() const
{
	return _impl->height();
}

int _Dialog::height_controls() const
{
	return _impl->height_controls();
}

int _Dialog::left() const
{
	return _impl->left();
}

int _Dialog::left_controls() const
{
	return _impl->left_controls();
}

void _Dialog::left_controls(int lc)
{
	_impl->left_controls(lc);
}

int _Dialog::right() const
{
	return _impl->right();
}

int _Dialog::right_controls() const
{
	return _impl->right_controls();
}

void _Dialog::right_controls(int rc)
{
	_impl->right_controls(rc);
}

const char* _Dialog::title() const
{
	return _impl->title();
}

void _Dialog::title(const char* t)
{
	_impl->title(t);
}

Title& _Dialog::title_object() const
{
	return _impl->title_object();
}

int _Dialog::top() const
{
	return _impl->top();
}

int _Dialog::top_controls() const
{
	return _impl->top_controls();
}

void _Dialog::top_controls(int tc)
{
	_impl->top_controls(tc);
}

void _Dialog::uninit()
{
}

int _Dialog::width() const
{
	return _impl->width();
}

int _Dialog::width_controls() const
{
	return _impl->width_controls();
}
