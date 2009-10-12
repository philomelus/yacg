#include "precompiled.hpp"
#include <yacg.hpp>
#include <yacg/Theme.hpp>
#include "ThemeImpl.hpp"
#include "utility.hpp"

using namespace yacg;
using namespace utility;

//=============================================================================
// local
//=============================================================================

namespace
{
#ifdef	_DEBUG
	void dump_theme_items(const Theme::types::value_type& i)
	{
	}
#endif

	void reset(Theme::types::value_type& i)
	{
		i.second.item->reset();
	}

	void reset_each(Theme::container::value_type& i)
	{
		std::for_each(i.second->begin(), i.second->end(), bind(reset, _1));
	}

	void reset8(Theme::types::value_type& i)
	{
		i.second.item->reset8();
	}

	void reset8_each(Theme::container::value_type& i)
	{
		std::for_each(i.second->begin(), i.second->end(), bind(reset8, _1));
	}

	void reset15(Theme::types::value_type& i)
	{
		i.second.item->reset15();
	}

	void reset15_each(Theme::container::value_type& i)
	{
		std::for_each(i.second->begin(), i.second->end(), bind(reset15, _1));
	}

	void reset16(Theme::types::value_type& i)
	{
		i.second.item->reset16();
	}

	void reset16_each(Theme::container::value_type& i)
	{
		std::for_each(i.second->begin(), i.second->end(), bind(reset16, _1));
	}

	void reset24(Theme::types::value_type& i)
	{
		i.second.item->reset24();
	}

	void reset24_each(Theme::container::value_type& i)
	{
		std::for_each(i.second->begin(), i.second->end(), bind(reset24, _1));
	}

	void reset32(Theme::types::value_type& i)
	{
		i.second.item->reset32();
	}

	void reset32_each(Theme::container::value_type& i)
	{
		std::for_each(i.second->begin(), i.second->end(), bind(reset32, _1));
	}
}

//=============================================================================
// struct THEME_COLOR:  Struct for holding default colors.
//=============================================================================

struct THEME_COLOR
{
	int a;
	int r;
	int g;
	int b;
};

//=============================================================================
// macro _THEMECOLOR:  Macro for defining default colors within a THEME_COLOR.
//=============================================================================

#define	THEMECOLOR(r, g, b)				{ 0, (r), (g), (b) }
#define	THEMECOLORA(r, g, b, a)			{ (a), (r), (g), (b) }

//=============================================================================
// macro _THEMERESET: Generate a _ThemeItem::reset implementation line
//=============================================================================

#define	_THEMERESET(dat, var, func, idx)						\
var = func(dat[idx].r, dat[idx].g, dat[idx].b)

//=============================================================================
// namespace
//=============================================================================

yacg::Theme yacg::DefaultTheme;
yacg::Theme* yacg::ActiveTheme = &DefaultTheme;

void yacg::yacg_init()
{
	DefaultTheme.reset();
}

#define	IMPL(LA, RA)												\
	bool yacg::operator==(const Theme::LA& l, const Theme::RA& r)	\
	{																\
		if (l._theme != r._theme)									\
			return false;											\
		if (l._theme == 0)											\
			return true;											\
		if (l._style != r._style)									\
			return false;											\
		if (l._style == l._theme->_styles.end())					\
			return true;											\
		return (l._type == r._type);								\
	}																\
	bool yacg::operator!=(const Theme::LA& l, const Theme::RA& r)	\
	{																\
		return (!(l == r));											\
	}

IMPL(iterator, iterator)
IMPL(iterator, const_iterator)
IMPL(const_iterator, iterator)
IMPL(const_iterator, const_iterator)

#undef	IMPL

const yacg::Theme::STYLE yacg::Theme::STYLE_FLAT;
const yacg::Theme::STYLE yacg::Theme::STYLE_3D;
const yacg::Theme::STYLE yacg::Theme::STYLE_BITMAP;
const yacg::Theme::STYLE yacg::Theme::STYLE_USER;
const yacg::Theme::STYLE yacg::Theme::STYLE_USEREND;

const yacg::Theme::TYPE yacg::Theme::TYPE_DEFAULT;
const yacg::Theme::TYPE yacg::Theme::TYPE_BITMAPBUTTON;
const yacg::Theme::TYPE yacg::Theme::TYPE_BOX;
const yacg::Theme::TYPE yacg::Theme::TYPE_BUTTON;
const yacg::Theme::TYPE yacg::Theme::TYPE_CHECKBOX;
const yacg::Theme::TYPE yacg::Theme::TYPE_CHECKEDBOX;
const yacg::Theme::TYPE yacg::Theme::TYPE_DIALOG;
const yacg::Theme::TYPE yacg::Theme::TYPE_EDITBOX;
const yacg::Theme::TYPE yacg::Theme::TYPE_LIST;
const yacg::Theme::TYPE yacg::Theme::TYPE_MENU;
const yacg::Theme::TYPE yacg::Theme::TYPE_PLANE;
const yacg::Theme::TYPE yacg::Theme::TYPE_RADIO;
const yacg::Theme::TYPE yacg::Theme::TYPE_RADIOGROUP;
const yacg::Theme::TYPE yacg::Theme::TYPE_SELECTIONBOX;
const yacg::Theme::TYPE yacg::Theme::TYPE_SLIDER;
const yacg::Theme::TYPE yacg::Theme::TYPE_TAB;
const yacg::Theme::TYPE yacg::Theme::TYPE_TABPAGE;
const yacg::Theme::TYPE yacg::Theme::TYPE_TEXTBOX;
const yacg::Theme::TYPE yacg::Theme::TYPE_TITLE;
const yacg::Theme::TYPE yacg::Theme::TYPE_WINDOW;
const yacg::Theme::TYPE yacg::Theme::TYPE_USER;
const yacg::Theme::TYPE yacg::Theme::TYPE_USEREND;

