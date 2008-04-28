//
//  boost/memory/config.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef __BOOST_MEMORY_CONFIG_HPP__
#define __BOOST_MEMORY_CONFIG_HPP__

// -------------------------------------------------------------------------
// Configurations

#ifndef BOOST_MEMORY_ALLOC_PADDING
#define BOOST_MEMORY_ALLOC_PADDING	32
#endif

#ifndef BOOST_MEMORY_BLOCK_TOTAL
#define BOOST_MEMORY_BLOCK_TOTAL	16384	// 16k
#endif

#ifndef BOOST_MEMORY_BLOCK_SIZE
#define BOOST_MEMORY_BLOCK_SIZE		(BOOST_MEMORY_BLOCK_TOTAL - BOOST_MEMORY_ALLOC_PADDING)
#endif

// -------------------------------------------------------------------------

#ifndef BOOST_MEMORY_CALL
#define BOOST_MEMORY_CALL
#endif

// -------------------------------------------------------------------------

#ifndef __NS_BOOST_BEGIN
#define __NS_BOOST_BEGIN	namespace boost {
#define __NS_BOOST_END		}
#endif

// -------------------------------------------------------------------------

#endif /* __BOOST_MEMORY_CONFIG_HPP__ */
