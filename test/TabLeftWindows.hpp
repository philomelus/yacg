//=============================================================================
// class TabLeftHorizWindow
//=============================================================================

class TabLeftHorizWindow : public _TabWindowBase<TabLeftHorizWindow>
{
public:
	void process()
	{
		_window.tab1().left();
		_window.page3().title();
		_TabWindowBase<TabLeftHorizWindow>::process(_window);
	}
	
private:
	class tabLeftHorizWindow : public yacg::_Window
	{
	public:
		tabLeftHorizWindow(BITMAP* bmp = screen)
				:
				_Window(bmp)
		{
			init();
		}
		yacg::Tab& tab1()
		{
			return *tab1_;
		}
		yacg::TabPage& page3()
		{
			return *page3_;
		}
		
	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			TAB_V_BEGIN_EX(tab1, 20, 20, 482, 334, yacg::Tab::l_left | yacg::Tab::o_horizontal | yacg::Tab::a_top | yacg::Tab::w_equal | yacg::Tab::t_title, "Top, Equal")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_V_BEGIN(page1, "Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_V_BEGIN(page2, "Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_V_BEGIN(page3, "Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
			TAB_BEGIN_EX(522, 20, 482, 334, yacg::Tab::l_left | yacg::Tab::o_horizontal | yacg::Tab::a_top | yacg::Tab::w_title | yacg::Tab::t_title, "Top, Title")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
			TAB_BEGIN_EX(20, 374, 482, 334, yacg::Tab::l_left | yacg::Tab::o_horizontal | yacg::Tab::a_bottom | yacg::Tab::w_equal | yacg::Tab::t_title, "Bottom, Equal")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
			TAB_BEGIN_EX(522, 374, 482, 334, yacg::Tab::l_left | yacg::Tab::o_horizontal | yacg::Tab::a_bottom | yacg::Tab::w_title | yacg::Tab::t_title, "Bottom, Title")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
		}

	private:
		yacg::Tab* tab1_;
		yacg::TabPage* page1_;
		yacg::TabPage* page2_;
		yacg::TabPage* page3_;
	} _window;

	friend _TabWindowBase<TabLeftHorizWindow>;
};

//=============================================================================
// class TabLeftVertWindow
//=============================================================================

class TabLeftVertWindow : public _TabWindowBase<TabLeftVertWindow>
{
public:
	void process()
	{
		_TabWindowBase<TabLeftVertWindow>::process(_window);
	}
	
private:
	class tabLeftVertWindow : public yacg::_Window
	{
	public:
		tabLeftVertWindow(BITMAP* bmp = screen)
				:
				_Window(bmp)
		{
			init();
		}
	
	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			TAB_BEGIN_EX(20, 20, 482, 334, yacg::Tab::l_left | yacg::Tab::o_vertical | yacg::Tab::a_top | yacg::Tab::w_equal | yacg::Tab::t_title, "Top, Equal")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
			TAB_BEGIN_EX(522, 20, 482, 334, yacg::Tab::l_left | yacg::Tab::o_vertical | yacg::Tab::a_top | yacg::Tab::w_title | yacg::Tab::t_title, "Top, Title")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
			TAB_BEGIN_EX(20, 374, 482, 334, yacg::Tab::l_left | yacg::Tab::o_vertical | yacg::Tab::a_bottom | yacg::Tab::w_equal | yacg::Tab::t_title, "Bottom, Equal")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
			TAB_BEGIN_EX(522, 374, 482, 334, yacg::Tab::l_left | yacg::Tab::o_vertical | yacg::Tab::a_bottom | yacg::Tab::w_title | yacg::Tab::t_title, "Bottom, Title")
				TABCOMMON_BEGIN()
					insert(*new yacg::Button(*this, RC - 70, BC - 30, 60, 20, "Common"));
				TABCOMMON_END()
				TABPAGE_BEGIN("Sm")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 1")
				TABPAGE_END()
				TABPAGE_BEGIN("Longer")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 2")
				TABPAGE_END()
				TABPAGE_BEGIN("Very Very Long")
					TITLE_EX(RC - 200, TC + 10, yacg::Title::calc, yacg::Title::calc, 0, "Page 3")
				TABPAGE_END()
			TAB_END()
		}
	} _window;

	friend _TabWindowBase<TabLeftVertWindow>;
};

