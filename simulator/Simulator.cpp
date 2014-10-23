
/* ************************************************************************ */

// Declaration
#include "simulator/Simulator.h"

// C++
#include <cassert>

// Simulator
#include "simulator/World.h"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Simulator::Simulator()
{
    // Nothing
}

/* ************************************************************************ */

Simulator::~Simulator()
{
    stop();
}

/* ************************************************************************ */

void Simulator::start()
{
    m_isRunning = true;
    m_thread = std::thread([this] {
        while (m_isRunning)
        {
            step();
        }
    });
}

/* ************************************************************************ */

void Simulator::stop()
{
    m_isRunning = false;
    m_thread.join();
}

/* ************************************************************************ */

void Simulator::step()
{
    assert(m_world);
    m_world->update();
}

/* ************************************************************************ */

}

/* ************************************************************************ */
