#ifndef	__YACG_SELECTIONBOX_HPP__
#define	__YACG_SELECTIONBOX_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
//=============================================================================
// class SelectionBox declaration
//=============================================================================

	class SelectionBox : public _ControlExLast
	{
	public:
		typedef void (*CALLBACK)(SelectionBox& c, void* d);

	public:	// Common - client usage expected
		SelectionBox(_Manager& m);
		~SelectionBox();
		
		int left() const;
		int right() const;
		
		int top() const;
		int bottom() const;

		int width() const;
		int height() const;

		bool isNew() const;
		
		std::pair<CALLBACK, void*> on_selection() const;
		void on_selection(CALLBACK f, void* d = 0);
		
	public:	// Uncommon - client usage rare, or only for derived
		bool mouse_down(BITMAP* bmp, int x, int y, int m);
		void paint(BITMAP* bmp);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		int _left;
		int _right;

		int _top;
		int _bottom;
		
		BITMAP* _pattern;
		
		mutable bool _new;
		
		CALLBACK _callback;
		void* _data;
	};
}

//=============================================================================
// class SelectionBox macros
//=============================================================================

#define	SELECTIONBOX()					insert(*new yacg::SelectionBox(*this));
#define	SELECTIONBOX_EX(f, d)			{ yacg::SelectionBox* o = new yacg::SelectionBox(*this); insert(*o); o->on_selection((f), (d)); }

#define	SELECTIONBOX_V(v)				v##_ = new yacg::SelectionBox(*this); insert(*v##_);
#define	SELECTIONBOX_V_EX(v, f, d)		v##_ = new yacg::SelectionBox(*this); insert(*v##_); v##_->on_selection((f), (d));

#endif
