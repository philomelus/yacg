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
	if (_themeSignal.connected())
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
	_activeHandlers(*this, _active);
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
	_dirtyHandlers(*this, o);
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
	_flagsHandlers(*this);
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

void _Control::attach_theme()
{
	if (!_themeSignal.connected())
		_themeSignal = _theme->connect(bind(theme_signal, this, _1));
}

EVENTID _Control::connect_theme(THEME_EVENT f)
{
	return _themeHandlers.connect(f);
}

void _Control::detach_theme()
{
	if (_themeSignal.connected())
		_themeSignal.disconnect();
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
	_themeHandlers(*this);
}

void _Control::theme_signal(Theme& t)
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
	_visibleHandlers(*this);
}

//-----------------------------------------------------------------------------

EVENTID _Control::connect_post_paint(POST_PAINT_EVENT f)
{
	return _postPaintHandlers.connect(f);
}

EVENTID _Control::connect_pre_paint(PRE_PAINT_EVENT f)
{
	return _prePaintHandlers.connect(f);
}

void _Control::disconnect_post_paint(EVENTID e)
{
	if (e.connected())
		e.disconnect();
}

void _Control::disconnect_pre_paint(EVENTID e)
{
	if (e.connected())
		e.disconnect();
}

void _Control::paint(BITMAP* bmp)
{
	pre_paint(bmp);
	paint_control(bmp, _dirty);
	post_paint(bmp);
}

void _Control::post_paint(BITMAP* bmp)
{
	_postPaintHandlers(*this, bmp);
}

void _Control::pre_paint(BITMAP* bmp)
{
	_prePaintHandlers(*this, bmp);
}

//-----------------------------------------------------------------------------

#ifdef	_DEBUG
void _Control::dump_control(const std::string& i) const
{
	static const flags_dump _flags_[] =
	{
		{ auto_delete,	"auto_delete",	auto_delete },
		{ 0,			0,				0 }
	};
	
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";

	dump_object(i, "yacg::_Control", this);
	dump_flags(sublevel, _flags_, _flags);
	dump_bool(sublevel, "Active", _active);
	dump_dirty(sublevel, "Active Changed Hint", _activeChangedHint);
	dump_dirty(sublevel, "Dirty", _dirty);
	dump_dirty(sublevel, "Flags Changed Hint", _flagsChangedHint);
	dump_hex(sublevel, "Manager", _manager);
	dump_dirty(sublevel, "Theme Changed Hint", _themeChangedHint);
	dump_hex(sublevel, "Theme", _theme);
	dump_connect(sublevel, "Theme Signal", _themeSignal.connected());
	dump_bool(sublevel, "Visible", _visible);
	dump_dirty(sublevel, "Visible Changed Hint", _visibleChangedHint);
}
#endif

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
	return _mouseDownHandlers(*this, bmp, d_event, x, y, m);
}

void _ControlEx::mouse_down_post(BITMAP* bmp, int x, int y, int m)
{
	_mouseDownHandlers(*this, bmp, d_post, x, y, m);
}

void _ControlEx::mouse_down_pre(BITMAP* bmp, int x, int y, int m)
{
	_mouseDownHandlers(*this, bmp, d_pre, x, y, m);
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
	_mouseMoveHandlers(*this, bmp, m_event, x, y);
}

void _ControlEx::mouse_move_post(BITMAP* bmp, int x, int y)
{
	_mouseMoveHandlers(*this, bmp, m_post, x, y);
}

void _ControlEx::mouse_move_pre(BITMAP* bmp, int x, int y)
{
	_mouseMoveHandlers(*this, bmp, m_pre, x, y);
}

//-----------------------------------------------------------------------------

#ifdef	_DEBUG
void _ControlEx::dump_control(const std::string& i) const
{
#if 0
	TRACE(" _ControlEx Flags:");
	bool prefix = false;
	const int flags = _Control::flags();
	if ((flags & d_mask) == d_none)
		TRACE(" d_none");
	else
	{
		prefix = dump_flag("d_event", flags, d_event, prefix);
		prefix = dump_flag("d_pre", flags, d_pre, prefix);
		prefix = dump_flag("d_post", flags, d_post, prefix);
	}
	if ((flags & d_mask) == d_none)
	{
		if (prefix)
			TRACE(" | m_none");
		else
			TRACE(" m_none");
	}
	else
	{
		prefix = dump_flag("m_event", flags, m_event, prefix);
		prefix = dump_flag("m_pre", flags, m_pre, prefix);
		prefix = dump_flag("m_post", flags, m_post, prefix);
	}
	TRACE("\n");
#endif
}
#endif

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
	// No more mouse down controls
	_mouseDownControls.clear();
	
	// Clean up controls
	container c;
	try
	{
		c = _controls;
		_controls.clear();
		std::for_each(c.begin(), c.end(), bind(delete_control, this, _1));
	}
	catch (...)
	{
		std::for_each(_controls.begin(), _controls.end(), bind(delete_control, this, _1));
		_controls.clear();
	}
}

