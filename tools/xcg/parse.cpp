#include "Common.hpp"
#include "Parse.hpp"

using namespace impl;

namespace
{
	void log_attrs(xmlNodePtr node, ELEMENTS& elements)
	{
		// Ignore white space
		if (node->name == NULL)
			return;

		// Locate node, creating it if needed
		ELEMENTS::iterator ei = elements.find(reinterpret_cast<const char*>(node->name));
		if (ei == elements.end())
		{
			ei = elements.insert(std::make_pair(reinterpret_cast<const char*>(node->name),
					Element(reinterpret_cast<const char*>(node->name)))).first;
		}

		// Add attributes
		if (node->properties)
		{
			for (xmlAttrPtr prop = node->properties; prop; prop = prop->next)
			{
				std::string	attr_name(reinterpret_cast<const char*>(prop->name));
				ei->second.attributes().push_back(attr_name);
			}
		}
	}

	void add_sub(ELEMENTS& elements, xmlNodePtr node, xmlNodePtr sub)
	{
		if (sub->name == NULL)
			return ;

		if (xmlNodeIsText(sub))
			return ;

		std::string	name(reinterpret_cast<const	char*>(node->name));
		Element& r	= elements[name];
		std::string	subname(reinterpret_cast<const char*>(sub->name));
		if (r.subs().count(subname) == 0)
		{
			r.subs()[subname]	= 1;
		}
		else
		{
			r.subs()[subname]++;
		}
	}
}

void impl::find_nodes(ELEMENTS& elements, xmlNodePtr node)
{
	// Ignore white space
	if (node->name == NULL)
		return;

	// Save attributes
	log_attrs(node, elements);

	// Look for sub-nodes
	for	(xmlNodePtr sub = node->xmlChildrenNode; sub; sub = sub->next)
	{
		add_sub(elements, node, sub);
		find_nodes(elements, sub);
	}
}
