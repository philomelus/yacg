#ifndef	__YACG_HPP__
#define	__YACG_HPP__

#ifndef	__STD_ALGORITHM__
#define	__STD_ALGORITHM__
#include <algorithm>
#endif

#ifndef	__STD_VECTOR__
#define	__STD_VECTOR__
#include <vector>
#endif

#ifndef	__BOOST_SIGNALS_HPP__
#define	__BOOST_SIGNALS_HPP__
#define	BOOST_SIGNALS_STATIC_LINK
#include <boost/signals.hpp>
#endif

#ifndef	__BOOST_UTILITY_HPP__
#define	__BOOST_UTILITY_HPP__
#include <boost/utility.hpp>
#endif

namespace yacg
{
	class _Manager;
	
//=============================================================================
// Namespace
//=============================================================================

	//-------------------------------------------------------------------------
	extern void yacg_init();

	//-------------------------------------------------------------------------
	typedef boost::signals::connection EVENTID;
}

#ifndef	__YACG_THEME_HPP__
#include <yacg/Theme.hpp>
#endif

namespace yacg
{
//=============================================================================
// class _Control declaration
//=============================================================================

	class _Control : private boost::noncopyable
	{
	//-------------------------------------------------------------------------
	public:
		static const int calc = -1;			// for coords or sizes to auto-calc
		
		static const int undefined = -2;	// for coords or sizes, literal meaning
		
		static const int dirty_all = 0xFFFFFFFF;	// dirty() for repaint EVERYTHING as if first time

		static const int update_display = 0x80000000;	// When dirty() value has this, do paint.  Otherwise wait until bit is set.

		static const int auto_delete = 0x00008000;	// when set in flags, control is deleted on _Manager destruction
		
	//-------------------------------------------------------------------------
	// construction / destruction
	
	public:
		_Control(_Manager& m, int f = auto_delete);
		
		virtual ~_Control();
		
	//-------------------------------------------------------------------------
	// active
	
	private:
		typedef boost::signal<void (_Control& c, bool n)> active_changed_slot;
		
	public:
		typedef active_changed_slot::slot_type ACTIVE_EVENT;
		
	public:
		bool active() const;
		
		void active(bool a);
	
		EVENTID connect_active(ACTIVE_EVENT f);

		void disconnect_active(EVENTID f);

	protected:
		virtual void active_changed();
	
		int _activeChangedHint;
		
	private:
		bool _active;

		active_changed_slot _activeHandlers;
		
	//-------------------------------------------------------------------------
	// dirty
	
	private:
		typedef boost::signal<void (_Control& c, int o)> dirty_changed_slot;
		
	public:
		typedef dirty_changed_slot::slot_type DIRTY_EVENT;
		
	public:
		void clean(int b = dirty_all);
		
		int dirty() const;

		void dirty(int d);
	
		EVENTID connect_dirty(DIRTY_EVENT f);

		void disconnect_dirty(EVENTID f);

	protected:
		virtual void dirty_changed(int o);
		
	private:	
		int _dirty;
		
		dirty_changed_slot _dirtyHandlers;
	
	//-------------------------------------------------------------------------
	// flags
	
	private:
		typedef boost::signal<void (_Control& c)> flags_changed_slot;
		
	public:
		typedef flags_changed_slot::slot_type FLAGS_EVENT;
		
	public:
		int flags() const;
		
		void flags(int f);

		EVENTID connect_flags(FLAGS_EVENT f);

		void disconnect_flags(EVENTID f);

	protected:
		virtual void flags_changed();

		int _flagsChangedHint;
		
	private:
		int _flags;
		
		flags_changed_slot _flagsHandlers;
	
	//-------------------------------------------------------------------------
	public:
		_Manager& manager() const;

		void manager(_Manager& m);
				
	private:
		_Manager* _manager;
	
	//-------------------------------------------------------------------------
	// theme

	private:
		typedef boost::signal<void (_Control& c)> theme_changed_slot;
		
	public:
		typedef theme_changed_slot::slot_type THEME_EVENT;
		
	public:
		void attach_theme();
		
		EVENTID connect_theme(THEME_EVENT f);

		void detach_theme();
		
		void disconnect_theme(EVENTID f);

		Theme& theme() const;
		
		void theme(Theme& t);
	
	protected:
		virtual void theme_changed();

		int _themeChangedHint;
	
	private:
		void theme_signal(Theme& t);
		
	private:
		Theme* _theme;

		theme_changed_slot _themeHandlers;

		EVENTID _themeSignal;
		
	//-------------------------------------------------------------------------
	// visibility
	
	private:
		typedef boost::signal<void (_Control& c)> visible_changed_slot;
		
	public:
		typedef visible_changed_slot::slot_type VISIBLE_EVENT;
		
	public:
		bool visible() const;
		
		void visible(bool v);

		EVENTID connect_visible(VISIBLE_EVENT f);

		void disconnect_visible(EVENTID f);
		
	protected:
		virtual void visible_changed();

		int _visibleChangedHint;
		
	private:
		bool _visible;

		visible_changed_slot _visibleHandlers;
				
	//-------------------------------------------------------------------------
	private:
		typedef boost::signal<void (_Control& c, BITMAP* bmp)> pre_paint_slot;
		typedef boost::signal<void (_Control& c, BITMAP* bmp)> post_paint_slot;
	
	public:
		typedef pre_paint_slot::slot_type PRE_PAINT_EVENT;
		typedef post_paint_slot::slot_type POST_PAINT_EVENT;

	public:
		EVENTID connect_pre_paint(PRE_PAINT_EVENT f);
		EVENTID connect_post_paint(POST_PAINT_EVENT f);
		
		void disconnect_pre_paint(EVENTID e);
		void disconnect_post_paint(EVENTID e);

		void paint(BITMAP* bmp);

	protected:
		virtual void paint_control(BITMAP* bmp, int dirty) = 0;

		virtual void pre_paint(BITMAP* bmp);
		virtual void post_paint(BITMAP* bmp);

	private:
		pre_paint_slot _prePaintHandlers;
		post_paint_slot _postPaintHandlers;
	
	//-------------------------------------------------------------------------
	public:
#ifdef	_DEBUG
		virtual void dump(const std::string& i) const = 0;
#endif
	
	protected:
#ifdef	_DEBUG
		void dump_control(const std::string& i) const;
#endif

	//-------------------------------------------------------------------------	
		friend _Manager;
	};

//=============================================================================
// class _ControlEx declaration : _Control with keyboard and mouse handling
//=============================================================================

	class _ControlEx : public _Control
	{
	//-------------------------------------------------------------------------
	public:
		static const int d_none			= 0x00;
		
		static const int d_event		= 0x01;
		
		static const int d_pre			= 0x02;
		
		static const int d_post			= 0x04;

		static const int d_prepost		= (d_pre | d_post);
		
		static const int d_preevent		= (d_pre | d_event);
		
		static const int d_postevent	= (d_post | d_event);

		static const int d_preeventpost	= (d_pre | d_post | d_event);
		static const int d_all			= d_preeventpost;
		
		static const int d_mask			= (d_pre | d_post | d_event);

	//-------------------------------------------------------------------------
	public:
		static const int m_none			= 0x00;
		
		static const int m_event		= 0x08;
		
		static const int m_pre			= 0x10;
		
		static const int m_post			= 0x20;

		static const int m_prepost		= (m_pre | m_post);
		
		static const int m_preevent		= (m_pre | m_event);
		
		static const int m_postevent	= (m_post | m_event);

		static const int m_preeventpost	= (m_pre | m_post | m_event);
		static const int m_all = m_preeventpost;
		
		static const int m_mask			= (m_pre | m_post | m_event);
	
	//-------------------------------------------------------------------------
	public:
		_ControlEx(_Manager& m, int f = 0);
		
	//-------------------------------------------------------------------------
	private:
		struct isTrue
		{
			typedef bool result_type;
			template<typename I>
			bool operator()(I b, I e) const
			{
				if (b == e)
					return false;
				while (b++ != e)
				{
					if (*b)
						return true;
				}
				return false;
			}
		};
		typedef boost::signal<bool (_ControlEx& c, BITMAP* bmp, int f, int x, int y, int m), isTrue> mouse_down_slot;
	
	public:
		typedef mouse_down_slot::slot_type MOUSE_DOWN_EVENT;
	
