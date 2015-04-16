
/* ************************************************************************ */

// Declaration
#include "simulator/Object.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Object::IdType Object::s_id = 0;

/* ************************************************************************ */

Object::Object(Simulation& simulation) noexcept
    : m_simulation(simulation)
    , m_id(++s_id)
{
    // Nothing to do
}

/* ************************************************************************ */

Object::~Object()
{
    // Nothing to do
}

/* ************************************************************************ */

}

/* ************************************************************************ */
