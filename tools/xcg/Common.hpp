#ifndef	__COMMON_HPP__
#define	__COMMON_HPP__

#include <locale>
#include <map>
#include <string>
#include <vector>

namespace impl
{
//=============================================================================
// Attributes for an element

	typedef std::vector<std::string> ATTRIBUTES;
	
//=============================================================================
// Elements contained within a root element

	typedef	std::map<std::string, int> SUBELEMENTS;
	
//=============================================================================
// Root element

	class Element
	{
	public:
	//-------------------------------------------------------------------------
		Element();
		Element(const std::string& n);
		Element(const Element& r);
		
		Element& operator=(const Element& r);
	
	//-------------------------------------------------------------------------
	public:
		ATTRIBUTES& attributes();
		const ATTRIBUTES& attributes() const;

	private:
		ATTRIBUTES _a;

	//-------------------------------------------------------------------------
	public:
		SUBELEMENTS& elements();
		const SUBELEMENTS& elements() const;
					
	private:
		SUBELEMENTS _e;

	//-------------------------------------------------------------------------
	public:
		const std::string& name() const;
		void name(const char* n);
		void name(const std::string& n);
		
	private:
		std::string _n;
	};

//=============================================================================
// List of elements.

	typedef	std::map<std::string, Element> ELEMENTS;

//=============================================================================
// Useful functions

	inline std::string& str_lower(std::string& s)
	{
		std::locale l;
		const std::string::iterator e = s.end();
		for (std::string::iterator i = s.begin(); i != e; ++i)
			*i = std::tolower(*i, l);
		return s;
	}
	
	inline std::string str_dir(const char* f)
	{
		std::string dir(f);
		str_lower(dir);
		return std::string(f, dir.rfind(".") == 0 ? dir.size() : dir.rfind("."));
	}
	
//=============================================================================
// class Element inline implementation
//=============================================================================

	inline Element::Element()
	{
	}

	inline Element::Element(const Element& r)
	{
		*this = r;
	}

	inline Element::Element(const std::string& n)
			:
			_n(n)
	{
	}
	
	inline Element& Element::operator=(const Element& r)
	{
		if (this != &r)
		{
			_n = r._n;
			_e = r._e;
			_a = r._a;
		}
		return *this;
	}

	inline ATTRIBUTES& Element::attributes()
	{
		return _a;
	}

	inline const ATTRIBUTES& Element::attributes() const
	{
		return _a;
	}

	inline SUBELEMENTS& Element::elements()
	{
		return _e;
	}

	inline const SUBELEMENTS& Element::elements() const
	{
		return _e;
	}

	inline const std::string& Element::name() const
	{
		return _n;
	}

	inline void Element::name(const std::string& n)
	{
		_n = n;
	}
	
	inline void Element::name(const char* n)
	{
		_n = n;
	}
}

#endif
