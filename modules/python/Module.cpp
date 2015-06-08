
// Declaration
#include "Module.hpp"

// C++
#include <cassert>
#include <string>

// Simulation
#include "core/Log.hpp"
#include "core/FilePath.hpp"
#include "simulator/Simulation.hpp"
#include "simulator/Configuration.hpp"
#include "simulator/Library.hpp"

// Module
#include "Python.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

/* ************************************************************************ */

namespace module {
namespace python {

/* ************************************************************************ */

Module::Module(const std::string& name)
{
    auto ends_with = [](const std::string& value, const std::string& ending) {
        if (ending.size() > value.size()) return false;
        return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
    };

    // External file
    if (ends_with(name, ".py"))
    {
        std::string foundPath;

        // Foreach possible paths
        for (const auto& p : simulator::Library::getLibraryPaths())
        {
            auto path = p + "/" + name;

            if (fileExists(path))
            {
                foundPath = path;
                break;
            }
        }

        if (foundPath.empty())
        {
            Log::warning("Unable to find: " + name);
        }
        else
        {
            m_source.initFile(name);
        }
    }
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    // Check if configuration contains code
    if (config.hasText())
    {
        if (m_source.isInitialized())
            Log::warning("Overriding external script by internal code");

        // Initialize from source text
        m_source.initSource(config.getText());
    }

    // Get function pointers
    m_configureFn = m_source.getFunction("configure");
    m_updateFn = m_source.getFunction("update");

#if ENABLE_RENDER
    m_drawFn = m_source.getFunction("draw");
#endif

    if (!m_configureFn)
        return;

    // Call configure
    if (!call(m_configureFn, &config))
        throw Exception();
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    if (!m_updateFn)
        return;

    // Call update
    if (!call(m_updateFn, dt, &simulation))
        throw Exception();
}

/* ************************************************************************ */

#if ENABLE_RENDER
void Module::draw(render::Context& context, const simulator::Simulation& simulation)
{
    if (!m_drawFn)
        return;

    // Call draw
    if (!call(m_drawFn, &context, &simulation))
        throw Exception();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */
