#ifndef	__YACG_THEME_HPP__
#define	__YACG_THEME_HPP__

#ifndef	__STD_MAP__
#define	__STD_MAP__
#include <map>
#endif

#ifndef	__STD_LIMITS__
#define	__STD_LIMITS__
#include <limits>
#endif

#ifndef	__BOOST_SIGNALS_HPP__
#define	__BOOST_SIGNALS_HPP__
#define	BOOST_SIGNALS_STATIC_LINK
#include <boost/signals.hpp>
#endif

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

namespace yacg
{
	class _DefaultTheme;
	class _ThemeItem;
	class Theme;
	class DefaultTheme3D;
	class DefaultThemeFlat;
	
//=============================================================================
// class _ThemeItem
//=============================================================================

	class _ThemeItem
	{
	//-------------------------------------------------------------------------
	public:
		enum FORMAT
		{
			none,
			eight,
			palette = eight,
			fifteen,
			sixteen,
			twentyfour,
			thirtytwo
		};
	
	//-------------------------------------------------------------------------
	public:
		_ThemeItem(FORMAT f);
		
		_ThemeItem(const _ThemeItem& o);
		
		_ThemeItem& operator=(const _ThemeItem& o);
		
		virtual ~_ThemeItem();

	//-------------------------------------------------------------------------
	private:
		typedef boost::signal<void (_ThemeItem* c, int h)> modified_slot;
		typedef modified_slot::slot_type MODIFIED_EVENT;
		
	public:
		EVENTID connect(MODIFIED_EVENT f);

		void disconnect(EVENTID f);

	protected:
		virtual void modified(int h = 0);

	private:	
		modified_slot _handlers;
	
	//-------------------------------------------------------------------------
	public:
		virtual void reset() = 0;
		
		virtual void reset8() = 0;

		virtual void reset15() = 0;

		virtual void reset16() = 0;

		virtual void reset24() = 0;

		virtual void reset32() = 0;
		
	//-------------------------------------------------------------------------
	public:
		int convert(FORMAT f, int c);
		
		int convert(FORMAT f, int c, FORMAT n);

		int makecol(int r, int g, int b);

	//-------------------------------------------------------------------------
	public:
#ifdef	_DEBUG
		virtual void dump() const;
#endif

	//-------------------------------------------------------------------------
	protected:
		FORMAT format() const;
		
		void format(FORMAT f);
	
	private:
		FORMAT _format;
	};

//=============================================================================
// class _DefaultTheme
//=============================================================================

	class _DefaultTheme : public _ThemeItem
	{
	//-------------------------------------------------------------------------
	public:
		_DefaultTheme();

		_DefaultTheme(const _DefaultTheme& o);
		
		_DefaultTheme& operator=(const _DefaultTheme& o);

	//-------------------------------------------------------------------------
	public:
		FONT* font() const;
		
		void font(FONT* f);
	
	private:
		FONT* _font;
	
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
		virtual void dump() const;
#endif
	};

//=============================================================================
// class DefaultTheme3D
//=============================================================================

	class DefaultTheme3D : public _DefaultTheme
	{
	//-------------------------------------------------------------------------
	public:
		DefaultTheme3D();

		DefaultTheme3D(const DefaultTheme3D& o);
		
		DefaultTheme3D& operator=(const DefaultTheme3D& o);
	
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
		int frame_light() const;
		
		void frame_light(int c);

		void frame_light(int r, int g, int b);

	private:	
		int _frameLight;
		
	//-------------------------------------------------------------------------
	public:
		int frame_lightest() const;
		
		void frame_lightest(int c);

		void frame_lightest(int r, int g, int b);

	private:
		int _frameLightest;
		
	//-------------------------------------------------------------------------
	public:
		int inactive_text_background() const;
		
		void inactive_text_background(int c);
		
		void inactive_text_background(int r, int g, int b);

	private:
		int _inactiveTextBackground;
		
	//-------------------------------------------------------------------------
	public:
		int inactive_text_foreground() const;
		
		void inactive_text_foreground(int c);
		
		void inactive_text_foreground(int r, int g, int b);

	private:
		int _inactiveTextForeground;

	//-------------------------------------------------------------------------
	public:
		int interior() const;
		
		void interior(int c);
		
		void interior(int r, int g, int b);

	private:		
		int _interior;

	//-------------------------------------------------------------------------
	public:
		int selection_background() const;
		
		void selection_background(int c);

		void selection_background(int r, int g, int b);

	private:		
		int _selectionBackground;

	//-------------------------------------------------------------------------
	public:
		int selection_foreground() const;
		
		void selection_foreground(int c);

		void selection_foreground(int r, int g, int b);

	private:
		int _selectionForeground;
		
	//-------------------------------------------------------------------------
	public:
		int text_background() const;
		
		void text_background(int c);

		void text_background(int r, int g, int b);

	private:
		int _textBackground;

	//-------------------------------------------------------------------------
	public:
		int text_foreground() const;
		
		void text_foreground(int c);

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
		virtual void dump() const;
#endif
	};
	
//=============================================================================
// class DefaultThemeFlat
//=============================================================================

	class DefaultThemeFlat : public _DefaultTheme
	{
	//-------------------------------------------------------------------------
	public:
		DefaultThemeFlat();
		
		DefaultThemeFlat(const DefaultThemeFlat& o);

		DefaultThemeFlat& operator=(const DefaultThemeFlat& o);
				
	//-------------------------------------------------------------------------
	public:
		int frame() const;
		
		void frame(int c);
		
		void frame(int r, int g, int b);
		
	private:
		int _frame;
		
	//-------------------------------------------------------------------------
	public:
		int inactive_text_background() const;
		
		void inactive_text_background(int c);
		
		void inactive_text_background(int r, int g, int b);

	private:
		int _inactiveTextBackground;
		
	//-------------------------------------------------------------------------
	public:
		int inactive_text_foreground() const;
		
		void inactive_text_foreground(int c);
		
		void inactive_text_foreground(int r, int g, int b);

	private:
		int _inactiveTextForeground;

	//-------------------------------------------------------------------------
	public:
		int interior() const;
		
		void interior(int c);
		
		void interior(int r, int g, int b);

	private:		
		int _interior;

	//-------------------------------------------------------------------------
	public:
		int selection_background() const;
		
		void selection_background(int c);

		void selection_background(int r, int g, int b);

	private:		
		int _selectionBackground;

	//-------------------------------------------------------------------------
	public:
		int selection_foreground() const;
		
		void selection_foreground(int c);

		void selection_foreground(int r, int g, int b);

	private:
		int _selectionForeground;
		
	//-------------------------------------------------------------------------
	public:
		int text_background() const;
		
		void text_background(int c);

		void text_background(int r, int g, int b);

	private:
		int _textBackground;

	//-------------------------------------------------------------------------
	public:
		int text_foreground() const;
		
		void text_foreground(int c);

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
		virtual void dump() const;
#endif
	};

//=============================================================================
// class Theme
//=============================================================================

	class Theme
	{
	//-------------------------------------------------------------------------
	public:
		typedef _ThemeItem::FORMAT FORMAT;
			
	//-------------------------------------------------------------------------
	public:
		typedef unsigned short STYLE;

		static const STYLE STYLE_FLAT			= 0;

		static const STYLE STYLE_3D				= STYLE_FLAT + 1;

		static const STYLE STYLE_BITMAP			= STYLE_3D + 1;

		static const STYLE STYLE_USER			= 16384;

		static const STYLE STYLE_USEREND		= std::numeric_limits<STYLE>::max;
	
	//-------------------------------------------------------------------------
	// types
	public:
		typedef unsigned short TYPE;
		
		static const TYPE TYPE_DEFAULT		= 0;
		static const TYPE TYPE_BITMAPBUTTON	= TYPE_DEFAULT + 1;
		static const TYPE TYPE_BOX			= TYPE_BITMAPBUTTON + 1;
		static const TYPE TYPE_BUTTON		= TYPE_BOX + 1;
		static const TYPE TYPE_CHECKBOX		= TYPE_BUTTON + 1;
		static const TYPE TYPE_CHECKEDBOX	= TYPE_CHECKBOX + 1;
		static const TYPE TYPE_DIALOG		= TYPE_CHECKEDBOX + 1;
		static const TYPE TYPE_EDITBOX		= TYPE_DIALOG + 1;
		static const TYPE TYPE_LIST			= TYPE_EDITBOX + 1;
		static const TYPE TYPE_MENU			= TYPE_LIST + 1;
		static const TYPE TYPE_PLANE		= TYPE_MENU + 1;
		static const TYPE TYPE_RADIO		= TYPE_PLANE + 1;
		static const TYPE TYPE_RADIOGROUP	= TYPE_RADIO + 1;
		static const TYPE TYPE_SELECTIONBOX	= TYPE_RADIOGROUP + 1;
		static const TYPE TYPE_SLIDER		= TYPE_SELECTIONBOX + 1;
		static const TYPE TYPE_TAB			= TYPE_SLIDER + 1;
		static const TYPE TYPE_TABPAGE		= TYPE_TAB + 1;
		static const TYPE TYPE_TEXTBOX		= TYPE_TABPAGE + 1;
		static const TYPE TYPE_TITLE		= TYPE_TEXTBOX + 1;
		static const TYPE TYPE_WINDOW		= TYPE_TITLE + 1;

