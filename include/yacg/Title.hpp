#ifndef	__YACG_TITLE_HPP__
#define	__YACG_TITLE_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
//=============================================================================
// class Title declaration
//=============================================================================

	namespace TitleImpl
	{
		class Base;
	}
	
	class Title : public _Control
	{
	public:
		// Horizonatl alignment
		static const int h_center	= 0x00 << 16;		// default
		static const int h_left		= 0x01 << 16;
		static const int h_right	= 0x02 << 16;
		static const int h_mask		= 0x03 << 16;
		
		// Vertical alignment
		static const int v_center	= 0x00 << 16;		// default
		static const int v_top		= 0x04 << 16;
		static const int v_bottom	= 0x08 << 16;
		static const int v_mask		= 0x0C << 16;
		
		// Frame type
		static const int f_none		= 0x00 << 16;		// default
		static const int f_out		= 0x10 << 16;
		static const int f_in		= 0x20 << 16;
		static const int f_flat		= 0x30 << 16;
		static const int f_mask		= 0x30 << 16;
	
		// Options
		// TODO:  These should be seperate option catagories!
		static const int o_resize_h = 0x00 << 22;			// default
		static const int o_no_resize_h = 0x01 << 22;		// With _Control::calc for w, don't resize after construction
		static const int o_resize_v = 0x00 << 22;			// default
		static const int o_no_resize_v = 0x02 << 22;		// With _Control::calc for v, don't resize after construction
		static const int o_mask = 0x03 << 22;
		
	public:	// Common - client usage expected
		Title(_Manager& m, int x, int y, int w, int h, int flags, const char* t = 0);
		~Title();
		
		int left() const;
		void left(int l);
		
		int right() const;
		void right(int r);
		
		int top() const;
		void top(int t);
		
		int bottom() const;
		void bottom(int b);
		
		int width() const;
		void width(int w);
		
		int height() const;
		void height(int h);

		FONT* font() const;
		void font(FONT *pFont);

		const char* title() const;
		void title(const char* t);

		const std::string& title_object() const;
		
	public:	// Uncommon - client usage rare, or only for derived
		void paint(BITMAP* b);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:		
		TitleImpl::Base* _impl;
	};
}

//=============================================================================
// class Title macros
//=============================================================================

#define	TITLE(x, y, w, h, f)				insert(*new yacg::Title(*this, (x), (y), (w), (h), (f)));
#define	TITLE_EX(x, y, w, h, f, t)			insert(*new yacg::Title(*this, (x), (y), (w), (h), (f), (t)));

#define	TITLE_V(v, x, y, w, h, f)			v##_ = new yacg::Title(*this, (x), (y), (w), (h), (f)); insert(*v##_);
#define	TITLE_V_EX(v, x, y, w, h, f, t)		v##_ = new yacg::Title(*this, (x), (y), (w), (h), (f), (t)); insert(*v##_);

#endif
