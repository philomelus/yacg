/// \file Scroll.hpp
/// Declaration of Scroll control.

#ifndef	__YACG_SCROLL_HPP__
#define	__YACG_SCROLL_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_THEME_HPP__
#include <yacg/Theme.hpp>
#endif

namespace yacg
{
//=============================================================================
// Forward declarations
//=============================================================================

	/// \if DEVELOPMENT
	namespace ScrollImpl
	{
		class Base;
	}
	/// \endif

//=============================================================================
// class ScrollTheme declaration
//=============================================================================

	class ScrollTheme : public ThemeBase
	{
	public:
		const char* name() const;

		
	private:
		static const char* _name;
	};

//=============================================================================
// class Scroll declaration
//=============================================================================

	//=========================================================================
	// Implements a %scroll control (sometimes called a %Scroll Bar).
	// A %scroll control represents a range of values.  Typically used to
	// give an idea of the range of something that is being viewed or controlled
	// in some way.  For example, the range of lines in a text file being viewed.
	
	class Scroll : public _ControlEx
	{
	public:
		//=====================================================================
		/// Callback prototype.  The callback will be called when the setting of
		/// the %scroll changes.
		/// \param s %scroll control who's setting changed.
		/// \param d User provided data (passed as second parameter of action funtion).
		/// \param b Pointer to BITMAP that %scroll is displayed on.
		/// \param prev Previous setting of %scroll.
		/// \param cur New setting of %scroll.
		typedef void (*CALLBACK)(Scroll& s, void* d, BITMAP* b, int prev, int cur);

	public:
		//=====================================================================
		/// \name	Type
		/// These constants determine the orientation of the %scroll.  Combine these
		/// constants with a Frame constant and pass in the in the \p f parameter of
		/// the constructor to set the orientation of the control.
		/// \{
		
		/// %Scroll is oriented horizontally (default).
		static const int t_horizontal	= 0x00000000;
		/// %Scroll is oriented vertically.
		static const int t_vertical		= 0x00010000;
		/// Constant used to extract type option from flags.
		static const int t_mask			= 0x00010000;
		/// \}
		
		//=====================================================================
		/// \name	Frame
		/// These constants determine the frame type of a %scroll.  Combine these
		/// constants with a Type constant and pass in the \p f parameter of the
		/// constructor to set the whether the control displays a frame.
		/// \note	o_frame is \b always on for a STYLE_FLAT theme.
		/// \{
		
		/// %Scroll doesn't display frame outside control area (default).
		static const int o_noframe		= 0x00000000;
		/// %Scroll will have a single pixel frame.  Forced on for STYLE_FLAT theme.
		static const int o_frame		= 0x00020000;
		/// Constant used to extract frame option from flags.
		static const int o_mask			= 0x00020000;
		/// \}
	
		//=====================================================================
		/// \name	Constants
		/// These constants are for use by code using a %scroll.  When creating or
		/// using a %scroll, the constructor is passed the width (horizontal) or
		/// height (vertical), and these constants can be used to determine the
		/// height (horizontal) or width (vertical) of the other axis.
		/// \note	The values may change in future revisions, but the names will
		///			remain the same.
		/// \{
		
		/// Width of vertical %scroll.
		static const int h_size = 16;
		/// Height of horiztonal %scroll.
		static const int v_size = 16;
		/// Width of vertical %scroll when framed.
		static const int h_size_framed = 18;
		/// Height of horizontal %scroll when framed.
		static const int v_size_framed = 18;
		/// \}

	public:
		//=====================================================================
		/// Public constructor
		/// \param m _Manager (or derived) that will control this %scroll.
		/// \param x Left coordinate of control.
		/// \param y Top coordinate of control.
		/// \param wh The width (of horizontal) or height (of vertical) of control.
		/// \param f Option flags.
		/// \param r Initial range of control.
		/// \param s Initial setting of control.
		Scroll(_Manager& m, int x, int y, int wh, int f, int r = 1, int s = 0);
		
		/// Public destructor.
		~Scroll();

		//=====================================================================
		/// Retreives the current setting of the control.
		/// \return Current setting.
		operator int() const;
		
		/// Sets the current setting of the control.
		/// \param s New setting.
		Scroll& operator=(int s);
		
		//=====================================================================
		/// Retreives the vertical coordinate of the bottom line of the control.
		/// \return Vertical coordinate of bottom of control.
		int bottom() const;

		//=====================================================================
		/// Retreives the current callback and user defined data.
		/// \return std::pair containing current callback and user data.		
		std::pair<CALLBACK, void*> callback() const;
		
		/// Sets the current setting changed callback and (optional) user defined data.
		/// \param f Function to call when %scroll setting has changed.
		/// \param d Optional caller defined data, passed verbatim to callback.
		void callback(CALLBACK f, void* d = 0);

		//=====================================================================
		/// Retreives the initial and repeat delays.
		/// \return std::pair of int's, where \p first is initial delay, and \p second is repeat delay.
		std::pair<int, int> delay() const;
		
		/// Sets the initial and repeat delays.
		/// Before repeating a command (line down, page down, etc.), at
		/// least \p initial delay must pass, and then the command will repeat
		/// every \p repeat delays.  The period is specified in vbl, so see the
		/// Allegro docs for the specifics.
		/// \note Don't count on a value meaning any particular period.  You have
		/// to adjust for the current definition of the vbl.  It's \b vital that
		/// you read the Allegro documentation for the vbl to understand how to
		/// adjust this value, \b especially if your program is cross-platform.
		/// \param initial Initial waiting period.
		/// \param repeat Repeat waiting period.
		void delay(int initial, int repeat);

		//=====================================================================
		/// Retreives the height of the control in pixels.
		/// \return Height of the control in pixels.
		int height() const;
		
		//=====================================================================
		/// Retreives the horizontal coordinate of the left side of control.
		/// \return Horizontal coordinate of the left side of control.
		int left() const;
		
		/// Sets the horizontal coordinate of the left side of the control.
		/// \param l New horizontal coordinate of the left side of the control.
		void left(int l);

		//=====================================================================
		/// Retreives the width (horizontal) or height (vertical) of the control.
		/// \return Width/Height of %scroll.
		int length() const;
		
		/// Sets the width (horizontal) or height (vertical) of the control.
		/// \param l New Width/Height of %scroll.
		void length(int l);

		//=====================================================================
		/// Retreives the value of the line update.  The line update value is used
		/// to increase or decrease the setting by a line, which is done by pushing
		/// one of the arrows.  Default value is 1.
		/// \return Value to modify setting by line.
		int line() const;
		
		/// Sets the value of the line update.  The line update value is used to
		/// increase or decrease the setting by a line, which is done by pushing
		/// one of the arrows.  Default value is 1.
		/// \param l New line value.
		void line(int l);
		
		//=====================================================================
		/// Retreives the value of the page update.  The page update value is used
		/// to increase or decrease the setting by a page, which is done by clicking
		/// in an area other than the thumb or arrows.  Default is 3.
		/// \return Value to modify setting by line.
		int page() const;

		/// Sets the value of the page update.  The page update value is used to
		/// increase or decrease the setting by a page, which is done by clicking
		/// in an area other than the thumb or arrows.
		/// \param p New page value.
		void page(int p);

		//=====================================================================
		/// Retreives the range of the %scroll.  The range of a %scroll is from
		/// 0 to the range - 1, and is capped in both directions to the range
		/// (i.e. < 0 is made 0, > range - 1 is changed to range - 1).
		/// \return The upper limit of the range + 1
		int range() const;

		/// Sets the range of the %scroll.  The range of a %scroll is from 0 to
		/// the range - 1, and is capped in both directions to the range (i.e.
		/// < 0 is made 0, > range - 1 is changed to range - 1).
		/// \param r New upper limit of the range + 1
		void range(int r);
		
		//=====================================================================
		/// Retreives the horizontal coordinate of the right side of control.
		/// \return Horizontal coordinate of the right side of control.
		int right() const;

		//=====================================================================
		/// Retreives the current setting of the control.
		/// \return Current setting.
		int setting() const;
		
		/// Sets the current setting of the control.
		/// \param s New setting.
		void setting(int s);

		//=====================================================================
		/// Retreives the vertical coordinate of the top of the control.
		/// \return Vertical coordinate of the top of the control.
		int top() const;
		
		/// Sets the vertical coordinate of the top of the control.
		/// \param t New vertical coordinate of the top of the control.
		void top(int t);

		//=====================================================================
		/// Retreives the width of the control in pixels.
		/// \return Width of the control.
		int width() const;
		
	public:
		//=====================================================================
		/// \if DEVELOPMENT
		
		/// \copydoc void yacg::_Control::paint(BITMAP* b)
		void paint(BITMAP* b);
		
		/// \copydoc bool yacg::_ControlEx::mouse_down(BITMAP* b, int x, int y, int m)
		bool mouse_down(BITMAP* b, int x, int y, int m);
		
#ifdef	_DEBUG
		/// \copydoc void yacg::_Control::dump() const
		void dump() const;
#endif

	private:
		ScrollImpl::Base* _impl;
		/// \endif
	};
}

#endif
