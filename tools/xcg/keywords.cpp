#include "common.hpp"
#include <boost/bind.hpp>

#pragma warning(push)
#pragma warning(disable:4244)
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning(pop)

#include <time.h>

using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace impl;
using boost::bind;
using boost::ref;

namespace
{
	inline std::string tolower(const std::string& s)
	{
		if (s.empty())
			return std::string();
		std::string n(s);
		std::locale l;
		const std::string::iterator e = n.end();
		for (std::string::iterator i = n.begin(); i != e; ++i)
			*i = std::tolower(*i, l);
		return std::string(n);
	}

	std::string transform(const std::string& s, char t)
	{
		if (s.empty())
			return std::string(s);
		std::string n(s);
		std::locale l = std::locale();
		switch (t)
		{
		case 'c':	// Capitalize first letter
			n[0] = std::toupper(n[0], l);
			break;

		case 'C':	// Capitalize everything
			{
				const std::string::iterator e = n.end();
				for (std::string::iterator i = n.begin(); i != e; ++i)
					*i = std::toupper(*i, l);
			}
			break;

		case 'p':	// Make plural
			{
				static const struct
				{
					const char* const title;
					const char* plural;
				} plurals[] =
				{
					{ "catagory",	"catagories" },
//					{ "data",		"data" },			// BUGBUG:
					{ "child",		"children" },
					{ 0, 0 }
				};
				
				// Use the direct replacement if it exists
				for (int i = 0; plurals[i].title; ++i)
				{
					if (n == plurals[i].title)
					{
						n = plurals[i].plural;
						return std::string(n);
					}
				}
				
				// Make it plural by adding an 's'
				if (n[n.size() - 1] != 's')
					n += 's';
			}
			break;

		case 't':	// Translate
			{
				const KEYWORDS& k = Keywords();
				const KEYWORDS::const_iterator e = k.end();
				for (KEYWORDS::const_iterator i = k.begin(); i != e; ++i)
				{
					if (n == (*i).name)
					{
						n = (*i).replacement;
						break;
					}
				}
			}
			break;

		case 'v':	// Validate
		case 'V':	// Validate and capitalize
			{
				static const struct
				{
					char invalid;
					char fix;
				} invalidCharacters[] =
				{
					{ '-',	'_' },
					{ 0,	0 }
				};
				while (true)
				{
					const std::string::iterator ne = n.end();
					for (std::string::iterator ni = n.begin(); ni != ne; ++ni)
					{
						for (int i = 0; invalidCharacters[i].invalid != 0; ++i)
						{
							if (*ni == invalidCharacters[i].invalid)
							{
								if (t == 'v')
								{
									// Replace the invalid char
									*ni = invalidCharacters[i].fix;
									break;
								}
								else
								{
									// Uppercase next letter
									std::string::iterator niu = ni;
									++niu;
									if (niu != ne)
										*niu = std::toupper(*niu, std::locale());
									
									// Remove offending character
									n.erase(ni);
									break;
								}
							}
						}
						if (invalidCharacters[i].invalid)
							break;
					}
					if (ni == ne)
						break;
				}
			}
			break;

		default:
			std::cerr << "Invalid transformation char: '" << t << "'!" << std::endl;
			break;
		}
		return std::string(n);
	}
}

//=============================================================================
// Update keywords within string

void impl::update_keywords(std::string& s, const std::string& root, const std::string& attribute,
		const std::string& child, const std::string& element)
{
	static const std::string t_attribute("attribute");
	static const std::string t_child("child");
	static const std::string t_element("element");
	static const std::string t_root("root");
	
	for (std::string::size_type i = 0; i < s.size(); ++i)
	{
		if (s[i] == '<')
		{
			// Make sure we can check for next charactor
			if (i + 1 >= s.size())
				break;
			
			// If its another <, then skip it and move on
			if (s[i + 1] == '<')
			{
				// Erase one of the <
				s.erase(i, 1);
				continue;
			}
			
			// Replacement string
			std::string replacement;
			std::string::size_type replacementSize;
			std::string key;
			std::string::size_type keySize;
			
			// Is it for an element?
			if (t_element == s.substr(i + 1, t_element.size()))
			{
				replacement = element;
				replacementSize = 0;
				key = t_element;
				keySize = key.size();
			}
			// Note.  Is it for an attribute?
			else if (t_attribute == s.substr(i + 1, t_attribute.size()))
			{
				replacement = attribute;
				replacementSize = 0;
				key = t_attribute;
				keySize = key.size();
			}
			// Nope.  Is it for a child element?
			else if (t_child == s.substr(i + 1, t_child.size()))
			{
				replacement = child;
				replacementSize = 0;
				key = t_child;
				keySize = key.size();
			}
			// Nope.  How about root element?
			else if (t_root == s.substr(i + 1, t_root.size()))
			{
				replacement = root;
				replacementSize = 0;
				key = t_root;
				keySize = key.size();
			}
			// Nope.  How about date?
			// Nope.  How about time?
			// Nope.  How about date and time?
			// Nope.  How about first char of attribute?
			else if ((s[i + 2] == '-' || s[i + 2] == '>')
					&& s[i + 1] == 'a')
			{
				replacement = attribute;
				replacementSize = 1;
				key = "a";
				keySize = 1;
			}
			// Nope.  How about first char of child element?
			else if ((s[i + 2] == '-' || s[i + 2] == '>')
					&& s[i + 1] == 'c')
			{
				replacement = child;
				replacementSize = 1;
				key = "c";
				keySize = 1;
			}
			// Nope.  How about first char of root?
			else if ((s[i + 2] == '-' || s[i + 2] == '>')
					&& s[i + 1] == 'r')
			{
				replacement = root;
				replacementSize = 1;
				key = "r";
				keySize = 1;
			}
			
			// Perform replacement if needed
			if (!key.empty())
			{
				// Locate end of replacement
				for (std::string::size_type ie = i + 1 + keySize; ie < s.size(); ++ie)
				{
					if (s[ie] == '>')
						break;
				}
				if (ie == s.size())
				{
					std::cerr << "Replacement token never closes: <" << key << "!" << std::endl;
					return;
				}
				
				// Perfrom any needed transformations
				if (s[i + 1 + keySize] == '-')
				{
					std::string transforms = s.substr(i + keySize + 2, ie - i - keySize - 2);
					if (transforms.find('t') != std::string::npos)
						replacement = transform(replacement, 't');
					std::string::size_type ti = transforms.find_first_of("vV");
					if (ti != std::string::npos)
						replacement = transform(replacement, transforms[ti]);
					if (transforms.find('p') != std::string::npos)
						replacement = transform(replacement, 'p');
					ti = transforms.find_first_of("cC");
					if (ti != std::string::npos)
						replacement = transform(replacement, transforms[ti]);
					if (replacementSize > 1)
						replacementSize = replacement.size();
				}
				
				// Erase keyword
				s.erase(i, ie - i + 1);
				
				// Insert replacement
				s.insert(i, replacementSize
						? replacement.substr(0, replacementSize) : replacement);
				
				// Update next character to look at
				i += (replacementSize ? replacementSize : replacement.size());
			}
		}
	}
}
