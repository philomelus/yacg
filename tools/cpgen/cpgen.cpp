#include <boost/filesystem/operations.hpp>

//#include <libxml/tree.h>
#include <libxml/parser.h>
//#include <libxml/xmlmemory.h>
//#include <libxslt/xslt.h>
//#include <libxslt/xsltInternals.h>
//#include <libxslt/transform.h>
//#include <libxslt/xsltutils.h>
//#include <libxml/xpath.h>
//#include <libxml/xpathInternals.h>
//#include <libxml/HTMLparser.h>
//#include <libxml/HTMLtree.h>

#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using boost::filesystem::path;

bool validate(xmlNodePtr n);
bool generate();

int main(int argc, char* argv[])
{
	// Currently only support 1 file at a time
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " filename [...]\n";
		return 1;
	}

	// Make sure all source files exist
	for (int i = 1; i < argc; ++i)
	{
//		path fname(argv[i]);
		if (!boost::filesystem::exists(argv[i]))
		{
			std::cout << argv[i] << " doesn't exist!\n";
			return 2;
		}
	}
	
	// Read in and parse each control file
	bool error = false;
	for (i = 1; i < argc; ++i)
	{
		// Load file
		xmlDocPtr doc = xmlParseFile(argv[i]);
		
		// Validate and add control at same time
		bool ok = validate(xmlDocGetRootElement(doc));
		if (!ok)
			error = true;

		// Close file since its no longer needed		
		xmlFreeDoc(doc);
	}

	// If an error occured during validation, don't generate
	if (error)
		return 3;
	
	// Generate control source
	error = generate();
		
	// Return 4 for generation error
	return (error ? 4 : 0);
}

bool validate(xmlNodePtr n)
{
	return true;
}

bool generate()
{
	return true;
}
