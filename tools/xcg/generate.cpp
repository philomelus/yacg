#include "Common.hpp"

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

namespace
{
	const char* major_divider ="//=============================================================================";
	
	const char* minor_divider ="//-------------------------------------------------------------------------";

	const int count_keywords = 62;
	
	struct
	{
		bool fail;
		const char* name;
		const char* replacement;
	} keywords[count_keywords] =
	{
		{ false, "auto", "Auto" },
		{ false, "bool", "Bool" },
		{ false, "break", "Break" },
		{ false, "case", "Case" },
		{ false, "catch", "Catch" },
		{ false, "char", "Char" },
		{ false, "class", "Class" },
		{ false, "const", "Const" },
		{ false, "const_cast", "Const_cast" },
		{ false, "continue", "Continue" },
		{ false, "default", "Cefault" },
		{ false, "delete", "Celete" },
		{ false, "do", "Do" },
		{ false, "double", "Double" },
		{ false, "dynamic_cast", "Dynamic_cast" },
		{ false, "else", "Else" },
		{ false, "enum", "Enum" },
		{ false, "explicit", "Explicit" },
		{ false, "export", "Export" },
		{ false, "extern", "Extern" },
		{ false, "false", "False" },
		{ false, "float", "Float" },
		{ false, "for", "For" },
		{ false, "friend", "Friend" },
		{ false, "goto", "Goto" },
		{ false, "if", "If" },
		{ false, "inline", "Inline" },
		{ false, "int", "Int" },
		{ false, "long", "Long" },
		{ false, "mutable", "Mutable" },
		{ false, "namespace", "Namespace" },
		{ false, "new ", "New " },
		{ false, "operator", "Operator" },
		{ false, "private", "Private" },
		{ false, "protected", "Protected" },
		{ false, "public", "Public" },
		{ false, "register", "Register" },
		{ false, "reinterpret_cast", "Reinterpret_cast" },
		{ false, "return", "Return" },
		{ false, "short", "Short" },
		{ false, "signed", "Signed" },
		{ false, "sizeof", "Sizeof" },
		{ false, "static", "Static" },
		{ false, "static_cast", "Static_cast" },
		{ false, "struct", "Struct" },
		{ false, "switch", "Switch" },
		{ false, "template", "Template" },
		{ false, "this", "This" },
		{ false, "throw", "Throw" },
		{ false, "true", "True" },
		{ false, "try", "Try" },
		{ false, "typedef", "Typedef" },
		{ false, "typeid", "Typeid" },
		{ false, "typename", "Typename" },
		{ false, "union", "Union" },
		{ false, "unsigned", "Unsigned" },
		{ false, "using", "Using" },
		{ false, "virtual", "Virtual" },
		{ false, "void", "Void" },
		{ false, "volatile", "Volatile" },
		{ false, "wchar_t", "Wchar_t" },
		{ false, "while", "While" }
	};
	
	const char* fixup(const std::string& n)
	{
		for (int i = 0; i < count_keywords; ++i)
		{
			if (n == keywords[i].name)
				return keywords[i].replacement;
		}
		return n.c_str();
	}
	
	enum W
	{
		global,
		attr,
		elem,
		decl,
		src,
		attr_decl,
		attr_src,
		elem_decl,
		elem_src
	};
	struct
	{
		W when;
		const char* name;		// attribute or element name to match
		const char* type;		// name of type
		const char* convert;	// name of function that accepts const char* and returns type
	} types[] =
	{	// when			name			type				convert
		{ global,		0,				0,					0 }
	};
}

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
	// Locate root element definition
	const ELEMENTS::const_iterator ei = _elements.find(c);
	
	// Cache some stuff for ease of use
	const Element& e = ei->second;
	const ATTRIBUTES& a = e.attributes();
	const SUBELEMENTS& s = e.elements();
	const std::string name = ei->first;
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");

	// Provide status
	cout << "Adding declaration of element class '" << class_name << "' ...";

	// Start class declaration
	o << major_divider << endl;
	o << "// Element '" << name << "' encapsulation" << endl;
	o << endl;
	o << "\tclass " << class_name << endl;
	o << "\t{" << endl;
	o << "\t" << minor_divider << endl;
	o << "\t// Construction & operators" << endl;
	o << "\tpublic:" << endl;
	o << "\t\t" << class_name << "();" << endl;
	o << "\t\t" << class_name << "(const char* f);" << endl;
	o << "\t\t" << class_name << "(xmlNodePtr n);" << endl;
	o << "\t\t" << class_name << "(const " << class_name << "& c);" << endl;
	o << "\t\tvirtual ~" << class_name << "() throw ();" << endl;
	o << endl;
	o << "\t\t" << class_name << "& operator=(const " << class_name << "& r);" << endl;
	o << endl;
	
	// Add attributes
	std::for_each(a.begin(), a.end(), bind(declaration_attributes, this,
			_1, ref(o)));
	
	// Add sub-elements
	std::for_each(s.begin(), s.end(), bind(declaration_elements, this,
			_1, ref(o)));
	
	// Add read from node function
	o << "\t" << minor_divider << endl;
	o << "\t// Read from node" << endl;
	o << "\tpublic:" << endl;
	o << "\t\tvoid read(xmlNodePtr p);" << endl;
	o << "\t" << endl;

	// Add element value
	o << "\t" << minor_divider << endl;
	o << "\t// Value of this element" << endl;
	o << "\tprivate:" << endl;
	o << "\t\ttypedef std::pair<bool, std::string> VALUE;\t\t// (element here?, text)" << endl;
	o << "\t\ttypedef std::vector<VALUE> VALUES;" << endl;
	o << "\t" << endl;
	o << "\tpublic:" << endl;
	o << "\t\tVALUES& values();" << endl;
	o << "\t\tconst VALUES& values() const;" << endl;
	o << "\t\tvoid value(std::string& v) const;" << endl;
	o << "\t" << endl;
	o << "\tprivate:" << endl;
	o << "\t\tVALUES _values;" << endl;
	o << endl;
	
	// Add write function(s)
	o << "\t" << minor_divider << endl;
	o << "\t// Write to file or node" << endl;
	o << "\tpublic:" << endl;
	o << "\t\tvoid write(const char* f, bool i = true, const char* t = \"\\t\") const;" << endl;
	o << "\t\tvoid write(xmlTextWriterPtr w) const;" << endl;
	
	// Close class
	o << "\t};" << endl;
	if (c != l)
		o << endl;
	
	// Close up status
	cout << " done!" << endl;
}

void Generator::declaration_attributes(const std::string& v, std::ostream& o) const
{
	//	//----...
	//	public:
	//		const std::string& attribute() const;
	//		void attribute(const char* s);
	//
	//	private:
	//		std::string _attribute;

	const std::string name = fixup(v);
	
	o << "\t" << minor_divider << endl;
	o << "\t// Attribute " << name << endl;
	o << "\tpublic:" << endl;
	o << "\t\tconst std::string& " << name << "() const;" << endl;
	o << "\t\tvoid " << name << "(const char* " << name[0] << ");" << endl;
	o << "\t" << endl;
	o << "\tprivate:" << endl;
	o << "\t\tstd::string _" << v << ";" << endl;
	o << "\t" << endl;
}

void Generator::declaration_elements(const std::string& v, std::ostream& o) const
{
	//	//----...
	//	public:
	//		std::vector<std::string>& elements();
	//		const std::vector<std::string>& elements() const;
	//
	//	private:
	//		std::vector<Element> _elements;
	
	const std::string name = fixup(v);
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");
	
	o << "\t" << minor_divider << endl;
	o << "\t// Child element " << v << "." << endl;
	o << "\tpublic:" << endl;
	o << "\t\tstd::vector<" << class_name << ">& " << plural_name << "();" << endl;
	o << "\t\tconst std::vector<" << class_name << ">& " << plural_name << "() const;" << endl;
	o << "\t" << endl;
	o << "\tprivate:" << endl;
	o << "\t\tstd::vector<" << class_name << "> _" << plural_name << ";" << endl;
	o << "\t" << endl;
}

void Generator::header(const std::vector<std::string>& h, const std::string& r,
		std::ostream& o) const
{
	// Add license
	license(o);
	
	// Add note about being machine generated
	time_t t;
	time(&t);
	o << "// This file was originally machine generated by XCG: " << ctime(&t);
	o << "//" << endl;
	o << endl;

	// Add inclusion macro
	std::string protection_macro = "__";
	protection_macro += r;
	protection_macro += "_HPP__";
	str_upper(protection_macro);
	o << "#ifndef\t" << protection_macro << endl;
	o << "#define\t" << protection_macro << endl;
	o << endl;
	
	// Add includes
	o << "#include <string>" << endl;
	o << "#include <vector>" << endl;
	o << "#include <libxml/parser.h>" << endl;
	o << "#include <libxml/xmlwriter.h>" << endl;
	o << endl;
	
	// Add namespace
	o << "namespace " << r << endl;
	o << "{" << endl;
	
	// Add all the class declarations
	std::for_each(h.begin(), h.end(), bind(declaration, this, _1, ref(o),
			ref(h[h.size() - 1])));
	
	// Close namespace
	o << "}" << endl;
	o << endl;
	
	// Include inlines
	o << "#include \"" << r << ".inl\"" << endl;
	o << endl;
	
	// Close protection macro
	o << "#endif\t\t// " << protection_macro << endl;
}

void Generator::inline_attributes(const std::string& a, std::ostream& o,
		const std::string& c) const
{
	//	inline const std::string& element::attribute() const
	//	{
	//		return _attribute;
	//	}
	//
	//	inline void element::attribute(const char* a)
	//	{
	//		_attribute = a;
	//	}
	//
	
	const std::string name = fixup(a);
	
	o << "\tinline const std::string& " << c << "::" << name << "() const" << endl;
	o << "\t{" << endl;
	o << "\t\treturn _" << a << ";" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
	o << "\tinline void " << c << "::" << name << "(const char* " << name[0] << ")" << endl;
	o << "\t{" << endl;
	o << "\t\t_" << a << " = " << name[0] << ";" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
}

void Generator::inline_elements(const std::string& a, std::ostream& o,
		const std::string& c) const
{
	//	inline std::vector<std::string>& e::v()
	//	{
	//		return _v;
	//	}
	//	
	//	inline const std::vector<std::string>& e::v() const
	//	{
	//		return _v;
	//	}
	//	
	
	const std::string name = fixup(a);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);

	o << "\tinline std::vector<" << class_name << ">& " << c << "::" << plural_name << "()" << endl;
	o << "\t{" << endl;
	o << "\t\treturn _" << plural_name << ";" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
	o << "\tinline const std::vector<" << class_name << ">& " << c << "::" << plural_name << "() const" << endl;
	o << "\t{" << endl;
	o << "\t\treturn _" << plural_name << ";" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
}

void Generator::inline_implementation(const std::string& c, std::ostream& o,
		const std::string& l) const
{
	// Locate root element definition
	const ELEMENTS::const_iterator ei = _elements.find(c);
	
	// Cache some stuff for ease of use
	const Element& e = ei->second;
	const ATTRIBUTES& a = e.attributes();
	const SUBELEMENTS& s = e.elements();
	const std::string name = ei->first;
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");

	// Provide status
	cout << "Adding inline implementations of element class '" << class_name << "' ...";

	// Start inline implementations
	o << major_divider << endl;
	o << "// class " << class_name << " inline implementation" << endl;
	o << endl;
	
	// Add default constructor
	o << "\tinline " << class_name << "::" << class_name << "()" << endl;
	o << "\t{" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;;

	// Add copy constructor
	o << "\tinline " << class_name << "::" << class_name << "(const " << class_name << "& r)" << endl;
	o << "\t{" << endl;
	o << "\t\t*this = r;" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
	
	// Add destructor
	o << "\tinline " << class_name << "::~" << class_name << "()" << endl;
	o << "\t{" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
	
	// Add attributes
	std::for_each(a.begin(), a.end(), bind(inline_attributes, this, _1, ref(o),
			ref(class_name)));
	
	// Add sub-elements
	std::for_each(s.begin(), s.end(), bind(inline_elements, this, _1, ref(o),
			ref(class_name)));

	// Add value implementation
	o << "\tinline " << class_name << "::VALUES& " << class_name << "::values()" << endl;
	o << "\t{" << endl;
	o << "\t\treturn _values;" << endl;
	o << "\t}" << endl;
	o << endl;
	o << "\tinline const " << class_name << "::VALUES& " << class_name << "::values() const" << endl;
	o << "\t{" << endl;
	o << "\t\treturn _values;" << endl;
	o << "\t}" << endl;
	if (c != l)
		o << endl;
	
	// Close up status
	cout << " done!" << endl;
}

void Generator::inlines(const std::vector<std::string>& c, const std::string& r,
		std::ostream& o) const
{
	// Add license
	license(o);
	
	// Add note about being machine generated
	time_t t;
	time(&t);
	o << "// This file was originally machine generated by XCG: " << ctime(&t);
	o << "//" << endl;
	o << endl;
	
	// Add inclusion macro
	std::string protection_macro = "__";
	protection_macro += r;
	protection_macro += "_INL__";
	str_upper(protection_macro);
	o << "#ifndef\t" << protection_macro << endl;
	o << "#define\t" << protection_macro << endl;
	o << endl;
	
	// Add namespace
	o << "namespace " << r << endl;
	o << "{" << endl;
	
	// Add all the inline implementation
	std::for_each(c.begin(), c.end(), bind(inline_implementation, this, _1,
			ref(o), ref(c[c.size() - 1])));
	
	// Close namespace
	o << "}" << endl;
	o << endl;
	
	// Close protection macro
	o << "#endif\t\t// " << protection_macro << endl;
}

void Generator::license(std::ostream& o) const
{
	o << "//" << endl;
	o << "// Copyright (c) 2004 Russell E. Gibson" << endl;
	o << "// email: russg@rnstech.com" << endl;
	o << "//" << endl;
	o << "// Permission is hereby granted, free of charge, to any person obtaining a copy" << endl;
	o << "// of this software and associated documentation files (the \"Software\"), to deal" << endl;
	o << "// in the Software without restriction, including without limitation the rights" << endl;
	o << "// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell" << endl;
	o << "// copies of the Software, and to permit persons to whom the Software is furnished" << endl;
	o << "// to do so, subject to the following conditions:" << endl;
	o << "//" << endl;
	o << "// The above copyright notice and this permission notice shall be included in all" << endl;
	o << "// copies or substantial portions of the Software." << endl;
	o << "//" << endl;
	o << "// THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR" << endl;
	o << "// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY," << endl;
	o << "// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE" << endl;
	o << "// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER" << endl;
	o << "// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM," << endl;
	o << "// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN" << endl;
	o << "// THE SOFTWARE." << endl;
	o << "//" << endl;
}

void Generator::sort_elements(const ELEMENTS::value_type& c, std::vector<std::string>& ve) const
{
	// Walk through all sub-elements
	std::for_each(c.second.elements().begin(), c.second.elements().end(), bind(sort_include, this, _1, ref(ve)));
	
	// Add this element if not already there
	if (std::find(ve.begin(), ve.end(), c.first) == ve.end())
		ve.push_back(c.first);
}