void _Manager::delete_control(control_info* ci)
{
	if (ci->mouseDownPre.connected())
		ci->mouseDownPre.disconnect();

	if (ci->mouseDownPost.connected())
		ci->mouseDownPost.disconnect();

	if (ci->mouseMovePre.connected())
		ci->mouseMovePre.disconnect();

	if (ci->mouseMove.connected())
		ci->mouseMove.disconnect();

	if (ci->mouseMovePost.connected())
		ci->mouseMovePost.disconnect();

	if ((ci->control->flags() & _Control::auto_delete) != 0)
		delete ci->control;
	
	delete ci;
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
		BITMAP* old = _bitmap;
		_bitmap = b;
		std::for_each(_controls.begin(), _controls.end(), bind(update_bitmap, this, _1));
		bitmap_changed(old);
	}
}

void _Manager::bitmap_changed(BITMAP* o)
{
	_bitmapChangedHandlers(*this, o);
}

EVENTID _Manager::connect_bitmap_changed(BITMAP_CHANGED_EVENT f)
{
	return _bitmapChangedHandlers.connect(f);
}

void _Manager::disconnect_bitmap_changed(EVENTID i)
{
	if (i.connected())
		i.disconnect();
}

void _Manager::update_bitmap(control_info* c)
{
	c->control->dirty(yacg::_Control::dirty_all);
}

//-----------------------------------------------------------------------------

#ifdef	_DEBUG
void _Manager::dump(const std::string& i) const
{
	std::string level(i);
	level += "    ";
	
	std::string sublevel(i);
	sublevel += "  ";
	
	dump_object(i, "yacg::_Manager", this);
	dump_bitmap(sublevel, "Bitmap", _bitmap);
	dump_bool(sublevel, "Dirty", _dirty);
	TRACE("%sMouse Move: %d,%d\n", sublevel.c_str(), _mouseX, _mouseY);
	dump_bool(sublevel, "Aborted", _aborted);
	dump_hex(sublevel, "Theme", _theme);
	dump_count(sublevel, "Controls", _controls.size());
	{
		dump_divider d1(sublevel, "THEME");
		_theme->dump(level);
	}
	{
		dump_divider d1(sublevel, "CONTROLS");
		std::for_each(_controls.begin(), _controls.end(), bind(dump_controls, this, _1, level));
	}
}
#endif

#ifdef	_DEBUG
void _Manager::dump_controls(control_info* c, const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";

	dump_divider d1(i, "CONTROL");

	dump_connect(sublevel, "Mouse Down", c->mouseDown);
	dump_connect(sublevel, "Mouse Down Pre", c->mouseDownPre.connected());
	dump_connect(sublevel, "Mouse Down Post", c->mouseDownPost.connected());
	dump_connect(sublevel, "Mouse Move", c->mouseMove.connected());
	dump_connect(sublevel, "Mouse Move Pre", c->mouseMovePre.connected());
	dump_connect(sublevel, "Mouse Move Post", c->mouseMovePost.connected());
	
	c->control->dump(sublevel);
}
#endif

//-----------------------------------------------------------------------------

EVENTID _Manager::connect_post_paint(POST_PAINT_EVENT f)
{
	return _postPaintHandlers.connect(f);
}

EVENTID _Manager::connect_pre_paint(PRE_PAINT_EVENT f)
{
	return _prePaintHandlers.connect(f);
}

void _Manager::dirty()
{
	_dirty = true;
}

void _Manager::disconnect_post_paint(EVENTID i)
{
	if (i.connected())
		i.disconnect();
}

void _Manager::disconnect_pre_paint(EVENTID i)
{
	if (i.connected())
		i.disconnect();
}

void _Manager::paint()
{
	// Fiull paint regardless of status
	{
		Update u(_bitmap);
		pre_paint();
		std::for_each(_controls.begin(), _controls.end(), bind(painter, this, _1));
		post_paint();
	}
	_dirty = false;
}

void _Manager::paint(_Control& c)
{
	// Full paint regardless of status
	Update u(_bitmap);
	pre_paint();
	c.dirty(_Control::dirty_all);
	c.paint(_bitmap);
	c.clean();
	post_paint();
}

