#include "precompiled.hpp"
#include "yacg/Radio.hpp"
#include "yacg/Box.hpp"
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
	namespace RadioImpl
	{
//=============================================================================
// class GroupImpl declaration
//=============================================================================

		class GroupImpl
		{
		public:
			typedef std::vector<Radio*> container;
			typedef container::iterator iterator;
			typedef container::const_iterator const_iterator;
			
		public:
			GroupImpl(_Manager& m, int x, int y, int w, int h, const char* t, int f);
			~GroupImpl();
			
			RadioGroup::iterator begin();
			RadioGroup::const_iterator begin() const;
			
			int bottom() const;
			
			bool empty() const;
			
			RadioGroup::iterator end();
			RadioGroup::const_iterator end() const;
			
			void erase(Radio& r);
			
			RadioGroup::iterator insert(RadioGroup::iterator i, Radio& r);
			std::pair<RadioGroup::iterator, RadioGroup::iterator> insert(
					RadioGroup::iterator i, int c, const char** t);
			std::pair<RadioGroup::iterator, RadioGroup::iterator> insert(
					RadioGroup::iterator i, const std::vector<std::string>& t);
			std::pair<RadioGroup::iterator, RadioGroup::iterator> insert(
					RadioGroup::iterator i, int c, Radio* r);
			std::pair<RadioGroup::iterator, RadioGroup::iterator> insert(
					RadioGroup::iterator i, const std::vector<Radio*>& r);
			
			int left() const;
			
			int right() const;
			
			int size() const;
			
			int top() const;
			
			const char* title() const;
			
			void title(const char* t);

		public:
#ifdef	_DEBUG
			void dump() const;
#endif
		private:
			int _left;
			int _right;
			
			int _top;
			int _bottom;
			
			int _width;
			int _height;
			
			container _controls;
			
			std::auto_ptr<Box> _frame;
			
			std::string _title;
			
			bool _ownControls;			// true if we create the controls (and thus must destroy)
		};

//=============================================================================
// class RadioBase declaration
//=============================================================================

		class RadioBase
		{
		public:
			RadioBase(Radio& o, int x, int y, int w, int h, const char* t, int f);
			
			int bottom() const;
			
			std::pair<Radio::CALLBACK, void*> callback() const;
			void callback(Radio::CALLBACK f, void* d);
			
			int height() const;
			
			int left() const;
			
			bool mouse_down(BITMAP* b, int x, int y, int m);
			
			int right() const;
			
			bool state() const;
			
			void state(bool s);
			
			int top() const;
			
			int width() const;
		
		public:
			virtual void paint(BITMAP* b) = 0;

#ifdef	_DEBUG
			void dump() const;
#endif
		
		protected:
			Radio& _owner;
			
			int _left;
			int _right;
			
			int _top;
			int _bottom;
			
			int _width;
			int _height;
			
			Radio::CALLBACK _callback;
			void* _data;
			
			std::string _title;
			
			bool _state;
		};
		
//=============================================================================
// class RadioImpl declaration
//=============================================================================

		class RadioImpl : public RadioBase
		{
		public:
			RadioImpl(Radio& o, int x, int y, int w, int h, const char* t, int f);
			
			void paint(BITMAP* bmp);
		};

//=============================================================================
// class RadioImpl3D declaration
//=============================================================================

		class RadioImpl3D : public RadioBase
		{
		public:
			RadioImpl3D(Radio& o, int x, int y, int w, int h, const char* t, int f);
			
			void paint(BITMAP* bmp);
		};

//=============================================================================
// class GroupImpl implementation
//=============================================================================

		GroupImpl::GroupImpl(_Manager& m, int x, int y, int w, int h, const char* t,
				int f)
				:
				_left(x),
				_right(Radio::undefined),
				_top(y),
				_bottom(Radio::undefined),
				_width(w),
				_height(h)
		{
			// Save title if provided
			if (t)
				_title = t;
			
			// Create frame if needed
			if ((f & RadioGroup::f_mask) != RadioGroup::f_none)
			{
				// If they provided a position
				if (_left != Radio::undefined && _top != Radio::undefined)
				{
					int flags = Box::i_filled | Box::o_titlecentered;
					switch (f & RadioGroup::f_mask)
					{
					case RadioGroup::f_flat:	flags |= Box::f_flat;	break;
					case RadioGroup::f_in:		flags |= Box::f_in;		break;
					case RadioGroup::f_out:		flags |= Box::f_out;	break;
					}
					if ((f & RadioGroup::t_mask) == RadioGroup::t_title)
						flags |= Box::t_title;
					switch (f & RadioGroup::a_mask)
					{
					case RadioGroup::a_topleft:		flags |= (Box::v_top | Box::h_left);		break;
					case RadioGroup::a_topcenter:	flags |= (Box::v_top | Box::h_centered);	break;
					case RadioGroup::a_topright:	flags |= (Box::v_top | Box::h_right);		break;
					case RadioGroup::a_bottomleft:	flags |= (Box::v_bottom | Box::h_left);		break;
					case RadioGroup::a_bottomcenter:flags |= (Box::v_bottom | Box::h_centered);	break;
					case RadioGroup::a_bottomright:	flags |= (Box::v_bottom | Box::h_right);	break;
					}
					_frame = std::auto_ptr<Box>(new Box(m, _left, _top, _width == Radio::calc ? 100 : _width,
							_height == Radio::calc ? 25 : _height, flags, t));
				}
			}
		}
		
		GroupImpl::~GroupImpl()
		{
			// TODO:
		}
		
		RadioGroup::iterator GroupImpl::begin()
		{
			return _controls.begin();
		}
		
		RadioGroup::const_iterator GroupImpl::begin() const
		{
			return _controls.begin();
		}
		
		int GroupImpl::bottom() const
		{
			return _bottom;
		}
		
#ifdef	_DEBUG
		void GroupImpl::dump() const
		{
		}
#endif

		bool GroupImpl::empty() const
		{
			return _controls.empty();
		}
		
		RadioGroup::iterator GroupImpl::end()
		{
			return _controls.end();
		}
		
		RadioGroup::const_iterator GroupImpl::end() const
		{
			return _controls.end();
		}
		
		void GroupImpl::erase(Radio& r)
		{
			const iterator end = _controls.end();
			iterator i = std::find(_controls.begin(), end, &r);
			if (i != end)
				_controls.erase(i);
		}

		RadioGroup::iterator GroupImpl::insert(RadioGroup::iterator i, Radio& r)
		{
			return _controls.insert(i, &r);
		}

		std::pair<RadioGroup::iterator, RadioGroup::iterator> GroupImpl::insert(
				RadioGroup::iterator i, int c, const char** t)
		{
// TODO:
return std::make_pair(_controls.end(), _controls.end());
		}

		std::pair<RadioGroup::iterator, RadioGroup::iterator> GroupImpl::insert(
				RadioGroup::iterator i, const std::vector<std::string>& t)
		{
// TODO:
return std::make_pair(_controls.end(), _controls.end());
		}

		std::pair<RadioGroup::iterator, RadioGroup::iterator> GroupImpl::insert(
				RadioGroup::iterator i, int c, Radio* r)
		{
// TODO:
return std::make_pair(_controls.end(), _controls.end());
		}

		std::pair<RadioGroup::iterator, RadioGroup::iterator> GroupImpl::insert(
				RadioGroup::iterator i, const std::vector<Radio*>& r)
		{
// TODO:
return std::make_pair(_controls.end(), _controls.end());
		}

		int GroupImpl::left() const
		{
			return _left;
		}
		
		int GroupImpl::right() const
		{
			return _right;
		}
		
		int GroupImpl::size() const
		{
			return numeric_cast<int>(_controls.size());
		}
		
		int GroupImpl::top() const
		{
			return _top;
		}
		
		const char* GroupImpl::title() const
		{
			if (_frame.get())
				return _frame->title();
			else
				return _title.c_str();
		}
		
		void GroupImpl::title(const char* t)
		{
			_title = t;
			_frame->title(t);
		}
		
//=============================================================================
// class RadioBase implementation
//=============================================================================

		RadioBase::RadioBase(Radio& o, int x, int y, int w, int h, const char* t, int f)
				:
				_owner(o)
		{
		}
		
		int RadioBase::bottom() const
		{
			return _bottom;
		}
		
		std::pair<Radio::CALLBACK, void*> RadioBase::callback() const
		{
			return std::make_pair(_callback, _data);
		}
		
		void RadioBase::callback(Radio::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}
		
#ifdef	_DEBUG
		void RadioBase::dump() const
		{
		}
#endif

		int RadioBase::height() const
		{
			return _height;
		}
		
		int RadioBase::left() const
		{
			return _left;
		}
		
		bool RadioBase::mouse_down(BITMAP* b, int x, int y, int m)
		{
			return false;
		}
		
		int RadioBase::right() const
		{
			return _right;
		}
		
		bool RadioBase::state() const
		{
			return _state;
		}
		
		void RadioBase::state(bool s)
		{
			// TODO:
			_state = s;
		}
		
		int RadioBase::top() const
		{
			return _top;
		}
		
		int RadioBase::width() const
		{
			return _width;
		}

//=============================================================================
// class RadioImpl implementation
//=============================================================================

		RadioImpl::RadioImpl(Radio& o, int x, int y, int w, int h, const char* t, int f)
				:
				RadioBase(o, x, y, w, h, t, f)
		{
		}
		
		void RadioImpl::paint(BITMAP* bmp)
		{
		}
		
//=============================================================================
// class RadioImpl3D implementation
//=============================================================================

		RadioImpl3D::RadioImpl3D(Radio& o, int x, int y, int w, int h, const char* t, int f)
				:
				RadioBase(o, x, y, w, h, t, f)
		{
		}

		void RadioImpl3D::paint(BITMAP* bmp)
		{
		}
	}
}

//=============================================================================
// class Radio implementation
//=============================================================================

Radio::Radio(_Manager& m, int x, int y, const char* t, int f)
		:
		_ControlEx(m, f | m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new RadioImpl::RadioImpl3D(*this, x, y, calc, calc, t, f);
	else
		_impl = new RadioImpl::RadioImpl(*this, x, y, calc, calc, t, f);
}

Radio::Radio(_Manager& m, int x, int y, int w, int h, const char* t, int f)
		:
		_ControlEx(m, f | m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new RadioImpl::RadioImpl3D(*this, x, y, w, h, t, f);
	else
		_impl = new RadioImpl::RadioImpl(*this, x, y, w, h, t, f);
}

Radio::~Radio()
{
	delete _impl;
}

int Radio::bottom() const
{
	return _impl->bottom();
}

std::pair<Radio::CALLBACK, void*> Radio::callback() const
{
	return _impl->callback();
}

void Radio::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

#ifdef	_DEBUG
void Radio::dump() const
{
}
#endif

int Radio::height() const
{
	return _impl->height();
}

int Radio::left() const
{
	return _impl->height();
}

bool Radio::mouse_down(BITMAP* b, int x, int y, int m)
{
	return _impl->mouse_down(b, x, y, m);
}

void Radio::paint(BITMAP* b)
{
	_impl->paint(b);
}

