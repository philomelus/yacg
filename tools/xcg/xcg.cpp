#include <cassert>
#include <ios>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>

#include "Common.hpp"
#include "Generate.hpp"
#include "Parse.hpp"

using namespace impl;

using std::endl;
using std::cout;
using std::cerr;
using boost::bind;
using boost::filesystem::path;
using boost::filesystem::filesystem_error;

namespace
{
//=============================================================================
// Determines if the file specified by f actually exits on disk

	void exists(const std::string& f, bool& abort)
	{
		try
		{
			path p(f, boost::filesystem::native);
			if (!boost::filesystem::exists(p))
			{
				cerr << "'" << f << "' doesn't exist!" << endl;
				abort = true;
			}
		}
		catch (...)
		{
			cerr << "Unable to determine if file \"" << f << "\" exists." << endl;
			abort = true;
		}
	}

//=============================================================================
// Generate source for file specified in f.

	void generate(const std::string& f, bool& abort)
	{
		ELEMENTS e;
		std::string r;
		impl::Parser p(f.c_str(), e, r, abort);
		if (!abort)
			impl::Generator(str_dir(f.c_str()), r, e);
	}

//=============================================================================
// Makes sure the directory f exists as a directory, either by creating it
// or verifying that the directory already exists as a directroy.

	void makedir(const std::string& f, bool& abort)
	{
		path dir(str_dir(f.c_str()), boost::filesystem::native);
		if (boost::filesystem::exists(dir))
		{
			if (!boost::filesystem::is_directory(dir))
			{
				cerr << "Source for \"" << f << "\" is supposed to go into directory \""
						<< dir.native_directory_string() << "\", but directory exists as a file!" << endl;
				abort = true;
			}
		}
		else
		{
			// Create directory
			try
			{
				boost::filesystem::create_directory(dir);
				cout << "Created directory \"" << dir.native_directory_string() << "\"" << endl;
			}
			catch (...)
			{
				// NOTE:  I'd like to use filesystem_error::what(), but the
				// constructors don't provide a way to change the "who" to my
				// name.  I don't want users to know how this is implemented,
				// and the who is set to a boost filesystem identification string,
				// and there isn't a way to change it as far as I can tell.
				cerr << "Unable to create directory \"" << dir.native_directory_string() << "\"!" << endl;
				abort = true;
			}
		}
	}

//=============================================================================
// Encapsulates the command line options.

	class Operation
	{
	public:
		Operation(int c, const char** a);
	
		bool abort() const;
		
		bool help() const;
		void help(bool h);
		
		std::vector<std::string>& source();
		const std::vector<std::string>& source() const;
	
	private:
		void generate(int c, const char** a, int& i);
		void write_default(int c, const char** a, int& i);
		
	private:
		bool _abort;
		std::vector<std::string> _source;
		bool _help;
	};
}

//=============================================================================
// class Operation implementation

Operation::Operation(int c, const char** a)
		:
		_abort(false),
		_help(false)
{
	// Validate command line options
	if (c == 1)
		_help = true;
	else
	{
		for (int i = 1; i < c; ++i)
		{
			std::string arg(a[i]);
			if (arg[0] != '-')
			{
				cerr << "Unknown argument " << i << ": \"" << arg << "\"" << endl;
				_abort = true;
				break;
			}
			if (arg[1] == '-')
			{
				// Check for full option names
				if (arg == "--help")
				{
					_help = true;
					break;
				}
				else if (arg == "--generate")
					generate(c, a, i);
				else if (arg == "--default")
					write_default(c, a, i);
				else
				{
					_help = true;
					break;
				}
			}
			else
			{
				// Check for short option names
				assert(arg.size() == 2);
				switch (arg[1])
				{
				case '?':
				case 'h':
				case 'H':
					_help = true;
					break;
				
				case 'g':
					generate(c, a, i);
					break;
				}
			}
			if (_help)
				break;
			if (_abort)
				break;
		}
	}
}

bool Operation::abort() const
{
	return _abort;
}

void Operation::generate(int c, const char** a, int& i)
{
	// Make sure we have another argument available
	if (c == i + 1)
	{
		cerr << "Missing file argument for --generate!" << endl;
		_help = true;
		return;
	}
	
	// Add next argument to source list
	++i;
	_source.push_back(a[i]);
}

inline bool Operation::help() const
{
	return _help;
}

inline void Operation::help(bool h)
{
	assert(_help == false);
	_help = h;
}

inline std::vector<std::string>& Operation::source()
{
	return _source;
}

inline const std::vector<std::string>& Operation::source() const
{
	return _source;
}

void Operation::write_default(int c, const char** a, int& i)
{
	// Make sure we have another argument available
	if (c == i + 1)
	{
		cerr << "Missing file argument for --defaults!" << endl;
		_help = true;
		return;
	}

	// This is an autonomous operation	
	_abort = true;

	// Create/open the designated file
	++a;
	std::ofstream f(a[i], std::ios_base::out | std::ios_base::trunc);
	if (!f)
	{
		cerr << "Unable to create/open \"" << f << "\" for default xml.  Operation aborted!" << endl;
		return;
	}
	
	// Write the file
	f << xmlDefault;
}

//=============================================================================
// function	main

int	main (int argc,	char** argv)
{
	// Parse command line
	Operation o(argc, const_cast<const char**>(argv));
	if (o.abort())
		return 1;
	
	// Display help if needed
	if (o.help())
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
		return 2;
	}

	// Make sure all source files exist
	const std::vector<std::string>& gen = o.source();
	bool abort = false;
	std::for_each(gen.begin(), gen.end(), bind(exists, _1, boost::ref(abort)));
	if (abort)
		return 3;
	
	// Make sure there is a directory for all source files
	std::for_each(gen.begin(), gen.end(), bind(makedir, _1, boost::ref(abort)));
	if (abort)
		return 4;

	// Generate source for each XML file
	std::for_each(gen.begin(), gen.end(), bind(generate, _1, boost::ref(abort)));
	if (abort)
		return 5;
	
	return 0;
}
