#include "precompiled.hpp"
#include "slider.hpp"

Slider::Slider()
{
	m_X = -1;
	m_Y = -1;
	m_X2 = -1;
	m_Y2 = -1;
	m_Width = -1;
	m_Height = -1;
	m_Initialized = false;
}

bool Slider::HitTest(int mx, int my)
{
	if (m_Initialized)
	{
		if ((mx >= m_X && mx <= m_X2)
			&&	(my >= m_Y && my <= m_Y2))
		{
			return true;
		}
	}

	return false;
}

bool Slider::Initialize(int x, int y, int w, int h, int nFlags)
{
	m_X = x;
	m_Y = y;
	m_X2 = x + w - 1;
	m_Y2 = y + h - 1;
	m_Width = w;
	m_Height = h;
	m_Initialized = true;

	return true;
}

void Slider::Paint(void *pAction, bool disabled, bool depressed)
{
	int		tlc;
	int		itlc;
	int		ibrc;
	int		brc;
	int		tc;
	int		to;

	if (m_Initialized)
	{
		// Figure out color of edges	
		if (depressed)
		{
			tlc = COLOR_VDGRAY;
			itlc = COLOR_DKGRAY;
			ibrc = COLOR_LTGRAY;
			brc = COLOR_WHITE;
			to = 1;
		}
		else
		{
			tlc = COLOR_WHITE;
			itlc = COLOR_LTGRAY;
			ibrc = COLOR_DKGRAY;
			brc = COLOR_VDGRAY;
			to = 0;
		}
	
		// Figure out color of title
		if (disabled)
			tc = COLOR_DKGRAY;
		else
			tc = COLOR_VDGRAY;
		
//		g_MousePtr.AddRef();
		{
			// top/left
			hline(screen, m_X, m_Y, m_X2, tlc);
			vline(screen, m_X, m_Y + 1, m_Y2, tlc);
			// inset top/left
			hline(screen, m_X + 1, m_Y + 1, m_X2 - 1, itlc);
			vline(screen, m_X + 1, m_Y + 2, m_Y2 - 1, itlc);
			// inset bottom/right
			hline(screen, m_X + 2, m_Y2 - 1, m_X2 - 1, ibrc);
			vline(screen, m_X2 - 1, m_Y + 2, m_Y2 - 2, ibrc);
			// bottom/right
			hline(screen, m_X + 1, m_Y2, m_X2, brc);
			vline(screen, m_X2, m_Y + 1, m_Y2 - 1, brc);
			// inside
			rectfill(screen, m_X + 2, m_Y + 2, m_X2 - 2, m_Y2 - 2, COLOR_GRAY);
		}
//		g_MousePtr.Release();
	}
}

void Slider::Terminate()
{
}
