#ifndef	__YACG_RADIO_HPP__
#define	__YACG_RADIO_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
	namespace RadioImpl
	{
		class RadioBase;
		class GroupImpl;
	}

	class Radio;
	
//=============================================================================
// class RadioGroup declaration
//=============================================================================

	class RadioGroup
	{
	public:
		typedef void (*CHANGED)(RadioGroup& g, Radio& n, void* d);
	
		typedef std::vector<Radio*>::iterator iterator;
		typedef std::vector<Radio*>::const_iterator const_iterator;
		
	public:
		// Frame
		static const int f_none			= 0x00 << 16;
		static const int f_flat			= 0x01 << 16;
		static const int f_in			= 0x02 << 16;
		static const int f_out			= 0x03 << 16;
		static const int f_mask			= 0x03 << 16;
		
		// Title (if frame is not f_none)
		static const int t_none			= 0x00 << 16;
		static const int t_title		= 0x04 << 16;
		static const int t_mask			= 0x04 << 16;
		
		// Title alignment
		static const int a_topleft		= 0x00 << 16;
		static const int a_topcenter	= 0x08 << 16;
		static const int a_topright		= 0x10 << 16;
		static const int a_bottomleft	= 0x18 << 16;
		static const int a_bottomcenter	= 0x20 << 16;
		static const int a_bottomright	= 0x28 << 16;
		static const int a_mask			= 0x38 << 16;

		// Radio options for Radio's created by object
		static const int r_righttext	= 0x00 << 16;
		static const int r_lefttext		= 0x40 << 16;
		static const int r_mask			= 0x40 << 16;
				
	public:
		RadioGroup(_Manager& m, int x = _Control::undefined, int y = _Control::undefined,
				int w = _Control::calc, int h = _Control::calc, const char* t = 0, int f = 0);
		virtual ~RadioGroup();
		
		int left() const;
		int right() const;
		
		int top() const;
		int bottom() const;
		
		const char* title() const;
		void title(const char* t);

		iterator begin();
		const_iterator begin() const;
		
		bool empty() const;
		
		iterator end();
		const_iterator end() const;
		
		void erase(Radio& r);
		
		iterator insert(Radio& r);
		iterator insert(iterator i, Radio& r);

		std::pair<iterator, iterator> insert(int c, const char** t);
		std::pair<iterator, iterator> insert(iterator i, int c, const char** t);

		std::pair<iterator, iterator> insert(const std::vector<std::string>& t);
		std::pair<iterator, iterator> insert(iterator i, const std::vector<std::string>& t);
		
		std::pair<iterator, iterator> insert(int c, Radio* r);
		std::pair<iterator, iterator> insert(iterator i, int c, Radio* r);

		std::pair<iterator, iterator> insert(const std::vector<Radio*>& r);
		std::pair<iterator, iterator> insert(iterator i, const std::vector<Radio*>& r);

		int size() const;
		
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		RadioImpl::GroupImpl* _impl;
	};
	
//=============================================================================
// class Radio declaration
//=============================================================================

	
	class Radio : public _ControlEx
	{
	public:
		typedef void (*CALLBACK)(Radio& r, void* d);
		
	public:
		static const int t_righttext	= 0x00 << 16;
		static const int t_lefttext		= 0x01 << 16;
		static const int t_mask			= 0x01 << 16;
		
	public:	// Common - client usage expected
		Radio(_Manager& m, int x, int y, const char* t, int f = 0);
		Radio(_Manager& m, int x, int y, int w, int h, const char* t, int f = 0);
		virtual ~Radio();

		int left() const;
		int right() const;
		
		int top() const;
		int bottom() const;
		
		int width() const;
		int height() const;
		
		std::pair<CALLBACK, void*> callback() const;
		void callback(CALLBACK f, void* d);

		bool state() const;
		void state(bool s);
		
		const char* title() const;
		void title(const char* t);
		
		const std::string& title_object() const;
				
	public:	// Uncommon - client usage rare, or only for derived
		bool mouse_down(BITMAP* b, int x, int y, int m);
		void paint(BITMAP* b);
		
#ifdef	_DEBUG
		void dump() const;
#endif

	private:
		RadioImpl::RadioBase* _impl;
	};
}

#endif
