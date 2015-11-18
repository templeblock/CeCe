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

// Declaration
#include "cece/plugins/cell/StoreMolecules.hpp"

// CeCe
#include "cece/core/DataTable.hpp"

// Plugin
#include "cece/plugins/cell/CellBase.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

void StoreMolecules::operator()(simulator::Object& object, simulator::Simulation& simulation, units::Duration)
{
    // Cast to cell
    auto& cell = object.castThrow<CellBase>("Cell object required");

    // Get data table
    auto& table = simulation.getDataTable("molecules");

    // Create new row
    // iteration;totalTime;id;molecules...
    table.addRow(
        makePair("iteration", simulation.getIteration()),
        makePair("totalTime", simulation.getTotalTime().value()),
        makePair("id", object.getId()),
        cell.getMolecules()
    );
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */