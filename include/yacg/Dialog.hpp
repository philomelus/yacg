#ifndef	__YACG_DIALOG_HPP__
#define	__YACG_DIALOG_HPP__

#ifndef	__YACG_HPP__
#include <yacg.hpp>
#endif

#ifndef	__YACG_TITLE_HPP__
#include <yacg/Title.hpp>
#endif

namespace yacg
{
//=============================================================================
// Forward Declarations
//=============================================================================

	namespace DialogImpl
	{
		class Base;
	}
		
//=============================================================================
// class _Dialog declaration
//=============================================================================

	class _Dialog : public _Manager
	{
	public:
		typedef void (*IDLE)(_Dialog& w, void* d);
		
	public:
		// NOTE:  Unable to make this a single stage construct/display because
		// the derived class provides the virtual init() function, and since
		// its derived from me, it's not constructed until AFTER I am constructed.
		// I NEED to find a way to accomplish this becuase I really don't like it
		// the way that it is!
		_Dialog(int x, int y, int w, int h, const char* t = 0, BITMAP* bmp = screen);
		_Dialog(int w, int h, const char* t = 0, BITMAP* bmp = screen);
		~_Dialog();
		
		int left() const;
		int left_controls() const;

		int right() const;
		int right_controls() const;
		
		int top() const;
		int top_controls() const;

		int bottom() const;
		int bottom_controls() const;
		
		int width() const;
		int width_controls() const;

		int height() const;
		int height_controls() const;
		
		void execute();

		std::pair<IDLE, void*> callback() const;
		void callback(IDLE f, void* d);
		
		const char* title() const;
		void title(const char* title);
		
		yacg::Title& title_object() const;
		
	protected:
		virtual void close();							// Call to stop execution of dialog
		virtual void frame(BITMAP* bmp, int l, int t, int r, int b, int w, int h);		// Called to paint frame/background and calculate control area
		virtual void init() = 0;						// Create and initialize controls
		virtual void uninit();							// Clean up controls if needed
		
	protected:
		void left_controls(int lc);
		void right_controls(int rc);
		
		void top_controls(int tc);
		void bottom_controls(int bc);

	private:
		DialogImpl::Base* _impl;
		friend DialogImpl::Base;
	};
	
//=============================================================================
// template class Dialog declaration
//=============================================================================

	template <class T>
	class Dialog
	{
	public:
		// No arguments
		Dialog()
		{
			T t;
			t.execute();
		}
		// 1 argument
		template <typename P1>
		Dialog(typename P1 p1)
		{
			T t(p1);
			t.execute();
		}
		// 2 arguments
		template <class P1, class P2>
		Dialog(typename P1 p1, typename P2 p2)
		{
			T t(p1, p2);
			t.execute();
		}
		// 3 arguments
		template <class P1, class P2, class P3>
		Dialog(typename P1 p1, typename P2 p2, typename P3 p3)
		{
			T t(p1, p2, p3);
			t.execute();
		}
		// 4 arguments
		template <class P1, class P2, class P3, class P4>
		Dialog(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4)
		{
			T t(p1, p2, p3, p4);
			t.execute();
		}
		// 5 arguments
		template <class P1, class P2, class P3, class P4, class P5>
		Dialog(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5)
		{
			T t(p1, p2, p3, p4, p5);
			t.execute();
		}
		// 6 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6>
		Dialog(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6)
		{
			T t(p1, p2, p3, p4, p5, p6);
			t.execute();
		}
		// 7 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
		Dialog(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6, typename P7 p7)
		{
			T t(p1, p2, p3, p4, p5, p6, p7);
			t.execute();
		}
		// 8 arguments
		template <class P1, class P2, class P3, class P4, class P5, class P6,
				class P7, class P8>
		Dialog(typename P1 p1, typename P2 p2, typename P3 p3, typename P4 p4,
				typename P5 p5, typename P6 p6, typename P7 p7, typename P8 p8)
		{
			T t(p1, p2, p3, p4, p5, p6, p7, p8);
			t.execute();
		}
	};
}

#endif
