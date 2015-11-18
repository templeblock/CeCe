/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "cece/core/ViewPtr.hpp"
#include "cece/simulator/Module.hpp"

// Plugin
#include "Module.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines {

/* ************************************************************************ */

/**
 * @brief Streamlines store state module.
 */
class StoreState : public simulator::Module
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param module A pointer to streamlines module.
     */
    explicit StoreState(ViewPtr<plugin::streamlines::Module> module) noexcept
        : m_streamlinesModule(module)
    {
        // Nothing to do
    }


// Public Operations
public:


    /**
     * @brief Update module state.
     *
     * @param simulation Simulation object.
     * @param dt         Simulation time step.
     */
    void update(simulator::Simulation& simulation, units::Time dt) override;


// Private Data Members
private:

    /// A pointer to streamlines module.
    ViewPtr<plugin::streamlines::Module> m_streamlinesModule;

};

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */