#include "precompiled.hpp"
#include "yacg/Menu.hpp"
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
	namespace MenuImpl
	{
//=============================================================================
// class MenuBarBase declaration
//=============================================================================

		class MenuBarBase
		{
		public:
			static const int h_space = 3;
			static const int v_space = 2;
			
		public:
			typedef std::list<Menu*> container;
			typedef container::iterator iterator;
			typedef container::const_iterator const_iterator;

		public:
			MenuBarBase(MenuBar& o, int x, int y, int w, int h);
			MenuBarBase(MenuBar& o, int x, int y);
			MenuBarBase(_Window& w, MenuBar& o);
			MenuBarBase(_Dialog& d, MenuBar& o);
			virtual ~MenuBarBase();
			
			void active_changed(bool o, bool n);

			MenuBar::iterator begin();
			MenuBar::const_iterator begin() const;

			int bottom() const;

#ifdef	_DEBUG
			void dump() const;
#endif

			bool empty() const;
			
			MenuBar::iterator end();
			MenuBar::const_iterator end() const;
					
			void erase(MenuBar::iterator i);

			void flags_changed(int o, int n);

			int height() const;
			
			MenuBar::iterator insert(MenuBar::iterator i, Menu& m);
			
			int left() const;

			bool mouse_down(BITMAP* bmp, int x, int y, int m);
			
			void mouse_move(BITMAP* bmp);
			
			void menus_begin();
			
			void menus_end();
			
			void paint(BITMAP* b);

			int right() const;
			
			int size() const;

			int top() const;

			int width() const;

		public:
			virtual void paint_frame(BITMAP* bmp) = 0;
		
		protected:
			MenuBar& _owner;
			
			int _left;
			int _right;
			
			int _top;
			int _bottom;
			
			int _width;
			int _height;
			
			container _menus;
			
			bool _insertingMenus;
			
			bool _calcWidth;
			
			MenuBase* _menuDisplayed;	// When non-zero, if mouse is down, this menu is waiting for a item selection or dismissal
		};
		
//=============================================================================
// class MenuBase declaration
//=============================================================================

		class MenuBase
		{
		public:
			static const int h_space = 3;
			static const int v_space = 2;
			
		public:
			typedef std::vector<MenuItem*> container;
			typedef container::iterator iterator;
			typedef container::const_iterator const_iterator;
			
		public:
			MenuBase(Menu& m, int id, const char* t);
			virtual ~MenuBase();
			
			void active_changed(bool o, bool n);

			Menu::iterator begin();
			Menu::const_iterator begin() const;
			
			int bottom() const;

#ifdef	_DEBUG
			void dump() const;
#endif

			bool empty() const;
			
			Menu::iterator end();
			Menu::const_iterator end() const;
					
			void erase(Menu::iterator i);

			void flags_changed(int o, int n);

			int height() const;
			void height(int h);
			
			int id() const;

			Menu::iterator insert(Menu::iterator i, MenuItem& mi);

			bool inTitle(int x, int y);
			
			bool inMenu(int x, int y);
			
			void items_begin();
			
			void items_end();
			
			int left() const;
			void left(int l);
			
			bool mouse_down(BITMAP* b, int x, int y, int m);

			void restore(BITMAP* bmp);

			int right() const;
			
			void save(BITMAP* bmp);

			void selected(BITMAP* bmp, int x, int y);
			
			int size() const;

			const char* title() const;
			void title(const char* t);
			
			const std::string& title_object() const;
			
			int top() const;
			void top(int t);
			
			int width() const;
			void width(int w);
		
			void paint_menu_items(BITMAP* bmp, int x, int y);
			
		public:
			virtual void paint_menu(BITMAP* b) = 0;
			virtual void paint_title(BITMAP* b) = 0;
			virtual void update_hilite(BITMAP* bmp, int x, int y) = 0;
		
		protected:
			MenuItem* y2mi(int y);
			
		protected:
			Menu& _owner;
			
			int _left;
			int _right;
			
			int _top;
			int _bottom;
			
			int _width;
			int _height;
			
			int _id;

			container _items;
			
			std::string _title;
			
			bool _insertingItems;
			
			int _menuHeight;
			int _menuWidth;
			
			BITMAP* _save;
		};
		
//=============================================================================
// class MenuItemBase declaration
//=============================================================================

		class MenuItemBase
		{
		public:
			MenuItemBase(MenuItem& o, int id, int f);
			MenuItemBase(const MenuItemBase& r);
			
			MenuItemBase& operator=(const MenuItem& i);
			
			virtual ~MenuItemBase();
			
			void check(bool c);

			std::pair<MenuItem::CALLBACK, void*> callback() const;
			void callback(MenuItem::CALLBACK f, void* d);
			
			bool checked() const;

			bool disabled() const;

#ifdef	_DEBUG
			void dump() const;
#endif

			void enable(bool e);

			bool enabled() const;

			int flags() const;
			void flags(int f);

			int height() const;

			int id() const;
			
			bool unchecked() const;
			
			int type() const;
		
			int width() const;

		public:
			virtual BITMAP* bitmap() const;
			virtual void bitmap(BITMAP* bmp);
			
			virtual Menu& menu();
			virtual const Menu& menu() const;
			virtual void menu(Menu& m);
			
			virtual const char* title() const;
			virtual void title(const char* t);
			
			virtual const std::string& title_object() const;

		public:
			virtual void paint(BITMAP* bmp, int x, int y, int w) = 0;
			
		protected:
			MenuItem& _owner;
			
			int _flags;
			int _id;
			
			MenuItem::CALLBACK _callback;
			void* _data;
			
			int _height;
			int _width;
		};

//=============================================================================
// class MenuItemBitmapBase declaration
//=============================================================================

		class MenuItemBitmapBase : public MenuItemBase
		{
		public:
			MenuItemBitmapBase(MenuItem& o, int id, int f, BITMAP* b);
			MenuItemBitmapBase(const MenuItemBitmapBase& r);
			virtual ~MenuItemBitmapBase();
			
			BITMAP* bitmap() const;
			void bitmap(BITMAP* bmp);
			
		protected:
			BITMAP* _bitmap;
		};

//=============================================================================
// class MenuItemSeperatorBase declaration
//=============================================================================

		class MenuItemSeperatorBase : public MenuItemBase
		{
		public:
			MenuItemSeperatorBase(MenuItem& o, int id, int f);
			MenuItemSeperatorBase(const MenuItemSeperatorBase& r);
		};
		
//=============================================================================
// class MenuItemSubMenuBase declaration
//=============================================================================

		class MenuItemSubMenuBase : public MenuItemBase
		{
		public:
			MenuItemSubMenuBase(MenuItem& o, int id, int f, Menu& m);
			MenuItemSubMenuBase(const MenuItemSubMenuBase& r);
			virtual ~MenuItemSubMenuBase();
			
			Menu& menu();
			const Menu& menu() const;
			void menu(Menu& m);

		protected:
			Menu* _menu;
		};
		
//=============================================================================
// class MenuItemTitleBase declaration
//=============================================================================

		class MenuItemTitleBase : public MenuItemBase
		{
		public:
			MenuItemTitleBase(MenuItem& o, int id, int f, const char* t);
			MenuItemTitleBase(const MenuItemTitleBase& r);
			virtual ~MenuItemTitleBase();
			
			const char* title() const;
			void title(const char* t);
			
			const std::string& title_object() const;

		protected:
			std::string _title;
		};
		
//=============================================================================
// class MenuBarImpl declaration
//=============================================================================

		class MenuBarImpl : public MenuBarBase
		{
		public:
			MenuBarImpl(MenuBar& o, int x, int y, int w, int h);
			MenuBarImpl(MenuBar& o, int x, int y);
			MenuBarImpl(_Window& w, MenuBar& o);
			MenuBarImpl(_Dialog& d, MenuBar& o);
		
			void paint_frame(BITMAP* bmp);
		};
		
//=============================================================================
// class MenuBarImpl3D declaration
//=============================================================================

		class MenuBarImpl3D : public MenuBarBase
		{
		public:
			MenuBarImpl3D(MenuBar& o, int x, int y, int w, int h);
			MenuBarImpl3D(MenuBar& o, int x, int y);
			MenuBarImpl3D(_Window& w, MenuBar& o);
			MenuBarImpl3D(_Dialog& d, MenuBar& o);
			
			void paint_frame(BITMAP* bmp);
		};

//=============================================================================
// class MenuImpl declaration
//=============================================================================

		class MenuImpl : public MenuBase
		{
		public:
			MenuImpl(Menu& m, int id, const char* t);

			void paint_menu(BITMAP* b);
			void paint_title(BITMAP* b);
			void update_hilite(BITMAP* bmp, int x, int y);
		};
		
//=============================================================================
// class MenuImpl3D declaration
//=============================================================================

		class MenuImpl3D : public MenuBase
		{
		public:
			MenuImpl3D(Menu& m, int id, const char* t);

			void paint_menu(BITMAP* b);
			void paint_title(BITMAP* b);
			void update_hilite(BITMAP* bmp, int x, int y);
		};
		
//=============================================================================
// class MenuItemBitmapImpl declaration
//=============================================================================

		class MenuItemBitmapImpl : public MenuItemBitmapBase
		{
		public:
			MenuItemBitmapImpl(MenuItem& o, int id, int f, BITMAP* b);
			MenuItemBitmapImpl(const MenuItemBitmapImpl& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};

//=============================================================================
// class MenuItemBitmapImpl3D declaration
//=============================================================================

		class MenuItemBitmapImpl3D : public MenuItemBitmapBase
		{
		public:
			MenuItemBitmapImpl3D(MenuItem& o, int id, int f, BITMAP* b);
			MenuItemBitmapImpl3D(const MenuItemBitmapImpl3D& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};

//=============================================================================
// class MenuItemSeperatorImpl declaration
//=============================================================================

		class MenuItemSeperatorImpl : public MenuItemSeperatorBase
		{
		public:
			MenuItemSeperatorImpl(MenuItem& o, int id, int f);
			MenuItemSeperatorImpl(const MenuItemSeperatorImpl& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};
		
//=============================================================================
// class MenuItemSeperatorImpl3D declaration
//=============================================================================

		class MenuItemSeperatorImpl3D : public MenuItemSeperatorBase
		{
		public:
			MenuItemSeperatorImpl3D(MenuItem& o, int id, int f);
			MenuItemSeperatorImpl3D(const MenuItemSeperatorImpl3D& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};

//=============================================================================
// class MenuItemSubMenuImpl declaration
//=============================================================================

		class MenuItemSubMenuImpl : public MenuItemSubMenuBase
		{
		public:
			MenuItemSubMenuImpl(MenuItem& o, int id, int f, Menu& m);
			MenuItemSubMenuImpl(const MenuItemSubMenuImpl& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};
		
//=============================================================================
// class MenuItemSubMenuImpl3D declaration
//=============================================================================

		class MenuItemSubMenuImpl3D : public MenuItemSubMenuBase
		{
		public:
			MenuItemSubMenuImpl3D(MenuItem& o, int id, int f, Menu& m);
			MenuItemSubMenuImpl3D(const MenuItemSubMenuImpl3D& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};
		
//=============================================================================
// class MenuItemTitleImpl declaration
//=============================================================================

		class MenuItemTitleImpl : public MenuItemTitleBase
		{
		public:
			MenuItemTitleImpl(MenuItem& o, int id, int f, const char* t);
			MenuItemTitleImpl(const MenuItemTitleImpl& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};
		
//=============================================================================
// class MenuItemTitleImpl3D declaration
//=============================================================================

		class MenuItemTitleImpl3D : public MenuItemTitleBase
		{
		public:
			MenuItemTitleImpl3D(MenuItem& o, int id, int f, const char* t);
			MenuItemTitleImpl3D(const MenuItemTitleImpl3D& r);

			void paint(BITMAP* bmp, int x, int y, int w);
		};
		
//=============================================================================
// class MenuBarBase implementation
//=============================================================================

		MenuBarBase::MenuBarBase(MenuBar& o, int x, int y, int w, int h)
				:
				_owner(o),
				_left(x),
				_right(0),
				_top(y),
				_bottom(0),
				_width(w),
				_height(h),
				_insertingMenus(false),
				_calcWidth(false),
				_menuDisplayed(0)
		{
			// Calculate initial width if desired
			if (w == MenuBar::calc)
			{
				_width = 8;
				_calcWidth = true;
			}
			
			// Calculate height if desired
			if (h == MenuBar::calc)
				_height = text_height(font) + 8;

			// Calculate right and bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
		}
		
		MenuBarBase::MenuBarBase(MenuBar& o, int x, int y)
				:
				_owner(o),
				_left(x),
				_right(0),
				_top(y),
				_bottom(0),
				_width(0),
				_height(0),
				_insertingMenus(false),
				_calcWidth(true),
				_menuDisplayed(0)
		{
			// Calculate width and height
			_height = 2 + v_space + text_height(font) + v_space + 2;
			_width = 2 + h_space + h_space + 2;
			
			// Calculate right and bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
		}
		
		MenuBarBase::MenuBarBase(_Window& w, MenuBar& o)
				:
				_owner(o),
				_insertingMenus(false),
				_calcWidth(false),
				_menuDisplayed(0)
		{
			// Calculate width and height
			_height = 2 + v_space + text_height(font) + v_space + 2;
			_width = w.bitmap()->w;
			
			// Calculate right and bottom
			_right = _left + _width - 1;
			_bottom = _top + _height - 1;
		}
		
		MenuBarBase::MenuBarBase(_Dialog& d, MenuBar& o)
				:
				_owner(o),
				_insertingMenus(false),
				_calcWidth(false),
				_menuDisplayed(0)
		{
			throw std::runtime_error("MenuBar(Dialog) not implemented yet!");
		}
		
		MenuBarBase::~MenuBarBase()
		{
			std::for_each(_menus.begin(), _menus.end(), object_delete<Menu>);
			_menus.clear();
		}
		
		void MenuBarBase::active_changed(bool o, bool n)
		{
			// TODO:
		}

		MenuBar::iterator MenuBarBase::begin()
		{
			return _menus.begin();
		}
		
		MenuBar::const_iterator MenuBarBase::begin() const
		{
			return _menus.begin();
		}

		int MenuBarBase::bottom() const
		{
			return _bottom;
		}

#ifdef	_DEBUG
		void MenuBarBase::dump() const
		{
			// TODO:
		}
#endif

		bool MenuBarBase::empty() const
		{
			return _menus.empty();
		}
		
		MenuBar::iterator MenuBarBase::end()
		{
			return _menus.end();
		}
		
		MenuBar::const_iterator MenuBarBase::end() const
		{
			return _menus.end();
		}
				
		void MenuBarBase::erase(MenuBar::iterator i)
		{
			_menus.erase(i);
		}

		void MenuBarBase::flags_changed(int o, int n)
		{
		}

		int MenuBarBase::height() const
		{
			return _height;
		}
		
		MenuBar::iterator MenuBarBase::insert(iterator i, Menu& m)
		{
			return _menus.insert(i, &m);
		}
		
		int MenuBarBase::left() const
		{
			return _left;
		}

		void MenuBarBase::menus_begin()
		{
			ASSERT(_insertingMenus == false);
			_insertingMenus = true;
		}
		
		void MenuBarBase::menus_end()
		{
			ASSERT(_insertingMenus == true);
			_insertingMenus = false;
			
			// Calcualte menu positions
			// frame(2) + space(3) + title() + space(3) + space(3) + title() + space(3) ...
			int left = _left + 2;
			const iterator end = _menus.end();
			for (iterator i = _menus.begin(); i != end; ++i)
			{
				MenuBase& mb = *(*i)->_impl;
				mb.left(left);
				mb.top(_top);
				mb.width(h_space + text_length(font, mb.title()) + h_space);
				mb.height(_height);
				left += mb.width();
				if (_calcWidth)
					_width += mb.width();
			}
			if (_calcWidth)
				_right = _left + _width - 1;
		}
		
		void MenuBarBase::paint(BITMAP* bmp)
		{
			// Draw outline
			paint_frame(bmp);
			
			// Draw menus
			const iterator end = _menus.end();
			for (iterator i = _menus.begin(); i != end; ++i)
				(*i)->_impl->paint_title(bmp);
		}

		bool MenuBarBase::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			bool isMenuDisplayed = (_menuDisplayed != 0);								// Is a menu waiting?
			bool inMenuBar = (x >= _left && x <= _right && y >= _top && y <= _bottom);	// Is mouse within menu bar?
			bool inMenu = (isMenuDisplayed ? _menuDisplayed->inMenu(x, y) : false);		// Within menu?

			// If no menu waiting and not in me, it's not for us
			if (!isMenuDisplayed && !inMenuBar)
				return false;
			
			// If menu waiting, and click isn't in menu or me, it's not for us
			if (isMenuDisplayed && !inMenuBar && !inMenu)
			{
				_menuDisplayed->restore(bmp);
				_menuDisplayed = 0;
				return false;
			}

			// If no menu is displayed
			if (!isMenuDisplayed)
			{
				MenuBase* m;
				
				// Is it within any menu titles?
				const iterator end = _menus.end();
				for (iterator i = _menus.begin(); i != end; ++i)
				{
					m = (*i)->_impl;
					if (m->inTitle(x, y))
						break;
				}
				// Nope, so ignore the mouse down
				if (i == end)
					return false;

				// Yup, so pop the menu down
				m->save(bmp);
				m->paint_menu(bmp);
				
				// Wait for mouse button to release
				int mx = x;
				int my = y;
				MenuBase* oldM = m;
				while ((mouse_b & 1) != 0)
				{
					if (mouse_x != mx || mouse_y != my)
					{
						mx = mouse_x;
						my = mouse_y;
						
						// Is mouse no longer in title?
						if (!oldM->inTitle(mx, my))
						{
							// Is is still in menu bar?
							if (mx >= _left && mx <= _right && my >= _top && my <= _bottom)
							{
								// Yes, so determine which menu it's in
								for (i = _menus.begin(); i != end; ++i)
								{
									m = (*i)->_impl;
									if (m->inTitle(mx, my))
										break;
								}
								
								// Is it in a title?
								if (i != end)
								{
									ASSERT(oldM != m);	// Siince we tested for inTitle to begin with
									
									// Close old menu
									oldM->restore(bmp);
									
									// Pop down new menu
									oldM = m;
									oldM->save(bmp);
									oldM->paint_menu(bmp);
								}
							}
							// Is it in menu?
							else if (oldM->inMenu(mx, my))
							{
								// Let menu hilite correct item
								oldM->update_hilite(bmp, mx, my);
							}
						}
					}
					else
						rest(1);
				}

				// Mouse up in title?
				if (oldM->inTitle(mx, my))
				{
					// Leave menu open and wait for next mouse down
					_menuDisplayed = oldM;
					return true;
				}
				
				// Mouse up in menu?
				if (oldM->inMenu(mx, my))
				{
					// Execute menu item
					oldM->selected(bmp, mx, my);
					return true;
				}
				
				// Close menu
				oldM->restore(bmp);
				return true;
			}
			else
			{
				// If it's not within menu, clean up
				if (!_menuDisplayed->inMenu(x, y))
				{
					_menuDisplayed->restore(bmp);
					_menuDisplayed = 0;
					
					// Allow logic to re-check for in me
					return mouse_down(bmp, x, y, m);
				}
				
				// Wait for mouse up
				int mx = x;
				int my = y;
				while ((mouse_b & 1) != 0)
				{
					if (mouse_x != mx || mouse_y != my)
					{
						mx = mouse_x;
						my = mouse_y;
					}
					else
						rest(1);
				}
				
				// Mouse up in menu?
				if (_menuDisplayed->inMenu(mx, my))
				{
					_menuDisplayed->selected(bmp, mx, my);
					_menuDisplayed = 0;
					return true;
				}

				// Don't "claim" the click
				_menuDisplayed->restore(bmp);
				_menuDisplayed = 0;
				return false;
			}
		}
		
		void MenuBarBase::mouse_move(BITMAP* bmp)
		{
			// If no menu is displayed, nothing to do
			if (_menuDisplayed == 0)
				return;
			
			// Allow menu to update hilited item
			const int x = mouse_x;
			const int y = mouse_y;
			if (_menuDisplayed->inMenu(x, y))
				_menuDisplayed->update_hilite(bmp, x, y);
		}
		
		int MenuBarBase::right() const
		{
			return _right;
		}
		
		int MenuBarBase::size() const
		{
			return numeric_cast<int>(_menus.size());
		}

		int MenuBarBase::top() const
		{
			return _top;
		}

		int MenuBarBase::width() const
		{
			return _width;
		}
		
//=============================================================================
// class MenuBase implementation
//=============================================================================

		MenuBase::MenuBase(Menu& m, int id, const char* t)
				:
				_owner(m),
				_left(0),
				_right(0),
				_top(0),
				_bottom(0),
				_width(0),
				_height(0),
				_id(id),
				_title(t),
				_insertingItems(false),
				_menuHeight(4),
				_menuWidth(0),
				_save(0)
		{
		}
		
		MenuBase::~MenuBase()
		{
			if (_save)
				destroy_bitmap(_save);
			std::for_each(_items.begin(), _items.end(), object_delete<MenuItem>);
			_items.clear();
		}
		
		void MenuBase::active_changed(bool o, bool n)
		{
		}

		Menu::iterator MenuBase::begin()
		{
			return _items.begin();
		}
		
		Menu::const_iterator MenuBase::begin() const
		{
			return _items.begin();
		}
		
		int MenuBase::bottom() const
		{
			return _bottom;
		}

#ifdef	_DEBUG
		void MenuBase::dump() const
		{
		}
#endif

		bool MenuBase::empty() const
		{
			return _items.empty();
		}
		
		Menu::iterator MenuBase::end()
		{
			return _items.end();
		}
		
		Menu::const_iterator MenuBase::end() const
		{
			return _items.end();
		}
				
		void MenuBase::erase(Menu::iterator i)
		{
			_items.erase(i);
		}

		void MenuBase::flags_changed(int o, int n)
		{
		}

		int MenuBase::height() const
		{
			return _height;
		}

		void MenuBase::height(int h)
		{
			_height = h;
			_bottom = _top + _height - 1;
		}
		
		int MenuBase::id() const
		{
			return _id;
		}
		
		Menu::iterator MenuBase::insert(Menu::iterator i, MenuItem& mi)
		{
			return _items.insert(i, &mi);
		}
		
		bool MenuBase::inTitle(int x, int y)
		{
			return (x >= _left && x <= _right && y >= _top && y <= _bottom);
		}
		
		bool MenuBase::inMenu(int x, int y)
		{
			return (x >= _left && x < (_left + _menuWidth)
					&& y > _bottom && y < (_bottom + _menuHeight));
		}
		
		void MenuBase::items_begin()
		{
			ASSERT(_insertingItems == false);
			_insertingItems = true;
		}
		
		void MenuBase::items_end()
		{
			ASSERT(_insertingItems == true);
			_insertingItems = false;
			
			// Determine the overall height and width of the items
			int height = 0;
			int width = 0;
			MenuItemBase* mi;
			const iterator end = _items.end();
			for (iterator i = _items.begin(); i != end; ++i)
			{
				mi = (*i)->_impl;
				height += mi->height();
				if (mi->width() > width)
					width = mi->width();
			}
			_menuWidth = width;
			_menuHeight = height + (2 * v_space);
			if (_save)
				destroy_bitmap(_save);
			_save = create_bitmap(_menuWidth, _menuHeight + 1);
		}
		
		int MenuBase::left() const
		{
			return _left;
		}
		
		void MenuBase::left(int l)
		{
			_left = l;
			_right = _left + _width - 1;
		}
		
		void MenuBase::paint_menu_items(BITMAP* bmp, int x, int y)
		{
			const int w = _menuWidth - ((x - _left) * 2);
			MenuItemBase* mi;
			const iterator end = _items.end();
			for (iterator i = _items.begin(); i != end; ++i)
			{
				mi = (*i)->_impl;
				mi->paint(bmp, x, y, w);
				y += mi->height();
			}
		}
		
		bool MenuBase::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			return false;
		}

		void MenuBase::restore(BITMAP* bmp)
		{
			Update u(bmp);
			blit(_save, bmp, 0, 0, _left, _bottom, _menuWidth, _menuHeight + 1);
		}
		
		int MenuBase::right() const
		{
			return _right;
		}
		
		void MenuBase::save(BITMAP *bmp)
		{
			Update u(bmp);
			blit(bmp, _save, _left, _bottom, 0, 0, _menuWidth, _menuHeight + 1);
		}
		
		void MenuBase::selected(BITMAP* bmp, int x, int y)
		{
			// Close menu
			{
				Update u(bmp);
				restore(bmp);
			}
			
			// Call callback
			MenuItem* mi = y2mi(y);
			std::pair<MenuItem::CALLBACK, void*> cb = mi->callback();
			if (cb.first)
				cb.first(bmp, *mi, cb.second);
		}
		
		int MenuBase::size() const
		{
			return numeric_cast<int>(_items.size());
		}

		const char* MenuBase::title() const
		{
			return _title.c_str();
		}
		
		void MenuBase::title(const char* t)
		{
			_title = t;
		}
		
		const std::string& MenuBase::title_object() const
		{
			return _title;
		}
		
		int MenuBase::top() const
		{
			return _top;
		}

		void MenuBase::top(int t)
		{
			_top = t;
			_bottom = _top + _height - 1;
		}
		
		int MenuBase::width() const
		{
			return _width;
		}
		
		void MenuBase::width(int w)
		{
			_width = w;
			_right = _left + _width - 1;
		}

		MenuItem* MenuBase::y2mi(int y)
		{
			MenuItem* mi = 0;
			int cur_y = _bottom + 1;
			const iterator end = _items.end();
			for (iterator i = _items.begin(); i != end; ++i)
			{
				cur_y += (*i)->_impl->height();
				if (cur_y >= y)
				{
					mi = *i;
					break;
				}
				++i;
			}
			return mi;
		}
		
//=============================================================================
// class MenuItemBase implementation
//=============================================================================

		MenuItemBase::MenuItemBase(MenuItem& o, int id, int f)
				:
				_owner(o),
				_id(id),
				_flags(f),
				_callback(0)
		{
		}

		MenuItemBase::MenuItemBase(const MenuItemBase& r)
				:
				_owner(r._owner),
				_id(r._id),
				_flags(r._flags),
				_callback(r._callback),
				_data(r._data),
				_width(r._width),
				_height(r._height)
		{
		}
		
		MenuItemBase& MenuItemBase::operator=(const MenuItem& i)
		{
			throw std::runtime_error("MenuItem::operator= not implemented yet!");
		}
		
		MenuItemBase::~MenuItemBase()
		{
		}
		
		BITMAP* MenuItemBase::bitmap() const
		{
			throw std::logic_error("MenuItem doesn't contain bitmap!");
		}
		
		void MenuItemBase::bitmap(BITMAP* bmp)
		{
			throw std::logic_error("MenuItem doesn't contain bitmap!");
		}

		std::pair<MenuItem::CALLBACK, void*> MenuItemBase::callback() const
		{
			return std::make_pair(_callback, _data);
		}
		
		void MenuItemBase::callback(MenuItem::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}
		
		void MenuItemBase::check(bool c)
		{
			_flags = ((_flags & (~MenuItem::c_mask))
					| (c ? MenuItem::c_checked : MenuItem::c_unchecked));
		}

		bool MenuItemBase::checked() const
		{
			return ((_flags & MenuItem::c_mask) != MenuItem::c_unchecked);
		}

		bool MenuItemBase::disabled() const
		{
			return ((_flags & MenuItem::e_mask) == MenuItem::e_disabled);
		}

#ifdef	_DEBUG
		void MenuItemBase::dump() const
		{
		}
#endif

		void MenuItemBase::enable(bool e)
		{
			_flags = ((_flags & (~MenuItem::e_mask))
					| (e ? MenuItem::e_enabled : MenuItem::e_disabled));
		}

		bool MenuItemBase::enabled() const
		{
			return ((_flags & MenuItem::e_mask) == MenuItem::e_enabled);
		}
		
		int MenuItemBase::flags() const
		{
			return _flags;
		}
		
		void MenuItemBase::flags(int f)
		{
			_flags = ((f & MenuItem::user_mask) | (_flags & MenuItem::item_mask));
		}

		int MenuItemBase::height() const
		{
			return _height;
		}
		
		int MenuItemBase::id() const
		{
			return _id;
		}
		
		Menu& MenuItemBase::menu()
		{
			throw std::logic_error("MenuItem doesn't contain sub-menu!");
		}
		
		const Menu& MenuItemBase::menu() const
		{
			throw std::logic_error("MenuItem doesn't contain sub-menu!");
		}
		
		void MenuItemBase::menu(Menu& m)
		{
			throw std::logic_error("MenuItem doesn't contain sub-menu!");
		}

		const char* MenuItemBase::title() const
		{
			throw std::logic_error("MenuItem doesn't contain title!");
		}
		
		void MenuItemBase::title(const char* t)
		{
			throw std::logic_error("MenuItem doesn't contain title!");
		}
		
		const std::string& MenuItemBase::title_object() const
		{
			throw std::logic_error("MenuItem doesn't contain title!");
		}
		
		int MenuItemBase::type() const
		{
			return (_flags & MenuItem::t_mask);
		}

		bool MenuItemBase::unchecked() const
		{
			return ((_flags & MenuItem::c_mask) == MenuItem::c_unchecked);
		}
		
		int MenuItemBase::width() const
		{
			return _width;
		}
		
//=============================================================================
// class MenuItemBitmapBase implementation
//=============================================================================

		MenuItemBitmapBase::MenuItemBitmapBase(MenuItem& o, int id, int f, BITMAP* b)
				:
				MenuItemBase(o, id, f),
				_bitmap(b)
		{
			_width = _bitmap->w;
			_height = _bitmap->h;
		}
		
		MenuItemBitmapBase::MenuItemBitmapBase(const MenuItemBitmapBase& r)
				:
				MenuItemBase(r),
				_bitmap(r._bitmap)
		{
		}
		
		MenuItemBitmapBase::~MenuItemBitmapBase()
		{
			if (_bitmap)
				destroy_bitmap(_bitmap);
		}
		
		BITMAP* MenuItemBitmapBase::bitmap() const
		{
			return _bitmap;
		}
		
		void MenuItemBitmapBase::bitmap(BITMAP* bmp)
		{
			_bitmap = bmp;
		}

//=============================================================================
// class MenuItemSeperatorBase implementation
//=============================================================================

		MenuItemSeperatorBase::MenuItemSeperatorBase(MenuItem& o, int id, int f)
				:
				MenuItemBase(o, id, f)
		{
			_width = 4;
			_height = 3;
		}
		
		MenuItemSeperatorBase::MenuItemSeperatorBase(const MenuItemSeperatorBase& r)
				:
				MenuItemBase(r)
		{
		}
		
//=============================================================================
// class MenuItemSubMenuBase implementation
//=============================================================================

		MenuItemSubMenuBase::MenuItemSubMenuBase(MenuItem& o, int id, int f, Menu& m)
				:
				MenuItemBase(o, id, f),
				_menu(&m)
		{
			_height = text_height(font) + 4;
			_width = 10 + text_length(font, _menu->title()) + 10;
		}
		
		MenuItemSubMenuBase::MenuItemSubMenuBase(const MenuItemSubMenuBase& r)
				:
				MenuItemBase(r),
				_menu(r._menu)
		{
		}
		
		MenuItemSubMenuBase::~MenuItemSubMenuBase()
		{
			delete _menu;
		}
		
		Menu& MenuItemSubMenuBase::menu()
		{
			return *_menu;
		}
		
		const Menu& MenuItemSubMenuBase::menu() const
		{
			return *_menu;
		}
		
		void MenuItemSubMenuBase::menu(Menu& m)
		{
			_menu = &m;
		}
		
//=============================================================================
// class MenuItemTitleBase implementation
//=============================================================================

		MenuItemTitleBase::MenuItemTitleBase(MenuItem& o, int id, int f, const char* t)
				:
				MenuItemBase(o, id, f)
		{
			if (t)
				_title = t;
			_height = text_height(font) + 4;
			_width = 10 + text_length(font, _title.c_str());
		}
		
		MenuItemTitleBase::MenuItemTitleBase(const MenuItemTitleBase& r)
				:
				MenuItemBase(r),
				_title(r._title)
		{
		}
		
		MenuItemTitleBase::~MenuItemTitleBase()
		{
		}
		
		const char* MenuItemTitleBase::title() const
		{
			return _title.c_str();
		}
		
		void MenuItemTitleBase::title(const char* t)
		{
			_title = t;
		}
		
		const std::string& MenuItemTitleBase::title_object() const
		{
			return _title;
		}

//=============================================================================
// class MenuBarImpl implementation
//=============================================================================

		MenuBarImpl::MenuBarImpl(MenuBar& o, int x, int y, int w, int h)
				:
				MenuBarBase(o, x, y, w, h)
		{
		}
		
		MenuBarImpl::MenuBarImpl(MenuBar& o, int x, int y)
				:
				MenuBarBase(o, x, y)
		{
		}
		
		MenuBarImpl::MenuBarImpl(_Window& w, MenuBar& o)
				:
				MenuBarBase(w, o)
		{
		}
		
		MenuBarImpl::MenuBarImpl(_Dialog& d, MenuBar& o)
				:
				MenuBarBase(d, o)
		{
		}
		
		void MenuBarImpl::paint_frame(BITMAP* bmp)
		{
		}
		
//=============================================================================
// class MenuBarImpl3D implementation
//=============================================================================

		MenuBarImpl3D::MenuBarImpl3D(MenuBar& o, int x, int y, int w, int h)
				:
				MenuBarBase(o, x, y, w, h)
		{
		}
		
		MenuBarImpl3D::MenuBarImpl3D(MenuBar& o, int x, int y)
				:
				MenuBarBase(o, x, y)
		{
		}
		
		MenuBarImpl3D::MenuBarImpl3D(_Window& w, MenuBar& o)
				:
				MenuBarBase(w, o)
		{
		}
		
		MenuBarImpl3D::MenuBarImpl3D(_Dialog& d, MenuBar& o)
				:
				MenuBarBase(d, o)
		{
		}

		void MenuBarImpl3D::paint_frame(BITMAP* bmp)
		{
			Update u(bmp);
			rect(bmp, _left + 1, _top + 1, _right, _bottom, Theme.WHITE);
			rect(bmp, _left, _top, _right - 1, _bottom - 1, Theme.GRAY_DARK);
			putpixel(bmp, _right, _top, Theme.WHITE);
			putpixel(bmp, _left, _bottom, Theme.WHITE);
		}
		
//=============================================================================
// class MenuImpl implementation
//=============================================================================

		MenuImpl::MenuImpl(Menu& m, int id, const char* t)
				:
				MenuBase(m, id, t)
		{
		}
		
		void MenuImpl::paint_menu(BITMAP* bmp)
		{
		}

		void MenuImpl::paint_title(BITMAP* bmp)
		{
		}

		void MenuImpl::update_hilite(BITMAP* bmp, int x, int y)
		{
		}
		
//=============================================================================
// class MenuImpl3D implementation
//=============================================================================

		MenuImpl3D::MenuImpl3D(Menu& m, int id, const char* t)
				:
				MenuBase(m, id, t)
		{
		}
		
		void MenuImpl3D::paint_menu(BITMAP* bmp)
		{
			// Paint frame
			{
				Update u(bmp);
				const int right = _left + _menuWidth - 1;
				const int bottom = _bottom + _menuHeight;
				vline(bmp, _left, _bottom, bottom, Theme.GRAY_DARK);
				vline(bmp, right, _bottom, bottom, Theme.GRAY_DARK);
				hline(bmp, _left + 1, bottom, right - 1, Theme.GRAY_DARK);
				rectfill(bmp, _left + 1, _bottom + 1, right - 1, bottom - 1,
						Theme.WHITE);
			}
			
			// Paint items
			const int x = _left + 1;
			const int y = _bottom + 1;
			paint_menu_items(bmp, x, y);
		}

		void MenuImpl3D::paint_title(BITMAP* bmp)
		{
			Update u(bmp);
			textout_ex(bmp, font, _title.c_str(), _left + 3, _top + 4, Theme.BLACK,
					Theme.GRAY);
		}

		void MenuImpl3D::update_hilite(BITMAP* bmp, int x, int y)
		{
			MenuItem* smi = y2mi(y);
			
		}
		
//=============================================================================
// class MenuItemBitmapImpl implementation
//=============================================================================

		MenuItemBitmapImpl::MenuItemBitmapImpl(MenuItem& o, int id, int f, BITMAP* b)
				:
				MenuItemBitmapBase(o, id, f, b)
		{
		}

		MenuItemBitmapImpl::MenuItemBitmapImpl(const MenuItemBitmapImpl& r)
				:
				MenuItemBitmapBase(r)
		{
		}
		
		void MenuItemBitmapImpl::paint(BITMAP* bmp, int x, int y, int w)
		{
		}
		
//=============================================================================
// class MenuItemBitmapImpl3D implementation
//=============================================================================

		MenuItemBitmapImpl3D::MenuItemBitmapImpl3D(MenuItem& o, int id, int f, BITMAP* b)
				:
				MenuItemBitmapBase(o, id, f, b)
		{
		}

		MenuItemBitmapImpl3D::MenuItemBitmapImpl3D(const MenuItemBitmapImpl3D& r)
				:
				MenuItemBitmapBase(r)
		{
		}
		
		void MenuItemBitmapImpl3D::paint(BITMAP* bmp, int x, int y, int w)
		{
		}
		
//=============================================================================
// class MenuItemSeperatorImpl implementation
//=============================================================================

		MenuItemSeperatorImpl::MenuItemSeperatorImpl(MenuItem& o, int id, int f)
				:
				MenuItemSeperatorBase(o, id, f)
		{
		}
		
		MenuItemSeperatorImpl::MenuItemSeperatorImpl(const MenuItemSeperatorImpl& r)
				:
				MenuItemSeperatorBase(r)
		{
		}
		
		void MenuItemSeperatorImpl::paint(BITMAP* bmp, int x, int y, int w)
		{
		}
		
//=============================================================================
// class MenuItemSeperatorImpl3D implementation
//=============================================================================

		MenuItemSeperatorImpl3D::MenuItemSeperatorImpl3D(MenuItem& o, int id, int f)
				:
				MenuItemSeperatorBase(o, id, f)
		{
		}

		MenuItemSeperatorImpl3D::MenuItemSeperatorImpl3D(const MenuItemSeperatorImpl3D& r)
				:
				MenuItemSeperatorBase(r)
		{
		}
		
		void MenuItemSeperatorImpl3D::paint(BITMAP* bmp, int x, int y, int w)
		{
			Update u(bmp);
			hline(bmp, x, y + 1, x + w - 1, Theme.GRAY_DARK);
		}
		
//=============================================================================
// class MenuItemSubMenuImpl implementation
//=============================================================================

		MenuItemSubMenuImpl::MenuItemSubMenuImpl(MenuItem& o, int id, int f, Menu& m)
				:
				MenuItemSubMenuBase(o, id, f, m)
		{
		}
		
		MenuItemSubMenuImpl::MenuItemSubMenuImpl(const MenuItemSubMenuImpl& r)
				:
				MenuItemSubMenuBase(r)
		{
		}
		
		void MenuItemSubMenuImpl::paint(BITMAP* bmp, int x, int y, int w)
		{
		}
		
//=============================================================================
// class MenuItemSubMenuImpl3D implementation
//=============================================================================

		MenuItemSubMenuImpl3D::MenuItemSubMenuImpl3D(MenuItem& o, int id, int f, Menu& m)
				:
				MenuItemSubMenuBase(o, id, f, m)
		{
		}
		
		MenuItemSubMenuImpl3D::MenuItemSubMenuImpl3D(const MenuItemSubMenuImpl3D& r)
				:
				MenuItemSubMenuBase(r)
		{
		}
		
		void MenuItemSubMenuImpl3D::paint(BITMAP* bmp, int x, int y, int w)
		{
			Update u(bmp);

			// Menu title
			textout_ex(bmp, font, _menu->title(), x + 3, y + 2,
					makecol(0, 0, 0), makecol(249, 248, 255));

			// Menu dingy
			const int left = x + 3 + text_length(font, _menu->title()) + 6;
			const int top = y + 2 + ((text_height(font) - 7) / 2);
			vline(bmp, left, top, top + 6, Theme.BLACK);
			vline(bmp, left + 1, top + 1, top + 5, Theme.BLACK);
			vline(bmp, left + 2, top + 2, top + 4, Theme.BLACK);
			putpixel(bmp, left + 3, top + 3, Theme.BLACK);
		}
		
//=============================================================================
// class MenuItemTitleImpl implementation
//=============================================================================

		MenuItemTitleImpl::MenuItemTitleImpl(MenuItem& o, int id, int f, const char* t)
				:
				MenuItemTitleBase(o, id, f, t)
		{
		}
		
		MenuItemTitleImpl::MenuItemTitleImpl(const MenuItemTitleImpl& r)
				:
				MenuItemTitleBase(r)
		{
		}

		void MenuItemTitleImpl::paint(BITMAP* bmp, int x, int y, int w)
		{
		}
		
//=============================================================================
// class MenuItemTitleImpl3D implementation
//=============================================================================

		MenuItemTitleImpl3D::MenuItemTitleImpl3D(MenuItem& o, int id, int f, const char* t)
				:
				MenuItemTitleBase(o, id, f, t)
		{
		}
		
		MenuItemTitleImpl3D::MenuItemTitleImpl3D(const MenuItemTitleImpl3D& r)
				:
				MenuItemTitleBase(r)
		{
		}

		void MenuItemTitleImpl3D::paint(BITMAP* bmp, int x, int y, int w)
		{
			Update u(bmp);
			textout_ex(bmp, font, _title.c_str(), x + 3, y + 2, Theme.BLACK,
					Theme.WHITE);
		}
	}
}

//=============================================================================
// class Menu implementation
//=============================================================================

Menu::Menu(int id, const char* t)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuImpl3D(*this, id, t);
	else
		_impl = new MenuImpl::MenuImpl(*this, id, t);
}

Menu::~Menu()
{
	delete _impl;
}

Menu::iterator Menu::begin()
{
	return _impl->begin();
}

Menu::const_iterator Menu::begin() const
{
	return _impl->begin();
}

int Menu::bottom() const
{
	return _impl->bottom();
}

#ifdef	_DEBUG
void Menu::dump() const
{
}
#endif

bool Menu::empty() const
{
	return _impl->empty();
}

Menu::iterator Menu::end()
{
	return _impl->end();
}

Menu::const_iterator Menu::end() const
{
	return _impl->end();
}

void Menu::erase(iterator i)
{
	_impl->erase(i);
}

int Menu::height() const
{
	return _impl->height();
}

int Menu::id() const
{
	return _impl->id();
}

Menu::iterator Menu::insert(MenuItem& mi)
{
	return _impl->insert(_impl->end(), mi);
}

Menu::iterator Menu::insert(iterator i, MenuItem& mi)
{
	return _impl->insert(i, mi);
}

void Menu::items_begin()
{
	_impl->items_begin();
}

void Menu::items_end()
{
	_impl->items_end();
}

int Menu::left() const
{
	return _impl->left();
}

int Menu::right() const
{
	return _impl->right();
}

int Menu::size() const
{
	return _impl->size();
}

const char* Menu::title() const
{
	return _impl->title();
}

void Menu::title(const char* t)
{
	_impl->title(t);
}

const std::string& Menu::title_object() const
{
	return _impl->title_object();
}

int Menu::top() const
{
	return _impl->top();
}

int Menu::width() const
{
	return _impl->width();
}

//=============================================================================
// class MenuBar declaration
//=============================================================================

MenuBar::MenuBar(_Manager& m, int x, int y, int w, int h)
		:
		_ControlExFirst(m, m_down | m_move)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuBarImpl3D(*this, x, y, w, h);
	else
		_impl = new MenuImpl::MenuBarImpl(*this, x, y, w, h);
}

MenuBar::MenuBar(_Manager& m, int x, int y)
		:
		_ControlExFirst(m, m_down | m_move)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuBarImpl3D(*this, x, y);
	else
		_impl = new MenuImpl::MenuBarImpl(*this, x, y);
}

MenuBar::MenuBar(_Window& w)
		:
		_ControlExFirst(w, m_down | m_move)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuBarImpl3D(w, *this);
	else
		_impl = new MenuImpl::MenuBarImpl(w, *this);
}

