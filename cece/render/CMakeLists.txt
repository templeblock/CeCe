# ######################################################################### #
# Georgiev Lab (c) 2015-2016                                                #
# ######################################################################### #
# Department of Cybernetics                                                 #
# Faculty of Applied Sciences                                               #
# University of West Bohemia in Pilsen                                      #
# ######################################################################### #
#                                                                           #
# This file is part of CeCe.                                                #
#                                                                           #
# CeCe is free software: you can redistribute it and/or modify              #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation, either version 3 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# CeCe is distributed in the hope that it will be useful,                   #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License         #
# along with CeCe.  If not, see <http://www.gnu.org/licenses/>.             #
#                                                                           #
# ######################################################################### #

if (!CONFIG_RENDER_ENABLE)
    message(WARNING "Cannot build 'render' without CONFIG_RENDER_ENABLE")
endif ()

# ######################################################################### #

# Sources
set(SRCS
    datatype.hpp
    Color.hpp
    Color.cpp
    errors.hpp
    errors.cpp
    Context.hpp
    Context.cpp
    Object.hpp
    Object.cpp
    Camera.hpp
    Camera.cpp
    Buffer.hpp
    Buffer.cpp
    GridBase.hpp
    Grid.hpp
    Grid.cpp
    GridVector.hpp
    GridVector.cpp
    GridColor.hpp
    GridColor.cpp
    GridColorSmooth.hpp
    GridColorSmooth.cpp
    GridColorColorMap.hpp
    GridColorColorMap.cpp
    Shader.hpp
    Shader.cpp
    Program.hpp
    Program.cpp
    VertexElement.hpp
    VertexFormat.hpp
    Texture.hpp
    Texture.cpp
    Circle.hpp
    Circle.cpp
    Rectangle.hpp
    Rectangle.cpp
    ImageData.hpp
    Lines.hpp
    Lines.cpp
    glext.h
    State.hpp
    Image.hpp
    Image.cpp
)

if (DEV_PHYSICS_BUILTIN_DEBUG)
    set(SRCS ${SRCS}
        PhysicsDebugger.hpp
        PhysicsDebugger.cpp
    )
endif ()

# ######################################################################### #

dir_pretend(SOURCES render/ ${SRCS})

set(SOURCES_RENDER ${SOURCES} PARENT_SCOPE)

# ######################################################################### #
