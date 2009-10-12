#ifdef	_DEBUG
#define	DEBUGMODE 1
#endif
#define ALLEGRO_USE_CONSOLE 1
#include <allegro.h>

#include "fonts.h"

#include <boost/bind.hpp>
using boost::bind;

#include <yacg.hpp>
//#include "yacg/Button.hpp>
#include <yacg/Box.hpp>
//#include <yacg/Checkbox.hpp>
//#include <yacg/CheckedBox.hpp>
#include <yacg/Plane.hpp>
//#include <yacg/List.hpp>
//#include <yacg/Menu.hpp>
//#include <yacg/Primitive.hpp>
//#include <yacg/Radio.hpp>
//#include <yacg/Scroll.hpp>
//#include <yacg/SelectionBox.hpp>
//#include <yacg/Tab.hpp>
//#include <yacg/Textbox.hpp>
//#include <yacg/Title.hpp>
#include <yacg/Window.hpp>

#include "BoxWindow.hpp"
//#include "CheckboxWindow.hpp"
//#include "CheckedBoxWindow.hpp"
//#include "DialogWindow.hpp"
//#include "ListWindow.hpp"
//#include "MenuWindow.hpp"
//#include "PrimitiveWindow.hpp"
//#include "RadioWindow.hpp"
//#include "ScrollWindow.hpp"
//#include "SelectionBoxWindow.hpp"
//#include "TabWindow.hpp"
//#include "TextboxWindow.hpp"
//#include "TitleWindow.hpp"

//#include "MainWindow.hpp"

int calledback = 0;

void callback5(yacg::Theme* t)
{
	++calledback;
}

void callback6(yacg::Theme* t, int a, int b)
{
	++calledback;
}

namespace
{
	void callback7(yacg::Theme* t)
	{
		++calledback;
	}

	void callback8(yacg::Theme* t, int a, int b)
	{
		++calledback;
	}
}

int main(int argc, const char* argv[])
{
	if (allegro_init() != 0)
		return 1;
	install_timer();
	install_keyboard(); 
	install_mouse();
	
	set_color_depth(32);
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) != 0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error setting graphics mode\n%s\n", allegro_error);
		return 2;
	}

//	DATAFILE* fonts = load_datafile("fonts.dat");
	
	yacg::yacg_init();
	show_mouse(screen);
//	MainWindow w;

	// Check theme callbacks
//	yacg::ActiveTheme->style(yacg::Theme::STYLE_FLAT);
	BoxWindow w;
//	w.process(reinterpret_cast<FONT*>(fonts[FIXEDSYS].dat));
	w.process();

//	unload_datafile(fonts);
	
	allegro_exit();
	
	return 0;
}

END_OF_MAIN()

#if 0
const char* CheckedBoxWindow::_text =
{
	"Line  0 Line  0 Line  0 Line  0 Line  0 Line  0\n"
	"Line  1 Line  1 Line  1 Line  1 Line  1 Line  1\n"
	"Line  2 Line  2 Line  2 Line  2 Line  2 Line  2\n"
	"Line  3 Line  3 Line  3 Line  3 Line  3 Line  3\n"
	"Line  4 Line  4 Line  4 Line  4 Line  4 Line  4\n"
	"Line  5 Line  5 Line  5 Line  5 Line  5 Line  5\n"
	"Line  6 Line  6 Line  6 Line  6 Line  6 Line  6\n"
	"Line  7 Line  7 Line  7 Line  7 Line  7 Line  7\n"
	"Line  8 Line  8 Line  8 Line  8 Line  8 Line  8\n"
	"Line  9 Line  9 Line  9 Line  9 Line  9 Line  9\n"
	"Line 10 Line 10 Line 10 Line 10 Line 10 Line 10\n"
	"Line 11 Line 11 Line 11 Line 11 Line 11 Line 11\n"
	"Line 12 Line 12 Line 12 Line 12 Line 12 Line 12\n"
	"Line 13 Line 13 Line 13 Line 13 Line 13 Line 13\n"
	"Line 14 Line 14 Line 14 Line 14 Line 14 Line 14\n"
	"Line 15 Line 15 Line 15 Line 15 Line 15 Line 15\n"
};
const char* TextboxWindow::_text =
{
	"Line  0 Line  0 Line  0 Line  0 Line  0 Line  0\n"
	"Line  1 Line  1 Line  1 Line  1 Line  1 Line  1\n"
	"Line  2 Line  2 Line  2 Line  2 Line  2 Line  2\n"
	"Line  3 Line  3 Line  3 Line  3 Line  3 Line  3\n"
	"Line  4 Line  4 Line  4 Line  4 Line  4 Line  4\n"
	"Line  5 Line  5 Line  5 Line  5 Line  5 Line  5\n"
	"Line  6 Line  6 Line  6 Line  6 Line  6 Line  6\n"
	"Line  7 Line  7 Line  7 Line  7 Line  7 Line  7\n"
	"Line  8 Line  8 Line  8 Line  8 Line  8 Line  8\n"
	"Line  9 Line  9 Line  9 Line  9 Line  9 Line  9\n"
	"Line 10 Line 10 Line 10 Line 10 Line 10 Line 10\n"
	"Line 11 Line 11 Line 11 Line 11 Line 11 Line 11\n"
	"Line 12 Line 12 Line 12 Line 12 Line 12 Line 12\n"
	"Line 13 Line 13 Line 13 Line 13 Line 13 Line 13\n"
	"Line 14 Line 14 Line 14 Line 14 Line 14 Line 14\n"
	"Line 15 Line 15 Line 15 Line 15 Line 15 Line 15\n"
};
#endif