int Radio::right() const
{
	return _impl->right();
}

bool Radio::state() const
{
	return _impl->state();
}

void Radio::state(bool s)
{
	_impl->state(s);
}

int Radio::top() const
{
	return _impl->top();
}

int Radio::width() const
{
	return _impl->width();
}

//=============================================================================
// class RadioGroup implementation
//=============================================================================

RadioGroup::RadioGroup(_Manager& m, int x, int y, int w, int h, const char* t, int f)
{
	_impl = new RadioImpl::GroupImpl(m, x, y, w, h, t, f);
}

RadioGroup::~RadioGroup()
{
	delete _impl;
}

RadioGroup::iterator RadioGroup::begin()
{
	return _impl->begin();
}

RadioGroup::const_iterator RadioGroup::begin() const
{
	return _impl->begin();
}

int RadioGroup::bottom() const
{
	return _impl->bottom();
}

#ifdef	_DEBUG
void RadioGroup::dump() const
{
}
#endif

bool RadioGroup::empty() const
{
	return _impl->empty();
}

RadioGroup::iterator RadioGroup::end()
{
	return _impl->end();
}

RadioGroup::const_iterator RadioGroup::end() const
{
	return _impl->end();
}

void RadioGroup::erase(Radio& r)
{
	_impl->erase(r);
}

RadioGroup::iterator RadioGroup::insert(Radio& r)
{
	return insert(end(), r);
}

RadioGroup::iterator RadioGroup::insert(iterator i, Radio& r)
{
	return _impl->insert(i, r);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(int c,
		const char** t)
{
	return insert(end(), c, t);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(iterator i,
		int c, const char** t)
{
	return _impl->insert(i, c, t);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(
		const std::vector<std::string>& t)
{
	return insert(end(), t);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(iterator i,
		const std::vector<std::string>& t)
{
	return _impl->insert(i, t);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(int c,
		Radio* r)
{
	return insert(end(), c, r);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(iterator i,
		int c, Radio* r)
{
	return _impl->insert(i, c, r);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(
		const std::vector<Radio*>& r)
{
	return insert(end(), r);
}

std::pair<RadioGroup::iterator, RadioGroup::iterator> RadioGroup::insert(iterator i,
		const std::vector<Radio*>& r)
{
	return _impl->insert(i, r);
}

int RadioGroup::left() const
{
	return _impl->left();
}

int RadioGroup::right() const
{
	return _impl->right();
}

int RadioGroup::size() const
{
	return _impl->size();
}

int RadioGroup::top() const
{
	return _impl->top();
}

const char* RadioGroup::title() const
{
	return _impl->title();
}

void RadioGroup::title(const char* t)
{
	_impl->title(t);
}
