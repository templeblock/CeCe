# ######################################################################### #
# Georgiev Lab (c) 2015                                                     #
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

# Sources
set(SRCS
    Plugin.cpp
    Types.hpp
    Reaction.hpp
    Reactions.hpp
    Reactions.cpp
    ReactionsParser.hpp
    ReactionsParser.cpp
    Context.hpp
    Function.hpp
    Diffusion.hpp
    Diffusion.cpp
    Functors.hpp
)

# Build plugin
build_plugin(stochastic-reactions
    SOURCES ${SRCS}
    PLUGINS_REQUIRED cell diffusion
)

# ######################################################################### #

# Add tests
build_test(stochastic-reactions
    SOURCES
        test/ReactionsParserTest.cpp
    LIBRARIES
        cece
    PLUGINS_REQUIRED
        stochastic-reactions
        diffusion
        cell
)

# ######################################################################### #
