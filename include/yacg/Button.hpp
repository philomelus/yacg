#ifndef	__YACG_BUTTON_HPP__
#define	__YACG_BUTTON_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
//=============================================================================
// Forward declarations
//=============================================================================

	/// \if DEVELOPMENT
	namespace ButtonImpl
	{
		class Base;
		class BitmapBase;
	}
	/// \endif

//=============================================================================
// class BitmapButton declaration
//=============================================================================

	class BitmapButton : public _ControlEx
	{
	public:
		/// \brief Callback prototype.
		
		/// TODO:  Callback detail
		/// \param c TODO: Param detail
		/// \param data TODO: Param detail
		/// \param b TODO: Param detail
		typedef void (*CALLBACK)(BitmapButton& c, void* data, BITMAP* b);

	public:
		/// \brief Constructor which makes bitmap size the %button size.
		/// \param m TODO: Param detail
		/// \param x TODO: Param detail
		/// \param y TODO: Param detail
		/// \param up TODO: Param detail
		/// \param down TODO: Param detail
		/// \param disabled TODO: Param detail
		BitmapButton(_Manager& m, int x, int y, BITMAP* up, BITMAP* down = 0, BITMAP* disabled = 0);
		
		/// \brief Constructor which takes %button size as parameters.
		/// \param m TODO: Param detail
		/// \param x TODO: Param detail
		/// \param y TODO: Param detail
		/// \param w TODO: Param detail
		/// \param h TODO: Param detail
		/// \param up TODO: Param detail
		/// \param down TODO: Param detail
		/// \param disabled TODO: Param detail
		BitmapButton(_Manager& m, int x, int y, int w, int h, BITMAP* up, BITMAP* down = 0, BITMAP* disabled = 0);

		/// \brief Destructor.
		~BitmapButton();

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int bottom() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		std::pair<CALLBACK, void*> callback() const;

		/// \brief TODO: Descroption here.
		/// \param f TODO: Param detail
		/// \param d TODO: Param detail
		void callback(CALLBACK f, void* d = 0);

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		BITMAP* disabled() const;

		/// \brief TODO: Descroption here.
		/// \param d TODO: Param detail
		void disabled(BITMAP* d);

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		BITMAP* down() const;

		/// \brief TODO: Descroption here.
		/// \param d TODO: Param detail
		void down(BITMAP* d);
		
		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int height() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int left() const;
		
		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int right() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int top() const;
		
		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int width() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		BITMAP* up() const;

		/// \brief TODO: Descroption here.
		/// \param u TODO: Param detail
		void up(BITMAP* u);

	public:
		/// \if DEVELOPMENT
		void active_changed(bool o, bool n);
		void paint(BITMAP* bmp);
		bool mouse_down(BITMAP* b, int x, int y, int m);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		ButtonImpl::BitmapBase* _impl;
		/// \endif
	};

//=============================================================================
// class Button declaration
//=============================================================================

	class Button : public _ControlEx
	{
	public:
		/// \brief Callback prototype.
		
		/// TODO:  Callback detail
		/// \param c TODO: Param detail
		/// \param data TODO: Param detail
		/// \param b TODO: Param detail
		typedef void (*CALLBACK)(Button& c, void* data, BITMAP* b);

	public:
		// \brief Constructor.
		/// \param m TODO: Param detail
		/// \param x TODO: Param detail
		/// \param y TODO: Param detail
		/// \param w TODO: Param detail
		/// \param h TODO: Param detail
		/// \param title TODO: Param detail
		Button(_Manager& m, int x, int y, int w, int h, const char* title = 0);
		
		// \brief Destructor.
		~Button();
		
		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int bottom() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		std::pair<CALLBACK, void*> callback() const;

		/// \brief TODO: Descroption here.
		/// \param f TODO: Param detail
		/// \param d TODO: Param detail
		void callback(CALLBACK f, void* d = 0);

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		FONT* font() const;

		/// \brief TODO: Descroption here.
		/// \param f TODO: Param detail
		void font(FONT* f);

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int height() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int left() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int right() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		const char* title() const;

		/// \brief TODO: Descroption here.
		/// \param t TODO: Param detail
		void title(const char* t);

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int top() const;

		/// \brief TODO: Descroption here.
		/// \return TODO: Description here.
		int width() const;

	public:
		/// \if DEVELOPMENT
		void active_changed(bool o, bool n);
		void paint(BITMAP* bmp);
		bool mouse_down(BITMAP* b, int x, int y, int m);
	
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		ButtonImpl::Base* _impl;
		/// \endif
	};
}

#endif
