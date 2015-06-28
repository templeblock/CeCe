
/* ************************************************************************ */

// Declaration
#include "CellBase.hpp"

// Simulator
#include "core/Units.hpp"
#include "parser/Parser.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

void CellBase::configure(const simulator::Configuration& config,
                         simulator::Simulation& simulation)
{
    Object::configure(config, simulation);

    config.callIfSetString("volume", [this](const std::string& value) {
        setVolume(parser::parse_value<units::Volume>(value));
    });
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */