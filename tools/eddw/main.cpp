#include "precompiled.hpp"

int main(int argc, const char* argv[])
{
	if (allegro_init() != 0)
		return 1;
	install_timer();
	install_keyboard(); 
	install_mouse();
	
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) != 0)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Error setting graphics mode\n%s\n", allegro_error);
		return 2;
	}

//	gui::gui_init();
//	show_mouse(screen);
//	MainWindow w;
//	w.process();

	allegro_exit();
	
	return 0;
}

END_OF_MAIN();
