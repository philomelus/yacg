#ifndef	__BOXWINDOW_HPP__
#define	__BOXWINDOW_HPP__

#ifndef	__UTILITY_HPP__
//#include "../src/utility.hpp"
#endif

class BoxWindow
{
public:
	void theme(yacg::Theme& t)
	{
		_window.theme(t);
	}
	yacg::Box& box()
	{
		return _window.b28();
	}
	void process()
	{
//		_window.ExitButton().callback(exit, this);
		_window.update();
		rectfill(screen, 390, 30, 469, 109, makecol(255, 255, 0));
		rectfill(screen, 510, 30, 589, 109, makecol(255, 255, 0));
		rectfill(screen, 390, 150, 469, 229, makecol(255, 255, 0));
		rectfill(screen, 510, 150, 589, 229, makecol(255, 255, 0));
		rectfill(screen, 390, 270, 469, 349, makecol(255, 255, 0));
		rectfill(screen, 510, 270, 589, 349, makecol(255, 255, 0));
		{
			show_mouse(0);
			acquire_bitmap(_window.bitmap());
			_window.b28().dirty(yacg::Box::dirty_all);
			_window.b28().paint(_window.bitmap());
			_window.b29().dirty(yacg::Box::dirty_all);
			_window.b29().paint(_window.bitmap());
			_window.b30().dirty(yacg::Box::dirty_all);
			_window.b30().paint(_window.bitmap());
			_window.b31().dirty(yacg::Box::dirty_all);
			_window.b31().paint(_window.bitmap());
			_window.b32().dirty(yacg::Box::dirty_all);
			_window.b32().paint(_window.bitmap());
			_window.b33().dirty(yacg::Box::dirty_all);
			_window.b33().paint(_window.bitmap());
			release_bitmap(_window.bitmap());
			show_mouse(_window.bitmap());
		}
//		_window.process_events();
std::pair<bool, bool> result = std::make_pair(true, false);
while (!result.second)
{
	if (result.first)
		_window.update();
	else
	{
		if (keypressed())
		{
			if (key[KEY_ESC])
				break;
			const boxWindow::iterator end = _window.end();
			for (boxWindow::iterator i = _window.begin(); i != end; ++i)
			{
				if (i->active())
					i->active(false);
				else
					i->active(true);
			}
			clear_keybuf();
		}
		rest(1);
	}
	result = _window.process_events_and_return();
}
_window.dump("");
	}

private:
#if 0
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<BoxWindow*>(data)->_window.abort();
	}
#endif

private:
	class boxWindow : public yacg::_Window
	{
	public:
		boxWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
#if 0
		yacg::Button& ExitButton()
		{
			return *ExitButton_;
		}
#endif
		yacg::Box& b28()
		{
			return *b28_;
		}
		yacg::Box& b29()
		{
			return *b29_;
		}
		yacg::Box& b30()
		{
			return *b30_;
		}
		yacg::Box& b31()
		{
			return *b31_;
		}
		yacg::Box& b32()
		{
			return *b32_;
		}
		yacg::Box& b33()
		{
			return *b33_;
		}
		
	private:
		void init()
		{
			insert(*new yacg::Plane(*this, yacg::Plane::undefined, yacg::Box::auto_delete));
//			ExitButton_ = new yacg::Button(*this,  803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			insert(*new yacg::Box(*this,  20, 20, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_left | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 1"));
			insert(*new yacg::Box(*this,  140, 20, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_right | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 2"));
			insert(*new yacg::Box(*this,  260, 20, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_centered | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 3"));
			insert(*new yacg::Box(*this,  20, 90, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_left | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 4"));
			insert(*new yacg::Box(*this, 140, 90, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_right | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 5"));
			insert(*new yacg::Box(*this, 260, 90, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_centered | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 6"));
			insert(*new yacg::Box(*this, 20, 160, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_left | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 7"));
			insert(*new yacg::Box(*this, 140, 160, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_right | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 8"));
			insert(*new yacg::Box(*this, 260, 160, 100, 50, yacg::Box::t_title | yacg::Box::v_top | yacg::Box::h_centered | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 9"));
			insert(*new yacg::Box(*this, 20, 275, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_left | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 10"));
			insert(*new yacg::Box(*this, 140, 275, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_right | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 11"));
			insert(*new yacg::Box(*this, 260, 275, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_centered | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 12"));
			insert(*new yacg::Box(*this, 20, 345, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_left | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 13"));
			insert(*new yacg::Box(*this, 140, 345, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_right | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 14"));
			insert(*new yacg::Box(*this, 260, 345, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_centered | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 15"));
			insert(*new yacg::Box(*this, 20, 415, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_left | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 16"));
			insert(*new yacg::Box(*this, 140, 415, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_right | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 17"));
			insert(*new yacg::Box(*this, 260, 415, 100, 50, yacg::Box::t_title | yacg::Box::v_bottom | yacg::Box::h_centered | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 18"));
			insert(*new yacg::Box(*this, 20, 530, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_left | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 19"));
			insert(*new yacg::Box(*this, 140, 530, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_right | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 20"));
			insert(*new yacg::Box(*this, 260, 530, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_centered | yacg::Box::o_titlecentered | yacg::Box::auto_delete, "Box 21"));
			insert(*new yacg::Box(*this, 20, 600, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_left | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 22"));
			insert(*new yacg::Box(*this, 140, 600, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_right | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 23"));
			insert(*new yacg::Box(*this, 260, 600, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_centered | yacg::Box::o_titleabove | yacg::Box::auto_delete, "Box 24"));
			insert(*new yacg::Box(*this, 20, 670, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_left | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 25"));
			insert(*new yacg::Box(*this, 140, 670, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_right | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 26"));
			insert(*new yacg::Box(*this, 260, 670, 100, 50, yacg::Box::t_title | yacg::Box::v_centered | yacg::Box::h_centered | yacg::Box::o_titlebelow | yacg::Box::auto_delete, "Box 27"));
			b28_ = new yacg::Box(*this,  380, 20, 100, 100, yacg::Box::f_flat | yacg::Box::t_notitle | yacg::Box::i_unfilled | yacg::Box::auto_delete, "Box 28"); insert(*b28_);
			b29_ = new yacg::Box(*this,  500, 20, 100, 100, yacg::Box::f_flat | yacg::Box::t_notitle | yacg::Box::i_filled | yacg::Box::auto_delete, "Box 29"); insert(*b29_);
			b30_ = new yacg::Box(*this,  380, 140, 100, 100, yacg::Box::f_in | yacg::Box::t_notitle | yacg::Box::i_unfilled | yacg::Box::auto_delete, "Box 30"); insert(*b30_);
			b31_ = new yacg::Box(*this,  500, 140, 100, 100, yacg::Box::f_in | yacg::Box::t_notitle | yacg::Box::i_filled | yacg::Box::auto_delete, "Box 31"); insert(*b31_);
			b32_ = new yacg::Box(*this,  380, 260, 100, 100, yacg::Box::f_out | yacg::Box::t_notitle | yacg::Box::i_unfilled | yacg::Box::auto_delete, "Box 32"); insert(*b32_);
			b33_ = new yacg::Box(*this,  500, 260, 100, 100, yacg::Box::f_out | yacg::Box::t_notitle | yacg::Box::i_filled | yacg::Box::auto_delete, "Box 33"); insert(*b33_);
		}

	private:
//		yacg::Button* ExitButton_;
		yacg::Box* b28_;
		yacg::Box* b29_;
		yacg::Box* b30_;
		yacg::Box* b31_;
		yacg::Box* b32_;
		yacg::Box* b33_;
	} _window;
};

#endif
