#ifndef	__CHECKEDBOXWINDOW_HPP__
#define	__CHECKEDBOXWINDOW_HPP__

class CheckedBoxWindow
{
public:
	void process()
	{
		_window.cb2().left();
		_window.ExitButton().callback(exit, this);
		const int size = _window.size();
		for (int i = 2; i < size; ++i)
		{
			yacg::_Control& c = _window[i];
			if (dynamic_cast<yacg::Textbox*>(&c) != 0)
				static_cast<yacg::Textbox&>(c).text(_text);
		}
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<CheckedBoxWindow*>(data)->_window.abort();
	}
		
private:
	class checkedBoxWindow : public yacg::_Window
	{
	public:
		checkedBoxWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::CheckedBox& cb2()
		{
			return *cb2_;
		}
		yacg::Textbox& tb2()
		{
			return *tb2_;
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
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this,  20, 20, 230, 230, 0, "Left"));
				cb1_ = pC.get();
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this,  270, 20, 230, 230, yacg::CheckedBox::b_button, "Left + Button"));
				cb2_ = pC.get();
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this, 520, 20, 230, 230, yacg::CheckedBox::h_right, "Right"));
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this, 770, 20, 230, 230, yacg::CheckedBox::h_right | yacg::CheckedBox::b_button, "Right + Button"));
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this, 20, 270, 230, 230, yacg::CheckedBox::v_bottom, "Left + Bottom"));
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this, 270, 270, 230, 230, yacg::CheckedBox::v_bottom | yacg::CheckedBox::b_button, "Left + Bottom + Button"));
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this, 520, 270, 230, 230, yacg::CheckedBox::h_right | yacg::CheckedBox::v_bottom, "Right + Bottom"));
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

			{
				std::auto_ptr<yacg::CheckedBox> pC(new yacg::CheckedBox(*this, 770, 270, 230, 230, yacg::CheckedBox::h_right | yacg::CheckedBox::v_bottom | yacg::CheckedBox::b_button, "Right + Bottom + Button"));
				insert(*pC.get());
				const int LC = pC->left_controls();
				const int TC = pC->top_controls();
				const int RC = pC->right_controls();
				const int BC = pC->bottom_controls();
				const int WC = pC->width_controls();
				const int HC = pC->height_controls();
				const int start_size = size();

				TEXTBOX(LC + 10, TC + 10, WC - 20, HC - 20, yacg::Textbox::s_both | yacg::Textbox::f_in)
				const int end_size = size();
				for (int n = start_size; n < end_size; ++n)
					pC->insert((*this)[n]);
				pC.release();
			}

		}
	private:
		yacg::Button* ExitButton_;
		yacg::CheckedBox* cb1_;
		yacg::Textbox* tb1_;
		yacg::CheckedBox* cb2_;
		yacg::Textbox* tb2_;
	} _window;

private:
	static const char* _text;
};

#endif
