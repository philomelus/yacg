#include "Control.hpp"

#include <fstream>
#include <ios>
#include <iostream>
#include <string>

#include <boost/bind.hpp>

using boost::bind;
using boost::ref;
using std::cout;
using std::cerr;
using std::endl;

void copy(const control::Control& c, const char* f);

void Test1()
{
	try
	{
		// Read in file to test read functions
		control::Control ctrl("test1.xml");
		
		// Write it out ourself to test the generated code
		copy(ctrl, "copy1.xml");
		
		// Ask libxml to write it to test write functions
		ctrl.write("copy2.xml");
		
		std::cout << "test1: Check 'copy1.xml' and 'copy2.xml'" << std::endl;
		std::cout << "test1: Otherwise it appears successful" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "test1: Exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "test1: unknown exception caught" << std::endl;
	}
}

class indent
{
public:
	indent()
			:
			_i(0)
	{
	}
	indent& operator++()
	{
		++_i;
		return *this;
	}
	indent& operator--()
	{
		--_i;
		return *this;
	}
	std::ostream& space(std::ostream& o) const
	{
		for (int i = 0; i < _i; ++i)
			o << "\t";
		return o;
	}
private:
	int _i;
};

class auto_indent
{
public:
	auto_indent(indent& i)
			:
			_i(i)
	{
		++_i;
	}
	~auto_indent()
	{
		--_i;
	}
private:
	indent& _i;
};

std::ostream& operator<<(std::ostream& o, const indent& i)
{
	i.space(o);
	return o;
}

void dump_constructors(const control::Constructor& a, std::ofstream& o, indent& l)
{
	o << l << "<constructor>";
	std::string v;
	a.value(v);
	o << v;
	o << "</constructor>" << endl;
}

void dump_names(const control::Name& a, std::ofstream& o, indent& l)
{
	o << l << "<name>";
	std::string v;
	a.value(v);
	o << v;
	o << "</name>" << endl;
}

void dump_params(const control::Param& a, std::ofstream& o, indent& l)
{
	o << l << "<param>";
	std::string v;
	a.value(v);
	o << v;
	o << "</param>" << endl;
}

void dump_returns(const control::Return& a, std::ofstream& o, indent& l)
{
	o << l << "<return>";
	std::string v;
	a.value(v);
	o << v;
	o << "</return>" << endl;
}

void dump_styles(const control::Style& a, std::ofstream& o, indent& l)
{
	o << l << "<style";
	if (!a.unique().empty())
		o << " unique=\"" << a.unique() << "\"";
	o << ">";
	std::string v;
	a.value(v);
	o << v;
	o << "</style>" << endl;
}

void dump_types(const control::Type& a, std::ofstream& o, indent& l)
{
	o << l << "<type>";
	std::string v;
	a.value(v);
	o << v;
	o << "</type>" << endl;
}

void dump_attributes(const control::Attribute& a, std::ofstream& o, indent& l)
{
	o << l << "<attribute";
	if (!a.editable().empty())
		o << " editable=\"" << a.editable() << "\"";
	if (!a.Virtual().empty())
		o << " virtual=\"" << a.Virtual() << "\"";
	o << ">" << endl;
	{
		auto_indent i(l);
		std::for_each(a.constructors().begin(), a.constructors().end(),
				bind(dump_constructors, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.names().begin(), a.names().end(),
				bind(dump_names, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.params().begin(), a.params().end(),
				bind(dump_params, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.Returns().begin(), a.Returns().end(),
				bind(dump_returns, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.styles().begin(), a.styles().end(),
				bind(dump_styles, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.types().begin(), a.types().end(),
				bind(dump_types, _1, ref(o), ref(l)));
	}
	o << l << "</attribute>" << endl;
}

void dump_base(const control::Base& a, std::ofstream& o, indent& l)
{
	o << l << "<base>";
	std::string v;
	a.value(v);
	o << v;
	o << "</base>" << endl;
}

void dump_bits(const control::Bits& a, std::ofstream& o, indent& l)
{
	o << l << "<bits>";
	std::string v;
	a.value(v);
	o << v;
	o << "</bits>" << endl;
}

void dump_flag(const control::Flag& a, std::ofstream& o, indent& l)
{
	o << l << "<flag>" << endl;
	{
		auto_indent i(l);
		std::for_each(a.bits().begin(), a.bits().end(),
				bind(dump_bits, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.names().begin(), a.names().end(),
				bind(dump_names, _1, ref(o), ref(l)));
	}
	o << l << "</flag>" << endl;
}

void dump_flags(const control::Flags& a, std::ofstream& o, indent& l)
{
	o << l << "<flags";
	if (!a.group().empty())
		o << " group=\"" << a.group() << "\"";
	o << ">" << endl;
	{
		auto_indent i(l);
		std::for_each(a.flags().begin(), a.flags().end(),
				bind(dump_flag, _1, ref(o), ref(l)));
	}
	o << l << "</flags>" << endl;
}

void dump_header(const control::Header& a, std::ofstream& o, indent& l)
{
	o << l << "<header";
	if (!a.type().empty())
		o << " type=\"" << a.type() << "\"";
	o << ">";
	std::string v;
	a.value(v);
	o << v;
	o << "</header>" << endl;
}

void dump_implementation(const control::Implementation& a, std::ofstream& o, indent& l)
{
	o << l << "<implementation";
	if (!a.type().empty())
		o << " type=\"" << a.type() << "\"";
	o << ">";
	std::string v;
	a.value(v);
	o << v;
	o << "</implementation>" << endl;
}

void dump_theme(const control::Theme& a, std::ofstream& o, indent& l)
{
	o << l << "<theme>" << endl;
	{
		auto_indent i(l);
		std::for_each(a.names().begin(), a.names().end(),
				bind(dump_names, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(a.types().begin(), a.types().end(),
				bind(dump_types, _1, ref(o), ref(l)));
	}
	o << l << "</theme>" << endl;
}

void copy(const control::Control& c, const char* f)
{
	typedef std::vector<std::string> VS;

	indent level;
	
	std::ofstream o("copy1.xml", std::ios_base::out | std::ios_base::trunc);
	
	o << "<?xml version=\"1.0\">" << endl;
	o << "<control>" << endl;
	{
		auto_indent i1(level);
		std::for_each(c.attributes().begin(), c.attributes().end(),
				bind(dump_attributes, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.bases().begin(), c.bases().end(),
				bind(dump_base, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.flags().begin(), c.flags().end(),
				bind(dump_flags, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.headers().begin(), c.headers().end(),
				bind(dump_header, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.implementations().begin(), c.implementations().end(),
				bind(dump_implementation, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.names().begin(), c.names().end(),
				bind(dump_names, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.themes().begin(), c.themes().end(),
				bind(dump_theme, _1, ref(o), ref(level)));
	}
	o << "</control>" << endl;
}
