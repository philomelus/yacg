#include "Common.hpp"

#include <algorithm>
#include <iostream>
#include <ctime>
#include <boost/bind.hpp>

#include "Generate.hpp"

using namespace impl;
using boost::bind;
using boost::ref;
using std::cout;
using std::cerr;
using std::endl;

//=============================================================================
// class Generator implementation
//=============================================================================

Generator::Generator(const std::string& d, const std::string& r, const ELEMENTS& e)
		:
		_directory(d),
		_elements(e)
{
	// Determine order of classes in header
	std::vector<std::string> sorted;
	std::for_each(_elements.begin(), _elements.end(), bind(sort_elements,
			this, _1, ref(sorted)));

	{
		// Open header file
		std::string	filename(_directory);
		filename += "\\";
		filename +=	r;
		filename +=	".hpp";
		std::ofstream h(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
		if (!h)
		{
			cerr << "Unable to create/open '" << filename << "'" << endl;
			return;
		}
		cout << "Generating \"" << filename << "\":" << endl;
		
		// Generate the header
		header(sorted, r, h);
	}
	
	// Sort for order in implementation files	
	std::sort(sorted.begin(), sorted.end());

	{
		// Open inlines file
		std::string	filename(_directory);
		filename += "\\";
		filename +=	r;
		filename +=	".inl";
		std::ofstream i(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
		if (!i)
		{
			cerr << "Unable to create/open '" << filename << "'" << endl;
			return;
		}
		cout << "Generating \"" << filename << "\":" << endl;
		
		// Create the inline implementation
		inlines(sorted, r, i);
	}

	// Create the source
	{
		// Open inlines file
		std::string	filename(_directory);
		filename += "\\";
		filename +=	r;
		filename +=	".cpp";
		std::ofstream i(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
		if (!i)
		{
			cerr << "Unable to create/open '" << filename << "'" << endl;
			return;
		}
		cout << "Generating \"" << filename << "\":" << endl;
		
		// Create the inline implementation
		source(sorted, r, i);
	}
}

void Generator::declaration(const std::string& c, std::ostream& o, const std::string& l) const
{
	std::string empty;
	
	// Locate root element definition
	const ELEMENTS::const_iterator ei = _elements.find(c);
	
	// Cache some stuff for ease of use
	const Element& e = ei->second;
	const ATTRIBUTES& a = e.attributes();
	const SUBELEMENTS& s = e.elements();

	// Start class declaration
	std::string prefix(Header_Element_Prefix);
	update_keywords(prefix, empty, empty, ei->first);
	o << prefix;
	
	// Add attributes
	std::for_each(a.begin(), a.end(), bind(declaration_attributes, this,
			_1, ref(o)));
	
	// Add sub-elements
	std::for_each(s.begin(), s.end(), bind(declaration_elements, this,
			_1, ref(o)));
	
	// Close up class declaration
	std::string postfix(Header_Element_Postfix);
	update_keywords(postfix, empty, empty, ei->first);
	o << postfix;
	if (c != l)
		o << endl;
}

void Generator::declaration_attributes(const std::string& v, std::ostream& o) const
{
	const std::string empty;
	std::string s(Header_Attribute);
	update_keywords(s, empty, v, empty);
	o << s;
}

void Generator::declaration_elements(const std::string& v, std::ostream& o) const
{
	const std::string empty;
	std::string s(Header_Child);
	update_keywords(s, empty, empty, v);
	o << s;
}

void Generator::header(const std::vector<std::string>& h, const std::string& r,
		std::ostream& o) const
{
	// Generate header begining with keywords replaced
	std::string prefix(Header_Prefix);
	update_keywords(prefix, r);
	o << prefix;
	
	// Add all the class declarations
	std::for_each(h.begin(), h.end(), bind(declaration, this, _1, ref(o),
			ref(h[h.size() - 1])));
	
	// Generate header ending with keywords replaced
	std::string postfix(Header_Postfix);
	update_keywords(postfix, r);
	o << postfix;
}

void Generator::inline_attributes(const std::string& a, std::ostream& o,
		const std::string& c) const
{
	std::string empty;
	std::string s = Inline_Attribute;
	update_keywords(s, empty, a, empty, c);
	o << s;
}

void Generator::inline_elements(const std::string& a, std::ostream& o,
		const std::string& c) const
{
	std::string empty;
	std::string s = Inline_Child;
	update_keywords(s, empty, empty, a, c);
	o << s;
}

void Generator::inline_implementation(const std::string& c, std::ostream& o,
		const std::string& l) const
{
	std::string empty;
	
	// Locate root element definition
	const ELEMENTS::const_iterator ei = _elements.find(c);
	
	// Cache some stuff for ease of use
	const Element& e = ei->second;
	const ATTRIBUTES& a = e.attributes();
	const SUBELEMENTS& s = e.elements();

	// Start class inline implementation
	std::string prefix(Inline_Element_Prefix);
	update_keywords(prefix, empty, empty, empty, ei->first);
	o << prefix;
	
	// Add attributes
	std::for_each(a.begin(), a.end(), bind(inline_attributes, this, _1, ref(o),
			ref(ei->first)));
	
	// Add sub-elements
	std::for_each(s.begin(), s.end(), bind(inline_elements, this, _1, ref(o),
			ref(ei->first)));
	
	// Close up class inline implementation
	std::string postfix(Inline_Element_Postfix);
	update_keywords(postfix, empty, empty, empty, ei->first);
	o << postfix;
	if (c != l)
		o << endl;
}

void Generator::inlines(const std::vector<std::string>& c, const std::string& r,
		std::ostream& o) const
{
	// Generate inline begining with keywords replaced
	std::string prefix(Inline_Prefix);
	update_keywords(prefix, r);
	o << prefix;
	
	// Add all the inline implementation
	std::for_each(c.begin(), c.end(), bind(inline_implementation, this, _1,
			ref(o), ref(c[c.size() - 1])));
	
	// Generate inline ending with keywords replaced
	std::string postfix(Inline_Postfix);
	update_keywords(postfix, r);
	o << postfix;
}

void Generator::sort_elements(const ELEMENTS::value_type& c, std::vector<std::string>& ve) const
{
	// Walk through all sub-elements
	std::for_each(c.second.elements().begin(), c.second.elements().end(),
			bind(sort_include, this, _1, ref(ve), ref(c.first)));
	
	// Add this element if not already there
	if (std::find(ve.begin(), ve.end(), c.first) == ve.end())
		ve.push_back(c.first);
}

void Generator::sort_include(const std::string& c, std::vector<std::string>& ve,
		const std::string& p) const
{
	// Don't recurse our parent again
	if (c == p)
		return;

	// Find the root element for this sub-element
	ELEMENTS::const_iterator e = _elements.find(c);
	
	// Walk through all sub-elements
	std::for_each(e->second.elements().begin(), e->second.elements().end(),
			bind(sort_include, this, _1, ref(ve), ref(c)));
	
	// Add this element if not already there
	if (std::find(ve.begin(), ve.end(), c) == ve.end())
		ve.push_back(c);
}

void Generator::source(const std::vector<std::string>& c, const std::string& r,
		std::ostream& o) const
{
	// Start source file
	std::string empty;
	std::string s(Source_File);
	update_keywords(s, r, empty, empty, empty);
	o << s;
	
	// Add the class implementations
	std::for_each(c.begin(), c.end(), bind(source_implementation, this, _1,
			ref(o), ref(c[c.size() - 1])));
}

void Generator::source_implementation(const std::string& c, std::ostream& o,
		const std::string& l) const
{
	std::string empty;
	
	// Locate root element definition
	const ELEMENTS::const_iterator ei = _elements.find(c);
	
	// Cache some stuff for ease of use
	const Element& e = ei->second;
	const ATTRIBUTES& a = e.attributes();
	SUBELEMENTS s = e.elements();
	std::sort(s.begin(), s.end());
	
	// Element prefix
	std::string t(Source_Prefix);
	update_keywords(t, empty, empty, empty, ei->first);
	o << t;

	// Add operator=
	t = Source_OperatorEqual_Prefix;
	update_keywords(t, empty, empty, empty, ei->first);
	o << t;
	std::for_each(a.begin(), a.end(), bind(source_ope_attributes, this, _1,
			ref(o)));
	std::for_each(s.begin(), s.end(), bind(source_ope_elements, this, _1, ref(o)));
	t = Source_OperatorEqual_Postfix;
	update_keywords(t, empty, empty, empty, ei->first);
	o << t;

	// Add read implementation
	t = Source_Read_Prefix;
	update_keywords(t, empty, empty, empty, ei->first);
	o << t;
	if (!a.empty())
	{
		o << "\t// Get attributes if provided" << endl;
		std::for_each(a.begin(), a.end(), bind(source_read_attributes, this, _1,
				ref(o), a.begin(), a.end()));
		o << "\t" << endl;
	}
	if (!s.empty())
	{
		t = Source_Read_Element_Prefix;
		update_keywords(t, empty, empty, empty, c);
		o << t;
		std::for_each(s.begin(), s.end(), bind(source_read_elements, this, _1,
				ref(o)));
		t = Source_Read_Element_Postfix;
		update_keywords(t, empty, empty, empty, c);
		o << t;
	}
	t = Source_Read_Postfix;
	update_keywords(t, empty, empty, empty, ei->first);
	o << t;

	// Add get value to file
	t = Source_Value;
	update_keywords(t, empty, empty, empty, c);
	o << t;

	// Add write to file
	t = Source_Write;
	update_keywords(t, empty, empty, empty, c);
	o << t;
	
	// Add write to xmlTextWriter
	t = Source_Write_Start;
	update_keywords(t, empty, empty, empty, c);
	o << t;
	std::for_each(a.begin(), a.end(), bind(source_write_attributes, this, _1,
			ref(o), ref(ei->first)));
	t = Source_Write_Middle;
	update_keywords(t, empty, empty, empty, c);
	o << t;
	if (!s.empty())
	{
		t = Source_Write_Element_Prefix;
		update_keywords(t, empty, empty, empty, c);
		o << t;
		std::for_each(s.begin(), s.end(), bind(source_write_elements, this, _1, ref(o)));
		t = Source_Write_Element_Postfix;
		update_keywords(t, empty, empty, empty, c);
		o << t;
	}
	t = Source_Write_End;
	update_keywords(t, empty, empty, empty, c);
	o << t;
}

void Generator::source_ope_attributes(const std::string& a, std::ostream& o) const
{
	std::string empty;
	std::string s = Source_OperatorEqual_Attribute;
	update_keywords(s, empty, a, empty, empty);
	o << s;
}

void Generator::source_ope_elements(const std::string& e, std::ostream& o) const
{
	std::string empty;
	std::string s = Source_OperatorEqual_Child;
	update_keywords(s, empty, empty, e, empty);
	o << s;
}

void Generator::source_read_attributes(const std::string& a, std::ostream& o,
		const ATTRIBUTES::const_iterator b, const ATTRIBUTES::const_iterator e) const
{
	std::string empty;
	if (a == *b)
		o << "\txmlChar* ";
	else
		o << "\t";
	std::string s(Source_Read_Attribute);
	update_keywords(s, empty, a, empty, empty);
	o << s;
}

void Generator::source_read_elements(const std::string& v, std::ostream& o) const
{
	std::string empty;
	std::string s(Source_Read_Element);
	update_keywords(s, empty, empty, v, empty);
	o << s;
}

void Generator::source_write_attributes(const std::string& v, std::ostream& o,
		const std::string& e) const
{
	std::string empty;
	std::string s = Source_Write_Attribute;
	update_keywords(s, empty, v, empty, e);
	o << s;
}

void Generator::source_write_elements(const std::string& e, std::ostream& o) const
{
	std::string empty;
	std::string s(Source_Write_Element);
	update_keywords(s, empty, empty, e, empty);
	o << s;
}
