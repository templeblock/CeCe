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
#include "simulator/ProgramContainer.hpp"

// C++
#include <algorithm>

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

/**
 * @brief Find parameter in container.
 *
 * @param data
 *
 * @return
 */
template<typename Container>
auto find(Container& data, const StringView& name) noexcept -> decltype(&(data.begin()->second))
{
    auto it = std::find_if(data.begin(), data.end(),
        [&name](const Pair<String, Program>& p) {
            return p.first == name;
        }
    );

    return it != data.end() ? &(it->second) : nullptr;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

bool ProgramContainer::exists(const StringView& name) const noexcept
{
    return find(m_data, name) != nullptr;
}

/* ************************************************************************ */

ViewPtr<const Program> ProgramContainer::get(const StringView& name) const noexcept
{
    auto ptr = find(m_data, name);

    if (ptr)
        return ptr;

    return nullptr;
}

/* ************************************************************************ */

void ProgramContainer::add(String name, Program program)
{
    auto ptr = find(m_data, name);

    if (ptr)
    {
        *ptr = std::move(program);
    }
    else
    {
        m_data.emplace_back(std::move(name), std::move(program));
    }
}

/* ************************************************************************ */

}

/* ************************************************************************ */
