#include "Common.hpp"
#include "Parse.hpp"

using namespace impl;

//=============================================================================
// class Parser implementation
//=============================================================================

Parser::Parser(const char* f, ELEMENTS& e)
		:
		_elements(e)
{
	xmlDocPtr doc = xmlParseFile(f);
	xmlNodePtr root = xmlDocGetRootElement(doc);
	if (root)
		recurse(root);
	xmlFreeDoc(doc);
}

void Parser::recurse(xmlNodePtr n)
{
#define	xmlChar2char(c)	reinterpret_cast<const char*>(c)

	// Ignore white space
	if (n->name == NULL)
		return;

	// Locate node, creating it if needed
	std::string name(xmlChar2char(n->name));
	ELEMENTS::iterator ei = _elements.find(name);
	if (ei == _elements.end())
	{
		ei = _elements.insert(std::make_pair(xmlChar2char(n->name), Element(
				xmlChar2char(n->name)))).first;
	}

	// Add attributes
	if (n->properties)
	{
		ATTRIBUTES& attr = ei->second.attributes();
		for (xmlAttrPtr a = n->properties; a; a = a->next)
			attr.push_back(xmlChar2char(a->name));
	}

	// Parse sub-elements if needed
	if (n->xmlChildrenNode)
	{
		for	(xmlNodePtr s = n->xmlChildrenNode; s; s = s->next)
		{
			// Add sub-element info
			if (s->name && !xmlNodeIsText(s))
			{
				Element& r = _elements[xmlChar2char(n->name)];
				std::string	sname(xmlChar2char(s->name));
				SUBELEMENTS& e = r.elements();
				if (e.count(sname) == 0)
					e[sname] = 1;
				else
					++(e[sname]);
			}

			// Check for recursive sub-elements
			recurse(s);
		}
	}

#undef xmlChar2char
}
