#include "precompiled.hpp"
#include "yacg/Button.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local objects
//=============================================================================

namespace yacg
{
	namespace ButtonImpl
	{
//=============================================================================
// class Base declaration
//=============================================================================

		class Base
		{
		public:
			Base(yacg::Button& o, int x, int y, int w, int h, const char* title);
			virtual ~Base();
			
			int bottom() const;
			
			std::pair<Button::CALLBACK, void*> callback() const;
			void callback(Button::CALLBACK f, void* d);

#ifdef	_DEBUG
			void dump() const;
#endif

			FONT* font() const;
			void font(FONT* f);
			
			int height() const;

			int left() const;

			bool mouse_down(BITMAP* b, int x, int y, int m);

			void paint(BITMAP* b);

			int right() const;
			
			const char* title() const;
			void title(const char* t);

			int top() const;

			int width() const;
		
		public:
			virtual void paint(BITMAP* bmp, bool down) = 0;

		protected:
			virtual void paint_face_down(BITMAP* b) = 0;
			virtual void paint_face_inactive(BITMAP* b) = 0;
			virtual void paint_face_up(BITMAP* b) = 0;

		protected:
			Button& _owner;
			
			Button::CALLBACK _callback;
			void* _data;
			
			int _left;
			int _right;

			int _top;
			int _bottom;

			int _width;
			int _height;

			FONT* _font;
			
			std::string _title;
		};
		
//=============================================================================
// class BitmapBase declaration
//=============================================================================

		class BitmapBase
		{
		public:
			BitmapBase(yacg::BitmapButton& o, int l, int t, BITMAP* up, BITMAP* dn, BITMAP* di);
			BitmapBase(yacg::BitmapButton& o, int l, int t, int w, int h, BITMAP* up, BITMAP* dn, BITMAP* di);
			
			int bottom() const;

			std::pair<BitmapButton::CALLBACK, void*> callback() const;
			void callback(BitmapButton::CALLBACK f, void* d);

			BITMAP* disabled() const;
			void disabled(BITMAP* d);
						
			BITMAP* down() const;
			void down(BITMAP* d);
						
#ifdef	_DEBUG
			void dump() const;
#endif

			int height() const;

			int left() const;			

			bool mouse_down(BITMAP* b, int x, int y, int m);

			void paint(BITMAP* b);

			int right() const;
			
			int top() const;

			BITMAP* up() const;
			void up(BITMAP* d);
			
			int width() const;

		public:		
			virtual void paint(BITMAP* bmp, bool down) = 0;

		protected:
			virtual void paint_face_up(BITMAP* b) = 0;
			virtual void paint_face_down(BITMAP* b) = 0;
			virtual void paint_face_inactive(BITMAP* b) = 0;
			
		protected:
			BitmapButton& _owner;
			
			BitmapButton::CALLBACK _callback;
			void* _data;
			
			int _left;
			int _right;

			int _top;
			int _bottom;

			int _width;
			int _height;

			BITMAP* _disabled;
			BITMAP* _down;
			BITMAP* _up;
		};
		
//=============================================================================
// class Bitmap declaration
//=============================================================================

		class Bitmap : public BitmapBase
		{
		public:
			Bitmap(yacg::BitmapButton& o, int l, int t, BITMAP* up, BITMAP* dn, BITMAP* di);
			Bitmap(yacg::BitmapButton& o, int l, int t, int w, int h, BITMAP* up, BITMAP* dn, BITMAP* di);
			void paint(BITMAP* bmp, bool down);
			void paint_face_up(BITMAP* b);
			void paint_face_down(BITMAP* b);
			void paint_face_inactive(BITMAP* b);
		};
		
//=============================================================================
// class Bitmap3D declaration
//=============================================================================

		class Bitmap3D : public BitmapBase
		{
		public:
			Bitmap3D(yacg::BitmapButton& o, int l, int t, BITMAP* up, BITMAP* dn, BITMAP* di);
			Bitmap3D(yacg::BitmapButton& o, int l, int t, int w, int h, BITMAP* up, BITMAP* dn, BITMAP* di);
			void paint(BITMAP* bmp, bool down);
			void paint_face_up(BITMAP* b);
			void paint_face_down(BITMAP* b);
			void paint_face_inactive(BITMAP* b);
		};
		
//=============================================================================
// class Button declaration
//=============================================================================

		class Button : public Base
		{
		public:
			Button(yacg::Button& o, int x, int y, int w, int h, const char* title);
			void paint(BITMAP* bmp, bool down);
			void paint_face_up(BITMAP* b);
			void paint_face_down(BITMAP* b);
			void paint_face_inactive(BITMAP* b);
		};
		
//=============================================================================
// class Button3D declaration
//=============================================================================

		class Button3D : public Base
		{
		public:
			Button3D(yacg::Button& o, int x, int y, int w, int h, const char* title);
			void paint(BITMAP* bmp, bool down);
			void paint_face_up(BITMAP* b);
			void paint_face_down(BITMAP* b);
			void paint_face_inactive(BITMAP* b);
		};
		
//=============================================================================
// class Base implementation
//=============================================================================

		Base::Base(yacg::Button& o, int x, int y, int w, int h, const char* title)
				:
				_owner(o),
				_callback(0),
				_left(x),
				_right(x + w - 1),
				_top(y),
				_bottom(y + h - 1),
				_width(w),
				_height(h),
				_font(0)
		{
			if (title)
				_title = title;
		}
		
		Base::~Base()
		{
		}
		
		int Base::bottom() const
		{
			return _bottom;
		}
			
		std::pair<yacg::Button::CALLBACK, void*> Base::callback() const
		{
			return std::make_pair(_callback, _data);
		}
		
		void Base::callback(yacg::Button::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}

#ifdef	_DEBUG
		void Base::dump() const
		{
			dump_xywh(_left, _top, _width, _height, _right, _bottom);
			TRACE(" Title: \"%s\"\n", _title.c_str());
			if (_font)
				TRACE(" Font: $%08X\n", _font);
			else
				TRACE(" Font: Allegro Global Font\n");
			TRACE(" Callback: %s\n", _callback ? "Provided" : "Not Specified");
			TRACE(" Data: $%08X\n", _data);
		}
#endif

		int Base::height() const
		{
			return _height;
		}
		
		FONT* Base::font() const
		{
			return _font;
		}
		
		void Base::font(FONT* f)
		{
			if (_font != f)
			{
				_font = f;
				_owner.dirty(_Control::all);
			}
		}
		
		int Base::left() const
		{
			return _left;
		}

		void Base::paint(BITMAP* bmp)
		{
			paint(bmp, false);
		}

		bool Base::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			// If mouse isn't down, nothing to do
			if ((m & 1) == 0)
				return false;
			
			// Is mouse within our boundary?
			if (x >= _left && x <= _right
					&& y >= _top && y <= _bottom)
			{
				// Update button to down position
				paint(bmp, true);
				
				// Wait for button to be released
				int mx = x;
				int my = y;
				bool oldHit = true;
				bool newHit;
				while ((mouse_b & 1) != 0)
				{
					// If mouse changed position
					if (mouse_x != mx || mouse_y != my)
					{
						// Save new position
						mx = mouse_x;
						my = mouse_y;
						
						// Determine if mouse is still in right arrow
						newHit = (mx >= _left && mx <= _right && my >= _top && my <= _bottom);
						
						// If mouse entered or left right arrow, update image
						if (oldHit != newHit)
						{
							paint(bmp, newHit);
							oldHit = newHit;
						}
					}
				}
				
				// If ended inside us, call action
				if (oldHit && _callback)
					_callback(_owner, _data, bmp);
				
				// Note we need painting
				_owner.dirty(_Control::all);
				
				return true;
			}
			
			return false;
		}

		int Base::right() const
		{
			return _right;
		}
		
		const char* Base::title() const
		{
			return _title.c_str();
		}
		
		void Base::title(const char* t)
		{
			_title = t;
			_owner.dirty(_Control::all);
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
// class BitmapBase implementation
//=============================================================================

		BitmapBase::BitmapBase(BitmapButton& o, int l, int t,
				BITMAP* up, BITMAP* dn, BITMAP* di)
				:
				_owner(o),
				_callback(0),
				_left(l),
				_right(-1),
				_top(t),
				_bottom(-1),
				_width(up ? up->w : -1),
				_height(up ? up->h : -1),
				_disabled(di),
				_down(dn),
				_up(up)
		{
			if (_up)
			{
				_right = _left + _width + 3;
				_bottom = _top + _height + 3;
			}
		}
		
		BitmapBase::BitmapBase(BitmapButton& o, int l, int t,
				int w, int h, BITMAP* up, BITMAP* dn, BITMAP* di)
				:
				_owner(o),
				_left(l),
				_right(-1),
				_top(t),
				_bottom(-1),
				_width(w),
				_height(h),
				_disabled(di),
				_down(dn),
				_up(up)
		{
			_right = _left + _width + 3;
			_bottom = _top + _height + 3;
		}
		
		int BitmapBase::bottom() const
		{
			return _bottom;
		}
		
		std::pair<BitmapButton::CALLBACK, void*> BitmapBase::callback() const
		{
			return std::make_pair(_callback, _data);
		}
		
		void BitmapBase::callback(BitmapButton::CALLBACK f, void* d)
		{
			_callback = f;
			_data = d;
		}
		
		BITMAP* BitmapBase::disabled() const
		{
			return _disabled;
		}
		
		void BitmapBase::disabled(BITMAP* d)
		{
			if (_disabled != d)
			{
				_disabled = d;
				_owner.dirty(BitmapButton::all);
			}
		}
					
		BITMAP* BitmapBase::down() const
		{
			return _down;
		}
		
		void BitmapBase::down(BITMAP* d)
		{
			if (_down != d)
			{
				_down = d;
				_owner.dirty(BitmapButton::all);
			}
		}
						
#ifdef	_DEBUG
		void BitmapBase::dump() const
		{
			dump_xywh(_left, _top, _width, _height, _right, _bottom);
			TRACE(" BITMAP buttons NOT supported yet...\n");
			TRACE(" Callback: %s\n", _callback ? "Provided" : "Not Specified");
			TRACE(" Data: $%08X\n", _data);
		}
#endif

		int BitmapBase::height() const
		{
			return _height;
		}

		int BitmapBase::left() const
		{
			return _left;
		}

		void BitmapBase::paint(BITMAP* bmp)
		{
			paint(bmp, false);
		}

		bool BitmapBase::mouse_down(BITMAP* bmp, int x, int y, int m)
		{
			// If mouse isn't down, nothing to do
			if ((m & 1) == 0)
				return false;
			
			// If we aren't set up correctly, nothing to do
			if (_width == -1 || _right == -1)
				return false;
			
			// Is mouse within our boundary?
			if (x >= _left && x <= _right
					&& y >= _top && y <= _bottom)
			{
				// Update button to down position
				paint(bmp, true);
				
				// Wait for button to be released
				int mx = x;
				int my = y;
				bool oldHit = true;
				bool newHit;
				while ((mouse_b & 1) != 0)
				{
					// If mouse changed position
					if (mouse_x != mx || mouse_y != my)
					{
						// Save new position
						mx = mouse_x;
						my = mouse_y;
						
						// Determine if mouse is still in right arrow
						newHit = (mx >= _left && mx <= _right && my >= _top && my <= _bottom);
						
						// If mouse entered or left right arrow, update image
						if (oldHit != newHit)
						{
							paint(bmp, newHit);
							oldHit = newHit;
						}
					}
				}
				
				// If ended inside us, call action
				if (oldHit && _callback)
					_callback(_owner, _data, bmp);
					
				return true;
			}
			
			return false;
		}

		int BitmapBase::right() const
		{
			return _right;
		}
		
		int BitmapBase::top() const
		{
			return _top;
		}
		
		BITMAP* BitmapBase::up() const
		{
			return _up;
		}
		
		void BitmapBase::up(BITMAP* d)
		{
			if (_up != d)
			{
				_up = d;
				_owner.dirty(BitmapButton::all);
				if (_up)
				{
					_width = _up->w;
					_height = _up->h;
					_right = _left + _width + 3;
					_bottom = _top + _height + 3;
				}
				else
				{
					_width = -1;
					_height = -1;
					_right = -1;
					_bottom = -1;
				}
			}
		}
		
		int BitmapBase::width() const
		{
			return _width;
		}

//=============================================================================
// class Bitmap declaration
//=============================================================================

		Bitmap::Bitmap(yacg::BitmapButton& o, int l, int t, BITMAP* up, BITMAP* dn, BITMAP* di)
				:
				BitmapBase(o, l, t, up, dn, di)
		{
		}
		
		Bitmap::Bitmap(yacg::BitmapButton& o, int l, int t, int w, int h, BITMAP* up, BITMAP* dn, BITMAP* di)
				:
				BitmapBase(o, l, t, w, h, up, dn, di)
		{
		}
		
		void Bitmap::paint(BITMAP* bmp, bool down)
		{
_asm int 3;
#if 0
			{
				Update u(bmp);
				// Figure out color of edges
				int tlc;
				int itlc;
				int ibrc;
				int brc;
				if (down)
				{
					tlc = Theme.GRAY_VERY_DARK;
					itlc = Theme.GRAY_DARK;
					ibrc = Theme.GRAY_LIGHT;
					brc = Theme.WHITE;
				}
				else
				{
					tlc = Theme.WHITE;
					itlc = Theme.GRAY_LIGHT;
					ibrc = Theme.GRAY_DARK;
					brc = Theme.GRAY_VERY_DARK;
				}

				// Top/left
				hline(bmp, _left, _top, _right, tlc);
				vline(bmp, _left, _top + 1, _bottom, tlc);

				// Inset top/left
				hline(bmp, _left + 1, _top + 1, _right - 1, itlc);
				vline(bmp, _left + 1, _top + 2, _bottom - 1, itlc);

				// Inset bottom/right
				hline(bmp, _left + 2, _bottom - 1, _right - 1, ibrc);
				vline(bmp, _right - 1, _top + 2, _bottom - 2, ibrc);

				// Bottom/right
				hline(bmp, _left + 1, _bottom, _right, brc);
				vline(bmp, _right, _top + 1, _bottom - 1, brc);

				// Inside
				rectfill(bmp, _left + 2, _top + 2, _right - 2, _bottom - 2, Theme.GRAY);
			}
			
			// Paint appropriate interior
			if (down)
				paint_face_down(bmp);
			else
			{
				if (_owner.active())
					paint_face_up(bmp);
				else
					paint_face_inactive(bmp);
			}
#endif
		}
		
		void Bitmap::paint_face_up(BITMAP* bmp)
		{
#if 0
			// Nothing to do if no bitmap
			if (_up == 0)
				return;
			
			// Blit bitmap
			Update u(bmp);
			blit(_up, bmp, 0, 0, _left + 2, _top + 2, _up->w, _up->h);
#endif
		}
		
		void Bitmap::paint_face_down(BITMAP* bmp)
		{
#if 0
			if (_down != 0)
			{
				// Blit bitmap to users bitmap
				Update u(bmp);
				blit(_down, bmp, 0, 0, _left + 2, _top + 2, _down->w, _down->h);
			}
			else if (_up)
			{
				// Use the up bitmap, offsetting by +1 towards right/bottom
				Update u(bmp);
				blit(_up, bmp, 0, 0, _left + 3, _top + 3, _up->w - 1, _up->h - 1);
				const int color = getpixel(_up, 1, 1);
				hline(bmp, _left + 2, _top + 2, _right - 2, color);
				vline(bmp, _left + 2, _top + 3, _bottom - 2, color);
			}
#endif
		}
		
		void Bitmap::paint_face_inactive(BITMAP* bmp)
		{
#if 0
			if (_disabled)
			{
				// Blit bitmap to users bitmap
				Update u(bmp);
				blit(_disabled, bmp, 0, 0, _left + 2, _top + 2, _disabled->w, _disabled->h);
			}
			else if (_up)
			{
				// Use the up bitmap, replacing blakc with dark gray
				BITMAP* tmp = create_bitmap(_up->w, _up->h);
				blit(_up, tmp, 0, 0, 0, 0, _up->w, _up->h);
				
				// Dim all pixels by 48 or smallest
				for (int x = 0; x < tmp->w; ++x)
				{
					for (int y = 0; y < tmp->h; ++y)
					{
						int p = getpixel(tmp, x, y);
						if (p != Theme.GRAY)
						{
							int r = getr(p);
							int g = getg(p);
							int b = getb(p);
							if (r > 47 && g > 47 && b > 47)
							{
								r -= 48;
								g -= 48;
								b -= 48;
							}
							else
							{
								int min = r;
								if (g < min)
									min = g;
								if (b < min)
									min = b;
								r -= min;
								g -= min;
								b -= min;
							}
							putpixel(tmp, x, y, makecol(r, g, b));
						}
					}
				}
				{
					Update u(bmp);
					blit(tmp, bmp, 0, 0, _left + 2, _top + 2, tmp->w, tmp->h);
				}
				destroy_bitmap(tmp);
			}
#endif
		}

//=============================================================================
// class Bitmap3D declaration
//=============================================================================

		Bitmap3D::Bitmap3D(yacg::BitmapButton& o, int l, int t, BITMAP* up, BITMAP* dn, BITMAP* di)
				:
				BitmapBase(o, l, t, up, dn, di)
		{
		}
		
		Bitmap3D::Bitmap3D(yacg::BitmapButton& o, int l, int t, int w, int h, BITMAP* up, BITMAP* dn, BITMAP* di)
				:
				BitmapBase(o, l, t, up, dn, di)
		{
		}

		void Bitmap3D::paint(BITMAP* bmp, bool down)
		{
// TODO:  The color's used here are ALL WRONG, as well as the DRAWING
			{
				Update u(bmp);
				// Figure out color of edges
				int tlc;
				int itlc;
				int ibrc;
				int brc;
				if (down)
				{
					tlc = Theme.GRAY_VERY_DARK;
					itlc = Theme.GRAY_DARK;
					ibrc = Theme.GRAY_LIGHT;
					brc = Theme.WHITE;
				}
				else
				{
					tlc = Theme.WHITE;
					itlc = Theme.GRAY_LIGHT;
					ibrc = Theme.GRAY_DARK;
					brc = Theme.GRAY_VERY_DARK;
				}

				// Top/left
				hline(bmp, _left, _top, _right, tlc);
				vline(bmp, _left, _top + 1, _bottom, tlc);

				// Inset top/left
				hline(bmp, _left + 1, _top + 1, _right - 1, itlc);
				vline(bmp, _left + 1, _top + 2, _bottom - 1, itlc);

				// Inset bottom/right
				hline(bmp, _left + 2, _bottom - 1, _right - 1, ibrc);
				vline(bmp, _right - 1, _top + 2, _bottom - 2, ibrc);

				// Bottom/right
				hline(bmp, _left + 1, _bottom, _right, brc);
				vline(bmp, _right, _top + 1, _bottom - 1, brc);

				// Inside
				rectfill(bmp, _left + 2, _top + 2, _right - 2, _bottom - 2, Theme.GRAY);
			}
			
			// Paint appropriate interior
			if (down)
				paint_face_down(bmp);
			else
			{
				if (_owner.active())
					paint_face_up(bmp);
				else
					paint_face_inactive(bmp);
			}
		}
		
		void Bitmap3D::paint_face_up(BITMAP* bmp)
		{
			// Nothing to do if no bitmap
			if (_up == 0)
				return;
			
			// Blit bitmap
			Update u(bmp);
			blit(_up, bmp, 0, 0, _left + 2, _top + 2, _up->w, _up->h);
		}
		
		void Bitmap3D::paint_face_down(BITMAP* bmp)
		{
			if (_down != 0)
			{
				// Blit bitmap to users bitmap
				Update u(bmp);
				blit(_down, bmp, 0, 0, _left + 2, _top + 2, _down->w, _down->h);
			}
			else if (_up)
			{
				// Use the up bitmap, offsetting by +1 towards right/bottom
				Update u(bmp);
				blit(_up, bmp, 0, 0, _left + 3, _top + 3, _up->w - 1, _up->h - 1);
				const int color = getpixel(_up, 1, 1);
				hline(bmp, _left + 2, _top + 2, _right - 2, color);
				vline(bmp, _left + 2, _top + 3, _bottom - 2, color);
			}
		}
		
		void Bitmap3D::paint_face_inactive(BITMAP* bmp)
		{
			if (_disabled)
			{
				// Blit bitmap to users bitmap
				Update u(bmp);
				blit(_disabled, bmp, 0, 0, _left + 2, _top + 2, _disabled->w, _disabled->h);
			}
			else if (_up)
			{
				// Use the up bitmap, replacing blakc with dark gray
				BITMAP* tmp = create_bitmap(_up->w, _up->h);
				blit(_up, tmp, 0, 0, 0, 0, _up->w, _up->h);
				
				// Dim all pixels by 48 or smallest
				for (int x = 0; x < tmp->w; ++x)
				{
					for (int y = 0; y < tmp->h; ++y)
					{
						int p = getpixel(tmp, x, y);
						if (p != Theme.GRAY)
						{
							int r = getr(p);
							int g = getg(p);
							int b = getb(p);
							if (r > 47 && g > 47 && b > 47)
							{
								r -= 48;
								g -= 48;
								b -= 48;
							}
							else
							{
								int min = r;
								if (g < min)
									min = g;
								if (b < min)
									min = b;
								r -= min;
								g -= min;
								b -= min;
							}
							putpixel(tmp, x, y, makecol(r, g, b));
						}
					}
				}
				{
					Update u(bmp);
					blit(tmp, bmp, 0, 0, _left + 2, _top + 2, tmp->w, tmp->h);
				}
				destroy_bitmap(tmp);
			}
		}

//=============================================================================
// class Button declaration
//=============================================================================

		Button::Button(yacg::Button& o, int x, int y, int w, int h, const char* title)
				:
				Base(o, x, y, w, h, title)
		{
		}
		
		void Button::paint(BITMAP* bmp, bool down)
		{
			{
				Update u(bmp);
				
				// Figure out color of edges
				rect(bmp, _left, _top, _right, _bottom, Theme.flat.Frame);
				rectfill(bmp, _left + 1, _top + 1, _right - 1, _bottom - 1, Theme.flat.Background);
			}
			
			// Paint appropriate interior
			if (down)
				paint_face_down(bmp);
			else
			{
				if (_owner.active())
					paint_face_up(bmp);
				else
					paint_face_inactive(bmp);
			}
		}
		
		void Button::paint_face_down(BITMAP* bmp)
		{
			const int color = color_plus_delta(Theme.flat.Foreground, 64);
			Update u(bmp);
			rect(bmp, _left + 2, _top + 2, _right - 2, _bottom - 2, color);
			if (!_title.empty())
			{
				FONT* f = _font == 0 ? ::font : _font;
				solid_mode();
				text_mode(-1);
				Clipper c(bmp, _left + 2, _top + 2, _right -2, _bottom - 2);
				textout_centre(bmp, f, _title.c_str(), _left + (_width / 2),
						_top + ((_height - text_height(f)) / 2), color);
			}
		}
		
		void Button::paint_face_inactive(BITMAP* bmp)
		{
			if (!_title.empty())
			{
				FONT* f = _font == 0 ? ::font : _font;
				solid_mode();
				text_mode(-1);
				Clipper c(bmp, _left + 2, _top + 2, _right -2, _bottom - 2);
				Update u(bmp);
				textout_centre(bmp, f, _title.c_str(), _left + (_width / 2), _top + ((_height
						- text_height(f)) / 2), color_minus_delta(Theme.flat.Foreground, 48));
			}
		}

		void Button::paint_face_up(BITMAP* bmp)
		{
			Update u(bmp);
			rect(bmp, _left + 2, _top + 2, _right - 2, _bottom - 2, Theme.flat.Background);
			if (!_title.empty())
			{
				FONT* f = _font == 0 ? ::font : _font;
				solid_mode();
				text_mode(-1);
				Clipper c(bmp, _left + 2, _top + 2, _right -2, _bottom - 2);
				textout_centre(bmp, f, _title.c_str(), _left + (_width / 2), _top + ((_height
						- text_height(f)) / 2), Theme.flat.Foreground);
			}
		}

//=============================================================================
// class Button3D declaration
//=============================================================================

		Button3D::Button3D(yacg::Button& o, int x, int y, int w, int h, const char* title)
				:
				Base(o, x, y, w, h, title)
		{
		}

		void Button3D::paint(BITMAP* bmp, bool down)
		{
			{
				Update u(bmp);
				
				// Figure out color of edges
				int tlc;
				int itlc;
				int ibrc;
				int brc;
				if (down)
				{
					tlc = Theme.GRAY_VERY_DARK;
					itlc = Theme.GRAY_DARK;
					ibrc = Theme.GRAY_LIGHT;
					brc = Theme.WHITE;
				}
				else
				{
					tlc = Theme.WHITE;
					itlc = Theme.GRAY_LIGHT;
					ibrc = Theme.GRAY_DARK;
					brc = Theme.GRAY_VERY_DARK;
				}

				// Top/left
				hline(bmp, _left, _top, _right, tlc);
				vline(bmp, _left, _top + 1, _bottom, tlc);

				// Inset top/left
				hline(bmp, _left + 1, _top + 1, _right - 1, itlc);
				vline(bmp, _left + 1, _top + 2, _bottom - 1, itlc);

				// Inset bottom/right
				hline(bmp, _left + 2, _bottom - 1, _right - 1, ibrc);
				vline(bmp, _right - 1, _top + 2, _bottom - 2, ibrc);

				// Bottom/right
				hline(bmp, _left + 1, _bottom, _right, brc);
				vline(bmp, _right, _top + 1, _bottom - 1, brc);

				// Inside
				rectfill(bmp, _left + 2, _top + 2, _right - 2, _bottom - 2, Theme.GRAY);
			}
			
			// Paint appropriate interior
			if (down)
				paint_face_down(bmp);
			else
			{
				if (_owner.active())
					paint_face_up(bmp);
				else
					paint_face_inactive(bmp);
			}
		}
		
		void Button3D::paint_face_down(BITMAP* bmp)
		{
			if (!_title.empty())
			{
				FONT* f = _font == 0 ? ::font : _font;
				solid_mode();
				text_mode(-1);
				Clipper c(bmp, _left + 2, _top + 2, _right -2, _bottom - 2);
				Update u(bmp);
				textout_centre(bmp, f, _title.c_str(), _left + (_width / 2) + 1,
						_top + ((_height - text_height(f)) / 2) + 1, Theme.BLACK);
			}
		}
		
		void Button3D::paint_face_inactive(BITMAP* bmp)
		{
			if (!_title.empty())
			{
				FONT* f = _font == 0 ? ::font : _font;
				solid_mode();
				text_mode(-1);
				Clipper c(bmp, _left + 2, _top + 2, _right -2, _bottom - 2);
				Update u(bmp);
				textout_centre(bmp, f, _title.c_str(), _left + (_width / 2), _top + ((_height
						- text_height(f)) / 2), Theme.GRAY_DARK);
			}
		}

		void Button3D::paint_face_up(BITMAP* bmp)
		{
			if (!_title.empty())
			{
				FONT* f = _font == 0 ? ::font : _font;
				solid_mode();
				text_mode(-1);
				Clipper c(bmp, _left + 2, _top + 2, _right -2, _bottom - 2);
				Update u(bmp);
				textout_centre(bmp, f, _title.c_str(), _left + (_width / 2), _top + ((_height
						- text_height(f)) / 2), Theme.BLACK);
			}
		}
	}
}

//=============================================================================
// class BitmapButton implementation
//=============================================================================

BitmapButton::BitmapButton(_Manager& m, int x, int y, BITMAP* up, BITMAP* down, BITMAP* disabled)
		:
		_ControlEx(m, m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new ButtonImpl::Bitmap3D(*this, x, y, up, down, disabled);
	else
		_impl = new ButtonImpl::Bitmap(*this, x, y, up, down, disabled);
}

BitmapButton::BitmapButton(_Manager& m, int x, int y, int w, int h, BITMAP* up, BITMAP* down, BITMAP* disabled)
		:
		_ControlEx(m, m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new ButtonImpl::Bitmap3D(*this, x, y, w, h, up, down, disabled);
	else
		_impl = new ButtonImpl::Bitmap(*this, x, y, w, h, up, down, disabled);
}

BitmapButton::~BitmapButton()
{
	delete _impl;
}

std::pair<BitmapButton::CALLBACK, void*> BitmapButton::callback() const
{
	return _impl->callback();
}

void BitmapButton::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

void BitmapButton::active_changed(bool o, bool n)
{
	dirty(all);
}

int BitmapButton::bottom() const
{
	return _impl->bottom();
}

BITMAP* BitmapButton::disabled() const
{
	return _impl->disabled();
}

void BitmapButton::disabled(BITMAP* d)
{
	return _impl->disabled(d);
}
			
