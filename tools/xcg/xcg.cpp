
#include <ios>
#include <iostream>
#include <locale>

#include "Common.hpp"
#include "Generate.hpp"
#include "Parse.hpp"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

using namespace impl;

using std::endl;
using std::cout;
using std::cerr;
using boost::filesystem::path;
using boost::filesystem::filesystem_error;

//=============================================================================
// LOCAL
//=============================================================================

namespace
{
	void dump(ELEMENTS& elements)
	{
		int i;
		ELEMENTS::iterator node_map_it;
		SUBS::iterator subs_it;

		for (node_map_it = elements.begin(); node_map_it != elements.end();
				node_map_it ++)
		{
			Element& r = (*node_map_it).second;
//			printf("Node Name: %s\n", r.name.c_str());
			printf("Node Name: %s\n", node_map_it->first.c_str());

			for (i = 0; i < (int)r.attributes().size(); i++)
			{
				printf("Attribute: %s\n", r.attributes()[i].c_str());
			}

			for (subs_it = r.subs().begin();subs_it != r.subs().end();subs_it++)
			{
				printf("Sub-Nodes: %s %d\n", (*subs_it).first.c_str(),
					(*subs_it).second);
			}

			printf("\n\n");
		}
	}

	std::string& str_lower(std::string& s)
	{
		std::locale l;
		const std::string::iterator e = s.end();
		for (std::string::iterator i = s.begin(); i != e; ++i)
			*i = std::tolower(*i, l);
		return s;
	}
	
	std::string str_dir(const char* f)
	{
		std::string dir(f);
		str_lower(dir);
		return std::string(f, dir.rfind(".") == 0 ? dir.size() : dir.rfind("."));
	}
}

//=============================================================================
// function	main
//=============================================================================

int	main (int argc,	char** argv)
{
	// Display help if desired
	bool help = false;
	for (int i = 1; i < argc; ++i)
	{
		if (stricmp(argv[i], "--help") == 0
				|| stricmp(argv[i], "-h") == 0
				|| stricmp(argv[i], "/h") == 0
				|| stricmp(argv[i], "-?") == 0
				|| stricmp(argv[i], "/?") == 0)
		{
			help = true;
			break;
		}
	}
	if (argc == 1)
		help = true;
	if (help)
	{
		cout << "Usage:" << endl;
		cout << "    " << argv[0] << " [options] filename [filename ...]" << endl;
		cout << std::endl;
		cout << "Options:" << endl;
		cout << "    --help = Display this help." << endl;
		cout << endl;
		cout << "Generates classes for reading/writing the specified XML files via libxml2." << endl;
		cout << "Source files for each XML file will be placed in a directory with the extension" << endl;
		cout << "of the filename removed.  Files will have the extension cpp and hpp." << endl;
		return 1;
	}

	// Make sure all source files exist
	bool exit = false;
	for (i = 1; i < argc; ++i)
	{
		if (!boost::filesystem::exists(argv[i]))
		{
			cerr << "'" << argv[i] << "' doesn't exist!" << endl;
			exit = true;
		}
	}
	if (exit)
		return 2;
	
	// Make sure there is a directory for all source files
	exit = false;
	for (i = 1; i < argc; ++i)
	{
		std::string dir = str_dir(argv[i]);
		if (boost::filesystem::exists(dir))
		{
			if (!boost::filesystem::is_directory(dir))
			{
				cerr << "Source for \"" << argv[i] << "\" is supposed to go into directory \"";
				cerr << dir << "\", but directory exists as a file!" << endl;
				exit = true;
			}
		}
		else
		{
			// Create directory
			try
			{
				boost::filesystem::create_directory(dir);
				cout << "Created directory \"" << dir << "\"" << endl;
			}
			catch (const filesystem_error&)
			{
				// NOTE:  I'd like to use e.what(), but the constructors don't
				// provide a way to change the "who" to my name.  I don't want
				// users to know how this is implemented, and the who is set to
				// a boost filesystem identification string, and there isn't a
				// way to change it as far as I can tell.
				cerr << "Unable to create directory \"" << dir << "\"!" << endl;
				exit = true;
			}
		}
	}
	if (exit)
		return 3;

	// Generate source for each XML file
	ELEMENTS elements;
	for (int i = 1; i < argc; ++i)
	{
		elements.clear();
		std::string dir = str_dir(argv[i]);
		xmlDocPtr doc = xmlParseFile(argv[i]);
		xmlNodePtr root = xmlDocGetRootElement(doc);
		if (root)
		{
			// Generate internal structure
			find_nodes(elements, root);
			
			// Generate code
			gen_code(dir, elements);
		}
		xmlFreeDoc(doc);
	}
	
	return 0;
}
