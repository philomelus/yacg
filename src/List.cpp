#include "precompiled.hpp"
#include "yacg/List.hpp"
#include "yacg/Scroll.hpp"
#include "yacg/Title.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

// TODO:
//	No callbacks at all - need for item selection changed, etc.

//=============================================================================
// local
//=============================================================================

namespace
{
#ifdef	_DEBUG
	void dumper(const std::pair<std::string, void*>& i)
	{
		TRACE("  Item:\n");
		TRACE("   Title: \"%s\"\n", i.first.c_str());
		TRACE("   Data: $%08X\n", i.second);
	}
#endif

	void max_length(const std::pair<std::string, void*>& i, const FONT* f, int* l)
	{
		const int length = text_length(f, i.first.c_str());
		if (length > *l)
			*l = length;
	}
}

namespace yacg
{
	namespace ListImpl
	{
//=============================================================================
// class Base declaation
//=============================================================================

		class Base
		{
		public:
			static const int dirty_frame = 0x00000001;
			static const int dirty_title = 0x00000002;
			static const int dirty_hscroll = 0x00000004;
			static const int dirty_vscroll = 0x00000008;
			static const int dirty_items = 0x00000010;
			
		public:
			Base(List& o, int l, int t, int w, int h, int f, const char* title);
			virtual ~Base();
			
			int bottom() const;

			int height() const;

			int left() const;

			int right() const;
			
			int top() const;

			int width() const;
			
			// Container API
			List::iterator begin();
			List::const_iterator begin() const;
			bool empty() const;
			List::iterator end();
			List::const_iterator end() const;
			void erase(List::iterator i);
			List::iterator insert(List::iterator i, const char* t, void* d);
			int size() const;

			// Implmentation specific
			virtual void paint(BITMAP* bmp) = 0;

			// For overrides
			void active_changed(bool o, bool n);
			void flags_changed();
			bool mouse_down(BITMAP* b, int x, int y, int m);
#ifdef	_DEBUG
			void dump() const;
#endif

			const char* title() const;
			void title(const char* t);

			// Control related
			virtual void reset(const char* title) = 0;

		protected:
			int calc_hrange() const;
			virtual void paint_items(BITMAP* bmp) = 0;
			
		protected:
			static void vhit(Scroll& s, void* d, BITMAP* b, int o, int c);
			static void hhit(Scroll& s, void* d, BITMAP* b, int o, int c);
			
		protected:
			typedef std::list<std::pair<std::string, void*> > container;

		protected:
			List& _owner;
			
			std::auto_ptr<Scroll> _hscroll;
			std::auto_ptr<Scroll> _vscroll;

			std::auto_ptr<Title> _title;
					
			int _left;
			int _left2;			// Left of list area
			int _right;
			int _right2;		// Right of list area
			
			int _top;
			int _top2;			// Top of list area
			int _bottom;
			int _bottom2;		// Bottom of list area
			
			int _width;
			int _width2;		// Width of list area
			int _height;
			int _height2;		// Height of list area
			
			container _items;
			
			FONT* _font;
			
			int _maxWidth;		// Maximum width of items
			int _itemHeight;	// Height of item within list area
			int _charAverage;	// Average width of a character in current font
			
			int _selected;
		};

//=============================================================================
// class Impl declaation
//=============================================================================

		class Impl : public Base
		{
		public:
			Impl(List& o, int l, int t, int w, int h, int f, const char* title);

			void paint(BITMAP* bmp);

			void reset(const char* title);

		private:
			void paint_items(BITMAP* bmp);
		};

//=============================================================================
// class Impl3D declaation
//=============================================================================

		class Impl3D : public Base
		{
		public:
			Impl3D(List& o, int l, int t, int w, int h, int f, const char* title);

			void paint(BITMAP* bmp);

			void reset(const char* title);

		private:
			void paint_items(BITMAP* bmp);
		};

//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(List& o, int x, int y, int w, int h, int f, const char* title)
				:
				_owner(o),
				_left(x),
				_right(x + w - 1),
				_top(y),
				_bottom(y + h - 1),
				_width(w),
				_height(h),
				_font(0),
				_selected(-1)
		{
		}

		Base::~Base()
		{
		}
		
		void Base::active_changed(bool o, bool n)
		{
			const int flags = _owner.flags();
			if ((flags & List::t_mask) == List::t_title)
				_title->active(n);
			if ((flags & List::h_mask) == List::h_scroll)
				_hscroll->active(n);
			if ((flags & List::v_mask) == List::v_scroll)
				_vscroll->active(n);
			_owner.dirty(List::all);
		}

		List::iterator Base::begin()
		{
			return _items.begin();
		}
		
		List::const_iterator Base::begin() const
		{
			return _items.begin();
		}
		
		int Base::bottom() const
		{
			return _bottom;
		}
		
		int Base::calc_hrange() const
		{
			const int item_chars = (_maxWidth / _charAverage) + (_maxWidth % _charAverage > 0 ? 1 : 0);
			const int list_chars = (_width2 / _charAverage) + (_width2 % _charAverage > 0 ? 1 : 0);
			if (list_chars > item_chars)
				return 1;
			return _maxWidth;
		}
		
#ifdef	_DEBUG
		void Base::dump() const
		{
			const int flags = _owner.flags();
			if ((flags & List::t_mask) == List::t_title)
			{
				TRACE(" Title\n");
				TRACE(">>>>>>>>>>\n");
				_title->dump();
				TRACE("<<<<<<<<<<\n");
			}
			dump_xywh(_left, _top, _width, _top, _right, _bottom);
			if ((flags & List::h_mask) == List::h_scroll)
			{
				TRACE(" Horizontal Scroll\n");
				TRACE(">>>>>>>>>>\n");
				_hscroll->dump();
				TRACE("<<<<<<<<<<\n");
			}
			if ((flags & List::v_mask) == List::v_scroll)
			{
				TRACE(" Vertical Scroll");
				TRACE(">>>>>>>>>>\n");
				_vscroll->dump();
				TRACE("<<<<<<<<<<\n");
			}
			TRACE(" Items: %d\n", int(_items.size()));
			if (!_items.empty())
			{
				TRACE(">>>>>>>>>>\n");
				std::for_each(_items.begin(), _items.end(), dumper);
				TRACE("<<<<<<<<<<\n");
			}
		}
#endif

		bool Base::empty() const
		{
			return _items.empty();
		}
		
		List::iterator Base::end()
		{
			return _items.end();
		}
		
		List::const_iterator Base::end() const
		{
			return _items.end();
		}
		
		void Base::erase(List::iterator i)
		{
			_items.erase(i);
			if ((_owner.flags() & List::v_mask) == List::v_scroll)
				_vscroll->range(int(_items.size()));
		}
		
		void Base::hhit(Scroll& s, void* d, BITMAP* b, int o, int n)
		{
#define	_this	reinterpret_cast<Base*>(d)
			_this->paint_items(b);
#undef	_this
		}

		int Base::height() const
		{
			return _height;
		}
		
		List::iterator Base::insert(List::iterator i, const char* t, void* d)
		{
			// Insert item
			List::iterator r = _items.insert(i, std::make_pair(std::string(t), d));
			
			// Update max line width and horizontal scroll range if needed
			const int length = text_length(_font ? _font : ::font, t);
			if (length > _maxWidth)
			{
				_maxWidth = length;
				if ((_owner.flags() & List::h_mask) == List::h_scroll)
					_hscroll->range(calc_hrange());
			}

			// Update vertical scroll range if needed
			if ((_owner.flags() & List::v_mask) == List::v_scroll)
				_vscroll->range(int(_items.size()));

			return r;
		}
		
		int Base::left() const
		{
			return _left;
		}
		
		bool Base::mouse_down(BITMAP* b, int x, int y, int m)
		{
			if (x >= _left + 2 && x <= _right2 && y >= _top2 && y <= _bottom2)
			{
				// Determine which item has been clicked on and select it
				_selected = (y - _top2) / _itemHeight;
				if ((_owner.flags() & List::v_mask) == List::v_scroll)
					_selected += _vscroll->setting();
				paint_items(b);
				while ((mouse_b & 1) != 0)
					;
				return true;
			}
			else
			{
				bool handled = false;
				const int flags = _owner.flags();
				if ((flags & List::h_mask) == List::h_scroll)
					handled = _hscroll->mouse_down(b, x, y, m);
				if (!handled && (flags & List::v_mask) == List::v_scroll)
					handled = _vscroll->mouse_down(b, x, y, m);
				return handled;
			}
		}

