#include "Common.hpp"

#include <iostream>

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
}

//=============================================================================
// class Generator implementation
//=============================================================================

Generator::Generator(const std::string& d, const ELEMENTS& e)
		:
		_directory(d)
{
	// Create the headers
	std::for_each(e.begin(), e.end(), bind(header, this, _1));
	
	// Create the inline implementations
	std::for_each(e.begin(), e.end(), bind(inlines, this, _1));
	
	// Create the standard implementations
	std::for_each(e.begin(), e.end(), bind(source, this, _1));
}

void Generator::header(const ELEMENTS::value_type& v)
{
	// Create and open file
	const std::string& name = v.first;
	std::string	filename(_directory);
	filename += "\\";
	filename +=	name;
	filename +=	".hpp";
	std::ofstream header(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!header)
	{
		cerr << "Unable to create and/or open \"" << filename << "\": File skipped!" << endl;
		return;
	}
	cout << "Generating \"" << filename << "\"..." << endl;
	
	// For effeciency sake, create locals for often-used values
	const ATTRIBUTES& a = v.second.attributes();
	const SUBELEMENTS& s = v.second.elements();
	std::string nameu = name;	// All uppercase
	str_upper(nameu);
	std::string namec = name;	// Uppercase first letter
	namec[0] = std::toupper(namec[0], std::locale());
	
	// Create header multiple inclusion protection value		
	std::string	mip =	"__";
	mip += nameu;
	mip += "_HPP__";
	
	// Start header output
	header << "#ifndef\t" << mip << endl
			<< "#define\t" << mip << endl
			<< endl
			<< "#include <string>" << endl
			<< "#include <vector>" << endl
			<< endl
			<< "#include <libxml/parser.h>" << endl
			<< endl;

	// Add includes for sub-elements
	std::for_each(s.begin(), s.end(), bind(header_includes, this, _1,
			ref(header)));
	
	// Start class declaration
	header << "namespace impl" << endl
			<< "{" << endl
		<< major_divider << endl
			<< "// Element \"" << name << "\" encapsulation." << endl
			<< endl
			<< "\tclass " << namec << endl
			<< "\t{" << endl
		<< "\t" << minor_divider << endl
			<< "\t// Construction, operators" << endl
			<< "\tpublic:" << endl
			<< "\t\t" << namec << "();" << endl
			<< "\t\t" << namec << "(const char* f);" << endl
			<< "\t\t" << namec << "(xmlNodePtr n);" << endl
			<< "\t\t" << namec << "(const " << namec << "& c);" << endl
			<< "\t\tvirtual ~" << namec << "() throw ();" << endl
			<< endl
			<< "\t\t" << namec << "& operator=(const " << namec << "& r);" << endl
			<< endl;

	// Add attributes
	std::for_each(a.begin(), a.end(), bind(header_attributes_declaration, this,
			_1, ref(header)));
	
	// Add sub-elements
	std::for_each(s.begin(), s.end(), bind(header_elements_declaration, this,
			_1, ref(header)));

	// Add read function
	header << "\t" << minor_divider << endl
			<< "\t// Read from node." << endl
			<< "\tpublic:" << endl
			<< "\t\tvoid read(xmlNodePtr p);" << endl
			<< endl;
			
	// Add value of this element
	header << "\t" << minor_divider << endl
			<< "\t// Value of this element." << endl
			<< "\tpublic:" << endl
			<< "\t\tconst std::string& value() const;" << endl
			<< "\t\tvoid value(const char* v);" << endl
			<< endl
			<< "\tprivate:" << endl
			<< "\t\tstd::string _value;" << endl
			<< endl;

	// Add write functions
	header << "\t" << minor_divider << endl
			<< "\t// Write to file or node." << endl
			<< "\tpublic:" << endl
			<< "\t\tvoid write(const char* f) const;" << endl
			<< "\t\tvoid write(xmlNodePtr p) const;" << endl;

	// Finish up
	header << "\t};" << endl
			<< "}" << endl
			<< endl
			<< "#include \"" << namec << ".inl\"" << endl
			<< endl
			<< "#endif" << endl;
}

