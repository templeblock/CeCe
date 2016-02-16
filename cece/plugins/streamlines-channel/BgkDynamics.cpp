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
#include "cece/plugins/streamlines-channel/BgkDynamics.hpp"

// C++
#include <algorithm>

// Plugin
#include "cece/plugins/streamlines-channel/Utils.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

BgkDynamics::DensityType
BgkDynamics::computeDensity(const DataType& data) const noexcept
{
    using std::begin;
    using std::end;

    const auto fSum = std::accumulate(begin(data), end(data), DensityType(0.0));
    const auto weights = Utils::getWeightHorizontalSum();

    return fSum / weights;
}

/* ************************************************************************ */

BgkDynamics::VelocityType
BgkDynamics::computeVelocity(const DataType& data) const noexcept
{
    const auto omega = getOmega();

    RealType cs2Wall = 0.0;

    // c_s^2 wall is same in all coordinate axes - calculate only for x.
    for (Utils::DirectionType iPop = 0; iPop < Utils::SIZE; ++iPop)
        cs2Wall += Utils::getWeightVertical(iPop) * Utils::DIRECTION_VELOCITIES[iPop][0] * Utils::DIRECTION_VELOCITIES[iPop][0];

    return computeMomentum(data) / (computeDensity(data) * (1.0 - 2.0 * omega / (2 - omega) * cs2Wall * Utils::SPEED_OF_SOUND_SQ_INV));
}

/* ************************************************************************ */

BgkDynamics::DensityType
BgkDynamics::computeEquilibrium(DirectionType iPop, DensityType density,
    VelocityType velocity) const noexcept
{
    // Equilibrium uses modified weights
    return Utils::calcEquilibrium(
        Utils::getWeightHorizontal(iPop),
        Utils::DIRECTION_VELOCITIES[iPop],
        density,
        velocity
    );
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */