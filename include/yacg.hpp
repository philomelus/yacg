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

	//=========================================================================
	extern void yacg_init();

	//=========================================================================
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
	//=========================================================================
	public:
		static const int calc = -1;
		
		static const int undefined = -2;
		
		static const int dirty_all = 0xFFFFFFFF;

		static const int update_display = 0x80000000;

	//=========================================================================
	// construction / destruction
	
	public:
		_Control(_Manager& m, int f = 0);
		
		virtual ~_Control();
		
	//=========================================================================
	// active
	
	private:
		typedef boost::signal<void (_Control* c, bool n)> active_changed_slot;
		
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
		
	//=========================================================================
	// dirty
	
	private:
		typedef boost::signal<void (_Control* c, int o)> dirty_changed_slot;
		
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
	
	//=========================================================================
	// flags
	
	private:
		typedef boost::signal<void (_Control* c)> flags_changed_slot;
		
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
	
	//=========================================================================
	public:
		_Manager& manager() const;

		void manager(_Manager& m);
				
	private:
		_Manager* _manager;
	
	//=========================================================================
	// theme

	private:
		typedef boost::signal<void (_Control* c)> theme_changed_slot;
		
	public:
		typedef theme_changed_slot::slot_type THEME_EVENT;
		
	public:
		Theme& theme() const;
		
		void theme(Theme& t);
	
		EVENTID connect_theme(THEME_EVENT f);

		void disconnect_theme(EVENTID f);

	protected:
		virtual void theme_changed();

		int _themeChangedHint;
	
	private:
		void theme_signal(Theme* t);
		
	private:
		Theme* _theme;

		theme_changed_slot _themeHandlers;

		EVENTID _themeSignal;
		
	//=====================================================================
	// visibility
	
	private:
		typedef boost::signal<void (_Control* c)> visible_changed_slot;
		
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
				
	//=========================================================================
	public:
		virtual void paint(BITMAP* bmp) = 0;
		
	//=========================================================================
#ifdef	_DEBUG
		virtual void dump() const = 0;
#endif
	
	//=========================================================================
	private:
		// Used by _Manager to track the connections made by Manager on behalf
		// of this control.
		std::vector<EVENTID> _connections;

		friend _Manager;
	};

//=============================================================================
// class _ControlEx declaration : _Control with keyboard and mouse handling
//=============================================================================

	class _ControlEx : public _Control
	{
	//=========================================================================
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

	//=========================================================================
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
	
	//=========================================================================
	public:
		static const int claimed = 0x8000;
	
	//=========================================================================
	public:
		_ControlEx(_Manager& m, int f = 0);
		
	//=========================================================================
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
		typedef boost::signal<bool (_ControlEx* c, BITMAP* bmp, int f, int x, int y, int m), isTrue> mouse_down_slot;
	
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
	
	//=========================================================================
	private:
		typedef boost::signal<void (_ControlEx* c, BITMAP* bmp, int f, int x, int y)> mouse_move_slot;
	
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
	
	//=========================================================================
	private:
		friend _Manager;
	};

//=============================================================================
// class _Manager declaration
//=============================================================================

	class _Manager : private boost::noncopyable
	{
	//=========================================================================
	private:
		typedef std::vector<_Control*> container;
		
	//=========================================================================
	/// bitmap
		
	public:
		BITMAP* bitmap() const;
		
		void bitmap(BITMAP* bmp);

	private:
		BITMAP* _bitmap;
		
	//=========================================================================
	public:
#ifdef	_DEBUG
		virtual void dump() const;
#endif

	//=========================================================================
	/// painting

	public:		
		void dirty();

		void paint();
		
		void paint(_Control& c);
		
		void update();
		
		void update(_Control& c);

	private:
		void painter(_Control* c);

		void updater(_Control* c);

	private:		
		bool _dirty;

	//=========================================================================
	// mouse down event
	
	private:
		typedef boost::signal<bool (_Manager* m, int x, int y, int b), _ControlEx::isTrue> mouse_down_slot;
		typedef mouse_down_slot::slot_type MOUSE_DOWN_EVENT;

	public:
		EVENTID connect_mouse_down(MOUSE_DOWN_EVENT f);
		
		void disconnect_mouse_down(EVENTID f);

	private:
		mouse_down_slot _mouseDownHandlers;
		
		container _mouseDownControls;
		boost::signal<void (BITMAP* bmp, int x, int y, int b)> _mouseDownControlsPre;
		boost::signal<void (BITMAP* bmp, int x, int y, int b)> _mouseDownControlsPost;
		
	//=========================================================================
	// mouse move event
	
	private:
		typedef boost::signal<void (_Manager*, int, int)> mouse_move_slot;
		typedef mouse_move_slot::slot_type MOUSE_MOVE_EVENT;

	public:
		EVENTID connect_mouse_move(MOUSE_MOVE_EVENT f);

		void disconnect_mouse_move(EVENTID f);

	private:
		bool mouse_down(_ControlEx& c, int x, int y, int m);

	private:
		int _mouseX;
		int _mouseY;

		mouse_move_slot _mouseMoveHandlers;	// _Manager-wide

		boost::signal<void (BITMAP* bmp, int x, int y)> _mouseMoveControls;
		boost::signal<void (BITMAP* bmp, int x, int y)> _mouseMoveControlsPre;
		boost::signal<void (BITMAP* bmp, int x, int y)> _mouseMoveControlsPost;

	//=========================================================================
	// common event
	
	private:
		typedef boost::signal<void (_Manager* m)> idle_slot;
		typedef idle_slot::slot_type IDLE;

	public:
		void abort();
		
		void process_events(IDLE f);

		void process_events();
		
		std::pair<bool, bool> process_events_and_return();

		void reset();

	private:
		bool _aborted;

		container _controls;
		
	//=========================================================================
	public:
		_Control& operator[](int i) const;

	//=========================================================================
	/// iteration
		
	public:
		typedef container::iterator iterator;

		typedef container::const_iterator const_iterator;

	public:		
		iterator begin();

		const_iterator begin() const;
		
		iterator end();

		const_iterator end() const;
		
	//=========================================================================
	public:
		void erase(_Control& c);
		
		void erase(iterator i);
	
	private:
		void disconnect_control_events(EVENTID& id);
	
	//=========================================================================
	public:
		iterator insert(_Control& c);
		
	//=========================================================================
	public:
		int size() const;

	//=========================================================================
	public:
		Theme& theme() const;
		
		void theme(Theme& t);

	private:
		void theme_updater(_Control* c);

	private:		
		Theme* _theme;
	
	//=========================================================================
	protected:
		_Manager(BITMAP* b = screen, Theme& t = *ActiveTheme);
		
		virtual ~_Manager();
	};
}

#endif
