/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// Declaration
#include "core/Vector.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

#ifndef __GNUC__
template class DLL_EXPORT Vector<float>;
#endif

/* ************************************************************************ */

#ifndef __GNUC__
template class DLL_EXPORT Vector<unsigned int>;
#endif

/* ************************************************************************ */

#ifndef __GNUC__
template class DLL_EXPORT Vector<int>;
#endif

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */
