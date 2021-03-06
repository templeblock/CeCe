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

# Project
project(cli CXX)

# ######################################################################### #

# Options
if (CONFIG_RENDER_ENABLE)
    option(CONFIG_CLI_ENABLE_IMAGE_CAPTURE "Enable support for image capturing" On)
    option(CONFIG_CLI_ENABLE_VIDEO_CAPTURE "Enable support for video capturing" On)
    option(CONFIG_CLI_ENABLE_RENDER_THREAD "Enable support for rendering thread" Off)
endif ()

# ######################################################################### #

# Set some definitions
add_definitions(
    -DAPP_NAME="${APP_NAME}"
)

if (DEV_PLUGINS)
    # Directory is in build directory
    add_definitions(
        -DDIR_PLUGINS="${CMAKE_BINARY_DIR}/${DIR_PLUGINS}"
    )
endif ()

if (CONFIG_RENDER_ENABLE)
    find_package(GLFW REQUIRED)
    include_directories(${GLFW_INCLUDE_DIR})
endif ()

if (CONFIG_CLI_ENABLE_IMAGE_CAPTURE OR CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
    if (NOT CONFIG_RENDER_ENABLE)
        message(WARNING "Video/image capture requires rendering support")
        set(CONFIG_CLI_ENABLE_IMAGE_CAPTURE Off)
        set(CONFIG_CLI_ENABLE_VIDEO_CAPTURE Off)
    else ()
        message(STATUS "Video/image capture support")

        # LibPNG is required
        find_package(PNG REQUIRED)
        if (WIN32)
            include_directories(${PNG_PNG_INCLUDE_DIR})
        else ()
            include_directories(${PNG_INCLUDE_DIRS})
        endif ()

        if (CONFIG_CLI_ENABLE_IMAGE_CAPTURE)
            add_definitions(-DCONFIG_CLI_ENABLE_IMAGE_CAPTURE=1)
        endif ()

        if (CONFIG_CLI_ENABLE_VIDEO_CAPTURE)
            add_definitions(-DCONFIG_CLI_ENABLE_VIDEO_CAPTURE=1)
        endif ()
    endif ()
endif ()

if (CONFIG_CLI_ENABLE_RENDER_THREAD)
    if (CONFIG_THREAD_SAFE)
        add_definitions(-DCECE_CLI_ENABLE_RENDER_THREAD)
    else ()
        message(WARNING "No thread safe support, disabling CLI rendering thread")
    endif ()
endif ()

# ######################################################################### #

set(CECE_CLI_ENABLE_IMAGE_CAPTURE ${CONFIG_CLI_ENABLE_IMAGE_CAPTURE})
set(CECE_CLI_ENABLE_VIDEO_CAPTURE ${CONFIG_CLI_ENABLE_VIDEO_CAPTURE})

# Sources
set(SRCS
    Arguments.hpp
    Arguments.cpp
    Image.hpp
    Image.cpp
    Simulator.hpp
    Simulator.cpp
    main.cpp
)

# Configure config
configure_file(config.hpp.in config.hpp @ONLY)

# Configure version file
configure_file(version.hpp.in version.hpp @ONLY)
include_directories(${PROJECT_BINARY_DIR})

if (UNIX AND NOT APPLE)
    set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_RPATH}:\$ORIGIN/../lib")
endif ()

if (APPLE)
    set(SRCS ${SRCS} icons/${APP_NAME}.icns)
    set_source_files_properties(icons/${APP_NAME}.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif ()

if (WIN32)
    string(TIMESTAMP BUILD_DATE "%Y-%m-%d" UTC)
    configure_file(version.rc version.rc @ONLY NEWLINE_STYLE WIN32)
    set(SRCS ${SRCS} ${CMAKE_CURRENT_BINARY_DIR}/version.rc)
endif ()

# ######################################################################### #

set(MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}-${PROJECT_NAME}")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2015 Georgiev Lab")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "cz.zcu.ccy.${APP_NAME}")
set(MACOSX_BUNDLE_ICON_FILE "${APP_NAME}.icns")

# ######################################################################### #

# Create executable
add_executable(${PROJECT_NAME}
    ${SRCS}
)

# ######################################################################### #

# Use different name
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${CMAKE_PROJECT_NAME}-${PROJECT_NAME}"
)

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "@loader_path/../lib;@loader_path/../plugins")
endif ()

# ######################################################################### #

# Link libraries
target_link_libraries(${PROJECT_NAME}
    cece
)

if (CONFIG_RENDER_ENABLE)
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARIES}
    )

    if (UNIX AND NOT APPLE)
        # In some cases the GLFW_LIBRARIES variable doesn't contains link to X11
        target_link_libraries(${PROJECT_NAME}
            -lX11
        )
    endif ()
endif ()

if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME}
        -pthread
    )
endif ()

if (CONFIG_CLI_ENABLE_IMAGE_CAPTURE)
    target_link_libraries(${PROJECT_NAME}
        ${PNG_LIBRARIES}
    )
endif ()

# ######################################################################### #

# Install
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION ${INSTALL_DIR_BUNDLE}
    RUNTIME DESTINATION ${INSTALL_DIR_RUNTIME}
    COMPONENT Runtime
)

if (APPLE)
    # Fix library paths
    install(
        CODE "
            execute_process(COMMAND \"${CMAKE_SOURCE_DIR}/scripts/macosx-libraries.sh\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_RUNTIME}/${CMAKE_PROJECT_NAME}-${PROJECT_NAME}\")
            "
        COMPONENT Runtime
    )
endif ()

# ######################################################################### #