//=============================================================================
// class _DefaultTheme implementation
//=============================================================================

_DefaultTheme::_DefaultTheme()
		:
		_ThemeItem(none)
{
	_font = ::font;
}

_DefaultTheme::_DefaultTheme(const _DefaultTheme& r)
		:
		_ThemeItem(r),
		_font(r._font)
{
}

_DefaultTheme& _DefaultTheme::operator=(const _DefaultTheme& r)
{
	if (this != &r)
	{
		_ThemeItem::operator=(r);
		_font = r._font;
	}
	return *this;
}

#ifdef	_DEBUG
void _DefaultTheme::dump(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";
	
	dump_object(i, "yacg::_DefaultTheme", this);
	
	dump_font(sublevel, _font);
	
	_ThemeItem::dump_item(level);
}
#endif

FONT* _DefaultTheme::font() const
{
	return _font;
}

void _DefaultTheme::font(FONT* f)
{
	if (_font != f)
	{
		_font = f;
		modified();
	}
}

void _DefaultTheme::reset()
{
	format(none);
	modified();
}

void _DefaultTheme::reset8()
{
	format(eight);
	modified();
}

void _DefaultTheme::reset15()
{
	format(fifteen);
	modified();
}

void _DefaultTheme::reset16()
{
	format(sixteen);
	modified();
}

void _DefaultTheme::reset24()
{
	format(twentyfour);
	modified();
}

void _DefaultTheme::reset32()
{
	format(none);
	modified();
}

//=============================================================================
// class _ThemeItem implementation
//=============================================================================

_ThemeItem::_ThemeItem(FORMAT f)
		:
		_format(f)
{
}

_ThemeItem::_ThemeItem(const _ThemeItem& o)
		:
		_format(o._format)
{
}

_ThemeItem::~_ThemeItem()
{
}

_ThemeItem& _ThemeItem::operator=(const _ThemeItem& o)
{
	if (this != &o)
	{
		_format = o._format;
		modified();
	}
	return *this;
}

EVENTID _ThemeItem::connect(MODIFIED_EVENT f)
{
	return _handlers.connect(f);
}

int _ThemeItem::convert(FORMAT f, int c)
{
	switch (f)
	{
	case eight:
		return makecol(getr8(c), getg8(c), getb8(c));
		break;
	
	case fifteen:
		return makecol(getr15(c), getg15(c), getb15(c));
		break;
		
	case sixteen:
		return makecol(getr16(c), getg16(c), getb16(c));
		break;
		
	case twentyfour:
		return makecol(getr24(c), getg24(c), getb24(c));
		break;
	
	case thirtytwo:
		return makecol(getr32(c), getg32(c), getb32(c));
		break;

	default:
		return makecol(getr(c), getg(c), getb(c));
		break;
	}
}

int _ThemeItem::convert(FORMAT f, int c, FORMAT n)
{
	int r;
	int g;
	int b;
	switch (f)
	{
	case eight:			r = getr8(c);  g = getg8(c);  b = getb8(c);  break;
	case fifteen:		r = getr15(c); g = getg15(c); b = getb15(c); break;
	case sixteen:		r = getr16(c); g = getg16(c); b = getb16(c); break;
	case twentyfour:	r = getr24(c); g = getg24(c); b = getb24(c); break;
	case thirtytwo:		r = getr32(c); g = getg32(c); b = getb32(c); break;
	default:			r = getr(c);   g = getg(c);   b = getb(c);   break;
	}
	switch (n)
	{
	case eight:			return makecol8(r, g, b);
	case fifteen:		return makecol15(r, g, b);
	case sixteen:		return makecol16(r, g, b);
	case twentyfour:	return makecol24(r, g, b);
	case thirtytwo:		return makecol32(r, g, b);
	default:			return ::makecol(r, g, b);
	}
}

void _ThemeItem::disconnect(EVENTID h)
{
	if (h.connected())
		h.disconnect();
}

#ifdef	_DEBUG
void _ThemeItem::dump_item(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	dump_object(i, "yacg::_ThemeItem", this);
	TRACE("%sFormat: %s\n", sublevel.c_str(), format2string(_format).c_str());
}
#endif

_ThemeItem::FORMAT _ThemeItem::format() const
{
	return _format;
}

void _ThemeItem::format(FORMAT f)
{
	_format = f;
}

#ifdef	_DEBUG
std::string _ThemeItem::format2string(FORMAT f)
{
	switch (f)
	{
	case none:			return std::string("none");
	case eight:			return std::string("eight");
	case fifteen:		return std::string("fifteen");
	case sixteen:		return std::string("sixteen");
	case twentyfour:	return std::string("twentyfour");
	case thirtytwo:		return std::string("thirtytwo");
	default:			return std::string("INVALID FORMAT");
	}
}
#endif

int _ThemeItem::makecol(int r, int g, int b)
{
	switch (_format)
	{
	case eight:
		return makecol8(r, g, b);
		break;
	
	case fifteen:
		return makecol15(r, g, b);
		break;
		
	case sixteen:
		return makecol16(r, g, b);
		break;
		
	case twentyfour:
		return makecol24(r, g, b);
		break;
	
	case thirtytwo:
		return makecol32(r, g, b);
		break;

	default:
		return makecol(r, g, b);
		break;
	}
}

void _ThemeItem::modified(int h)
{
	_handlers(*this, h);
}

//=============================================================================
// class Theme implementation
//=============================================================================

