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
// class TabBottomHoriz implementation
//=============================================================================

		TabBottomHoriz::TabBottomHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Calculate top of tabs
			_bottomTab = _bottom - (text_height(font) + 7);		// selected frame (2) + frame (1) + space (2) + title + space (2)
			
			// Left and right of user area are just the left and right of control
			_leftTab = _left;
			_rightTab = _right;
		}
		
		int TabBottomHoriz::bottom_controls() const
		{
			return _bottomTab - 1;
		}
		
		int TabBottomHoriz::height_controls() const
		{
			return (_bottomTab - _topTab);
		}
		
		bool TabBottomHoriz::isInTabs(int x, int y) const
		{
			return (x >= _left && x <= _right && y >= _bottomTab && y <= _bottom);
		}
		
		void TabBottomHoriz::paint_tab(BITMAP* bmp, int l, int t, int r, int b, bool sel,
				const char* s, bool down)
		{
			if (sel)
			{
				b += 2;
				
				// Overwrite original frame
				hline(bmp, l + 1, t - 1, r - 1, Theme.flat.Background);
				
				// Left
				vline(bmp, l, t, b - 3, Theme.flat.Frame);
				
				// Bottom/right
				vline(bmp, r, t, b - 3, Theme.flat.Frame);
				hline(bmp, l + 2, b, r - 2, Theme.flat.Frame);
				putpixel(bmp, l + 1, b - 1, Theme.flat.Frame);
				putpixel(bmp, r - 1, b - 1, Theme.flat.Frame);

				// Determine correct text color
				const int text_color = down ? color_plus_delta(Theme.flat.Foreground, 64)
						: Theme.flat.Foreground;

				// Title
				{
					Clipper c(bmp, l + 1, t + 1, r - 1, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 5, text_color, Theme.flat.Background);
				}
			}
			else
			{
				// Left
				vline(bmp, l, t, b - 3, Theme.flat.Frame);
				
				// Bottom/right
				vline(bmp, r, t, b - 3, Theme.flat.Frame);
				hline(bmp, l + 2, b, r - 2, Theme.flat.Frame);
				putpixel(bmp, l + 1, b - 1, Theme.flat.Frame);
				putpixel(bmp, r - 1, b - 1, Theme.flat.Frame);
				
				// Title
				{
					Clipper c(bmp, l + 1, t + 1, r - 1, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 3, Theme.flat.Foreground, Theme.flat.Background);
				}
			}
		}

		void TabBottomHoriz::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			rect(bmp, _left, _topTab, _right, _bottomTab - 1, Theme.flat.Frame);
			if (!tabsOnly)
				rectfill(bmp, _left + 1, _topTab + 1, _right - 1, _bottomTab - 2, Theme.flat.Background);
			
			// Paint tabs
			rectfill(bmp, _left, _bottomTab, _right, _bottom, Theme.flat.Background);
			const int count = int(_tabs.size());
			for (int i = 0; i < count; ++i)
			{
				const tab& t = _tabs[i];
				paint_tab(bmp, t.l, _bottomTab, t.r, _bottom - 2, i == _selected,
						_pages[i]->title(), down);
			}
		}

		void TabBottomHoriz::recalc()
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
						_tabs.push_back(tab(left + ii.first, _bottomTab, left + ii.second
								+ (i < endmo ? 1 : 0), _bottom));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to title width
				{
					// Calculate tabs for each page's title width
					_tabs.clear();
					int left;
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
						_tabs.push_back(tab(left, _bottomTab, left + length
								- (i < countmo ? 0 : 1), _bottom));
						left += length;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
		int TabBottomHoriz::top_controls() const
		{
			return _topTab;
		}

//=============================================================================
// class TabBottomHoriz3D implementation
//=============================================================================

		TabBottomHoriz3D::TabBottomHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Calculate top of tabs
			_bottomTab = _bottom - (text_height(font) + 7);		// selected frame (2) + frame (2) + space (2) + title + space (2)
			
			// Left and right of user area are just the left and right of control
			_leftTab = _left;
			_rightTab = _right;
		}
		
		int TabBottomHoriz3D::bottom_controls() const
		{
			return _bottomTab - 1;
		}
		
		int TabBottomHoriz3D::height_controls() const
		{
			return (_bottomTab - _topTab);
		}
		
		bool TabBottomHoriz3D::isInTabs(int x, int y) const
		{
			return (x >= _left && x <= _right && y >= _bottomTab && y <= _bottom);
		}
		
		void TabBottomHoriz3D::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s, bool first)
		{
			if (w == sel)
			{
				b += 2;
				l -= 2;
				r += 2;
				
				// Overwrite original frame
				hline(bmp, l + 2, t - 1, r - 2, Theme.GRAY);
				hline(bmp, l + 2, t - 2, r - 2, Theme.GRAY);
				
				// Left
				vline(bmp, l, t - (first ? 1 : 0), b - 1, Theme.GRAY);
				vline(bmp, l + 1, t - (first ? 2 : 0), b - 2, Theme.WHITE);
				
				// Bottom/right
				vline(bmp, r, t, b, Theme.GRAY_VERY_DARK);
				hline(bmp, l, b, r - 1, Theme.GRAY_VERY_DARK);
				vline(bmp, r - 1, t - 1, b - 1, Theme.GRAY_DARK);
				hline(bmp, l + 1, b - 1, r - 2, Theme.GRAY_DARK);

				// Title
				{
					Clipper c(bmp, l + 1, t + 1, r - 1, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 4, Theme.BLACK, -1);
				}
			}
			else
			{
				// Left
				if (w != sel_plus_1)
				{
					vline(bmp, l, t, b - 1, Theme.GRAY);
					vline(bmp, l + 1, t, b - 2, Theme.WHITE);
				}
				
				// Bottom/right
				if (w != sel_minus_1)
				{
					vline(bmp, r, t, b, Theme.GRAY_VERY_DARK);
					vline(bmp, r - 1, t, b - 1, Theme.GRAY_DARK);
				}
				else
					vline(bmp, r - 2, t, b - 2, Theme.GRAY_DARK);
				hline(bmp, l + (w != sel_plus_1 ? 0 : 2), b, r - 1, Theme.GRAY_VERY_DARK);
				hline(bmp, l + (w != sel_plus_1 ? 1 : 2), b - 1, r - 2, Theme.GRAY_DARK);
				
				// Title
				{
					Clipper c(bmp, l + 1, t + 1, r - 1, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 2, Theme.BLACK, -1);
				}
			}
		}
	
		void TabBottomHoriz3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			box(bmp, _left, _topTab, _right, _bottomTab - 1, false, Theme.GRAY);

			// Paint tabs
			rectfill(bmp, _left, _bottomTab, _right, _bottom, Theme.GRAY);
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
				paint_tab(bmp, t.l, _bottomTab, t.r, _bottom - 2, w, _pages[i]->title(), i == 0);
			}
		}
		
		void TabBottomHoriz3D::recalc()
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
						_tabs.push_back(tab(left + i->first, _bottomTab, left + i->second, _bottom));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to title width
				{
					// Calculate tabs for each page's title width
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
						_tabs.push_back(tab(left, _bottomTab, left + length - 1, _bottom));
						left += length;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
		int TabBottomHoriz3D::top_controls() const
		{
			return _topTab;
		}
		
//=============================================================================
// class TabBottomVert implementation
//=============================================================================

		TabBottomVert::TabBottomVert(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
		}
		
		bool TabBottomVert::isInTabs(int x, int y) const
		{
			return false;
		}
		
		void TabBottomVert::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			rect(bmp, _left, _top, _right, _bottom, Theme.flat.Frame);
			rectfill(bmp, _left + 1, _top + 2, _right - 1, _bottom - 1, Theme.flat.Background);
			Clipper (bmp, _left + 1, _top + 1, _right - 1, _bottom - 1);
			textout_centre_ex(bmp, font, 
					"h_vertical not supported with (o_horizontal | l_bottom) yet!",
					_left + (_width / 2), _top + ((_height - text_height(font))/ 2),
					Theme.flat.Foreground, Theme.flat.Background);
		}
		
		void TabBottomVert::recalc()
		{
		}
		
//=============================================================================
// class TabBottomVert3D implementation
//=============================================================================

		TabBottomVert3D::TabBottomVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
		}
		
		bool TabBottomVert3D::isInTabs(int x, int y) const
		{
			return false;
		}
		
		void TabBottomVert3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			box(bmp, _left, _top, _right, _bottom, false, Theme.GRAY);
			Clipper (bmp, _left + 2, _top + 2, _right - 2, _bottom - 2);
			textout_centre_ex(bmp, font, 
					"h_vertical not supported with (o_horizontal | l_bottom) yet!",
					_left + (_width / 2), _top + ((_height - text_height(font))/ 2),
					Theme.BLACK, Theme.GRAY);
		}
		
		void TabBottomVert3D::recalc()
		{
		}
	}
}
