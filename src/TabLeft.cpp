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
// class TabLeftHoriz implementation
//=============================================================================

		TabLeftHoriz::TabLeftHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Right and bottom are just the right and bottom of the control
			_rightTab = _right;
			_bottomTab = _bottom;
			
			// _leftTab has to be calculated in recalc because it needs the
			// pages to determine.
			_leftTab = _left + 9;					// selected frame(2) + frame(1) + space(3) + title + space(3)
		}
		
		bool TabLeftHoriz::isInTabs(int x, int y) const
		{
			return (x >= _left && x < _leftTab && y >= _topTab && y <= _bottom);
		}
		
		void TabLeftHoriz::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s, bool down)
		{
			if (w == sel)
			{
				l -= 2;
				
				// Overwrite original frame
				vline(bmp, r + 1, t + 1, b - 1, Theme.flat.Background);
				
				// Top
				hline(bmp, l, t, r, Theme.flat.Frame);
				
				// Left
				vline(bmp, l, t + 1, b - 1, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l, b, r, Theme.flat.Frame);
				
				// Determine correct color
				const int text_color = down ? color_plus_delta(Theme.flat.Foreground, 64)
						: Theme.flat.Foreground;

				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 3, text_color, Theme.flat.Background);
				}
			}
			else
			{
				// Top
				hline(bmp, l, t, r, Theme.flat.Frame);
				
				// Left
				vline(bmp, l, t + 1, b - 1, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l, b, r, Theme.flat.Frame);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 3, Theme.flat.Foreground, Theme.flat.Background);
				}
			}
		}

		void TabLeftHoriz::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			if (!tabsOnly)
			{
				rect(bmp, _leftTab, _topTab, _rightTab, _bottomTab, Theme.flat.Frame);
				rectfill(bmp, _leftTab + 1, _topTab + 1, _rightTab - 1, _bottomTab - 1,
						Theme.flat.Background);
			}

			// Paint tabs
			rectfill(bmp, _left, _topTab, _leftTab - 1, _bottom, Theme.flat.Background);
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
				paint_tab(bmp, t.l, t.t, t.r, t.b, w, _pages[i]->title(), down);
			}
		}
		
		void TabLeftHoriz::recalc()
		{
			const int flags = _owner.flags();
			switch (flags & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across height
				{
					// Determine the longest title
					const int count = int(_pages.size());
					int longest = 0;
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title());
						if (length > longest)
							longest = length;
					}

					// Update left side of user control area
					longest += 9;								// selected frame(2) + frame(1) + space(3) + title + space(3)
					_leftTab = _left + longest;
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab;
					const int height = text_height(font) + 6;	// frame(1) + space(2) + font + space(2) + frame(1)
					for (int i = 0; i < count; ++i)
					{
						_tabs.push_back(tab(_left + 2, top, _leftTab - 1, top + height));
						top += height;
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to title width
				{
					// Determine the longest title
					const int count = int(_pages.size());
					int longest = 0;
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title());
						if (length > longest)
							longest = length;
					}

					// Update left side of user control area
					longest += 9;								// selected frame(2) + frame(1) + space(3) + title + space(3)
					_leftTab = _left + longest;

					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab;
					const int height = text_height(font) + 6;	// frame(1) + space(2) + font + space(2) + frame(1)
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title()) + 7;	// frame(1) + space(3) + title + space(3)
						_tabs.push_back(tab(_leftTab - length, top, _leftTab - 1, top + height));
						top += height;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
//=============================================================================
// class TabLeftHoriz3D implementation
//=============================================================================

		TabLeftHoriz3D::TabLeftHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Right and bottom are just the right and bottom of the control
			_rightTab = _right;
			_bottomTab = _bottom;
			
			// _leftTab has to be calculated in recalc because it needs the
			// pages to determine.
			_leftTab = _left + 10;					// selected frame(2) + frame(2) + space(3) + title + space(3)
		}
		
		bool TabLeftHoriz3D::isInTabs(int x, int y) const
		{
			return (x >= _left && x < _leftTab && y >= _topTab && y <= _bottom);
		}
		
		void TabLeftHoriz3D::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s, int pl)
		{
			if (w == sel)
			{
				t -= 2;
				b += 2;
				l -= 2;
				
				// Overwrite original frame
				vline(bmp, r + 1, t + 2, b - 2, Theme.GRAY);
				vline(bmp, r + 2, t + 2, b - 2, Theme.GRAY);
				
				// Top
				hline(bmp, l, t, r + 1, Theme.GRAY);
				hline(bmp, l, t + 1, r + 1, Theme.WHITE);
				
				// Left
				vline(bmp, l, t + 1, b - 1, Theme.GRAY);
				vline(bmp, l + 1, t + 2, b - 2, Theme.WHITE);
				
				// Bottom
				hline(bmp, l, b, r, Theme.GRAY_VERY_DARK);
				hline(bmp, l + 1, b - 1, r + 1, Theme.GRAY_DARK);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 6, Theme.BLACK, -1);
				}
			}
			else
			{
				// Top
				if (w != sel_plus_1)
				{
					hline(bmp, l, t, r, Theme.GRAY);
					hline(bmp, l, t + 1, r, Theme.WHITE);
				}
				else if (pl > l)
				{
					hline(bmp, l, t, pl - 3, Theme.GRAY);
					hline(bmp, l, t + 1, pl - 3, Theme.WHITE);
				}
				
				// Left
				vline(bmp, l, t + (w == sel_plus_1 ? 2 : 1), b - 1, Theme.GRAY);
				vline(bmp, l + 1, t + 2, b - 2, Theme.WHITE);
				
				// Bottom
				hline(bmp, l, b, r, Theme.GRAY_VERY_DARK);
				hline(bmp, l + 1, b - 1, r, Theme.GRAY_DARK);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 4, Theme.BLACK, -1);
				}
			}
		}

		void TabLeftHoriz3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			box(bmp, _leftTab, _topTab, _rightTab, _bottomTab, false, (tabsOnly ? - 1
					: Theme.GRAY));

			// Paint tabs
			rectfill(bmp, _left, _topTab, _leftTab - 1, _bottom, Theme.GRAY);
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
				paint_tab(bmp, t.l, t.t, t.r, t.b, w, _pages[i]->title(),
						i > 0 ? _tabs[i - 1].l : 0);
			}
		}
		
		void TabLeftHoriz3D::recalc()
		{
			const int flags = _owner.flags();
			switch (flags & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across height
				{
					// Determine the longest title
					const int count = int(_pages.size());
					int longest = 0;
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title());
						if (length > longest)
							longest = length;
					}

					// Update left side of user control area
					longest += 10;								// selected frame(2) + frame(2) + space(3) + title + space(3)
					_leftTab = _left + longest;
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab + 2;
					const int height = text_height(font) + 8;	// frame(2) + space(2) + font + space(2) + frame(2)
					for (int i = 0; i < count; ++i)
					{
						_tabs.push_back(tab(_left + 2, top, _leftTab - 1, top + height - 1));
						top += height;
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to title width
				{
					// Determine the longest title
					const int count = int(_pages.size());
					int longest = 0;
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title());
						if (length > longest)
							longest = length;
					}

					// Update left side of user control area
					longest += 10;								// selected frame(2) + frame(2) + space(3) + title + space(3)
					_leftTab = _left + longest;

					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab + 2;
					const int height = text_height(font) + 8;	// frame(2) + space(2) + font + space(2) + frame(2)
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title()) + 8;	// space(3) + title + space(3) + frame(2)
						_tabs.push_back(tab(_leftTab - length, top, _leftTab - 1, top + height - 1));
						top += height;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
