#ifndef	__GENERATE_HPP__
#define	__GENERATE_HPP__

#ifndef	__COMMON_HPP__
#include "Common.hpp"
#endif

namespace impl
{
//=============================================================================
// Given a root ELEMENT, generate the appropriate class source and headers.

	class Generater
	{
	//----------------------------------------------------------------------------
	public:
		Generater(const std::string& d, const ELEMENTS& e);
	};
}

#endif