namespace
{
	const THEME_COLOR Theme_colors[] =
	{
		// Aqua		
		{	0,		0,		255,	255	},
		{	0,		0,		191,	191	},
		{	0,		127,	255,	255	},
		{	0,		0,		127,	127	},
		{	0,		191,	255,	255	},

		// Black
		{	0,		0,		0,		0	},

		// Blue		
		{	0,		0,		0,		255	},
		{	0,		0,		0,		191	},
		{	0,		159,	159,	255	},
		{	0,		0,		0,		159	},
		{	0,		191,	191,	255	},

		// Gray
		{	0,		212,	208,	200	},
		{	0,		128,	128,	128	},
		{	0,		223,	223,	223	},
		{	0,		95,		95,		95	},
		{	0,		245,	245,	245	},
		
		// Green
		{	0,		0,		255,	0	},
		{	0,		0,		191,	0	},
		{	0,		127,	255,	127	},
		{	0,		0,		159,	0	},
		{	0,		191,	255,	191	},
		
		// Purple
		{	0,		255,	0,		255	},
		{	0,		191,	0,		191	},
		{	0,		255,	159,	255	},
		{	0,		159,	0,		159	},
		{	0,		255,	191,	255	},
		
		// Red
		{	0,		255,	0,		0	},
		{	0,		191,	0,		0	},
		{	0,		255,	127,	127	},
		{	0,		159,	0,		0	},
		{	0,		255,	191,	191	},
		
		// Yellow
		{	0,		255,	255,	0	},
		{	0,		191,	191,	0	},
		{	0,		255,	255,	127	},
		{	0,		127,	127,	0	},
		{	0,		255,	255,	191	},

		// White
		{	0,		255,	255,	255	}
	};
	
	enum
	{
		p_aqua = 0,
		p_aqua_dark,
		p_aqua_light,
		p_aqua_very_dark,
		p_aqua_very_light,
		
		p_black,

		p_blue,
		p_blue_dark,
		p_blue_light,
		p_blue_very_dark,
		p_blue_very_light,

		p_gray,
		p_gray_dark,
		p_gray_light,
		p_gray_very_dark,
		p_gray_very_light,

		p_green,
		p_green_dark,
		p_green_light,
		p_green_very_dark,
		p_green_very_light,

		p_purple,
		p_purple_dark,
		p_purple_light,
		p_purple_very_dark,
		p_purple_very_light,

		p_red,
		p_red_dark,
		p_red_light,
		p_red_very_dark,
		p_red_very_light,

		p_yellow,
		p_yellow_dark,
		p_yellow_light,
		p_yellow_very_dark,
		p_yellow_very_light,

		p_white
	};
}

//-----------------------------------------------------------------------------

Theme::Theme(_ThemeItem::FORMAT f)
		:
		_format(f),
		_style(_styles.end())
{
	// Add the default 3d theme
	{
		std::auto_ptr<types> t(new types);
		type_data td;
		td.connection = _3d.connect(bind(&Theme::item_modified, this, _1, _2));
		td.item = &_3d;
		t->insert(std::make_pair(TYPE_DEFAULT, td));
		_styles.insert(std::make_pair(STYLE_3D, t.get()));
		t.release();
	}

	// Add the default flat theme
	{
		std::auto_ptr<types> t(new types);
		type_data td;
		td.connection = _flat.connect(bind(&Theme::item_modified, this, _1, _2));
		td.item = &_flat;
		t->insert(std::make_pair(TYPE_DEFAULT, td));
		_styles.insert(std::make_pair(STYLE_FLAT, t.get()));
		t.release();
	}
	
	// Default style is 3D
	_style = _styles.find(STYLE_3D);

	// Initialize the standard colors
	switch (f)
	{
	case _ThemeItem::none:
		reset();
		break;
		
	case _ThemeItem::eight:
		reset8();
		break;

	case _ThemeItem::fifteen:
		reset15();
		break;

	case _ThemeItem::sixteen:
		reset16();
		break;

	case _ThemeItem::twentyfour:
		reset24();
		break;

	case _ThemeItem::thirtytwo:
		reset32();
		break;
	
#ifdef	_DEBUG
	default:
		TRACE("yacg::Theme: Format %d unknown to constructor!", f);
		break;
#endif
	}
}

Theme::Theme(const Theme& r)
{
	*this = r;
}

Theme::~Theme()
{
	// Clean up allocated types containers
	std::for_each(_styles.begin(), _styles.end(),
			bind(&Theme::delete_styles, this, _1));
	_styles.clear();
}

Theme& Theme::operator=(const Theme& r)
{
	if (this != &r)
	{
		_style = r._style;
		_3d = r._3d;
		_flat = r._flat;
		std::for_each(r._styles.begin(), r._styles.end(), bind(&Theme::copy_theme, this, _1));
	}
	return *this;
}

void Theme::copy_theme(const Theme::container::value_type& i)
{
	// Copy all _ThemeItems for the style
	std::for_each(i.second->begin(), i.second->end(),
			bind(&Theme::copy_types, this, _1, i.first));
}

void Theme::copy_types(const Theme::types::value_type& i, STYLE s)
{
	// Insert the _ThemeItem for the style
	insert(s, i.first, *i.second.item);
}

void Theme::delete_styles(Theme::container::value_type& i)
{
	// NOTE:  Call from DESTRUCTOR only, or styles/items will be left in object
	// with an invalid state!
	
	// Disconnect modified event handler from all items
	std::for_each(i.second->begin(), i.second->end(),
			bind(&Theme::disconnect_type, this, _1));
	
	// Delete collection object
	delete i.second;
}

void Theme::disconnect_type(Theme::types::value_type& i)
{
	// NOTE:  Call from DESTRUCTOR only, or items will be left in object with an
	// invalid state!
	i.second.item->disconnect(i.second.connection);
}

//-----------------------------------------------------------------------------

Theme::iterator Theme::begin()
{
	iterator i;
	i._theme = this;
	i._style = _style;
	if (i._style != _styles.end())
		i._type = i._style->second->begin();
	return i;
}

Theme::const_iterator Theme::begin() const
{
	const_iterator i;
	i._theme = this;
	i._style = _style;
	if (i._style != _styles.end())
		i._type = i._style->second->begin();
	return i;
}

Theme::iterator Theme::end()
{
	iterator i;
	i._theme = this;
	i._style = _style;
	if (i._style != _styles.end())
		i._type = i._style->second->end();
	return i;
}

