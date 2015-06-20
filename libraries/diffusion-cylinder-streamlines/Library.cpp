
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Simulator
#include "core/compatibility.hpp"
#include "simulator/Library.hpp"
#include "simulator/LibraryApi.hpp"
#include "simulator/Simulation.hpp"

// Module
#include "Module.hpp"

// Modules
#include "../diffusion/Module.hpp"
#include "../cylinder-streamlines/Module.hpp"

/* ************************************************************************ */

class DiffusionCylinderStreamlinesApi : public simulator::LibraryApi
{
    std::unique_ptr<simulator::Module> createModule(simulator::Simulation& simulation, const std::string& name) NOEXCEPT override
    {
        return std::unique_ptr<simulator::Module>(new module::diffusion_cylinder_streamlines::Module{
            simulation.useModule<module::diffusion::Module>("diffusion"),
            simulation.useModule<module::cylinder_streamlines::Module>("cylinder-streamlines")
        });
    }
};

/* ************************************************************************ */

DEFINE_LIBRARY(diffusion_cylinder_streamlines, DiffusionCylinderStreamlinesApi)

/* ************************************************************************ */