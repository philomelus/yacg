//
// Copyright (c) 2004 Russell E. Gibson
// email: russg@rnstech.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "TestSuite.hpp"

#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <string>

#include <boost/bind.hpp>

#include <indent.hpp>

using namespace utility;

using boost::bind;
using boost::ref;
using std::cout;
using std::cerr;
using std::endl;

void copy(const testSuite::TestSuite& o, const char* f);

void Test2()
{
	cout << "test2: Executing" << endl;
	
	try
	{
		// Read in file to test read functions
		testSuite::TestSuite ts("test2.xml");
		
		// Write it out ourself to test the generated code
		copy(ts, "test2copy1.xml");
		
		// Ask libxml to write it to test write functions
		ts.write("test2copy2.xml");
		
		std::cout << "test2: Check 'test2copy1.xml' and 'test2copy2.xml'" << std::endl;
		std::cout << "test2: Otherwise it appears successful" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "test2: Exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "test2: unknown exception caught" << std::endl;
	}
}

void dump_documentations(const testSuite::Documentation& a, std::ofstream& o, indent& l)
{
	o << l << "<documentation>";
	std::string v;
	a.value(v);
	o << v;
	o << "</documentation>" << endl;
}

void dump_params(const testSuite::Param& p, std::ofstream& o, indent& l)
{
	o << l << "<param";
	if (!p.name().empty())
		o << " name=\"" << p.name() << "\"";
	o << ">";
	std::string v;
	p.value(v);
	if (!v.empty())
		o << v;
	o << "</param>" << endl;
}

void dump_datas(const testSuite::Data& d, std::ofstream& o, indent& l)
{
	o << l << "<data";
	if (!d.type().empty())
		o << " type=\"" << d.type() << "\"";
	std::string v;
	d.value(v);
	if (v.empty() && d.params().empty())
	{
		o << "/>" << endl;
	}
	else
	{
		o << ">" << endl;
		if (!v.empty())
			o << l << v << endl;
		{
			auto_indent i(l);
			std::for_each(d.params().begin(), d.params().end(),
					bind(dump_params, _1, ref(o), ref(l)));
		}
		o << l << "</data>" << endl;
	}
}

void dump_values(const testSuite::Value& v, std::ofstream& o, indent& l)
{
	o << l << "<value";
	if (!v.type().empty())
		o << " type=\"" << v.type() << "\"";
	o << ">";
	std::string vv;
	v.value(vv);
	if (!vv.empty())
		o << vv;
	o << "</value>" << endl;
}

