#ifndef	__LISTWINDOW_HPP__
#define	__LISTWINDOW_HPP__

class ListWindow
{
public:
	void process()
	{
		_window.ExitButton().callback(exit, this);
		_window.list2().left();
		const int size = _window.size();
		for (int i = 2; i < size; ++i)
		{
			yacg::List& l = static_cast<yacg::List&>(_window[i]);
			for (int n = 0; n < 30; ++n)
			{
				std::ostrstream o;
				o << "This is a test of a really long item number " << n << " to see if we handle it correctly!" << std::ends;
				l.insert(o.str());
				o.freeze(false);
			}
		}
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<ListWindow*>(data)->_window.abort();
	}

private:
	class listWindow : public yacg::_Window
	{
	public:
		listWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::Button& ExitButton()
		{
			return *ExitButton_;
		}
		yacg::List& list2()
		{
			return *list2_;
		}

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			ExitButton_ = new yacg::Button(*this,  803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			LIST_V(list1, 20, 20, 200, 200, yacg::List::t_notitle | yacg::List::v_noscroll | yacg::List::h_noscroll)
			list1_->title("Nothing");
			LIST_V_EX(list2, 240, 20, 200, 200, yacg::List::t_title | yacg::List::v_noscroll | yacg::List::h_noscroll, "Title")
			LIST_EX(20, 240, 200, 200, yacg::List::t_notitle | yacg::List::v_scroll | yacg::List::h_noscroll, "Vert")
			LIST_EX(240, 240, 200, 200, yacg::List::t_title | yacg::List::v_scroll  | yacg::List::h_noscroll, "Vert + Title")
			LIST_EX(20, 460, 200, 200, yacg::List::t_notitle | yacg::List::v_noscroll | yacg::List::h_scroll, "Horizontal")
			LIST_EX(240, 460, 200, 200, yacg::List::t_title | yacg::List::v_noscroll | yacg::List::h_scroll, "Horiz + Title")
			LIST_EX(460, 460, 200, 200, yacg::List::t_notitle | yacg::List::v_scroll | yacg::List::h_scroll, "Horiz + Vert")
			LIST_EX(680, 460, 200, 200, yacg::List::t_title | yacg::List::v_scroll  | yacg::List::h_scroll, "Horiz + Vert + Title")
		}

	private:
		yacg::List* list2_;
		yacg::Button* ExitButton_;
		yacg::List* list1_;
	} _window;
	friend listWindow;
};

#endif
