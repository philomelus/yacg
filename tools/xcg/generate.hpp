#ifndef	__GENERATE_HPP__
#define	__GENERATE_HPP__

#include <fstream>

#ifndef	__COMMON_HPP__
#include "Common.hpp"
#endif

namespace impl
{
//=============================================================================
// Given a root ELEMENT, generate the appropriate class source and headers.

	class Generator
	{
	//-------------------------------------------------------------------------
	private:
		typedef std::vector<std::string> LIST;
		
	//-------------------------------------------------------------------------
	public:
		Generator(const std::string& d, const ELEMENTS& e);
	
	private:
		const std::string _directory;

	//-------------------------------------------------------------------------
	// Header file generation	
	private:
		void header(const ELEMENTS::value_type& v);
		void header_attributes(const std::string& v, std::ofstream* o);
		void header_elements(const std::string& v, std::ofstream* o);
		void header_includes(const std::string& v, std::ofstream* o);
	
	//-------------------------------------------------------------------------
	// Source file generation	
	private:
		void source(const ELEMENTS::value_type& v);
		void source_attributes_copy(const std::string& v, std::ofstream* o);
		void source_attributes_read(const std::string& v, std::ofstream* o);
		void source_attributes_write(const std::string& v, std::ofstream* o);
		void source_elements_copy(const std::string& v, std::ofstream* o);
		void source_elements_read(const std::string& v, std::ofstream* o);
		void source_elements_write(const std::string& v, std::ofstream* o);
	};
}

#endif
