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
// class TabRightHoriz implementation
//=============================================================================

		TabRightHoriz::TabRightHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Left and bottom are just the left and bottom of the control
			_leftTab = _left;
			_bottomTab = _bottom;
			
			// _rightTop has to be calculated in recalc because it needs the
			// pages to determine.
			_rightTab = _right - (3 + 0 + 3 + 1 + 2);		// space + title + space + frame + selected frame
		}
		
		bool TabRightHoriz::isInTabs(int x, int y) const
		{
			return (x > _rightTab && x <= _right && y >= _topTab && y <= _bottom);
		}
		
		void TabRightHoriz::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s, bool down)
		{
			if (w == sel)
			{
				r += 2;
				
				// Overwrite original frame
				vline(bmp, l - 1, t + 1, b - 1, Theme.flat.Background);
				
				// Top
				hline(bmp, l, t, r - 1, Theme.flat.Frame);
				
				// Right
				vline(bmp, r, t, b, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.flat.Frame);
				
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
				hline(bmp, l, t, r - 1, Theme.flat.Frame);
				
				// Right
				vline(bmp, r, t, b, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.flat.Frame);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 3, Theme.flat.Foreground, Theme.flat.Background);
				}
			}
		}

		void TabRightHoriz::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			if (!tabsOnly)
			{
				rect(bmp, _left, _topTab, _rightTab, _bottom, Theme.flat.Frame);
				rectfill(bmp, _left + 1, _topTab + 1, _rightTab - 1, _bottom - 1,
						Theme.flat.Background);
			}

			// Paint tabs
			rectfill(bmp, _rightTab + 1, _topTab, _right, _bottom, Theme.flat.Background);
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
		
		void TabRightHoriz::recalc()
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
					longest += 9;								// space(3) + title + space(3) + frame(1) + selected frame(2)
					_rightTab = _right - longest;
					longest -= 2;								// Removed selected frame(2)
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab;
					const int height = text_height(font) + 6;	// frame(1) + space(2) + font + space(2) + frame(1)
					for (int i = 0; i < count; ++i)
					{
						_tabs.push_back(tab(_rightTab + 1, top, _rightTab + longest, top + height));
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
					longest += 9;								// space(3) + title + space(3) + frame(1) + selected frame(2)
					_rightTab = _right - longest;

					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab;
					const int height = text_height(font) + 6;	// frame(1) + space(2) + font + space(2) + frame(1)
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title()) + 7;	// space(3) + title + space(3) + frame(1)
						_tabs.push_back(tab(_rightTab + 1, top, _rightTab + length + 1, top + height));
						top += height;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}
		
		int TabRightHoriz::right_controls() const
		{
			return _rightTab;
		}
		
		int TabRightHoriz::width_controls() const
		{
			return ((_rightTab - _left) + 1);
		}
		
//=============================================================================
// class TabRightHoriz3D implementation
//=============================================================================

		TabRightHoriz3D::TabRightHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Left and bottom are just the left and bottom of the control
			_leftTab = _left;
			_bottomTab = _bottom;
			
			// _rightTop has to be calculated in recalc because it needs the
			// pages to determine.
			_rightTab = _right - (3 + 0 + 3 + 2 + 2);		// space + title + space + frame + selected frame
		}
		
		bool TabRightHoriz3D::isInTabs(int x, int y) const
		{
			return (x > _rightTab && x <= _right && y >= _topTab && y <= _bottom);
		}
		
		void TabRightHoriz3D::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s, int pr)
		{
			if (w == sel)
			{
				t -= 2;
				b += 2;
				r += 2;
				
				// Overwrite original frame
				vline(bmp, l - 1, t + 2, b - 2, Theme.GRAY);
				vline(bmp, l - 2, t + 2, b - 2, Theme.GRAY);
				
				// Top
				hline(bmp, l - 1, t, r - 1, Theme.GRAY);
				hline(bmp, l - 2, t + 1, r - 2, Theme.WHITE);
				
				// Right
				vline(bmp, r, t, b, Theme.GRAY_VERY_DARK);
				vline(bmp, r - 1, t + 1, b - 1, Theme.GRAY_DARK);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.GRAY_VERY_DARK);
				hline(bmp, l - 1, b - 1, r - 2, Theme.GRAY_DARK);
				
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
					hline(bmp, l, t, r - 1, Theme.GRAY);
					hline(bmp, l, t + 1, r - 2, Theme.WHITE);
				}
				else if (r > pr)
				{
					hline(bmp, pr + 3, t, r - 1, Theme.GRAY);
					hline(bmp, pr + 3, t + 1, r - 2, Theme.WHITE);
				}
				
				// Right
				vline(bmp, r, t, b, Theme.GRAY_VERY_DARK);
				vline(bmp, r - 1, t + 1, b - 1, Theme.GRAY_DARK);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.GRAY_VERY_DARK);
				hline(bmp, l, b - 1, r - 2, Theme.GRAY_DARK);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 1, r - 2, b - 1);
					textout_centre_ex(bmp, font, s, l + ((r - l) / 2),
							t + 4, Theme.BLACK, -1);
				}
			}
		}

		void TabRightHoriz3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			box(bmp, _left, _topTab, _rightTab, _bottom, false, (tabsOnly ? - 1
					: Theme.GRAY));

			// Paint tabs
			rectfill(bmp, _rightTab + 1, _topTab, _right, _bottom, Theme.GRAY);
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
				paint_tab(bmp, t.l, t.t, t.r, t.b, w, _pages[i]->title(), i > 0
						? _tabs[i - 1].r : 0);
			}
		}
		
		void TabRightHoriz3D::recalc()
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
					longest += 10;								// space(3) + title + space(3) + frame(2) + selected frame(2)
					_rightTab = _right - longest;
					longest -= 2;								// Removed selected frame(2)
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab + 2;
					const int height = text_height(font) + 8;	// frame(2) + space(2) + font + space(2) + frame(2)
					for (int i = 0; i < count; ++i)
					{
						_tabs.push_back(tab(_rightTab + 1, top, _rightTab + longest, top + height - 1));
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
					longest += 10;								// space(3) + title + space(3) + frame(2) + selected frame(2)
					_rightTab = _right - longest;

					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab + 2;
					const int height = text_height(font) + 8;	// frame(2) + space(2) + font + space(2) + frame(2)
					for (int i = 0; i < count; ++i)
					{
						const int length = text_length(font, _pages[i]->title()) + 8;	// space(3) + title + space(3) + frame(2)
						_tabs.push_back(tab(_rightTab + 1, top, _rightTab + length, top + height - 1));
						top += height;
					}
				}
				break;
			
			case Tab::w_page:		// User supplied width
				break;
			}
		}

		int TabRightHoriz3D::right_controls() const
		{
			return _rightTab;
		}
		
		int TabRightHoriz3D::width_controls() const
		{
			return ((_rightTab - _left) + 1);
		}
		
