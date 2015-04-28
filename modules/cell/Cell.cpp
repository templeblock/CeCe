
/* ************************************************************************ */

// Declaration
#include "Cell.hpp"

/* ************************************************************************ */

namespace module {
namespace cell {

/* ************************************************************************ */

Cell::Cell(simulator::Simulation& simulation, simulator::Object::Type type) noexcept
    : CellBase(simulation, type)
{
#if ENABLE_PHYSICS
    b2CircleShape shape;
    shape.m_radius = getRadius();

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    getBody()->CreateFixture(&fixtureDef);
#endif
}

/* ************************************************************************ */

void Cell::update(units::Duration dt)
{
    CellBase::update(dt);

#if ENABLE_PHYSICS
    // Update cell shape
    b2CircleShape shape;
    shape.m_radius = getRadius();

    // Update fixture
    b2Fixture* fixture = getBody()->GetFixtureList();
    assert(fixture);
    getBody()->DestroyFixture(fixture);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    getBody()->CreateFixture(&fixtureDef);
#endif
}

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Cell::drawInit(render::Context& context)
{
    m_renderObject.init(context);
}
#endif

/* ************************************************************************ */

#ifdef ENABLE_RENDER
void Cell::draw(render::Context& context)
{
    const auto pos = getPosition();
    const auto radius = calcSphereRadius(getVolume());

    // Transform
    context.matrixPush();
    context.matrixTranslate(pos);
    context.matrixScale(2 * radius);
    context.matrixRotate(getBody()->GetAngle());

    m_renderObject.draw(context, radius);

    context.matrixPop();
}
#endif

/* ************************************************************************ */

}
}

/* ************************************************************************ */