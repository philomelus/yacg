#include "Common.hpp"

#include <iostream>

#include <boost/bind.hpp>

#include "Generate.hpp"

using namespace impl;
using boost::bind;
using std::cout;
using std::cerr;
using std::endl;

// TODO:  Add option for namespace inclusion, as well as name of namespace
// TODO:  Add way to specify whether single or multiple of sub-elements

#define	major_divider	"//============================================================================="
#define	minor_divider	"//-------------------------------------------------------------------------"

//=============================================================================
// class Generator implementation
//=============================================================================

Generator::Generator(const std::string& d, const ELEMENTS& e)
		:
		_directory(d)
{
	// Create the headers
	std::for_each(e.begin(), e.end(), bind(header, this, _1));
	
	// Create the implementations
//	std::for_each(e.begin(), e.end(), bind(source, this, _1));
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
	
	// Create header multiple inclusion protection value		
	std::string	mip =	"__";
	mip += name;
	mip += "_HPP__";
	str_upper(mip);
	
	// Start header output
	header << "#ifndef\t" << mip << endl
			<< "#define\t" << mip << endl
			<< endl
			<< "#include <string>" << endl
			<< "#include <vector>" << endl
			<< endl
			<< "#include <libxml/parse.h>" << endl
			<< endl;

	// Add includes for sub-elements
	std::for_each(s.begin(), s.end(), bind(header_includes, this, _1, &header));
	
	// Start class declaration
	header << "namespace impl" << endl
			<< "{" << endl
		<< major_divider << endl
			<< "// Element \"" << name << "\" encapsulation." << endl
			<< "\tclass " << name << endl
			<< "\t{" << endl
		<< "\t" << minor_divider << endl
			<< "\tpublic:" << endl
			<< "\t\t" << name << "();" << endl
			<< "\t\t" << name << "(const char* f);" << endl
			<< "\t\t" << name << "(const " << name << "& c);" << endl
			<< "\t\tvirtual ~" << name << "() throw ();" << endl
			<< endl
			<< "\t\t" << name << "& operator=(const " << name << "& r);" << endl
			<< endl
		<< "\t" << minor_divider << endl
			<< "\tpublic:" << endl
			<< "\t\tvoid write(const char* f) const;" << endl
			<< "\t\tvoid write(xmlNodePtr p) const;" << endl
			<< endl;

	// Add attributes
	std::for_each(a.begin(), a.end(), bind(header_attributes, this, _1, &header));
	
	// Add the sub-elements
	std::for_each(s.begin(), s.end(), bind(header_elements, this, _1, &header));
	
	// Value of this element
	header << "\t" << minor_divider << endl
			<< "\tpublic:" << endl
			<< "\t\tconst std::string& value() const;" << endl
			<< "\t\tvoid value(const char* v);" << endl
			<< endl
			<< "\tprivate:" << endl
			<< "\t\tstd::string _value;" << endl;

	// Finish off header
	header << "\t};" << endl
			<< "}" << endl
			<< endl
			<< "#endif" << endl;
}

void Generator::header_attributes(const ATTRIBUTES::value_type& v, std::ofstream* o)
{
	//	//----...
	//	public:
	//		const std::string& attribute() const;
	//		void attribute(const char* s);
	//
	//	private:
	//		std::string _attribute;

	const std::string& name = v.first;
	(*o) << "\t" << minor_divider << endl
			<< "\tpublic:" << endl
			<< "\t\tconst std::string& " << name << "() const;" << endl
			<< "\t\tvoid " << name << "(const char* " << name[0] << ");" << endl
			<< endl
			<< "\tprivate:" << endl
			<< "\t\tstd::string _" << name << ";" << endl
			<< endl;
}

void Generator::header_elements(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	//	//----...
	//	public:
	//		std::vector<std::string>& element();
	//		const std::vector<std::string>& element() const;
	//
	//	private:
	//		std::vector<std::string> _element;

	const std::string& name = v.first;
	(*o) << "\t" << minor_divider << endl
			<< "\tpublic:" << endl
			<< "\t\tstd::vector<std::string>& " << name << "();" << endl
			<< "\t\tconst std::vector<std::string>& " << name << "() const;" << endl
			<< endl
			<< "\tprivate:" << endl
			<< "\t\tstd::vector<std::string> _" << name << ";" << endl
			<< endl;
}