void Generator::sort_include(const std::string& c, std::vector<std::string>& ve) const
{
	// Find the root element for this sub-element
	ELEMENTS::const_iterator e = _elements.find(c);
	
	// Walk through all sub-elements
	std::for_each(e->second.elements().begin(), e->second.elements().end(),
			bind(sort_include, this, _1, ref(ve)));
	
	// Add this element if not already there
	if (std::find(ve.begin(), ve.end(), c) == ve.end())
		ve.push_back(c);
}

void Generator::source(const std::vector<std::string>& c, const std::string& r,
		std::ostream& o) const
{
	// Add license
	license(o);
	
	// Add note about being machine generated
	time_t t;
	time(&t);
	o << "// This file was originally machine generated by XCG: " << ctime(&t);
	o << "//" << endl;
	o << endl;

	// Add includes
	o << "#include <algorithm>" << endl;
	o << "#include <locale>" << endl;
	o << "#include \"" << r << ".hpp\"" << endl;
	o << endl;

	// Add namespace
	o << "using namespace " << r << ";" << endl;
	o << endl;

	// Add anonymous namespace
	o << major_divider << endl;
	o << "// Local functions" << endl;
	o << endl;
	o << "namespace" << endl;
	o << "{" << endl;
	o << "\t// Called on reading to clear out whitespace and control characters from" << endl;
	o << "\t// a XML_TEXT_NODE.  The idea is to get the actual value of an element" << endl;
	o << "\t// rather than the whitespace used in formatting the file its in." << endl;
	o << "\txmlChar* trim(xmlChar* t)" << endl;
	o << "\t{" << endl;
	o << "\t\tstd::locale l = std::locale();" << endl;
	o << "\t\twhile (*t)" << endl;
	o << "\t\t{" << endl;
	o << "\t\t\twhile (*t && (std::iscntrl(*t, l) || *t == ' '))" << endl;
	o << "\t\t\t\t++t;" << endl;
	o << "\t\t\tif (*t < 127 && *t > 33)\t\t// HACK" << endl;
	o << "\t\t\t\tbreak;" << endl;
	o << "\t\t}" << endl;
	o << "\t\treturn t;" << endl;
	o << "\t}" << endl;
	o << "\t" <<  endl;
	o << "\t// Functor to call write on each child element" << endl;
	o << "\ttemplate <typename T>" << endl;
	o << "\tclass writer" << endl;
	o << "\t{" << endl;
	o << "\tpublic:" << endl;
	o << "\t\twriter(xmlTextWriterPtr w)" << endl;
	o << "\t\t\t\t:" << endl;
	o << "\t\t\t\t_w(w)" << endl;
	o << "\t\t{" << endl;
	o << "\t\t}" << endl;
	o << "\t\tvoid operator()(const T& t)" << endl;
	o << "\t\t{" << endl;
	o << "\t\t\tt.write(_w);" << endl;
	o << "\t\t}" << endl;
	o << "\tprivate:" << endl;
	o << "\t\txmlTextWriterPtr _w;" << endl;
	o << "\t};" << endl;
	o << "}" << endl;
	o << endl;
	
	// Add the class implementations
	std::for_each(c.begin(), c.end(), bind(source_implementation, this, _1,
			ref(o), ref(c[c.size() - 1])));
}

