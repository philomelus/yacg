#include <ios>
#include <iostream>

extern void Test1();
extern void Test1();
extern void Test1();

int main(int c, const char* a[])
{
	Test1();
//	Test2();
//	Test3();
	return 0;
}

#if 0
#include <ios>
#include <iostream>
#include <libxml/parser.h>
#include <string>
#include <hex.hpp>
#include <strstream>

std::string& str_clean(std::string& s)
{
restart:
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] < 32 || s[i] > 127)
		{
			std::ostrstream o;
			o << utility::hex(s[i]) << std::ends;
			s.erase(i, 1);
			s.insert(i, o.str());
			goto restart;
		}
	}
	return s;
}

std::string& str_trim(std::string& s)
{
return s;
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] > 32 && s[i] < 128)
			break;
	}
	if (i < s.size())
		s.erase(0, i);
	else
	{
		s.clear();
		return s;
	}
	for (i = s.size() - 1; i >= 0; --i)
	{
		if (s[i] > 32 && s[i] < 128)
			break;
	}
	if (i < s.size() - 1)
		s.erase(i + 1);
	return s;
}

#define	MAKESURE(c)							\
if (c)										\
{											\
	std::cout << "Oh, fuck!" << std::endl;	\
	exit(1);								\
}

void recurse(xmlNodePtr r, int t = 0)
{
	for (xmlNodePtr n = r; n; n = n->next)
	{
		// Dump nodes at same level
		if (n->type == XML_ELEMENT_NODE)
		{
			std::cout << "<";
			std::string element((const char*) n->name);
			std::cout << element;

			// Dump the attributes
			for (xmlAttrPtr a = n->properties; a; a = a->next)
			{
				MAKESURE(a->children->type != XML_TEXT_NODE);
				std::cout << " " << a->name << "=\"" << (const char*) xmlNodeGetContent(a->children) << "\"";
			}
			std::cout << ">";

			// Find the value for this node
			for (xmlNodePtr c = n->children; c; c = c->next)
			{
				if (c->type != XML_TEXT_NODE)
					continue;
				std::string s((const char*) xmlNodeGetContent(c));
				str_trim(s);
				if (s.size() > 0)
					std::cout << s;
			}
			
			// Recurse into any child nodes
			recurse(n->children, t + 1);

			std::cout << "</" << element << ">";
		}
	}
}

int main(int c, const char* a[])
{
	xmlInitParser();
	xmlLineNumbersDefault(1);
	xmlDocPtr d = xmlParseFile(a[1]);
	recurse(xmlDocGetRootElement(d));
	
	return 0;
}
#endif