		int Base::right() const
		{
			return _right;
		}
		
		int Base::size() const
		{
			return int(_items.size());
		}

		void Base::vhit(Scroll& s, void* d, BITMAP* b, int o, int n)
		{
#define	_this	reinterpret_cast<Base*>(d)
			_this->paint_items(b);
#undef	_this
		}
		
		const char* Base::title() const
		{
			if ((_owner.flags() & List::t_mask) == List::t_notitle)
				return 0;
			return _title->title();
		}
		
		void Base::title(const char* t)
		{
			if ((_owner.flags() & List::t_mask) == List::t_title)
				_title->title(t);
		}

		int Base::top() const
		{
			return _top;
		}
		
		int Base::width() const
		{
			return _width;
		}

//=============================================================================
// class Impl implementation
//=============================================================================

		Impl::Impl(List& o, int x, int y, int w, int h, int f, const char* title)
				:
				Base(o, x, y, w, h, f, title)
		{
			reset(title);
		}

		void Impl::paint(BITMAP* bmp)
		{
			const int dirty = _owner.dirty();
			
			const int flags = _owner.flags();
			
			const bool isHoriz = ((flags & List::h_mask) == List::h_scroll);
			const bool isVert = ((flags & List::v_mask) == List::v_scroll);

			if ((dirty & dirty_frame) != 0)
			{
				Update u(bmp);
			
				rect(bmp, _left, _top, _right, _bottom, Theme.flat.Frame);
				
				// If both scrolls, update area where they connect
				if (isVert && isHoriz)
				{
					rectfill(bmp, _right - Scroll::h_size, _bottom - Scroll::v_size,
							_right - 1, _bottom - 1, Theme.BLACK);
				}
 			}

			// Paint the items
			if ((dirty & dirty_items) != 0)
				paint_items(bmp);

			// Paint horizontal screen if needed
			if (isHoriz && (dirty & dirty_hscroll) != 0)
				_hscroll->paint(bmp);
			
			// Paint vertical scroll if needed
			if (isVert && (dirty & dirty_vscroll) != 0)
				_vscroll->paint(bmp);
			
			// Pain title if needed
			if ((flags & List::t_mask) == List::t_title && (dirty & dirty_title) != 0)
				_title->paint(bmp);
		}

		void Impl::paint_items(BITMAP* bmp)
		{
			text_mode(-1);

			// Calculate starting item
			container::const_iterator i = _items.begin();
			int current = (((_owner.flags() & List::v_mask) == List::v_scroll) ? _vscroll->setting() : 0);
			std::advance(i, current);

			// Determine horizontal offset (based on h scroll)
			int left_start = 0;
			if ((_owner.flags() & List::h_mask) == List::h_scroll)
				left_start += _hscroll->setting();

			// Create item bitmap
			BITMAP* temp = create_bitmap(_maxWidth * 2, _itemHeight);

			// Loop through and paint the items			
			const container::const_iterator end = _items.end();
			const int x = _left2 + 2;
			int y = _top2;
			{
				Clipper c(bmp, _left + 2, _top2, _right2, _bottom2);
				Update u(bmp);
				Acquire a1(bmp);
				Acquire a2(temp);
				rectfill(bmp, _left2, _top2, _right2, _bottom2, Theme.flat.Background);
				while (i != end)
				{
					// Clear the item bitmap
					clear_to_color(temp, current == _selected ? Theme.flat.SelectionBackground
							: Theme.flat.Background);
					
					// Print the line
					if (current == _selected)
					{
						textout_ex(temp, _font ? _font : ::font, i->first.c_str(), 0, 1,
								Theme.flat.SelectionForeground, Theme.flat.SelectionBackground);
					}
					else
					{
						textout_ex(temp, _font ? _font : ::font, i->first.c_str(), 0, 1,
								Theme.flat.Foreground, Theme.flat.Background);
					}

					// Update the item on user bitmap
					blit(temp, bmp, left_start, 0, x, y, _width2, _itemHeight);

					// Prepare for next item
					y += _itemHeight;
					if (y > _bottom2)
						break;
					++i;
					++current;
				}
			}
			
			// Clean up
			destroy_bitmap(temp);
		}
		
		void Impl::reset(const char* title)
		{
			const int flags = _owner.flags();
			const FONT* f = _font ? _font : ::font;
			
			// Reset positions
			_left2 = _left + 1;
			_right2 = _left + _width - 2;
			_top2 = _top + 1;
			_bottom2 = _bottom - 1;
			_width2 = _width - 2;
			_height2 = _height - 2;
			
			// Recalculate item height
			_itemHeight = text_height(f) + 2;
			
			// Recalculate item's maximum width
			_maxWidth = 0;
			std::for_each(_items.begin(), _items.end(), bind(max_length, _1, f, &_maxWidth));
			_charAverage = (text_length(f, "AaBbCcDdEeFfGgHhIiJjKkLlMmNn"
					"OoPpQqRrSsTtUuVvWwXxYyZz1234567890") / 62);
			
			// Update title per new flags
			const bool isTitle = ((flags & List::t_mask) == List::t_title);
			const bool wasTitle = (_title.get() != 0);
			if (isTitle != wasTitle)
			{
				if (wasTitle)
				{
					// Get rid of title
					delete _title.release();
				}
				else
				{
					// Add title
					_title = std::auto_ptr<Title>(new Title(_owner.manager(),
							_left, _top, _width, _Control::calc,
							Title::h_center | Title::v_top | Title::f_out, title));
					_top2 = _title->bottom() + 1;
					_height2 -= (_title->height() - 1);
				}
			}
			else if (isTitle)
			{
				// Make sure top and height are reset for title space
				_top2 = _title->bottom() + 1;
				_height2 -= (_title->height() - 1);
			}

			// Update scrolls per new flags
			const bool isHoriz = ((flags & List::h_mask) == List::h_scroll);
			const bool wasHoriz = (_hscroll.get() != 0);
			const bool isVert = ((flags & List::v_mask) == List::v_scroll);
			const bool wasVert = (_vscroll.get() != 0);
			if (isVert != wasVert)
			{
				if (wasVert)
				{
					// Get rid of vertical scroll
					delete _vscroll.release();
				}
				else
				{
					// Remove scroll area from control
					_right2 -= (Scroll::v_size - 1);
					_width2 -= (Scroll::v_size - 1);
					
					// Create vertical scroll
					const int height = isHoriz ? _height2 - (Scroll::h_size - 1)
							+ 2 : _height2 + 2;
					_vscroll = std::auto_ptr<Scroll>(new Scroll(_owner.manager(),
							_right2 + 1, _top2 - 1, height, Scroll::t_vertical));
					
					// Hook up our vertical scroll callback
					_vscroll->callback(vhit, this);
					
					// Set range, line, and page sizes
					_vscroll->range(int(_items.size()));
					_vscroll->line(1);
					_vscroll->page(height / _itemHeight);
				}
			}
			else if (isVert)
			{
				// Make sure vertical scroll is at correct position and height
				const int height = isHoriz ? _height2 - (Scroll::v_size - 1) : _height2;
				_right2 -= (Scroll::h_size - 1);
				_width2 -= (Scroll::h_size - 1);
				_vscroll->left(_right2 + 1);
				_vscroll->top(_top2);
				_vscroll->length(height);

				// Set range, line, and page sizes
				_vscroll->range(int(_items.size()));
				_vscroll->line(1);
				_vscroll->page(height / _itemHeight);
			}
			if (isHoriz != wasHoriz)
			{
				if (wasHoriz)
				{
					// Get rid of horizontal scroll
					delete _hscroll.release();
				}
				else
				{
					// Remove scroll area from control
					_bottom2 -= (Scroll::v_size - 1);
					_height2 -= (Scroll::v_size - 1);
					
					// Create horizontal scroll
					_hscroll = std::auto_ptr<Scroll>(new Scroll(_owner.manager(),
							_left, _bottom2 + 1, _width2 + 2, Scroll::t_horizontal));
					
					// Hook up our horiontal scroll callback
					_hscroll->callback(hhit, this);

					// Update scroll range, line, page sizes
					_hscroll->range(calc_hrange());
					_hscroll->line(_charAverage);
					_hscroll->page((_width2 / _charAverage) * _charAverage);
				}
			}
			else if (isHoriz)
			{
				// Make sure horizontal scroll is correct width
				_hscroll->length(_width2 + 2);

				// Update scroll range, line, page sizes
				_hscroll->range(calc_hrange());
				_hscroll->line(_charAverage);
				_hscroll->page((_width2 / _charAverage) * _charAverage);
			}
			
			// Everything needs to be redrawn
			_owner.dirty(List::all);
		}

//=============================================================================
// class Impl3D implementation
//=============================================================================