BITMAP* BitmapButton::down() const
{
	return _impl->down();
}

void BitmapButton::down(BITMAP* d)
{
	_impl->down(d);
}
						
#ifdef	_DEBUG
void BitmapButton::dump() const
{
	TRACE("yacg::BitmapButton @ $%08X\n", this);
	_impl->dump();
}
#endif

int BitmapButton::height() const
{
	return _impl->height();
}

int BitmapButton::left() const
{
	return _impl->left();
}
		
void BitmapButton::paint(BITMAP* bmp)
{
	return _impl->paint(bmp, false);
}

bool BitmapButton::mouse_down(BITMAP* b, int x, int y, int m)
{
	return _impl->mouse_down(b, x, y, m);
}

int BitmapButton::right() const
{
	return _impl->right();
}

int BitmapButton::top() const
{
	return _impl->top();
}

BITMAP* BitmapButton::up() const
{
	return _impl->up();
}

void BitmapButton::up(BITMAP* u)
{
	_impl->up(u);
}

int BitmapButton::width() const
{
	return _impl->width();
}

//=============================================================================
// class Button implementation
//=============================================================================

Button::Button(_Manager& m, int x, int y, int w, int h, const char* title)
		:
		_ControlEx(m, m_down)
{
	if (Theme.style == Configuration::STYLE_3D)
		_impl = new ButtonImpl::Button3D(*this, x, y, w, h, title);
	else
		_impl = new ButtonImpl::Button(*this, x, y, w, h, title);
}

