
#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/GridVector.hpp"
#include "render/GridColor.hpp"
#endif

#ifdef ENABLE_PHYSICS
#include "Box2D/Box2D.h"
#endif

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Module for streamlines.
 */
class Module : public simulator::Module
{

// Public Types
public:


    /// Grid type.
    using GridType = core::Grid<VelocityVector>;

    /// Grid size type.
    using SizeType = GridType::SizeType;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module()
        : Module(300)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param size Signal grid size.
     */
    template<typename... Args>
    Module(Args&&... args)
        : m_grid(std::forward<Args>(args)...)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    virtual ~Module();


// Public Accessors
public:


    /**
     * @brief Returns grid.
     *
     * @return
     */
    GridType& getGrid() noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const GridType& getGrid() const noexcept
    {
        return m_grid;
    }


    /**
     * @brief Returns flow speed.
     *
     * @return
     */
    float getFlowSpeed() const noexcept
    {
        return m_flowSpeed;
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    render::GridVector& getRenderObject() noexcept
    {
        return m_renderObject;
    }
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Returns render object.
     *
     * @return
     */
    const render::GridVector& getRenderObject() const noexcept
    {
        return m_renderObject;
    }
#endif


// Public Mutators
public:


    /**
     * @brief Mark state to be updated.
     */
    void markUpdate() noexcept
    {
        m_update = true;
    }


    /**
     * @brief Set flow speed.
     *
     * @param speed.
     */
    void setFlowSpeed(float speed) noexcept
    {
        m_flowSpeed = speed;
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation
     */
    void update(units::Duration dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     */
    void configure(const simulator::ConfigurationBase& config) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize module for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context) override;
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif


// Private Data Members
private:

    /// Flow speed.
    float m_flowSpeed = 50.f;

    /// Velocity grid.
    GridType m_grid;

    /// If streams should be updated
    bool m_update = true;

#ifdef ENABLE_RENDER
    /// Render grid for velocities
    render::GridVector m_renderObject;

    /// Rendering grid with filled cells.
    render::GridColor m_renderObjectColor;
#endif

#ifdef ENABLE_RENDER
    /// If render object should be updated
    bool m_renderUpdate = false;
#endif
};

/* ************************************************************************ */

}
}

/* ************************************************************************ */
