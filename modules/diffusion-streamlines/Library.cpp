
/* ************************************************************************ */

// Declaration
#include "Library.hpp"

// Module
#include "Module.hpp"

/* ************************************************************************ */

DEFINE_LIBRARY_CREATE(name)
{
    return new module::diffusion_streamlines::Module{};
}

/* ************************************************************************ */