void Generator::header_includes(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	//	#ifndef	__ELEMENT_HPP__
	//	#include "element.hpp"
	//	#endif
	
	// Generate header multiple inclusion protection
	std::string	mip =	"__";
	mip += v.first;
	mip += "_HPP__";
	str_upper(mip);

	// Include header
	(*o) << "#ifndef\t" << mip << endl
			<< "#include \"" << v.first << ".hpp\"" << endl
			<< "#endif" << endl
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
	const SUBELEMENTS& se = n.elements();

	// Include files
	source << "#include \"" << name << ".h\"" << endl
			<< endl;

	// Standard	Constructor
	source << name << "::" << name << "()" << endl
			<< "{" << endl
			<< "\t// Do any value initialization here that is necessary." << endl
			<< endl
			<< "}" << endl
			<< endl;

	// Standard	copy constructor
	source << name << "::" << name << "(const " << name << "& c)" << endl
			<< "{" << endl
			<< "\t// copy all attributes:" << endl;
	std::for_each(a.begin(), a.end(), bind(source_attributes_copy, this, _1, &source));
	source << endl;
	source << "\t// copy all sub-nodes:" << endl;
	std::for_each(se.begin(), se.end(), bind(source_elements_copy, this, _1, &source));
	source << endl;
	source << "\t// copy the text content:" << endl
			<< "\tm_text_content = c.m_text_content;" << endl
			<< endl
			<< "}" << endl
			<< endl;

	// Standard	destructor
	source << name << "::~" << name << "()" << endl
			<< "{" << endl
			<< "\t// Do anything necessary in the destructor here" << endl
			<< "}" << endl
			<< endl;

	// Default assignment operator
	source << name << "& " << name << "::operator=(const " << name << "& c)" << endl
			<< "{" << endl
			<< "// copy all attributes:" << endl;
	std::for_each(a.begin(), a.end(), bind(source_attributes_copy, this, _1, &source));
	source << endl;
	source << "\t// copy all sub-nodes:" << endl;
	std::for_each(se.begin(), se.end(), bind(source_elements_copy, this, _1, &source));
	source << endl;
	source << "\t// copy the text value:" << endl
			<< "\tm_text_content = c.m_text_content;" << endl
			<< endl
			<< "\treturn *this;" << endl
			<< "}" << endl
			<< endl;

	// Read	the	element
	source << "void " << name << "::Read(xmlNodePtr node)" << endl
			<< "{" << endl
			<< "\txmlNodePtr sub;" << endl
			<< endl
			<< "\tif(strcmp(reinterpret_cast<const char*>(node->name), \"" << name << "\") != 0){" << endl
			<< "\t\tthrow AnException(0, FL, \"Wrong node name (%s).  We expect (" << name << ")\", node->name);" << endl
			<< "\t}" << endl
			<< endl;
	if (a.size() > 0)
		source << "\t// Get the attributes:" << endl;
	std::for_each(a.begin(), a.end(), bind(source_attributes_read, this, _1, &source));
	if (se.size() > 0)
	{
		source << "\t// Get the sub nodes:" << endl
				<< "\tfor(sub = node->xmlChildrenNode; sub != NULL; sub = sub->next){" << endl;
		std::for_each(se.begin(), se.end(), bind(source_elements_read, this, _1, &source));
		source << endl
				<< "\t}" << endl
				<< endl;
	}
	source << "\t// Get the node content if any:" << endl
			<< "\tstring tmp_txt;" << endl
			<< "\tm_text_content = \"\";" << endl
			<< "\tm_text_content.release();" << endl
			<< "\tfor(sub = node->xmlChildrenNode; sub != NULL; sub = sub->next){" << endl
			<< "\t\tif(xmlNodeIsText(sub)){" << endl
			<< "\t\t\ttmp_txt << xmlNodeGetContent(sub);" << endl
			<< "\t\t\ttmp_txt.ltrim().rtrim();" << endl
			<< "\t\t\tm_text_content += tmp_txt;" << endl
			<< "\t\t}" << endl
			<< "\t}" << endl
			<< "}" << endl
			<< endl;

	// Read the element
	source << "void " << name << "::ReadRoot(xmlDocPtr doc)" << endl
			<< "{" << endl
			<< "\txmlNodePtr root;" << endl
			<< "\troot = xmlDocGetRootElement(doc);" << endl
			<< "\tthis->Read(root);" << endl
			<< "}" << endl
			<< endl;

	// Write the element
	source << "void " << name << "::Write(xmlNodePtr parent)" << endl
			<< "{" << endl
			<< "\txmlNodePtr sub;" << endl
			<< endl
			<< "\tsub = xmlNewChild(parent, NULL, \"" << name << "\"," << endl
			<< "\t\tm_text_content);" << endl
			<< endl;
	if (a.size() > 0)
		source << "\t// Set the attributes:" << endl;
	std::for_each(a.begin(), a.end(), bind(source_attributes_write, this, _1, &source));
	if (se.size() >	0)
	{
		source << "\t// Create the sub nodes:" << endl
				<< "\tint i;" << endl
				<< endl;
		std::for_each(se.begin(), se.end(), bind(source_elements_write, this, _1, &source));
	}
	source << "}" << endl;
	source << endl;

	// Write the element as the root of new document
	source << "void " << name << "::WriteRoot(xmlDocPtr doc)" << endl
			<< "{" << endl
			<< "\txmlNodePtr sub;" << endl
			<< endl
			<< "\tdoc->children = xmlNewDocNode(doc, NULL, \"" << name << "\"," << endl
			<< "\t\tm_text_content);" << endl
			<< "\tsub = doc->children;" << endl
			<< endl;
	if (a.size() > 0)
		source << "\t// Set the attributes:" << endl;
	std::for_each(a.begin(), a.end(), bind(source_attributes_write, this, _1, &source));
	if (n.elements().size() >	0)
	{
		source << "\t// Create the sub nodes:" << endl
				<< "\tint i;" << endl
				<< endl;
		std::for_each(se.begin(), se.end(), bind(source_elements_write, this, _1, &source));
	}
	source << "}" << endl;
	source << endl;
}

