#ifndef	__YACG_BOXTHEME_HPP__
#define	__YACG_BOXTHEME_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_THEME_HPP__
#include <yacg/Theme.hpp>
#endif

namespace yacg
{
//=============================================================================
// class _BoxTheme
//=============================================================================

	class _BoxTheme : public _ThemeItem
	{
	//-------------------------------------------------------------------------
	protected:
		_BoxTheme(Theme& t = *ActiveTheme);

		_BoxTheme(const _BoxTheme& o);
		
		_BoxTheme& operator=(const _BoxTheme& o);
	
	//-------------------------------------------------------------------------
	public:
		FONT* font() const;
		
		void font(FONT* f);
	
	private:
		FONT* _font;
	
	//-------------------------------------------------------------------------
	public:
#ifdef	_DEBUG
		void dump() const;
#endif
	};
	
//=============================================================================
// class BoxTheme3D
//=============================================================================

	class BoxTheme3D : public _BoxTheme
	{
	//-------------------------------------------------------------------------
	public:
		BoxTheme3D(Theme& t = *ActiveTheme);

		BoxTheme3D(const BoxTheme3D& o);
		
		BoxTheme3D& operator=(const BoxTheme3D& o);
			
	//-------------------------------------------------------------------------
	public:
		int frame_lightest() const;

		void frame_lightest(int c);

		void frame_lightest(int r, int g, int b);

	private:
		int _frameLightest;
	
	//-------------------------------------------------------------------------
	public:
		int frame_light() const;

		void frame_light(int c);

		void frame_light(int r, int g, int b);
		
	private:
		int _frameLight;

	//-------------------------------------------------------------------------
	public:
		int frame_dark() const;

		void frame_dark(int c);

		void frame_dark(int r, int g, int b);
		
	private:
		int _frameDark;

	//-------------------------------------------------------------------------
	public:
		int frame_darkest() const;

		void frame_darkest(int c);

		void frame_darkest(int r, int g, int b);
			
	private:
		int _frameDarkest;

	//-------------------------------------------------------------------------
	public:
		int inactive_text_background() const;

		void inactive_text_background(int f);

		void inactive_text_background(int r, int g, int b);

	private:		
		int _inactiveTextBackground;
	
	//-------------------------------------------------------------------------
	public:
		int inactive_text_foreground() const;

		void inactive_text_foreground(int f);

		void inactive_text_foreground(int r, int g, int b);

	private:		
		int _inactiveTextForeground;

	//-------------------------------------------------------------------------
	public:
		int interior() const;

		void interior(int i);

		void interior(int r, int g, int b);

	private:		
		int _interior;

	//-------------------------------------------------------------------------
	public:
		int text_background() const;

		void text_background(int f);

		void text_background(int r, int g, int b);

	private:		
		int _textBackground;
		
	//-------------------------------------------------------------------------
	public:
		int text_foreground() const;

		void text_foreground(int f);

		void text_foreground(int r, int g, int b);

	private:
		int _textForeground;

	//-------------------------------------------------------------------------
	public:
		void reset();
		
		void reset8();

		void reset15();

		void reset16();

		void reset24();

		void reset32();

	//-------------------------------------------------------------------------
	public:
#ifdef	_DEBUG
		void dump() const;
#endif
	};
	
//=============================================================================
// class BoxThemeFlat
//=============================================================================

	class BoxThemeFlat : public _BoxTheme
	{
	//-------------------------------------------------------------------------
	public:
		BoxThemeFlat(Theme& t = *ActiveTheme);

		BoxThemeFlat(const BoxThemeFlat& o);
		
		BoxThemeFlat& operator=(const BoxThemeFlat& o);
			
	//-------------------------------------------------------------------------
	public:
		int frame() const;
		
		void frame(int f);
		
		void frame(int r, int g, int b);
	
	private:
		int _frame;
	
	//-------------------------------------------------------------------------
	public:
		int inactive_text_background() const;

		void inactive_text_background(int f);

		void inactive_text_background(int r, int g, int b);

	private:		
		int _inactiveTextBackground;
	
	//-------------------------------------------------------------------------
	public:
		int inactive_text_foreground() const;

		void inactive_text_foreground(int f);

		void inactive_text_foreground(int r, int g, int b);

	private:		
		int _inactiveTextForeground;

	//-------------------------------------------------------------------------
	public:
		int interior() const;

		void interior(int i);

		void interior(int r, int g, int b);

	private:		
		int _interior;

	//-------------------------------------------------------------------------
	public:
		int text_background() const;

		void text_background(int f);

		void text_background(int r, int g, int b);

	private:		
		int _textBackground;
		
	//-------------------------------------------------------------------------
	public:
		int text_foreground() const;

		void text_foreground(int f);

		void text_foreground(int r, int g, int b);

	private:
		int _textForeground;

	//-------------------------------------------------------------------------
	public:
		void reset();
		
		void reset8();

		void reset15();

		void reset16();

		void reset24();

		void reset32();

	//-------------------------------------------------------------------------
#ifdef	_DEBUG
		void dump() const;
#endif
	};
}

#endif
