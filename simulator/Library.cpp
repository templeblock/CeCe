
// Declaration
#include "Library.hpp"

// C++
#include <map>
#include <cstdint>
#include <cassert>

#if __linux__
// Linux
#include <dlfcn.h>
#elif __WIN32__
#include <windows.h>
#else
#error Unsupported platform
#endif

// Simulator
#include "Module.hpp"

/* ************************************************************************ */

namespace {

/* ************************************************************************ */

#if __linux__
const std::string g_prefix = "libmodule-";
#elif __WIN32__
const std::string g_prefix = "libmodule-";
#endif

/* ************************************************************************ */

#if __linux__
const std::string g_extension = ".so";
#elif __WIN32__
const std::string g_extension = ".dll";
#endif

/* ************************************************************************ */

}

/* ************************************************************************ */

namespace simulator {

/* ************************************************************************ */

struct Library::Impl
{
#if __linux__
    void* lib;
#elif __WIN32__
    HMODULE* lib;
#endif

    /// Error string
    std::string error;


    /**
     * @brief Returns if library is loaded.
     *
     * @return
     */
    bool isLoaded() const noexcept
    {
#if __linux__
        return lib != nullptr;
#elif __WIN32__
        return lib != nullptr;
#endif
    }

    /**
     * @brief Returns address of required symbol.
     *
     * @param name
     */
    void* getAddr(const char* name) const noexcept
    {
#if __linux__
        return dlsym(lib, name);
#elif __WIN32__
        return GetProcAddress(lib, name);
#endif
    }


    /**
     * @brief Returns address of required symbol.
     *
     * @param name
     */
    template<typename T>
    T getAddr(const char* name) const noexcept
    {
        return reinterpret_cast<T>(reinterpret_cast<std::intptr_t>(getAddr(name)));
    }
};

/* ************************************************************************ */

Library::Library(const std::string& name)
    : m_impl{new Impl{nullptr}}
    , m_createModule(nullptr)
{
    const std::string filename = g_prefix + name + g_extension;
#if __linux__
    m_impl->lib = dlopen(filename.c_str(), RTLD_LAZY);
    if (!m_impl->lib) m_impl->error = dlerror();
#elif __WIN32__
    m_impl->lib = LoadLibrary(filename.c_str());
#endif

    // Set create function
    if (m_impl->isLoaded())
        m_createModule = m_impl->getAddr<CreateModule>("create_module");
}

/* ************************************************************************ */

Library::~Library()
{
#if __linux__
    dlclose(m_impl->lib);
#elif __WIN32__
    FreeLibrary(m_impl->lib);
#endif
}

/* ************************************************************************ */

bool Library::isLoaded() const noexcept
{
    return m_impl->isLoaded();
}

/* ************************************************************************ */

const std::string& Library::getError() const noexcept
{
    return m_impl->error;
}

/* ************************************************************************ */

std::unique_ptr<Module> Library::createModule(Simulation* simulation, const std::string& name)
{
    assert(m_createModule);
    return std::unique_ptr<Module>{m_createModule(simulation, name.c_str())};
}

/* ************************************************************************ */

}

/* ************************************************************************ */