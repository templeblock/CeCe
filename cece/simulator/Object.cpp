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

// Declaration
#include "cece/simulator/Object.hpp"

// C++
#include <cassert>
#include <string>
#include <sstream>

// CeCe
#include "cece/core/Units.hpp"
#include "cece/core/Log.hpp"
#include "cece/core/Real.hpp"
#include "cece/core/FileStream.hpp"
#include "cece/simulator/Simulation.hpp"

/* ************************************************************************ */

namespace cece {
namespace simulator {

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if ENABLE_PHYSICS
/**
 * @brief Convert object type into Box2 body type.
 *
 * @param type
 *
 * @return
 */
b2BodyType convert(Object::Type type) noexcept
{
    switch (type)
    {
    default:                    return b2_staticBody;
    case Object::Type::Static:  return b2_staticBody;
    case Object::Type::Dynamic: return b2_dynamicBody;
    case Object::Type::Pinned:  return b2_dynamicBody;
    }
}
#endif

/* ************************************************************************ */

/**
 * @brief Split string into multiple strings separated by separator.
 *
 * @param value
 * @param separator
 *
 * @return
 */
DynamicArray<String> split(String value, char separator) noexcept
{
    DynamicArray<String> elems;
    std::istringstream ss(std::move(value));
    String item;

    while (std::getline(ss, item, separator))
    {
        elems.push_back(item);
    }

    return elems;
}

/* ************************************************************************ */

}

/* ************************************************************************ */

Object::IdType s_id = 0;

/* ************************************************************************ */

Object::Object(Simulation& simulation, Type type) noexcept
    : m_simulation(simulation)
    , m_id(++s_id)
    , m_type(type)
{
#if ENABLE_PHYSICS
    auto& world = getSimulation().getWorld();

    b2BodyDef bodyDef;
    bodyDef.type = convert(type);
    bodyDef.userData = this;

    // Create body
    m_body = world.CreateBody(&bodyDef);

    // Pin the body
    if (m_type == Type::Pinned)
    {
        b2BodyDef pivotDef;
        pivotDef.position = m_body->GetWorldCenter();
        m_pinBody = world.CreateBody(&pivotDef);

        b2RevoluteJointDef jointDef;
        jointDef.collideConnected = false;
        jointDef.Initialize(m_pinBody, m_body, m_pinBody->GetPosition());

        m_pinJoint = world.CreateJoint(&jointDef);
    }
#endif
}

/* ************************************************************************ */

Object::~Object()
{
#if ENABLE_PHYSICS
    auto& world = getSimulation().getWorld();

    // Pin the body
    if (m_type == Type::Pinned)
    {
        world.DestroyJoint(m_pinJoint);
        world.DestroyBody(m_pinBody);
    }

    assert(m_body);
    world.DestroyBody(m_body);
#endif
}

/* ************************************************************************ */

PositionVector Object::getPosition() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertPosition(m_body->GetPosition());
#else
    return m_position;
#endif
}

/* ************************************************************************ */

PositionVector Object::getMassCenterPosition() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertPosition(m_body->GetWorldCenter());
#else
    return m_position;
#endif
}

/* ************************************************************************ */

PositionVector Object::getMassCenterOffset() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertPosition(m_body->GetLocalCenter());
#else
    return m_position;
#endif
}

/* ************************************************************************ */

PositionVector Object::getWorldPosition(PositionVector local) const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertPosition(m_body->GetWorldPoint(getConverter().convertPosition(local)));
#else
    return local;
#endif
}

/* ************************************************************************ */

units::Angle Object::getRotation() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertAngle(m_body->GetAngle());
#else
    return m_rotation;
#endif
}

/* ************************************************************************ */

VelocityVector Object::getVelocity() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertLinearVelocity(m_body->GetLinearVelocity());
#else
    return m_velocity;
#endif
}

/* ************************************************************************ */

units::AngularVelocity Object::getAngularVelocity() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertAngularVelocity(m_body->GetAngularVelocity());
#else
    return Zero;
#endif
}

/* ************************************************************************ */

units::Mass Object::getMass() const noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    return getConverter().convertMass(m_body->GetMass());
#else
    return {1};
#endif
}

/* ************************************************************************ */

#if ENABLE_PHYSICS
ConverterBox2D& Object::getConverter() noexcept
{
    return m_simulation.getConverter();
}
#endif

/* ************************************************************************ */

#if ENABLE_PHYSICS
const ConverterBox2D& Object::getConverter() const noexcept
{
    return m_simulation.getConverter();
}
#endif

/* ************************************************************************ */

units::Length Object::getMaxTranslation() const noexcept
{
#if ENABLE_PHYSICS
    return getConverter().getMaxObjectTranslation();
#endif
}

/* ************************************************************************ */

