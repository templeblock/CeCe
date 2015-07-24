/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <sstream>

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Input string stream type.
 */
using InStringStream = std::istringstream;

/* ************************************************************************ */

/**
 * @brief Output string stream type.
 */
using OutStringStream = std::ostringstream;

/* ************************************************************************ */

/**
 * @brief Input/Output string stream type.
 */
using StringStream = std::stringstream;

/* ************************************************************************ */

}

/* ************************************************************************ */