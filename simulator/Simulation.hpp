
#pragma once

/* ************************************************************************ */

// C++
#include <memory>
#include <vector>
#include <cassert>

// Simulator
#include "core/Units.hpp"
#include "core/Vector.hpp"
#include "simulator/Module.hpp"
#include "simulator/Object.hpp"

#ifdef ENABLE_RENDER
#include "render/Context.hpp"
#endif

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Type for step number.
 */
using StepNumber = unsigned long long;

/* ************************************************************************ */

/**
 * @brief Simulation class.
 */
class Simulation
{


// Public Types
public:


    /// Module container type.
    using ModuleContainer = std::vector<std::unique_ptr<Module>>;

    /// Object container type.
    using ObjectContainer = std::vector<std::unique_ptr<Object>>;


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
     * @brief Returns step number.
     *
     * @return
     */
    StepNumber getStepNumber() const noexcept
    {
        return m_stepNumber;
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
    units::Duration getTimeStep() const noexcept
    {
        return m_timeStep;
    }


    /**
     * @brief Find module by type.
     *
     * @tparam ModuleType Module type
     *
     * @return Pointer to module. If module doesn't exists, nullptr is returned.
     */
    template<typename ModuleType>
    ModuleType* findModule() noexcept
    {
        for (auto& module : getModules())
        {
            if (auto ptr = dynamic_cast<ModuleType*>(module.get()))
                return ptr;
        }

        return nullptr;
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
     * @brief Returns world size.
     *
     * @return
     */
    const Vector<units::Length>& getWorldSize() const noexcept
    {
        return m_worldSize;
    }


// Public Mutators
public:


    /**
     * @brief Set simulation time step.
     *
     * @param dt
     */
    void setTimeStep(units::Duration dt) noexcept
    {
        m_timeStep = dt;
    }


    /**
     * @brief Add new module.
     *
     * @param mod
     *
     * @return A pointer to inserted module.
     */
    template<typename T>
    T* addModule(std::unique_ptr<T> mod)
    {
        assert(mod);
        m_modules.push_back(std::move(mod));
        return reinterpret_cast<T*>(m_modules.back().get());
    }


    /**
     * @brief Create module.
     *
     * @param args...
     *
     * @return A pointer to created module.
     */
    template<typename T, typename... Args>
    T* createModule(Args&&... args)
    {
        return addModule(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
    }


    /**
     * @brief Change world size.
     *
     * @param size
     */
    void setWorldSize(Vector<units::Length> size) noexcept
    {
        m_worldSize = std::move(size);
    }


    /**
     * @brief Change world size.
     *
     * @param width  New width.
     * @param height New height.
     */
    void setWorldSize(units::Length width, units::Length height) noexcept
    {
        setWorldSize({width, height});
    }


    /**
     * @brief Add a new object to the world.
     *
     * @param obj
     */
    template<typename T>
    T* addObject(std::unique_ptr<T> obj)
    {
        assert(obj);
        m_objects.push_back(std::move(obj));
        T* ptr = static_cast<T*>(m_objects.back().get());
#ifdef ENABLE_RENDER
        m_drawInitList.push_back(ptr);
#endif
        return ptr;
    }


    /**
     * @brief Create a new object.
     *
     * @param args...
     *
     * @return
     */
    template<typename T, typename... Args>
    T* createObject(Args&&... args)
    {
        return addObject(std::unique_ptr<T>(new T(std::forward<Args>(args)...)));
    }


// Public Operations
public:


    /**
     * @brief Reset simulation.
     */
    void reset();


    /**
     * @brief Update simulation.
     *
     * @param dt
     */
    void update(units::Duration dt) noexcept;


    /**
     * @brief Update simulation by time step.
     */
    void update()
    {
        update(getTimeStep());
    }


#ifdef ENABLE_RENDER
    /**
     * @brief Initialize simulation for rendering.
     *
     * @param context
     */
    void drawInit(render::Context& context);
#endif


#ifdef ENABLE_RENDER
    /**
     * @brief Render simulation.
     *
     * @param context Rendering context.
     */
    void draw(render::Context& context);
#endif


// Private Data Members
private:

    /// Number of simulation steps.
    StepNumber m_stepNumber = 0;

    /// Simulation step.
    units::Duration m_timeStep;

    /// Simulation modules.
    ModuleContainer m_modules;

    /// World size.
    Vector<units::Length> m_worldSize{units::um(400), units::um(400)};

    /// Simulation objects.
    ObjectContainer m_objects;

#ifdef ENABLE_RENDER
    /// List of objects that requires init.
    std::vector<Object*> m_drawInitList;
#endif
};

/* ************************************************************************ */

}

/* ************************************************************************ */