		static const TYPE TYPE_USER = 16384;
		
		static const TYPE TYPE_USEREND = std::numeric_limits<TYPE>::max;
	
	//-------------------------------------------------------------------------
	public:
		struct type_data
		{
			EVENTID connection;
			_ThemeItem* item;
		};
		typedef std::map<TYPE, type_data> types;
		typedef std::map<STYLE, types*> container;
	
	//-------------------------------------------------------------------------
	public:
		class iterator;
		
	//-------------------------------------------------------------------------
	public:
		class const_iterator
				: public std::iterator<std::forward_iterator_tag, const _ThemeItem>
		{
		//---------------------------------------------------------------------
		public:
			const_iterator();
			
			const_iterator(const const_iterator& i);

			const_iterator(const iterator& i);
			
			const_iterator& operator=(const const_iterator& i);
			
			const _ThemeItem& operator*();

			const _ThemeItem* operator->();

			const_iterator& operator++();

			const_iterator operator++(int);
		
		//---------------------------------------------------------------------
		private:
			Theme::container::const_iterator _style;
			
			Theme::types::const_iterator _type;

			const Theme* _theme;
			
			friend Theme;
			friend bool operator==(const Theme::iterator& l, const Theme::const_iterator& r);
			friend bool operator==(const Theme::const_iterator& l, const Theme::iterator& r);
			friend bool operator==(const Theme::const_iterator& l, const Theme::const_iterator& r);
			friend bool operator!=(const Theme::iterator& l, const Theme::const_iterator& r);
			friend bool operator!=(const Theme::const_iterator& l, const Theme::iterator& r);
			friend bool operator!=(const Theme::const_iterator& l, const Theme::const_iterator& r);
		};
		
	//-------------------------------------------------------------------------
	public:
		class iterator
				: public std::iterator<std::forward_iterator_tag, _ThemeItem>
		{
		//---------------------------------------------------------------------
		public:
			iterator();
			
			iterator(const iterator& i);
			
			iterator& operator=(const iterator& i);
			
			_ThemeItem& operator*();
			
			_ThemeItem* operator->();

			iterator& operator++();

			iterator operator++(int);
		
		//---------------------------------------------------------------------
		private:
			Theme* _theme;
			
			Theme::container::iterator _style;
			
			Theme::types::iterator _type;

			friend Theme;
			friend const_iterator;
			friend bool operator==(const Theme::iterator& l, const Theme::iterator& r);
			friend bool operator==(const Theme::iterator& l, const Theme::const_iterator& r);
			friend bool operator==(const Theme::const_iterator& l, const Theme::iterator& r);
			friend bool operator!=(const Theme::iterator& l, const Theme::iterator& r);
			friend bool operator!=(const Theme::iterator& l, const Theme::const_iterator& r);
			friend bool operator!=(const Theme::const_iterator& l, const Theme::iterator& r);
		};

	//-------------------------------------------------------------------------
	public:
		Theme(FORMAT f = _ThemeItem::none);
		
		virtual ~Theme();

		Theme(const Theme& o);
		
		Theme& operator=(const Theme& o);
		
	private:
		void copy_theme(const container::value_type& i);
		
		void copy_types(const types::value_type& i, STYLE s);

		void delete_styles(Theme::container::value_type& i);

		void disconnect_type(Theme::types::value_type& i);

	//-------------------------------------------------------------------------
	public:
		template <typename T> T& at(TYPE t);
		
	//-------------------------------------------------------------------------
	public:
		iterator begin();
		
		const_iterator begin() const;
		
	//-------------------------------------------------------------------------
	public:
#ifdef	_DEBUG
		virtual void dump() const;
#endif

	//-------------------------------------------------------------------------
	public:
		bool empty() const;

	//-------------------------------------------------------------------------
	public:
		void erase(iterator i);
		
	//-------------------------------------------------------------------------
	public:
		iterator end();

		const_iterator end() const;
		
	//-------------------------------------------------------------------------
	public:
		iterator find(TYPE t);
		
	//-------------------------------------------------------------------------
	public:
		FORMAT format() const;
		
		void format(FORMAT f);
		
	private:
		FORMAT _format;
		
	//-------------------------------------------------------------------------
	public:
		iterator insert(TYPE y, _ThemeItem& ti);

		iterator insert(STYLE s, TYPE t, _ThemeItem& ti);

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
		size_t size() const;
		
		size_t size(STYLE s) const;
	
	//-------------------------------------------------------------------------
	public:
		STYLE style() const;
		
		void style(STYLE s);


	private:		
		container::iterator _style;

	//-------------------------------------------------------------------------
	private:
		typedef boost::signal<void (Theme* c)> modified_slot;
		
		typedef modified_slot::slot_type MODIFIED_EVENT;
		
	public:
		EVENTID connect(MODIFIED_EVENT f);

		void disconnect(EVENTID f);

	protected:
		virtual void modified();

	private:	
		modified_slot _handlers;
	
	private:
		void item_modified(_ThemeItem* i, int h);
	
	//-------------------------------------------------------------------------
	private:
		container _styles;

		DefaultThemeFlat _flat;
		
		DefaultTheme3D _3d;

	//-------------------------------------------------------------------------
	private:
		int _AQUA;
		int _AQUA_DARK;
		int _AQUA_LIGHT;
		int _AQUA_VERY_DARK;
		int _AQUA_VERY_LIGHT;
		int _BLACK;
		int _BLUE;
		int _BLUE_DARK;
		int _BLUE_LIGHT;
		int _BLUE_VERY_DARK;
		int _BLUE_VERY_LIGHT;
		int _GRAY;
		int _GRAY_DARK;
		int _GRAY_LIGHT;
		int _GRAY_VERY_DARK;
		int _GRAY_VERY_LIGHT;		
		int _GREEN;
		int _GREEN_DARK;
		int _GREEN_LIGHT;
		int _GREEN_VERY_DARK;
		int _GREEN_VERY_LIGHT;
		int _PURPLE;
		int _PURPLE_DARK;
		int _PURPLE_LIGHT;
		int _PURPLE_VERY_DARK;
		int _PURPLE_VERY_LIGHT;
		int _RED;
		int _RED_DARK;
		int _RED_LIGHT;
		int _RED_VERY_DARK;
		int _RED_VERY_LIGHT;
		int _YELLOW;
		int _YELLOW_DARK;
		int _YELLOW_LIGHT;
		int _YELLOW_VERY_DARK;
		int _YELLOW_VERY_LIGHT;
		int _WHITE;
		
	//-------------------------------------------------------------------------
		friend int AQUA(Theme* t);
		friend int AQUA_DARK(Theme* t);
		friend int AQUA_LIGHT(Theme* t);
		friend int AQUA_VERY_DARK(Theme* t);
		friend int AQUA_VERY_LIGHT(Theme* t);
		friend int BLACK(Theme* t);
		friend int BLUE(Theme* t);
		friend int BLUE_DARK(Theme* t);
		friend int BLUE_LIGHT(Theme* t);
		friend int BLUE_VERY_DARK(Theme* t);
		friend int BLUE_VERY_LIGHT(Theme* t);
		friend int GRAY(Theme* t);
		friend int GRAY_DARK(Theme* t);
		friend int GRAY_LIGHT(Theme* t);
		friend int GRAY_VERY_DARK(Theme* t);
		friend int GRAY_VERY_LIGHT(Theme* t);
		friend int GREEN(Theme* t);
		friend int GREEN_DARK(Theme* t);
		friend int GREEN_LIGHT(Theme* t);
		friend int GREEN_VERY_DARK(Theme* t);
		friend int GREEN_VERY_LIGHT(Theme* t);
		friend int PURPLE(Theme* t);
		friend int PURPLE_DARK(Theme* t);
		friend int PURPLE_LIGHT(Theme* t);
		friend int PURPLE_VERY_DARK(Theme* t);
		friend int PURPLE_VERY_LIGHT(Theme* t);
		friend int RED(Theme* t);
		friend int RED_DARK(Theme* t);
		friend int RED_LIGHT(Theme* t);
		friend int RED_VERY_DARK(Theme* t);
		friend int RED_VERY_LIGHT(Theme* t);
		friend int YELLOW(Theme* t);
		friend int YELLOW_DARK(Theme* t);
		friend int YELLOW_LIGHT(Theme* t);
		friend int YELLOW_VERY_DARK(Theme* t);
		friend int YELLOW_VERY_LIGHT(Theme* t);
		friend int WHITE(Theme* t);
		
