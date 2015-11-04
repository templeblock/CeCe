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

// C++
#include <cassert>
#include <functional>

// Simulator
#include "core/Real.hpp"
#include "core/Units.hpp"
#include "core/UnitsCtors.hpp"
#include "core/Vector.hpp"
#include "core/Log.hpp"
#include "core/String.hpp"
#include "core/Map.hpp"
#include "core/Pair.hpp"
#include "core/UniquePtr.hpp"
#include "core/ViewPtr.hpp"
#include "core/DynamicArray.hpp"
#include "core/TimeMeasurement.hpp"
#include "core/ListenerContainer.hpp"
#include "core/DataTable.hpp"
#include "core/Parameters.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"
#include "simulator/ObjectType.hpp"
#include "simulator/Plugin.hpp"
#include "simulator/Program.hpp"
#include "simulator/ProgramContainer.hpp"
#include "simulator/SimulationListener.hpp"
#include "simulator/ModuleContainer.hpp"
#include "simulator/ObjectContainer.hpp"
#include "simulator/ObjectTypeContainer.hpp"

#if ENABLE_RENDER
#include "core/TriBool.hpp"
#include "render/Context.hpp"
#include "render/Color.hpp"
#include "render/Object.hpp"
#if CONFIG_RENDER_TEXT_ENABLE
#include "render/Font.hpp"
#endif
#endif

#if ENABLE_PHYSICS
// Box2D
#include <Box2D/Box2D.h>
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

class Simulator;
class Configuration;

/* ************************************************************************ */

/**
 * @brief Type for iteration number.
 */
using IterationNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief Type for iteration count.
 */
using IterationCount = IterationNumber;

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class Simulation
    : public ListenerContainer<SimulationListener>
{


// Public Types
public:


    /// Type of simulation parameter value.
    using ParameterValueType = float;

    /// Initialization function.
    using Initializer = std::function<void(Simulation&)>;

    /// Initializer container type.
    using InitializerContainer = DynamicArray<Initializer>;

    /// Data table container type.
    using DataTableContainer = Map<String, DataTable>;


// Public Ctors
public:


    /**
     * @brief Constructor.
     */
    Simulation() noexcept;


    /**
     * @brief Virtual destructor.
     */
    virtual ~Simulation();


// Public Accessors
public:


    /**
     * @brief Returns if simulation is initialized.
     *
     * @return
     */
    bool isInitialized() const noexcept
    {
        return m_initialized;
    }


    /**
     * @brief Returns current iteration.
     *
     * @return
     */
    IterationNumber getIteration() const noexcept
    {
        return m_iteration;
    }


    /**
     * @brief Returns a number of total iterations.
     *
     * @return
     */
    IterationNumber getIterations() const noexcept
    {
        return m_iterations;
    }


    /**
     * @brief Returns if there is unlimited iterations.
     *
     * @return
     */
    bool hasUnlimitedIterations() const noexcept
    {
        return getIterations() == 0;
    }


    /**
     * @brief Return a list of modules.
     *
     * @return
     */
    const ModuleContainer& getModules() const noexcept
    {
        return m_modules;
    }


    /**
     * @brief Returns simulation time step.
     *
     * @return
     */
    units::Time getTimeStep() const noexcept
    {
        return m_timeStep;
    }


    /**
     * @brief Returns total simulation time.
     *
     * @return
     */
    units::Time getTotalTime() const noexcept
    {
        return m_totalTime;
    }


    /**
     * @brief If real-time step is enabled.
     *
     * @return
     */
    bool isTimeStepRealTime() const noexcept
    {
        return m_timeStep == Zero;
    }


    /**
     * @brief Returns gravity vector.
     *
     * @return
     */
    AccelerationVector getGravity() const noexcept;


    /**
     * @brief Returns if plugin with given name is loaded.
     *
     * @param name Plugin name.
     *
     * @return If plugin is loaded.
     */
    bool isPluginLoaded(const String& name) const noexcept
    {
        return m_plugins.find(name) != m_plugins.end();
    }


    /**
     * @brief If simulation uses given module.
     *
     * @param name Module name.
     *
     * @return
     */
    bool hasModule(const StringView& name) const noexcept
    {
        return m_modules.exists(name);
    }


    /**
     * @brief Get module by name.
     *
     * @param name Module name.
     *
     * @return Pointer to module. If module doesn't exists, nullptr is returned.
     */
    ViewPtr<Module> getModule(const StringView& name) noexcept
    {
        return m_modules.get(name);
    }


    /**
     * @brief Find module by type.
     *
     * @tparam ModuleType Module type
     *
     * @param name Module name.
     *
     * @return Pointer to module. If module doesn't exists, nullptr is returned.
     */
    template<typename ModuleType>
    ModuleType* getModule(const String& name) noexcept
    {
        auto module = getModule(name);
        if (!module)
            return nullptr;

        assert(dynamic_cast<ModuleType*>(module));
        return static_cast<ModuleType*>(module);
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    ObjectContainer& getObjects() noexcept
    {
        return m_objects;
    }


    /**
     * @brief Return a list of world objects.
     *
     * @return
     */
    const ObjectContainer& getObjects() const noexcept
    {
        return m_objects;
    }


    /**
     * @brief Return a number of objects.
     *
     * @return
     */
    unsigned long getObjectCount() const noexcept
    {
        return m_objects.getCount();
    }


    /**
     * @brief Return a number of objects by type.
     *
     * @param className Name of required class.
     *
     * @return
     */
    unsigned long getObjectCountType(const String& className) const noexcept
    {
        return m_objects.getCountByType(className);
    }


    /**
     * @brief Returns world size.
     *
     * @return
     */
    const SizeVector& getWorldSize() const noexcept
    {
        return m_worldSize;
    }


    /**
     * @brief Returns simulation parameters.
     *
     * @return
     */
    Parameters& getParameters() noexcept
    {
        return m_parameters;
    }


    /**
     * @brief Returns simulation parameters.
     *
     * @return
     */
    const Parameters& getParameters() const noexcept
    {
        return m_parameters;
    }


    /**
     * @brief Check if simulation parameter exists.
     *
     * @param name Parameter name.
     *
     * @return
     */
    bool hasParameters(const String& name) const noexcept
    {
        return m_parameters.exists(name);
    }


    /**
     * @brief Returns parameter by name.
     *
     * @param name Parameter name.
     *
     * @return Parameter value.
     */
    ParameterValueType getParameter(const String& name) const
    {
        return m_parameters.get(name);
    }


    /**
     * @brief Returns parameter by name.
     *
     * @param name Parameter name.
     * @param def  Default value if parameter is not set.
     *
     * @return Parameter value or default value.
     */
    ParameterValueType getParameter(const String& name, ParameterValueType def) const noexcept
    {
        return m_parameters.get(name, def);
    }


    /**
     * @brief Find object class by name.
     *
     * @param name
     *
     * @return
     */
    ViewPtr<const ObjectType> findObjectType(const StringView& name) const noexcept;


#if ENABLE_PHYSICS
    /**
     * @brief Returns physics world.
     *
     * @return
     */
    b2World& getWorld() noexcept
    {
        return m_world;
    }
#endif


#if ENABLE_PHYSICS
    /**
     * @brief Returns physics world.
     *
     * @return
     */
    const b2World& getWorld() const noexcept
    {
        return m_world;
    }
#endif


#if ENABLE_PHYSICS
    /**
     * @brief Returns physics engine time step.
     *
     * @return
     */
    units::Time getPhysicsEngineTimeStep() const noexcept
    {
        return m_physicsEngineTimeStep;
    }
#endif


#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    /**
     * @brief Returns if physics debug data is shown.
     *
     * @return
     */
    bool isDrawPhysics() const noexcept
    {
        return m_drawPhysics;
    }
#endif


    /**
     * @brief Returns a map of preddefined programs.
     *
     * @return
     */
    const ProgramContainer& getPrograms() const noexcept
    {
        return m_programs;
    }


    /**
     * @brief Check if program with given name exists.
     *
     * @param name Program name.
     *
     * @return
     */
    bool hasProgram(const String& name) const noexcept
    {
        return m_programs.exists(name);
    }


    /**
     * @brief Returns program.
     *
     * @param name Program name.
     *
     * @return Pointer to program.
     */
    Program getProgram(const String& name)
    {
        auto ptr = m_programs.get(name);

        // Try to build program from module
        if (ptr == nullptr)
            return buildProgram(name);

        return *ptr;
    }


    /**
     * @brief Returns data table exists.
     *
     * @param name Data table name.
     *
     * @return
     */
    bool hasDataTable(const StringView& name) const noexcept
    {
        return m_dataTables.find(name.getData()) != m_dataTables.end();
    }


    /**
     * @brief Returns data table with given name.
     *
     * @param name Data table name.
     *
     * @return Data table.
     */
    DataTable& getDataTable(const StringView& name) noexcept
    {
        return m_dataTables[name.getData()];
    }


    /**
     * @brief Returns data table with given name.
     *
     * @param name Data table name.
     *
     * @return Data table.
     */
    const DataTable& getDataTable(const StringView& name) const
    {
        auto it = m_dataTables.find(name.getData());

        if (it == m_dataTables.end())
            throw InvalidArgumentException("Unable to find data table: " + String(name));

        return it->second;
    }


#if ENABLE_RENDER
    /**
     * @brief Get visualize flag.
     *
     * @return
     */
    TriBool getVisualize() const noexcept
    {
        return m_visualize;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Get background color.
     *
     * @return
     */
    const render::Color& getBackgroundColor() const noexcept
    {
        return m_backgroundColor;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Returns font color.
     *
     * return
     */
    const render::Color& getFontColor() const noexcept
    {
        return m_fontColor;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Get font size.
     *
     * @return
     */
    unsigned int getFontSize() const noexcept
    {
        return m_fontSize;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Returns if simulation time should be rendered.
     *
     * @return
     */
    bool isSimulationTimeRender() const noexcept
    {
        return m_simulationTimeRender;
    }
#endif


// Public Mutators
public:


    /**
     * @brief Register initializer.
     *
     * @param init Initializer function.
     */
    void addInitializer(Initializer init)
    {
        m_initializers.push_back(std::move(init));
    }


    /**
     * @brief Set total number of simulation iterations.
     *
     * @param iterations Number of iterations. If value is 0, there is
     *                   unlimited number of iterations.
     */
    void setIterations(IterationNumber iterations) noexcept
    {
        m_iterations = iterations;
    }


    /**
     * @brief Set simulation time step.
     *
     * @param dt Time step.
     */
    void setTimeStep(units::Time dt)
    {
        if (dt == Zero)
            throw InvalidArgumentException("Time step cannot be zero");

        m_timeStep = dt;
    }


    /**
     * @brief Sets simulation parameters.
     *
     * @param parameters
     */
    void setParameters(Parameters parameters) noexcept
    {
        m_parameters = std::move(parameters);
    }


    /**
     * @brief Sets simulation parameter by name.
     *
     * @param name  Parameter name.
     * @param value Parameter value.
     */
    void setParameter(const String& name, ParameterValueType value) noexcept
    {
        m_parameters.set(name, value);
    }


    /**
     * @brief Change simulation world gravity.
     *
     * @param gravity
     */
    void setGravity(const AccelerationVector& gravity) noexcept;


    /**
     * @brief Register object class.
     *
     * @param rec.
     */
    void addObjectType(ObjectType rec)
    {
        m_objectClasses.add(std::move(rec));
    }


    /**
     * @brief Add new module.
     *
     * @param name   Module name.
     * @param module Pointer to module.
     *
     * @return A pointer to inserted module.
     */
    ViewPtr<Module> addModule(String name, UniquePtr<Module> module)
    {
        return m_modules.add(std::move(name), std::move(module));
    }


    /**
     * @brief Add new module.
     *
     * @param name Module name.
     * @param mod  Pointer to module.
     *
     * @return A pointer to inserted module.
     */
    template<typename T>
    ViewPtr<T> addModule(String name, UniquePtr<T> mod)
    {
        return addModule(std::move(name), UniquePtr<Module>(mod));
    }


    /**
     * @brief Create module.
     *
     * @param name Module name.
     * @param args...
     *
     * @return A pointer to created module.
     */
    template<typename T, typename... Args>
    ViewPtr<T> createModule(String name, Args&&... args)
    {
        return addModule(std::move(name), makeUnique<T>(std::forward<Args>(args)...));
    }


    /**
     * @brief Load module from library and add it into simulation.
     *
     * @param path      Path to module.
     * @param storePath Path under what is module stored and can be accessed.
     *                  If is empty, `path` is used. This is useful when module
     *                  extends functionality of another module but the behaviour
     *                  is same as original module.
     *
     * @return A pointer to created module.
     */
    ViewPtr<Module> useModule(const String& path, String storePath = {});


    /**
     * @brief Load module from library and add it into simulation.
     *
     * @tparam ModuleType
     *
     * @param path Path to module.
     *
     * @return A pointer to created module.
     */
    template<typename ModuleType>
    ViewPtr<ModuleType> useModule(const String& path)
    {
        return useModule(path);
    }


    /**
     * @brief Change world size.
     *
     * @param size
     */
    void setWorldSize(SizeVector size) noexcept
    {
        m_worldSize = std::move(size);
    }


    /**
     * @brief Add a new object to the world.
     *
     * @param obj
     */
    template<typename T>
    ViewPtr<T> addObject(UniquePtr<T> obj)
    {
        Assert(obj);
        return m_objects.addObject(std::move(obj));
    }


    /**
     * @brief Create a new object.
     *
     * @param args...
     *
     * @return
     */
    template<typename T, typename... Args>
    ViewPtr<T> createObject(Args&&... args)
    {
        return addObject(makeUnique<T>(*this, std::forward<Args>(args)...));
    }


    /**
     * @brief Build object by name.
     *
     * @param name Object name.
     * @param type Type of created object.
     *
     * @return Created object.
     */
    Object* buildObject(const String& name, Object::Type type = Object::Type::Dynamic);


    /**
     * @brief Add request to object deletion.
     *
     * @param obj Pointer to object.
     *
     * @note Object is not directly deleted, it just queued to be deleted (in update()).
     * This is important in case the object is deleted from within (some program want
     * to delete object) and direct deletion will cause crash.
     */
    void deleteObject(ViewPtr<Object> obj)
    {
        m_objects.deleteObject(obj);
    }


#if ENABLE_PHYSICS
    /**
     * @brief Set physics engine simulation time step.
     *
     * @param dt Time step.
     */
    void setPhysicsEngineTimeStep(units::Time dt) noexcept
    {
        m_physicsEngineTimeStep = dt;
    }
#endif


#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    /**
     * @brief If physics debug data should be shown.
     */
    void setDrawPhysics(bool flag) noexcept
    {
        m_drawPhysics = flag;
    }
#endif


    /**
     * @brief Register new program.
     *
     * @param name    Program name.
     * @param program Added program.
     */
    void addProgram(String name, Program program)
    {
        m_programs.add(std::move(name), std::move(program));
    }


    /**
     * @brief Create a program from module.
     *
     * @param name Program name.
     *
     * @return Created program.
     */
    Program buildProgram(const String& path);


    /**
     * @brief Create new data table.
     *
     * @param name Data table name.
     *
     * @return Created data table.
     */
    DataTable& addDataTable(const StringView& name)
    {
        return m_dataTables[name.getData()];
    }


#if ENABLE_RENDER
    /**
     * @brief Set visualize flag.
     *
     * @param value
     */
    void setVisualize(TriBool value) noexcept
    {
        m_visualize = value;
    }
#endif


#if ENABLE_RENDER
    /**
     * @brief Set background color.
     *
     * @param color
     */
    void setBackgroundColor(render::Color color) noexcept
    {
        m_backgroundColor = color;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Set font color.
     *
     * @param color
     */
    void setFontColor(render::Color color) noexcept
    {
        m_fontColor = color;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Set font size.
     *
     * @param size
     */
    void setFontSize(unsigned int size) noexcept
    {
        m_fontSize = size;
    }
#endif


#if CONFIG_RENDER_TEXT_ENABLE
    /**
     * @brief Set if simulation time should be rendered.
     *
     * @param flag
     */
    void setSimulationTimeRender(bool flag) noexcept
    {
        m_simulationTimeRender = flag;
    }
#endif


// Public Operations
public:


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Initialize simulation.
     */
    void initialize();


    /**
     * @brief Configure simulation.
     *
     * @param config
     */
    void configure(const Configuration& config);


    /**
     * @brief Update simulation.
     *
     * @param dt
     *
     * @return If next step can be calculated.
     */
    bool update(units::Duration dt);


    /**
     * @brief Update simulation by time step.
     *
     * @return If next step can be calculated.
     */
    bool update()
    {
        return update(getTimeStep());
    }


#if ENABLE_RENDER
    /**
     * @brief Render simulation.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context);
#endif


    /**
     * @brief Loads required plugin with given name.
     *
     * @param name Plugin name.
     *
     * @return API to loaded plugin.
     *
     * @throw In case the plugin cannot be loaded.
     */
    ViewPtr<PluginApi> requirePlugin(const String& name);


    /**
     * @brief Loads plugin with given name.
     *
     * In case the library was loaded before, it't not loaded again.
     *
     * @param name Plugin name.
     *
     * @return Pointer to plugin API or nullptr if plugin cannot be loaded.
     *
     * @see requirePlugin
     */
    ViewPtr<PluginApi> loadPlugin(const String& name) noexcept;


    /**
     * @brief Store data tables into files.
     */
    void storeDataTables();


    /**
     * @brief Calculate coefficient used to transform simulation units (time step)
     * to physical engine.
     *
     * @return
     */
    RealType calcPhysicalEngineCoefficient() const noexcept
    {
        return getPhysicsEngineTimeStep() / getTimeStep();
    }


    /**
     * @brief Find object at qiven position.
     *
     * @param position
     *
     * @return
     */
    ViewPtr<Object> query(const PositionVector& position) const noexcept;


// Protected Operations
protected:


    /**
     * @brief Update modules.
     *
     * @param dt Time step.
     */
    void updateModules(units::Time dt);


    /**
     * @brief Update objects.
     *
     * @param dt Time step.
     */
    void updateObjects(units::Time dt);


    /**
     * @brief Detect objects that cannot be visible in the scene and can be
     * safely deleted.
     */
    void detectDeserters();


    /**
     * @brief Delete objects marked for deletion.
     */
    void deleteObjects();


// Private Data Members
private:

    /// If simulation is initialized.
    bool m_initialized = false;

    /// A list of simulation initializers.
    InitializerContainer m_initializers;

    /// Number of simulation steps.
    IterationNumber m_iteration = 0;

    /// Number of iterations.
    IterationNumber m_iterations = 0;

    /// Simulation step.
    units::Time m_timeStep = Zero;

    /// Total simulation time.
    units::Time m_totalTime = Zero;

    /// World size.
    SizeVector m_worldSize{ units::um(400), units::um(400) };

    /// Simulation parameters.
    Parameters m_parameters;

    /// Used plugins.
    Map<String, ViewPtr<PluginApi>> m_plugins;

    /// Simulation modules.
    ModuleContainer m_modules;

#ifdef ENABLE_PHYSICS
    b2World m_world;
#endif

#if ENABLE_PHYSICS
    /// Physics engine time step
    units::Time m_physicsEngineTimeStep = units::s(1.0 / 60.0);
#endif

    /// Simulation objects.
    ObjectContainer m_objects;

    /// A map of preddefined programs.
    ProgramContainer m_programs;

    /// Managed data tables.
    DataTableContainer m_dataTables;

    /// Registered object classes.
    ObjectTypeContainer m_objectClasses;

#if ENABLE_RENDER
    /// Option if visualization should be enabled.
    TriBool m_visualize = Indeterminate;
#endif

#if ENABLE_RENDER
    /// Background (clear) color.
    render::Color m_backgroundColor = render::colors::WHITE;
#endif

#if ENABLE_RENDER && ENABLE_PHYSICS && ENABLE_PHYSICS_DEBUG
    bool m_drawPhysics = false;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// Font renderer.
    render::ObjectPtr<render::Font> m_font;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// Font color.
    render::Color m_fontColor = render::colors::WHITE;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// Font size.
    unsigned int m_fontSize = 30;
#endif

#if CONFIG_RENDER_TEXT_ENABLE
    /// If time should be rendered.
    bool m_simulationTimeRender = false;
#endif
};

/* ************************************************************************ */

/**
 * @brief Time measurement functor with printing current iteration.
 */
struct TimeMeasurementIterationOutput
{
    /// Simulation.
    ViewPtr<const Simulation> m_simulation;


    /**
     * @brief Constructor.
     *
     * @param sim Simulation.
     */
    explicit TimeMeasurementIterationOutput(ViewPtr<const Simulation> sim)
        : m_simulation(sim)
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param sim Simulation
     */
    explicit TimeMeasurementIterationOutput(const Simulation& sim)
        : m_simulation(&sim)
    {
        // Nothing to do
    }


    /**
     * @brief Functor function.
     *
     * @param out  Output stream.
     * @param name Measurement name.
     * @param dt   Measured time.
     */
    void operator()(OutStream& out, const String& name, Clock::duration dt) const noexcept
    {
        using namespace std::chrono;
        out << name << ";" << m_simulation->getIteration() << ";" << duration_cast<microseconds>(dt).count() << "\n";
    }

};

/* ************************************************************************ */

}

/* ************************************************************************ */