void dump_elements(const testSuite::Element& e, std::ofstream& o, indent& l)
{
	o << l << "<element";
	if (!e.datatypeLibrary().empty())
		o << " datatypeLibrary=\"" << e.datatypeLibrary() << "\"";
	if (!e.name().empty())
		o << " name=\"" << e.name() << "\"";
	o << ">" << endl;
	std::string v;
	e.value(v);
	if (!v.empty())
		o << l << v << endl;
	{
		auto_indent i(l);
		std::for_each(e.datas().begin(), e.datas().end(),
				bind(dump_datas, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(e.Values().begin(), e.Values().end(),
				bind(dump_values, _1, ref(o), ref(l)));
	}
	o << l << "</element>" << endl;
}

void dump_corrects(const testSuite::Correct& c, std::ofstream& o, indent& l)
{
	o << l << "<correct>" << endl;
	std::string v;
	c.value(v);
	if (!v.empty())
		o << v;
	{
		auto_indent i(l);
		std::for_each(c.elements().begin(), c.elements().end(),
				bind(dump_elements, _1, ref(o), ref(l)));
	}
	o << l << "</correct>" << endl;
}

void dump_docs(const testSuite::Doc& r, std::ofstream& o, indent& l)
{
	std::string v;
	r.value(v);
	if (v.empty())
		o << l << "<doc/>" << endl;
	else
		o << l << "<doc>" << v << "</doc>" << endl;
}

void dump_invalids(const testSuite::Invalid& i, std::ofstream& o, indent& l)
{
	std::string v;
	i.value(v);
	if (i.docs().empty())
	{
		if (v.empty())
			o << l << "<invalid/>" << endl;
		else
			o << l << "<invalid>" << v << "</invalid>" << endl;
	}
	else
	{
		o << l << "<invalid>" << endl;
		if (!v.empty())
			o << l << v << endl;
		{
			auto_indent in(l);
			std::for_each(i.docs().begin(), i.docs().end(),
					bind(dump_docs, _1, ref(o), ref(l)));
		}
		o << l << "</invalid>" << endl;
	}
}

void dump_requires(const testSuite::Requires& r, std::ofstream& o, indent& l)
{
	o << l << "<requires";
	if (!r.datatypeLibrary().empty())
		o << " datatypeLibrary=\"" << r.datatypeLibrary() << "\"";
	std::string v;
	r.value(v);
	if (v.empty())
		o << "/>" << endl;
	else
		o << ">" << v << "</requires>" << endl;
}

void dump_valids(const testSuite::Valid& v, std::ofstream& o, indent& l)
{
	o << l << "<valid";
	if (!v.dtd().empty())
		o << " dtd=\"" << v.dtd() << "\"";
	std::string vv;
	v.value(vv);
	if (v.docs().empty())
	{
		if (vv.empty())
			o << "/>" << endl;
		else
			o << ">" << vv << "</valid>" << endl;
	}
	else
	{
		o << ">" << endl;
		if (!vv.empty())
			o << l << vv << endl;
		{
			auto_indent in(l);
			std::for_each(v.docs().begin(), v.docs().end(),
					bind(dump_docs, _1, ref(o), ref(l)));
		}
		o << l << "</valid>" << endl;
	}
}

void dump_testCases(const testSuite::TestCase& t, std::ofstream& o, indent& l)
{
	o << l << "<testCase>" << endl;
	std::string v;
	t.value(v);
	if (!v.empty())
		o << v;
	{
		auto_indent i(l);
		std::for_each(t.corrects().begin(), t.corrects().end(),
				bind(dump_corrects, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(t.invalids().begin(), t.invalids().end(),
				bind(dump_invalids, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(t.requires().begin(), t.requires().end(),
				bind(dump_requires, _1, ref(o), ref(l)));
	}
	{
		auto_indent i(l);
		std::for_each(t.valids().begin(), t.valids().end(),
				bind(dump_valids, _1, ref(o), ref(l)));
	}
	o << l << "</testCase>" << endl;
}

void dump_testSuites(const testSuite::TestSuite& t, std::ofstream& o, indent& l)
{
	o << l << "<testSuite>" << endl;
	std::string v;
	t.value(v);
	if (!v.empty())
		o << v;
	{
		auto_indent i1(l);
		std::for_each(t.documentations().begin(), t.documentations().end(),
				bind(dump_documentations, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(t.testCases().begin(), t.testCases().end(),
				bind(dump_testCases, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(t.testSuites().begin(), t.testSuites().end(),
				bind(dump_testSuites, _1, ref(o), ref(l)));
	}
	o << l << "</testSuite>" << endl;
}

void copy(const testSuite::TestSuite& t, const char* f)
{
	indent level;
	
	std::ofstream o(f, std::ios_base::out | std::ios_base::trunc);
	
	o << "<?xml version=\"1.0\" encoding=\"iso8859-1\" standalone=\"yes\"?>" << endl;
	o << "<testSuite>" << endl;
	std::string v;
	t.value(v);
	if (!v.empty())
		o << v << endl;
	{
		auto_indent i1(level);
		std::for_each(t.documentations().begin(), t.documentations().end(),
				bind(dump_documentations, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(t.testCases().begin(), t.testCases().end(),
				bind(dump_testCases, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(t.testSuites().begin(), t.testSuites().end(),
				bind(dump_testSuites, _1, ref(o), ref(level)));
	}
	o << "</testSuite>" << endl;
}
