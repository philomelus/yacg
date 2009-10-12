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
		Window(P1 p1)
		{
			T t(p1);
			t.process_events();
		}
		// 2 arguments
		template <typename P1, typename P2>
		Window(P1 p1, P2 p2)
		{
			T t(p1, p2);
			t.process_events();
		}
		// 3 arguments
		template <typename P1, typename P2, typename P3>
		Window(P1 p1, P2 p2, P3 p3)
		{
			T t(p1, p2, p3);
			t.process_events();
		}
		// 4 arguments
		template <typename P1, typename P2, typename P3, typename P4>
		Window(P1 p1, P2 p2, P3 p3, P4 p4)
		{
			T t(p1, p2, p3, p4);
			t.process_events();
		}
		// 5 arguments
		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		Window(P1 p1, P2 p2, P3 p3, P4 p4,
			   P5 p5)
		{
			T t(p1, p2, p3, p4, p5);
			t.process_events();
		}
		// 6 arguments
		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		Window(P1 p1, P2 p2, P3 p3, P4 p4,
			   P5 p5, P6 p6)
		{
			T t(p1, p2, p3, p4, p5, p6);
			t.process_events();
		}
		// 7 arguments
		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		Window(P1 p1, P2 p2, P3 p3, P4 p4,
			   P5 p5, P6 p6, P7 p7)
		{
			T t(p1, p2, p3, p4, p5, p6, p7);
			t.process_events();
		}
		// 8 arguments
		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
				  typename P7, typename P8>
		Window(P1 p1, P2 p2, P3 p3, P4 p4,
			   P5 p5, P6 p6, P7 p7, P8 p8)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8);
			t.process_events();
		}
		// 9 arguments
		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
				  typename P7, typename P8, typename P9>
		Window(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8, p9);
			t.process_events();
		}
		// 10 arguments
		template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6,
				  typename P7, typename P8, typename P9, typename P10>
		Window(P1 p1, P2 p2, P3 p3, P4 p4,
			   P5 p5, P6 p6, P7 p7, P8 p8,
			   P9 p9, P10 p10)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
			t.process_events();
		}
	};
}

#endif
