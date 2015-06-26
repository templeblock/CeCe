/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Vector.hpp"
#include "core/Units.hpp"
#include "core/Grid.hpp"
#include "core/StaticArray.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#include "render/Object.hpp"
#include "render/GridColor.hpp"
#include "render/GridVector.hpp"
#endif

// Module
#include "Lattice.hpp"

/* ************************************************************************ */

namespace module {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Module for streamlines.
 */
class Module : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     */
    Module();


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
    Lattice& getLattice() NOEXCEPT
    {
        return m_lattice;
    }


    /**
     * @brief Returns grid.
     *
     * @return
     */
    const Lattice& getLattice() const NOEXCEPT
    {
        return m_lattice;
    }


    /**
     * @brief Returns inflow velocity.
     *
     * @return
     */
    units::Velocity getVelocityInflow() const NOEXCEPT
    {
        return m_velocityInflow;
    }


    /**
     * @brief Returns fluid viscosity.
     *
     * @return
     */
    units::Viscosity getViscosity() const NOEXCEPT
    {
        return m_viscosity;
    }


// Public Mutators
public:


    /**
     * @brief Set inflow velocity.
     *
     * @param velocity
     */
    void setVelocityInflow(units::Velocity velocity) NOEXCEPT
    {
        m_velocityInflow = velocity;
    }


    /**
     * @brief Set fluid viscosity.
     *
     * @param viscosity
     */
    void setViscosity(units::Viscosity viscosity) NOEXCEPT
    {
        m_viscosity = viscosity;
    }


// Public Operations
public:


    /**
     * @brief Initialize lattice.
     */
    void init();


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
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


#ifdef ENABLE_RENDER
    /**
     * @brief Render module.
     *
     * @param context
     * @param simulation
     */
    void draw(render::Context& context, const simulator::Simulation& simulation) override;
#endif

// Protected Operations
protected:


    /**
     * @brief Update dynamic obstacle map from dynamic objects.
     *
     * @param simulation
     * @param v_max
     */
    void updateDynamicObstacleMap(const simulator::Simulation& simulation, const VelocityVector& v_max);


    /**
     * @brief Apply streamlines to objects.
     *
     * @param simulation
     * @param v_max
     */
    void applyToObjects(const simulator::Simulation& simulation, const VelocityVector& v_max);


    /**
     * @brief Apply boundary conditions.
     *
     * @param simulation
     */
    void applyBoundaryConditions(const simulator::Simulation& simulation, const VelocityVector& v_max);


// Private Data Members
private:

    /// In-flow velocity.
    units::Velocity m_velocityInflow = units::um_s(10.f);

    /// Fluid viscosity (of Water).
    units::Viscosity m_viscosity = units::g_m_s(0.6531);

    /// Lattice.
    Lattice m_lattice;

#if ENABLE_RENDER && OPT_DRAW_VELOCITY
    /// Rendering grid with filled cells.
    render::ObjectPtr<render::GridColor> m_drawable;
#endif

#if ENABLE_RENDER
    /// Render grid for velocities
    render::ObjectPtr<render::GridVector> m_drawableVector;
#endif

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */