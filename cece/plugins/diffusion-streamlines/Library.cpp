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

// CeCe
#include "cece/simulator/Plugin.hpp"
#include "cece/simulator/PluginApi.hpp"
#include "cece/simulator/Simulation.hpp"

// Plugin
#include "cece/plugins/diffusion-streamlines/Module.hpp"

// Plugins
#include "cece/plugins/diffusion/Module.hpp"
#include "cece/plugins/streamlines/Module.hpp"

/* ************************************************************************ */

using namespace cece;
using namespace cece::simulator;

/* ************************************************************************ */

class DiffusionStreamlinesApi : public PluginApi
{
    UniquePtr<Module> createModule(Simulation& simulation, const String& name) noexcept override
    {
        return makeUnique<plugin::diffusion_streamlines::Module>(
            simulation.useModule<plugin::diffusion::Module>("diffusion"),
            simulation.useModule<plugin::streamlines::Module>("streamlines")
        );
    }
};

/* ************************************************************************ */

DEFINE_PLUGIN(diffusion_streamlines, DiffusionStreamlinesApi)

/* ************************************************************************ */