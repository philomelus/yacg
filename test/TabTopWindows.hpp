//=============================================================================
// class TabTopHorizWindow
//=============================================================================

class TabTopHorizWindow : public _TabWindowBase<TabTopHorizWindow>
{
public:
	void process()
	{
		_TabWindowBase<TabTopHorizWindow>::process(_window);
	}
	
private:
	class TabTest : public yacg::_Window
	{
	public:
		TabTest(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			TAB_BEGIN_EX(20, 20, 482, 334, yacg::Tab::l_top | yacg::Tab::o_horizontal | yacg::Tab::a_left | yacg::Tab::w_equal | yacg::Tab::t_title, "Left, Equal")
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
			TAB_BEGIN_EX(522, 20, 482, 334, yacg::Tab::l_top | yacg::Tab::o_horizontal | yacg::Tab::a_left | yacg::Tab::w_title | yacg::Tab::t_title, "Left, Title")
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
			TAB_BEGIN_EX(20, 374, 482, 334, yacg::Tab::l_top | yacg::Tab::o_horizontal | yacg::Tab::a_right | yacg::Tab::w_equal | yacg::Tab::t_title, "Right, Equal")
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
			TAB_BEGIN_EX(522, 374, 482, 334, yacg::Tab::l_top | yacg::Tab::o_horizontal | yacg::Tab::a_right | yacg::Tab::w_title | yacg::Tab::t_title, "Right, Title")
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

	friend _TabWindowBase<TabTopHorizWindow>;
};

//=============================================================================
// class TabTopVertWindow
//=============================================================================

class TabTopVertWindow : public _TabWindowBase<TabTopVertWindow>
{
public:
	void process()
	{
		_TabWindowBase<TabTopVertWindow>::process(_window);
	}
	
private:
	class TabTest : public yacg::_Window
	{
	public:
		TabTest(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			TAB_BEGIN_EX(20, 20, 482, 334, yacg::Tab::l_top | yacg::Tab::o_vertical | yacg::Tab::a_left | yacg::Tab::w_equal | yacg::Tab::t_title, "Left, Equal")
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
			TAB_BEGIN_EX(522, 20, 482, 334, yacg::Tab::l_top | yacg::Tab::o_vertical | yacg::Tab::a_left | yacg::Tab::w_title | yacg::Tab::t_title, "Left, Title")
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
			TAB_BEGIN_EX(20, 374, 482, 334, yacg::Tab::l_top | yacg::Tab::o_vertical | yacg::Tab::a_right | yacg::Tab::w_equal | yacg::Tab::t_title, "Right, Equal")
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
			TAB_BEGIN_EX(522, 374, 482, 334, yacg::Tab::l_top | yacg::Tab::o_vertical | yacg::Tab::a_right | yacg::Tab::w_title | yacg::Tab::t_title, "Right, Title")
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

	friend _TabWindowBase<TabTopVertWindow>;
};
