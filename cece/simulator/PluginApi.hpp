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
#include "cece/core/UniquePtr.hpp"
#include "cece/core/String.hpp"
#include "cece/simulator/Program.hpp"
#include "cece/simulator/Module.hpp"
#include "cece/simulator/Object.hpp"
#include "cece/simulator/Simulation.hpp"
#include "cece/simulator/Configuration.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

/**
 * @brief Library API type.
 */
class PluginApi
{

// Public Ctors & Dtors
public:


    /**
     * @brief Destructor.
     */
    virtual ~PluginApi()
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns loader extension.
     *
     * @return Supported extension or empty string if loader is not provided.
     */
    virtual String getLoaderExtension() const noexcept
    {
        return {};
    }


// Public Operations
public:


    /**
     * @brief Create a new simulation from source file.
     *
     * @param filename Path to source file.
     *
     * @return Pointer to created simulation.
     */
    virtual UniquePtr<Simulation> fromFile(const FilePath& filename) const;


    /**
     * @brief Create a new simulation from source code.
     *
     * @param source   String with source.
     * @param filename Path to source file.
     *
     * @return Pointer to created simulation.
     */
    virtual UniquePtr<Simulation> fromSource(const String& source, const FilePath& filename = "<source>") const;


    /**
     * @brief Store simulation into file.
     *
     * @param simulation Source simulation.
     * @param filename   Path to source file.
     */
    virtual void toFile(const Simulation& simulation, const FilePath& filename) const;


    /**
     * @brief Convert simulation into source code.
     *
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    virtual String toSource(const Simulation& simulation, const FilePath& filename = "<source>") const;


    /**
     * @brief Read simulation from input stream.
     *
     * @param is       Source stream.
     * @param filename Source file name.
     *
     * @return Created simulation.
     */
    virtual UniquePtr<simulator::Simulation> fromStream(InStream& is,
        const FilePath& filename = "<stream>") const
    {
        throw RuntimeException("Plugin doesn't offer loader");
    }


    /**
     * @brief Write simulation into output stream.
     *
     * @param os         Output stream.
     * @param simulation Source simulation.
     *
     * @return Source code.
     */
    virtual void toStream(OutStream& os, const Simulation& simulation, const FilePath& filename = "<stream>") const
    {
        throw RuntimeException("Plugin doesn't offer loader");
    }


    /**
     * @brief Init simulation.
     *
     * @param simulation Simulation.
     */
    virtual void initSimulation(Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Finalize simulation.
     *
     * @param simulation Simulation.
     */
    virtual void finalizeSimulation(Simulation& simulation)
    {
        // Nothing to do
    }


    /**
     * @brief Configure plugin.
     *
     * @param simulation Current simulation.
     * @param config     Plugin configuration.
     */
    virtual void configure(Simulation& simulation, const Configuration& config)
    {
        // Nothing to do
    }


    /**
     * @brief Create initializer.
     *
     * @param simulation Simulation for that module is created.
     * @param code       Program code.
     *
     * @return Created initializer.
     */
    virtual Simulation::Initializer createInitializer(Simulation& simulation, String code)
    {
        return {};
    }


    /**
     * @brief Create module from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Module name.
     *
     * @return Created module.
     */
    virtual UniquePtr<Module> createModule(Simulation& simulation, const String& name)
    {
        return nullptr;
    }


    /**
     * @brief Create object from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Object name.
     * @param type       Object type.
     *
     * @return Created object.
     */
    virtual UniquePtr<Object> createObject(Simulation& simulation, const String& name, Object::Type type = Object::Type::Dynamic)
    {
        return nullptr;
    }


    /**
     * @brief Create program from current library.
     *
     * @param simulation Simulation for that module is created.
     * @param name       Program name.
     * @param code       Optional program code.
     *
     * @return Created program.
     */
    virtual Program createProgram(Simulation& simulation, const String& name, String code = {})
    {
        return {};
    }

};

/* ************************************************************************ */

}
}

/* ************************************************************************ */