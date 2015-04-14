
/* ************************************************************************ */

// Declaration
#include "simulator/SimulationFactory.hpp"

// C++
#include <fstream>

// Simulator
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

std::unique_ptr<Simulation> SimulationFactory::createSimulation() const
{
    return std::unique_ptr<Simulation>(new Simulation());
}

/* ************************************************************************ */

std::unique_ptr<Simulation> SimulationFactory::fromFile(const std::string& filename) const
{
    std::string source;

    {
        std::ifstream file(filename, std::ios::in);

        std::string line;
        while (std::getline(file, line))
        {
            // Read source
            source.append(line);
        }
    }

    return fromSource(source);
}

/* ************************************************************************ */

}

/* ************************************************************************ */