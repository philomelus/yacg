#ifndef	__YACG_CHECKBOX_HPP__
#define	__YACG_CHECKBOX_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
//=============================================================================
// Forward declarations
//=============================================================================

	/// \if DEVELOPMENT
	namespace CheckboxImpl
	{
		class Base;
	}
	/// \endif

//=============================================================================
// class Checkbox declaration
//=============================================================================

	// Implements a %checkbox control.  A %checkbox represents a true/false
	// type of "boolean" value.  Typically, they are used to represent a simple
	// on/of option.  This particular implementation of a %checkbox also supports
	// a third value, entitled "gray", that represents neither true nor false.
	class Checkbox : public _ControlEx
	{
	public:
		/// Callback prototype for %checkbox value change.  This callback is
		/// executed when the value of the checkbox changes, regardless of how
		/// the change was made (i.e. even if the code that created the checkbox
		/// sets the value, the callback will get called if the value is different).
		/// \note One can save setup code by setting the value at setup, so that
		/// the code that deals with value changes while the control is displayed
		/// takes care of it at setup time as well.
		/// \param c Reference to Checkbox object who's value was changed.
		/// \param d Caller defined data, passed as the \p d parameter to \c callback.
		/// \param bmp The BITMAP the Checkbox is painted on.
		typedef void (*CALLBACK)(Checkbox& c, void* d, BITMAP* bmp);
	
	public:
		/// \name States
		/// These constants determine the number of states the %checkbox can hold.
		/// Combine one of these constants with other constants and pass in the
		/// \p f parameter of the constructor to set the number of states the
		/// %checkbox can represent.
		/// \{
		
		/// %Checkbox will have 2 states (default).
		static const int s_boolean	= 0x00000000;
		/// %Checkbox will have 3 states.
		static const int s_tristate	= 0x00010000;
		/// Constant used to extract state option from flags.
		static const int s_mask		= 0x00010000;
		/// \}
		
		
		/// \name Title
		/// These constants determine the location of the title of the %checkbox.
		/// Combine one of these constants with other constants and pass in the
		/// \p f parameter of the constructor to specify the location of the title.
		/// \{
		
		/// Title will apper to right of button (default).
		static const int t_righttitle	= 0x00000000;
		/// Title will appear to left of button.
		static const int t_lefttitle	= 0x00020000;
		/// Constant used to extract title option from flags.
		static const int t_mask			= 0x00020000;
		/// \}

		
		/// \name Style
		/// These constants determine the style of the button of the %checkbox.
		/// Combine one of these constants with other constants and pass in the
		/// \p f parameter of the constructor to set the style of the button.
		/// \{
		
		/// Button is a square box.
		static const int b_box				= 0x00000000;
		/// Button is a push button, with title inside.
		static const int b_button			= 0x00040000;
		/// Button is a push button, with title to right (or left if t_lefttile is used).
		static const int b_buttonoutside	= 0x00080000;
		/// Constant used to extract button option from flags.
		static const int b_mask				= 0x000C0000;
		/// \}
		
		/// Represents possible values of a %checkbox.  Because \c unchecked is
		/// zero (and guaranteed to stay zero) the c++ boolean based conditionals
		/// work:  <tt> if (!%checkbox.state()) </tt> means if checkbox is \b not
		/// checked \b or grayed).
		enum STATE
		{
			/// State of %checkbox is unchecked (false, off, disabled, etc.).
			unchecked = 0,
			/// State of %checkbox is checked (true, on, enabled, etc.).
			checked = 1,
			/// State of %checkbox is indeterminate, aliased as grayed.
			gray = 2
		};
		
	public:
		/// Public constructor
		/// \param m _Manager that will control this control.
		/// \param x Left coordinate of control.
		/// \param y Top coordinate of control.
		/// \param w Width of control in pixels.
		/// \param h Height of control in pixels.
		/// \param f Option flags.
		/// \param title Pointer to title string (optional).
		Checkbox(_Manager& m, int x, int y, int w, int h, int f, const char* title = 0);

		/// Destructor.
		~Checkbox();

		/// Sets the current value of the control.
		/// \param v STATE enum representing the desired value of the %checkbox.
		Checkbox& operator=(STATE v);
		
		/// Sets the current value of the control.
		/// \param v true is translated to \c checked, and false is translated to \c unchecked.
		Checkbox& operator=(bool v);
		
		/// Retreives the current value of the control.
		/// \return \c STATE enum representing value of %checkbox.
		operator STATE() const;

		/// Retreives the current value of the control.
		/// \return Returns true if value is \c checked or \c gray.
		bool operator !() const;
		
		/// Retreives the vertical coordinate of the bottom line of the control.
		/// \return Vertical coordinate of bottom of control.
		int bottom() const;
		
		/// Retreives the current callback and user defined data.
		/// \return std::pair containing current callback and user data.		
		std::pair<CALLBACK, void*> callback() const;

		/// Sets the current callback and user defined data.
		/// \param f Function to call when %checkbox value has changed.
		/// \param d Caller defined data, passed verbatim to callback (optional).
		void callback(CALLBACK f, void* d = 0);
		
		/// Retreives the height of the control in pixels.
		/// \return Height of the control in pixels.
		int height() const;

		/// Retreives the horizontal coordinate of the left side of control.
		/// \return Horizontal coordinate of the left side of control.
		int left() const;

		/// Sets the horizontal coordinate of the left side of the control.
		/// \param l New horizontal coordinate of the left side of the control.
		void left(int l);
		
		/// Retreives the horizontal coordinate of the right side of control.
		/// \return Horizontal coordinate of the right side of control.
		int right() const;
		
		/// Retreives the title of the control.
		/// \return Pointer to zero terminated array of char's.
		const char* title() const;

		/// Sets the title of the control.
		/// \param t Pointer to zero terminated array of char's.
		void title(const char* t);
		
		/// Retreives a reference to the std::string object used to hold the
		/// title of the control.
		/// \return Const reference to std::string object.
		const std::string& title_object() const;

		/// Retreives the vertical coordinate of the top of the control.
		/// \return Vertical coordinate of the top of the control.
		int top() const;

		/// Sets the vertical coordinate of the top of the control.
		/// \param t New vertical coordinate of the top of the control.
		void top(int t);
		
		/// Retreives the current value of the control.
		/// \return \c STATE enum representing value of %checkbox.
		STATE value() const;

		/// Sets the current value of the control.
		/// \param s STATE enum representing the desired value of the %checkbox.
		void value(STATE s);
		
		/// Retreives the width of the control in pixels.
		/// \return Width of the control.
		int width() const;

	public:
		/// \if DEVELOPMENT
		void active_changed(bool o, bool n);
		void flags_changed(int o, int n);
		void paint(BITMAP* b);
		bool mouse_down(BITMAP* b, int x, int y, int m);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		CheckboxImpl::Base* _impl;
		/// \endif
	};
}

#endif
