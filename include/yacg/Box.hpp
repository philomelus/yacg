#ifndef	__YACG_BOX_HPP__
#define	__YACG_BOX_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_BOXTHEME_HPP__
#include <yacg/BoxTheme.hpp>
#endif

namespace yacg
{
//=============================================================================
// Forward declarations
//=============================================================================

	namespace BoxImpl
	{
		class Base;
	}

//=============================================================================
// class Box declaration
//=============================================================================

	class Box : public _Control
	{
	public:
	//-------------------------------------------------------------------------
	// frame
	
		static const int f_flat = 0x00;				// default
		static const int f_in = 0x01 << 16;
		static const int f_out = 0x02 << 16;
		static const int f_mask = 0x03 << 16;
		
	//-------------------------------------------------------------------------
	// title
	
		static const int t_notitle = 0x00 << 16;	// default
		static const int t_title = 0x04 << 16;
		static const int t_mask = 0x04 << 16;

	//-------------------------------------------------------------------------
	// title vertical alignment
	
		static const int v_top = 0x00 << 16;		// default
		static const int v_bottom = 0x08 << 16;
		static const int v_centered = 0x10 << 16;
		static const int v_mask = 0x18 << 16;
		
	//-------------------------------------------------------------------------
	// title horizontal alignment
	
		static const int h_left = 0x00 << 16;		// default
		static const int h_right = 0x20 << 16;
		static const int h_centered = 0x40 << 16;
		static const int h_mask = 0x60 << 16;

	//-------------------------------------------------------------------------
	// title cell vertical alignment
	
		static const int o_titlecentered = 0x000 << 16;	// default
		static const int o_titleabove = 0x080 << 16;
		static const int o_titlebelow = 0x100 << 16;
		static const int o_mask = 0x180 << 16;

	//-------------------------------------------------------------------------
	// title cell horizontal alignment
	
		static const int i_unfilled = 0x000 << 16;	// default
		static const int i_filled = 0x200 << 16;
		static const int i_mask = 0x200 << 16;

	//-------------------------------------------------------------------------
	public:
		Box(_Manager& m, int x, int y, int w, int h, int f = f_flat | t_notitle
				| v_top | h_left | o_titlecentered| i_unfilled | auto_delete,
				const char* t = 0);
		~Box();

	//-------------------------------------------------------------------------
		int bottom() const;
		
	//-------------------------------------------------------------------------
#ifdef	_DEBUG
		void dump(const std::string& i) const;
#endif

	//-------------------------------------------------------------------------
		FONT* font() const;
		void font(FONT* f);
		
	//-------------------------------------------------------------------------
		int height() const;
		void height(int h);
		
	//-------------------------------------------------------------------------
		int left() const;
		void left(int l);
		
	//-------------------------------------------------------------------------
		int right() const;
		
	//-------------------------------------------------------------------------
		Theme& theme() const;
		void theme(Theme& t);
		void theme(_BoxTheme& t);

	//-------------------------------------------------------------------------
		void theme_changed();
		
	//-------------------------------------------------------------------------
		const char* title() const;
		void title(const char* t);
		
		const std::string& title_string() const;
		
	//-------------------------------------------------------------------------
		int top() const;
		void top(int t);
		
	//-------------------------------------------------------------------------
		int width() const;
		void width(int w);

	//-------------------------------------------------------------------------
	protected:
		void paint_control(BITMAP* bmp, int dirty);

	//-------------------------------------------------------------------------
	private:
		BoxImpl::Base* _impl;
	};
}

#endif
