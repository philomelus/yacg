#include "precompiled.hpp"
#include "yacg.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

namespace
{
}

//=============================================================================
// class _Control implementation
//=============================================================================

_Control::_Control(_Manager& m, int f)
		:
		_manager(&m),
		_flags(f),
		_dirty(dirty_all),
		_active(true),
		_visible(true),
		_theme(&(m.theme())),
		_activeChangedHint(dirty_all),
		_flagsChangedHint(dirty_all),
		_themeChangedHint(dirty_all),
		_visibleChangedHint(dirty_all)
{
	_themeSignal = _theme->connect(bind(theme_signal, this, _1));
}

_Control::~_Control()
{
	_themeSignal.disconnect();
}

//-----------------------------------------------------------------------------

bool _Control::active() const
{
	return _active;
}

void _Control::active(bool a)
{
	// Is active state changing?
	if (_active != a)
	{
		// Yes, so update it
		_active = a;
		
		// Notify active state logic
		active_changed();
	}
}

void _Control::active_changed()
{
	// Mark us dirty
	dirty(_activeChangedHint);
	
	// Generate event
	_activeHandlers(this, _active);
}

EVENTID _Control::connect_active(ACTIVE_EVENT f)
{
	return _activeHandlers.connect(f);
}

void _Control::disconnect_active(EVENTID e)
{
	if (e.connected())
		e.disconnect();
}

//-----------------------------------------------------------------------------

void _Control::clean(int o)
{
	// Clears desired dirty bits
	_dirty &= (~o);
}

int _Control::dirty() const
{
	return _dirty;
}

void _Control::dirty(int d)
{
	// Update dirty bits
	const int o = _dirty;
	_dirty |= d;
	
	// If flags actually changed, notify dirty logic
	if (o != _dirty)
		dirty_changed(o);

	// If dirty, let manager know
	if (_dirty)
		_manager->dirty();
}

void _Control::dirty_changed(int o)
{
	// Generate event
	_dirtyHandlers(this, o);
}

EVENTID _Control::connect_dirty(DIRTY_EVENT f)
{
	return _dirtyHandlers.connect(f);
}

void _Control::disconnect_dirty(EVENTID e)
{
	if (e.connected())
		e.disconnect();
}

//-----------------------------------------------------------------------------

int _Control::flags() const
{
	return _flags;
}

void _Control::flags(int f)
{
	// Are flags changing?
	if ((_flags & 0xFFFF0000) != (f & 0xFFFF0000))
	{
		// Update flags
		_flags = (f & 0xFFFF0000) | (_flags & 0xFFFF);
		
		// If its still changed, force a repaint
		if ((_flags & 0xFFFF0000) == (f & 0xFFFF0000))
			dirty(_flagsChangedHint);

		// Let all who care know
		flags_changed();
	}
}

void _Control::flags_changed()
{
	// Generate event
	_flagsHandlers(this);
}

EVENTID _Control::connect_flags(FLAGS_EVENT f)
{
	return _flagsHandlers.connect(f);
}

void _Control::disconnect_flags(EVENTID f)
{
	if (f.connected())
		f.disconnect();
}

//-----------------------------------------------------------------------------

_Manager& _Control::manager() const
{
	return *_manager;
}

void _Control::manager(_Manager& m)
{
	// Make sure original manager no longer uses us
	_manager->erase(*this);
	
	// Save new manager
	_manager = &m;
}

//-----------------------------------------------------------------------------

EVENTID _Control::connect_theme(THEME_EVENT f)
{
	return _themeHandlers.connect(f);
}

void _Control::disconnect_theme(EVENTID f)
{
	if (f.connected())
		f.disconnect();
}

Theme& _Control::theme() const
{
	return *_theme;
}

void _Control::theme(Theme& t)
{
	// Save pointer to theme
	_theme = &t;
	
	// Notify theme logic
	theme_changed();
}

void _Control::theme_changed()
{
	// Mark us dirty
	dirty(_themeChangedHint);
	
	// Generate event
	_themeHandlers(this);
}

void _Control::theme_signal(Theme* t)
{
	theme_changed();
}

//-----------------------------------------------------------------------------

EVENTID _Control::connect_visible(VISIBLE_EVENT f)
{
	return _visibleHandlers.connect(f);
}

void _Control::disconnect_visible(EVENTID f)
{
	if (f.connected())
		f.disconnect();
}

bool _Control::visible() const
{
	return _visible;
}

void _Control::visible(bool v)
{
	// Is visibility changing?
	if (_visible != v)
	{
		// Update visibility
		_visible = v;
		
		// Signal anyone who cares
		visible_changed();
	}
}

void _Control::visible_changed()
{
	// Mark us dirty
	dirty(_visibleChangedHint);
	
	// Generate event
	_visibleHandlers(this);
}

//=============================================================================
// class _ControlEx implementation
//=============================================================================

_ControlEx::_ControlEx(_Manager& m, int f)
		:
		_Control(m, f)
{
}

//-----------------------------------------------------------------------------

EVENTID _ControlEx::connect_mouse_down(MOUSE_DOWN_EVENT f)
{
	return _mouseDownHandlers.connect(f);
}

void _ControlEx::disconnect_mouse_down(EVENTID f)
{
	if (f.connected())
		f.disconnect();
}

bool _ControlEx::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	return _mouseDownHandlers(this, bmp, d_event, x, y, m);
}

void _ControlEx::mouse_down_post(BITMAP* bmp, int x, int y, int m)
{
	_mouseDownHandlers(this, bmp, d_post, x, y, m);
}

void _ControlEx::mouse_down_pre(BITMAP* bmp, int x, int y, int m)
{
	_mouseDownHandlers(this, bmp, d_pre, x, y, m);
}

//-----------------------------------------------------------------------------

EVENTID _ControlEx::connect_mouse_move(MOUSE_MOVE_EVENT f)
{
	return _mouseMoveHandlers.connect(f);
}

void _ControlEx::disconnect_mouse_move(EVENTID f)
{
	if (f.connected())
		f.disconnect();
}

void _ControlEx::mouse_move(BITMAP* bmp, int x, int y)
{
	_mouseMoveHandlers(this, bmp, m_event, x, y);
}

void _ControlEx::mouse_move_post(BITMAP* bmp, int x, int y)
{
	_mouseMoveHandlers(this, bmp, m_post, x, y);
}

void _ControlEx::mouse_move_pre(BITMAP* bmp, int x, int y)
{
	_mouseMoveHandlers(this, bmp, m_pre, x, y);
}

//=============================================================================
// class _Manager implementation
//=============================================================================

_Manager::_Manager(BITMAP* b, Theme& t)
		:
		_bitmap(b),
		_dirty(true),
		_mouseX(0),
		_mouseY(0),
		_aborted(false),
		_theme(&t)
{
}

_Manager::~_Manager()
{
}

//-----------------------------------------------------------------------------

BITMAP* _Manager::bitmap() const
{
	return _bitmap;
}

void _Manager::bitmap(BITMAP* b)
{
	if (_bitmap != b)
	{
		_bitmap = b;
		std::for_each(_controls.begin(), _controls.end(), control_dirty);
	}
}

//-----------------------------------------------------------------------------

#ifdef	_DEBUG
void _Manager::dump() const
{
	// TODO:
	std::for_each(_controls.begin(), _controls.end(), control_dump);
}
#endif

//-----------------------------------------------------------------------------

void _Manager::dirty()
{
	_dirty = true;
}

void _Manager::paint()
{
	{
		Update u(_bitmap);
		std::for_each(_controls.begin(), _controls.end(), bind(painter, this, _1));
	}
	_dirty = false;
}

void _Manager::paint(_Control& c)
{
	c.paint(_bitmap);
}