void Generator::header_attributes_declaration(const std::string& v, std::ofstream& o)
{
	//	//----...
	//	public:
	//		const std::string& attribute() const;
	//		void attribute(const char* s);
	//
	//	private:
	//		std::string _attribute;

	const std::string name = fixup(v);
	
	o << "\t" << minor_divider << endl
			<< "\t// Attribute " << name << "." << endl
			<< "\tpublic:" << endl
			<< "\t\tconst std::string& " << name << "() const;" << endl
			<< "\t\tvoid " << name << "(const char* " << name[0] << ");" << endl
			<< endl
			<< "\tprivate:" << endl
			<< "\t\tstd::string _" << v << ";" << endl
			<< endl;
}

void Generator::header_elements_declaration(const std::string& v, std::ofstream& o)
{
	//	//----...
	//	public:
	//		std::vector<std::string>& element();
	//		const std::vector<std::string>& element() const;
	//
	//	private:
	//		std::vector<std::string> _element;
	
	const std::string name = fixup(v);
	
	std::string names = name;
	if (names[names.size() - 1] != 's')
		names += "s";
	
	std::string namec = name;
	namec[0] = std::toupper(namec[0], std::locale());
	
	o << "\t" << minor_divider << endl
			<< "\t// Sub-element " << v << "." << endl
			<< "\tpublic:" << endl
			<< "\t\tstd::vector<" << namec << ">& " << names << "();" << endl
			<< "\t\tconst std::vector<" << namec << ">& " << names << "() const;" << endl
			<< endl
			<< "\tprivate:" << endl
			<< "\t\tstd::vector<" << namec << "> _" << names << ";" << endl
			<< endl;
}

void Generator::header_includes(const std::string& v, std::ofstream& o)
{
	//	#ifndef	__ELEMENT_HPP__
	//	#include "Element.hpp"
	//	#endif
	
	std::string	mip = "__";
	mip += v;
	mip += "_HPP__";
	str_upper(mip);

	std::string namec = v;
	namec[0] = std::toupper(namec[0], std::locale());
	
	// Include header
	o << "#ifndef\t" << mip << endl
			<< "#include \"" << namec << ".hpp\"" << endl
			<< "#endif" << endl
			<< endl;
}

void Generator::inlines(const ELEMENTS::value_type& v)
{
	// Create cached version of name
	const std::string& name = v.first;

	// Create uppercased first letter'd name
	std::string namec = name;
	namec[0] = std::toupper(namec[0], std::locale());

	// Create and open file
	std::string	filename(_directory);
	filename += "\\";
	filename +=	namec;
	filename +=	".inl";
	std::ofstream header(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!header)
	{
		cerr << "Unable to create and/or open \"" << filename << "\": File skipped!" << endl;
		return;
	}
	cout << "Generating \"" << filename << "\"..." << endl;
	
	// For effeciency sake, create locals for often-used values
	const ATTRIBUTES& a = v.second.attributes();
	const SUBELEMENTS& s = v.second.elements();
	std::string nameu = name;	// All uppercase
	str_upper(nameu);
	
	// Create header multiple inclusion protection value		
	std::string	mip =	"__";
	mip += nameu;
	mip += "_INL__";
	
	// Start header output
	header << "#ifndef\t" << mip << endl
			<< "#define\t" << mip << endl
			<< endl;

	// Add namespace
	header << "namespace impl" << endl	// TODO: namespace
			<< "{" << endl;
	
	// Start inline implementations
	header << major_divider << endl
			<< "// class " << namec << " inline implementation" << endl
			<< endl;
	
	// Add default constructor
	header << "\tinline " << namec << "::" << namec << "()" << endl
			<< "\t{" << endl
			<< "\t}" << endl
			<< endl;

	// Add copy constructor
	header << "\tinline " << namec << "::" << namec << "(const " << namec << "& r)" << endl
			<< "\t{" << endl
			<< "\t\t*this = r;" << endl
			<< "\t}" << endl
			<< endl;
	
	// Add destructor
	header << "\tinline " << namec << "::~" << namec << "()" << endl
			<< "\t{" << endl
			<< "\t}" << endl
			<< endl;
	
	// Add attributes
	std::for_each(a.begin(), a.end(), bind(inlines_attributes, this, _1,
			ref(namec), ref(header)));
	
	// Add sub-elements
	std::for_each(s.begin(), s.end(), bind(inlines_elements, this, _1,
			ref(namec), ref(header)));

	// Add value implementation
	header << "\tinline const std::string& " << namec << "::value() const" << endl
			<< "\t{" << endl
			<< "\t\treturn _value;" << endl
			<< "\t}" << endl
			<< endl
			<< "\tinline void " << namec << "::value(const char* v)" << endl
			<< "\t{" << endl
			<< "\t\t_value = v;" << endl
			<< "\t}" << endl
			<< endl;

	// Finish namespace
	header << "}" << endl
			<< endl;
	
	// Finish header
	header << "#endif" << endl;
}

void Generator::inlines_attributes(const std::string& v, const std::string& e, std::ofstream& o)
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

	const std::string name = fixup(v);
	
	o << "\tinline const std::string& " << e << "::" << name << "() const" << endl
			<< "\t{" << endl
			<< "\t\treturn _" << v << ";" << endl
			<< "\t}" << endl
			<< endl
			<< "\tinline void " << e << "::" << name << "(const char* " << name[0] << ")" << endl
			<< "\t{" << endl
			<< "\t\t_" << v << " = " << name[0] << ";" << endl
			<< "\t}" << endl
			<< endl;
}

void Generator::inlines_elements(const std::string& v, const std::string& e, std::ofstream& o)
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
	
	const std::string name = fixup(v);
	
	std::string names = name;
	if (names[names.size() - 1] != 's')
		names += "s";
	
	std::string namec = name;
	namec[0] = std::toupper(namec[0], std::locale());

	o << "\tinline std::vector<" << namec << ">& " << e << "::" << names << "()" << endl
			<< "\t{" << endl
			<< "\t\treturn _" << names << ";" << endl
			<< "\t}" << endl
			<< endl
			<< "\tinline const std::vector<" << namec << ">& " << e << "::" << names << "() const" << endl
			<< "\t{" << endl
			<< "\t\treturn _" << names << ";" << endl
			<< "\t}" << endl
			<< endl;
}