void Generator::source_implementation(const std::string& c, std::ostream& o,
		const std::string& l) const
{
	// Locate root element definition
	const ELEMENTS::const_iterator ei = _elements.find(c);
	
	// Cache some stuff for ease of use
	const Element& e = ei->second;
	const ATTRIBUTES& a = e.attributes();
	SUBELEMENTS s = e.elements();
	std::sort(s.begin(), s.end());
	const std::string name = ei->first;
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");
	const std::string ef = s.empty() ? "" : s[0];
	const std::string el = s.empty() ? "" : s[s.size() - 1];
	
	// Tell user what's happening
	cout << "Adding implementations of element class '" << class_name << "' ...";

	// Add comment
	o << major_divider << endl;
	o << "// Element class " << class_name << " implementation" << endl;
	o << endl;
	
	// Add read from file constructor
	o << class_name << "::" << class_name << "(const char* f)" << endl;
	o << "{" << endl;
	o << "\t// Read in document" << endl;
	o << "\txmlDocPtr d = xmlParseFile(f);" << endl;
	o << "\tif (!d)" << endl;
	o << "\t\tthrow std::runtime_error(\"file parse failed\");" << endl;
	o << "\t\t" << endl;
	o << "\ttry" << endl;
	o << "\t{" << endl;
	o << "\t\t// Initialize from root node" << endl;
	o << "\t\txmlNodePtr r = xmlDocGetRootElement(d);" << endl;
	o << "\t\tif (r)" << endl;
	o << "\t\t\tread(r);" << endl;
	o << "\t\t" << endl;
	o << "\t\t// Clean up" << endl;
	o << "\t\txmlFreeDoc(d);" << endl;
	o << "\t}" << endl;
	o << "\tcatch (...)" << endl;
	o << "\t{" << endl;
	o << "\t\txmlFreeDoc(d);" << endl;
	o << "\t\tthrow;" << endl;
	o << "\t}" << endl;
	o << "}" << endl;
	o << endl;

	// Add from node constructor
	o << class_name << "::" << class_name << "(xmlNodePtr n)" << endl;
	o << "{" << endl;
	o << "\t// Initialize from node" << endl;
	o << "\tread(n);" << endl;
	o << "}" << endl;
	o << endl;

	// Add operator=
	o << class_name << "& " << class_name << "::operator=(const " << class_name << "& r)" << endl;
	o << "{" << endl;
	o << "\tif (this != &r)" << endl;
	o << "\t{" << endl;
	std::for_each(a.begin(), a.end(), bind(source_ope_attributes, this, _1,
			ref(o)));
	std::for_each(s.begin(), s.end(), bind(source_ope_elements, this, _1, ref(o)));
	o << "\t\t_values = r._values;" << endl;
	o << "\t}" << endl;
	o << "\treturn *this;" << endl;
	o << "}" << endl;
	o << endl;

	// Add read implementation
	o << "void " << class_name << "::read(xmlNodePtr n)" << endl;
	o << "{" << endl;
	o << "\t// Make sure node is this element" << endl;
	o << "\tif (xmlStrcmp(n->name, reinterpret_cast<const xmlChar*>(\"" << name << "\")) != 0)" << endl;
	o << "\t\tthrow std::runtime_error(\"expecting node '" << name << "'\");" << endl;
	o << endl;
	if (!a.empty())
	{
		o << "\t// Get attributes if provided" << endl;
		std::for_each(a.begin(), a.end(), bind(source_read_attributes, this, _1,
				ref(o), a.begin(), a.end()));
		o << "\t" << endl;
	}
	if (!s.empty())
	{
		o << "\t// Read the child elements" << endl;
		o << "\tfor (xmlNodePtr s = n->children; s; s = s->next)" << endl;
		o << "\t{" << endl;
		o << "\t\t// Ignore non-element children" << endl;
		o << "\t\tif (s->type != XML_ELEMENT_NODE)" << endl;
		o << "\t\t\tcontinue;" << endl;
		o << "\t\t" << endl;
		std::for_each(s.begin(), s.end(), bind(source_read_elements, this, _1,
				ref(o)));
		o << "\t}" << endl;
		o << endl;
	}
	o << "\t// Retreive value" << endl;
	o << "\tfor (xmlNodePtr c = n->children; c; c = c->next)" << endl;
	o << "\t{" << endl;
	o << "\t\tif (c->type == XML_TEXT_NODE)" << endl;
	o << "\t\t{" << endl;
	o << "\t\t\txmlChar* t = trim(c->content);" << endl;
	o << "\t\t\tif (*t)" << endl;
	o << "\t\t\t{" << endl;
	o << "\t\t\t\t_values.push_back(std::make_pair(false," << endl;
	o << "\t\t\t\t\t\treinterpret_cast<const char*>(t)));" << endl;
	o << "\t\t\t}" << endl;
	o << "\t\t}" << endl;
	o << "\t\telse if (c->type == XML_ELEMENT_NODE)" << endl;
	o << "\t\t\t_values.push_back(std::make_pair(true, std::string()));" << endl;
	o << "\t}" << endl;
	o << "}" << endl;
	o << endl;

	// Add get value to file
	o << "void " << class_name << "::value(std::string& v) const" << endl;
	o << "{" << endl;
	o << "\tconst VALUES::const_iterator end = _values.end();" << endl;
	o << "\tfor (VALUES::const_iterator i = _values.begin(); i != end; ++i)" << endl;
	o << "\t\tv += i->second;" << endl;
	o << "}" << endl;
	o << endl;

	// Add write to file
	o << "void " << class_name << "::write(const char* f, bool i, const char* t) const" << endl;
	o << "{" << endl;
	o << "\t// Create a new writer" << endl;
	o << "\txmlTextWriterPtr w = xmlNewTextWriterFilename(f, 0);" << endl;
	o << "\tif (!w)" << endl;
	o << "\t\tthrow std::runtime_error(\"unable to create xmlTextWriter\");" << endl;
	o << "\t" << endl;
	o << "\t// Set indentation if desired" << endl;
	o << "\tif (i)" << endl;
	o << "\t{" << endl;
	o << "\t\txmlTextWriterSetIndent(w, 1);" << endl;
	o << "\t\txmlTextWriterSetIndentString(w, reinterpret_cast<xmlChar*>(t ? t : \"\\t\"));" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
	o << "\ttry" << endl;
	o << "\t{" << endl;
	o << "\t\t// Create document" << endl;
	o << "\t\tint r = xmlTextWriterStartDocument(w, \"1.0\", \"iso8859-1\", \"yes\");" << endl;
	o << "\t\tif (r < 0)" << endl;
	o << "\t\t\tthrow std::runtime_error(\"unable to start document\");" << endl;
	o << "\t" << endl;
	o << "\t\t// Add the child elements" << endl;
	o << "\t\twrite(w);" << endl;
	o << "\t\t" << endl;
	o << "\t\t// Close the writer" << endl;
	o << "\t\txmlFreeTextWriter(w);" << endl;
	o << "\t}" << endl;
	o << "\tcatch (...)" << endl;
	o << "\t{" << endl;
	o << "\t\txmlFreeTextWriter(w);" << endl;
	o << "\t\tthrow;" << endl;
	o << "\t}" << endl;
	o << "}" << endl;
	o << endl;

	// Add write to xmlTextWriter
	o << "void " << class_name << "::write(xmlTextWriterPtr w) const" << endl;
	o << "{" << endl;
	o << "\t// Create element" << endl;
	o << "\tint r = xmlTextWriterStartElement(w, reinterpret_cast<const xmlChar*>(\"" << name << "\"));" << endl;
	o << "\tif (r < 0)" << endl;
	o << "\t\tthrow std::runtime_error(\"creation of element '"<< name << "' failed\");" << endl;
	o << "\t" << endl;
	std::for_each(a.begin(), a.end(), bind(source_write_attributes, this, _1, ref(o), ref(name)));
	o << "\t// Write value if needed" << endl;
	o << "\tstd::string v;" << endl;
	o << "\tvalue(v);" << endl;
	o << "\tif (!v.empty())" << endl;
	o << "\t{" << endl;
	o << "\t\tr = xmlTextWriterWriteString(w, reinterpret_cast<const xmlChar*>(v.c_str()));" << endl;
	o << "\t\tif (r < 0)" << endl;
	o << "\t\t\tthrow std::runtime_error(\"failed writing '" << name << "' value\");" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
	if (!s.empty())
	{
		o << "\t// Add child elements" << endl;
		std::for_each(s.begin(), s.end(), bind(source_write_elements, this, _1, ref(o)));
		o << "\t" << endl;
	}
	o << "\t// All done!" << endl;
	o << "\tr = xmlTextWriterEndElement(w);" << endl;
	o << "\tif (r < 0)" << endl;
	o << "\t\tthrow std::runtime_error(\"failed closing '" << name << "' element\");" << endl;
	o << "}" << endl;
	o << endl;

	// Close up status
	cout << " done!" << endl;
}

