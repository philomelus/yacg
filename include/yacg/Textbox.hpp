#ifndef	__YACG_TEXTBOX_HPP__
#define	__YACG_TEXTBOX_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
//=============================================================================
// class Textbox declaration
//=============================================================================

	namespace TextboxImpl
	{
		class Base;
	}
	
	class Textbox : public _ControlEx
	{
	public:
		// Horizonatl alignment
		static const int h_center = 0x00 << 16;
		static const int h_left = 0x01 << 16;
		static const int h_right = 0x02 << 16;
		static const int h_mask = 0x03 << 16;
		
		// Vertical alignment
		static const int v_center = 0x00;
		static const int v_top = 0x01 << 18;
		static const int v_bottom = 0x02 << 18;
		static const int v_mask = 0x03 << 18;
		
		// Frame type
		static const int f_none = 0x00;
		static const int f_out = 0x01 << 20;
		static const int f_in = 0x02 << 20;
		static const int f_mask = 0x03 << 20;

		// Scroll's
		static const int s_none = 0x00 << 22;
		static const int s_horizontal = 0x01 << 22;
		static const int s_vertical = 0x02 << 22;
		static const int s_both = 0x03 << 22;
		static const int s_mask = 0x03 << 22;

	public:	// Common - client usage expected
		Textbox(_Manager& m, int x, int y, int w, int h, int f, const char* t = 0);
		~Textbox();

		const char* operator[](int i) const;

		int background() const;
		void background(int c);
		
		int bottom() const;
		
		int count() const;

		FONT* font() const;
		void font(FONT* f);

		int foreground() const;
		void foreground(int c);
		
		int height() const;
		
		int left() const;

		std::pair<int, int> margin_h() const;
		void margin_h(int l, int r);
				
		std::pair<int, int> margin_v() const;
		void margin_v(int t, int b);
				
		int right() const;

		void text(const char* t);

		int top() const;
		
		int width() const;

#ifdef	_DEBUG
		void dump() const;
#endif

	public:	// Uncommon - client usage rare, or only for derived
		void paint(BITMAP* b);
		bool mouse_down(BITMAP* b, int x, int y, int m);
	
	private:		
		TextboxImpl::Base* _impl;
	};
}

//=============================================================================
// class Textbox macros
//=============================================================================

#define	TEXTBOX(x, y, w, h, f)				insert(*new yacg::Textbox(*this, (x), (y), (w), (h), (f)));
#define	TEXTBOX_EX(x, y, w, h, f, t)		insert(*new yacg::Textbox(*this, (x), (y), (w), (h), (f), (t)));

#define	TEXTBOX_V(v, x, y, w, h, f)			v##_ = new yacg::Textbox(*this, (x), (y), (w), (h), (f)); insert(*v##_);
#define	TEXTBOX_V_EX(v, x, y, w, h, f, t)	v##_ = new yacg::Textbox(*this, (x), (y), (w), (h), (f), (t)); insert(*v##_);

#endif
