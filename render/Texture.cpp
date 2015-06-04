
/* ************************************************************************ */

// Declaration
#include "render/Texture.hpp"

// C++
#include <cassert>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// OpenGL
#ifdef __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#elif defined(_WIN32)
#include <GL/gl.h>
#include "render/glext.h"
#pragma comment(lib, "opengl32.lib")
#endif

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Texture::Texture(Context& context)
{
    // Generate texture
    gl(glGenTextures(1, &m_id));
    assert(m_id != 0);

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    gl(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}

/* ************************************************************************ */

Texture::Texture(Context& context, core::Vector<SizeType> size, const Color& color)
    : Texture(context)
{
    // Init texture
    resize(std::move(size), color);
}

/* ************************************************************************ */

Texture::~Texture()
{
    assert(isInitialized());
    gl(glDeleteTextures(1, &m_id));
}

/* ************************************************************************ */

void Texture::resize(core::Vector<SizeType> size, const Color& color)
{
    assert(isInitialized());

    const auto width = size.getWidth();
    const auto height = size.getHeight();

    // Create initial buffer
    std::vector<Color> colors(width * height, color);

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_FLOAT, colors.data()));
}

/* ************************************************************************ */

void Texture::update(const core::Vector<SizeType>& size, const Color* colors)
{
    const int width = size.getWidth();
    const int height = size.getHeight();

    gl(glBindTexture(GL_TEXTURE_2D, m_id));
    gl(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, colors));
}

/* ************************************************************************ */

}

/* ************************************************************************ */