MenuBar::MenuBar(_Dialog& d)
		:
		_ControlExFirst(d, m_down | m_move)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuBarImpl3D(d, *this);
	else
		_impl = new MenuImpl::MenuBarImpl(d, *this);
}

MenuBar::~MenuBar()
{
	delete _impl;
}

void MenuBar::active_changed(bool o, bool n)
{
	_impl->active_changed(o, n);
}

MenuBar::iterator MenuBar::begin()
{
	return _impl->begin();
}

MenuBar::const_iterator MenuBar::begin() const
{
	return _impl->begin();
}

int MenuBar::bottom() const
{
	return _impl->bottom();
}

#ifdef	_DEBUG
void MenuBar::dump() const
{
}
#endif

bool MenuBar::empty() const
{
	return _impl->empty();
}

MenuBar::iterator MenuBar::end()
{
	return _impl->end();
}

MenuBar::const_iterator MenuBar::end() const
{
	return _impl->end();
}

void MenuBar::erase(iterator i)
{
	_impl->erase(i);
}

void MenuBar::flags_changed(int o, int n)
{
	_impl->flags_changed(o, n);
}

int MenuBar::height() const
{
	return _impl->height();
}

MenuBar::iterator MenuBar::insert(Menu& m)
{
	return _impl->insert(_impl->end(), m);
}

MenuBar::iterator MenuBar::insert(iterator i, Menu& m)
{
	return _impl->insert(i, m);
}

void MenuBar::menus_begin()
{
	_impl->menus_begin();
}

void MenuBar::menus_end()
{
	_impl->menus_end();
}

int MenuBar::left() const
{
	return _impl->left();
}

void MenuBar::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

bool MenuBar::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	return _impl->mouse_down(bmp, x, y, m);
}

void MenuBar::mouse_move(BITMAP* bmp)
{
	_impl->mouse_move(bmp);
}

int MenuBar::right() const
{
	return _impl->right();
}

int MenuBar::size() const
{
	return _impl->size();
}

int MenuBar::top() const
{
	return _impl->top();
}

int MenuBar::width() const
{
	return _impl->width();
}

//=============================================================================
// class MenuItem declaration
//=============================================================================

MenuItem::MenuItem(int id, int f, const char* t)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuItemTitleImpl3D(*this, id, f, t);
	else
		_impl = new MenuImpl::MenuItemTitleImpl(*this, id, f, t);
}

MenuItem::MenuItem(int id, int f, BITMAP* bmp)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuItemBitmapImpl3D(*this, id, f, bmp);
	else
		_impl = new MenuImpl::MenuItemBitmapImpl(*this, id, f, bmp);
}

MenuItem::MenuItem(int id, int f, Menu& m)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuItemSubMenuImpl3D(*this, id, f, m);
	else
		_impl = new MenuImpl::MenuItemSubMenuImpl(*this, id, f, m);
}

MenuItem::MenuItem(int id, int f, bool)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new MenuImpl::MenuItemSeperatorImpl3D(*this, id, f);
	else
		_impl = new MenuImpl::MenuItemSeperatorImpl(*this, id, f);
}

MenuItem::MenuItem(const MenuItem& i)
{
	if (Theme.style == Configuration::STYLE_3D)
	{
		switch (i.type())
		{
		case t_seperator:
			_impl = new MenuImpl::MenuItemSeperatorImpl3D(
					*static_cast<MenuImpl::MenuItemSeperatorImpl3D*>(i._impl));
			break;

		case t_bitmap:
			_impl = new MenuImpl::MenuItemBitmapImpl3D(
					*static_cast<MenuImpl::MenuItemBitmapImpl3D*>(i._impl));
			break;

		case t_title:
			_impl = new MenuImpl::MenuItemTitleImpl3D(
					*static_cast<MenuImpl::MenuItemTitleImpl3D*>(i._impl));
			break;

		case t_submenu:
			_impl = new MenuImpl::MenuItemSubMenuImpl3D(
					*static_cast<MenuImpl::MenuItemSubMenuImpl3D*>(i._impl));
			break;
		}
	}
	else
	{
		switch (i.type())
		{
		case t_seperator:
			_impl = new MenuImpl::MenuItemSeperatorImpl(
					*static_cast<MenuImpl::MenuItemSeperatorImpl*>(i._impl));
			break;

		case t_bitmap:
			_impl = new MenuImpl::MenuItemBitmapImpl(
					*static_cast<MenuImpl::MenuItemBitmapImpl*>(i._impl));
			break;

		case t_title:
			_impl = new MenuImpl::MenuItemTitleImpl(
					*static_cast<MenuImpl::MenuItemTitleImpl*>(i._impl));
			break;

		case t_submenu:
			_impl = new MenuImpl::MenuItemSubMenuImpl(
					*static_cast<MenuImpl::MenuItemSubMenuImpl*>(i._impl));
			break;
		}
	}
}

