#ifndef	__PARSE_HPP__
#define	__PARSE_HPP__

#include <libxml/parser.h>

namespace impl
{
	void find_nodes(ELEMENTS& elements, xmlNodePtr node);
}

#endif
