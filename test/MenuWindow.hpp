#ifndef	__MENUWINDOW_HPP__
#define	__MENUWINDOW_HPP__

class MenuWindow
{
public:
	void process()
	{
		_window.menubar1().left();
		_window.menu1().id();
		_window.menuitem1().id();
		_window.ExitButton().callback(exit, this);
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<MenuWindow*>(data)->_window.abort();
	}
	static void action(BITMAP* bmp, yacg::MenuItem& i, void* d)
	{
		std::ostrstream o;
		o << "Menu item: " << i.title() << std::ends;
		allegro_message(o.str());
		o.freeze(false);
	}
	
private:
	class menuWindow : public yacg::_Window
	{
	public:
		menuWindow(BITMAP* bmp = screen)
				:
				_Window(bmp)
		{
			init();
		}
	
	private:
		yacg::Button* ExitButton_;
		yacg::MenuBar* menubar1_;
		yacg::Menu* menu1_;
		yacg::MenuItem* menuitem1_;
		yacg::Menu* menu2_;
		yacg::MenuItem* menuitem2_;
		yacg::MenuItem* menuitem3_;
	
	public:
		yacg::Button& ExitButton() { return *ExitButton_; }
		yacg::MenuBar& menubar1() { return *menubar1_; }
		yacg::Menu& menu1() { return *menu1_; }
		yacg::MenuItem& menuitem1() { return *menuitem1_; }

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			ExitButton_ = new yacg::Button(*this, 803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			MENUBAR_BEGIN(20, 100)
				MENU_BEGIN(0, "One")
					MENUITEM_V_EX(menuitem1, 0, "One", action, 0)
					MENUITEM_V_SEPERATOR(menuitem2, 1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_V_SUBMENU_BEGIN(menu2, 4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_V_SUBMENU_END(menuitem3, 2)
				MENU_V_END(menu1)
				MENU_BEGIN(1, "Two")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
				MENU_BEGIN(2, "Three")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
				MENU_BEGIN(3, "Four")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
			MENUBAR_V_END(menubar1)

			MENUBAR_BEGIN_W(*this)
				MENU_BEGIN(0, "One")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_V_END(menu1)
				MENU_BEGIN(1, "Two")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
				MENU_BEGIN(2, "Three")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
				MENU_BEGIN(3, "Four")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
			MENUBAR_END()
			
			MENUBAR_BEGIN_EX(20, 200, 400, yacg::MenuBar::calc)
				MENU_BEGIN(0, "One")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_V_END(menu1)
				MENU_BEGIN(1, "Two")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
				MENU_BEGIN(2, "Three")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
				MENU_BEGIN(3, "Four")
					MENUITEM(0, "One")
					MENUITEM_SEPERATOR(1)
					MENUITEM(1, "Two")
					MENUITEM_SEPERATOR(1)
					MENUITEM_SUBMENU_BEGIN(4, "Three")
						MENUITEM(2, "One")
						MENUITEM_SEPERATOR(3)
						MENUITEM(4, "Two")
					MENUITEM_SUBMENU_END(2, "Three")
				MENU_END()
			MENUBAR_END()
		}
	} _window;
};

#endif
