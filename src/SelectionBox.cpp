#include "precompiled.hpp"
#include "yacg/SelectionBox.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace
{
}

//=============================================================================
// class SelectionBox implementation
//=============================================================================

SelectionBox::SelectionBox(_Manager& m)
		:
		_ControlExLast(m, m_down),
		_left(0),
		_right(0),
		_top(0),
		_bottom(0),
		_pattern(0),
		_new(false),
		_callback(0)
{
	_pattern = create_bitmap(2, 2);
	putpixel(_pattern, 0, 0, Theme.BLACK);
	putpixel(_pattern, 0, 1, Theme.WHITE);
	putpixel(_pattern, 1, 0, Theme.WHITE);
	putpixel(_pattern, 1, 1, Theme.BLACK);
}

SelectionBox::~SelectionBox()
{
	destroy_bitmap(_pattern);
}

#ifdef	_DEBUG
void SelectionBox::dump() const
{
}
#endif

int SelectionBox::height() const
{
	_new = false;
	return ((_bottom - _top) + 1);
}

bool SelectionBox::isNew() const
{
	return _new;
}

int SelectionBox::left() const
{
	_new = false;
	return _left;
}

bool SelectionBox::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	// Save bitmap contents
	auto_bitmap save(create_bitmap(bmp->w, bmp->h));
	{
		Update u(bmp);
		blit(bmp, save, 0, 0, 0, 0, bmp->w, bmp->h);
	}

	// Wait for button release
	drawing_mode(DRAW_MODE_COPY_PATTERN, _pattern, 0, 0);
	int mx = mouse_x;
	int my = mouse_y;
	rect(bmp, x, y, mx, my, Theme.BLACK);
	int sx;
	int sy;
	int ex;
	int ey;
	int w;
	int h;
	while ((mouse_b & 1) != 0)
	{
		if (mouse_x != mx || mouse_y != my)
		{
			// Determine cordinates of old selection
			sx = MIN(x, mx);
			sy = MIN(y, my);
			ex = MAX(x, mx);
			ey = MAX(y, my);
			w = (ex - sx) + 1;
			h = (ey - sy) + 1;

			// Remove old selection
			solid_mode();
			Update u(bmp);
			blit(save, bmp, sx, sy, sx, sy, w, 1);				// top
			blit(save, bmp, sx, ey, sx, ey, w, 1);				// bottom
			blit(save, bmp, sx, sy + 1, sx, sy + 1, 1, h - 1);	// left
			blit(save, bmp, ex, sy + 1, ex, sy + 1, 1, h - 1);	// right
			
			// Draw new selection
			drawing_mode(DRAW_MODE_COPY_PATTERN, _pattern, 0, 0);
			mx = mouse_x;
			my = mouse_y;
			rect(bmp, x, y, mx, my, Theme.BLACK);
		}
		else
			rest(1);
	}
	
	// Restore bitmap
	solid_mode();
	{
		Update u(bmp);
		blit(save, bmp, 0, 0, 0, 0, bmp->w, bmp->h);
	}

	// Update selection boundaries
	_left = MIN(x, mx);
	_top = MIN(y, my);
	_right = MAX(x, mx);
	_bottom = MAX(y, my);
	_new = true;
	
	// Call callback if provided
	if (_callback)
		_callback(*this, _data);
	
	return true;
}

int SelectionBox::bottom() const
{
	_new = false;
	return _bottom;
}

std::pair<SelectionBox::CALLBACK, void*> SelectionBox::on_selection() const
{
	return std::make_pair(_callback, _data);
}

void SelectionBox::on_selection(CALLBACK f, void* d)
{
	_callback = f;
	_data = d;
}

void SelectionBox::paint(BITMAP* bmp)
{
	// Nothing to do
}

int SelectionBox::right() const
{
	_new = false;
	return _right;
}

int SelectionBox::top() const
{
	_new = false;
	return _top;
}

int SelectionBox::width() const
{
	_new = false;
	return ((_right - _left) + 1);
}
