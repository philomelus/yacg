#ifndef	__MAINWINDOW_HPP__
#define	__MAINWINDOW_HPP__

#define	ACTION(w, t)	\
static void action_##w(yacg::Button& c, void* data, BITMAP* b)	\
{	\
	yacg::Theme.style = yacg::Configuration::STYLE_FLAT;	\
	t##Window w;	\
	w.process();	\
	yacg::Theme.style = yacg::Configuration::STYLE_3D;	\
	_this->_window.paint();	\
}	\
static void action_##w##3d(yacg::Button& c, void* data, BITMAP* b)	\
{	\
	t##Window w;	\
	w.process();	\
	_this->_window.paint();	\
}

class MainWindow
{
public:
	void process()
	{
		const yacg::Button::CALLBACK actions[] =
		{
			0,
			0,
			action_exit,
			action_toggle,
			action_blackblack,
			action_grayblack,
			action_list,
			action_list3d,
			action_scroll,
			action_scroll3d,
			action_textbox,
			action_textbox3d,
			action_box,
			action_box3d,
			action_tabtophoriz,
			action_tabtophoriz3d,
			action_tabtopvert,
			action_tabtopvert3d,
			action_tabbothoriz,
			action_tabbothoriz3d,
			action_tabbotvert,
			action_tabbotvert3d,
			action_tablefthoriz,
			action_tablefthoriz3d,
			action_tableftvert,
			action_tableftvert3d,
			action_tabrighthoriz,
			action_tabrighthoriz3d,
			action_tabrightvert,
			action_tabrightvert3d,
			action_checkbox,
			action_checkbox3d,
			action_checkedbox,
			action_checkedbox3d,
			action_title,
			action_title3d,
			action_menu,
			action_menu3d,
			action_selectionbox,
			action_selectionbox3d,
			action_primitive,
			action_primitive3d,
			action_dialog1,
			action_dialog13d,
			action_dialog2,
			action_dialog23d,
			action_radio,
			action_radio3d
		};
		const int size = _window.size();
		for (int i = 0; i < size; ++i)
		{
			yacg::_Control& c = _window[i];
			c.active(true);
			if (dynamic_cast<yacg::Button*>(&c) != 0)
				static_cast<yacg::Button&>(c).callback(actions[i], this);
		}
		_window.scroll().left();
		_window.scroll3d().left();
		_windowed = true;
		_window.process_events();
#if 0
		_window.update();
		_running = true;
		_repaint = false;
		while (_running)
		{
			if (!_window.process_events())
				rest(1);
			else
				_window.update();
			if (_repaint)
			{
				_window.paint();
				_repaint = false;
			}
		}
#endif
	}
	
private:
#define	_this	reinterpret_cast<MainWindow*>(data)
	ACTION(box, Box);
	static void action_blackblack(yacg::Button& c, void* data, BITMAP* b)
	{
		yacg::Plane& p = static_cast<yacg::Plane&>(_this->_window[0]);
		p.color(makecol(0, 0, 0));
		p.flat_color(makecol(0, 0, 0));
		_this->_window.paint();
	}
	ACTION(checkbox, Checkbox);
	ACTION(checkedbox, CheckedBox);
	static void action_dialog1(yacg::Button& c, void* data, BITMAP* b)
	{
		yacg::Theme.style = yacg::Configuration::STYLE_FLAT;
		DialogWindow1 w;
		yacg::Theme.style = yacg::Configuration::STYLE_3D;
	}
	static void action_dialog13d(yacg::Button& c, void* data, BITMAP* b)
	{
		DialogWindow1 w(1);
	}
	static void action_dialog2(yacg::Button& c, void* data, BITMAP* b)
	{
		yacg::Theme.style = yacg::Configuration::STYLE_FLAT;
		DialogWindow2 w;
		yacg::Theme.style = yacg::Configuration::STYLE_3D;
	}
	static void action_dialog23d(yacg::Button& c, void* data, BITMAP* b)
	{
		DialogWindow2 w(1, 2);
	}
	static void action_exit(yacg::Button& c, void* data, BITMAP* b)
	{
		_this->_window.abort();
	}
	static void action_grayblack(yacg::Button& c, void* data, BITMAP* b)
	{
		yacg::Plane& p = static_cast<yacg::Plane&>(_this->_window[0]);
		p.color(makecol(212, 208, 200));
		p.flat_color(makecol(0, 0, 0));
		_this->_window.paint();
	}
	ACTION(list, List);
	ACTION(menu, Menu);
	ACTION(primitive, Primitive);
	ACTION(radio, Radio);
	ACTION(scroll, Scroll);
	ACTION(selectionbox, SelectionBox);
	ACTION(tabbothoriz, TabBotHoriz);
	ACTION(tabbotvert, TabBotVert);
	ACTION(tablefthoriz, TabLeftHoriz);
	ACTION(tableftvert, TabLeftVert);
	ACTION(tabrighthoriz, TabRightHoriz);
	ACTION(tabrightvert, TabRightVert);
	ACTION(tabtophoriz, TabTopHoriz);
	ACTION(tabtopvert, TabTopVert);
	ACTION(textbox, Textbox);
	ACTION(title, Title);
	static void action_toggle(yacg::Button& c, void* data, BITMAP* b)
	{
		if (_this->_windowed)
		{
			_this->_windowed = false;
			set_gfx_mode(GFX_AUTODETECT, 1024, 768, 0, 0);
			_this->_window.bitmap(screen);
			_this->_window.paint();
		}
		else
		{
			_this->_windowed = true;
			set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0);
			_this->_window.bitmap(screen);
			_this->_window.paint();
		}
	}
