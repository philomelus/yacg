#ifndef	__YACG_THEME_INL__
#define	__YACG_THEME_INL__

namespace yacg
{
//=============================================================================
// class Theme color access functions
//=============================================================================

	//-------------------------------------------------------------------------

	inline int AQUA(Theme* t)
	{
		return t->_AQUA;
	}

	inline int AQUA_DARK(Theme* t)
	{
		return t->_AQUA_DARK;
	}
	
	inline int AQUA_LIGHT(Theme* t)
	{
		return t->_AQUA_LIGHT;
	}
	
	inline int AQUA_VERY_DARK(Theme* t)
	{
		return t->_AQUA_VERY_DARK;
	}
	
	inline int AQUA_VERY_LIGHT(Theme* t)
	{
		return t->_AQUA_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------

	inline int BLACK(Theme* t)
	{
		return t->_BLACK;
	}

	//-------------------------------------------------------------------------

	inline int BLUE(Theme* t)
	{
		return t->_BLUE;
	}

	inline int BLUE_DARK(Theme* t)
	{
		return t->_BLUE_DARK;
	}
	
	inline int BLUE_LIGHT(Theme* t)
	{
		return t->_BLUE_LIGHT;
	}
	
	inline int BLUE_VERY_DARK(Theme* t)
	{
		return t->_BLUE_VERY_DARK;
	}
	
	inline int BLUE_VERY_LIGHT(Theme* t)
	{
		return t->_BLUE_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------
	
	inline int GRAY(Theme* t)
	{
		return t->_GRAY;
	}
	
	inline int GRAY_DARK(Theme* t)
	{
		return t->_GRAY_DARK;
	}
	
	inline int GRAY_LIGHT(Theme* t)
	{
		return t->_GRAY_LIGHT;
	}
	
	inline int GRAY_VERY_DARK(Theme* t)
	{
		return t->_GRAY_VERY_DARK;
	}
	
	inline int GRAY_VERY_LIGHT(Theme* t)
	{
		return t->_GRAY_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------
	
	inline int GREEN(Theme* t)
	{
		return t->_GREEN;
	}
	
	inline int GREEN_DARK(Theme* t)
	{
		return t->_GREEN_DARK;
	}
	
	inline int GREEN_LIGHT(Theme* t)
	{
		return t->_GREEN_LIGHT;
	}
	
	inline int GREEN_VERY_DARK(Theme* t)
	{
		return t->_GREEN_VERY_DARK;
	}
	
	inline int GREEN_VERY_LIGHT(Theme* t)
	{
		return t->_GREEN_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------
	
	inline int PURPLE(Theme* t)
	{
		return t->_PURPLE;
	}
	
	inline int PURPLE_DARK(Theme* t)
	{
		return t->_PURPLE_DARK;
	}
	
	inline int PURPLE_LIGHT(Theme* t)
	{
		return t->_PURPLE_LIGHT;
	}
	
	inline int PURPLE_VERY_DARK(Theme* t)
	{
		return t->_PURPLE_VERY_DARK;
	}
	
	inline int PURPLE_VERY_LIGHT(Theme* t)
	{
		return t->_PURPLE_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------
	
	inline int RED(Theme* t)
	{
		return t->_RED;
	}
	
	inline int RED_DARK(Theme* t)
	{
		return t->_RED_DARK;
	}
	
	inline int RED_LIGHT(Theme* t)
	{
		return t->_RED_LIGHT;
	}
	
	inline int RED_VERY_DARK(Theme* t)
	{
		return t->_RED_VERY_DARK;
	}
	
	inline int RED_VERY_LIGHT(Theme* t)
	{
		return t->_RED_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------
	
	inline int YELLOW(Theme* t)
	{
		return t->_YELLOW;
	}
	
	inline int YELLOW_DARK(Theme* t)
	{
		return t->_YELLOW_DARK;
	}
	
	inline int YELLOW_LIGHT(Theme* t)
	{
		return t->_YELLOW_LIGHT;
	}
	
	inline int YELLOW_VERY_DARK(Theme* t)
	{
		return t->_YELLOW_VERY_DARK;
	}
	
	inline int YELLOW_VERY_LIGHT(Theme* t)
	{
		return t->_YELLOW_VERY_LIGHT;
	}
	
	//-------------------------------------------------------------------------
	
	inline int WHITE(Theme* t)
	{
		return t->_WHITE;
	}

//=============================================================================
// class Theme inline implementation
//=============================================================================

	template <typename T>
	inline T& Theme::at(TYPE t)
	{
		types::iterator i = _style->second->find(t);
		if (i == _style->second->end())
			throw std::logic_error("yacg::Configuration::at(TYPE t)");
#ifdef	_DEBUG
		return *dynamic_cast<T*>(i->second.item);
#else
		return *static_cast<T*>(i->second.item);
#endif
	}
}

#endif
