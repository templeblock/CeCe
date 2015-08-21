/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// C++
#include <cassert>
#include <type_traits>

// Simulator
#include "core/Vector.hpp"
#include "core/Units.hpp"
#include "simulator/Shape.hpp"

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

/**
 * @brief Map shape to grid.
 *
 * @tparam FnIn
 * @tparam FnOut
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param fnIn
 * @param fnOut
 * @param shape  Shape
 * @param center Center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param min    Minimum coordinates (default is {0, 0}).
 */
template<typename FnIn, typename FnOut, typename T, typename StepT>
void mapShapeToGrid(FnIn fnIn, FnOut fnOut, const Shape& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = {})
{
    switch (shape.getType())
    {
    default:
        break;

    case ShapeType::Circle:
        mapShapeToGrid(fnIn, fnOut, shape.getCircle(), steps, center, rotation, max, min);
        break;

    case ShapeType::Rectangle:
        mapShapeToGrid(fnIn, fnOut, shape.getRectangle(), steps, center, rotation, max, min);
        break;

    case ShapeType::Edges:
        mapShapeToGrid(fnIn, fnOut, shape.getEdges(), steps, center, rotation, max, min);
        break;
    }
}

/* ************************************************************************ */

/**
 * @brief Map shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Shape
 * @param center Center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT>
OutIt mapShapeToGrid(OutIt out, const Shape& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = {})
{
    mapShapeToGrid(
        [&out] (Vector<T>&& coord) { *out++ = coord; },
        [] (Vector<T>&& coord) {},
        shape, steps, center, rotation, max, min
    );

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map circle shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param min    Minimum coordinates (default is {0, 0}).
 */
template<typename FnIn, typename FnOut, typename T, typename StepT>
void mapShapeToGrid(FnIn fnIn, FnOut fnOut, const ShapeCircle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = {})
{
    // Get signed type
    using Ts = typename std::make_signed<T>::type;

    // Radius steps in grid
    const auto radiusSteps = Vector<float>(shape.radius / steps);
    const auto shapeCenter = Vector<Ts>(center + shape.center.rotated(rotation) / steps);
    const auto maxS = Vector<Ts>(max);
    const auto minS = Vector<Ts>(min);

    // Foreach grid given by radius steps
    for (Ts x = Ts(-radiusSteps.getX()); x < Ts(radiusSteps.getX()); ++x)
    {
        for (Ts y = Ts(-radiusSteps.getY()); y < Ts(radiusSteps.getY()); ++y)
        {
            // Calculate normalized length for ellipse
            const Vector<float> xyVec{ static_cast<float>(x), static_cast<float>(y) };
            const Vector<float> lenVec = operator/(xyVec, radiusSteps);
            const float len = lenVec.getLengthSquared();

            if (len > 1.f)
                continue;

            // Calculate grid coordinates
            const Vector<Ts> coord = shapeCenter + Vector<Ts>(xyVec);

            // Check if coordinates are in range
            if (coord.inRange(minS, maxS))
                fnIn(Vector<T>(coord));
            else
                fnOut(Vector<T>(coord));
        }
    }
}

/* ************************************************************************ */

/**
 * @brief Map circle shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param min    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT>
OutIt mapShapeToGrid(OutIt out, const ShapeCircle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = {})
{
    mapShapeToGrid(
        [&out](Vector<T>&& coord) { *out++ = coord; },
        [](Vector<T>&& coord) {},
        shape, steps, center, rotation, max, min
    );

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map rectangle shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Rectangle shape
 * @param center Rectangle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename FnIn, typename FnOut, typename T, typename StepT>
void mapShapeToGrid(FnIn fnIn, FnOut fnOut, const ShapeRectangle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = {})
{
    // Get signed type
    using Ts = typename std::make_signed<T>::type;

    // Radius steps in grid
    const auto sizeSteps = Vector<float>(shape.size / steps / 2.f);
    const auto shapeCenter = Vector<Ts>(center + shape.center / steps);
    const auto maxS = Vector<Ts>(max);
    const auto minS = Vector<Ts>(min);

    // Foreach grid given by radius steps
    for (Ts x = Ts(-sizeSteps.getX()); x < Ts(sizeSteps.getX()); ++x)
    {
        for (Ts y = Ts(-sizeSteps.getY()); y < Ts(sizeSteps.getY()); ++y)
        {
            // Calculate grid coordinates
            const auto coord = shapeCenter + Vector<Ts>{x, y};

            // Check if coordinates are in range
            if (coord.inRange(minS, maxS))
                fnIn(Vector<T>(coord));
            else
                fnOut(Vector<T>(coord));
        }
    }
}

/* ************************************************************************ */

