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

#include "Tv.hpp"

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

void copy(const tv::Tv& t, const char* f);

void Test3()
{
	cout << "test3: Executing" << endl;

	try
	{
		// Read in file to test read functions
		tv::Tv tv("test3.xml");
		
		// Write it out ourself to test the generated code
		copy(tv, "test3copy1.xml");
		
		// Ask libxml to write it to test write functions
		tv.write("test3copy2.xml");
		
		std::cout << "test3: Check 'test3copy2.xml'" << std::endl;
		std::cout << "test3: Otherwise it appears successful" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "test3: Exception: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "test3: unknown exception caught" << std::endl;
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

void dump_audios(const tv::Audio& a, std::ofstream& o, indent& l)
{
	o << l << "<audio>";
	o << "</audio>" << endl;
}

void dump_categorys(const tv::Category& c, std::ofstream& o, indent& l)
{
	o << l << "<category>";
	o << "</category>" << endl;
}

void dump_credits(const tv::Credits& c, std::ofstream& o, indent& l)
{
	o << l << "<credits>";
	o << "</credits>" << endl;
}

void dump_dates(const tv::Date& d, std::ofstream& o, indent& l)
{
	o << l << "<date>";
	o << "</date>" << endl;
}

void dump_descs(const tv::Desc& d, std::ofstream& o, indent& l)
{
	o << l << "<desc>";
	o << "</desc>" << endl;
}

void dump_episode_nums(const tv::Episode_num& e, std::ofstream& o, indent& l)
{
	o << l << "<episode-num>";
	o << "</episode-num>" << endl;
}

void dump_lengths(const tv::Length& len, std::ofstream& o, indent& l)
{
	o << l << "<length>";
	o << "</length>" << endl;
}

void dump_previously_showns(const tv::Previously_shown& p, std::ofstream& o, indent& l)
{
	o << l << "<previously-shown>";
	o << "</previously-shown>" << endl;
}

void dump_ratings(const tv::Rating& p, std::ofstream& o, indent& l)
{
	o << l << "<rating>";
	o << "</rating>" << endl;
}

void dump_star_ratings(const tv::Star_rating& s, std::ofstream& o, indent& l)
{
	o << l << "<star-rating>";
	o << "</star-rating>" << endl;
}

void dump_sub_titles(const tv::Sub_title& s, std::ofstream& o, indent& l)
{
	o << l << "<sub-titles>";
	o << "</sub-titles>" << endl;
}

void dump_subtitles(const tv::Subtitles& s, std::ofstream& o, indent& l)
{
	o << l << "<subtitles>";
	o << "</subtitles>" << endl;
}

void dump_titles(const tv::Title& p, std::ofstream& o, indent& l)
{
	o << l << "<title>";
	o << "</title>" << endl;
}

void dump_videos(const tv::Video& p, std::ofstream& o, indent& l)
{
	o << l << "<video>";
	o << "</video>" << endl;
}

void dump_channels(const tv::Channel& c, std::ofstream& o, indent& l)
{
	o << l << "<channel";
	if (!c.id().empty())
		o << " id=\"" << c.id() << "\"";
	o << ">" << endl;
	std::string v;
	c.value(v);
	if (!v.empty())
		o << l << v << endl;
	{
		auto_indent i1(l);
		std::for_each(c.display_names().begin(), c.display_names().end(),
				bind(dumpValue<tv::Display_name>, _1, "display_name", ref(o), ref(l)));
	}
	o << l << "</channel>" << endl;
}

void dump_programmes(const tv::Programme& p, std::ofstream& o, indent& l)
{
	o << l << "<programme";
	if (!p.channel().empty())
		o << " channel=\"" << p.channel() << "\"";
	if (!p.start().empty())
		o << " start=\"" << p.start() << "\"";
	if (!p.stop().empty())
		o << " stop=\"" << p.stop() << "\"";
	o << ">" << endl;
	std::string v;
	p.value(v);
	if (!v.empty())
		o << l << v << endl;
	{
		auto_indent i1(l);
		std::for_each(p.audios().begin(), p.audios().end(),
				bind(dump_audios, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.categorys().begin(), p.categorys().end(),
				bind(dump_categorys, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.credits().begin(), p.credits().end(),
				bind(dump_credits, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.dates().begin(), p.dates().end(),
				bind(dump_dates, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.descs().begin(), p.descs().end(),
				bind(dump_descs, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.episode_nums().begin(), p.episode_nums().end(),
				bind(dump_episode_nums, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.lengths().begin(), p.lengths().end(),
				bind(dump_lengths, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.previously_showns().begin(), p.previously_showns().end(),
				bind(dump_previously_showns, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.ratings().begin(), p.ratings().end(),
				bind(dump_ratings, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.star_ratings().begin(), p.star_ratings().end(),
				bind(dump_star_ratings, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.sub_titles().begin(), p.sub_titles().end(),
				bind(dump_sub_titles, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.subtitles().begin(), p.subtitles().end(),
				bind(dump_subtitles, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.titles().begin(), p.titles().end(),
				bind(dump_titles, _1, ref(o), ref(l)));
	}
	{
		auto_indent i1(l);
		std::for_each(p.videos().begin(), p.videos().end(),
				bind(dump_videos, _1, ref(o), ref(l)));
	}
	o << l << "</programme>" << endl;
}

void copy(const tv::Tv& t, const char* f)
{
	indent level;

	std::ofstream o(f, std::ios_base::out | std::ios_base::trunc);
	
	o << "<?xml version=\"1.0\" encoding=\"iso8859-1\" standalone=\"yes\"?>" << endl;
	o << "<tv";
	if (!t.generator_info_name().empty())
		o << " generator_info_name=\"" << t.generator_info_name() << "\"";
	if (!t.generator_info_url().empty())
		o << " generator_info_url=\"" << t.generator_info_url() << "\"";
	if (!t.source_info_name().empty())
		o << " source_info_name=\"" << t.source_info_name() << "\"";
	if (!t.source_info_url().empty())
		o << " source_info_url=\"" << t.source_info_url() << "\"";
	o << ">" << endl;
	std::string v;
	t.value(v);
	if (!v.empty())
		o << v << endl;
	{
		auto_indent i1(level);
		std::for_each(t.channels().begin(), t.channels().end(),
				bind(dump_channels, _1, ref(o), ref(level)));
	}
	{
		auto_indent i1(level);
		std::for_each(t.programmes().begin(), t.programmes().end(),
				bind(dump_programmes, _1, ref(o), ref(level)));
	}
	o << "</tv>" << endl;
}
