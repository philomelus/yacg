#ifndef	__YACG_TAB_HPP__
#define	__YACG_TAB_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

#ifndef	__YACG_TITLE_HPP__
#include "yacg/Title.hpp"
#endif

namespace yacg
{
	namespace TabImpl
	{
		class TabBase;
		class TabPageImpl;
	}
	class TabPage;
	class Tab;
	
//=============================================================================
// class Tab
//=============================================================================

	class Tab : public _ControlEx
	{
	public:
		typedef void (*TABCHANGED)(Tab& t, TabPage& o, TabPage& n, void* d);

	public:
		// Location of tabs
		static const int l_top = 0x00 << 16;			// default
		static const int l_left = 0x01 << 16;
		static const int l_right = 0x02 << 16;
		static const int l_bottom = 0x03 << 16;
		static const int l_mask = 0x03 << 16;

		// Orientation of tabs
		static const int o_horizontal = 0x00 << 16;		// default for top/bottom
		static const int o_vertical = 0x04 << 16;		// default for left/right
		static const int o_mask = 0x04 << 16;
		
		// Tab allignment within location
		static const int a_left = 0x00 << 16;			// default for top/bottom
		static const int a_top = 0x00 << 16;			// default for left/right
		static const int a_right = 0x08 << 16;
		static const int a_bottom = 0x08 << 16;
		static const int a_mask = 0x08 << 16;

		// How to determine length of tabs
		static const int w_equal = 0x00 << 16;			// equally distributed
		static const int w_title = 0x10 << 16;			// by width/height of page titles
		static const int w_page = 0x20 << 16;			// specified width per page
		static const int w_mask = 0x30 << 16;

		// Whether tab has an overall title
		static const int t_notitle = 0x00 << 16;		// default
		static const int t_title = 0x40 << 16;
		static const int t_mask = 0x40 << 16;
		
	public:
		typedef std::vector<TabPage*>::iterator p_iterator;
		typedef std::vector<TabPage*>::const_iterator p_const_iterator;
		
		typedef std::vector<_Control*>::iterator c_iterator;
		typedef std::vector<_Control*>::const_iterator c_const_iterator;

	public:	// Common - client usage expected
		Tab(_Manager& m, int l, int t, int w, int h, int f, const char* title = 0);
		~Tab();

		int left() const;
		int right() const;

		int left_controls() const;		// left of area for user controls
		int right_controls() const;		// right of area for user controls
		
		int top() const;
		int bottom() const;

		int top_controls() const;		// top of area for user controls
		int bottom_controls() const;	// bottom of area for user controls
		
		int width() const;
		int height() const;
		
		int width_controls() const;		// width of area for user controls
		int height_controls() const;	// height of area for user controls
		
		std::pair<TABCHANGED, void*> tab_changed() const;
		void tab_changed(TABCHANGED f, void* d = 0);
		
		const char* title() const;
		void title(const char* t);
		
		Title& title_control() const;
		
		void initial(int s);			// Initially displayed page
		
		int selected() const;
		void selected(int s);
		
		// Tab Pages
		void pages_begin();				// To prevent calculations after each tab is inserted, call this before the first
		void pages_end();				// and this after the last one.  Macros do this automatically.
		
		p_iterator beginp();
		p_const_iterator beginp() const;

		p_iterator endp();
		p_const_iterator endp() const;
		
		bool emptyp() const;
		
		void erasep(p_iterator i);
		
		p_iterator insertp(TabPage& tp);
		p_iterator insertp(p_iterator i, TabPage& tp);
		
		int sizep() const;

		// Common controls
		c_iterator beginc();
		c_const_iterator beginc() const;

		c_iterator endc();
		c_const_iterator endc() const;
		
		bool emptyc() const;
		
		void erasec(c_iterator i);
		
		c_iterator insertc(_Control& c);
		c_iterator insertc(_ControlEx& c);
		
		int sizec() const;
		
	public:	// Uncommon - client usage rare, or only for derived
		void active_changed(bool o, bool n);
		void flags_changed(int o, int n);
		void paint(BITMAP* bmp);
		bool mouse_down(BITMAP* b, int x, int y, int m);

#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		TabImpl::TabBase* _impl;
	};

//=============================================================================
// class TabPage
//=============================================================================

	class TabPage
	{
	public:
		typedef void (*ACTIVATED)(Tab& t, TabPage& p, void* d);
		typedef void (*DEACTIVATED)(Tab& t, TabPage& p, void* d);
	
	public:	// Common - client usage expected
		typedef std::vector<_Control*>::iterator iterator;
		typedef std::vector<_Control*>::const_iterator const_iterator;
	
	public:
		TabPage(Tab& o, const char* t);
		virtual ~TabPage();
		
		iterator begin();
		const_iterator begin() const;

		iterator end();
		const_iterator end() const;
		
		bool empty() const;
		
		void erase(iterator i);
		
		iterator insert(iterator i, _Control& c);
		iterator insert(iterator i, _ControlEx& c);

		int length() const;
		void length(int l);
		
		int size() const;

		const char* title() const;
		void title(const char* t);
		
		const std::string& title_string() const;
		
		std::pair<ACTIVATED, void*> activated() const;
		void activated(ACTIVATED f, void* d = 0);
		
		std::pair<DEACTIVATED, void*> deactivated() const;
		void deactivated(DEACTIVATED f, void* d = 0);

#ifdef	_DEBUG
		void dump() const;
#endif

	public:	// Uncommon - client usage rare, or only for derived
		virtual void paint(BITMAP* bmp);
		virtual bool mouse_down(BITMAP* bmp, int x, int y, int m); 
	
	private:
		TabImpl::TabPageImpl* _impl;
		friend TabImpl::TabBase;
	};
}

//=============================================================================
// class Tab macros
//=============================================================================

#define	TAB_BEGIN(l, t, w, h, f)				{ std::auto_ptr<yacg::Tab> pTab(new yacg::Tab(*this, (l), (t), (w), (h), (f))); insert(*pTab.get()); const int LC = pTab->left_controls(); const int TC = pTab->top_controls(); const int RC = pTab->right_controls(); const int BC = pTab->bottom_controls(); const int WC = pTab->width_controls(); const int HC = pTab->height_controls(); pTab->pages_begin();
#define	TAB_BEGIN_EX(l, t, w, h, f, title)		{ std::auto_ptr<yacg::Tab> pTab(new yacg::Tab(*this, (l), (t), (w), (h), (f), (title))); insert(*pTab.get()); const int LC = pTab->left_controls(); const int TC = pTab->top_controls(); const int RC = pTab->right_controls(); const int BC = pTab->bottom_controls(); const int WC = pTab->width_controls(); const int HC = pTab->height_controls(); pTab->pages_begin();
#define	TAB_END()								pTab->pages_end(); pTab->initial(0); pTab.release(); }

#define	TAB_V_BEGIN(v, l, t, w, h, f)			{ std::auto_ptr<yacg::Tab> pTab(new yacg::Tab(*this, (l), (t), (w), (h), (f))); v##_ = pTab.get(); insert(*pTab.get()); const int LC = pTab->left_controls(); const int TC = pTab->top_controls(); const int RC = pTab->right_controls(); const int BC = pTab->bottom_controls(); const int WC = pTab->width_controls(); const int HC = pTab->height_controls(); pTab->pages_begin();
#define	TAB_V_BEGIN_EX(v, l, t, w, h, f, title)	{ std::auto_ptr<yacg::Tab> pTab(new yacg::Tab(*this, (l), (t), (w), (h), (f), (title))); v##_ = pTab.get(); insert(*pTab.get()); const int LC = pTab->left_controls(); const int TC = pTab->top_controls(); const int RC = pTab->right_controls(); const int BC = pTab->bottom_controls(); const int WC = pTab->width_controls(); const int HC = pTab->height_controls(); pTab->pages_begin();

#define	TABCOMMON_BEGIN()						{ const int start_size = size();
#define	TABCOMMON_END()							const int end_size = size(); for (int n = start_size; n < end_size; ++n) { yacg::_Control& c = (*this)[n]; if (dynamic_cast<yacg::_ControlEx*>(&c) == 0) pTab->insertc(c); else pTab->insertc(static_cast<yacg::_ControlEx&>(c)); } }

//=============================================================================
// class TabPage macros
//=============================================================================

#define	TABPAGE_BEGIN(t)		{ std::auto_ptr<yacg::TabPage> page(new yacg::TabPage(*pTab.get(), t)); const int start_size = size();
#define	TABPAGE_END()			const int end_size = size(); for (int n = start_size; n < end_size; ++n) { yacg::_Control& c = (*this)[n]; c.visible(false); if (dynamic_cast<yacg::_ControlEx*>(&c) == 0) page->insert(page->end(), c); else page->insert(page->end(), static_cast<yacg::_ControlEx&>(c)); } pTab->insertp(*page.get()); page.release(); }

#define	TABPAGE_V_BEGIN(v, t)	{ std::auto_ptr<yacg::TabPage> page(new yacg::TabPage(*pTab.get(), t)); v##_ = page.get(); const int start_size = size();

#endif
