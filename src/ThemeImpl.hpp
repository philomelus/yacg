#ifndef	__THEMEIMPL_HPP__
#define	__THEMEIMPL_HPP__

//=============================================================================
// macro _THEMEFUNC: Implement theme color access functions.
//=============================================================================

#define	_THEMEFUNC(cla, var, func)			\
int cla##::##func##() const					\
{											\
	return var##;							\
}											\
void cla##::##func##(int c)					\
{											\
	if (var## != c)							\
	{										\
		var## = c;							\
		modified();							\
	}										\
}											\
void cla##::##func##(int r, int g, int b)	\
{											\
	func##(_ThemeItem::makecol(r, g, b));	\
}

//=============================================================================
// macro CONVERT_COLOR: For _ThemeItem derived classes, convert a color from
// one specified format to internal format of object.
//=============================================================================

#define	CONVERT_COLOR(var, type)	var## = convert(old, var##, type##)

#endif
