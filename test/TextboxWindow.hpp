#ifndef	__TEXTBOXWINDOW_HPP__
#define	__TEXTBOXWINDOW_HPP__

class TextboxWindow
{
public:
	void process()
	{
		_window.tb2().left();
		const int size = _window.size();
		static_cast<yacg::Button&>(_window[1]).callback(exit, this);
		for (int i = 2; i < size; ++i)
		{
			yacg::Textbox& c = static_cast<yacg::Textbox&>(_window[i]);
			c.active(true);
			c.text(_text);
			if (yacg::Theme.style != yacg::Configuration::STYLE_FLAT)
			{
				if ((c.flags() & yacg::Textbox::f_mask) == yacg::Textbox::f_in)
					c.background(makecol(255, 255, 255));
			}
		}
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<TextboxWindow*>(data)->_window.abort();
	}
		
private:
	class textboxWindow : public yacg::_Window
	{
	public:
		textboxWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::Textbox& tb2()
		{
			return *tb2_;
		}

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			insert(*new yacg::Button(*this, 803, 727, 200, 20, "Exit"));
			TEXTBOX_V(tb1, 20, 40, 300, 100, 0);
			TEXTBOX_V_EX(tb2, 20, 160, 300, 100, yacg::Textbox::s_horizontal, "Testing 1, 2, 3");
			TEXTBOX_EX(20, 280, 300, 100, yacg::Textbox::s_vertical, "Testing, 1, 2, 3");
			TEXTBOX(20, 400, 300, 100, yacg::Textbox::s_horizontal | yacg::Textbox::s_vertical);
			TEXTBOX(340, 40, 300, 100, yacg::Textbox::f_in);
			TEXTBOX(340, 160, 300, 100, yacg::Textbox::f_in | yacg::Textbox::s_horizontal);
			TEXTBOX(340, 280, 300, 100, yacg::Textbox::f_in | yacg::Textbox::s_vertical);
			TEXTBOX(340, 400, 300, 100, yacg::Textbox::f_in | yacg::Textbox::s_horizontal | yacg::Textbox::s_vertical);
			TEXTBOX(660, 40, 300, 100, yacg::Textbox::f_out);
			TEXTBOX(660, 160, 300, 100, yacg::Textbox::f_out | yacg::Textbox::s_horizontal);
			TEXTBOX(660, 280, 300, 100, yacg::Textbox::f_out | yacg::Textbox::s_vertical);
			TEXTBOX(660, 400, 300, 100, yacg::Textbox::f_out | yacg::Textbox::s_horizontal | yacg::Textbox::s_vertical);
		}

	private:
		yacg::Textbox* tb1_;
		yacg::Textbox* tb2_;
	} _window;

private:
	static const char* _text;
};

#endif
