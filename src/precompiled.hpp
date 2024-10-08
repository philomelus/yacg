#ifndef	__PRECOMPILED_HPP__
#define	__PRECOMPILED_HPP__

// Boost signals option: statically linked
#define	BOOST_SIGNALS_STATIC_LINK

// Allegro
#ifdef	_DEBUG
#define	DEBUGMODE 1
#endif
#include "allegro.h"

// std
#include <algorithm>
#include <limits>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <iomanip>

// boost
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/lambda/algorithm.hpp>
#include <boost/cast.hpp>
#include <boost/signals2.hpp>

using boost::bind;
using boost::numeric_cast;
using boost::ref;
using namespace boost::signals2;

#endif
