#ifndef	__YACG_MENU_HPP__
#define	__YACG_MENU_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_DIALOG_HPP__
#include <yacg/Dialog.hpp>
#endif

#ifndef	__YACG_WINDOW_HPP__
#include <yacg/Window.hpp>
#endif

namespace yacg
{
	class Menu;
	class MenuBar;
	class MenuItem;
	
	namespace MenuImpl
	{
		class MenuBase;
		class MenuBarBase;
		class MenuItemBase;
	}
	
//=============================================================================
// class MenuBar declaration
//=============================================================================

	class MenuBar : public _ControlExFirst
	{
	public:
		typedef std::list<Menu*>::iterator iterator;
		typedef std::list<Menu*>::const_iterator const_iterator;
		
	public:	// Common - client usage expected
		MenuBar(_Manager& m, int x, int y, int w, int h = calc);
		MenuBar(_Manager& m, int x, int y);							// Width of menus
		MenuBar(_Window& w);										// At top of and full width of window
		MenuBar(_Dialog& d);										// At top of and fill width of dialog
		~MenuBar();

		int left() const;
		int right() const;
		
		int top() const;
		int bottom() const;

		int width() const;
		int height() const;
		
		void menus_begin();											// To prevent calculations after each menu is inserted, call this before the first
		void menus_end();											// and this after the last one.  Macros do this automatically.
		
		iterator begin();
		const_iterator begin() const;
		
		iterator end();
		const_iterator end() const;
				
		iterator insert(Menu& m);
		iterator insert(iterator i, Menu& m);
		
		void erase(iterator i);

		bool empty() const;
		
		int size() const;

	public:	// Uncommon - client usage rare, or only for derived
		void active_changed(bool o, bool n);
		void flags_changed(int o, int n);
		void paint(BITMAP* b);
		bool mouse_down(BITMAP* bmp, int x, int y, int m);
		void mouse_move(BITMAP* bmp);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		MenuImpl::MenuBarBase* _impl;
	};

//=============================================================================
// class Menu declaration
//=============================================================================

	class Menu
	{
	public:
		typedef std::vector<MenuItem*>::iterator iterator;
		typedef std::vector<MenuItem*>::const_iterator const_iterator;
		
	public:	// Common - client usage expected
		Menu(int id, const char* t);
		virtual ~Menu();
	
		int left() const;
		int right() const;
		
		int top() const;
		int bottom() const;

		int width() const;
		int height() const;
		
		int id() const;

		void items_begin();											// To prevent calculations after each item is inserted, call this before the first
		void items_end();											// and this after the last one.  Macros do this automatically.

		iterator begin();
		const_iterator begin() const;
		
		iterator end();
		const_iterator end() const;
				
		iterator insert(MenuItem& mi);
		iterator insert(iterator i, MenuItem& mi);
		
		void erase(iterator i);

		bool empty() const;
		
		int size() const;

		const char* title() const;
		void title(const char* t);
		
		const std::string& title_object() const;

	public:		
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		MenuImpl::MenuBase* _impl;
		friend MenuImpl::MenuBarBase;
	};

//=============================================================================
// class MenuItem declaration
//=============================================================================

	class MenuItem
	{
	public:
		// Type of menu item
		static const int t_seperator	= 0x00;
		static const int t_bitmap		= 0x01;
		static const int t_title		= 0x02;
		static const int t_submenu		= 0x03;
		static const int t_mask			= 0x03;

		// Enablement status of item
		static const int e_enabled		= 0x00;
		static const int e_disabled		= 0x04;
		static const int e_mask			= 0x04;
		
		// Checked status of item
		static const int c_unchecked	= 0x00;
		static const int c_checked		= 0x08;
		static const int c_bitmaped		= 0x10;
		static const int c_mask			= 0x18;
		
		// Mask to allow user to store state info in flags
		static const int user_mask		= 0xFFFFFFF0;
		static const int item_mask		= 0x0000000F;
	
	public:
		typedef void (*CALLBACK)(BITMAP* bmp, MenuItem& i, void* d);
		
	public:
		MenuItem(int id, int f, const char* t);
		MenuItem(int id, int f, BITMAP* b);
		MenuItem(int id, int f, Menu& m);
		MenuItem(int id, int f, bool);						// seperator
		virtual ~MenuItem();

		MenuItem(const MenuItem& i);
		MenuItem& operator=(const MenuItem& i);

		std::pair<CALLBACK, void*> callback() const;
		void callback(CALLBACK f, void* d = 0);
				
		bool unchecked() const;
		bool checked() const;
		void check(bool c);
		
		bool disabled() const;
		bool enabled() const;
		void enable(bool e);

		int flags() const;
		void flags(int f);
		
		int id() const;
		
		int type() const;
		
		const char* title() const;
		void title(const char* t);
		
		const std::string& title_object() const;

		Menu& menu();
		const Menu& menu() const;
		void menu(Menu& m);
		
		BITMAP* bitmap() const;
		void bitmap(BITMAP* bmp);

	private:
		MenuImpl::MenuItemBase* _impl;
		friend MenuImpl::MenuItemBase;
		friend MenuImpl::MenuBase;
	};
}

