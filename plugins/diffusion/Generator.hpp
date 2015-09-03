/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/Real.hpp"
#include "core/String.hpp"
#include "core/Units.hpp"
#include "core/ViewPtr.hpp"
#include "simulator/Module.hpp"

// Plugin
#include "Module.hpp"

/* ************************************************************************ */

namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion generation.
 */
class Generator : public simulator::Module
{

// Public Types
public:


    /// Signal production.
    using ProductionRate = units::Divide<units::MolarConcentration, units::Time>::type;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param module A pointer to diffusion module.
     */
    explicit Generator(ViewPtr<plugin::diffusion::Module> module) noexcept
        : m_diffusionModule(module)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param dt         Simulation time step.
     * @param simulation Simulation object.
     */
    void update(units::Time dt, simulator::Simulation& simulation) override;


    /**
     * @brief Configure module.
     *
     * @param config
     * @param simulation
     */
    void configure(const simulator::Configuration& config, simulator::Simulation& simulation) override;


// Private Structures
private:


    /**
     * @brief Diffusion source.
     */
    struct Source
    {
        /// Signal name.
        String name;

        /// Source position.
        PositionVector position;

        /// Signal production.
        ProductionRate production;

        /// Rectangle size.
        PositionVector size;
    };


// Private Data Members
private:


    /// Diffusion sources.
    DynamicArray<Source> m_sources;

    /// A pointer to diffusion module.
    ViewPtr<plugin::diffusion::Module> m_diffusionModule;

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */