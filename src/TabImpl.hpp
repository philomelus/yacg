#ifndef	__TABIMPL_HPP__
#define	__TABIMPL_HPP__

namespace yacg
{
	namespace TabImpl
	{
//=============================================================================
// class TabBase declaration
//=============================================================================

		class TabBase
		{
		public:
			enum loc
			{
				normal,
				sel_minus_1,
				sel,
				sel_plus_1
			};

		public:
			TabBase(Tab& o, int l, int t, int w, int h, int f, const char* title);
			virtual ~TabBase();
			
			void active_changed(bool o, bool n);
			
			Tab::c_iterator beginc();
			Tab::c_const_iterator beginc() const;

			Tab::p_iterator beginp();
			Tab::p_const_iterator beginp() const;

			int bottom() const;
			virtual int bottom_controls() const;

			Tab::c_iterator endc();
			Tab::c_const_iterator endc() const;
			
			Tab::p_iterator endp();
			Tab::p_const_iterator endp() const;
			
			bool emptyc() const;
			
			bool emptyp() const;
			
			void erasec(Tab::c_iterator i);
			
			void erasep(Tab::p_iterator i);
			
			void flags_changed(int o, int n);

			int height() const;
			virtual int height_controls() const;
			
			void initial(int s);
			
			Tab::c_iterator insertc(_Control& c);
			Tab::c_iterator insertc(_ControlEx& c);
			
			Tab::p_iterator insertp(Tab::p_iterator i, TabPage& tp);
			
			int left() const;
			virtual int left_controls() const;

			void pages_begin();
			void pages_end();
			
			void paint(BITMAP* bmp);

			bool mouse_down(BITMAP* bmp, int x, int y, int m);

			int right() const;
			virtual int right_controls() const;
			
			int selected() const;
			void selected(int s);
			
			int sizec() const;
			
			int sizep() const;

			std::pair<Tab::TABCHANGED, void*> tab_changed() const;
			void tab_changed(Tab::TABCHANGED f, void* d = 0);
			
			const char* title() const;
			void title(const char* t);

			Title& title_control() const;
			
			int top() const;
			virtual int top_controls() const;
			
			int width() const;
			virtual int width_controls() const;

#ifdef	_DEBUG
			void dump() const;
#endif

		public:
			virtual bool isInTabs(int x, int y) const = 0;
			virtual void paint_tabs(BITMAP* bmp, bool tabsOnly = false, bool down = false) = 0;
			virtual void recalc() = 0;
		
		private:
			void change_tab(int o, int n);
			
		private:
			typedef std::vector<_Control*> c_container;
			typedef std::vector<TabPage*> p_container;
			
		protected:
			Tab& _owner;
			
			int _left;
			int _leftTab;
			int _right;
			int _rightTab;
			
			int _top;
			int _topTab;
			int _bottom;
			int _bottomTab;
			
			int _width;
			int _height;
			
			p_container _pages;
			
			c_container _controls;
			c_container _eventControls;

			Tab::TABCHANGED _tabChanged;
			void* _data;

			int _selected;
			
			std::auto_ptr<Title> _title;
			
			struct tab
			{
				tab(int _l, int _t, int _r, int _b)
						:
						l(_l),
						t(_t),
						r(_r),
						b(_b)
				{
				}
				
				int l;
				int t;
				int r;
				int b;
			};
			typedef std::vector<tab> t_container;
			t_container _tabs;
			
			bool _insertingPages;
		};

		
//=============================================================================
// class TabPageImpl declaration
//=============================================================================

		class TabPageImpl
		{
		public:
			TabPageImpl(TabPage& o, const char* t);

			void activate(Tab& t);
			
			std::pair<TabPage::ACTIVATED, void*> activated() const;
			void activated(TabPage::ACTIVATED f, void* d);

			TabPage::iterator begin();
			TabPage::const_iterator begin() const;

			void deactivate(Tab& t);
			
			std::pair<TabPage::DEACTIVATED, void*> deactivated() const;
			void deactivated(TabPage::DEACTIVATED f, void* d);

			void dirty();		// Sets it for all controls within page
			
#ifdef	_DEBUG
			void dump() const;
#endif

			bool empty() const;

			TabPage::iterator end();
			TabPage::const_iterator end() const;

			void erase(TabPage::iterator i);

			TabPage::iterator insert(TabPage::iterator i, _Control& c);
			TabPage::iterator insert(TabPage::iterator i, _ControlEx& c);

			int length() const;
			void length(int l);
			
			void paint(BITMAP* bmp);