/**
 * @brief Map edges shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Edges shape
 * @param steps
 * @param center Edges center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param min    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT>
OutIt mapShapeToGrid(OutIt out, const ShapeEdges& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = Zero)
{
    mapShapeToGrid(
        [&out](Vector<T>&& coord) { *out++ = coord; },
        [](Vector<T>&& coord) {},
        shape, steps, center, rotation, max, min
    );

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map edges shape to grid.
 *
 * @tparam FnIn
 * @tparam FnOut
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param fnIn   Callback function to inner coordinates.
 * @param fnOut  Callback function to outer coordinates.
 * @param shape  Edges shape
 * @param steps  Grid cell size.
 * @param center Edges center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 *
 * @link http://alienryderflex.com/polygon_fill/
 */
template<typename FnIn, typename FnOut, typename T, typename StepT>
void mapShapeToGrid(FnIn fnIn, FnOut fnOut, const ShapeEdges& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = Zero)
{
    // Get signed type
    using Ts = typename std::make_signed<T>::type;

    DynamicArray<Ts> nodes;
    nodes.reserve(10);

    // Alias to edges
    DynamicArray<Vector<RealType>> edges;
    edges.reserve(shape.edges.size());

    for (const auto& edge : shape.edges)
        edges.push_back(center + edge / steps);

    //  Loop through the rows of the image.
    for (auto y = Ts(min.getY()); y < Ts(max.getY()); ++y)
    {
        nodes.clear();

        // Build a list of nodes
        auto j = edges.size() - 1;
        for (std::size_t i = 0u; i < edges.size(); ++i)
        {
            const auto yi = edges[i].getY();
            const auto yj = edges[j].getY();

            if ((yi <  static_cast<RealType>(y) &&
                 yj >= static_cast<RealType>(y)) ||
                (yj <  static_cast<RealType>(y) &&
                 yi >= static_cast<RealType>(y)))
            {
                nodes.push_back(
                    edges[i].getX() + (y - yi)
                    / (yj - yi)
                    * (edges[j].getX() - edges[i].getX())
                );
            }
            j = i;
        }

        // Sort the nodes
        std::sort(nodes.begin(), nodes.end());

        //  Fill the pixels between node pairs.
        for (std::size_t i = 0u; i < nodes.size(); i += 2)
        {
            if (nodes[i] >= static_cast<Ts>(max.getX()))
            {
                fnOut(Vector<T>(nodes[i], y));
            }
            else if (nodes[i + 1] > static_cast<Ts>(min.getX()))
            {
                if (nodes[i] < static_cast<Ts>(min.getX()))
                    nodes[i] = min.getX();

                if (nodes[i + 1] > static_cast<Ts>(max.getX()))
                    nodes[i + 1] = max.getX();

                for (Ts x = nodes[i]; x < nodes[i + 1]; ++x)
                    fnIn(Vector<T>(x, y));
            }
            else
            {
                fnOut(Vector<T>(nodes[i], y));
            }
        }
    }
}

/* ************************************************************************ */

/**
 * @brief Map rectangle shape to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Rectangle shape
 * @param center Rectangle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT>
OutIt mapShapeToGrid(OutIt out, const ShapeRectangle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, units::Angle rotation, const Vector<T>& max,
    const Vector<T>& min = {})
{
    mapShapeToGrid(
        [&out](Vector<T>&& coord) { *out++ = coord; },
        [](Vector<T>&& coord) {},
        shape, steps, center, rotation, max, min
    );

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map shape border to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename FnIn, typename FnOut, typename T, typename StepT, typename OffT = int>
void mapShapeBorderToGrid(FnIn fnIn, FnOut fnOut, const Shape& shape, const Vector<StepT>& steps,
    const Vector<T>& center, const Vector<T>& max, const Vector<T>& min = {},
    OffT off = OffT{})
{
    switch (shape.getType())
    {
    default:
        break;

    case ShapeType::Circle:
        return mapShapeBorderToGrid(fnIn, fnOut, shape.getCircle(), steps, center, max, min, off);
    }
}

/* ************************************************************************ */

/**
 * @brief Map shape border to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 */
template<typename OutIt, typename T, typename StepT, typename OffT = int>
OutIt mapShapeBorderToGrid(OutIt out, const Shape& shape, const Vector<StepT>& steps,
    const Vector<T>& center, const Vector<T>& max, const Vector<T>& min = {},
    OffT off = OffT{})
{
    mapShapeBorderToGrid(
        [&out](Vector<T>&& coord) { *out++ = coord; },
        [](Vector<T>&& coord) {},
        shape, steps, center, max, min, off
    );

    return out;
}

