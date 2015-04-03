
/* ************************************************************************ */

// Declaration
#include "render/Program.hpp"

// C++
#include <cassert>
#include <vector>
#include <string>
#include <stdexcept>

// Simulator
#include "render/errors.hpp"

/* ************************************************************************ */

namespace render {

/* ************************************************************************ */

Program::Program(const Shader& vs, const Shader& fs) noexcept
{
    m_id = glCreateProgram();
    assert(m_id);

    gl(glAttachShader(m_id, vs.getId()));
    gl(glAttachShader(m_id, fs.getId()));

    gl(glLinkProgram(m_id));

    GLint isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int *)&isLinked);
    if(isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> err(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &err[0]);
        throw std::runtime_error(std::string(err.begin(), err.end()));
    }

    glDetachShader(m_id, vs.getId());
    glDetachShader(m_id, fs.getId());
}

/* ************************************************************************ */

Program::~Program()
{
    gl(glDeleteProgram(m_id));
}

/* ************************************************************************ */

}

/* ************************************************************************ */