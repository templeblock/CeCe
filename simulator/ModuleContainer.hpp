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
#include "core/String.hpp"
#include "core/StringView.hpp"
#include "core/UniquePtr.hpp"
#include "core/ViewPtr.hpp"
#include "core/Pair.hpp"
#include "core/DynamicArray.hpp"
#include "simulator/Module.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Container for modules.
 */
class ModuleContainer
{

// Public Types
public:


    /// Data type.
    using DataType = DynamicArray<Pair<String, UniquePtr<Module>>>;

    /// Value type.
    using ValueType = DataType::value_type;


// Public Accessors
public:


    /**
     * @brief Returns if module with given name exists.
     *
     * @param name Module name.
     *
     * @return
     */
    bool exists(const StringView& name) const noexcept;


    /**
     * @brief Returns parameter with given value.
     *
     * @param name Module name.
     *
     * @return Pointer to module. Can be nullptr.
     */
    ViewPtr<Module> get(const StringView& name) const noexcept;


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DataType::iterator begin() noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DataType::const_iterator begin() const noexcept
    {
        return m_data.begin();
    }


    /**
     * @brief Returns begin iterator.
     *
     * @return
     */
    DataType::const_iterator cbegin() const noexcept
    {
        return m_data.cbegin();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DataType::iterator end() noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DataType::const_iterator end() const noexcept
    {
        return m_data.end();
    }


    /**
     * @brief Returns end iterator.
     *
     * @return
     */
    DataType::const_iterator cend() const noexcept
    {
        return m_data.cend();
    }


// Public Mutators
public:


    /**
     * @brief Store or replace module.
     *
     * @param name   Module name.
     * @param module Module object.
     *
     * @return Pointer to added module.
     */
    ViewPtr<Module> add(String name, UniquePtr<Module> module);


// Private Data Members
private:

    /// Data.
    DataType m_data;

};

/* ************************************************************************ */

}

/* ************************************************************************ */