#ifndef	__YACG_WINDOW_HPP__
#define	__YACG_WINDOW_HPP__

namespace yacg
{
//=============================================================================
// class _Window declaration
//=============================================================================

	class _Window : public _Manager
	{
	public:
		_Window(BITMAP* b = screen);
	};

//=============================================================================
// template class Window declaration
//=============================================================================

	template <class T>
	class Window
	{
	public:
		// No arguments
		Window()
		{
			T t;
			t.process_events();
		}
		// 1 argument
		template <typename P1>
		Window(typename P1 p1)
		{
			T t(p1);
			t.process_events();
		}
		// 2 arguments
		template <class P1, class P2>
		Window(typename P1 p1, typename P2 p2)
		{
			T t(p1, p2);
			t.process_events();
		}
		// 3 arguments
		template <class P1, class P2, class P3>
		Window(typename P1 p1, typename P2 p2, typename P3 p3)
		{
			T t(p1, p2, p3);
			t.process_events();
		}
		// 4 arguments
		template <class P1, class P2, class P3, class P4>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4)
		{
			T t(p1, p2, p3, p4);
			t.process_events();
		}
		// 5 arguments
		template <class P1, class P2, class P3, class P4, class P5>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5)
		{
			T t(p1, p2, p3, p4, p5);
			t.process_events();
		}
		// 6 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6)
		{
			T t(p1, p2, p3, p4, p5, p6);
			t.process_events();
		}
		// 7 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6, typename P7 p7)
		{
			T t(p1, p2, p3, p4, p5, p6, p7);
			t.process_events();
		}
		// 8 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6,
				class P7, class P8>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6, typename P7 p7, typename P8 p8)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8);
			t.process_events();
		}
		// 9 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6,
				class P7, class P8, class P9>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6, typename P7 p7, typename P8 p8,
				typename P9 p9)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8, p9);
			t.process_events();
		}
		// 10 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6,
				class P7, class P8, class P9, class P10>
		Window(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6, typename P7 p7, typename P8 p8,
				typename P9 p9, typename P10 p10)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
			t.process_events();
		}
	};
}

#endif