void Generator::source_attributes_copy(const ATTRIBUTES::value_type& v, std::ofstream* o)
{
	(*o) << "\tm_" << v.first << " = c.m_" << v.first << ";" << endl;
}

void Generator::source_attributes_read(const ATTRIBUTES::value_type& v, std::ofstream* o)
{
	(*o) << "\tm_" << v.first << " << xmlGetProp(node, \"" << v.first << "\");" << endl;
}

void Generator::source_attributes_write(const ATTRIBUTES::value_type& v, std::ofstream* o)
{
	(*o) << "\t// Attribute " << v.first << ":" << endl
			<< "\txmlSetProp(sub, \"" << v.first << "\"," << endl
			<< "\t\tm_" << v.first << ");" << endl
			<< endl;
}

void Generator::source_elements_copy(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	(*o) << "\t" << v.first << "_list = c." << v.first << "_list;" << endl;
}

void Generator::source_elements_read(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	(*o) << "\t\tif(strcmp(reinterpret_cast<const char*>(sub->name), \"" << v.first << "\") == 0){" << endl
			<< "\t\t\t" << v.first << " " << v.first << "_tmp;" << endl
			<< "\t\t\t" << v.first << "_tmp.Read(sub);" << endl
			<< "\t\t\t" << v.first << "_list.push_back(" << v.first << "_tmp);" << endl
			<< "\t\t\tcontinue;" << endl
			<< "\t\t}" << endl;
}

void Generator::source_elements_write(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	(*o) << "\tfor(i = 0; i < (int)" << v.first << "_list.size(); i++){" << endl
			<< "\t\t" << v.first << "_list[i].Write(sub);" << endl
			<< "\t}" << endl
			<< endl;
}
