
#pragma once

/* ************************************************************************ */

// C++
#include <string>

// Simulator
#include "core/compatibility.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief File path type.
 */
using FilePath = std::string;

/* ************************************************************************ */

/**
 * @brief Tests if file path exists.
 *
 * @param path
 *
 * @return
 */
bool fileExists(const FilePath& path) NOEXCEPT;

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */