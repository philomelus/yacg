
#include <ios>
#include <iostream>

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
	for (int i = 1; i < argc; ++i)
	{
		ELEMENTS e;
		impl::Parser p(argv[i], e);
		impl::Generator(str_dir(argv[i]), e);
	}
	
	return 0;
}