#undef	_this

private:
	class mainWindow : public yacg::_Window
	{
	public:
		mainWindow(BITMAP* bmp = screen)
				:
				_Window(bmp)
		{
			init();
		}
		yacg::Button& scroll()
		{
			return *scroll_;
		}
		yacg::Button& scroll3d()
		{
			return *scroll3d_;
		}
	
	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			TITLE_EX(0, 0, 1024, yacg::Title::calc, 0, "GUI Test Application")
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			insert(*new yacg::Button(*this, 803, 697, 200, 20, "Toggle Windowed Mode"));
			insert(*new yacg::Button(*this, 583, 727, 200, 20, "Black / Black"));
			insert(*new yacg::Button(*this, 583, 697, 200, 20, "Gray / Black"));
			list_ = new yacg::Button(*this,  20, 30, 200, 20, "List"); insert(*list_);
			list3d_ = new yacg::Button(*this,  240, 30, 200, 20, "List 3D"); insert(*list3d_);
			scroll_ = new yacg::Button(*this,  20, 60, 200, 20, "Scroll"); insert(*scroll_);
			scroll3d_ = new yacg::Button(*this,  240, 60, 200, 20); insert(*scroll3d_);
			scroll3d_->title("Scroll 3D");
			insert(*new yacg::Button(*this, 20, 90, 200, 20, "Textbox"));
			insert(*new yacg::Button(*this, 240, 90, 200, 20, "Textbox 3D"));
			insert(*new yacg::Button(*this, 20, 120, 200, 20, "Box"));
			insert(*new yacg::Button(*this, 240, 120, 200, 20, "Box 3D"));
			insert(*new yacg::Button(*this, 520, 30, 200, 20, "Tab Top Horiz"));
			insert(*new yacg::Button(*this, 740, 30, 200, 20, "Tab Top Horiz 3D"));
			insert(*new yacg::Button(*this, 520, 60, 200, 20, "Tab Top Vert"));
			insert(*new yacg::Button(*this, 740, 60, 200, 20, "Tab Top Vert 3D"));
			insert(*new yacg::Button(*this, 520, 90, 200, 20, "Tab Bottom Horiz"));
			insert(*new yacg::Button(*this, 740, 90, 200, 20, "Tab Bottom Horiz 3D"));
			insert(*new yacg::Button(*this, 520, 120, 200, 20, "Tab Bottom Vert"));
			insert(*new yacg::Button(*this, 740, 120, 200, 20, "Tab Bottom Vert 3D"));
			insert(*new yacg::Button(*this, 520, 150, 200, 20, "Tab Left Horiz"));
			insert(*new yacg::Button(*this, 740, 150, 200, 20, "Tab Left Horiz 3D"));
			insert(*new yacg::Button(*this, 520, 180, 200, 20, "Tab Left Vert"));
			insert(*new yacg::Button(*this, 740, 180, 200, 20, "Tab Left Vert 3D"));
			insert(*new yacg::Button(*this, 520, 210, 200, 20, "Tab Right Horiz"));
			insert(*new yacg::Button(*this, 740, 210, 200, 20, "Tab Right Horiz 3D"));
			insert(*new yacg::Button(*this, 520, 240, 200, 20, "Tab Right Vert"));
			insert(*new yacg::Button(*this, 740, 240, 200, 20, "Tab Right Vert 3D"));
			insert(*new yacg::Button(*this, 20, 150, 200, 20, "Checkbox"));
			insert(*new yacg::Button(*this, 240, 150, 200, 20, "Checkbox 3D"));
			insert(*new yacg::Button(*this, 20, 180, 200, 20, "CheckedBox"));
			insert(*new yacg::Button(*this, 240, 180, 200, 20, "CheckedBox 3D"));
			insert(*new yacg::Button(*this, 20, 210, 200, 20, "Title"));
			insert(*new yacg::Button(*this, 240, 210, 200, 20, "Title 3D"));
			insert(*new yacg::Button(*this, 20, 240, 200, 20, "Menu"));
			insert(*new yacg::Button(*this, 240, 240, 200, 20, "Menu 3D"));
			insert(*new yacg::Button(*this, 20, 270, 200, 20, "Selection Box"));
			insert(*new yacg::Button(*this, 240, 270, 200, 20, "Selection Box 3D"));
			insert(*new yacg::Button(*this, 20, 300, 200, 20, "Primitive"));
			insert(*new yacg::Button(*this, 240, 300, 200, 20, "Primitive 3D"));
			insert(*new yacg::Button(*this, 20, 330, 200, 20, "Dialog 1"));
			insert(*new yacg::Button(*this, 240, 330, 200, 20, "Dialog 1 3D"));
			insert(*new yacg::Button(*this, 20, 360, 200, 20, "Dialog 2"));
			insert(*new yacg::Button(*this, 240, 360, 200, 20, "Dialog 2 3D"));
			insert(*new yacg::Button(*this, 20, 390, 200, 20, "Radio"));
			insert(*new yacg::Button(*this, 240, 390, 200, 20, "Radio 3D"));
		}
	private:
		yacg::Button* list_;
		yacg::Button* list3d_;
		yacg::Button* scroll_;
		yacg::Button* scroll3d_;
	} _window;

private:
	bool _windowed;
};

#endif