//=============================================================================
// class TabRightVert implementation
//=============================================================================

		TabRightVert::TabRightVert(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Left and bottom are just the left and bottom of the control
			_leftTab = _left;
			_bottomTab = _bottom;
			
			// Calculate the left side of the tabs
			_charWidth = (text_length(font, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRr"
					"SsTtUuVvWwXxYyZz1234567890") / 62);
			_rightTab = _right - (_charWidth + 9);			// space(3) + character + space(3) + frame(1) + selected frame(2)
		}
		
		bool TabRightVert::isInTabs(int x, int y) const
		{
			return (x > _rightTab && x <= _right && y >= _topTab && y <= _bottom);
		}
		
		void TabRightVert::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s, bool down)
		{
			if (w == sel)
			{
				r += 2;
				
				// Overwrite original frame
				vline(bmp, l - 1, t + 1, b - 1, Theme.flat.Background);
				
				// Top
				hline(bmp, l, t, r - 1, Theme.flat.Frame);
				
				// Right
				vline(bmp, r, t, b, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.flat.Frame);
				
				// Determine correct color
				const int text_color = down ? color_plus_delta(Theme.flat.Foreground, 64)
						: Theme.flat.Foreground;

				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					const int left = l + (((r - l) + 1) / 2);
					const int count = int(strlen(s));
					const int height = text_height(font) + 2;
					int top = t + 4;
					const int total_height_string = height * count;
					const int total_height_tab = (b - t) - 7;		// height - frame(1) - space(3) space(3) - frame(1)
					if (total_height_string < total_height_tab)
						top += ((total_height_tab - total_height_string) / 2);
					char str[2];
					str[1] = 0;
					for (int i = 0; i < count; ++i)
					{
						str[0] = s[i];
						textout_centre_ex(bmp, font, str, left, top + 1,
								text_color, Theme.flat.Background);
						top += height;
					}
				}
			}
			else
			{
				// Top
				hline(bmp, l, t, r - 1, Theme.flat.Frame);
				
				// Right
				vline(bmp, r, t, b, Theme.flat.Frame);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.flat.Frame);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					const int left = l + (((r - l) + 1) / 2);
					const int count = int(strlen(s));
					const int height = text_height(font) + 2;
					int top = t + 4;
					const int total_height_string = height * count;
					const int total_height_tab = (b - t) - 7;		// height - frame(1) - space(3) space(3) - frame(1)
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

		void TabRightVert::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			rect(bmp, _left, _topTab, _rightTab, _bottom, Theme.flat.Frame);
			if (!tabsOnly)
			{
				rectfill(bmp, _left + 1, _topTab + 1, _rightTab - 1, _bottom - 1,
						Theme.flat.Background);
			}

			// Paint tabs
			rectfill(bmp, _rightTab + 1, _topTab, _right, _bottom, Theme.flat.Background);
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
		
		void TabRightVert::recalc()
		{
			const int flags = _owner.flags();
			switch (flags & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across height
				{
					// Get list of tab positions
					distribution t;
					distribute(height_controls(), int(_pages.size()), t);
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					const int top = _topTab;
					const int left = _rightTab + 1;
					const int right = _rightTab + _charWidth + 7;	// space(3) + char + space(3) + frame(1)
					const int count = int(t.size());
					for (int i = 0; i < count; ++i)
					{
						const std::pair<int, int>& ii = t[i];
						_tabs.push_back(tab(left, top + ii.first, right,
								top + ii.second + (i == count - 1 ? 0 : 1)));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to number of characters in title
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

					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab;
					const int left = _rightTab + 1;
					const int right = _rightTab + _charWidth + 7;	// space(3) + char + space(3) + frame(1)
					const int char_height = text_height(font) + 2;	// space(1) + char + space(1)
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
// class TabRightVert3D implementation
//=============================================================================

		TabRightVert3D::TabRightVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title)
				:
				TabBase(o, l, t, w, h, f, title)
		{
			// Left and bottom are just the left and bottom of the control
			_leftTab = _left;
			_bottomTab = _bottom;
			
			// Calculate the left side of the tabs
			_charWidth = (text_length(font, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRr"
					"SsTtUuVvWwXxYyZz1234567890") / 62);
			_rightTab = _right - (_charWidth + 10);			// space(3) + character + space(3) + frame(2) + selected frame(2)
		}
		
		bool TabRightVert3D::isInTabs(int x, int y) const
		{
			return (x > _rightTab && x <= _right && y >= _topTab && y <= _bottom);
		}
		
		void TabRightVert3D::paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
				const char* s)
		{
			if (w == sel)
			{
				t -= 2;
				b += 2;
				r += 2;
				
				// Overwrite original frame
				vline(bmp, l - 1, t + 2, b - 2, Theme.GRAY);
				vline(bmp, l - 2, t + 2, b - 2, Theme.GRAY);
				
				// Top
				hline(bmp, l - 1, t, r - 1, Theme.GRAY);
				hline(bmp, l - 2, t + 1, r - 2, Theme.WHITE);
				
				// Right
				vline(bmp, r, t, b, Theme.GRAY_VERY_DARK);
				vline(bmp, r - 1, t + 1, b - 1, Theme.GRAY_DARK);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.GRAY_VERY_DARK);
				hline(bmp, l - 1, b - 1, r - 2, Theme.GRAY_DARK);

				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					const int left = l + (((r - l) + 1) / 2);
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
					hline(bmp, l, t, r - 1, Theme.GRAY);
					hline(bmp, l, t + 1, r - 2, Theme.WHITE);
				}
				
				// Right
				vline(bmp, r, (w == sel_plus_1 ? t + 2 : t), b, Theme.GRAY_VERY_DARK);
				vline(bmp, r - 1, t + (w == sel_plus_1 ? 2 : 1), b - 1, Theme.GRAY_DARK);
				
				// Bottom
				hline(bmp, l, b, r - 1, Theme.GRAY_VERY_DARK);
				hline(bmp, l, b - 1, r - 2, Theme.GRAY_DARK);
				
				// Title
				{
					Clipper c(bmp, l + 2, t + 2, r - 2, b - 2);
					const int left = l + (((r - l) + 1) / 2);
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
		}

		void TabRightVert3D::paint_tabs(BITMAP* bmp, bool tabsOnly, bool down)
		{
			// Paint frame
			box(bmp, _left, _topTab, _rightTab, _bottom, false, (tabsOnly ? -1 : Theme.GRAY));

			// Paint tabs
			rectfill(bmp, _rightTab + 1, _topTab, _right, _bottom, Theme.GRAY);
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
		
		void TabRightVert3D::recalc()
		{
			const int flags = _owner.flags();
			switch (flags & Tab::w_mask)
			{
			case Tab::w_equal:		// Distributed equally across height
				{
					// Get list of tab positions
					distribution t;
					distribute(height_controls() - 4, int(_pages.size()), t);
					
					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					const int top = _topTab + 2;
					const int left = _rightTab + 1;
					const int right = _rightTab + _charWidth + 8;
					const distribution::const_iterator end = t.end();
					for (distribution::const_iterator i = t.begin(); i != end; ++i)
					{
						_tabs.push_back(tab(left, top + i->first, right, top + i->second));
					}
				}
				break;
			
			case Tab::w_title:		// Distributed according to number of characters in title
				{

					// Clear old list of coordinates
					_tabs.clear();
					
					// Calculate new coordinates
					int top = _topTab + 2;
					const int left = _rightTab + 1;
					const int right = _rightTab + _charWidth + 8;
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
