#ifndef	__YACG_PLANE_HPP__
#define	__YACG_PLANE_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_PLANETHEME_HPP__
#include <yacg/PlaneTheme.hpp>
#endif

namespace yacg
{
//=============================================================================
// class Plane declaration
//=============================================================================

	class Plane : public _Control
	{
	//-------------------------------------------------------------------------
	public:
		Plane(_Manager& m, int c = undefined, int f = auto_delete);
	
	//-------------------------------------------------------------------------
	public:
		int color() const;
		void color(int c);

	private:
		int _color;

	//-------------------------------------------------------------------------
	public:
		Theme& theme() const;

		void theme(PlaneTheme& t);

		void theme(Theme& t);
		
	//-------------------------------------------------------------------------
	public:
		void theme_changed();
	
	//-------------------------------------------------------------------------
	public:
#ifdef	_DEBUG
		void dump(const std::string& i) const;
#endif

	//-------------------------------------------------------------------------
	protected:
		void paint_control(BITMAP* b, int dirty);

	};
}

#endif