Button::~Button()
{
	delete _impl;
}

std::pair<Button::CALLBACK, void*> Button::callback() const
{
	return _impl->callback();
}

void Button::callback(CALLBACK f, void* d)
{
	_impl->callback(f, d);
}

void Button::active_changed(bool o, bool n)
{
	dirty(all);
}

int Button::bottom() const
{
	return _impl->bottom();
}

#ifdef	_DEBUG
void Button::dump() const
{
	TRACE("yacg::Button @ $%08X\n", this);
	_impl->dump();
}
#endif

FONT* Button::font() const
{
	return _impl->font();
}

void Button::font(FONT* f)
{
	_impl->font(f);
}

int Button::height() const
{
	return _impl->height();
}

int Button::left() const
{
	return _impl->left();
}
		
void Button::paint(BITMAP* bmp)
{
	return _impl->paint(bmp, false);
}

bool Button::mouse_down(BITMAP* b, int x, int y, int m)
{
	return _impl->mouse_down(b, x, y, m);
}

int Button::right() const
{
	return _impl->right();
}

const char* Button::title() const
{
	return _impl->title();
}

void Button::title(const char* t)
{
	_impl->title(t);
}

int Button::top() const
{
	return _impl->top();
}

int Button::width() const
{
	return _impl->width();
}