void _Manager::painter(_Control* c)
{
	if (c->visible())
	{
		c->dirty(_Control::dirty_all);
		c->paint(_bitmap);
		c->clean();
	}
}

void _Manager::update()
{
	{
		Update u(_bitmap);
		std::for_each(_controls.begin(), _controls.end(), bind(updater, this, _1));
	}
	_dirty = false;
}

void _Manager::update(_Control& c)
{
	Update u(_bitmap);
	c.paint(_bitmap);
}

void _Manager::updater(_Control* c)
{
	if (c->visible())
	{
		if (c->dirty() != 0)
		{
			c->paint(_bitmap);
			c->clean();
		}
	}
}

//-----------------------------------------------------------------------------

EVENTID _Manager::connect_mouse_down(MOUSE_DOWN_EVENT f)
{
	return _mouseDownHandlers.connect(f);
}

void _Manager::disconnect_mouse_down(EVENTID i)
{
	if (i.connected())
		i.disconnect();
}

//-----------------------------------------------------------------------------

EVENTID _Manager::connect_mouse_move(MOUSE_MOVE_EVENT f)
{
	return _mouseMoveHandlers.connect(f);
}

void _Manager::disconnect_mouse_move(EVENTID i)
{
	if (i.connected())
		i.disconnect();
}

//-----------------------------------------------------------------------------

void _Manager::abort()
{
	_aborted = true;
}

void _Manager::process_events()
{
	_aborted = false;
	std::pair<bool, bool> result = std::make_pair(true, false);
	while (!result.second)
	{
		if (result.first)
			update();
		else
			rest(1);
		result = process_events_and_return();
	}
}

void _Manager::process_events(IDLE f)
{
	idle_slot s;
	EVENTID id = s.connect(f);
	_aborted = false;
	std::pair<bool, bool> result = std::make_pair(true, false);
	while (!result.second)
	{
		if (result.first)
			update();
		else
			s(this);
		result = process_events_and_return();
	}
	id.disconnect();
}

std::pair<bool, bool> _Manager::process_events_and_return()
{
	// I fmouse requires polling, perform poll now
	if (mouse_needs_poll())
		poll_mouse();
	
	// Process mouse_move event if needed
	int mx = mouse_x;
	int my = mouse_y;
	if (mx != _mouseX || my != _mouseY)
	{
		// Save mouse position
		_mouseX = mx;
		_mouseY = my;

		// Generate global event
		_mouseMoveHandlers(this, mx, my);
		
		// Generate control specific pre-event
		_mouseMoveControlsPre(_bitmap, mx, my);
		
		// Generate control specific event
		_mouseMoveControls(_bitmap, mx, my);
		
		// Generate control specific post-event
		_mouseMoveControlsPost(_bitmap, mx, my);
	}

	// Process mouse down event if needed
	if ((mouse_b & 1) != 0)
	{
		// Remember original mouse click location
		const int mb = mouse_b | 1;	// FORCE button on in case it's been released already
		mx = mouse_x;
		my = mouse_y;
		
		// Generate global event
		_mouseDownHandlers(this, mx, my, mb);
		
		// Generate control specific pre event
		_mouseDownControlsPre(_bitmap, mx, my, mb);
		
		// Generate control specific event
		_ControlEx* c;
		const container::iterator end = _mouseDownControls.end();
		for (container::iterator i = _mouseDownControls.begin(); i != end; ++i)
		{
			c = static_cast<_ControlEx*>(*i);
			
			// If control is visible and active
			if (c->visible() && c->active())
			{
				// If control claims event
				if (c->mouse_down(_bitmap, mx, my, mb))
				{
					// Note that a control is dirty
					_dirty = true;
					
					// No need to check other controls
					break;
				}
			}
		}
		
		// Generate control specific post event
		_mouseDownControlsPost(_bitmap, mx, my, mb);
	}

	return std::make_pair(_dirty, _aborted);
}

