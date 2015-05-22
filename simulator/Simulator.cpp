
/* ************************************************************************ */

// Declaration
#include "simulator/Simulator.hpp"

// C++
#include <cassert>
#include <thread>
#include <chrono>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

Simulator::~Simulator()
{
    stop();
}

/* ************************************************************************ */

void Simulator::start()
{
    m_isRunning = true;

    while (m_isRunning)
    {
        step();
    }
}

/* ************************************************************************ */

void Simulator::stop()
{
    m_isRunning = false;
}

/* ************************************************************************ */

void Simulator::step()
{
    // When to wake-up
    auto sleep = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(16);

    {
        update(0.016f);
    }

    // Sleep
    std::this_thread::sleep_until(sleep);
}

/* ************************************************************************ */

void Simulator::reset()
{
    assert(m_simulation);
    m_simulation->reset();
}

/* ************************************************************************ */

bool Simulator::update(units::Duration dt)
{
    assert(m_simulation);
    return m_simulation->update(dt);
}

/* ************************************************************************ */

bool Simulator::update()
{
    assert(m_simulation);
    return m_simulation->update();
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Simulator::drawInit()
{
    m_renderContext.init();
}
#endif

/* ************************************************************************ */

#if ENABLE_RENDER
void Simulator::draw()
{
    assert(m_simulation);
    m_simulation->draw(m_renderContext);
}
#endif

/* ************************************************************************ */

#if ENABLE_RENDER
void Simulator::drawFinalize()
{
    // TODO: implement
}
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */
