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

cmake_minimum_required(VERSION 2.8.8)

# ######################################################################### #

# CeCe shared library
project(cece CXX)

# ######################################################################### #

# Options
option(DEV_RENDER_GL_ERRORS "Enable OpenGL errors checking." On)

# ######################################################################### #

# Include Python API
if (UNIX AND NOT APPLE)
    # Link boost dynamically on Linux
    set(Boost_USE_STATIC_LIBS Off)
endif ()

find_package(Boost REQUIRED filesystem system)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})

# ######################################################################### #

# Include OpenGL if rendering is enabled
if (CONFIG_RENDER_ENABLE)
    # OpenGL library is required
    find_package(OpenGL REQUIRED)

    # Include OpenGL path
    include_directories(${OPENGL_INCLUDE_DIR})

    # Enable errors
    if (DEV_RENDER_GL_ERRORS)
        add_definitions(-DDEV_RENDER_GL_ERRORS=1)
    endif ()
endif ()

# ######################################################################### #

# Convert configuration names
set(CECE_ENABLE_RENDER ${CONFIG_RENDER_ENABLE})
set(CECE_ENABLE_BOX2D_PHYSICS ${CONFIG_PHYSICS_BUILTIN})
set(CECE_ENABLE_BOX2D_PHYSICS_DEBUG ${DEV_PHYSICS_BUILTIN_DEBUG})
set(CECE_THREAD_SAFE ${CONFIG_THREAD_SAFE})

# Create config file
configure_file(config.hpp.in config.hpp)

# Exports macro is not propagated to subdirectories
add_definitions(-D${PROJECT_NAME}_EXPORTS)

# ######################################################################### #

# Core files
add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(loader)
add_subdirectory(init)
add_subdirectory(module)
add_subdirectory(object)
add_subdirectory(program)
add_subdirectory(plugin)

# Renderer files
if (CONFIG_RENDER_ENABLE)
    add_subdirectory(render)
endif ()

# Simulation files
add_subdirectory(simulator)

# ######################################################################### #

# Sources
set(SRCS
    ${SOURCES_CORE}
    ${SOURCES_CONFIG}
    ${SOURCES_LOADER}
    ${SOURCES_INIT}
    ${SOURCES_MODULE}
    ${SOURCES_OBJECT}
    ${SOURCES_PROGRAM}
    ${SOURCES_PLUGIN}
    ${SOURCES_SIMULATOR}
)

if (CONFIG_RENDER_ENABLE)
    set(SRCS ${SRCS} ${SOURCES_RENDER})
endif ()

# Create library
add_library(${PROJECT_NAME} SHARED ${SRCS})

# ######################################################################### #

# Generate export header (MSVC)
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME}
    EXPORT_FILE_NAME "export.hpp"
)

# Set framework flag
#set_target_properties(${PROJECT_NAME} PROPERTIES FRAMEWORK 1)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_RPATH On)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${Boost_LIBRARIES}
)

if (CONFIG_RENDER_ENABLE)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif ()

if (CONFIG_PHYSICS_BUILTIN)
    # Link libraries
    target_link_libraries(${PROJECT_NAME} ${BOX2D_LIBRARIES})
endif ()

if (MINGW)
    target_link_libraries(${PROJECT_NAME} Ws2_32)
elseif (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} dl)
endif ()

# ######################################################################### #

# Install
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${INSTALL_DIR_LIBRARY}
    LIBRARY DESTINATION ${INSTALL_DIR_LIBRARY}
    COMPONENT Library
)

# Install header files
#install(DIRECTORY .
#    DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}/${PROJECT_NAME}
#    COMPONENT Headers
#    FILES_MATCHING PATTERN "*.hpp"
#)

# Install configuration file
#install(FILES
#        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/config.hpp
#        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/export.hpp
#    DESTINATION ${INSTALL_DIR_INCLUDE}/${CMAKE_PROJECT_NAME}
#    COMPONENT Headers
#)

if (APPLE)
    # Fix library paths
    install(
        CODE "
            execute_process(COMMAND \"${CMAKE_SOURCE_DIR}/resources/scripts/macosx-libraries.sh\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_LIBRARY}/${CMAKE_SHARED_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}\")
            "
        COMPONENT Runtime
    )
endif ()

if (APPLE)
    get_filename_component(LIBRARY_PATH ${BOX2D_LIBRARIES} REALPATH)

    # Install Box2D library
    install(FILES
        ${LIBRARY_PATH}
        DESTINATION ${INSTALL_DIR_LIBRARY}
    )
endif ()

if (WIN32)
    # Install Box2D library
    install(FILES
        "${BOX2D_LIBRARY_PATH}"
        DESTINATION ${INSTALL_DIR_LIBRARY}
    )
endif ()

# ######################################################################### #
