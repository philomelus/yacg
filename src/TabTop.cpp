#include "precompiled.hpp"
#include "yacg/Tab.hpp"
#include "TabImpl.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

namespace yacg
{
	namespace TabImpl
	{
//=============================================================================
// class TabTopHoriz implementation
//=============================================================================

		TabTopHoriz::TabTopHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Determine bottom of tabs
			_bottomTab = _topTab + text_height(font) + 7;		// selected frame (2) + frame (1) + space (2) + title + space (2)
			
			// Left and right are just the left and right of the control
			_leftTab = _left;
			_rightTab = _right;
		}
		
		int TabTopHoriz::bottom_controls() const
		{
			return _bottom;
		}

		
		int TabTopHoriz::height_controls() const
		{
			return (_bottom - (_bottomTab + 1)) + 1;
		}
		
		bool TabTopHoriz::isInTabs(int x, int y) const
		{
			return (x >= _left && x <= _right && y >= _topTab && y <= _bottomTab);
		}
		
		void TabTopHoriz::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, bool down)
		{
			if (w == sel)
			{
				t -= 2;
				
				// Clear frame line for selected
				hline(bmp, l + 1, b + 1, r - 1, Theme.flat.Background);
				
				// Top/left
				hline(bmp, l + 2, t, r - 2, Theme.flat.Frame);
				vline(bmp, l, t + 2, b, Theme.flat.Frame);
				putpixel(bmp, l + 1, t + 1, Theme.flat.Frame);
				
				// Right
				vline(bmp, r, t + 2, b, Theme.flat.Frame);
				putpixel(bmp, r - 1, t + 1, Theme.flat.Frame);

				// Determine correct color
				const int text_color = down ? color_plus_delta(Theme.flat.Foreground, 64)
						: Theme.flat.Foreground;

				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 4, text_color, Theme.flat.Background);
				}
			}
			else
			{
				// Top/left
				hline(bmp, l + 2, t, r - 2, Theme.flat.Frame);
				vline(bmp, l, t + 2, b, Theme.flat.Frame);
				putpixel(bmp, l + 1, t + 1, Theme.flat.Frame);
				
				// Right
				vline(bmp, r, t + 2, b, Theme.flat.Frame);
				putpixel(bmp, r - 1, t + 1, Theme.flat.Frame);

				// Title
				{
					Clipper c(bmp, l + 1, t + 1, r - 1, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 4, Theme.flat.Foreground, Theme.flat.Background);
				}
			}
		}
	
		void TabTopHoriz::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			hline(bmp, _left, _bottomTab + 1, _right, Theme.flat.Frame);
			if (!tabsOnly)
			{
				vline(bmp, _left, _bottomTab + 2, _bottom, Theme.flat.Frame);
				hline(bmp, _left + 1, _bottom, _right - 1, Theme.flat.Frame);
				vline(bmp, _right, _bottomTab + 2, _bottom, Theme.flat.Frame);
			}

			// Clear background
			if (!tabsOnly)
			{
				rectfill(bmp, _left + 2, _bottomTab + 2, _right - 2, _bottom - 2,
						Theme.flat.Background);
			}
			
			// Paint tabs
			rectfill(bmp, _left, _topTab, _right, _bottomTab, Theme.flat.Background);
			const int count = int(_tabs.size());
			for (int i = 0; i < count; ++i)
			{
				tab& t = _tabs[i];
				loc w = normal;
				if (i == _selected - 1)
					w = sel_minus_1;
				else if (i == _selected)
					w = sel;
				else if (i == _selected + 1)
					w = sel_plus_1;
				paint_tab(bmp, t.l, _topTab + 2, t.r, _bottomTab, w, _pages[i]->title(),
						down);
			}
		}
		
		void TabTopHoriz::recalc()
		{
			switch (_owner.flags() & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across width
				{
					// Get list of tab positions
					distribution t;
					distribute(_width, int(_pages.size()), t);
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					const int left = _left;
					const int end = int(t.size());
					const int endmo = end - 1;
					for (int i = 0; i < end; ++i)
					{
						std::pair<int, int>& ii = t[i];
						_tabs.push_back(tab(left + ii.first, _topTab, left + ii.second
								+ ((i < endmo) ? 1 : 0), _bottomTab));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to title width
				{
					// Calculate tabs for each page's title width
					_tabs.clear();
					int left = _left;
					const int count = int(_pages.size());
					if ((_owner.flags() & Tab::a_mask) == Tab::a_left)
						left = _left;
					else
					{
						int total = 0;
						for (int i = 0; i < count; ++i)
							total += (text_length(font, _pages[i]->title()) + 8);		// frame (1) + space (3) + title + space (3) + frame (1)
						left = _right - total + 1;
					}
					const int countmo = count - 1;
					int length;
					for (int i = 0; i < count; ++i)
					{
						length = text_length(font, _pages[i]->title()) + 8;				// frame (1) + space (3) + title + space (3) + frame (1)
						_tabs.push_back(tab(left, _topTab, left + length - (i < countmo
								? 0 : 1), _bottomTab));
						left += length;
					}
				}
				break;

			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
		int TabTopHoriz::top_controls() const
		{
			return (_bottomTab + 1);
		}
		
//=============================================================================
// class TabTopHoriz3D implementation
//=============================================================================

		TabTopHoriz3D::TabTopHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Determine bottom of tabs
			_bottomTab = _topTab + text_height(font) + 7;		// selected frame (2) + frame (2) + space (2) + title + space (2)
			
			// Left and right are just the left and right of the control
			_leftTab = _left;
			_rightTab = _right;
		}
		
		int TabTopHoriz3D::bottom_controls() const
		{
			return _bottom;
		}

		
		int TabTopHoriz3D::height_controls() const
		{
			return (_bottom - (_bottomTab + 1)) + 1;
		}
		
		bool TabTopHoriz3D::isInTabs(int x, int y) const
		{
			return (x >= _left && x <= _right && y >= _topTab && y <= _bottomTab);
		}
		
		void TabTopHoriz3D::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s)
		{
			if (w == sel)
			{
				t -= 2;
				l -= 2;
				r += 2;
				
				// Overwrite original frame
				hline(bmp, l + 1, b + 1, r, Theme.GRAY);

				// Top/left
				hline(bmp, l + 2, t, r - 2, Theme.WHITE);
				vline(bmp, l, t + 2, b, Theme.WHITE);
				putpixel(bmp, l + 1, t + 1, Theme.WHITE);
				
				// right
				vline(bmp, r, t + 2, b + 1, Theme.GRAY_VERY_DARK);
				putpixel(bmp, r - 1, t + 1, Theme.GRAY_VERY_DARK);
				
				// Inset right
				vline(bmp, r - 1, t + 2, b + 1, Theme.GRAY_DARK);

				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, (l - 2) + ((r - l) / 2),
							t + 5, Theme.BLACK, -1);
				}
			}
			else
			{
				// Top/left
				hline(bmp, l + (w == sel_plus_1 ? 3 : 2), t, r - 2, Theme.WHITE);
				if (w != sel_plus_1)
				{
					vline(bmp, l, t + 2, b, Theme.WHITE);
					putpixel(bmp, l + 1, t + 1, Theme.WHITE);
				}
				
				if (w != sel_minus_1)
				{
					// right
					vline(bmp, r, t + 2, b, Theme.GRAY_VERY_DARK);
					putpixel(bmp, r - 1, t + 1, Theme.GRAY_VERY_DARK);
					
					// Inset right
					vline(bmp, r - 1, t + 2, b, Theme.GRAY_DARK);
				}

				// Title
				{
					Clipper c(bmp, l + 1, t + 1, r - 1, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 4, Theme.BLACK, -1);
				}
			}
		}
		
		void TabTopHoriz3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			hline(bmp, _left, _bottomTab + 1, _right - 1, Theme.WHITE);
			if (!tabsOnly)
			{
				vline(bmp, _left, _bottomTab + 2, _bottom - 1, Theme.WHITE);
				hline(bmp, _left + 1, _bottom - 1, _right - 1, Theme.GRAY_DARK);
				vline(bmp, _right - 1, _bottomTab + 2, _bottom - 2, Theme.GRAY_DARK);
				hline(bmp, _left, _bottom, _right, Theme.GRAY_VERY_DARK);
				vline(bmp, _right, _bottomTab + 1, _bottom - 1, Theme.GRAY_VERY_DARK);
			}

			// Clear background
			if (!tabsOnly)
				rectfill(bmp, _left + 2, _bottomTab + 2, _right - 2, _bottom - 2, Theme.GRAY);
			
			// Paint tabs
			rectfill(bmp, _left, _topTab, _right, _bottomTab, Theme.GRAY);
			const int count = int(_tabs.size());
			for (int i = 0; i < count; ++i)
			{
				tab& t = _tabs[i];
				loc w = normal;
				if (i == _selected - 1)
					w = sel_minus_1;
				else if (i == _selected)
					w = sel;
				else if (i == _selected + 1)
					w = sel_plus_1;
				paint_tab(bmp, t.l, _topTab + 2, t.r, _bottomTab, w, _pages[i]->title());
			}
		}
		
		void TabTopHoriz3D::recalc()
		{
			switch (_owner.flags() & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across width
				{
					// Get list of tab positions
					distribution t;
					distribute(_width - 4, int(_pages.size()), t);
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					const int left = _left + 2;
					const distribution::const_iterator end = t.end();
					for (distribution::const_iterator i = t.begin(); i != end; ++i)
					{
						_tabs.push_back(tab(left + i->first, _topTab, left + i->second, _bottomTab));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to title width
				{
					// Calculate tabs based on each page's title width
					_tabs.clear();
					const int count = int(_pages.size());
					int left;
					if ((_owner.flags() & Tab::a_mask) == Tab::a_left)
						left = _left + 2;
					else
					{
						int total = 0;
						for (int i = 0; i < count; ++i)
							total += (text_length(font, _pages[i]->title()) + 10);			// frame (2) + space (3) + title + space (3) + frame (2)
						left = _right - (total + 1);
					}
					int length;
					for (int i = 0; i < count; ++i)
					{
						length = text_length(font, _pages[i]->title()) + 10;				// frame (2) + space (3) + title + space (3) + frame (2)
						_tabs.push_back(tab(left, _topTab, left + length - 1, _bottomTab));
						left += length;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
		int TabTopHoriz3D::top_controls() const
		{
			return (_bottomTab + 1);
		}

//=============================================================================
// class TabTopVert implementation
//=============================================================================

		TabTopVert::TabTopVert(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
		}
		
		bool TabTopVert::isInTabs(int x, int y) const
		{
			return false;
		}
		
		void TabTopVert::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			rect(bmp, _left, _top, _right, _bottom, Theme.flat.Frame);
			rectfill(bmp, _left + 1, _top + 2, _right - 1, _bottom - 1, Theme.flat.Background);
			Clipper (bmp, _left + 1, _top + 1, _right - 1, _bottom - 1);
			textout_centre_ex(bmp, font, 
					"h_vertical not supported with (o_horizontal | l_top) yet!",
					_left + (_width / 2), _top + ((_height - text_height(font))/ 2),
					Theme.flat.Foreground, Theme.flat.Background);
		}
		
		void TabTopVert::recalc()
		{
		}
		
//=============================================================================
// class TabTopVert3D implementation
//=============================================================================

		TabTopVert3D::TabTopVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
		}
		
		bool TabTopVert3D::isInTabs(int x, int y) const
		{
			return false;
		}
		
		void TabTopVert3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			box(bmp, _left, _top, _right, _bottom, false, Theme.GRAY);
			Clipper (bmp, _left + 2, _top + 2, _right - 2, _bottom - 2);
			textout_centre_ex(bmp, font, 
					"h_vertical not supported with (o_horizontal | l_top) yet!",
					_left + (_width / 2), _top + ((_height - text_height(font))/ 2),
					Theme.BLACK, Theme.GRAY);
		}
		
		void TabTopVert3D::recalc()
		{
		}
	}
}