void Generator::source_ope_attributes(const std::string& a, std::ostream& o) const
{
	//	_attribute = r._attribute;

	o << "\t\t_" << a << " = r._" << a << ";" << endl;
}

void Generator::source_ope_elements(const std::string& e, std::ostream& o) const
{
	//	_elements = r._elements;
	
	const std::string name = fixup(e);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");

	o << "\t\t_" << plural_name << " = r._" << plural_name << ";" << endl;
}

void Generator::source_read_attributes(const std::string& v, std::ostream& o,
		const ATTRIBUTES::const_iterator b, const ATTRIBUTES::const_iterator e) const
{
	if (v == *b)
		o << "\txmlChar* ";
	else
		o << "\t";
	o << "a = xmlGetProp(n, reinterpret_cast<xmlChar*>(\"" << v << "\"));" << endl;
	o << "\tif (a)" << endl;
	o << "\t{" << endl;
	o << "\t\t_" << v << " = reinterpret_cast<const char*>(a);" << endl;
	o << "\t\txmlFree(a);" << endl;
	o << "\t}" << endl;
}

void Generator::source_read_elements(const std::string& v, std::ostream& o) const
{
	const std::string name = fixup(v);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);
	
	o << "\t\tif (xmlStrcmp(s->name, reinterpret_cast<const xmlChar*>(\"" << v << "\")) == 0)" << endl;
	o << "\t\t{" << endl;
	o << "\t\t\t_" << plural_name << ".push_back(" << class_name << "(s));" << endl;
	o << "\t\t\tcontinue;" << endl;
	o << "\t\t}" << endl;
}

