#ifndef	__SCROLLWINDOW_HPP__
#define	__SCROLLWINDOW_HPP__

#pragma warning(push)
#pragma warning(disable:4800)

class ScrollWindow
{
public:
	void process()
	{
		static const int defaults[48][7] =
		{
			// range, line, setting, page, initial delay, repeat delay, enabled
			{ 16, 1, 0, 9, 30, 12, true },
			{ 320, 8, 0, 24, 30, 12, true },
			{ 15, 3, 0, 6, 30, 12, true },
			{ 20, 4, 0, 8, 30, 12, true },
			{ 25, 5, 0, 10, 30, 12, true },
			{ 30, 6, 0, 12, 30, 12, true },
			{ 35, 7, 0, 14, 30, 12, true },
			{ 40, 8, 0, 16, 30, 12, true },
			{ 45, 9, 0, 18, 30, 12, true },
			{ 50, 10, 0, 20, 30, 12, true },
			{ 21, 1, 0, 2, 30, 12, true },
			{ 32, 2, 0, 4, 30, 12, true },
			{ 43, 3, 0, 6, 30, 12, true },
			{ 54, 4, 0, 8, 30, 12, true },
			{ 65, 5, 0, 10, 30, 12, true },
			{ 76, 6, 0, 12, 30, 12, true },
			{ 87, 7, 0, 14, 30, 12, true },
			{ 98, 8, 0, 16, 30, 12, true },
			{ 109, 9, 0, 18, 30, 12, true },
			{ 120, 10, 0, 20, 30, 12, true },
			{ 0, 1, 0, 2, 30, 12, true },
			{ 1, 2, 0, 4, 30, 12, true },
			{ 0, 1, 0, 2, 30, 12, false },
			{ 1, 2, 0, 4, 30, 12, false },
			{ 16, 1, 0, 9, 30, 12, true },	// Vertical
			{ 320, 8, 0, 24, 30, 12, true },
			{ 15, 3, 0, 6, 30, 12, true },
			{ 20, 4, 0, 8, 30, 12, true },
			{ 25, 5, 0, 10, 30, 12, true },
			{ 30, 6, 0, 12, 30, 12, true },
			{ 35, 7, 0, 14, 30, 12, true },
			{ 40, 8, 0, 16, 30, 12, true },
			{ 45, 9, 0, 18, 30, 12, true },
			{ 50, 10, 0, 20, 30, 12, true },
			{ 21, 1, 0, 2, 30, 12, true },
			{ 32, 2, 0, 4, 30, 12, true },
			{ 43, 3, 0, 6, 30, 12, true },
			{ 54, 4, 0, 8, 30, 12, true },
			{ 65, 5, 0, 10, 30, 12, true },
			{ 76, 6, 0, 12, 30, 12, true },
			{ 87, 7, 0, 14, 30, 12, true },
			{ 98, 8, 0, 16, 30, 12, true },
			{ 109, 9, 0, 18, 30, 12, true },
			{ 120, 10, 0, 20, 30, 12, true },
			{ 0, 1, 0, 2, 30, 12, true },
			{ 1, 2, 0, 4, 30, 12, true },
			{ 0, 1, 0, 2, 30, 12, false },
			{ 1, 2, 0, 4, 30, 12, false }
		};
		
		const int size = _window.size();
		_window.ExitButton().callback(exit, this);
		for (int i = 2; i < 50; ++i)
		{
			yacg::Scroll& s = static_cast<yacg::Scroll&>(_window[i]);
			s.range(defaults[i - 2][0]);
			s.line(defaults[i - 2][1]);
			s.setting(defaults[i - 2][2]);
			s.page(defaults[i - 2][3]);
			s.delay(defaults[i - 2][4], defaults[i - 2][5]);
			s.active(defaults[i - 2][6]);
		}
		_window.process_events();
	}
	void dump()
	{
		_window.dump();
	}
	
private:	
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<ScrollWindow*>(data)->_window.abort();
	}

