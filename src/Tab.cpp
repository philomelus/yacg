#include "precompiled.hpp"
#include "yacg/Tab.hpp"
#include "TabImpl.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local objects
//=============================================================================

namespace
{
#ifdef	_DEBUG
	void dump_pages(const TabPage* p)
	{
		p->dump();
	}
#endif
}

namespace yacg
{
	namespace TabImpl
	{
//=============================================================================
// class TabBase implementation
//=============================================================================

		TabBase::TabBase(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				_owner(o),
				_left(l),
				_right(l + w - 1),
				_top(t),
				_bottom(t + h - 1),
				_width(w),
				_height(h),
				_tabChanged(0),
				_selected(0),
				_insertingPages(false)
		{
			// Create title and initialize top
			const int flags = _owner.flags();
			if ((flags & Tab::t_mask) == Tab::t_title)
			{
				_title = std::auto_ptr<Title>(new Title(_owner.manager(), _left,
						_top, _width, Title::calc, Title::o_no_resize_v, title));
				_topTab = _title->bottom() + 1;
			}
			else
				_topTab = _top;
		}
		
		TabBase::~TabBase()
		{
		}
		
		void TabBase::active_changed(bool o, bool n)
		{
		}
		
		Tab::c_iterator TabBase::beginc()
		{
			return _controls.begin();
		}
		
		Tab::c_const_iterator TabBase::beginc() const
		{
			return _controls.begin();
		}
		
		Tab::p_iterator TabBase::beginp()
		{
			return _pages.begin();
		}
		
		Tab::p_const_iterator TabBase::beginp() const
		{
			return _pages.begin();
		}
		
		int TabBase::bottom() const
		{
			return _bottom;
		}

		int TabBase::bottom_controls() const
		{
			return _bottomTab;
		}

		void TabBase::change_tab(int o, int n)
		{
			// Call callback if provided
			if (_tabChanged)
				_tabChanged(_owner, *_pages[o], *_pages[n], _data);

			// Deactivate old page
			_pages[o]->_impl->deactivate(_owner);
			
			// Activate new page
			_pages[n]->_impl->activate(_owner);

			// Mark all common controls dirty
			std::for_each(_controls.begin(), _controls.end(), control_dirty);
			
			// Save new setting
			_selected = n;
		}

#ifdef	_DEBUG
		void TabBase::dump() const
		{
		}
#endif

		bool TabBase::emptyc() const
		{
			return _controls.empty();
		}

		
		bool TabBase::emptyp() const
		{
			return _pages.empty();
		}

		
		Tab::c_iterator TabBase::endc()
		{
			return _controls.end();
		}

		
		Tab::c_const_iterator TabBase::endc() const
		{
			return _controls.end();
		}

		
		Tab::p_iterator TabBase::endp()
		{
			return _pages.end();
		}

		
		Tab::p_const_iterator TabBase::endp() const
		{
			return _pages.end();
		}

		
		void TabBase::erasec(Tab::c_iterator i)
		{
			if (i != _controls.end())
			{
				_Control* c = *i;
				if (dynamic_cast<_ControlEx*>(c) != 0)
				{
					Tab::c_iterator ii = std::find(_eventControls.begin(), _eventControls.end(),
							reinterpret_cast<_ControlEx*>(c));
					if (ii != _eventControls.end())
						_eventControls.erase(ii);
				}
				_controls.erase(i);
			}
		}
		
		void TabBase::erasep(Tab::p_iterator i)
		{
			_pages.erase(i);
			recalc();
			_owner.dirty(Tab::all);
		}
		
		void TabBase::flags_changed(int o, int n)
		{
			// TODO:
		}
		
		int TabBase::height() const
		{
			return _height;
		}
		
		int TabBase::height_controls() const
		{
			return ((_bottomTab - _topTab) + 1);
		}
		
		void TabBase::initial(int s)
		{
			// Activate new page
			_pages[s]->_impl->activate(_owner);

			// Mark all common controls dirty
			std::for_each(_controls.begin(), _controls.end(), control_dirty);
			
			// Save new setting
			_selected = s;
		}
		
		Tab::c_iterator TabBase::insertc(_Control& c)
		{
			return _controls.insert(_controls.end(), &c);
		}
		
		
		Tab::c_iterator TabBase::insertc(_ControlEx& c)
		{
			_eventControls.insert(_eventControls.end(), &c);
			return _controls.insert(_controls.end(), &c);
		}
		
		Tab::p_iterator TabBase::insertp(Tab::p_iterator i, TabPage& tp)
		{
			Tab::p_iterator ii = _pages.insert(i, &tp);
			if (!_insertingPages)
			{
				recalc();
				_owner.dirty(Tab::all);
			}
			return ii;
		}
		
		int TabBase::left() const
		{
			return _left;
		}
		
		int TabBase::left_controls() const
		{
			return _leftTab;
		}
		
		void TabBase::pages_begin()
		{
			if (!_insertingPages)
				_insertingPages = true;
		}

		void TabBase::pages_end()
		{
			if (_insertingPages)
			{
				_insertingPages = false;
				recalc();
				_owner.dirty(Tab::all);
			}
		}
		
		void TabBase::paint(BITMAP* bmp)
		{
			const int flags = _owner.flags();
			
			// Update title if needed
			if ((flags & Tab::t_mask) == Tab::t_title)
				_title->paint(bmp);
				
			// Let derived paint the tabs and frame
			paint_tabs(bmp);
		}
		
		bool TabBase::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			// If click wasn't within tabs, get out now
			if (!isInTabs(x, y))
				return false;
			
			// Determine which tab it was in
			const int oldSel = _selected;
			int newSel;
			const int count = int(_pages.size());
			int nl;
			int nr;
			int nt;
			int nb;
			for (newSel = 0; newSel < count; ++newSel)
			{
				const tab& t = _tabs[newSel];
				if (x >= t.l && x <= t.r && y >= t.t && y <= t.b)
				{
					nl = t.l;
					nr = t.r;
					nt = t.t;
					nb = t.b;
					break;
				}
			}
			if (newSel == count)
				return false;
			_selected = newSel;

			// Update bitmap
			{
				Update u(bmp);
				paint_tabs(bmp, true, true);
			}
			
			// Track until mouse is released
			int mx = x;
			int my = y;
			bool oldHit = true;
			bool newHit = true;
			const tab& t = _tabs[newSel];
			while ((mouse_b & 1) != 0)
			{
				// If mouse moved
				if (mouse_x != mx || mouse_y != my)
				{
					// Update mouse position
					mx = mouse_x;
					my = mouse_y;
					
					// Determine whether its still in use
					newHit = (mx >= nl && mx <= nr && my >= nt && my <= nb);
					
					// Update bitmap if hit changed
					if (oldHit != newHit)
					{
						if (_selected == oldSel)
							_selected = newSel;
						else
							_selected = oldSel;
						{
							Update u(bmp);
							paint_tabs(bmp, true, newHit);
						}
						oldHit = newHit;
					}
				}
			}
			
			// If tab wasn't changed, get out
			if (_selected == oldSel)
			{
				_pages[_selected]->_impl->dirty();
				_owner.manager().dirty();
				std::for_each(_controls.begin(), _controls.end(), control_dirty);
				{
					Update u(bmp);
					paint_tabs(bmp, true, false);
				}
				return false;
			}
			
			// Repaint entire control
			{
				Update u(bmp);
				paint_tabs(bmp);
			}

			// Change selection to new page
			change_tab(oldSel, newSel);
			
			return true;
		}
		
