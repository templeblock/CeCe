/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/*                                                                          */
/* This file is part of CeCe.                                               */
/*                                                                          */
/* CeCe is free software: you can redistribute it and/or modify             */
/* it under the terms of the GNU General Public License as published by     */
/* the Free Software Foundation, either version 3 of the License, or        */
/* (at your option) any later version.                                      */
/*                                                                          */
/* CeCe is distributed in the hope that it will be useful,                  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of           */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            */
/* GNU General Public License for more details.                             */
/*                                                                          */
/* You should have received a copy of the GNU General Public License        */
/* along with CeCe.  If not, see <http://www.gnu.org/licenses/>.            */
/*                                                                          */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// CeCe
#include "cece/core/Real.hpp"
#include "cece/core/String.hpp"
#include "cece/core/Units.hpp"
#include "cece/core/ViewPtr.hpp"
#include "cece/core/VectorUnits.hpp"
#include "cece/core/IterationRange.hpp"
#include "cece/module/Module.hpp"

// Plugin
#include "cece/plugins/diffusion/Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace diffusion {

/* ************************************************************************ */

/**
 * @brief Module for diffusion generation.
 */
class Generator : public module::Module
{

// Public Types
public:


    /// Signal production.
    using ProductionRate = units::Divide<units::MolarConcentration, units::Time>::type;


// Public Ctors & Dtors
public:


    using module::Module::Module;


// Public Operations
public:


    /**
     * @brief Load module configuration.
     *
     * @param config Source configuration.
     */
    void loadConfig(const config::Configuration& config) override;


    /**
     * @brief Update module state.
     */
    void update() override;


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
        units::PositionVector position;

        /// Signal production.
        ProductionRate production;

        /// Rectangle size.
        units::PositionVector size;

        /// List of iteration ranges when the generator is active.
        DynamicArray<IterationRange> active;
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
}

/* ************************************************************************ */
