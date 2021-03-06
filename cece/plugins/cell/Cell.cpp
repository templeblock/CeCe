/* ************************************************************************ */
/* Georgiev Lab (c) 2015-2016                                               */
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
#include "cece/plugins/cell/Cell.hpp"

// CeCe
#include "cece/core/Assert.hpp"

/* ************************************************************************ */

namespace cece {
namespace plugin {
namespace cell {

/* ************************************************************************ */

Cell::Cell(simulator::Simulation& simulation, String typeName, object::Object::Type type) noexcept
    : CellBase(simulation, std::move(typeName), type)
{
    setDensity(units::kg(1200) / units::m3(1));

    // Create initial shape
    getMutableShapes().push_back(Shape::makeCircle(getRadius()));

    initShapes();
}

/* ************************************************************************ */

void Cell::update(units::Duration dt)
{
    CellBase::update(dt);

    // Update shape radius
    auto& shapes = getMutableShapes();
    Assert(shapes.size() == 1);
    shapes[0].getCircle().radius = getRadius();

    // Recreate shapes
    initShapes();
}

/* ************************************************************************ */

void Cell::configure(const config::Configuration& config, simulator::Simulation& simulation)
{
    CellBase::configure(config, simulation);
}

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Cell::draw(render::Context& context)
{
    if (!m_renderObject)
        m_renderObject.create(context);

    const RenderState& state = m_drawableState.getFront();

    // Transform
    context.matrixPush();
    context.matrixTranslate(state.position);
    context.matrixScale(2 * state.radius.value());
    context.matrixRotate(state.rotation);
    context.colorPush();
    context.enableAlpha();
    m_renderObject->draw(context, state.radius / units::Length(1), state.color);
    context.disableAlpha();
    context.colorPop();
    context.matrixPop();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Cell::drawStoreState()
{
    RenderState& state = m_drawableState.getBack();

    state.position = getPosition();
    state.radius = calcRadius(getVolume());
    state.rotation = getRotation();
    state.color = calcFluorescentColor();
}
#endif

/* ************************************************************************ */

#ifdef CECE_ENABLE_RENDER
void Cell::drawSwapState()
{
    m_drawableState.swap();
}
#endif

/* ************************************************************************ */

}
}
}

/* ************************************************************************ */