			bool mouse_down(BITMAP* bmp, int x, int y, int m);
			
			int size() const;

			const char* title() const;
			void title(const char* t);
			
			const std::string& title_string() const;
				
		private:
			typedef std::vector<_Control*> container;
			
		protected:
			TabPage& _owner;
			
			container _controls;
			container _eventControls;
			
			std::string _title;
			
			TabPage::ACTIVATED _activated;
			void* _activatedData;
			
			TabPage::DEACTIVATED _deactivated;
			void* _deactivatedData;
			
			int _length;				// Tab length
		};

//=============================================================================
// class TabBottomHoriz declaration
//=============================================================================

		class TabBottomHoriz : public TabBase
		{
		public:
			TabBottomHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title);
		
			int bottom_controls() const;
			int height_controls() const;
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
			int top_controls() const;

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, bool sel, const char* s, bool down);
		};
		
//=============================================================================
// class TabBottomHoriz3D declaration
//=============================================================================

		class TabBottomHoriz3D : public TabBase
		{
		public:
			TabBottomHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			int bottom_controls() const;
			int height_controls() const;
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
			int top_controls() const;
		
		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, bool first);
		};

//=============================================================================
// class TabBottomVert declaration
//=============================================================================

		class TabBottomVert : public TabBase
		{
		public:
			TabBottomVert(Tab& o, int l, int t, int w, int h, int f, const char* title);
		
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
		};
		
//=============================================================================
// class TabBottomVert3D declaration
//=============================================================================

		class TabBottomVert3D : public TabBase
		{
		public:
			TabBottomVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
		};

//=============================================================================
// class TabLeftHoriz declaration
//=============================================================================

		class TabLeftHoriz : public TabBase
		{
		public:
			TabLeftHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w,
					const char* s, bool down);
		};
		
//=============================================================================
// class TabLeftHoriz3D declaration
//=============================================================================

		class TabLeftHoriz3D : public TabBase
		{
		public:
			TabLeftHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
		
		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, int pl);
		};

//=============================================================================
// class TabLeftVert declaration
//=============================================================================

		class TabLeftVert : public TabBase
		{
		public:
			TabLeftVert(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s);
				
		private:
			int _charWidth;
		};
		
//=============================================================================
// class TabLeftVert3D declaration
//=============================================================================

		class TabLeftVert3D : public TabBase
		{
		public:
			TabLeftVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s);
				
		private:
			int _charWidth;
		};

//=============================================================================
// class TabRightHoriz declaration
//=============================================================================

		class TabRightHoriz : public TabBase
		{
		public:
			TabRightHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
			int right_controls() const;
			int width_controls() const;

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, bool down);
		};
		
//=============================================================================
// class TabRightHoriz3D declaration
//=============================================================================

		class TabRightHoriz3D : public TabBase
		{
		public:
			TabRightHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
			int right_controls() const;
			int width_controls() const;

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, int pr);
		};

//=============================================================================
// class TabRightVert declaration
//=============================================================================

		class TabRightVert : public TabBase
		{
		public:
			TabRightVert(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();

		private:		
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, bool down);
			
		private:
			int _charWidth;
		};

//=============================================================================
// class TabRightVert3D declaration
//=============================================================================

		class TabRightVert3D : public TabBase
		{
		public:
			TabRightVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();

		private:		
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s);
			
		private:
			int _charWidth;
		};

//=============================================================================
// class TabTopHoriz declaration
//=============================================================================

		class TabTopHoriz : public TabBase
		{
		public:
			TabTopHoriz(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			int bottom_controls() const;
			int height_controls() const;
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
			int top_controls() const;

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s, bool down);
		};
		
//=============================================================================
// class TabTopHoriz3D declaration
//=============================================================================

		class TabTopHoriz3D : public TabBase
		{
		public:
			TabTopHoriz3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			int bottom_controls() const;
			int height_controls() const;
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
			int top_controls() const;

		private:
			void paint_tab(BITMAP* bmp, int l, int t, int r, int b, loc w, const char* s);
		};
		
//=============================================================================
// class TabTopVert declaration
//=============================================================================

		class TabTopVert : public TabBase
		{
		public:
			TabTopVert(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
		};
		
//=============================================================================
// class TabTopVert3D declaration
//=============================================================================

		class TabTopVert3D : public TabBase
		{
		public:
			TabTopVert3D(Tab& o, int l, int t, int w, int h, int f, const char* title);
			
			bool isInTabs(int x, int y) const;
			void paint_tabs(BITMAP* bmp, bool tabsOnly, bool down);
			void recalc();
		};
	}
}
		
#endif