void Generator::source(const ELEMENTS::value_type& v)
{
	// Create and open file
	const std::string& name = v.first;
	std::string	filename(_directory);
	filename += "\\";
	filename +=	name;
	filename +=	".cpp";
	std::ofstream source(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!source)
	{
		cerr << "Unable to create and/or open \"" << filename << "\": File skipped!" << endl;
		return;
	}
	cout << "Generating \"" << filename << "\"..." << endl;

	// For effeciency sake, create locals for often-used values
	const Element& n = v.second;
	const ATTRIBUTES& a = n.attributes();
	const SUBELEMENTS& s = n.elements();
	std::string namec = name;
	namec[0] = std::toupper(namec[0], std::locale());

	// Include files
	source << "#include \"" << name << ".hpp\"" << endl
			<< endl
			<< "using namespace impl;" << endl	// TODO: namespace
			<< endl;

	// Add read from file constructor
	source << namec << "::" << namec << "(const char* f)" << endl
			<< "{" << endl
			<< "\t// Read in document" << endl
			<< "\txmlDocPtr d = xmlParseFile(f);" << endl
			<< "\tif (!d)" << endl
			<< "\t\tthrow std::runtime_error(\"file parse failed\");" << endl
			<< "\t\t" << endl
			<< "\ttry" << endl
			<< "\t{" << endl
			<< "\t\t// Initialize from root node" << endl
			<< "\t\txmlNodePtr r = xmlDocGetRootElement(d);" << endl
			<< "\t\tif (r)" << endl
			<< "\t\t\tread(r);" << endl
			<< "\t\t" << endl
			<< "\t\t// Clean up" << endl
			<< "\t\txmlFreeDoc(d);" << endl
			<< "\t}" << endl
			<< "\tcatch (...)" << endl
			<< "\t{" << endl
			<< "\t\txmlFreeDoc(d);" << endl
			<< "\t\tthrow;" << endl
			<< "\t}" << endl
			<< "}" << endl
			<< endl;

	// Add from node constructor
	source << namec << "::" << namec << "(xmlNodePtr n)" << endl
			<< "{" << endl
			<< "\t// Initialize from node" << endl
			<< "\tread(n);" << endl
			<< "}" << endl
			<< endl;

	// Add operator=
	source << namec << "& " << namec << "::operator=(const " << namec << "& r)" << endl
			<< "{" << endl
			<< "\tif (this != &r)" << endl
			<< "\t{" << endl;
	std::for_each(a.begin(), a.end(), bind(source_ope_attributes, this, _1,
			ref(source)));
	std::for_each(s.begin(), s.end(), bind(source_ope_elements, this, _1,
			ref(source)));
	source << "\t\t_value = r._value;" << endl
			<< "\t}" << endl
			<< "\treturn *this;" << endl
			<< "}" << endl
			<< endl;

	// Add read implementation
	source << "void " << namec << "::read(xmlNodePtr n)" << endl
			<< "{" << endl
			<< "\t// Make sure node is this element" << endl
			<< "\tif (strcmp(reinterpret_cast<const char*>(n->name), \"" << name << "\") != 0)" << endl
			<< "\t\tthrow std::runtime_error(\"expected node to be '" << name << "'\");" << endl
			<< endl;
	if (!a.empty())
	{
		source << "\t// Get attributes if provided" << endl
				<< "\txmlChar* a;" << endl;
		std::for_each(a.begin(), a.end(), bind(source_read_attributes, this, _1,
				ref(source)));
		source << endl;
	}
	if (!s.empty())
	{
		source << "\t// Read the child elements" << endl
				<< "\tfor (xmlNodePtr s = n->next; s; s = s->next)" << endl
				<< "\t{" << endl;
		std::for_each(s.begin(), s.end(), bind(source_read_elements, this, _1,
				ref(source)));
		source << "\t}" << endl
				<< endl;
	}
	source << "\t// Retreive value" << endl
			<< "\tfor (xmlNodePtr c = n->children; c; c = c->next)" << endl
			<< "\t{" << endl
			<< "\t\tif (xmlNodeIsText(c))" << endl
			<< "\t\t{" << endl
			<< "\t\t\txmlChar* t = xmlNodeGetContent(c);" << endl
			<< "\t\t\t_value += reinterpret_cast<const char*>(t);" << endl
			<< "\t\t\txmlFree(t);" << endl
			<< "\t\t}" << endl
			<< "\t}" << endl
			<< "}" << endl
			<< endl;

	// Add write to file
	source << "void " << namec << "::write(const char* f) const" << endl
			<< "{" << endl
			<< "\t// Create document" << endl
			<< "\txmlDocPtr d = xmlNewDoc(BAD_CAST XML_DEFAULT_VERSION);" << endl
			<< "\tif (!d)" << endl
			<< "\t\tthrow std::runtime_error(\"new document creation failed\");" << endl
			<< "\t" << endl
			<< "\ttry" << endl
			<< "\t{"
			<< "\t\t// Create root node" << endl
			<< "\t\txmlNodePtr r = xmlNewDocNode(d, NULL, BAD_CAST \"" << name << "\"," << endl
			<< "\t\t\t\tBAD_CAST _value.c_str());" << endl
			<< "\t\tif (!r)" << endl
			<< "\t\t\tthrow std::runtime_error(\"failed creating root '" << name << "'\");" << endl
			<< "\t\t" << endl
			<< "\t\t// Add the child elements" << endl
			<< "\t\twrite(r);" << endl
			<< "\t\t" << endl
			<< "\t\t// Save to file" << endl
			<< "\t\tint s = xmlSaveFormatFile(f, d, 1);" << endl
			<< "\t\tif (s == -1)" << endl
			<< "\t\t\tthrow std::runtime_error(\"failed writing file\");" << endl
			<< "\t\t" << endl
			<< "\t\t// All done!" << endl
			<< "\t\txmlFreeDoc(d);" << endl
			<< "\t}" << endl
			<< "\tcatch (...)" << endl
			<< "\t{" << endl
			<< "\t\txmlFreeDoc(d);" << endl
			<< "\t\tthrow;" << endl
			<< "\t}" << endl
			<< "}" << endl
			<< endl;

	// Add write to node
	source << "void " << namec << "::write(xmlNodePtr p) const" << endl
			<< "{" << endl
			<< "\t// Create node for this object" << endl
			<< "\txmlNodePtr s = xmlNewChild(p, NULL, reinterpret_cast<const xmlChar*>(\"" << namec << "\")," << endl
			<< "\t\t\treinterpret_cast<const xmlChar*>(_value.c_str()));" << endl
			<< "\tif (!s)" << endl
			<< "\t\tthrow std::runtime_error(\"failed to create child node '" << name << "'!\");" << endl
			<< "\t" << endl;
	std::for_each(a.begin(), a.end(), bind(source_write_attributes, this, _1,
			ref(source)));
	if (!s.empty())
	{
		source << "\tsize_t i;" << endl;
		std::for_each(s.begin(), s.end(), bind(source_write_elements, this, _1,
				ref(source)));
	}
	source << "}" << endl;
}

