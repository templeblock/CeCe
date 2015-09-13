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

namespace plugin {
namespace agglutination {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

std::random_device g_rd;

/* ************************************************************************ */

std::default_random_engine g_gen(g_rd());

/* ************************************************************************ */

RealType getRelativeReceptorProportion(
    RealType radius,
    unsigned int numberOfRec)
{
    return 1 - std::exp(- numberOfRec / (4 * core::constants::PI * radius * radius));
}

/* ************************************************************************ */

RealType getAssociationPropensity(
    units::Duration step,
    RealType radius1,
    RealType radius2,
    unsigned int numberOfRec1,
    unsigned int numberOfRec2,
    RealType Ka)
{
    return  getRelativeReceptorProportion(radius1, numberOfRec1) *
            getRelativeReceptorProportion(radius2, numberOfRec2) *
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

}

/* ************************************************************************ */

void Module::update(units::Duration dt, simulator::Simulation& simulation)
{
    // Store time step
    m_step = dt;

    auto _ = measure_time("agglutination", simulator::TimeMeasurementIterationOutput(simulation));

    // Get physics world
    auto& world = simulation.getWorld();

    // Foreach pending bodies
    for (const auto& p : m_toJoin)
    {
        b2WeldJointDef joint;
        joint.Initialize(p.bodyA, p.bodyB, p.bodyA->GetWorldCenter());
        JointUserData* jUserData = new JointUserData();
        jUserData->module = this;
        jUserData->Kd = p.dConst;
        joint.userData = jUserData;
        world.CreateJoint(&joint);
    }

    m_toJoin.clear();

    // Joints to remove
    DynamicArray<b2Joint*> toRemove;

    // Foreach active joints
    for (auto joint = world.GetJointList(); joint != nullptr; joint = joint->GetNext())
    {
        const JointUserData* jUserData = reinterpret_cast<const JointUserData*>(joint->GetUserData());
        // Not our joint
        if (jUserData == nullptr)
            continue;
        if (jUserData->guard != '@')
            continue;

        std::bernoulli_distribution dist(
            getDisassociationPropensity(
                m_step,
                jUserData->Kd
            )
        );

        if (dist(g_gen))
        {
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
        m_bonds.push_back(Bond{
            c_bond.get<RealType>("association-constant"),
            c_bond.get<RealType>("disassociation-constant"),
            c_bond.get("ligand"),
            c_bond.get("receptor")
        });
    }
}

/* ************************************************************************ */

void Module::BeginContact(b2Contact* contact)
{
    auto ba = contact->GetFixtureA()->GetBody();
    auto bb = contact->GetFixtureB()->GetBody();
    auto oa = static_cast<simulator::Object*>(ba->GetUserData());
    auto ob = static_cast<simulator::Object*>(bb->GetUserData());
    if (!oa->is<plugin::cell::CellBase>() || !ob->is<plugin::cell::CellBase>())
        return;
    auto& ca = static_cast<simulator::Object*>(ba->GetUserData())->castThrow<plugin::cell::CellBase>();
    auto& cb = static_cast<simulator::Object*>(bb->GetUserData())->castThrow<plugin::cell::CellBase>();
    auto radius1 = ca.getShapes()[0].getCircle().radius;
    auto radius2 = cb.getShapes()[0].getCircle().radius;

    for (unsigned int i = 0; i < m_bonds.size(); i++)
    {
        std::bernoulli_distribution dist1(
            getAssociationPropensity(m_step, radius1.value(), radius2.value(),
                ca.getMoleculeCount(m_bonds[i].receptor), cb.getMoleculeCount(m_bonds[i].ligand),
                m_bonds[i].aConst));
        if (dist1(g_gen))
        {
            Log::debug("Joined: ", ba, ", ", bb);
            m_toJoin.push_back(JointDef{ba, bb, m_bonds[i].dConst});
            continue;
        }
        std::bernoulli_distribution dist2(
            getAssociationPropensity(m_step, radius1.value(), radius2.value(),
                cb.getMoleculeCount(m_bonds[i].receptor), ca.getMoleculeCount(m_bonds[i].ligand),
                m_bonds[i].aConst));
        if (dist2(g_gen))
        {
            Log::debug("Joined: ", ba, ", ", bb);
            m_toJoin.push_back(JointDef{ba, bb, m_bonds[i].dConst});
            continue;
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
