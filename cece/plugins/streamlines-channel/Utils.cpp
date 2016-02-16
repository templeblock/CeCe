/* ************************************************************************ */
/* Georgiev Lab (c) 2016                                                    */
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
#include "cece/plugins/streamlines-channel/Utils.hpp"

// C++
#include <algorithm>

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace streamlines_channel {

/* ************************************************************************ */

RealType Utils::getWeightHorizontalSum() noexcept
{
    using std::begin;
    using std::end;

    return std::accumulate(begin(s_weightsHorizontal), end(s_weightsHorizontal), RealType(0.0));
}

/* ************************************************************************ */

RealType Utils::getWeightVerticalSum() noexcept
{
    using std::begin;
    using std::end;

    return std::accumulate(begin(s_weightsVertical), end(s_weightsVertical), RealType(0.0));
}

/* ************************************************************************ */

void Utils::initModel(RealType height)
{
    auto fHeight = [] (RealType height) {
        return (1.0 / height);
        //return ((N - a) / (N - 1))
    };

    constexpr StaticArray<RealType, 3> weights2d = {4.0 / 9.0, 1.0 / 9.0, 1.0 / 36.0};
    StaticArray<RealType, 6> weights;

    for (int i = 0; i < 3; ++i)
    {
        weights[i] = weights2d[i] / (1 + 0.5 * fHeight(height));
        weights[i + 3] = 0.25 * fHeight(height) * weights[i];
    }

    s_weightsHorizontal[0] = weights[0];
    s_weightsHorizontal[1] = weights[2];
    s_weightsHorizontal[2] = weights[1];
    s_weightsHorizontal[3] = weights[2];
    s_weightsHorizontal[4] = weights[1];
    s_weightsHorizontal[5] = weights[2];
    s_weightsHorizontal[6] = weights[1];
    s_weightsHorizontal[7] = weights[2];
    s_weightsHorizontal[8] = weights[1];

    s_weightsVertical[0] = weights[3];
    s_weightsVertical[1] = weights[5];
    s_weightsVertical[2] = weights[4];
    s_weightsVertical[3] = weights[5];
    s_weightsVertical[4] = weights[4];
    s_weightsVertical[5] = weights[5];
    s_weightsVertical[6] = weights[4];
    s_weightsVertical[7] = weights[5];
    s_weightsVertical[8] = weights[4];
}

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */