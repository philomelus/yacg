#ifndef	__COMMON_HPP__
#define	__COMMON_HPP__

#include <map>
#include <string>
#include <vector>

namespace impl
{
//=============================================================================
// List of attributes for an element

	typedef std::vector<std::string> ATTRIBUTES;
	
//=============================================================================
// List of elements for an element

	typedef	std::map<std::string, int> SUBS;
	
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
		const std::string& name() const;
		void name(const char* n);
		void name(const std::string& n);
		
	private:
		std::string _n;
	
	//-------------------------------------------------------------------------
	public:
		SUBS& subs();
		const SUBS& subs() const;
					
	private:
		SUBS _s;
	};

//=============================================================================
// List of elements.

	typedef	std::map<std::string, Element> ELEMENTS;

//=============================================================================
// class Element inline implementation

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
			_s = r._s;
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
	
	inline SUBS& Element::subs()
	{
		return _s;
	}

	inline const SUBS& Element::subs() const
	{
		return _s;
	}
}

#endif
