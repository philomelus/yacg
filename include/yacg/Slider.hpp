#ifndef	__SLIDER_HPP__
#define	__SLIDER_HPP__

#ifndef	__MOUSEMGR_HPP__
#include "mousemgr.hpp"
#endif

class Slider : public _Control
{
public:
	Slider();
	bool HitTest(int x, int y);
	bool Initialize(int x, int y, int w, int h, int nFlags);
	void Paint(void *pAction, bool disabled, bool depressed);
	void Terminate();
	
protected:
	Manager* m_pMMgr;
	int m_X;				// Horizontal position of upper left
	int m_Y;				// Vertical position of upper left
	int m_X2;				// m_X + m_Width - 1
	int m_Y2;				// m_Y + m_Height - 1
	int m_Width;			// Width
	int m_Height;			// Height
	bool m_Initialized;	
};

#endif
