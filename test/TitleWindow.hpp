#ifndef	__TITLEWINDOW_HPP__
#define	__TITLEWINDOW_HPP__

class TitleWindow
{
public:
	void process()
	{
		_window.t1().left();
		_window.ExitButton().callback(exit, this);
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<TitleWindow*>(data)->_window.abort();
	}
		
private:
	class titleWindow : public yacg::_Window
	{
	public:
		titleWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::Button& ExitButton()
		{
			return *ExitButton_;
		}
		yacg::Title& t1()
		{
			return *t1_;
		}

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			ExitButton_ = new yacg::Button(*this, 803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			insert(*new yacg::Box(*this, 18, 18, 404, 34, yacg::Box::f_flat));
			TITLE_V_EX(t1, 20, 20, 400, 30, 0, "default")
			insert(*new yacg::Box(*this, 18, 58, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 60, 400, 30, yacg::Title::h_center | yacg::Title::v_center | yacg::Title::f_none, "defaults (specified)")
			insert(*new yacg::Box(*this, 18, 98, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 100, 400, 30, yacg::Title::h_center | yacg::Title::v_top | yacg::Title::f_out, "center, top, out")
			insert(*new yacg::Box(*this, 18, 138, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 140, 400, 30, yacg::Title::h_center | yacg::Title::v_bottom | yacg::Title::f_in, "center, bottom, in")
			insert(*new yacg::Box(*this, 18, 178, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 180, 400, 30, yacg::Title::h_left | yacg::Title::v_center | yacg::Title::f_flat, "left, center, flat")
			insert(*new yacg::Box(*this, 18, 218, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 220, 400, 30, yacg::Title::h_left | yacg::Title::v_top | yacg::Title::f_none, "left, top, none")
			insert(*new yacg::Box(*this, 18, 258, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 260, 400, 30, yacg::Title::h_left | yacg::Title::v_bottom | yacg::Title::f_out, "left, bottom, out")
			insert(*new yacg::Box(*this, 18, 298, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 300, 400, 30, yacg::Title::h_right | yacg::Title::v_center | yacg::Title::f_in, "right, center, in")
			insert(*new yacg::Box(*this, 18, 338, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 340, 400, 30, yacg::Title::h_right | yacg::Title::v_top | yacg::Title::f_flat, "right, top, flat")
			insert(*new yacg::Box(*this, 18, 378, 404, 34, yacg::Box::f_flat));
			TITLE_EX(20, 380, 400, 30, yacg::Title::h_right | yacg::Title::v_bottom | yacg::Title::f_none, "right, bottom, none")
		}

	private:
		yacg::Button* ExitButton_;
		yacg::Title* t1_;
		yacg::Title* t2_;
	} _window;
};

#endif
