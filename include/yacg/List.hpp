#ifndef	__YACG_LIST_HPP__
#define	__YACG_LIST_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

#ifndef	__STD_LIST__
#define	__STD_LIST__
#include <list>
#endif

namespace yacg
{
//=============================================================================
// class List declaration
//=============================================================================

	namespace ListImpl
	{
		class Base;
	}
	
	class List : public _ControlEx
	{
	public:
		static const int t_title = 0x01 << 16;
		static const int t_notitle = 0x00 << 16;
		static const int t_mask = 0x01 << 16;
		
		static const int s_sorted = 0x00 << 16;
		static const int s_unsorted = 0x02 << 16;
		static const int s_mask = 0x02 << 16;
		
		static const int v_scroll = 0x04 << 16;
		static const int v_noscroll = 0x00 << 16;
		static const int v_mask = 0x04 << 16;
		
		static const int h_scroll = 0x08 << 16;
		static const int h_noscroll = 0x00 << 16;
		static const int h_mask = 0x08 << 16;
	
	public:
		typedef std::list<std::pair<std::string, void*> >::iterator iterator;
		typedef std::list<std::pair<std::string, void*> >::const_iterator const_iterator;
		
	public:	// Common - client usage expected
		List(_Manager& m, int x, int y, int w, int h, int f, const char* t = 0);
		~List();

		int left() const;
		int right() const;
		
		int top() const;
		int bottom() const;

		int width() const;
		int height() const;
		
		iterator begin();
		const_iterator begin() const;
		
		iterator end();
		const_iterator end() const;
				
		iterator insert(const char* t, void* d = 0);
		iterator insert(iterator i, const char* t, void* d = 0);
		
		void erase(iterator i);

		bool empty() const;
		
		int size() const;

		FONT* font() const;
		void font(FONT* f);
		
		const char* title() const;
		void title(const char* t);

	public:	// Uncommon - client usage rare, or only for derived
		void active_changed(bool o, bool n);
		void flags_changed(int o, int n);
		void paint(BITMAP* b);
		bool mouse_down(BITMAP* bmp, int x, int y, int m);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		ListImpl::Base* _impl;
	};
}

//=============================================================================
// class List macros
//=============================================================================

#define	LIST(x, y, w, h, f)				insert(*new yacg::List(*this, (x), (y), (w), (h), (f)));
#define	LIST_EX(x, y, w, h, f, t)		insert(*new yacg::List(*this, (x), (y), (w), (h), (f), (t)));

#define	LIST_V(v, x, y, w, h, f)		v##_ = new yacg::List(*this, (x), (y), (w), (h), (f)); insert(*v##_);
#define	LIST_V_EX(v, x, y, w, h, f, t)	v##_ = new yacg::List(*this, (x), (y), (w), (h), (f), (t)); insert(*v##_);

#endif
