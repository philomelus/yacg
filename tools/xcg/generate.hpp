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
		void header_attributes_declaration(const std::string& v, std::ofstream& o);
		void header_elements_declaration(const std::string& v, std::ofstream& o);
		void header_includes(const std::string& v, std::ofstream& o);

	//-------------------------------------------------------------------------
	// Inline file generation	
	private:
		void inlines(const ELEMENTS::value_type& v);
		void inlines_attributes(const std::string& v, const std::string& e, std::ofstream& o);
		void inlines_elements(const std::string& v, const std::string& e, std::ofstream& o);
		
	//-------------------------------------------------------------------------
	// Source file generation	
	private:
		void source(const ELEMENTS::value_type& v);
		void source_ope_attributes(const std::string& v, std::ofstream& o);
		void source_ope_elements(const std::string& v, std::ofstream& o);
		void source_read_attributes(const std::string& v, std::ofstream& o,
				const ATTRIBUTES::const_iterator b, const ATTRIBUTES::const_iterator e);
		void source_read_elements(const std::string& v, std::ofstream& o);
		void source_write_attributes(const std::string& v, std::ofstream& o);
		void source_write_elements(const std::string& v, std::ofstream& o);
	};
}

#endif