		Impl3D::Impl3D(List& o, int x, int y, int w, int h, int f, const char* title)
				:
				Base(o, x, y, w, h, f, title)
		{
			reset(title);
		}

		void Impl3D::paint(BITMAP* bmp)
		{
			const int dirty = _owner.dirty();
			
			const int flags = _owner.flags();
			
			const bool isHoriz = ((flags & List::h_mask) == List::h_scroll);
			const bool isVert = ((flags & List::v_mask) == List::v_scroll);

			if ((dirty & dirty_frame) != 0)
			{
				Update u(bmp);
			
				// Frame top/left
				hline(bmp, _left, _top, _right - 1, Theme.GRAY_DARK);
				vline(bmp, _left, _top+ 1, _bottom - 1, Theme.GRAY_DARK);

				// Inset top/left
				hline(bmp, _left + 1, _top + 1, _right - 2, Theme.BLACK);
				vline(bmp, _left + 1, _top + 2, _bottom - 2, Theme.BLACK);

				// Inset bottom/right
				hline(bmp, _left + 1, _bottom - 1, _right - 1, Theme.GRAY);
				vline(bmp, _right - 1, _top + 2, _bottom - 1, Theme.GRAY);

				// Bottom/right
				hline(bmp, _left, _bottom, _right, Theme.WHITE);
				vline(bmp, _right, _top, _bottom - 1, Theme.WHITE);
				
				// If both scrolls, update area where the connect
				if (isVert && isHoriz)
				{
					rectfill(bmp, _right - Scroll::h_size, _bottom - Scroll::v_size,
							_right - 2, _bottom - 2, Theme.GRAY);
				}
 			}

			// Paint the items
			if ((dirty & dirty_items) != 0)
				paint_items(bmp);

			// Paint horizontal screen if needed
			if (isHoriz && (dirty & dirty_hscroll) != 0)
				_hscroll->paint(bmp);
			
			// Paint vertical scroll if needed
			if (isVert && (dirty & dirty_vscroll) != 0)
				_vscroll->paint(bmp);
			
			// Pain title if needed
			if ((flags & List::t_mask) == List::t_title && (dirty & dirty_title) != 0)
				_title->paint(bmp);
		}

		void Impl3D::paint_items(BITMAP* bmp)
		{
			text_mode(-1);

			// Calculate starting item
			container::const_iterator i = _items.begin();
			int current = (((_owner.flags() & List::v_mask) == List::v_scroll) ? _vscroll->setting() : 0);
			std::advance(i, current);

			// Determine horizontal offset (based on h scroll)
			int left_start = 0;
			if ((_owner.flags() & List::h_mask) == List::h_scroll)
				left_start += _hscroll->setting();

			// Create item bitmap
			BITMAP* temp = create_bitmap(_maxWidth * 2, _itemHeight);

			// Loop through and paint the items			
			const container::const_iterator end = _items.end();
			const int x = _left2 + 2;
			int y = _top2;
			{
				Clipper c(bmp, _left + 2, _top2, _right2, _bottom2);
				Update u(bmp);
				Acquire a1(bmp);
				Acquire a2(temp);
				rectfill(bmp, _left2, _top2, _right2, _bottom2, Theme.WHITE);
				while (i != end)
				{
					// Clear the item bitmap
					clear_to_color(temp, current == _selected ? Theme.SelectionBackground : Theme.WHITE);
					
					// Print the line
					if (current == _selected)
					{
						textout_ex(temp, _font ? _font : ::font, i->first.c_str(), 0, 1,
								Theme.SelectionForeground, Theme.SelectionBackground);
					}
					else
					{
						textout_ex(temp, _font ? _font : ::font, i->first.c_str(), 0, 1,
								Theme.BLACK, Theme.WHITE);
					}

					// Update the item on user bitmap
					blit(temp, bmp, left_start, 0, x, y, _width2, _itemHeight);

					// Prepare for next item
					y += _itemHeight;
					if (y > _bottom2)
						break;
					++i;
					++current;
				}
			}
			
			// Clean up
			destroy_bitmap(temp);
		}
		
