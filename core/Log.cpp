
/* ************************************************************************ */

// Declaration
#include "core/Log.hpp"

// C++
#include <iostream>

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

std::ostream* Log::m_output = &std::cout;

/* ************************************************************************ */

std::ostream* Log::m_error = &std::cerr;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
