#ifndef	__PARSE_HPP__
#define	__PARSE_HPP__

#include <libxml/parser.h>

namespace impl
{
//=============================================================================
// Given a file in XML 1.0/1.1 compatible format, generate a hierarchy of
// elements and attributes contained within the file.
// NOTE:  Only ASCII currently supported.

	class Parser
	{
	//-------------------------------------------------------------------------
	public:
		Parser(const char* f, ELEMENTS& e);

	private:
		void recurse(xmlNodePtr n);
			
	private:
		ELEMENTS& _elements;
	};
}

#endif