//=============================================================================
// class TabLeftVert implementation
//=============================================================================

		TabLeftVert::TabLeftVert(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Right and bottom are just the right and bottom of the control
			_rightTab = _right;
			_bottomTab = _bottom;
			
			// Calculate the right side of the tabs
			_charWidth = (text_length(font, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRr"
					"SsTtUuVvWwXxYyZz1234567890") / 62);
			_leftTab = _left + (_charWidth + 9);			// selected frame(2) + frame(1) + space(3) + character(~8) + space(3)
		}
		
		bool TabLeftVert::isInTabs(int x, int y) const
		{
			return (x >= _left && x < _leftTab && y >= _topTab && y <= _bottom);
		}
		
		void TabLeftVert::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s)
		{
			if (w == sel)
			{
				l -= 2;
				
				// Overwrite original frame
				vline(bmp, r + 1, t + 1, b - 1, Theme.flat.Background);
				
				// Top
				hline(bmp, l, t, r, Theme.flat.Frame);
				
				// Left
				vline(bmp, l, t + 1, b, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l + 1, b, r, Theme.flat.Frame);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					const int left = l + ((r - l) / 2) + 2;
					const int count = int(strlen(s));
					const int height = text_height(font) + 2;
					int top = t + 5;
					const int total_height_string = height * count;
					const int total_height_tab = (b - t) - 9;		// height - frame(2) - space(3) space(3) - frame(2)
					if (total_height_string < total_height_tab)
						top += ((total_height_tab - total_height_string) / 2);
					char str[2];
					str[1] = 0;
					for (int i = 0; i < count; ++i)
					{
						str[0] = s[i];
						textout_centre_ex(bmp, font, str, left, top + 1,
								Theme.flat.Foreground, Theme.flat.Background);
						top += height;
					}
				}
			}
			else
			{
				// Top
				hline(bmp, l, t, r, Theme.flat.Frame);
				
				// Left
				vline(bmp, l, t + 1, b, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l + 1, b, r, Theme.flat.Frame);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					int top = t + 5;								// frame(2) + space(3)
					const int left = l + ((r - l) / 2) + 2;			// middle of tab
					const int count = int(strlen(s));
					const int height = text_height(font) + 2;		// space(1) + char + space(1)
					const int total_height_string = height * count;
					const int total_height_tab = (b - t) - 9;		// height - frame(2) - space(3) space(3) - frame(2)
					if (total_height_string < total_height_tab)
						top += ((total_height_tab - total_height_string) / 2);
					char str[2];
					str[1] = 0;
					for (int i = 0; i < count; ++i)
					{
						str[0] = s[i];
						textout_centre_ex(bmp, font, str, left, top + 1,
								Theme.flat.Foreground, Theme.flat.Background);
						top += height;
					}
				}
			}
		}

		void TabLeftVert::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			rect(bmp, _leftTab, _topTab, _rightTab, _bottomTab, Theme.flat.Frame);
			if (!tabsOnly)
			{
				rectfill(bmp, _leftTab + 1, _topTab + 1, _rightTab - 1,
						_bottomTab - 1, Theme.flat.Background);
			}
			
			// Paint tabs
			rectfill(bmp, _left, _topTab, _leftTab - 1, _bottomTab, Theme.flat.Background);
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
				paint_tab(bmp, t.l, t.t, t.r, t.b, w, _pages[i]->title());
			}
		}
		
		void TabLeftVert::recalc()
		{
			_tabs.clear();
			const int flags = _owner.flags();
			switch (flags & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across height
				{
					// Get list of tab positions
					distribution t;
					distribute(height_controls(), int(_pages.size()), t);
					
					// Calculate new coordinates
					const int right = _leftTab - 1;
					const int left = right - _charWidth - 6;	// frame(1) + space(3) + char(~8) + space(3)
					const int count = int(t.size());
					for (int i = 0; i < count; ++i)
					{
						const std::pair<int, int>& ii = t[i];
						_tabs.push_back(tab(left, _topTab + ii.first, right,
								_topTab + ii.second + (i == count - 1 ? 0 : 1)));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to number of characters in title
				{
					// Calculate new coordinates
					int top = _topTab;
					const int right = _leftTab - 1;
					const int left = _leftTab - _charWidth - 6;	// frame(1) + space(3) + char(~8) + space(3)
					const int char_height = text_height(font) + 2;	// space(1) + char + space(1)
					const int count = int(_pages.size());
					for (int i = 0; i < count; ++i)
					{
						const int height = (int(_pages[i]->title_string().size())
								* char_height) + 8;			// frame(1) + space(3) + char's + space(3) + frame(1)
						_tabs.push_back(tab(left, top, right, top + height));
						top += height;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
//=============================================================================
// class TabLeftVert3D implementation
//=============================================================================

		TabLeftVert3D::TabLeftVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Right and bottom are just the right and bottom of the control
			_rightTab = _right;
			_bottomTab = _bottom;
			
			// Calculate the right side of the tabs
			_charWidth = (text_length(font, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRr"
					"SsTtUuVvWwXxYyZz1234567890") / 62);
			_leftTab = _left + (_charWidth + 10);			// selected frame(2) + frame(2) + space(3) + character(~8) + space(3)
		}
		
		bool TabLeftVert3D::isInTabs(int x, int y) const
		{
			return (x >= _left && x < _leftTab && y >= _topTab && y <= _bottom);
		}
		
		void TabLeftVert3D::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s)
		{
			if (w == sel)
			{
				t -= 2;
				b += 2;
				l -= 2;
				
				// Overwrite original frame
				vline(bmp, r + 1, t + 2, b - 2, Theme.GRAY);
				vline(bmp, r + 2, t + 2, b - 2, Theme.GRAY);
				
				// Top
				hline(bmp, l, t, r + 1, Theme.GRAY);
				hline(bmp, l, t + 1, r + 1, Theme.WHITE);
				
				// Left
				vline(bmp, l, t + 1, b - 1, Theme.GRAY);
				vline(bmp, l + 1, t + 2, b - 2, Theme.WHITE);
				
				// Bottom
				hline(bmp, l, b, r, Theme.GRAY_VERY_DARK);
				hline(bmp, l + 1, b - 1, r + 1, Theme.GRAY_DARK);

				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					const int left = l + ((r - l) / 2) + 2;
					const int count = int(strlen(s));
					const int height = text_height(font) + 2;
					int top = t + 5;
					const int total_height_string = height * count;
					const int total_height_tab = (b - t) - 9;		// height - frame(2) - space(3) space(3) - frame(2)
					if (total_height_string < total_height_tab)
						top += ((total_height_tab - total_height_string) / 2);
					char str[2];
					str[1] = 0;
					for (int i = 0; i < count; ++i)
					{
						str[0] = s[i];
						textout_centre_ex(bmp, font, str, left, top + 1,
								Theme.BLACK, -1);
						top += height;
					}
				}
			}
			else
			{
				// Top
				if (w != sel_plus_1)
				{
					hline(bmp, l, t, r, Theme.GRAY);
					hline(bmp, l, t + 1, r, Theme.WHITE);
				}
				
				// Left
				vline(bmp, l, t + (w == sel_plus_1 ? 2 : 1), b - 1, Theme.GRAY);
				vline(bmp, l + 1, t + (2 == sel_plus_1 ? 3 : 2), b - 2, Theme.WHITE);
				
				// Bottom
				hline(bmp, l, b, r, Theme.GRAY_VERY_DARK);
				hline(bmp, l + 1, b - 1, r, Theme.GRAY_DARK);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					int top = t + 5;								// frame(2) + space(3)
					const int left = l + ((r - l) / 2) + 2;			// middle of tab
					const int count = int(strlen(s));
					const int height = text_height(font) + 2;		// space(1) + char + space(1)
					const int total_height_string = height * count;
					const int total_height_tab = (b - t) - 9;		// height - frame(2) - space(3) space(3) - frame(2)
					if (total_height_string < total_height_tab)
						top += ((total_height_tab - total_height_string) / 2);
					char str[2];
					str[1] = 0;
					for (int i = 0; i < count; ++i)
					{
						str[0] = s[i];
						textout_centre_ex(bmp, font, str, left, top + 1,
								Theme.BLACK, -1);
						top += height;
					}
				}
			}
		}

		void TabLeftVert3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			box(bmp, _leftTab, _topTab, _rightTab, _bottomTab, false, (tabsOnly ? -1 : Theme.GRAY));
			
			// Paint tabs
			rectfill(bmp, _left, _topTab, _leftTab - 1, _bottomTab, Theme.GRAY);
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
				paint_tab(bmp, t.l, t.t, t.r, t.b, w, _pages[i]->title());
			}
		}
		
		void TabLeftVert3D::recalc()
		{
			_tabs.clear();
			const int flags = _owner.flags();
			switch (flags & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across height
				{
					// Get list of tab positions
					distribution t;
					distribute(height_controls() - 4, int(_pages.size()), t);
					
					// Calculate new coordinates
					const int top = _topTab + 2;
					const int right = _leftTab - 1;
					const int left = right - _charWidth - 7;	// frame(2) + space(3) + char(~8) + space(3)
					const distribution::const_iterator end = t.end();
					for (distribution::const_iterator i = t.begin(); i != end; ++i)
					{
						_tabs.push_back(tab(left, top + i->first, right, top + i->second));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to number of characters in title
				{
					// Calculate new coordinates
					int top = _topTab + 2;
					const int right = _leftTab - 1;
					const int left = _leftTab - _charWidth - 7;	// frame(2) + space(3) + char(~8) + space(3)
					const int char_height = text_height(font) + 2;	// space(1) + char + space(1)
					const int count = int(_pages.size());
					for (int i = 0; i < count; ++i)
					{
						const int height = (int(_pages[i]->title_string().size())
								* char_height) + 10;			// frame(2) + space(3) + char's + space(3) + frame(2)
						_tabs.push_back(tab(left, top, right, top + height - 1));
						top += height;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
	}
}
