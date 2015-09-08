/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/String.hpp"
#include "core/UniquePtr.hpp"
#include "core/Vector.hpp"
#include "core/StaticArray.hpp"
#include "render/Color.hpp"
#include "render/Buffer.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"
#include "render/Texture.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

class Context;

/* ************************************************************************ */

/**
 * @brief Class storing information about rendered font.
 */
class Font
{

// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param name    Font name.
     */
    Font(Context& context, const String& name);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param data    Font data.
     * @param size    Data size.
     */
    Font(Context& context, const unsigned char* data, unsigned int size);


    /**
     * @brief Constructor.
     *
     * @param context Rendering context.
     * @param data    Font data.
     * @param size    Data size.
     */
    template<std::size_t N>
    Font(Context& context, const StaticArray<unsigned char, N>& data)
        : Font(context, data.data(), N)
    {
        // Nothing to do
    }


    /**
     * @brief Destructor.
     */
    ~Font();


// Public Mutators
public:


    /**
     * @brief Set font size.
     *
     * @param size
     */
    void setSize(unsigned int size) noexcept;


// Public Operators
public:


    /**
     * @brief Render text.
     *
     * @param context Rendering context.
     * @param text    Text to render.
     * @param color   Text color.
     * @parma pos     Text position.
     */
    void draw(Context& context, const String& text, const Color& color, const Vector<float> pos = Zero) noexcept;


// Private Data Members
private:

    /// Implementation.
    struct Impl;
    UniquePtr<Impl> m_impl;

    /// Vertex buffer.
    Buffer m_buffer;

    /// Texture for glyph render.
    Texture m_texture;

    /// Vertex shader.
    Shader m_vertexShader;

    /// Fragment shader.
    Shader m_fragmentShader;

    /// Shader program.
    Program m_program;

    /// Texture pointer.
    Program::UniformId m_texPtr;

    /// Color pointer.
    Program::UniformId m_colorPtr;

};

/* ************************************************************************ */

}

/* ************************************************************************ */