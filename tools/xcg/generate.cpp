#include "Common.hpp"

#include <ios>
#include <iostream>
#include <fstream>
#include <boost/bind.hpp>

#include "Generate.hpp"

using namespace impl;
using boost::bind;
using std::cout;
using std::cerr;
using std::endl;

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
	std::for_each(e.begin(), e.end(), bind(source, this, _1));
}

void Generator::header(const ELEMENTS::value_type& v)
{
	// Create and open file
	const std::string& name = v.first;
	std::string	filename(_directory);
	filename += "\\";
	filename +=	name;
	filename +=	".h";
	std::ofstream header(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
	if (!header)
	{
		cerr << "Unable to create and/or open \"" << filename << "\"" << endl;
		return;
	}
	cout << "Generating \"" << filename << "\"..." << endl;
	
	// For effeciency sake, create locals for often-used values
	const Element& n = v.second;
	const ATTRIBUTES& a = n.attributes();
	const SUBELEMENTS& se = n.elements();
	
	// Create header multiple inclusion protection value		
	std::string	ifdefname =	name;
	ifdefname += "_H";
	str_upper(ifdefname);
	
	// Start header output
	header << "#ifndef\t" << ifdefname << endl
			<< "#define\t" << ifdefname << endl
			<< endl
			<< "//\tEverything for xml" << endl
			<< "#include <xmlinc.h>" << endl
			<< endl
			<< "#include <std::string.h>" << endl
			<< "using namespace SLib;" << endl
			<< endl
			<< "#include <vector>" << endl
			<< "using namespace std;" << endl
			<< endl;

	// Add includes for sub-elements
	header << "// Includes for the sub-nodes:" << endl;
	std::for_each(se.begin(), se.end(), bind(header_includes, this, _1, &header));
	header << endl;
	
	// Start class declaration
	header << "/**" << endl
			<< "  * This class	handles	xml	read/write for the " << name << " node." << endl
			<< "  * All attributes	are	represented	as public member " << endl
			<< "  * variables,	and	all	sub	nodes are represented as direct" << endl
			<< "  * class member variables, or	as vectors." << endl
			<< "  *" << endl
			<< "  * @author <author_name>" << endl
			<< "  * @version <version>" << endl
			<< "  * @copyright	<copyright>" << endl
			<< "  */" << endl
			<< "class " << name << endl
			<< "{" << endl
			<< "\tpublic:" << endl
			<< "\t\t/// Default Constructor" << endl
			<< "\t\t" << name << "();" << endl
			<< endl
			<< "\t\t/// Default Copy Constructor" << endl
			<< "\t\t" << name << "(const " << name << "& c);" << endl
			<< endl
			<< "\t\t/// Standard Destructor" << endl
			<< "\t\tvirtual ~" << name << "();" << endl
			<< endl
			<< "\t\t/// Default assignment	operator" << endl
			<< "\t\t" << name << "& operator=(const " << name << "& c);" << endl
			<< endl
			<< "\t\t/// Reads the given xml Node and it's children" << endl
			<< "\t\tvoid Read(xmlNodePtr node);" << endl
			<< endl
			<< "\t\t/// Reads the Root	node of	the	given xml document" << endl
			<< "\t\tvoid ReadRoot(xmlDocPtr doc);" << endl
			<< endl
			<< "\t\t/// Adds our contents as a	new	child to the given node" << endl
			<< "\t\tvoid Write(xmlNodePtr parent);" << endl
			<< endl
			<< "\t\t/// Adds our contents as the root of a	new	xml	document" << endl
			<< "\t\tvoid WriteRoot(xmlDocPtr doc);" << endl
			<< endl;

	// Add attributes
	header << "\t\t// The attributes:" << endl;
	std::for_each(a.begin(), a.end(), bind(header_attributes, this, _1, &header));
	header << endl;
	
	// Add the sub-elements
	header << "\t\t// And the sub nodes:" << endl;
	std::for_each(se.begin(), se.end(), bind(header_elements, this, _1, &header));
	header << endl;
	
	// Finish up class declaration
	header << "\t\t// And finally, any text that was in the node:" << endl
			<< "\t\tstring m_text_content;\n" << endl
			<< endl
			<< "};" << endl
			<< endl
			<< "#endif	// " << ifdefname << " Defined" << endl;
}

void Generator::header_attributes(const std::string& v, std::ofstream* o)
{
	(*o) << "\t\tstring m_" << v << ";" << endl;
}

void Generator::header_elements(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	(*o) << "\t\tvector < " << v.first << " > " << v.first << "_list;" << endl;
}

void Generator::header_includes(const SUBELEMENTS::value_type& v, std::ofstream* o)
{
	(*o) << "#include \"" << v.first << ".h\"" << endl;
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
		cerr << "Unable to create and/or open \"" << filename << "\"" << endl;
		return;
	}
	cout << "Generating \"" << filename << "\"..." << endl;

	// For effeciency sake, create locals for often-used values
	const Element& n = v.second;
	const ATTRIBUTES& a = n.attributes();
	const SUBELEMENTS& se = n.elements();

	// Include files
	source << "#include \"" << name << ".h\"" << endl
			<< endl
			<< "#include <AnException.h>" << endl
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

void Generator::source_attributes_copy(const std::string& v, std::ofstream* o)
{
	(*o) << "\tm_" << v << " = c.m_" << v << ";" << endl;
}

void Generator::source_attributes_read(const std::string& v, std::ofstream* o)
{
	(*o) << "\tm_" << v << " << xmlGetProp(node, \"" << v << "\");" << endl;
}

void Generator::source_attributes_write(const std::string& v, std::ofstream* o)
{
	(*o) << "\t// Attribute " << v << ":" << endl
			<< "\txmlSetProp(sub, \"" << v << "\"," << endl
			<< "\t\tm_" << v << ");" << endl
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