void Generator::source_write_attributes(const std::string& v, std::ostream& o,
		const std::string& e) const
{
	//	// Add attribute group if needed
	//	if (!_group.empty())
	//	{
	//		r = xmlTextWriterWriteAttribute(w, reinterpret_cast<const xmlChar*>("group"),
	//				reinterpret_cast<const xmlChar*>(_group.c_str()));
	//		if (r < 0)
	//		{
	//			throw std::runtime_error("addition of attribute 'group' "
	//					"to element 'flags' failed");
	//		}
	//	}
	//

	o << "\t// Add attribute " << v << " if needed" << endl;
	o << "\tif (!_" << v << ".empty())" << endl;
	o << "\t{" << endl;
	o << "\t\tr = xmlTextWriterWriteAttribute(w, reinterpret_cast<const xmlChar*>(\"" << v << "\")," << endl;
	o << "\t\t\t\treinterpret_cast<const xmlChar*>(_" << v << ".c_str()));" << endl;
	o << "\t\tif (r < 0)" << endl;
	o << "\t\t{" << endl;
	o << "\t\t\tthrow std::runtime_error(\"addition of attribute '" << v << "' \"" << endl;
	o << "\t\t\t\t\t\"to element '" << e << "' failed\");" << endl;
	o << "\t\t}" << endl;
	o << "\t}" << endl;
	o << "\t" << endl;
}

void Generator::source_write_elements(const std::string& e, std::ostream& o) const
{
	//	std::for_each(_elements.begin(), _elements.end(), writer<Element>(w));

	const std::string name = fixup(e);
	const std::string plural_name = name + (name[name.size() - 1] == 's'
			? "" : "s");
	const std::string class_name = std::toupper(name[0], std::locale())
			+ name.substr(1, std::string::npos);
	
	o << "\tstd::for_each(_" << plural_name << ".begin(), _"<< plural_name
			<< ".end(), writer<" << class_name << ">(w));" << endl;
}