	public:
		EVENTID connect_mouse_down(MOUSE_DOWN_EVENT f);

		void disconnect_mouse_down(EVENTID f);
	
	protected:
		virtual bool mouse_down(BITMAP* bmp, int x, int y, int m);
		virtual void mouse_down_pre(BITMAP* bmp, int x, int y, int m);
		virtual void mouse_down_post(BITMAP* bmp, int x, int y, int m);
	
	private:
		mouse_down_slot _mouseDownHandlers;
	
	//-------------------------------------------------------------------------
	private:
		typedef boost::signal<void (_ControlEx& c, BITMAP* bmp, int f, int x, int y)> mouse_move_slot;
	
	public:
		typedef mouse_move_slot::slot_type MOUSE_MOVE_EVENT;
	
	public:
		EVENTID connect_mouse_move(MOUSE_MOVE_EVENT f);

		void disconnect_mouse_move(EVENTID f);
	
	protected:
		virtual void mouse_move(BITMAP* bmp, int x, int y);
		virtual void mouse_move_pre(BITMAP* bmp, int x, int y);
		virtual void mouse_move_post(BITMAP* bmp, int x, int y);
	
	private:
		mouse_move_slot _mouseMoveHandlers;

	//-------------------------------------------------------------------------	
	public:
#ifdef	_DEBUG
		void dump_control(const std::string& i) const;
#endif

	//-------------------------------------------------------------------------
	private:
		friend _Manager;
	};

//=============================================================================
// class _Manager declaration
//=============================================================================

	class _Manager : private boost::noncopyable
	{
	//-------------------------------------------------------------------------
	private:
		typedef std::vector<_ControlEx*> events;
		struct control_info
		{
			control_info(_Control& c);
			
			bool mouseDown;
			EVENTID mouseDownPre;
			EVENTID mouseDownPost;
			EVENTID mouseMovePre;
			EVENTID mouseMove;
			EVENTID mouseMovePost;
			_Control* control;
		};
		typedef std::list<control_info*> container;
		
	//-------------------------------------------------------------------------
	/// bitmap
	
	private:
		typedef boost::signal<void (_Manager& m, BITMAP* o)> bitmap_changed_slot;
	
	public:
		typedef bitmap_changed_slot::slot_type BITMAP_CHANGED_EVENT;

	public:
		BITMAP* bitmap() const;
		
		void bitmap(BITMAP* bmp);

		EVENTID connect_bitmap_changed(BITMAP_CHANGED_EVENT f);
		
		void disconnect_bitmap_changed(EVENTID e);

	protected:
		virtual void bitmap_changed(BITMAP* o);
		
	private:
		void update_bitmap(control_info* c);

	private:
		BITMAP* _bitmap;

		bitmap_changed_slot _bitmapChangedHandlers;

	//-------------------------------------------------------------------------
	// debug

	public:
#ifdef	_DEBUG
		virtual void dump(const std::string& i) const;
		
		void dump_controls(control_info* c, const std::string& i) const;
#endif

	//-------------------------------------------------------------------------
	/// painting

	private:
		typedef boost::signal<void (_Manager&)> pre_paint_slot;
		typedef boost::signal<void (_Manager&)> post_paint_slot;
	
	public:
		typedef pre_paint_slot::slot_type PRE_PAINT_EVENT;
		typedef post_paint_slot::slot_type POST_PAINT_EVENT;

	public:
		EVENTID connect_pre_paint(PRE_PAINT_EVENT f);
		EVENTID connect_post_paint(POST_PAINT_EVENT f);
		
		void disconnect_pre_paint(EVENTID e);
		void disconnect_post_paint(EVENTID e);
		
		void dirty();

		void paint();
		
		void paint(_Control& c);
		
		void update();
		
		void update(_Control& c);

	protected:
		virtual void pre_paint();
		virtual void post_paint();
		
	private:
		void painter(control_info* c);

		void updater(control_info* c);

	private:		
		bool _dirty;

		pre_paint_slot _prePaintHandlers;
		post_paint_slot _postPaintHandlers;
	
	//-------------------------------------------------------------------------
	// mouse down event
	
	private:
		typedef boost::signal<bool (_Manager& m, int x, int y, int b), _ControlEx::isTrue> mouse_down_slot;
	