void _Manager::reset()
{
	_aborted = false;
}

//-----------------------------------------------------------------------------

_Control& _Manager::operator[](int i) const
{
	return *_controls[i];
}

//-----------------------------------------------------------------------------

_Manager::iterator _Manager::begin()
{
	return _controls.begin();
}

_Manager::const_iterator _Manager::begin() const
{
	return _controls.begin();
}

_Manager::iterator _Manager::end()
{
	return _controls.end();
}

_Manager::const_iterator _Manager::end() const
{
	return _controls.end();
}

//-----------------------------------------------------------------------------

void _Manager::disconnect_control_events(EVENTID& id)
{
	id.disconnect();
}

void _Manager::erase(_Control& c)
{
	// Locater iterator for control
	iterator i = std::find(_controls.begin(), _controls.end(), &c);
	if (i != _controls.end())
		erase(i);
}

void _Manager::erase(iterator i)
{
	// Remove from container
	_controls.erase(i);
	
	// Disconnect all events
	std::for_each((*i)->_connections.begin(), (*i)->_connections.end(),
			bind(disconnect_control_events, this, _1));
}

//-----------------------------------------------------------------------------

_Manager::iterator _Manager::insert(_Control& c)
{
	// TODO:  Major issues with failure in this function.  All kinds of stuff
	// will be left in bad states if anything after the first insertion fails!
	
	// Insert into control list
	iterator i = std::find(_controls.begin(), _controls.end(), &c);
	if (i == _controls.end())
		i = _controls.insert(_controls.end(), &c);

	// Get control's flags locally
	const int flags = c.flags();

	// Does it desire mouse down event?
	if ((flags & _ControlEx::d_event) != 0)
		_mouseDownControls.push_back(&c);

	// Does it desire mouse down pre event?
	if ((flags & _ControlEx::d_pre) != 0)
	{
		EVENTID id = _mouseDownControlsPre.connect(bind(&_ControlEx::mouse_down_pre,
				static_cast<_ControlEx*>(&c), _1, _2, _3, _4));
		c._connections.push_back(id);
	}

	// Does it desire mouse down post event?
	if ((flags & _ControlEx::d_post) != 0)
	{
		EVENTID id = _mouseDownControlsPost.connect(bind(&_ControlEx::mouse_down_post,
				static_cast<_ControlEx*>(&c), _1, _2, _3, _4));
		c._connections.push_back(id);
	}
	
	// Does it desire mouse move event?
	if ((flags & _ControlEx::m_event) != 0)
	{
		EVENTID id = _mouseMoveControls.connect(bind(&_ControlEx::mouse_move,
				static_cast<_ControlEx*>(&c), _1, _2, _3));
		c._connections.push_back(id);
	}

	// Does it desire mouse move pre event?
	if ((flags & _ControlEx::m_pre) != 0)
	{
		EVENTID id = _mouseMoveControlsPre.connect(bind(&_ControlEx::mouse_move_pre,
				static_cast<_ControlEx*>(&c), _1, _2, _3));
		c._connections.push_back(id);
	}

	// Does it desire mouse move post event?
	if ((flags & _ControlEx::m_post) != 0)
	{
		EVENTID id = _mouseMoveControlsPost.connect(bind(&_ControlEx::mouse_move_post,
				static_cast<_ControlEx*>(&c), _1, _2, _3));
		c._connections.push_back(id);
	}

	return i;
}

//-----------------------------------------------------------------------------

int _Manager::size() const
{
	return numeric_cast<int>(_controls.size());
}	

//-----------------------------------------------------------------------------

Theme& _Manager::theme() const
{
	return *_theme;
}

void _Manager::theme(Theme& t)
{
	if (_theme != &t)
	{
		_theme = &t;
		std::for_each(_controls.begin(), _controls.end(), bind(theme_updater,
				this, _1));
	}
}

void _Manager::theme_updater(_Control* c)
{
	c->theme(*_theme);
}
