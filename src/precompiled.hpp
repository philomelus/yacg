#ifndef	__PRECOMPILED_HPP__
#define	__PRECOMPILED_HPP__

// Boost signals option: statically linked
#define	BOOST_SIGNALS_STATIC_LINK

// Allegro
#pragma warning(push)
#pragma warning(disable:4312)
#ifdef	_DEBUG
#define	DEBUGMODE 1
#endif
#include "allegro.h"
#pragma warning(pop)

#define	__STD_ALGORITHM__
#include <algorithm>

#define	__STD_LIMITS__
#include <limits>

#define	__STD_LIST__
#include <list>

#define	__STD_MAP__
#include <map>

#define	__STD_STRSTREAM__
#include <strstream>

#define	__STD_STRING__
#include <string>

#define	__BOOST_LAMBDA_LAMBDA__
#include <boost/lambda/lambda.hpp>

#define	__BOOST_BIND__
#include <boost/bind.hpp>

#define	__BOOST_LAMBDA_ALGORITHM__
#include <boost/lambda/algorithm.hpp>

#define	__BOOST_CAST_HPP__
#include <boost/cast.hpp>

#define	__BOOST_SIGNALS_HPP__
#include <boost/signals.hpp>

using boost::bind;
using boost::numeric_cast;
using boost::ref;
using boost::signal;

#endif