		int TabBase::right() const
		{
			return _right;
		}
		
		int TabBase::right_controls() const
		{
			return _rightTab;
		}
		
		int TabBase::selected() const
		{
			return _selected;
		}
		
		void TabBase::selected(int s)
		{
			if (_selected != s)
				change_tab(_selected, s);
		}
		
		int TabBase::sizec() const
		{
			return int(_controls.size());
		}
		
		int TabBase::sizep() const
		{
			return int(_pages.size());
		}
		
		std::pair<Tab::TABCHANGED, void*> TabBase::tab_changed() const
		{
			return std::make_pair(_tabChanged, _data);
		}
		
		void TabBase::tab_changed(Tab::TABCHANGED f, void* d)
		{
			_tabChanged = f;
			_data = d;
		}
		
		const char* TabBase::title() const
		{
			return _title->title();
		}
		
		void TabBase::title(const char* t)
		{
			_title->title(t);
		}

		
		Title& TabBase::title_control() const
		{
			return *_title.get();
		}
		
		int TabBase::top() const
		{
			return _top;
		}
		
		int TabBase::top_controls() const
		{
			return _topTab;
		}
		
		int TabBase::width() const
		{
			return _width;
		}
		
		int TabBase::width_controls() const
		{
			return ((_rightTab - _leftTab) + 1);
		}
		
//=============================================================================
// class TabPageImpl declaration
//=============================================================================

