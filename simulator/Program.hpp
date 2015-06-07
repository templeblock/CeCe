
#pragma once

/* ************************************************************************ */

// C++
#include <functional>

// Simulator
#include "core/Units.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Object;

/* ************************************************************************ */

/**
 * @brief Program for objects.
 */
using Program = std::function<void(Object&, units::Duration)>;

/* ************************************************************************ */

}

/* ************************************************************************ */
