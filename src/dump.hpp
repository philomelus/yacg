#ifndef	__DUMP_HPP__
#define	__DUMP_HPP__

namespace utility
{
	struct flags_dump
	{
		int bit;
		const char* title;
		int mask;
	};
	
	inline void dump_bitmap(const std::string& i, const std::string& t, BITMAP* bmp)
	{
		std::string bmps;
		if (bmp == ::screen)
			bmps = "Allegro Global Screen";
		else
		{
			std::ostrstream o;
			o << "$" << std::hex << std::setw(8) << std::setfill('0') << bmp << std::ends;
			bmps = o.str();
			o.freeze(false);
		}
		TRACE("%s%s: %s (%d x %d @ %d bpp)\n", i.c_str(), t.c_str(), bmps.c_str(),
				bmp->w, bmp->h, bitmap_color_depth(bmp));
	}
	
	inline void dump_bool(const std::string& i, const std::string& t, bool v)
	{
		TRACE("%s%s: %s\n", i.c_str(), t.c_str(), (v ? "True" : "False"));
	}
	
	inline void dump_color(const std::string& i, const std::string& t, int c)
	{
		if (get_color_depth() == 8)
		{
			TRACE("%s%s: %d (%d,%d,%d)\n", i.c_str(), t.c_str(), c, getr(c),
					getg(c), getb(c));
		}
		else
		{
			TRACE("%s%s: %d,%d,%d\n", i.c_str(), t.c_str(), getr(c), getg(c),
					getb(c));
		}
	}

	inline void dump_connect(const std::string& i, const std::string& t, bool c)
	{
		if (c)
			TRACE("%s%s: Connected\n", i.c_str(), t.c_str());
		else
			TRACE("%s%s:\n", i.c_str(), t.c_str());
	}
	
	inline void dump_dirty(const std::string& i, const std::string& t, int d)
	{
		std::string ds;
		if (d == yacg::_Control::dirty_all)
			ds = "dirty_all";
		else if (d == 0)
			ds = "clean";
		else
		{
			std::ostrstream o;
			o << "$" << std::hex << std::setw(8) << std::setfill('0')
					<< (d & (~yacg::_Control::update_display)) << std::ends;
			ds = o.str();
			o.freeze(false);
			if ((d & yacg::_Control::update_display) != 0)
				ds += " | update_display";
		}
		TRACE("%s%s: %s\n", i.c_str(), t.c_str(), ds.c_str());
	}
	
#if 0
	inline std::string dump_coordinate(int c)
	{
		std::ostrstream o;
		switch (c)
		{
		case yacg::_Control::undefined:
			return std::string("undefined");
			
		case yacg::_Control::calc:
			return std::string("calc");
			
		default:
			{
				std::ostrstream o;
				o << c;
				std::string s(o.str(), o.tellp());
				o.freeze(false);
				return s;
			}
			break;
		}
	}
#endif

	inline void dump_count(const std::string& i, const std::string& t, unsigned long v)
	{
		TRACE("%s%s: %d\n", i.c_str(), t.c_str(), v);
	}
	
	class dump_divider
	{
	public:
		dump_divider(const std::string& i, const std::string& t)
				:
				_indent(i),
				_title(t)
		{
			TRACE("%s|----- %s BEGIN ---->\n", _indent.c_str(), _title.c_str());
		}
		~dump_divider()
		{
			TRACE("%s|<---- %s END   -----\n", _indent.c_str(), _title.c_str());
		}
	
	private:
		const std::string _indent;
		const std::string _title;
	};
	
	inline void dump_flags(const std::string& i, const flags_dump* f, int v)
	{
		std::ostrstream o;
		o << i << "Flags:";
		bool first = true;
		int index = 0;
		while (f[index].title != 0)
		{
			if ((v & f[index].mask) == f[index].bit)
			{
				if (!first)
					o << " |";
				o << " " << f[index].title;
				first = false;
			}
			++index;
		}
		o << std::endl << std::ends;
		TRACE(o.str());
		o.freeze(false);
	}
	
#if 0
	inline bool dump_flag(const char* n, int v, int m, bool prefix)
	{
		if ((v & m) == 0)
			return false;
		if (prefix)
			TRACE(" | %s", n);
		else
			TRACE(" %s", n);
		return true;
	}
#endif

	inline void dump_font(const std::string& i, const std::string& t, FONT* f)
	{
		std::string fs;
		if (f == ::font)
			fs = "Allegro Global Font";
		else
		{
			std::ostrstream o;
			o << "$" << std::hex << std::setw(8) << std::setfill('0') << f << std::ends;
			fs = o.str();
			o.freeze(false);
		}
		TRACE("%s%s: %s\n", i.c_str(), t.c_str(), fs.c_str());
	}

	inline void dump_font(const std::string& i, FONT* f)
	{
		dump_font(i, "Font", f);
	}

	inline void dump_hex(const std::string& i, const std::string& t, unsigned int v)
	{
		TRACE("%s%s: $%08X\n", i.c_str(), t.c_str(), v);
	}
	
	inline void dump_hex(const std::string& i, const std::string& t, const void* v)
	{
		TRACE("%s%s: $%08X\n", i.c_str(), t.c_str(), v);
	}
	
	inline void dump_object(const std::string& i, const std::string& n, const void* o)
	{
		TRACE("%s%s @ $%08X\n", i.c_str(), n.c_str(), o);
	}

#if 0	
	inline void dump_wh(int w, int h)
	{
		TRACE(" Width: %d\n", dump_coordinate(w).c_str());
		TRACE(" Height: %d\n", dump_coordinate(h).c_str());
	}

	inline void dump_whrb(int w, int h, int r, int b)
	{
		TRACE(" Width (right): %s (%s)\n", dump_coordinate(w).c_str(),
				dump_coordinate(r).c_str());;
		TRACE(" Height (bottom): %s (%s)\n", dump_coordinate(h).c_str(),
				dump_coordinate(b).c_str());
	}

	inline void dump_xy(int x, int y)
	{
		TRACE(" Left: %s\n", dump_coordinate(x).c_str());
		TRACE(" Top: %s\n", dump_coordinate(y).c_str());
	}
	
	inline void dump_xywhrb(int x, int y, int w, int h, int r, int b)
	{
		dump_xy(x, y);
		dump_whrb(w, h, r, b);
	}
#endif
}

#endif
