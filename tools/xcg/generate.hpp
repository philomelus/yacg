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
		Generator(const std::string& d, const std::string& r, const ELEMENTS& e);

	private:	
		void sort_elements(const ELEMENTS::value_type& c, std::vector<std::string>& ve) const;
		void sort_include(const std::string& c, std::vector<std::string>& ve) const;
		
	private:
		const std::string _directory;
		const ELEMENTS& _elements;

	//-------------------------------------------------------------------------
	// Add license agreement to stream.
	private:
		void license(std::ostream& o) const;

	//-------------------------------------------------------------------------
	// Header file generation	
	private:
		void header(const std::vector<std::string>& c, const std::string& r, std::ostream& o) const;
		void declaration(const std::string& n, std::ostream& o, const std::string& l) const;
		void declaration_attributes(const std::string& v, std::ostream& o) const;
		void declaration_elements(const std::string& v, std::ostream& o) const;

	//-------------------------------------------------------------------------
	// Inline file generation	
	private:
		void inlines(const std::vector<std::string>& c, const std::string& r, std::ostream& o) const;
		void inline_implementation(const std::string& n,std::ostream& o, const std::string& l) const;
		void inline_attributes(const std::string& v, std::ostream& o, const std::string& c) const;
		void inline_elements(const std::string& v, std::ostream& o, const std::string& c) const;
		
	//-------------------------------------------------------------------------
	// Source file generation	
	private:
		void source(const std::vector<std::string>& c, const std::string& r, std::ostream& o) const;
		void source_implementation(const std::string& n,std::ostream& o, const std::string& l) const;
		void source_ope_attributes(const std::string& v, std::ostream& o) const;
		void source_ope_elements(const std::string& v, std::ostream& o) const;
		void source_read_attributes(const std::string& v, std::ostream& o,
				const ATTRIBUTES::const_iterator b, const ATTRIBUTES::const_iterator e) const;
		void source_read_elements(const std::string& v, std::ostream& o) const;
		void source_write_attributes(const std::string& v, std::ostream& o) const;
		void source_write_elements(const std::string& v, std::ostream& o,
				const std::string& f, const std::string& l) const;
	};
}

#endif