void Generator::source_ope_attributes(const std::string& v, std::ofstream& o)
{
	//	_attribute = r._attribute;

	o << "\t\t_" << v << " = r._" << v << ";" << endl;
}

void Generator::source_ope_elements(const std::string& v, std::ofstream& o)
{
	//	_elements = r._elements;
	
	const std::string name = fixup(v);
	
	std::string names = name;
	if (names[names.size() - 1] != 's')
		names += "s";

	o << "\t\t_" << names << " = r._" << names << ";" << endl;
}

void Generator::source_read_attributes(const std::string& v, std::ofstream& o)
{
	o << "\ta = xmlGetProp(n, reinterpret_cast<xmlChar*>(\"" << v << "\"));" << endl
			<< "\tif (a)" << endl
			<< "\t{" << endl
			<< "\t\t_" << v << " = reinterpret_cast<const char*>(a);" << endl
			<< "\t\txmlFree(a);" << endl
			<< "\t}" << endl;
}

void Generator::source_read_elements(const std::string& v, std::ofstream& o)
{
	const std::string name = fixup(v);
	
	std::string names = name;
	if (names[names.size() - 1] != 's')
		names += "s";
	
	std::string namec = name;
	namec[0] = std::toupper(namec[0], std::locale());
	
	o << "\t\tif (strcmp(reinterpret_cast<const char*>(s->name), \"" << v << "\") == 0)" << endl
			<< "\t\t{" << endl
			<< "\t\t\t_" << names << ".push_back(" << namec << "(n));" << endl
			<< "\t\t\tcontinue;" << endl
			<< "\t\t}" << endl;
}

void Generator::source_write_attributes(const std::string& v, std::ofstream& o)
{
	//	// Write attribute attribute.
	//	if (!_attribute.empty())
	//	{
	//		a = xmlSetProp(n, "attribute", _attribute);
	//		if (!a)
	//			throw std::runtime_error("write attribute attribute failed");
	//	}
	//
	o << "\t// Write attribute " << v << endl
			<< "\tif (!_" << v << ".empty())" << endl
			<< "\t{" << endl
			<< "\t\txmlAttrPtr a = xmlSetProp(s, reinterpret_cast<const xmlChar*>(\"" << v << "\")," << endl
			<< "\t\t\t\treinterpret_cast<const xmlChar*>(_" << v << ".c_str()));" << endl
			<< "\t\tif (!a)" << endl
			<< "\t\t\tthrow std::runtime_error(\"write attribute '" << v << "' failed!\");" << endl
			<< "\t}" << endl
			<< endl;
}

void Generator::source_write_elements(const std::string& v, std::ofstream& o)
{
	//	// Write element elements
	//	for (i = 0; i < _elements.size(); ++i)
	//		_elements[i].write(s);
	//

	const std::string name = fixup(v);
	
	std::string names = name;
	if (names[names.size() - 1] != 's')
		names += "s";
	
	std::string namec = name;
	namec[0] = std::toupper(namec[0], std::locale());

	o << "\t// Write " << name << " elements" << endl
			<< "\tfor (i = 0; i < _" << names << ".size(); ++i)" << endl
			<< "\t\t_" << names << "[i].write(s);" << endl
			<< endl;
}
