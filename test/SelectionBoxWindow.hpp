#ifndef	__SELECTIONBOXWINDOW_HPP__
#define	__SELECTIONBOXWINDOW_HPP__

class SelectionBoxWindow
{
public:
	void process()
	{
		_window.sb().left();
		_window.ExitButton().callback(exit, this);
		_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<SelectionBoxWindow*>(data)->_window.abort();
	}
	static void action(yacg::SelectionBox& c, void* d)
	{
		std::ostrstream o;
		o << "Start: " << c.left() << "," << c.top() << "  End: "
				<< c.right() << "," << c.bottom() << std::ends;
		allegro_message(o.str());
		o.freeze(false);
	}
	
private:
	class selectionBoxWindow : public yacg::_Window
	{
	public:
		selectionBoxWindow(BITMAP* b = screen)
				:
				yacg::_Window(b)
		{
			init();
		}
		yacg::Button& ExitButton()
		{
			return *ExitButton_;
		}
		yacg::SelectionBox& dummy()
		{
			return *dummy_;
		}
		yacg::SelectionBox& sb()
		{
			return *sb_;
		}

	private:
		void init()
		{
			PLANE_EX(makecol(212, 208, 200), makecol(0, 0, 0))
			ExitButton_ = new yacg::Button(*this, 803, 727, 200, 20, "Exit"); insert(*ExitButton_);
			SELECTIONBOX_V_EX(sb, action, 0)
			SELECTIONBOX_V(dummy)
			SELECTIONBOX()
			SELECTIONBOX_EX(action, 0)
		}

	private:
		yacg::Button* ExitButton_;
		yacg::SelectionBox* dummy_;
		yacg::SelectionBox* sb_;
	} _window;
};

#endif