		TabPageImpl::TabPageImpl(TabPage& o, const char* t)
				:
				_owner(o),
				_activated(0),
				_deactivated(0),
				_length(0)
		{
			if (t)
				_title = t;
		}

		void TabPageImpl::activate(Tab& t)
		{
			// Execute callback
			if (_activated)
				_activated(t, _owner, _activatedData);

			// Show all controls for this page
			std::for_each(_controls.begin(), _controls.end(), control_show);
		}
		
		std::pair<TabPage::ACTIVATED, void*> TabPageImpl::activated() const
		{
			return std::make_pair(_activated, _activatedData);
		}
		
		void TabPageImpl::activated(TabPage::ACTIVATED f, void* d)
		{
			_activated = f;
			_activatedData = d;
		}

		TabPage::iterator TabPageImpl::begin()
		{
			return _controls.begin();
		}
		
		TabPage::const_iterator TabPageImpl::begin() const
		{
			return _controls.begin();
		}

		void TabPageImpl::deactivate(Tab& t)
		{
			// Hide all controls for this page
			std::for_each(_controls.begin(), _controls.end(), control_hide);
			
			// Execute callback if provided
			if (_deactivated)
				_deactivated(t, _owner, _deactivatedData);
		}
		
		std::pair<TabPage::DEACTIVATED, void*> TabPageImpl::deactivated() const
		{
			return std::make_pair(_deactivated, _deactivatedData);
		}
		
		void TabPageImpl::deactivated(TabPage::DEACTIVATED f, void* d)
		{
			_deactivated = f;
			_deactivatedData = d;
		}

		void TabPageImpl::dirty()
		{
			std::for_each(_controls.begin(), _controls.end(), control_dirty);
		}
		
#ifdef	_DEBUG
		void TabPageImpl::dump() const
		{
		}
#endif

		bool TabPageImpl::empty() const
		{
			return _controls.empty();
		}

		TabPage::iterator TabPageImpl::end()
		{
			return _controls.end();
		}
		
		TabPage::const_iterator TabPageImpl::end() const
		{
			return _controls.end();
		}

		void TabPageImpl::erase(TabPage::iterator i)
		{
			if (i != _controls.end())
			{
				_Control* c = *i;
				if (dynamic_cast<_ControlEx*>(c) != 0)
				{
					TabPage::iterator ii = std::find(_eventControls.begin(),
							_eventControls.end(), reinterpret_cast<_ControlEx*>(c));
					if (ii != _eventControls.end())
						_eventControls.erase(ii);
				}
				_controls.erase(i);
			}
		}

		TabPage::iterator TabPageImpl::insert(TabPage::iterator i, _Control& c)
		{
			return _controls.insert(i, &c);
		}
		
		TabPage::iterator TabPageImpl::insert(TabPage::iterator i, _ControlEx& c)
		{
			_eventControls.insert(_eventControls.end(), &c);
			return _controls.insert(i, &c);
		}

		int TabPageImpl::length() const
		{
			return _length;
		}
		
		void TabPageImpl::length(int l)
		{
			if (_length != l)
			{
				_length = l;
				// TODO:  Notify parent of tab length change
			}
		}
		
		void TabPageImpl::paint(BITMAP* bmp)
		{
		}

		bool TabPageImpl::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			return false;
		}
		
		int TabPageImpl::size() const
		{
			return int(_controls.size());
		}

		const char* TabPageImpl::title() const
		{
			return _title.c_str();
		}
		
		void TabPageImpl::title(const char* t)
		{
			_title = t;
			// TODO:  Handle different title width types
		}

		const std::string& TabPageImpl::title_string() const
		{
			return _title;
		}
	}
}

//=============================================================================
// class Tab implementation
//=============================================================================

