#ifndef	__PRIMITIVEWINDOW_HPP__
#define	__PRIMITIVEWINDOW_HPP__

class PrimitiveWindow
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
		reinterpret_cast<PrimitiveWindow*>(data)->_window.abort();
	}
		
private:
	class primitiveWindow : public yacg::_Window
	{
	public:
		primitiveWindow(BITMAP* b = screen)
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
			ExitButton_ = new yacg::Button(*this,  803, 727, 200, 20, "Exit"); insert(*ExitButton_);

			insert(*new yacg::Arc(*this, 40, 40, 45, 225, 18, yacg::Theme.RED));

			insert(*new yacg::Circle(*this, 80, 40, 18, yacg::Theme.BLUE, false));
			insert(*new yacg::Circle(*this, 120, 40, 18, yacg::Theme.BLUE, true));

			insert(*new yacg::Ellipse(*this, 160, 40, 18, 9, yacg::Theme.GREEN, false));
			insert(*new yacg::Ellipse(*this, 200, 40, 9, 18, yacg::Theme.GREEN, true));

			insert(*new yacg::Rect(*this, 222, 22, 258, 58, yacg::Theme.GRAY_LIGHT, false));
			insert(*new yacg::Rect(*this, 262, 22, 298, 58, yacg::Theme.GRAY_LIGHT, true));

			insert(*new yacg::Triangle(*this, 322, 20, 300, 59, 337, 59, yacg::Theme.GRAY_DARK, false));
			insert(*new yacg::Triangle(*this, 362, 20, 340, 59, 377, 59, yacg::Theme.GRAY_DARK, true));

			insert(*new yacg::Circle(*this, 390, 30, 5, yacg::Theme.BLUE, true));
			insert(*new yacg::Rect(*this, 380, 20, 419, 59, yacg::Theme.RED, false));
			insert(*new yacg::Triangle(*this, 409, 35, 404, 45, 414, 45, yacg::Theme.GREEN, true));
			insert(*new yacg::FloodFill(*this, 400, 40,
					yacg::Theme.style == yacg::Configuration::STYLE_3D ? yacg::Theme.BLACK : yacg::Theme.WHITE));

			insert(*new yacg::Line(*this, 440, 20, 479, 59, yacg::Theme.YELLOW));
			insert(*new yacg::Line(*this, 440, 20, 479, yacg::Theme.YELLOW, true));
			insert(*new yacg::Line(*this, 440, 20, 59, yacg::Theme.YELLOW, false));

			insert(*new yacg::Pixel(*this, 498, 40, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 499, 40, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 500, 40, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 501, 40, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 502, 40, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 500, 38, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 500, 39, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 500, 41, yacg::Theme.WHITE));
			insert(*new yacg::Pixel(*this, 500, 42, yacg::Theme.WHITE));

			const int p1[8][2] =
			{
				{ 540,20 },
				{ 545,35 },
				{ 560,40 },
				{ 545,45 },
				{ 540,60 },
				{ 535,45 },
				{ 520,40 },
				{ 535,35 }
			};
			insert(*new yacg::Polygon(*this, 8, p1, yacg::Theme.PURPLE, false));
			const int p2x[8] = { 580, 585, 600, 585, 580, 575, 560, 575 };
			const int p2y[8] = { 20, 35, 40, 45, 60, 45, 40, 35 };
			insert(*new yacg::Polygon(*this, 8, p2x, p2y, yacg::Theme.PURPLE, true));

			insert(*new yacg::Spline(*this, 620, 40, 630, 20, 650, 60, 660, 40, yacg::Theme.YELLOW_LIGHT));
		}

	private:
		yacg::Button* ExitButton_;
		yacg::Title* t1_;
		yacg::Title* t2_;
	} _window;
};

#endif