Theme::const_iterator Theme::end() const
{
	const_iterator i;
	i._theme = this;
	i._style = _style;
	if (i._style != _styles.end())
		i._type = i._style->second->end();
	return i;
}

//-----------------------------------------------------------------------------

#ifdef	_DEBUG
void Theme::dump(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";

	std::string level(i);
	level += "    ";
	
	dump_object(i, "yacg::Theme", this);
	TRACE("%sActive Style: %s\n", sublevel.c_str(), style2string(_style->first).c_str());
	TRACE("%sFormat: %s\n", sublevel.c_str(), _ThemeItem::format2string(_format).c_str());
	dump_count(sublevel, "Styles", _styles.size());
	{
		dump_divider d1(sublevel, "GLOBAL COLORS");
		dump_color(level, "AQUA", _AQUA);
		dump_color(level, "AQUA_DARK", _AQUA_DARK);
		dump_color(level, "AQUA_LIGHT", _AQUA_LIGHT);
		dump_color(level, "AQUA_VERY_DARK", _AQUA_VERY_DARK);
		dump_color(level, "AQUA_VERY_LIGHT", _AQUA_VERY_LIGHT);
		dump_color(level, "BLACK", _BLACK);
		dump_color(level, "BLUE", _BLUE);
		dump_color(level, "BLUE_DARK", _BLUE_DARK);
		dump_color(level, "BLUE_LIGHT", _BLUE_LIGHT);
		dump_color(level, "BLUE_VERY_DARK", _BLUE_VERY_DARK);
		dump_color(level, "BLUE_VERY_LIGHT", _BLUE_VERY_LIGHT);
		dump_color(level, "GRAY", _GRAY);
		dump_color(level, "GRAY_DARK", _GRAY_DARK);
		dump_color(level, "GRAY_LIGHT", _GRAY_LIGHT);
		dump_color(level, "GRAY_VERY_DARK", _GRAY_VERY_DARK);
		dump_color(level, "GRAY_VERY_LIGHT", _GRAY_VERY_LIGHT);
		dump_color(level, "GREEN", _GREEN);
		dump_color(level, "GREEN_DARK", _GREEN_DARK);
		dump_color(level, "GREEN_LIGHT", _GREEN_LIGHT);
		dump_color(level, "GREEN_VERY_DARK", _GREEN_VERY_DARK);
		dump_color(level, "GREEN_VERY_LIGHT", _GREEN_VERY_LIGHT);
		dump_color(level, "PURPLE", _PURPLE);
		dump_color(level, "PURPLE_DARK", _PURPLE_DARK);
		dump_color(level, "PURPLE_LIGHT", _PURPLE_LIGHT);
		dump_color(level, "PURPLE_VERY_DARK", _PURPLE_VERY_DARK);
		dump_color(level, "PURPLE_VERY_LIGHT", _PURPLE_VERY_LIGHT);
		dump_color(level, "RED", _RED);
		dump_color(level, "RED_DARK", _RED_DARK);
		dump_color(level, "RED_LIGHT", _RED_LIGHT);
		dump_color(level, "RED_VERY_DARK", _RED_VERY_DARK);
		dump_color(level, "RED_VERY_LIGHT", _RED_VERY_LIGHT);
		dump_color(level, "YELLOW", _YELLOW);
		dump_color(level, "YELLOW_DARK", _YELLOW_DARK);
		dump_color(level, "YELLOW_LIGHT", _YELLOW_LIGHT);
		dump_color(level, "YELLOW_VERY_DARK", _YELLOW_VERY_DARK);
		dump_color(level, "YELLOW_VERY_LIGHT", _YELLOW_VERY_LIGHT);
		dump_color(level, "WHITE", _WHITE);
	}
	{
		dump_divider d1(sublevel, "STYLES");
		std::for_each(_styles.begin(), _styles.end(), bind(&Theme::dump_styles, this, _1, level));
	}
}

void Theme::dump_styles(const Theme::container::value_type& v, const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";
	
	dump_divider d1(i, "STYLE");
	TRACE("%sStyle: %s\n", sublevel.c_str(), style2string(v.first).c_str());
	{
		dump_divider d2(sublevel, "TYPES");
		std::for_each(v.second->begin(), v.second->end(), bind(&Theme::dump_type, this, _1, level));
	}
}

void Theme::dump_type(const Theme::types::value_type& v, const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";
	
	TRACE("%sType: %s\n", i.c_str(), type2string(v.first).c_str());
	v.second.item->dump(i);
}

std::string Theme::style2string(STYLE s)
{
	switch (s)
	{
	case STYLE_FLAT:
		return std::string("STYLE_FLAT");
		
	case STYLE_3D:
		return std::string("STYLE_3D");
		
	case STYLE_BITMAP:
		return std::string("STYLE_BITMAP");
		
	default:
		if (s >= STYLE_USER)
			return std::string("Program Defined");
		else
			return std::string("Unknown Reserved");
	}
}