	public:
		typedef mouse_down_slot::slot_type MOUSE_DOWN_EVENT;

	public:
		EVENTID connect_mouse_down(MOUSE_DOWN_EVENT f);
		
		void disconnect_mouse_down(EVENTID f);

	protected:
		virtual void mouse_down(int x, int y, int b);
		
	private:
		mouse_down_slot _mouseDownHandlers;
		
		events _mouseDownControls;
		boost::signal<void (BITMAP* bmp, int x, int y, int b)> _mouseDownControlsPre;
		boost::signal<void (BITMAP* bmp, int x, int y, int b)> _mouseDownControlsPost;
		
	//-------------------------------------------------------------------------
	// mouse move event
	
	private:
		typedef boost::signal<void (_Manager& m, int x, int y)> mouse_move_slot;
	
	public:
		typedef mouse_move_slot::slot_type MOUSE_MOVE_EVENT;

	public:
		EVENTID connect_mouse_move(MOUSE_MOVE_EVENT f);

		void disconnect_mouse_move(EVENTID f);

	protected:
		virtual void mouse_move(int x, int y);
		
	private:
		int _mouseX;
		int _mouseY;

		mouse_move_slot _mouseMoveHandlers;	// _Manager-wide

		boost::signal<void (BITMAP* bmp, int x, int y)> _mouseMoveControls;
		boost::signal<void (BITMAP* bmp, int x, int y)> _mouseMoveControlsPre;
		boost::signal<void (BITMAP* bmp, int x, int y)> _mouseMoveControlsPost;

	//-------------------------------------------------------------------------
	// common event
	
	private:
		typedef boost::signal<void (_Manager& m)> idle_slot;
		typedef idle_slot::slot_type IDLE;

	public:
		void abort();
		
		void process_events(IDLE f);

		void process_events();
		
		std::pair<bool, bool> process_events_and_return();

		void reset();

	private:
		bool _aborted;
		
	//-------------------------------------------------------------------------
	// access
	
	public:
		_Control& operator[](int i) const;

	//-------------------------------------------------------------------------
	/// iteration
		
	public:
		class iterator : public container::iterator
		{
		public:
			//-----------------------------------------------------------------
			iterator();
			iterator(const iterator& r);

			iterator& operator=(const iterator& r);

			//-----------------------------------------------------------------			
			_Control& operator*() const;
			_Control* operator->() const;
		
		private:
			iterator(const container::iterator& r);
			
			friend _Manager;
		};
		class const_iterator : public container::const_iterator
		{
		public:
			//-----------------------------------------------------------------
			const_iterator();
			const_iterator(const const_iterator& r);
			const_iterator(const iterator& r);

			const_iterator& operator=(const const_iterator& r);
			
			//-----------------------------------------------------------------
			const _Control& operator*() const;
			const _Control* operator->() const;
		
		private:
			const_iterator(const container::const_iterator& r);
			
			friend _Manager;
		};

	public:		
		iterator begin();

		const_iterator begin() const;
		
		iterator end();

		const_iterator end() const;
		
	//-------------------------------------------------------------------------
	private:
		typedef boost::signal<void (_Manager& m)> controls_changed_slot;
	
	public:
		typedef controls_changed_slot::slot_type CONTROLS_CHANGED_EVENT;

	public:
		EVENTID connect_controls_changed(CONTROLS_CHANGED_EVENT f);
		
		void disconnect_controls_changed(EVENTID e);
		
		void erase(_Control& c);
		
		void erase(iterator i);
	
		iterator insert(_Control& c);

		iterator insert(iterator i, _Control& c);

	protected:
		virtual void controls_changed();
	
	private:
		container _controls;
	
		controls_changed_slot _controlsChangedHandlers;
		
	//-------------------------------------------------------------------------
	public:
		int size() const;

	//-------------------------------------------------------------------------
	public:
		Theme& theme() const;
		
		void theme(Theme& t);

	private:
		void theme_updater(control_info* c);

	private:		
		Theme* _theme;
	
	//-------------------------------------------------------------------------
	protected:
		_Manager(BITMAP* b = screen, Theme& t = *ActiveTheme);
		
		virtual ~_Manager();
		
		void delete_control(control_info* ci);
	};
}

#endif
