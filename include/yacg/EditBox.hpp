#ifndef	__YACG_EDITBOX_HPP__
#define	__YACG_EDITBOX_HPP__

#ifndef	__YACG_HPP__
#include "yacg.hpp"
#endif

namespace yacg
{
//=============================================================================
// class EditBox declaration
//=============================================================================

	namespace EditboxImpl
	{
		class _EditboxImpl;
	}
	
	class Editbox : public _ControlEx
	{
	public:
		Editbox(int x, int y, int w, int h, int f);
		virtual ~Editbox();
		
	private:
		EditboxImpl::_EditboxImpl* _impl;
	};
}

#endif