		void Impl3D::reset(const char* title)
		{
			const int flags = _owner.flags();
			const FONT* f = _font ? _font : ::font;
			
			// Reset positions
			_left2 = _left + 2;
			_right2 = _left + _width - 3;
			_top2 = _top + 2;
			_bottom2 = _top + _height - 3;
			_width2 = _width - 4;
			_height2 = _height - 4;
			
			// Recalculate item height
			_itemHeight = text_height(f) + 2;
			
			// Recalculate item's maximum width
			_maxWidth = 0;
			std::for_each(_items.begin(), _items.end(), bind(max_length, _1, f, &_maxWidth));
			_charAverage = (text_length(f, "AaBbCcDdEeFfGgHhIiJjKkLlMmNn"
					"OoPpQqRrSsTtUuVvWwXxYyZz1234567890") / 62);
			
			// Update title per new flags
			const bool isTitle = ((flags & List::t_mask) == List::t_title);
			const bool wasTitle = (_title.get() != 0);
			if (isTitle != wasTitle)
			{
				if (wasTitle)
				{
					// Get rid of title
					delete _title.release();
				}
				else
				{
					// Add title
					_title = std::auto_ptr<Title>(new Title(_owner.manager(), _left + 2,
							_top + 2, _width - 4, _Control::calc,
							Title::h_center | Title::v_top | Title::f_out, title));
					_top2 = _title->bottom() + 1;
					_height2 -= _title->height();
				}
			}
			else if (isTitle)
			{
				// Make sure top and height are reset for title space
				_top2 = _title->bottom() + 1;
				_height2 -= _title->height();
			}

			// Update scrolls per new flags
			const bool isHoriz = ((flags & List::h_mask) == List::h_scroll);
			const bool wasHoriz = (_hscroll.get() != 0);
			const bool isVert = ((flags & List::v_mask) == List::v_scroll);
			const bool wasVert = (_vscroll.get() != 0);
			if (isVert != wasVert)
			{
				if (wasVert)
				{
					// Get rid of vertical scroll
					delete _vscroll.release();
				}
				else
				{
					// Remove scroll area from control
					_right2 -= Scroll::h_size;
					_width2 -= Scroll::h_size;
					
					// Create vertical scroll
					const int height = isHoriz ? _height2 - Scroll::v_size : _height2;
					_vscroll = std::auto_ptr<Scroll>(new Scroll(_owner.manager(),
							_right2 + 1, _top2, height + (isHoriz ? 1 : 0),
							Scroll::t_vertical));
					
					// Hook up our vertical scroll callback
					_vscroll->callback(vhit, this);
					
					// Set range, line, and page sizes
					_vscroll->range(int(_items.size()));
					_vscroll->line(1);
					_vscroll->page(height / _itemHeight);
				}
			}
			else if (isVert)
			{
				// Make sure vertical scroll is at correct position and height
				const int height = isHoriz ? _height2 - Scroll::v_size : _height2;
				_right2 -= Scroll::h_size;
				_width2 -= Scroll::h_size;
				_vscroll->left(_right2 + 1);
				_vscroll->top(_top2);
				_vscroll->length(height + (isHoriz ? 1 : 0));

				// Set range, line, and page sizes
				_vscroll->range(int(_items.size()));
				_vscroll->line(1);
				_vscroll->page(height / _itemHeight);
			}
			if (isHoriz != wasHoriz)
			{
				if (wasHoriz)
				{
					// Get rid of horizontal scroll
					delete _hscroll.release();
				}
				else
				{
					// Remove scroll area from control
					_bottom2 -= Scroll::v_size;
					_height2 -= Scroll::v_size;
					
					// Create horizontal scroll
					_hscroll = std::auto_ptr<Scroll>(new Scroll(_owner.manager(),
							_left + 2, _bottom2 + 1, _width2 + (isVert ? 1 : 0),
							Scroll::t_horizontal));
					
					// Hook up our horiontal scroll callback
					_hscroll->callback(hhit, this);

					// Update scroll range, line, page sizes
					_hscroll->range(calc_hrange());
					_hscroll->line(_charAverage);
					_hscroll->page((_width2 / _charAverage) * _charAverage);
				}
			}
			else if (isHoriz)
			{
				// Make sure horizontal scroll is correct width
				_hscroll->length(_width2 + (isVert ? 1 : 0));

				// Update scroll range, line, page sizes
				_hscroll->range(calc_hrange());
				_hscroll->line(_charAverage);
				_hscroll->page((_width2 / _charAverage) * _charAverage);
			}
			
			// Everything needs to be redrawn
			_owner.dirty(List::all);
		}
	}
}

