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

// This tests the CDATA capabilities, which currently fail

#include "code.hpp"

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

void copy(const code::Code& c, const char* f);

void Test4()
{
	cout << "test4: Executing" << endl;

	try
	{
		// Read in file to test read functions
		code::Code c("test4.xml");
		
		// Write it out ourself to test the generated code
		copy(c, "test4copy1.xml");
		
		// Ask libxml to write it to test write functions
		c.write("test4copy2.xml");
		
		std::cout << "test4: Check 'test4copy1.xml' and 'test4copy2.xml'" << std::endl;
		std::cout << "test4: Otherwise it appears successful" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "test4: Exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "test4: unknown exception caught" << std::endl;
	}
}

template <typename T>
void dumpValue(const T& v, const char* n, std::ostream& o, indent& l)
{
	o << l << "<" << n;
	std::string val;
	v.value(val);
	if (val.empty())
		o << "/>" << endl;
	else
		o << ">" << val << "</" << n << ">" << endl;
}

void dump_child3(const code::Child3& c, std::ofstream& o, indent& l)
{
	o << l << "<child3>" << endl;
	std::string v;
	c.value(v);
	if (!v.empty())
		o << l << v << endl;
	{
		auto_indent i(l);
		std::for_each(c.child4s().begin(), c.child4s().end(),
				bind(dumpValue<code::Child4>, _1, "child4", ref(o), ref(l)));
	}
	o << l << "</child3>" << endl;
}

void dump_child2(const code::Child2& c, std::ofstream& o, indent& l)
{
	o << l << "<child2>" << endl;
	std::string v;
	c.value(v);
	if (!v.empty())
		o << l << v << endl;
	{
		auto_indent i(l);
		std::for_each(c.child3s().begin(), c.child3s().end(),
				bind(dump_child3, _1, ref(o), ref(l)));
	}
	o << l << "</child2>" << endl;
}

void dump_child1(const code::Child1& c, std::ofstream& o, indent& l)
{
	o << l << "<child1>" << endl;
	std::string v;
	c.value(v);
	if (!v.empty())
		o << l << v << endl;
	{
		auto_indent i(l);
		std::for_each(c.child2s().begin(), c.child2s().end(),
				bind(dump_child2, _1, ref(o), ref(l)));
	}
	o << l << "</child1>" << endl;
}

void copy(const code::Code& c, const char* f)
{
	indent level;
	
	std::ofstream o(f, std::ios_base::out | std::ios_base::trunc);
	
	o << "<?xml version=\"1.0\">" << endl;
	o << "<code>" << endl;
	{
		auto_indent i1(level);
		std::for_each(c.child1s().begin(), c.child1s().end(),
				bind(dump_child1, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(c.datas().begin(), c.datas().end(),
				bind(dumpValue<code::Data>, _1, "data", ref(o), ref(level)));
	}
	o << "</code>" << endl;
}