MenuItem::~MenuItem()
{
	delete _impl;
}

MenuItem& MenuItem::operator=(const MenuItem& i)
{
	throw std::runtime_error("MenuItem::operator= not implemented yet!");
}

BITMAP* MenuItem::bitmap() const
{
	return _impl->bitmap();
}

void MenuItem::bitmap(BITMAP* bmp)
{
	_impl->bitmap(bmp);
}

std::pair<MenuItem::CALLBACK, void*> MenuItem::callback() const
{
	return _impl->callback();
}

void MenuItem::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

void MenuItem::check(bool c)
{
	_impl->check(c);
}

bool MenuItem::checked() const
{
	return _impl->checked();
}

bool MenuItem::disabled() const
{
	return _impl->disabled();
}

void MenuItem::enable(bool e)
{
	_impl->enable(e);
}

bool MenuItem::enabled() const
{
	return _impl->enabled();
}

int MenuItem::flags() const
{
	return _impl->flags();
}

void MenuItem::flags(int f)
{
	_impl->flags(f);
}

int MenuItem::id() const
{
	return _impl->id();
}

Menu& MenuItem::menu()
{
	return _impl->menu();
}

const Menu& MenuItem::menu() const
{
	return _impl->menu();
}

void MenuItem::menu(Menu& m)
{
	_impl->menu(m);
}

const char* MenuItem::title() const
{
	return _impl->title();
}

void MenuItem::title(const char* t)
{
	_impl->title(t);
}

const std::string& MenuItem::title_object() const
{
	return _impl->title_object();
}

int MenuItem::type() const
{
	return _impl->type();
}

bool MenuItem::unchecked() const
{
	return _impl->unchecked();
}
