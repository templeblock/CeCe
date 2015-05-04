
/* ************************************************************************ */

// Declaration
#include "render/Grid.hpp"

// C++
#include <vector>
#include <cassert>

// OpenGL
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>

// Simulator
#include "render/Context.hpp"
#include "render/errors.hpp" // TODO: remove

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

void Grid::init(Context& context)
{
    m_buffer.init(context);
}

/* ************************************************************************ */

void Grid::draw(Context& context) noexcept
{
    // Set vertex buffer
    context.setVertexBuffer(&m_buffer);

    gl(glEnableClientState(GL_VERTEX_ARRAY));
    gl(glVertexPointer(2, GL_FLOAT, 0, 0));

    auto width = getSize().getWidth();
    auto height = getSize().getHeight();

    // Draw grid
    context.draw(PrimitiveType::Lines, 2 * ((width + 1) + (height + 1)));

    // Disable states
    gl(glDisableClientState(GL_VERTEX_ARRAY));

    // Unbind vertex buffer
    //context.setVertexBuffer(nullptr);
}

/* ************************************************************************ */

void Grid::resize(Vector<PositionType> size) noexcept
{
    GridBase::resize(std::move(size));

    // Get grid size
    const auto width = getSize().getWidth();
    const auto height = getSize().getHeight();

    constexpr Vector<float> start{-0.5f};
    const Vector<float> step = 1.f / getSize();

    struct Vertex { GLfloat x, y; };

    std::vector<Vertex> vertices;
    vertices.reserve((width + 1) * (height + 1));

    // X lines
    for (PositionType i = 0; i <= width; ++i)
    {
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY()});
        vertices.push_back(Vertex{start.getX() + i * step.getX(), start.getY() + 1.f});
    }

    // Y lines
    for (PositionType i = 0; i <= height; ++i)
    {
        vertices.push_back(Vertex{start.getX(), start.getY() + i * step.getY()});
        vertices.push_back(Vertex{start.getX() + 1.f, start.getY() + i * step.getY()});
    }

    // Initialize buffer
    m_buffer.resize(
        vertices.size() * sizeof(decltype(vertices)::value_type),
        vertices.data()
    );
}

/* ************************************************************************ */

}

/* ************************************************************************ */