/* ************************************************************************ */

/**
 * @brief Map circle shape border to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 * @tparam OffT  Radius offset type.
 *
 * @param fnIn   Callback function when coordinates is in range.
 * @param fnOut  Callback function when coordinates is out of range.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 * @param off    Radius offset.
 *
 * @see https://web.archive.org/web/20120225095359/http://homepage.smc.edu/kennedy_john/belipse.pdf
 */
template<typename FnIn, typename FnOut, typename T, typename StepT, typename OffT = int>
void mapShapeBorderToGrid(FnIn fnIn, FnOut fnOut, const ShapeCircle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, const Vector<T>& max, const Vector<T>& min = {},
    OffT off = OffT{})
{
    // Get signed type
    using Ts = typename std::make_signed<T>::type;

    // Radius steps in grid
    const auto radius = Vector<T>(shape.radius / steps + off);
    const auto shapeCenter = Vector<Ts>(center + shape.center / steps);
    const auto maxS = Vector<Ts>(max);
    const auto minS = Vector<Ts>(min);

    auto putCoord = [&fnIn, &fnOut, &shapeCenter, &minS, &maxS](Vector<Ts> xy) {
        // Calculate grid coordinates
        const auto coord = shapeCenter + xy;

        // Check if coordinates are in range
        if (coord.inRange(minS, maxS))
            fnIn(Vector<T>(coord));
        else
            fnOut(Vector<T>(coord));
    };

    const auto radiusSquare = radius * radius;
    const auto twoSquare = 2 * radiusSquare;

    Vector<Ts> xy(radius.getX(), 0);
    Vector<Ts> change(
        radiusSquare.getY() * (1 - 2 * radius.getX()),
        radiusSquare.getX()
    );

    Ts error = 0;
    Vector<Ts> stopping(twoSquare.getY() * radius.getX(), 0);

    while (stopping.getX() >= stopping.getY())
    {
        putCoord(xy);
        putCoord(xy * Vector<Ts>(-1,  1));
        putCoord(xy * Vector<Ts>(-1, -1));
        putCoord(xy * Vector<Ts>( 1, -1));

        ++xy.y();
        stopping.y() += twoSquare.getX();
        error += change.getY();
        change.y() += twoSquare.getX();

        if ((2 * error + change.getX()) > 0)
        {
            --xy.x();
            stopping.x() -= twoSquare.getY();
            error += change.getX();
            change.x() += twoSquare.getY();
        }
    }

    xy = Vector<Ts>(0, radius.getY());
    change = Vector<Ts>(
        radiusSquare.getY(),
        radiusSquare.getX() * (1 - 2 * radius.getY())
    );

    error = 0;
    stopping = Vector<Ts>(0, twoSquare.getY() * radius.getY());

    while (stopping.getX() <= stopping.getY())
    {
        putCoord(xy);
        putCoord(xy * Vector<Ts>(-1,  1));
        putCoord(xy * Vector<Ts>(-1, -1));
        putCoord(xy * Vector<Ts>( 1, -1));

        ++xy.x();
        stopping.x() += twoSquare.getY();
        error += change.getX();
        change.x() += twoSquare.getY();

        if ((2 * error + change.getY()) > 0)
        {
            --xy.y();
            stopping.y() -= twoSquare.getX();
            error += change.getY();
            change.y() += twoSquare.getX();
        }
    }
}

/* ************************************************************************ */

/**
 * @brief Map circle shape border to grid.
 *
 * @tparam OutIt Output iterator type.
 * @tparam T     Coordinate value type.
 * @tparam StepT Step type.
 * @tparam OffT  Radius offset type.
 *
 * @param out    Output iterator. It's required to be valid for all incrementations.
 * @param shape  Circle shape
 * @param center Circle center in grid coordinates.
 * @param max    Maximum coordinates.
 * @param mim    Minimum coordinates (default is {0, 0}).
 * @param off    Radius offset.
 *
 * @see https://web.archive.org/web/20120225095359/http://homepage.smc.edu/kennedy_john/belipse.pdf
 */
template<typename OutIt, typename T, typename StepT, typename OffT = int>
OutIt mapShapeBorderToGrid(OutIt out, const ShapeCircle& shape, const Vector<StepT>& steps,
    const Vector<T>& center, const Vector<T>& max, const Vector<T>& min = {},
    OffT off = OffT{})
{
    mapShapeBorderToGrid(
        [&out](Vector<T>&& coord) { *out++ = coord; },
        [](Vector<T>&& coord) {},
        shape, steps, center, max, min, off
    );

    return out;
}

/* ************************************************************************ */

}

/* ************************************************************************ */
