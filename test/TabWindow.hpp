#ifndef	__TABWINDOW_HPP__
#define	__TABWINDOW_HPP__

//=============================================================================
// class _TabWindowBase
//=============================================================================

template <class B>
class _TabWindowBase
{
protected:
	void process(yacg::_Manager& w)
	{
		const int size = w.size();
		static_cast<yacg::Button&>(w[1]).callback(exit, this);
		static_cast<B*>(this)->_window.process_events();
	}

private:
	static void exit(yacg::Button& c, void* data, BITMAP* b)
	{
		reinterpret_cast<B*>(data)->_window.abort();
	}
};

#include "TabBottomWindows.hpp"
#include "TabLeftWindows.hpp"
#include "TabRightWindows.hpp"
#include "TabTopWindows.hpp"

#endif
