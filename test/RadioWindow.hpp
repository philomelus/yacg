#ifndef	__RADIOWINDOW_HPP__
#define	__RADIOWINDOW_HPP__

class RadioWindow
{
public:
	void process()
	{
		_window.ExitButton().callback(exit, this);
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<RadioWindow*>(data)->_window.abort();
	}
		
private:
	class radioWindow : public yacg::_Window
	{
	public:
		radioWindow(BITMAP* bmp = screen)
				:
				yacg::_Window(bmp)
		{
			init();
		}
		~radioWindow()
		{
			delete _radioGroup1;
			delete _radioGroup2;
			delete _radioGroup3;
		}
		yacg::Button& ExitButton()
		{
			return *ExitButton_;
		}

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			ExitButton_ = new yacg::Button(*this,  803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			{
				const char* radioTitles[] = 
				{
					{ "Radio 1" },
					{ "Radio 2" },
					{ "Radio 3" },
					{ "Radio 4" },
					{ "Radio 5" }
				};
				_radioGroup1 = new yacg::RadioGroup(*this, 20, 20, yacg::Radio::calc, yacg::Radio::calc,
						"Group 1", yacg::RadioGroup::f_flat | yacg::RadioGroup::t_title
						| yacg::RadioGroup::a_topleft);
				_radioGroup1->insert(5, radioTitles);
			}
			_radioGroup2 = new yacg::RadioGroup(*this);
			{
				yacg::Radio* r = new yacg::Radio(*this, 400, 20, "Radio 1");
				_radioGroup2->insert(*r);
				r = new yacg::Radio(*this, 400, 40, "Radio 2", yacg::Radio::t_lefttext);
				_radioGroup2->insert(*r);
				r = new yacg::Radio(*this, 400, 60, "Radio 3");
				_radioGroup2->insert(*r);
				r = new yacg::Radio(*this, 400, 100, yacg::Radio::calc, yacg::Radio::calc, "Radio 4", yacg::Radio::t_lefttext);
				_radioGroup2->insert(*r);
				r = new yacg::Radio(*this, 400, 100, yacg::Radio::calc, yacg::Radio::calc, "Radio 5");
				_radioGroup2->insert(*r);
			}
			{
				const char* radioTitles[] = 
				{
					{ "Radio 1" },
					{ "Radio 2" },
					{ "Radio 3" },
					{ "Radio 4" },
					{ "Radio 5" },
					{ "Radio 6" },
					{ "Radio 7" },
					{ "Radio 8" },
					{ "Radio 9" },
					{ "Radio 10" }
				};
				_radioGroup3 = new yacg::RadioGroup(*this, 20, 400, yacg::Radio::calc, yacg::Radio::calc,
						"Group 3", yacg::RadioGroup::f_flat | yacg::RadioGroup::t_title
						| yacg::RadioGroup::a_topleft | yacg::RadioGroup::r_lefttext);
				_radioGroup3->insert(10, radioTitles);
			}
		}
	
	private:
		yacg::Button* ExitButton_;
		yacg::RadioGroup* _radioGroup1;
		yacg::RadioGroup* _radioGroup2;
		yacg::RadioGroup* _radioGroup3;
	} _window;
};

#endif
