#include "Common.hpp"

#include <algorithm>
#include <iostream>

#include "Parse.hpp"

using namespace impl;
using std::cerr;
using std::cout;
using std::endl;

//=============================================================================
// class Parser implementation
//=============================================================================

Parser::Parser(const char* f, ELEMENTS& e, bool& abort)
		:
		_elements(e)
{
	// Parse the file
	cout << "Parsing \"" << f << "\"..." << endl;
	xmlDocPtr doc = xmlParseFile(f);
	if (doc == 0)
	{
		cerr << "File \"" << f << "\" contained errors - skipping!" << endl;
		abort = true;
		return;
	}
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
		ei = _elements.insert(std::make_pair(xmlChar2char(n->name), Element())).first;

	// Add attributes
	if (n->properties)
	{
		ATTRIBUTES& attr = ei->second.attributes();
		for (xmlAttrPtr a = n->properties; a; a = a->next)
		{
			const std::string name(xmlChar2char(a->name));
			if (std::find(attr.begin(), attr.end(), name) == attr.end())
				attr.push_back(name);
		}
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
				const std::string name(xmlChar2char(s->name));
				SUBELEMENTS& e = r.elements();
				if (std::find(e.begin(), e.end(), name) == e.end())
					e.push_back(name);
			}

			// Check for recursive sub-elements
			recurse(s);
		}
	}

#undef xmlChar2char
}