void Object::setType(Type type) noexcept
{
    m_type = type;
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->SetType(convert(type));
#endif
}

/* ************************************************************************ */

void Object::setPosition(PositionVector pos) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->SetTransform(getConverter().convertPosition(pos), m_body->GetAngle());

    if (m_pinBody)
        m_pinBody->SetTransform(getConverter().convertPosition(pos), 0);
#else
    m_position = std::move(pos);
#endif
}

/* ************************************************************************ */

void Object::setRotation(units::Angle angle) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->SetTransform(m_body->GetPosition(), getConverter().convertAngle(angle));
#else
    m_rotation = angle;
#endif
}

/* ************************************************************************ */

void Object::setVelocity(VelocityVector vel) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->SetLinearVelocity(getConverter().convertLinearVelocity(vel));
#else
    m_velocity = std::move(vel);
#endif
}

/* ************************************************************************ */

void Object::setAngularVelocity(units::AngularVelocity vel) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->SetAngularVelocity(getConverter().convertAngularVelocity(vel));
#endif
}

/* ************************************************************************ */

void Object::applyForce(const ForceVector& force) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->ApplyForceToCenter(getConverter().convertForce(force), true);
#endif
}

/* ************************************************************************ */

void Object::applyForce(const ForceVector& force, const PositionVector& offset) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->ApplyForce(
        getConverter().convertForce(force),
        m_body->GetWorldPoint(getConverter().convertPosition(offset)),
        true
    );

    m_force += force;
#endif
}

/* ************************************************************************ */

void Object::applyLinearImpulse(const ImpulseVector& impulse, const PositionVector& offset) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->ApplyLinearImpulse(
        getConverter().convertLinearImpulse(impulse),
        m_body->GetWorldPoint(getConverter().convertPosition(offset)),
        true
    );
#else
    m_velocity += 1 / getMass() * impulse;
#endif
}

/* ************************************************************************ */

void Object::applyAngularImpulse(const units::Impulse& impulse) noexcept
{
#if ENABLE_PHYSICS
    Assert(m_body);
    m_body->ApplyAngularImpulse(getConverter().convertAngularImpulse(impulse), true);
#endif
}

/* ************************************************************************ */

void Object::useProgram(const String& name) noexcept
{
    addProgram(getSimulation().getProgram(name));
}

/* ************************************************************************ */

void Object::destroy()
{
    m_simulation.deleteObject(this);
}

/* ************************************************************************ */

void Object::update(units::Duration dt)
{
#if !ENABLE_PHYSICS
    // Calculate new object position
    setPosition(getPosition() + getVelocity() * dt);
#endif

    // Call object programs
    for (auto& program : getPrograms())
        program(*this, getSimulation(), dt);

    // Store streamlines data
    if (m_dataOut)
    {
        const auto pos = getPosition();
        const auto posMass = getMassCenterPosition();
        const auto vel = getVelocity();
        const auto force = getForce();

        *m_dataOut <<
            // iteration
            m_simulation.getIteration() << ";" <<
            // totalTime
            m_simulation.getTotalTime().value() << ";" <<
            // id
            getId() << ";" <<
            // x
            pos.getX().value() << ";" <<
            // y
            pos.getY().value() << ";" <<
            // massX
            posMass.getX().value() << ";" <<
            // massY
            posMass.getY().value() << ";" <<
            // velX
            vel.getX().value() << ";" <<
            // velY
            vel.getY().value() << ";" <<
            // forceX
            force.getX().value() << ";" <<
            // forceY
            force.getY().value() << ";" <<
            // angle
            getRotation() << ";" <<
            // omega
            getAngularVelocity().value() <<
            "\n"
        ;

        m_dataOut->flush();
    }
}

/* ************************************************************************ */

void Object::configure(const Configuration& config, Simulation& simulation)
{
#if ENABLE_RENDER
    // Set object position
    setVisible(config.get("visible", isVisible()));
#endif

    // Set object position
    setPosition(config.get("position", getPosition()));

    // Set object velocity
    setVelocity(config.get("velocity", getVelocity()));

    // Set object density
    setDensity(config.get("density", getDensity()));

    // Set object programs
    if (config.has("programs"))
    {
        for (const auto& name : split(config.get("programs"), ' '))
        {
            auto program = simulation.getProgram(name);

            if (program)
                addProgram(std::move(program));
            else
                Log::warning("Unable to create program: ", name);
        }
    }

    if (config.has("data-out"))
    {
        m_dataOut = makeUnique<OutFileStream>(config.get("data-out"));
        *m_dataOut << "iteration;totalTime;id;x;y;massX;massY;velX;velY;forceX;forceY;angle;omega\n";
    }
}

/* ************************************************************************ */

}
}

/* ************************************************************************ */