void _Manager::painter(control_info* c)
{
	_Control* cc = c->control;
	if (cc->visible())
	{
		// Force everything dirty for full paint
		cc->dirty(_Control::dirty_all);
		
		// Paint the control
		cc->paint(_bitmap);
		
		// Mark control clean
		cc->clean();
	}
}

void _Manager::post_paint()
{
	_postPaintHandlers(*this);
}

void _Manager::pre_paint()
{
	_prePaintHandlers(*this);
}

void _Manager::update()
{
	// Paint only controls that need it and want it
	{
		Update u(_bitmap);
		pre_paint();
		std::for_each(_controls.begin(), _controls.end(), bind(updater, this, _1));
		post_paint();
	}
	_dirty = false;
}

void _Manager::update(_Control& c)
{
	// Paint control if it needs it and wants it
	if (c.visible() && (c.dirty() & _Control::update_display) != 0)
	{
		Update u(_bitmap);
		pre_paint();
		c.paint(_bitmap);
		post_paint();
	}
}

void _Manager::updater(control_info* c)
{
	// Paint control if needed and wanted
	_Control* cc = c->control;
	if (cc->visible() && (cc->dirty() & _Control::update_display) != 0)
	{
		cc->paint(_bitmap);
		cc->clean();
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

void _Manager::mouse_down(int x, int y, int b)
{
	_mouseDownHandlers(*this, x, y, b);
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

void _Manager::mouse_move(int x, int y)
{
	_mouseMoveHandlers(*this, x, y);
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
			s(*this);
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
		mouse_move(mx, my);
		
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
		mouse_down(mx, my, mb);
		
		// Generate control specific pre event
		_mouseDownControlsPre(_bitmap, mx, my, mb);
		
		// Generate control specific event
		_ControlEx* c;
		const std::vector<_ControlEx*>::iterator end = _mouseDownControls.end();
		for (std::vector<_ControlEx*>::iterator i = _mouseDownControls.begin(); i != end; ++i)
		{
			c = *i;
			
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
	container::const_iterator it = _controls.begin();
	std::advance(it, i);
	return *(*it)->control;
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

EVENTID _Manager::connect_controls_changed(CONTROLS_CHANGED_EVENT f)
{
	return _controlsChangedHandlers.connect(f);
}

void _Manager::controls_changed()
{
	_controlsChangedHandlers(*this);
}

void _Manager::disconnect_controls_changed(EVENTID i)
{
	if (i.connected())
		i.disconnect();
}

void _Manager::erase(_Control& c)
{
	const container::iterator end = _controls.end();
	for (container::iterator i = _controls.begin(); i != end; ++i)
	{
		if ((*i)->control == &c)
		{
			erase(i);
			return;
		}
	}
}

void _Manager::erase(iterator i)
{
	control_info* ci = i.container::iterator::operator*();
	
	// Disconnect from mouse down events
	if (ci->mouseDown)
	{
		events::iterator ei = std::find(_mouseDownControls.begin(),
				_mouseDownControls.end(), ci->control);
		ASSERT(ei != _mouseDownControls.end());
		_mouseDownControls.erase(ei);
	}
	
	// Disconnect from mouse down pre events
	if (ci->mouseDownPre.connected())
		ci->mouseDownPre.disconnect();
	
	// Disconnect from mouse down post events
	if (ci->mouseDownPost.connected())
		ci->mouseDownPost.disconnect();
	
	// Disconnect from mouse move pre events
	if (ci->mouseMovePre.connected())
		ci->mouseMovePre.disconnect();
	
	// Disconnect from mouse move events
	if (ci->mouseMove.connected())
		ci->mouseMove.disconnect();
	
	// Disconnect from mouse move post events
	if (ci->mouseMovePost.connected())
		ci->mouseMovePost.disconnect();
	
	// Remove from container
	_controls.erase(i);
	
	// Generate changed event
	controls_changed();
}

_Manager::iterator _Manager::insert(_Control& c)
{
	// Allocate control info
	std::auto_ptr<control_info> ci(new control_info(c));
	
	// Get control's flags locally
	const int flags = c.flags();

	iterator i;
	try
	{
		// Does it desire mouse down pre event?
		if ((flags & _ControlEx::d_pre) != 0)
		{
			ci->mouseDownPre = _mouseDownControlsPre.connect(bind(&_ControlEx::mouse_down_pre,
					static_cast<_ControlEx*>(&c), _1, _2, _3, _4));
		}

		// Does it desire mouse down post event?
		if ((flags & _ControlEx::d_post) != 0)
		{
			ci->mouseDownPost = _mouseDownControlsPost.connect(bind(&_ControlEx::mouse_down_post,
					static_cast<_ControlEx*>(&c), _1, _2, _3, _4));
		}
		
		// Does it desire mouse move event?
		if ((flags & _ControlEx::m_event) != 0)
		{
			ci->mouseMove = _mouseMoveControls.connect(bind(&_ControlEx::mouse_move,
					static_cast<_ControlEx*>(&c), _1, _2, _3));
		}

		// Does it desire mouse move pre event?
		if ((flags & _ControlEx::m_pre) != 0)
		{
			ci->mouseMovePre = _mouseMoveControlsPre.connect(bind(&_ControlEx::mouse_move_pre,
					static_cast<_ControlEx*>(&c), _1, _2, _3));
		}

		// Does it desire mouse move post event?
		if ((flags & _ControlEx::m_post) != 0)
		{
			ci->mouseMovePost = _mouseMoveControlsPost.connect(bind(&_ControlEx::mouse_move_post,
					static_cast<_ControlEx*>(&c), _1, _2, _3));
		}

		// Does it desire mouse down event?
		if ((flags & _ControlEx::d_event) != 0)
		{
			_mouseDownControls.insert(_mouseDownControls.end(), static_cast<_ControlEx*>(&c));
			ci->mouseDown = true;
		}

		// Insert into control list
		i = _controls.insert(_controls.end(), ci.get());
	}
	catch (...)
	{
		// NOTE:  I know I know,, but I really couldn't figure out a better way
		// to maintain state than this.  My assumption is that boost::signals::connection
		// cannot be copied, and with that assumption, I could figure out a better way
		// to maintain the integrity of this object than this.  It sucks, but it does
		// work, so either fix it or live with it.  If it had required nested try/catch,
		// I WOULD have figured out a different way.
		
		// Disconnect from mouse down events
		if (ci->mouseDown)
		{
			events::iterator i = std::find(_mouseDownControls.begin(),
					_mouseDownControls.end(), &c);
			ASSERT(i != _mouseDownControls.end());
			_mouseDownControls.erase(i);
		}

		// Disconnect from mouse down pre events
		if (ci->mouseDownPre.connected())
			ci->mouseDownPre.disconnect();

		// Disconnect from mouse down post events
		if (ci->mouseDownPost.connected())
			ci->mouseDownPost.disconnect();

		// Disconnect from mouse move pre events
		if (ci->mouseMovePre.connected())
			ci->mouseMovePre.disconnect();

		// Disconnect from mouse move events
		if (ci->mouseMove.connected())
			ci->mouseMove.disconnect();

		// Disconnect from mouse move post events
		if (ci->mouseMovePost.connected())
			ci->mouseMovePost.disconnect();
		
		// Pass exception on
		throw;
	}
	
	// Ratain ownership of control info
	ci.release();

	// Generate changed event
	controls_changed();
	
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

void _Manager::theme_updater(control_info* c)
{
	c->control->theme(*_theme);
}

//=============================================================================
// class _Manager::iterator implementation
//=============================================================================

_Manager::iterator::iterator()
{
}

_Manager::iterator::iterator(const iterator& r)
		:
		container::iterator(r)
{
}

_Manager::iterator::iterator(const container::iterator& r)
		:
		container::iterator(r)
{
}

_Manager::iterator& _Manager::iterator::operator=(const iterator& r)
{
	if (this != &r)
		container::iterator::operator=(r);
	return *this;
}

_Control& _Manager::iterator::operator*() const
{
	return *(container::iterator::operator*()->control);
}

_Control* _Manager::iterator::operator->() const
{
	return container::iterator::operator*()->control;
}

//=============================================================================
// class _Manager::const_iterator implementation
//=============================================================================

_Manager::const_iterator::const_iterator()
{
}

_Manager::const_iterator::const_iterator(const const_iterator& r)
		:
		container::const_iterator(r)
{
}

_Manager::const_iterator::const_iterator(const container::const_iterator& r)
		:
		container::const_iterator(r)
{
}

_Manager::const_iterator::const_iterator(const iterator& r)
		:
		container::const_iterator(r)
{
}

_Manager::const_iterator& _Manager::const_iterator::operator=(const const_iterator& r)
{
	if (this != &r)
		container::const_iterator::operator=(r);
	return *this;
}

const _Control& _Manager::const_iterator::operator*() const
{
	return *(container::const_iterator::operator*()->control);
}

const _Control* _Manager::const_iterator::operator->() const
{
	return container::const_iterator::operator*()->control;
}

//=============================================================================
// class _Manager::contol_info implementation
//=============================================================================

_Manager::control_info::control_info(_Control& c)
		:
		mouseDown(false),
		control(&c)
{
}
