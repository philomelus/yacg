#ifndef	__CHECKBOXWINDOW_HPP__
#define	__CHECKBOXWINDOW_HPP__

class CheckboxWindow
{
public:
	void process()
	{
		const int disable[] =
		{
			4,
			5,
			8,
			9,
			13,
			14,
			15,
			19,
			20,
			21,
			24,
			25,
			28,
			29,
			32,
			33,
			-1
		};
		_window.nf3().left();
		_window.nf4().left();
		const int size = _window.size();
		_window[0].active(true);
		yacg::Button& b = static_cast<yacg::Button&>(_window[1]);
		b.callback(exit, this);
		for (int i = 0; disable[i] != -1; ++i)
			static_cast<yacg::Checkbox&>(_window[disable[i]]).active(false);
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<CheckboxWindow*>(data)->_window.abort();
	}
		
private:
	class checkboxWindow : public yacg::_Window
	{
	public:
		checkboxWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::Checkbox& nf3()
		{
			return *nf3_;
		}
		yacg::Checkbox& nf4()
		{
			return *nf4_;
		}

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			nf1_ = new yacg::Checkbox(*this,  20, 20, yacg::Checkbox::calc, yacg::Checkbox::calc, 0, "No flags"); insert(*nf1_);
			nf2_ = new yacg::Checkbox(*this,  320, 20, yacg::Checkbox::calc, yacg::Checkbox::calc, 0, "No flags"); insert(*nf2_);
			*nf2_ = true;
			nf3_ = new yacg::Checkbox(*this,  40, 50, yacg::Checkbox::calc, yacg::Checkbox::calc, 0, "No flags"); insert(*nf3_);
			nf4_ = new yacg::Checkbox(*this,  340, 50, yacg::Checkbox::calc, yacg::Checkbox::calc, 0, "No flags"); insert(*nf4_);
			*nf4_ = true;
			
			insert(*new yacg::Checkbox(*this,20, 130, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::t_lefttitle, "Left Title"));
			dummy_ = new yacg::Checkbox(*this,  320, 130, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::t_lefttitle, "Left Title"); insert(*dummy_);
			*dummy_ = true;
			insert(*new yacg::Checkbox(*this,40, 160, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::t_lefttitle, "Left Title"));
			dummy_ = new yacg::Checkbox(*this,  340, 160, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::t_lefttitle, "Left Title"); insert(*dummy_);
			*dummy_ = true;
			
			insert(*new yacg::Checkbox(*this,20, 240, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate, "Tristate"));
			dummy_ = new yacg::Checkbox(*this,  320, 240, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate, "Tristate"); insert(*dummy_);
			*dummy_ = true;
			dummy_ = new yacg::Checkbox(*this,  620, 240, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate, "Tristate"); insert(*dummy_);
			*dummy_ = yacg::Checkbox::gray;
			insert(*new yacg::Checkbox(*this,40, 270, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate, "Tristate"));
			dummy_ = new yacg::Checkbox(*this,  340, 270, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate, "Tristate"); insert(*dummy_);
			*dummy_ = true;
			dummy_ = new yacg::Checkbox(*this,  640, 270, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate, "Tristate"); insert(*dummy_);
			*dummy_ = yacg::Checkbox::gray;
			
			insert(*new yacg::Checkbox(*this,20, 350, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate | yacg::Checkbox::t_lefttitle, "Left Title + Tristate"));
			dummy_ = new yacg::Checkbox(*this,  320, 350, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate | yacg::Checkbox::t_lefttitle, "Left Title + Tristate"); insert(*dummy_);
			*dummy_ = true;
			dummy_ = new yacg::Checkbox(*this,  620, 350, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate | yacg::Checkbox::t_lefttitle, "Left Title + Tristate"); insert(*dummy_);
			*dummy_ = yacg::Checkbox::gray;
			insert(*new yacg::Checkbox(*this,40, 380, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate | yacg::Checkbox::t_lefttitle, "Left Title + Tristate"));
			dummy_ = new yacg::Checkbox(*this,  340, 380, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate | yacg::Checkbox::t_lefttitle, "Left Title + Tristate"); insert(*dummy_);
			*dummy_ = true;
			dummy_ = new yacg::Checkbox(*this,  640, 380, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::s_tristate | yacg::Checkbox::t_lefttitle, "Left Title + Tristate"); insert(*dummy_);
			*dummy_ = yacg::Checkbox::gray;

			insert(*new yacg::Checkbox(*this,20, 460, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_button, "Button"));
			dummy_ = new yacg::Checkbox(*this,  320, 460, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_button, "Button"); insert(*dummy_);
			*dummy_ = true;
			insert(*new yacg::Checkbox(*this,40, 490, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_button, "Button"));
			dummy_ = new yacg::Checkbox(*this,  340, 490, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_button, "Button"); insert(*dummy_);
			*dummy_ = true;

			insert(*new yacg::Checkbox(*this,20, 570, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside, "Button + outside"));
			dummy_ = new yacg::Checkbox(*this,  320, 570, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside, "Button + outside"); insert(*dummy_);
			*dummy_ = true;
			insert(*new yacg::Checkbox(*this,40, 600, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside, "Button + outside"));
			dummy_ = new yacg::Checkbox(*this,  340, 600, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside, "Button + outside"); insert(*dummy_);
			*dummy_ = true;

			insert(*new yacg::Checkbox(*this,20, 680, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside | yacg::Checkbox::t_lefttitle, "Button + outside + left"));
			dummy_ = new yacg::Checkbox(*this,  320, 680, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside | yacg::Checkbox::t_lefttitle, "Button + outside + left"); insert(*dummy_);
			*dummy_ = true;
			insert(*new yacg::Checkbox(*this,40, 710, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside | yacg::Checkbox::t_lefttitle, "Button + outside + left"));
			dummy_ = new yacg::Checkbox(*this,  340, 710, yacg::Checkbox::calc, yacg::Checkbox::calc, yacg::Checkbox::b_buttonoutside | yacg::Checkbox::t_lefttitle, "Button + outside + left"); insert(*dummy_);
			*dummy_ = true;
		}

	private:
		yacg::Checkbox* dummy_;
		yacg::Checkbox* nf1_;
		yacg::Checkbox* nf2_;
		yacg::Checkbox* nf3_;
		yacg::Checkbox* nf4_;
	} _window;
};

#endif
