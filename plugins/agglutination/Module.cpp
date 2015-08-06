/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Vaclav Pelisek <pelisekv@ntis.zcu.cz>                            */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "Module.hpp"

// C++
#include <random>
#include <algorithm>

// Simulator
#include "core/Log.hpp"
#include "core/constants.hpp"
#include "core/TimeMeasurement.hpp"
#include "simulator/Simulation.hpp"

/* ************************************************************************ */

namespace plugin
{
namespace agglutination
{

/* ************************************************************************ */

RealType getRelativeReceptorProportion(
    RealType radius,
    RealType recSize,
    unsigned int numberOfRec)
{
    return numberOfRec * recSize / (4 * core::constants::PI * radius * radius);
}

/* ************************************************************************ */

RealType getAssociationPropensity(
    units::Duration step,
    RealType radius1,
    RealType radius2,
    RealType recSize,
    unsigned int numberOfRec1,
    unsigned int numberOfRec2,
    RealType Ka)
{
    return  std::min(getRelativeReceptorProportion(radius1, recSize, numberOfRec1), RealType(1)) *
            std::min(getRelativeReceptorProportion(radius2, recSize, numberOfRec2), RealType(1)) *
            (1 - std::exp(-Ka * step.value()));
}

/* ************************************************************************ */

RealType getDisassociationPropensity(
    units::Duration step,
    RealType Kd)
{
    return 1 - std::exp(-Kd * step.value());
}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    auto _ = measure_time("agglutination", simulator::TimeMeasurementIterationOutput(simulation));
    step = dt;

    // Get physics world
    auto& world = simulation.getWorld();

    // Foreach pending bodies
    for (auto p : m_toJoin) {
        // b2RevoluteJointDef joint;
        b2WeldJointDef joint;
        joint.Initialize(std::get<0>(p), std::get<1>(p), std::get<0>(p)->GetWorldCenter());
        jointUserData* jUserData = new jointUserData();
        jUserData->module = this;
        jUserData->Kd = std::get<2>(p);
        joint.userData = jUserData;
        world.CreateJoint(&joint);
    }

    m_toJoin.clear();

    // Joints to remove
    DynamicArray<b2Joint*> toRemove;
    std::random_device rd;
    std::default_random_engine e1(rd());

    // Foreach active joints
    for (auto joint = world.GetJointList(); joint != nullptr; joint = joint->GetNext()) {
        jointUserData* jUserData = (jointUserData*)joint->GetUserData();
        // Not our joint
        if (jUserData == nullptr)
            continue;
        if (jUserData->guard != '@')
            continue;

        std::bernoulli_distribution dist(
            getDisassociationPropensity(
                step,
                jUserData->Kd
            )
        );

        if (dist(e1)) {
            Log::debug("Released: ", joint->GetBodyA(), ", ", joint->GetBodyB());
            toRemove.push_back(joint);
            delete jUserData;
        }
    }

    // Destroy joints
    for (auto joint : toRemove)
        world.DestroyJoint(joint);
}

/* ************************************************************************ */

void Module::configure(const simulator::Configuration& config, simulator::Simulation& simulation)
{
    simulation.getWorld().SetContactListener(this);

    for (auto&& c_bond : config.getConfigurations("bond"))
    {
        m_bonds.push_back(
            Bond(
                c_bond.get<RealType>("association-constant"),
                c_bond.get<RealType>("disassociation-constant"),
                c_bond.get("ligand"),
                c_bond.get("receptor"),
                c_bond.get<RealType>("receptor-size")
            )
        );
    }
}

/* ************************************************************************ */

void Module::BeginContact(b2Contact* contact)
{
    auto fa = contact->GetFixtureA();
    auto fb = contact->GetFixtureB();
    auto ba = fa->GetBody();
    auto bb = fb->GetBody();
    auto& oa = static_cast<simulator::Object*>(ba->GetUserData())->castThrow<plugin::cell::CellBase>();
    auto& ob = static_cast<simulator::Object*>(bb->GetUserData())->castThrow<plugin::cell::CellBase>();
    auto radius1 = oa.getShapes()[0].getCircle().radius;
    auto radius2 = ob.getShapes()[0].getCircle().radius;

    std::random_device rd;
    std::default_random_engine e1(rd());

    for (unsigned int i = 0; i < m_bonds.size(); i++)
    {
        std::bernoulli_distribution dist(
            getAssociationPropensity(
                step,
                radius1.value(),
                radius2.value(),
                m_bonds[i].receptorSize,
                oa.getMoleculeCount(m_bonds[i].receptor),
                oa.getMoleculeCount(m_bonds[i].ligand),
                m_bonds[i].aConst
            )
        );
        if (dist(e1))
        {
            Log::debug("Joined: ", ba, ", ", bb);
            m_toJoin.emplace_back(ba, bb, m_bonds[i].dConst);
        }
    }
}

/* ************************************************************************ */

void Module::EndContact(b2Contact* contact)
{
    // Nothing to do?
}

/* ************************************************************************ */
}
}

/* ************************************************************************ */