	//-------------------------------------------------------------------------
		friend bool operator==(const Theme::iterator& l, const Theme::iterator& r);
		friend bool operator==(const Theme::iterator& l, const Theme::const_iterator& r);
		friend bool operator==(const Theme::const_iterator& l, const Theme::iterator& r);
		friend bool operator==(const Theme::const_iterator& l, const Theme::const_iterator& r);
		friend bool operator!=(const Theme::iterator& l, const Theme::iterator& r);
		friend bool operator!=(const Theme::iterator& l, const Theme::const_iterator& r);
		friend bool operator!=(const Theme::const_iterator& l, const Theme::iterator& r);
		friend bool operator!=(const Theme::const_iterator& l, const Theme::const_iterator& r);
	};

//=============================================================================
// Namespace Variables
//=============================================================================

	//-------------------------------------------------------------------------
	extern Theme DefaultTheme;
	
	//-------------------------------------------------------------------------
	extern Theme* ActiveTheme;

//=============================================================================
// Namespace Functions
//=============================================================================

	//-------------------------------------------------------------------------
	int AQUA(Theme* t = ActiveTheme);

	int AQUA_DARK(Theme* t = ActiveTheme);
	
	int AQUA_LIGHT(Theme* t = ActiveTheme);
	
	int AQUA_VERY_DARK(Theme* t = ActiveTheme);
	
	int AQUA_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int BLACK(Theme* t = ActiveTheme);

	//-------------------------------------------------------------------------
	int BLUE(Theme* t = ActiveTheme);

	int BLUE_DARK(Theme* t = ActiveTheme);
	
	int BLUE_LIGHT(Theme* t = ActiveTheme);
	
	int BLUE_VERY_DARK(Theme* t = ActiveTheme);
	
	int BLUE_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int GRAY(Theme* t = ActiveTheme);
	
	int GRAY_DARK(Theme* t = ActiveTheme);
	
	int GRAY_LIGHT(Theme* t = ActiveTheme);
	
	int GRAY_VERY_DARK(Theme* t = ActiveTheme);
	
	int GRAY_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int GREEN(Theme* t = ActiveTheme);
	
	int GREEN_DARK(Theme* t = ActiveTheme);
	
	int GREEN_LIGHT(Theme* t = ActiveTheme);
	
	int GREEN_VERY_DARK(Theme* t = ActiveTheme);
	
	int GREEN_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int PURPLE(Theme* t = ActiveTheme);
	
	int PURPLE_DARK(Theme* t = ActiveTheme);
	
	int PURPLE_LIGHT(Theme* t = ActiveTheme);
	
	int PURPLE_VERY_DARK(Theme* t = ActiveTheme);
	
	int PURPLE_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int RED(Theme* t = ActiveTheme);
	
	int RED_DARK(Theme* t = ActiveTheme);
	
	int RED_LIGHT(Theme* t = ActiveTheme);
	
	int RED_VERY_DARK(Theme* t = ActiveTheme);
	
	int RED_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int YELLOW(Theme* t = ActiveTheme);
	
	int YELLOW_DARK(Theme* t = ActiveTheme);
	
	int YELLOW_LIGHT(Theme* t = ActiveTheme);
	
	int YELLOW_VERY_DARK(Theme* t = ActiveTheme);
	
	int YELLOW_VERY_LIGHT(Theme* t = ActiveTheme);
	
	//-------------------------------------------------------------------------
	int WHITE(Theme* t = ActiveTheme);

//=============================================================================
// class Theme helpers
//=============================================================================

	bool operator==(const Theme::iterator& l, const Theme::iterator& r);

	bool operator==(const Theme::iterator& l, const Theme::const_iterator& r);

	bool operator==(const Theme::const_iterator& l, const Theme::iterator& r);

	bool operator==(const Theme::const_iterator& l, const Theme::const_iterator& r);

	bool operator!=(const Theme::iterator& l, const Theme::iterator& r);

	bool operator!=(const Theme::iterator& l, const Theme::const_iterator& r);

	bool operator!=(const Theme::const_iterator& l, const Theme::iterator& r);

	bool operator!=(const Theme::const_iterator& l, const Theme::const_iterator& r);
}

#include <yacg/Theme.inl>

#endif