//=============================================================================
// class MenuBar macros
//=============================================================================

#define	MENUBAR_BEGIN(x, y)	\
{	\
	std::auto_ptr<yacg::MenuBar> mb(new yacg::MenuBar(*this, (x), (y)));	\
	insert(*mb.get());	\
	mb->menus_begin();

#define	MENUBAR_BEGIN_W(w)	\
{	\
	std::auto_ptr<yacg::MenuBar> mb(new yacg::MenuBar(w));	\
	insert(*mb.get());	\
	mb->menus_begin();

#define	MENUBAR_BEGIN_D(d)	\
{	\
	std::auto_ptr<yacg::MenuBar> mb(new yacg::MenuBar(d));	\
	insert(*mb.get());	\
	mb->menus_begin();

#define	MENUBAR_BEGIN_EX(x, y, w, h)	\
{	\
	std::auto_ptr<yacg::MenuBar> mb(new yacg::MenuBar(*this, (x), (y), (w), (h)));	\
	insert(*mb.get());	\
	mb->menus_begin();

#define	MENUBAR_END()	\
	mb->menus_end();	\
	mb.release();	\
}

#define	MENUBAR_V_END(v)	\
	mb->menus_end();	\
	v##_ = mb.release();	\
}

//=============================================================================
// class Menu macros
//=============================================================================

#define	MENU_BEGIN(i, t)	\
{	\
	std::auto_ptr<yacg::Menu> menu(new yacg::Menu((i), (t)));	\
	menu->items_begin();	\
	mb->insert(*menu.get());

#define	MENU_END()	\
	menu->items_end();	\
	menu.release();	\
}

#define	MENU_V_END(v)	\
	menu->items_end();	\
	v##_ = menu.release();	\
}

//=============================================================================
// class MenuItem macros
//=============================================================================

#define	MENUITEM(i, t)					menu->insert(*new yacg::MenuItem(i, yacg::MenuItem::t_title, reinterpret_cast<const char*>(t)));
#define	MENUITEM_EX(i, t, f, d)			{ yacg::MenuItem* mi = new yacg::MenuItem(i, yacg::MenuItem::t_title, reinterpret_cast<const char*>(t)); menu->insert(*mi); mi->callback((f), (d)); }

#define	MENUITEM_BITMAP(i, b)			menu->insert(*new yacg::MenuItem(i, yacg::MenuItem::t_bitmap, reinterpret_cast<const BITMAP*>(b)));
#define	MENUITEM_BITMAP_EX(i, b, f, d)	{ yacg::MenuItem* mi = new yacg::MenuItem(i, yacg::MenuItem::t_bitmap, reinterpret_cast<const BITMAP*>(b)); menu->insert(*mi); mi->callback((f), (d)); }

#define	MENUITEM_SEPERATOR(i)	menu->insert(*new yacg::MenuItem(i, yacg::MenuItem::t_seperator, true));

#define	MENUITEM_SUBMENU_BEGIN(i, t)	\
{	\
	yacg::Menu* parentMenu = menu.get();	\
	std::auto_ptr<yacg::Menu> menu(new yacg::Menu((i), (t)));

#define	MENUITEM_SUBMENU_END(i, t)	\
	parentMenu->insert(*new yacg::MenuItem((i), yacg::MenuItem::t_submenu, *menu.get()));	\
	menu.release();	\
}

#define	MENUITEM_V(v, i, t)					v##_ = new yacg::MenuItem(i, yacg::MenuItem::t_title, reinterpret_cast<const char*>(t)); menu->insert(*v##_);
#define	MENUITEM_V_EX(v, i, t, f, d)		v##_ = new yacg::MenuItem(i, yacg::MenuItem::t_title, reinterpret_cast<const char*>(t)); menu->insert(*v##_); v##_->callback((f), (d));

#define	MENUITEM_V_BITMAP(v, i, b)			v##_ = new yacg::MenuItem(i, yacg::MenuItem::t_bitmap, reinterpret_cast<const BITMAP*>(b)); menu->insert(*v##_);
#define	MENUITEM_V_BITMAP_EX(v, i, b, f, d)	v##_ = new yacg::MenuItem(i, yacg::MenuItem::t_bitmap, reinterpret_cast<const BITMAP*>(b)); menu->insert(*v##_); v##_->callback((f), (d));

#define	MENUITEM_V_SEPERATOR(v, i)	v##_ = new yacg::MenuItem(i, yacg::MenuItem::t_seperator, true); menu->insert(*v##_);

#define	MENUITEM_V_SUBMENU_BEGIN(v, i, t)	\
{	\
	yacg::Menu* parentMenu = menu.get();	\
	std::auto_ptr<yacg::Menu> menu(new yacg::Menu((i), (t)));	\
	v##_ = menu.get();

#define	MENUITEM_V_SUBMENU_END(v, i)	\
	v##_ = new yacg::MenuItem((i), yacg::MenuItem::t_submenu, *menu.get());	\
	parentMenu->insert(*v##_);	\
	menu.release();	\
}

#endif