std::string Theme::type2string(TYPE t)
{
	switch (t)
	{
	case TYPE_DEFAULT:
		return std::string("TYPE_DEFAULT");
		
	case TYPE_BITMAPBUTTON:
		return std::string("TYPE_BITMAPBUTTON");
		
	case TYPE_BOX:
		return std::string("TYPE_BOX");
		
	case TYPE_BUTTON:
		return std::string("TYPE_BUTTON");
		
	case TYPE_CHECKBOX:
		return std::string("TYPE_CHECKBOX");
		
	case TYPE_CHECKEDBOX:
		return std::string("TYPE_CHECKEDBOX");
		
	case TYPE_DIALOG:
		return std::string("TYPE_DIALOG");
		
	case TYPE_EDITBOX:
		return std::string("TYPE_EDITBOX");
		
	case TYPE_LIST:
		return std::string("TYPE_LIST");
		
	case TYPE_MENU:
		return std::string("TYPE_MENU");
		
	case TYPE_PLANE:
		return std::string("TYPE_PLANE");
		
	case TYPE_RADIO:
		return std::string("TYPE_RADIO");
		
	case TYPE_RADIOGROUP:
		return std::string("TYPE_RADIOGROUP");
		
	case TYPE_SELECTIONBOX:
		return std::string("TYPE_SELECTIONBOX");
		
	case TYPE_SLIDER:
		return std::string("TYPE_SLIDER");
		
	case TYPE_TAB:
		return std::string("TYPE_TAB");
		
	case TYPE_TABPAGE:
		return std::string("TYPE_TABPAGE");
		
	case TYPE_TEXTBOX:
		return std::string("TYPE_TEXTBOX");
		
	case TYPE_TITLE:
		return std::string("TYPE_TITLE");
		
	case TYPE_WINDOW:
		return std::string("TYPE_WINDOW");
		
	default:
		if (t >= TYPE_USER)
			return std::string("Program Defined");
		else
			return std::string("Unknown Reserved");
	}
}
#endif

//-----------------------------------------------------------------------------

bool Theme::empty() const
{
	if (_style == _styles.end())
		return true;
	return _style->second->empty();
}

//-----------------------------------------------------------------------------

void Theme::erase(iterator i)
{
	ASSERT(i._theme == this);
	if (i._style != _styles.end())
	{
		// Save copy of data for disconnection
		type_data td = i._type->second;
		
		// Erase type
		_style->second->erase(i._type);
		
		// Disconnect modification handler
		td.item->disconnect(td.connection);
		
		// Generate modification event
		modified();
	}
}

Theme::iterator Theme::insert(TYPE t, _ThemeItem& ti)
{
	// Default construct iterator
	iterator i;
	
	// Initialize known fields
	i._theme = this;
	i._style = _style;
	
	// Connect up the item modified event handler
	type_data td;
	td.connection = ti.connect(bind(&Theme::item_modified, this, _1, _2));
	td.item = &ti;
	
	// Associate type with item data
	i._type = _style->second->insert(std::make_pair(t, td)).first;
	
	// Generate modification events
	modified();
	
	return i;
}

Theme::iterator Theme::insert(STYLE s, TYPE t, _ThemeItem& ti)
{
	iterator i;
	i._theme = this;
	
	// Is _style the desired style?
	if (_style->first != s)
	{
		// No, does it exist?
		i._style = _styles.find(s);
		if (i._style == _styles.end())
			// No, so create it
			i._style = _styles.insert(std::make_pair(s, new types)).first;
	}
	else
		i._style = _style;

	// Prepare type data by connecting up modification handler	
	type_data td;
	td.connection = ti.connect(bind(&Theme::item_modified, this, _1, _2));
	td.item = &ti;
	
	// Insert theme item into style
	i._type = i._style->second->insert(std::make_pair(t, td)).first;
	
	// Generate modification events
	modified();
	
	return i;
}

//-----------------------------------------------------------------------------

Theme::iterator Theme::find(TYPE t)
{
	if (_style == _styles.end())
		return end();
	iterator i;
	i._theme = this;
	i._style = _style;
	i._type = _style->second->find(t);
	return i;
}

//-----------------------------------------------------------------------------

Theme::FORMAT Theme::format() const
{
	return _format;
}

void Theme::format(FORMAT f)
{
	switch (f)
	{
	case _ThemeItem::none:
		reset();
		break;
		
	case _ThemeItem::eight:
		reset8();
		break;

	case _ThemeItem::fifteen:
		reset15();
		break;

	case _ThemeItem::sixteen:
		reset16();
		break;

	case _ThemeItem::twentyfour:
		reset24();
		break;

	case _ThemeItem::thirtytwo:
		reset32();
		break;
	
#ifdef	_DEBUG
	default:
		TRACE("yacg::Theme::format: Format %d unknown!", f);
		break;
#endif
	}
}

//-----------------------------------------------------------------------------

#define	_RESET(var, func, idx)	_THEMERESET(Theme_colors, var, func, idx)

#define	RESET(var, idx)			_RESET(var, makecol, idx)
#define	RESET8(var, idx)		_RESET(var, makecol8, idx)
#define	RESET15(var, idx)		_RESET(var, makecol15, idx)
#define	RESET16(var, idx)		_RESET(var, makecol16, idx)
#define	RESET24(var, idx)		_RESET(var, makecol24, idx)
#define	RESET32(var, idx)		_RESET(var, makecol32, idx)

#define	RESET_ALL(macro)							\
	macro(_AQUA, p_aqua);							\
	macro(_AQUA_DARK, p_aqua_dark);					\
	macro(_AQUA_LIGHT, p_aqua_light);				\
	macro(_AQUA_VERY_DARK, p_aqua_very_dark);		\
	macro(_AQUA_VERY_LIGHT, p_aqua_very_light);		\
	macro(_BLACK, p_black);							\
	macro(_BLUE, p_blue);							\
	macro(_BLUE_DARK, p_blue_dark);					\
	macro(_BLUE_LIGHT, p_blue_light);				\
	macro(_BLUE_VERY_DARK, p_blue_very_dark);		\
	macro(_BLUE_VERY_LIGHT, p_blue_very_light);		\
	macro(_GRAY, p_gray);							\
	macro(_GRAY_DARK, p_gray_dark);					\
	macro(_GRAY_LIGHT, p_gray_light);				\
	macro(_GRAY_VERY_DARK, p_gray_very_dark);		\
	macro(_GRAY_VERY_LIGHT, p_gray_very_light);		\
	macro(_GREEN, p_green);							\
	macro(_GREEN_DARK, p_green_dark);				\
	macro(_GREEN_LIGHT, p_green_light);				\
	macro(_GREEN_VERY_DARK, p_green_very_dark);		\
	macro(_GREEN_VERY_LIGHT, p_green_very_light);	\
	macro(_PURPLE, p_purple);						\
	macro(_PURPLE_DARK, p_purple_dark);				\
	macro(_PURPLE_LIGHT, p_purple_light);			\
	macro(_PURPLE_VERY_DARK, p_purple_very_dark);	\
	macro(_PURPLE_VERY_LIGHT, p_purple_very_light);	\
	macro(_RED, p_red);								\
	macro(_RED_DARK, p_red_dark);					\
	macro(_RED_LIGHT, p_red_light);					\
	macro(_RED_VERY_DARK, p_red_very_dark);			\
	macro(_RED_VERY_LIGHT, p_red_very_light);		\
	macro(_YELLOW, p_yellow);						\
	macro(_YELLOW_DARK, p_yellow_dark);				\
	macro(_YELLOW_LIGHT, p_yellow_light);			\
	macro(_YELLOW_VERY_DARK, p_yellow_very_dark);	\
	macro(_YELLOW_VERY_LIGHT, p_yellow_very_light);	\
	macro(_WHITE, p_white)

void Theme::reset()
{
	// Initialize common colors
	RESET_ALL(RESET);
	
	// Update styles
	std::for_each(_styles.begin(), _styles.end(), bind(reset_each, _1));
	
	// Save format
	_format = _ThemeItem::none;
	
	// Generate modification event
	modified();
}

void Theme::reset8()
{
	// Initialize common colors
	RESET_ALL(RESET8);

	// Update styles
	std::for_each(_styles.begin(), _styles.end(), bind(reset8_each, _1));
	
	// Save format
	_format = _ThemeItem::eight;
	
	// Generate modification event
	modified();
}

void Theme::reset15()
{
	// Initialize common colors
	RESET_ALL(RESET15);

	// Update styles
	std::for_each(_styles.begin(), _styles.end(), bind(reset15_each, _1));
	
	// Save format
	_format = _ThemeItem::fifteen;
	
	// Generate modification event
	modified();
}

void Theme::reset16()
{
	// Initialize common colors
	RESET_ALL(RESET15);

	// Update styles
	std::for_each(_styles.begin(), _styles.end(), bind(reset16_each, _1));
	
	// Save format
	_format = _ThemeItem::sixteen;
	
	// Generate modification event
	modified();
}

void Theme::reset24()
{
	// Initialize common colors
	RESET_ALL(RESET24);

	// Update styles
	std::for_each(_styles.begin(), _styles.end(), bind(reset24_each, _1));
	
	// Save format
	_format = _ThemeItem::twentyfour;
	
	// Generate modification event
	modified();
}

void Theme::reset32()
{
	// Initialize common colors
	RESET_ALL(RESET32);

	// Update styles
	std::for_each(_styles.begin(), _styles.end(), bind(reset32_each, _1));
	
	// Save format
	_format = _ThemeItem::thirtytwo;
	
	// Generate modification event
	modified();
}

//-----------------------------------------------------------------------------

int Theme::size() const
{
	if (_style == _styles.end())
		return 0;
	return int(_style->second->size());
}

int Theme::size(STYLE s) const
{
	if (s == _style->first)
		return _style->second->size();
	container::const_iterator i = _styles.find(s);
	if (i == _styles.end())
		return 0;
	return int(i->second->size());
}

//-----------------------------------------------------------------------------

Theme::STYLE Theme::style() const
{
	return _style->first;
}

void Theme::style(STYLE s)
{
	// If the new style is the current style, ignore change request
	if (_style != _styles.end())
	{
		if (_style->first == s)
			return;
	}

	// Find the new style	
	_style = _styles.find(s);
	
	// Create it if it doesn't exist
	if (_style == _styles.end())
		_style = _styles.insert(std::make_pair(s, new types)).first;
	
	// Generate modified event, since the active style changed
	modified();
}

//-----------------------------------------------------------------------------

EVENTID Theme::connect(MODIFIED_EVENT f)
{
	return _handlers.connect(f);
}

void Theme::disconnect(EVENTID f)
{
	if (f.connected())
		f.disconnect();
}

void Theme::item_modified(_ThemeItem& i, int h)
{
	modified();
}

void Theme::modified()
{
	_handlers(*this);
}

#undef	_RESET
#undef	RESET
#undef	RESET8
#undef	RESET15
#undef	RESET16
#undef	RESET24
#undef	RESET32
#undef	RESET_ALL

//=============================================================================
// class Theme::iterator implementation
//=============================================================================

Theme::iterator::iterator()
		:
		_theme(0)
{
}

Theme::iterator::iterator(const iterator& i)
{
	*this = i;
}

Theme::iterator& Theme::iterator::operator=(const iterator& i)
{
	if (this != &i)
	{
		_theme = i._theme;
		_style = i._style;
		_type = i._type;
	}
	return *this;
}

_ThemeItem& Theme::iterator::operator*()
{
	return *_type->second.item;
}

_ThemeItem* Theme::iterator::operator->()
{
	return _type->second.item;
}

Theme::iterator& Theme::iterator::operator++()
{
	++_type;
	return *this;
}

Theme::iterator Theme::iterator::operator++(int)
{
	Theme::iterator o(*this);
	++_type;
	return o;
}

//=============================================================================
// class Theme::const_iterator implementation
//=============================================================================

Theme::const_iterator::const_iterator()
		:
		_theme(0)
{
}

Theme::const_iterator::const_iterator(const const_iterator& i)
{
	*this = i;
}

Theme::const_iterator::const_iterator(const iterator& i)
		:
		_style(i._style),
		_type(i._type),
		_theme(i._theme)
{
}

Theme::const_iterator& Theme::const_iterator::operator=(const const_iterator& i)
{
	if (this != &i)
	{
		_theme = i._theme;
		_style = i._style;
		_type = i._type;
	}
	return *this;
}

const _ThemeItem& Theme::const_iterator::operator*()
{
	return *_type->second.item;
}

const _ThemeItem* Theme::const_iterator::operator->()
{
	return _type->second.item;
}

Theme::const_iterator& Theme::const_iterator::operator++()
{
	++_type;
	return *this;
}

Theme::const_iterator Theme::const_iterator::operator++(int)
{
	Theme::const_iterator o(*this);
	++_type;
	return o;
}

//=============================================================================
// class DefaultTheme3D implementation
//=============================================================================

#define	IMPL(var, func)			_THEMEFUNC(DefaultTheme3D, var, func)

#define	_RESET(var, func, idx)	_THEMERESET(Theme3D_colors, var, func, idx)

#define	RESET(var, idx)			_RESET(var, ::makecol, idx)
#define	RESET8(var, idx)		_RESET(var, makecol8, idx)
#define	RESET15(var, idx)		_RESET(var, makecol15, idx)
#define	RESET16(var, idx)		_RESET(var, makecol16, idx)
#define	RESET24(var, idx)		_RESET(var, makecol24, idx)
#define	RESET32(var, idx)		_RESET(var, makecol32, idx)

#define	RESET_ALL(macro)										\
	macro(_frameDark, t3d_FrameDark);							\
	macro(_frameDarkest, t3d_FrameDarkest);						\
	macro(_frameLight, t3d_FrameLight);							\
	macro(_frameLightest, t3d_FrameLightest);					\
	macro(_inactiveTextBackground, t3d_InactiveTextBackground);	\
	macro(_inactiveTextForeground, t3d_InactiveTextForeground);	\
	macro(_interior, t3d_Interior);								\
	macro(_selectionBackground, t3d_SelectionBackground);		\
	macro(_selectionForeground, t3d_SelectionForeground);		\
	macro(_textBackground, t3d_TextBackground);					\
	macro(_textForeground, t3d_TextForeground)

namespace
{
	const THEME_COLOR Theme3D_colors[] =
	{
		THEMECOLOR(128, 128, 128),		// GRAY_DARK
		THEMECOLOR(95, 95, 95),			// GRAY_VERY_DARK
		THEMECOLOR(212, 208, 200),		// GRAY
		THEMECOLOR(255, 255, 255),		// WHITE
		THEMECOLOR(255, 255, 255),		// WHITE
		THEMECOLOR(128, 128, 128),		// GRAY DARK
		THEMECOLOR(212, 208, 200),		// GRAY
		THEMECOLOR(10, 36, 106),		// BLUE DARK (kinda bright)
		THEMECOLOR(255, 255, 255),		// WHITE
		THEMECOLOR(255, 255, 255),		// WHITE
		THEMECOLOR(0, 0, 0)				// BLACK
	};
	
	enum
	{
		t3d_FrameDark = 0,
		t3d_FrameDarkest,
		t3d_FrameLight,
		t3d_FrameLightest,
		t3d_InactiveTextBackground,
		t3d_InactiveTextForeground,
		t3d_Interior,
		t3d_SelectionBackground,
		t3d_SelectionForeground,
		t3d_TextBackground,
		t3d_TextForeground
	};
}

DefaultTheme3D::DefaultTheme3D()
{
	RESET_ALL(RESET);
}

DefaultTheme3D::DefaultTheme3D(const DefaultTheme3D& r)
		:
		_DefaultTheme(r),
		_frameDark(r._frameDark),
		_frameDarkest(r._frameDarkest),
		_frameLight(r._frameLight),
		_frameLightest(r._frameLightest)
{
}

DefaultTheme3D& DefaultTheme3D::operator=(const DefaultTheme3D& r)
{
	if (this != &r)
	{
		_DefaultTheme::operator=(r);
		_frameDark = r._frameDark;
		_frameDarkest = r._frameDarkest;
		_frameLight = r._frameLight;
		_frameLightest = r._frameLightest;
	}
	return *this;
}

#ifdef	_DEBUG
void DefaultTheme3D::dump(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";

	std::string level(i);
	level += "    ";
	
	dump_object(i, "yacg::DefaultTheme3D", this);
	
	dump_color(sublevel, "Frame Lightest", _frameLightest);
	dump_color(sublevel, "Frame Light", _frameLight);
	dump_color(sublevel, "Frame Dark", _frameDark);
	dump_color(sublevel, "Frame Darkest", _frameDarkest);
	dump_color(sublevel, "Inactive Text Background", _inactiveTextBackground);
	dump_color(sublevel, "Inactive Text Foreground", _inactiveTextForeground);
	dump_color(sublevel, "Interior", _interior);
	dump_color(sublevel, "Selection Background", _selectionBackground);
	dump_color(sublevel, "Selection Foreground", _selectionForeground);
	dump_color(sublevel, "Text Background", _textBackground);
	dump_color(sublevel, "Text Foreground", _textForeground);
	
	_DefaultTheme::dump(level);
}
#endif

void DefaultTheme3D::reset()
{
	_DefaultTheme::reset();
	RESET_ALL(RESET);
}

void DefaultTheme3D::reset8()
{
	_DefaultTheme::reset8();
	RESET_ALL(RESET8);
}

void DefaultTheme3D::reset15()
{
	_DefaultTheme::reset15();
	RESET_ALL(RESET15);
}

void DefaultTheme3D::reset16()
{
	_DefaultTheme::reset16();
	RESET_ALL(RESET16);
}

void DefaultTheme3D::reset24()
{
	_DefaultTheme::reset24();
	RESET_ALL(RESET24);
}

void DefaultTheme3D::reset32()
{
	_DefaultTheme::reset32();
	RESET_ALL(RESET32);
}

IMPL(_frameDark, frame_dark)
IMPL(_frameDarkest, frame_darkest)
IMPL(_frameLight, frame_light)
IMPL(_frameLightest, frame_lightest)
IMPL(_inactiveTextBackground, inactive_text_background)
IMPL(_inactiveTextForeground, inactive_text_foreground)
IMPL(_interior, interior)
IMPL(_selectionBackground, selection_background)
IMPL(_selectionForeground, selection_foreground)
IMPL(_textBackground, text_background)
IMPL(_textForeground, text_foreground)

#undef	IMPL
#undef	_RESET
#undef	RESET
#undef	RESET8
#undef	RESET15
#undef	RESET16
#undef	RESET24
#undef	RESET32
#undef	RESET_ALL

//=============================================================================
// class DefaultThemeFlat implementation
//=============================================================================

#define	IMPL(var, func)			_THEMEFUNC(DefaultThemeFlat, var, func)

#define	_RESET(var, func, idx)	_THEMERESET(ThemeFlat_colors, var, func, idx)

#define	RESET(var, idx)			_RESET(var, ::makecol, idx)
#define	RESET8(var, idx)		_RESET(var, makecol8, idx)
#define	RESET15(var, idx)		_RESET(var, makecol15, idx)
#define	RESET16(var, idx)		_RESET(var, makecol16, idx)
#define	RESET24(var, idx)		_RESET(var, makecol24, idx)
#define	RESET32(var, idx)		_RESET(var, makecol32, idx)

#define	RESET_ALL(macro)										\
	macro(_frame, f_Frame);										\
	macro(_inactiveTextBackground, f_InactiveTextBackground);	\
	macro(_inactiveTextForeground, f_InactiveTextForeground);	\
	macro(_interior, f_Interior);								\
	macro(_selectionBackground, f_SelectionBackground);			\
	macro(_selectionForeground, f_SelectionForeground);			\
	macro(_textBackground, f_TextBackground);					\
	macro(_textForeground, f_TextForeground)

namespace
{
	const THEME_COLOR ThemeFlat_colors[] =
	{
		THEMECOLOR(255, 255, 255),		// WHITE
		THEMECOLOR(0, 0, 0),			// BLACK
		THEMECOLOR(128, 128, 128),		// GRAY DARK
		THEMECOLOR(0, 0, 0),			// GRAY
		THEMECOLOR(10, 36, 106),		// BLUE DARK (kinda bright)
		THEMECOLOR(255, 255, 255),		// WHITE
		THEMECOLOR(0, 0, 0),			// BLACK
		THEMECOLOR(255, 255, 255)		// WHITE
	};
	
	enum
	{
		f_Frame,
		f_InactiveTextBackground,
		f_InactiveTextForeground,
		f_Interior,
		f_SelectionBackground,
		f_SelectionForeground,
		f_TextBackground,
		f_TextForeground
	};
}

DefaultThemeFlat::DefaultThemeFlat()
{
	RESET_ALL(RESET);
}

DefaultThemeFlat::DefaultThemeFlat(const DefaultThemeFlat& r)
		:
		_DefaultTheme(r),
		_frame(r._frame),
		_inactiveTextBackground(r._inactiveTextBackground),
		_inactiveTextForeground(r._inactiveTextForeground),
		_interior(r._interior),
		_selectionBackground(r._selectionBackground),
		_selectionForeground(r._selectionForeground),
		_textBackground(r._textBackground),
		_textForeground(r._textForeground)
{
}

DefaultThemeFlat& DefaultThemeFlat::operator=(const DefaultThemeFlat& r)
{
	if (this != &r)
	{
		_DefaultTheme::operator=(r);
		_frame = r._frame;
		_inactiveTextBackground = r._inactiveTextBackground;
		_inactiveTextForeground = r._inactiveTextForeground;
		_interior = r._interior;
		_selectionBackground = r._selectionBackground;
		_selectionForeground = r._selectionForeground;
		_textBackground = r._textBackground;
		_textForeground = r._textForeground;
	}
	return *this;
}

IMPL(_frame, frame)
IMPL(_inactiveTextBackground, inactive_text_background)
IMPL(_inactiveTextForeground, inactive_text_foreground)
IMPL(_interior, interior)
IMPL(_selectionBackground, selection_background)
IMPL(_selectionForeground, selection_foreground)
IMPL(_textBackground, text_background)
IMPL(_textForeground, text_foreground)

#ifdef	_DEBUG
void DefaultThemeFlat::dump(const std::string& i) const
{
	std::string sublevel(i);
	sublevel += "  ";
	
	std::string level(i);
	level += "    ";

	dump_object(i, "yacg::DefaultThemeFlat", this);	

	dump_color(sublevel, "Frame", _frame);
	dump_color(sublevel, "Inactive Text Background", _inactiveTextBackground);
	dump_color(sublevel, "Inactive Text Foreground", _inactiveTextForeground);
	dump_color(sublevel, "Interior", _interior);
	dump_color(sublevel, "Selection Background", _selectionBackground);
	dump_color(sublevel, "Selection Foreground", _selectionForeground);
	dump_color(sublevel, "Text Background", _textBackground);
	dump_color(sublevel, "Text Foreground", _textForeground);
	
	_DefaultTheme::dump(level);
}
#endif

void DefaultThemeFlat::reset()
{
	_DefaultTheme::reset();
	RESET_ALL(RESET);
}

void DefaultThemeFlat::reset8()
{
	_DefaultTheme::reset8();
	RESET_ALL(RESET8);
}

void DefaultThemeFlat::reset15()
{
	_DefaultTheme::reset15();
	RESET_ALL(RESET15);
}

void DefaultThemeFlat::reset16()
{
	_DefaultTheme::reset16();
	RESET_ALL(RESET16);
}

void DefaultThemeFlat::reset24()
{
	_DefaultTheme::reset24();
	RESET_ALL(RESET24);
}

void DefaultThemeFlat::reset32()
{
	_DefaultTheme::reset32();
	RESET_ALL(RESET32);
}

#undef	IMPL
#undef	_RESET
#undef	RESET
#undef	RESET8
#undef	RESET15
#undef	RESET16
#undef	RESET24
#undef	RESET32
#undef	RESET_ALL
