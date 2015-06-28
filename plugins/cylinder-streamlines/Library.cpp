
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/compatibility.hpp"
#include "simulator/Library.hpp"
#include "simulator/PluginApi.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

class CylinderStreamlinesApi : public simulator::PluginApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) NOEXCEPT override
    {
        return std::unique_ptr<simulator::Module>(new module::cylinder_streamlines::Module{});
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(cylinder_streamlines, CylinderStreamlinesApi)

/* ************************************************************************ */