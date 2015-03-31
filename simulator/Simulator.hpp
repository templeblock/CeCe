
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <atomic>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class World;

/* ************************************************************************ */

/**
 * @brief Simulator class.
 */
class Simulator final
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Simulator();


    /**
     * @brief Destructor.
     */
    ~Simulator();


/// Public Accessors
public:


    /**
     * @brief If simulation is running.
     *
     * @return
     */
    bool isRunning() const noexcept
    {
        return m_isRunning;
    }


    /**
     * @brief Returns current world.
     *
     * @return A pointer to current world or nullptr.
     */
    World* getWorld() const noexcept
    {
        return m_world.get();
    }


public:


    /**
     * @brief Set new current world. Previous world will be deleted.
     *
     * @param world
     */
    void setWorld(std::unique_ptr<World> world) noexcept
    {
        m_world = std::move(world);
    }


public:


    /**
     * @brief Start simulation.
     */
    void start();


    /**
     * @brief Stop simulation.
     */
    void stop();


    /**
     * @brief Perform one simulation step.
     */
    void step();


    /**
     * @brief Reset simulation.
     */
    void reset();


// Data Members
private:

    /// Flag if thread is running
    std::atomic<bool> m_isRunning{false};

    /// Simulated world.
    std::unique_ptr<World> m_world;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
