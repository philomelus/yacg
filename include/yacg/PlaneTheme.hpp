#ifndef	__YACG_PLANETHEME_HPP__
#define	__YACG_PLANETHEME_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_THEME_HPP__
#include <yacg/Theme.hpp>
#endif

namespace yacg
{
//=============================================================================
// class PlaneTheme
//=============================================================================

	class PlaneTheme : public _ThemeItem
	{
	//-------------------------------------------------------------------------
	public:
		PlaneTheme(Theme& t);
		PlaneTheme(const PlaneTheme& r);
		
		PlaneTheme& operator=(const PlaneTheme& r);
		
	//-------------------------------------------------------------------------
	public:
		int color() const;
		
		void color(int f);

		void color(int r, int g, int b);
	
	private:
		int _color;
	
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
		void dump(const std::string& i) const;
#endif
	};
}

#endif
