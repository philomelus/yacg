#include "Common.hpp"
#include "Generate.hpp"

using namespace impl;

namespace
{
	void gen_header(const std::string& d, const Element& n, const std::string& name)
	{
		int	i;
		SUBS::const_iterator subs_it;
		std::string	filename(d);
		filename += "\\";
		filename +=	name;
		filename +=	".h";

		FILE* outfile =	fopen(filename.c_str(),	"w");
		if (outfile	== NULL)
		{
			printf("Error opening %s for output.\n", filename.c_str());
			return ;
		}

		std::string	ifdefname =	name;
		ifdefname += "_H";
		{
			const std::string::iterator	end	= ifdefname.end();
			for	(std::string::iterator i = ifdefname.begin(); i	!= end;	++i)
				*i = toupper(*i);
		}

		fprintf(outfile,
				"#ifndef %s\n"
				"#define %s\n"
				"\n"
				"//	Everything for xml\n"
				"#include <xmlinc.h>\n"
				"\n"
				"#include <std::string.h>\n"
				"using namespace SLib;\n"
				"\n"
				"#include <vector>\n"
				"using namespace std;\n"
				"\n",
				ifdefname.c_str(), ifdefname.c_str());

		fprintf(outfile, "// Includes for the sub-nodes:\n");
		for	(subs_it = n.subs().begin(); subs_it != n.subs().end(); ++subs_it)
		{
			fprintf(outfile, "#include \"%s.h\"\n",
					(*subs_it).first.c_str());
		}

		fprintf(outfile,
				"\n"
				"/**\n"
				"  * This class	handles	xml	read/write for the %s node.\n"
				"  * All attributes	are	represented	as public member \n"
				"  * variables,	and	all	sub	nodes are represented as direct\n"
				"  * class member variables, or	as vectors.\n"
				"  *\n"
				"  * @author <author_name>\n"
				"  * @version <version>\n"
				"  * @copyright	<copyright>\n"
				"  */\n"
				"class %s\n"
				"{\n"
				"\tpublic:\n"
				"\t\t/// Default Constructor\n"
				"\t\t%s();\n\n"
				"\t\t/// Default Copy Constructor\n"
				"\t\t%s(const %s& c);\n\n"
				"\t\t/// Standard Destructor\n"
				"\t\tvirtual ~%s();\n\n"
				"\t\t/// Default assignment	operator\n"
				"\t\t%s& operator=(const %s& c);\n\n"
				"\t\t/// Reads the given xml Node and it's children\n"
				"\t\tvoid Read(xmlNodePtr node);\n\n"
				"\t\t/// Reads the Root	node of	the	given xml document\n"
				"\t\tvoid ReadRoot(xmlDocPtr doc);\n\n"
				"\t\t/// Adds our contents as a	new	child to the given node\n"
				"\t\tvoid Write(xmlNodePtr parent);\n\n"
				"\t\t/// Adds our contents as the root of a	new	xml	document\n"
				"\t\tvoid WriteRoot(xmlDocPtr doc);\n\n"
				"\t\t//	The	attributes:\n",
				name.c_str(), name.c_str(),
				name.c_str(),
				name.c_str(), name.c_str(),
				name.c_str(),
				name.c_str(), name.c_str());

		for	(i = 0;	i <	(int) n.attributes().size(); i++)
		{
			fprintf(outfile, "\t\tstring m_%s;\n", n.attributes()[i].c_str());
		}

		fprintf(outfile, "\n\t\t// And the sub nodes:\n");

		for	(subs_it = n.subs().begin();subs_it != n.subs().end();subs_it++)
		{
			fprintf(outfile, "\t\tvector < %s >	%s_list;\n",
					subs_it->first.c_str(),
					subs_it->first.c_str());
		}

		fprintf(outfile,
				"\n\t\t// And finally, any text	that was in	the	node:\n"
				"\t\tstring	m_text_content;\n");

		fprintf(outfile,
				"\n\n};\n\n"
				"#endif	// %s Defined\n", ifdefname.c_str());

		fclose(outfile);
	}

