#include "precompiled.hpp"
#include <yacg/Plane.hpp>
#include <yacg/PlaneTheme.hpp>
#include "ThemeImpl.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// class Plane implementation
//=============================================================================

Plane::Plane(_Manager& m, int c, int f)
		:
		_Control(m, f),
		_color(c)
{
	// Disable dirty hints since they mena the same thing for us
	_activeChangedHint = 0;

	// Initialize color if needed
	if (_color == undefined)
	{
		// Initialize color based on current theme.  Since we do the same action
		// regardless of style, we don;t really pay attention to the styles, and
		// just let the active style's color work for us.
		Theme& t = theme();
		Theme::iterator i = t.find(Theme::TYPE_PLANE);
		if (i != t.end())
		{
			PlaneTheme& ti = static_cast<PlaneTheme&>(*i);
			_color = ti.color();
		}
		else
		{
			// Locate flat type
			i = t.find(Theme::TYPE_DEFAULT);
			ASSERT(i != t.end());
			switch (t.style())
			{
			case Theme::STYLE_3D:
				{
					DefaultTheme3D& ti = static_cast<DefaultTheme3D&>(*i);
					_color = ti.interior();
				}
				break;
				
			case Theme::STYLE_FLAT:
				{
					DefaultThemeFlat& ti = static_cast<DefaultThemeFlat&>(*i);
					_color = ti.interior();
				}
				break;
	#ifdef	_DEBUG
			default:
				TRACE("yacg::Plane:  Style %d not implemented\n", theme().style());
				break;
	#endif
			}
		}
	}
}

int Plane::color() const
{
	return _color;
}

void Plane::color(int c)
{
	if (_color != c)
	{
		_color = c;
		dirty(dirty_all);
	}
}

#ifdef	_DEBUG
void Plane::dump(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";
	
	dump_object(i, "yacg::Plane", this);
	dump_color(sublevel, "Color", _color);
	
	_Control::dump_control(level);
}
#endif

void Plane::paint_control(BITMAP* bmp, int dirty)
{
	clear_to_color(bmp, _color);
}

Theme& Plane::theme() const
{
	return _Control::theme();
}

void Plane::theme(PlaneTheme& t)
{
	_color = t.color();
	_Control::theme_changed();
}

void Plane::theme(Theme& t)
{
	_Control::theme(t);
}

void Plane::theme_changed()
{
	// Call base
	_Control::theme_changed();

	// Update our color
	Theme& t = theme();
	Theme::iterator i = t.find(Theme::TYPE_PLANE);
	if (i != t.end())
	{
		PlaneTheme& ti = static_cast<PlaneTheme&>(*i);
		_color = ti.color();
	}
	else
	{
		// Locate flat type
		i = t.find(Theme::TYPE_DEFAULT);
		ASSERT(i != t.end());
		switch (t.style())
		{
		case Theme::STYLE_3D:
			{
				DefaultTheme3D& ti = static_cast<DefaultTheme3D&>(*i);
				_color = ti.interior();
			}
			break;
			
		case Theme::STYLE_FLAT:
			{
				DefaultThemeFlat& ti = static_cast<DefaultThemeFlat&>(*i);
				_color = ti.interior();
			}
			break;
#ifdef	_DEBUG
		default:
			TRACE("yacg::Plane:  Style %d not implemented\n", theme().style());
			break;
#endif
		}
	}
}

//=============================================================================
// class _PlaneTheme implementation
//=============================================================================

//-----------------------------------------------------------------------------

#define	IMPL(var, func)			_THEMEFUNC(PlaneTheme, var, func)

#define	IMPL_PROPS()	\
IMPL(_color, color)

//-----------------------------------------------------------------------------

#define	IMPL_RESET(type, num)		\
void PlaneTheme::reset##num()		\
{									\
	const FORMAT old = format();	\
	CONVERT_COLOR(_color, type##);	\
	format(type##);					\
	modified();						\
}

#define	IMPL_RESETS()		\
IMPL_RESET(none,)			\
IMPL_RESET(eight, 8)		\
IMPL_RESET(fifteen, 15)		\
IMPL_RESET(sixteen, 16)		\
IMPL_RESET(twentyfour, 24)	\
IMPL_RESET(thirtytwo, 32)

//-----------------------------------------------------------------------------

PlaneTheme::PlaneTheme(Theme& t)
		:
		_ThemeItem(none)
{
	// Plane theme?
	Theme::iterator i = t.find(Theme::TYPE_PLANE);
	if (i != t.end())
		// Yes, so initialize us from it
		*this = static_cast<PlaneTheme&>(*i);
	else
	{
		// No, so get the default theme
		i = t.find(Theme::TYPE_DEFAULT);
		ASSERT(i != t.end());
		switch (t.style())
		{
		case Theme::STYLE_3D:
			{
				DefaultTheme3D& ti = static_cast<DefaultTheme3D&>(*i);
				_color = ti.interior();
			}
			break;
			
		case Theme::STYLE_FLAT:
			{
				DefaultThemeFlat& ti = static_cast<DefaultThemeFlat&>(*i);
				_color = ti.interior();
			}
			break;
#ifdef	_DEBUG
		default:
			TRACE("yacg::PlaneTheme:  Style %d not implemented\n", t.style());
			break;
#endif
		}
	}
}

PlaneTheme::PlaneTheme(const PlaneTheme& r)
		:
		_ThemeItem(r),
		_color(r._color)
{
}

PlaneTheme& PlaneTheme::operator=(const PlaneTheme& r)
{
	if (this != &r)
	{
		_ThemeItem::operator=(r);
		_color = r._color;
	}
	return *this;
}

#ifdef	_DEBUG
void PlaneTheme::dump(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";

	dump_object(i, "yacg::PlaneTheme", this);
	dump_color(sublevel, "Color", _color);
	
	_ThemeItem::dump_item(level);
}
#endif

IMPL_PROPS();

IMPL_RESETS();

#undef	IMPL
#undef	IMPL_PROPS
#undef	IMPL_RESET
#undef	IMPL_RESETS