Tab::Tab(_Manager& m, int l, int t, int w, int h, int f, const char* title)
		:
		_ControlEx(m, f | m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
	{
		switch (f & l_mask)
		{
		case l_bottom:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabBottomVert3D(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabBottomHoriz3D(*this, l, t, w, h, f, title);
			break;

		case l_left:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabLeftVert3D(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabLeftHoriz3D(*this, l, t, w, h, f, title);
			break;

		case l_right:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabRightVert3D(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabRightHoriz3D(*this, l, t, w, h, f, title);
			break;

		case l_top:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabTopVert3D(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabTopHoriz3D(*this, l, t, w, h, f, title);
			break;
		}
	}
	else
	{
		switch (f & l_mask)
		{
		case l_bottom:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabBottomVert(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabBottomHoriz(*this, l, t, w, h, f, title);
			break;

		case l_left:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabLeftVert(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabLeftHoriz(*this, l, t, w, h, f, title);
			break;

		case l_right:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabRightVert(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabRightHoriz(*this, l, t, w, h, f, title);
			break;

		case l_top:
			if ((f & o_mask) == o_vertical)
				_impl = new TabImpl::TabTopVert(*this, l, t, w, h, f, title);
			else
				_impl = new TabImpl::TabTopHoriz(*this, l, t, w, h, f, title);
			break;
		}
	}
}

Tab::~Tab()
{
	delete _impl;
}

void Tab::active_changed(bool o, bool n)
{
	_impl->active_changed(o, n);
}

Tab::c_iterator Tab::beginc()
{
	return _impl->beginc();
}

Tab::c_const_iterator Tab::beginc() const
{
	return _impl->beginc();
}

Tab::p_iterator Tab::beginp()
{
	return _impl->beginp();
}

Tab::p_const_iterator Tab::beginp() const
{
	return _impl->beginp();
}

int Tab::bottom() const
{
	return _impl->bottom();
}

int Tab::bottom_controls() const
{
	return _impl->bottom_controls();
}
			
#ifdef	_DEBUG
void Tab::dump() const
{
	TRACE("gui::Tab @ %08X\n", this);
	TRACE("Flags:");
	const int f = flags();
	switch (f & o_mask)
	{
	case o_vertical:
		TRACE(" o_vertical");
		break;
	
	case o_horizontal:
		TRACE(" o_horizontal");
		break;
	}
	switch (f & l_mask)
	{
	case l_left:
		if ((f & o_mask) == o_horizontal)
			TRACE(" | l_top");
		else
			TRACE(" | l_left");
		break;
	
	case l_right:
		if ((f & o_mask) == o_horizontal)
			TRACE(" | l_bottom");
		else
			TRACE(" | l_right");
		break;
	}
	switch (f & a_mask)
	{
	case a_left:
		if ((f & o_mask) == o_horizontal)
			TRACE(" | a_left");
		else
			TRACE(" | a_top");
		break;
	
	case a_right:
		if ((f & o_mask) == o_horizontal)
			TRACE(" | a_right");
		else
			TRACE(" | a_bottom");
		break;
	}
	switch (f & w_mask)
	{
	case w_equal:
		TRACE(" | w_equal");
		break;
	
	case w_title:
		TRACE(" | w_title");
		break;
	
	case w_page:
		TRACE(" | w_page");
		break;
	}
	switch (f & t_mask)
	{
	case t_title:
		TRACE(" | t_title\n");
		break;
	
	case t_notitle:
		TRACE(" | t_notitle\n");
		break;
	}
	_impl->dump();
	TRACE("Common Controls: %d\n", sizec());
	if (!emptyc())
	{
		TRACE(">>>>>>>>>>\n");
		std::for_each(beginc(), endc(), control_dump);
		TRACE("<<<<<<<<<<\n");
	}
	TRACE("Tab Pages: %d\n", sizep());
	if (!emptyp())
	{
		TRACE(">>>>>>>>>>\n");
		std::for_each(beginp(), endp(), dump_pages);
		TRACE("<<<<<<<<<<\n");
	}
}
#endif

bool Tab::emptyc() const
{
	return _impl->emptyc();
}

bool Tab::emptyp() const
{
	return _impl->emptyp();
}

Tab::c_iterator Tab::endc()
{
	return _impl->endc();
}

Tab::c_const_iterator Tab::endc() const
{
	return _impl->endc();
}

Tab::p_iterator Tab::endp()
{
	return _impl->endp();
}

Tab::p_const_iterator Tab::endp() const
{
	return _impl->endp();
}

void Tab::erasec(c_iterator i)
{
	_impl->erasec(i);
}

void Tab::erasep(p_iterator i)
{
	_impl->erasep(i);
}

void Tab::flags_changed(int o, int n)
{
	_impl->flags_changed(o, n);
}

int Tab::height() const
{
	return _impl->height();
}

int Tab::height_controls() const
{
	return _impl->height_controls();
}

void Tab::initial(int s)
{
	_impl->initial(s);
}

Tab::c_iterator Tab::insertc(_Control& c)
{
	return _impl->insertc(c);
}

Tab::c_iterator Tab::insertc(_ControlEx& c)
{
	return _impl->insertc(c);
}

Tab::p_iterator Tab::insertp(TabPage& tp)
{
	return _impl->insertp(_impl->endp(), tp);
}

Tab::p_iterator Tab::insertp(p_iterator i, TabPage& tp)
{
	return _impl->insertp(i, tp);
}

int Tab::left() const
{
	return _impl->left();
}

int Tab::left_controls() const
{
	return _impl->left_controls();
}

void Tab::pages_begin()
{
	_impl->pages_begin();
}

void Tab::pages_end()
{
	_impl->pages_end();
}

void Tab::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

bool Tab::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	return _impl->mouse_down(bmp, x, y, m);
}

int Tab::right() const
{
	return _impl->right();
}

int Tab::right_controls() const
{
	return _impl->right_controls();
}

int Tab::selected() const
{
	return _impl->selected();
}

void Tab::selected(int s)
{
	_impl->selected(s);
}

int Tab::sizec() const
{
	return _impl->sizec();
}

int Tab::sizep() const
{
	return _impl->sizep();
}

std::pair<Tab::TABCHANGED, void*> Tab::tab_changed() const
{
	return _impl->tab_changed();
}

void Tab::tab_changed(TABCHANGED f, void* d)
{
	_impl->tab_changed(f, d);
}

const char* Tab::title() const
{
	return _impl->title();
}

void Tab::title(const char* t)
{
	_impl->title(t);
}

Title& Tab::title_control() const
{
	return _impl->title_control();
}

int Tab::top() const
{
	return _impl->top();
}

int Tab::top_controls() const
{
	return _impl->top_controls();
}

int Tab::width() const
{
	return _impl->width();
}

int Tab::width_controls() const
{
	return _impl->width_controls();
}

//=============================================================================
// class TabPage implementation
//=============================================================================

TabPage::TabPage(Tab& o, const char* t)
{
	_impl = new TabImpl::TabPageImpl(*this, t);
}

TabPage::~TabPage()
{
	delete _impl;
}

std::pair<TabPage::ACTIVATED, void*> TabPage::activated() const
{
	return _impl->activated();
}

void TabPage::activated(ACTIVATED f, void* d)
{
	_impl->activated();
}

TabPage::iterator TabPage::begin()
{
	return _impl->begin();
}

TabPage::const_iterator TabPage::begin() const
{
	return _impl->begin();
}

std::pair<TabPage::DEACTIVATED, void*> TabPage::deactivated() const
{
	return _impl->deactivated();
}

void TabPage::deactivated(DEACTIVATED f, void* d)
{
	_impl->deactivated(f, d);
}

#ifdef	_DEBUG
void TabPage::dump() const
{
	TRACE("gui::TabPage @ %08X\n", this);
	_impl->dump();
}
#endif

bool TabPage::empty() const
{
	return _impl->empty();
}

TabPage::iterator TabPage::end()
{
	return _impl->end();
}

TabPage::const_iterator TabPage::end() const
{
	return _impl->end();
}

void TabPage::erase(iterator i)
{
	_impl->erase(i);
}

TabPage::iterator TabPage::insert(iterator i, _Control& c)
{
	return _impl->insert(i, c);
}

TabPage::iterator TabPage::insert(iterator i, _ControlEx& c)
{
	return _impl->insert(i, c);
}

int TabPage::length() const
{
	return _impl->length();
}

void TabPage::length(int l)
{
	_impl->length(l);
}

void TabPage::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

bool TabPage::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	return _impl->mouse_down(bmp, x, y, m);
}

int TabPage::size() const
{
	return _impl->size();
}

const char* TabPage::title() const
{
	return _impl->title();
}

void TabPage::title(const char* t)
{
	_impl->title(t);
}

const std::string& TabPage::title_string() const
{
	return _impl->title_string();
}