	void gen_body(const std::string& d, const Element& n, const std::string& name)
	{
		int	i;
		SUBS::const_iterator subs_it;
		std::string	filename(d);
		filename += "\\";
		filename +=	name;
		filename +=	".cpp";

		FILE* outfile =	fopen(filename.c_str(),	"w");
		if (outfile	== NULL)
		{
			printf("Error opening %s for output.\n", filename.c_str());
			return ;
		}

		fprintf(outfile,
				"#include \"%s.h\"\n\n"
				"#include <AnException.h>\n\n",	name.c_str());

		// Standard	Constructor
		fprintf(outfile,
				"%s::%s()\n"
				"{\n"
				"\t// Do any value initialization here that	is necessary.\n"
				"\n"
				"}\n\n",
				name.c_str(),	name.c_str());

		// Standard	Copy Constructor
		fprintf(outfile,
				"%s::%s(const %s& c)\n"
				"{\n"
				"\t// copy all attributes:\n",
				name.c_str(),	name.c_str(),	name.c_str());

		for	(i = 0;	i <	(int)n.attributes().size(); i++)
		{
			fprintf(outfile, "\tm_%s = c.m_%s;\n", n.attributes()[i].c_str(), n.attributes()[i].c_str());
		}

		fprintf(outfile,
				"\n"
				"\t// copy all sub-nodes:\n");

		for	(subs_it = n.subs().begin();subs_it != n.subs().end();subs_it++)
		{
			fprintf(outfile,
					"\t%s_list = c.%s_list;\n",
					(*subs_it).first.c_str(),
					(*subs_it).first.c_str());
		}

		fprintf(outfile,
				"\n\t//	copy the text content:\n"
				"\tm_text_content =	c.m_text_content;\n\n"
				"}\n\n");


		// Standard	Destructor
		fprintf(outfile,
				"%s::~%s()\n"
				"{\n"
				"\t// Do anything necessary	in the destructor here\n"
				"}\n\n",
				name.c_str(),	name.c_str());

		// Default assignment operator
		fprintf(outfile,
				"%s& %s::operator=(const %s& c)\n"
				"{\n"
				"\t// copy all attributes:\n",
				name.c_str(),	name.c_str(),	name.c_str());

		for	(i = 0;	i <	(int)n.attributes().size(); i++)
		{
			fprintf(outfile, "\tm_%s = c.m_%s;\n", n.attributes()[i].c_str(), n.attributes()[i].c_str());
		}

		fprintf(outfile,
				"\n"
				"\t// copy all sub-nodes:\n");

		for	(subs_it = n.subs().begin();subs_it != n.subs().end();subs_it++)
		{
			fprintf(outfile,
					"\t%s_list = c.%s_list;\n",
					(*subs_it).first.c_str(),
					(*subs_it).first.c_str());
		}

		fprintf(outfile,
				"\n\t//	copy the text value:\n"
				"\tm_text_content =	c.m_text_content;\n\n"
				"\treturn *this;\n"
				"}\n\n");

		// Read	the	xml	node:
		fprintf(outfile,
				"void %s::Read(xmlNodePtr node)\n"
				"{\n"
				"\txmlNodePtr sub;\n"
				"\n"
				"\tif(strcmp(reinterpret_cast<const	char*>(node->name),	\"%s\")	!= 0){\n"
				"\t\tthrow AnException(0, FL, \"Wrong node name	(%%s).	We expect (%s)\", node->name);\n"
				"\t}\n"
				"\n",
				name.c_str(),
				name.c_str(),
				name.c_str());

		if (n.attributes().size() > 0)
		{
			fprintf(outfile,
					"\t// Get the attributes:\n");
		}

		for	(i = 0;	i <	(int)n.attributes().size(); i++)
		{
			fprintf(outfile,
					"\tm_%s	<< xmlGetProp(node,	\"%s\");\n",
					n.attributes()[i].c_str(),
					n.attributes()[i].c_str());
		}

		if (n.subs().size() >	0)
		{
			fprintf(outfile,
					"\t// Get the sub nodes:\n"
					"\tfor(sub = node->xmlChildrenNode;	sub	!= NULL; sub = sub->next){\n");

			for	(subs_it = n.subs().begin();subs_it != n.subs().end();subs_it++)
			{
				fprintf(outfile,
						"\t\tif(strcmp(reinterpret_cast<const char*>(sub->name), \"%s\") ==	0){\n"
						"\t\t\t%s %s_tmp;\n"
						"\t\t\t%s_tmp.Read(sub);\n"
						"\t\t\t%s_list.push_back(%s_tmp);\n"
						"\t\t\tcontinue;\n"
						"\t\t}\n",
						(*subs_it).first.c_str(),
						(*subs_it).first.c_str(), (*subs_it).first.c_str(),
						(*subs_it).first.c_str(),
						(*subs_it).first.c_str(), (*subs_it).first.c_str());
			}

			fprintf(outfile,
					"\n\t}\n\n");
		}

		fprintf(outfile,
				"\t// Get the node content if any:\n"
				"\tstring tmp_txt;\n"
				"\tm_text_content =	\"\";\n"
				"\tm_text_content.release();\n"
				"\tfor(sub = node->xmlChildrenNode;	sub	!= NULL; sub = sub->next){\n"
				"\t\tif(xmlNodeIsText(sub)){\n"
				"\t\t\ttmp_txt << xmlNodeGetContent(sub);\n"
				"\t\t\ttmp_txt.ltrim().rtrim();\n"
				"\t\t\tm_text_content += tmp_txt;\n"
				"\t\t}\n"
				"\t}\n"
				"}\n\n");

		// Read	the	xml	node:
		fprintf(outfile,
				"void %s::ReadRoot(xmlDocPtr doc)\n"
				"{\n"
				"\txmlNodePtr root;\n"
				"\troot	= xmlDocGetRootElement(doc);\n"
				"\tthis->Read(root);\n"
				"}\n\n",
				name.c_str());

		// Write the xml node:
		fprintf(outfile,
				"void %s::Write(xmlNodePtr parent)\n"
				"{\n"
				"\txmlNodePtr sub;\n\n"
				"\tsub = xmlNewChild(parent, NULL, \"%s\",\n"
				"\t\tm_text_content);\n\n",
				name.c_str(),
				name.c_str());

		if (n.attributes().size() > 0)
		{
			fprintf(outfile,
					"\t// Set the attributes:\n");
		}

		for	(i = 0;	i <	(int)n.attributes().size(); i++)
		{
			fprintf(outfile,
					"\t// Attribute	%s:\n"
					"\txmlSetProp(sub, \"%s\",\n"
					"\t\tm_%s);\n\n",
					n.attributes()[i].c_str(),
					n.attributes()[i].c_str(),
					n.attributes()[i].c_str());
		}

		if (n.subs().size() >	0)
		{
			fprintf(outfile,
					"\t// Create the sub nodes:\n"
					"\tint i;\n\n");

			for	(subs_it = n.subs().begin();subs_it != n.subs().end();subs_it++)
			{
				fprintf(outfile,
						"\tfor(i = 0; i	< (int)%s_list.size(); i++){\n"
						"\t\t%s_list[i].Write(sub);\n"
						"\t}\n\n",
						(*subs_it).first.c_str(),
						(*subs_it).first.c_str());
			}
		}

		fprintf(outfile, "}\n\n");

		// Write the xml node as the root of a new doc:
		fprintf(outfile,
				"void %s::WriteRoot(xmlDocPtr doc)\n"
				"{\n"
				"\txmlNodePtr sub;\n\n"
				"\tdoc->children = xmlNewDocNode(doc, NULL,	\"%s\",\n"
				"\t\tm_text_content);\n"
				"\tsub = doc->children;\n\n",
				name.c_str(),
				name.c_str());

		if (n.attributes().size() > 0)
		{
			fprintf(outfile,
					"\t// Set the attributes:\n");
		}

		for	(i = 0;	i <	(int)n.attributes().size(); i++)
		{
			fprintf(outfile,
					"\t// Attribute	%s:\n"
					"\txmlSetProp(sub, \"%s\",\n"
					"\t\tm_%s);\n\n",
					n.attributes()[i].c_str(),
					n.attributes()[i].c_str(),
					n.attributes()[i].c_str());
		}

		if (n.subs().size() >	0)
		{
			fprintf(outfile,
					"\t// Create the sub nodes:\n"
					"\tint i;\n\n");

			for	(subs_it = n.subs().begin();subs_it != n.subs().end();subs_it++)
			{
				fprintf(outfile,
						"\tfor(i = 0; i	< (int)%s_list.size(); i++){\n"
						"\t\t%s_list[i].Write(sub);\n"
						"\t}\n\n",
						(*subs_it).first.c_str(),
						(*subs_it).first.c_str());
			}
		}

		fprintf(outfile, "}\n\n");

		// That's it.
		fclose(outfile);
	}
}

void impl::gen_code(const std::string& d, ELEMENTS& elements)
{
	ELEMENTS::iterator	node_map_it;
	for	(node_map_it = elements.begin(); node_map_it !=	elements.end();
			node_map_it	++)
	{
		gen_header(d, node_map_it->second, node_map_it->first);
		gen_body(d, node_map_it->second, node_map_it->first);
	}
}