private:
	class scrollWindow : public yacg::_Window
	{
	public:
		scrollWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::Button& ExitButton()
		{
			return *ExitButton_;
		}
		yacg::Scroll& scroll1()
		{
			return *scroll1_;
		}
		
	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			ExitButton_ = new yacg::Button(*this, 803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			scroll1_ = new yacg::Scroll(*this, 20, 20, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame);
			insert(*scroll1_);
			scroll1_->range(0);
			insert(*new yacg::Scroll(*this, 20, 50, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 80, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 110, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 140, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 170, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 200, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 230, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 260, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 290, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 320, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 350, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 380, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 410, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 440, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 470, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 500, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 530, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 560, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 590, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 620, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 650, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 20, 680, 300, yacg::Scroll::t_horizontal | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 20, 710, 300, yacg::Scroll::t_horizontal));
			insert(*new yacg::Scroll(*this, 500, 20, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 530, 20, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 560, 20, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 590, 20, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 620, 20, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 650, 20, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 680, 20, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 710, 20, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 740, 20, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 770, 20, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 800, 20, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 830, 20, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 500, 365, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 530, 365, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 560, 365, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 590, 365, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 620, 365, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 650, 365, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 680, 365, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 710, 365, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 740, 365, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 770, 365, 325, yacg::Scroll::t_vertical));
			insert(*new yacg::Scroll(*this, 800, 365, 325, yacg::Scroll::t_vertical | yacg::Scroll::o_frame));
			insert(*new yacg::Scroll(*this, 830, 365, 325, yacg::Scroll::t_vertical));
			TITLE_EX(330, 20, yacg::Title::calc, yacg::Title::calc, 0, " 16 x  1 x  9")
			TITLE_EX(330, 50, yacg::Title::calc, yacg::Title::calc, 0, "320 x  8 x  24")
			TITLE_EX(330, 80, yacg::Title::calc, yacg::Title::calc, 0, " 15 x  3 x  6")
			TITLE_EX(330, 110, yacg::Title::calc, yacg::Title::calc, 0, " 20 x  4 x  8")
			TITLE_EX(330, 140, yacg::Title::calc, yacg::Title::calc, 0, " 25 x  5 x 10")
			TITLE_EX(330, 170, yacg::Title::calc, yacg::Title::calc, 0, " 30 x  6 x 12")
			TITLE_EX(330, 200, yacg::Title::calc, yacg::Title::calc, 0, " 35 x  7 x 14")
			TITLE_EX(330, 230, yacg::Title::calc, yacg::Title::calc, 0, " 40 x  8 x 16")
			TITLE_EX(330, 260, yacg::Title::calc, yacg::Title::calc, 0, " 45 x  9 x 18")
			TITLE_EX(330, 290, yacg::Title::calc, yacg::Title::calc, 0, " 50 x 10 x 20")
			TITLE_EX(330, 320, yacg::Title::calc, yacg::Title::calc, 0, " 21 x  1 x  2")
			TITLE_EX(330, 350, yacg::Title::calc, yacg::Title::calc, 0, " 32 x  2 x  4")
			TITLE_EX(330, 380, yacg::Title::calc, yacg::Title::calc, 0, " 43 x  3 x  6")
			TITLE_EX(330, 410, yacg::Title::calc, yacg::Title::calc, 0, " 54 x  4 x  8")
			TITLE_EX(330, 440, yacg::Title::calc, yacg::Title::calc, 0, " 65 x  5 x 10")
			TITLE_EX(330, 470, yacg::Title::calc, yacg::Title::calc, 0, " 76 x  6 x 12")
			TITLE_EX(330, 500, yacg::Title::calc, yacg::Title::calc, 0, " 87 x  7 x 14")
			TITLE_EX(330, 530, yacg::Title::calc, yacg::Title::calc, 0, " 98 x  8 x 16")
			TITLE_EX(330, 560, yacg::Title::calc, yacg::Title::calc, 0, "109 x  9 x 18")
			TITLE_EX(330, 590, yacg::Title::calc, yacg::Title::calc, 0, "120 x 10 x 20")
			TITLE_EX(330, 620, yacg::Title::calc, yacg::Title::calc, 0, "  0 x  1 x  2")
			TITLE_EX(330, 650, yacg::Title::calc, yacg::Title::calc, 0, "  1 x  2 x  4")
			TITLE_EX(330, 680, yacg::Title::calc, yacg::Title::calc, 0, "  0 x  1 x  2")
			TITLE_EX(330, 710, yacg::Title::calc, yacg::Title::calc, 0, "  1 x  2 x  4")
		}
	
	private:
		yacg::Button* ExitButton_;
		yacg::Scroll* scroll1_;
	} _window;
};

#pragma warning(pop)

#endif
