#ifndef	__YACG_CHECKEDBOX_HPP__
#define	__YACG_CHECKEDBOX_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
	//=========================================================================
	/// \if DEVELOPMENT
	namespace CheckedBoxImpl
	{
		class Base;
	}
	/// \endif
	
	//=========================================================================

	class CheckedBox : public _ControlEx
	{
	public:
		//=====================================================================		
		/// \brief Callback prototype.
		
		/// Prototype definition for callback that will be called when the setting
		/// of the %checkbox changes.
		/// \param c TODO: Parameter description
		/// \param bmp TODO: Parameter description
		/// \param d TODO: Parameter description
		typedef void (*CALLBACK)(CheckedBox& c, BITMAP* bmp, void* d);
	
	public:
		//=====================================================================		
		/// \name	Button
		/// TOOD: Button flag description
		
		/// \{
		/// \brief TODO:  Constant description
		static const int b_box		= 0x00000000;
		/// \brief TODO:  Constant description
		static const int b_button	= 0x00010000;
		/// \brief TODO:  Constant description
		static const int b_mask		= 0x00010000;
		/// \}

		//=====================================================================		
		/// \name	Horizontal Alignment
		/// TOOD: Horizontal Alignment flag description
		
		/// \{
		/// \brief TODO:  Constant description
		static const int h_left		= 0x00000000;
		/// \brief TODO:  Constant description
		static const int h_right	= 0x00020000;
		/// \brief TODO:  Constant description
		static const int h_mask		= 0x00020000;
		/// \}
		
		//=====================================================================		
		/// \name	Vertical Alignment
		/// TOOD: Vertical Alignment flag description
		
		/// \{
		/// \brief TODO:  Constant description
		static const int v_top		= 0x00000000;
		/// \brief TODO:  Constant description
		static const int v_bottom	= 0x00040000;
		/// \brief TODO:  Constant description
		static const int v_mask		= 0x00040000;
		/// \}
	
	public:
		
		typedef std::vector<_Control*>::iterator iterator;
		typedef std::vector<_Control*>::const_iterator const_iterator;

	public:
		CheckedBox(_Manager& m, int x, int y, int w, int h, int f, const char* t = 0);
		~CheckedBox();
		
		iterator begin();

		const_iterator begin() const;

		int bottom() const;

		int bottom_controls() const;
		
		std::pair<CALLBACK, void*> callback() const;

		void callback(CALLBACK f, void* d = 0);

		iterator end();

		const_iterator end() const;
		
		bool empty() const;
		
		void erase(iterator i);

		int height() const;

		int height_controls() const;

		iterator insert(_Control& c);

		iterator insert(_ControlEx& c);

		int left() const;
		
		int left_controls() const;
		
		int right() const;
		
		int right_controls() const;
		
		int size() const;

		bool state() const;

		void state(bool s);

		const char* title() const;

		void title(const char* t);
		
		const std::string& title_object() const;

		int top() const;
		
		int top_controls() const;
		
		int width() const;

		int width_controls() const;

	public:
		/// \if DEVELOPMENT
		void active_changed(bool o, bool n);
		void flags_changed(int o, int n);
		void paint(BITMAP* bmp);
		bool mouse_down(BITMAP* b, int x, int y, int m);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		CheckedBoxImpl::Base* _impl;
		/// \endif
	};
}

#endif