//=============================================================================
// class List implementation
//=============================================================================

List::List(_Manager& m, int l, int t, int w, int h, int f, const char* title)
		:
		_ControlEx(m, f | m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new ListImpl::Impl3D(*this, l, t, w, h, f, title);
	else
		_impl = new ListImpl::Impl(*this, l, t, w, h, f, title);
}

List::~List()
{
	delete _impl;
}

void List::active_changed(bool o, bool n)
{
	_impl->active_changed(o, n);
}

List::iterator List::begin()
{
	return _impl->begin();
}

List::const_iterator List::begin() const
{
	return _impl->begin();
}

int List::bottom() const
{
	return _impl->bottom();
}

#ifdef	_DEBUG
void List::dump() const
{
	TRACE("gui::List @ $%08X\n", this);
	TRACE(" Flags:");
	const int _flags = flags();
	switch (_flags & t_mask)
	{
	case t_title:
		TRACE(" t_title");
		break;
	
	case t_notitle:
		TRACE(" t_notitle");
		break;
	}
	switch (_flags & s_mask)
	{
	case s_sorted:
		TRACE(" | s_sorted");
		break;
	
	case s_unsorted:
		TRACE(" | s_unsorted");
		break;
	}
	switch (_flags & v_mask)
	{
	case v_scroll:
		TRACE(" | v_scroll");
		break;
	
	case v_noscroll:
		TRACE(" | v_no_scroll");
		break;
	}
	switch (_flags & h_mask)
	{
	case h_scroll:
		TRACE(" | h_scroll\n");
		break;
	
	case h_noscroll:
		TRACE(" | h_no_scroll\n");
		break;
	}
	_impl->dump();
}
#endif

bool List::empty() const
{
	return _impl->empty();
}

List::iterator List::end()
{
	return _impl->end();
}

List::const_iterator List::end() const
{
	return _impl->end();
}

void List::erase(iterator i)
{
	_impl->erase(i);
}

void List::flags_changed(int o, int n)
{
	std::string s;
	if ((o & t_mask) == t_title)
	{
		if (_impl->title())
			s = _impl->title();
	}
	_impl->reset(s.c_str());
}

int List::height() const
{
	return _impl->height();
}

List::iterator List::insert(const char* t, void* d)
{
	return _impl->insert(end(), t, d);
}

List::iterator List::insert(iterator i, const char* t, void* d)
{
	return _impl->insert(i, t, d);
}

int List::left() const
{
	return _impl->left();
}

void List::paint(BITMAP* bmp)
{
	_impl->paint(bmp);
}

bool List::mouse_down(BITMAP* bmp, int x, int y, int m)
{
	return _impl->mouse_down(bmp, x, y, m);
}

int List::right() const
{
	return _impl->right();
}

int List::size() const
{
	return _impl->size();
}

const char* List::title() const
{
	return _impl->title();
}

void List::title(const char* t)
{
	_impl->title(t);
}

int List::top() const
{
	return _impl->top();
}

int List::width() const
{
	return _impl->width();
}
