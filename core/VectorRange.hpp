/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

#pragma once

/* ************************************************************************ */

// Simulator
#include "core/compatibility.hpp"
#include "core/Iterator.hpp"
#include "core/Range.hpp"
#include "core/Vector.hpp"

/* ************************************************************************ */

#ifndef _MSC_VER
inline namespace core {
#endif

/* ************************************************************************ */

/**
 * @brief Wrapper for vector iterator.
 */
template<typename T>
struct IteratorVector
{

// Public Types
public:


    /// Iterator traits
    using iterator_category = std::input_iterator_tag;
    using value_type = Vector<T>;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param value Current value.
     * @param max   Maximum value.
     * @param min   Minimum value.
     */
    IteratorVector(Vector<T> value, Vector<T> max, Vector<T> min = Vector<T>::Zero) NOEXCEPT
        : m_value(std::move(value))
        , m_max(std::move(max))
        , m_min(std::move(min))
    {
        // Nothing to do
    }


// Public Operators
public:


    /**
     * @brief Cast to value.
     */
    operator Vector<T>() const NOEXCEPT
    {
        return m_value;
    }


    /**
     * @brief Compares values.
     *
     * @param rhs Second value.
     *
     * @return Are values equal?
     */
    bool operator==(const IteratorVector& rhs) const NOEXCEPT
    {
        return m_value == rhs.m_value;
    }


    /**
     * @brief Compares values.
     *
     * @param rhs Second value.
     *
     * @return Aren't values equal?
     */
    bool operator!=(const IteratorVector& rhs) const NOEXCEPT
    {
        return !operator==(rhs);
    }


    /**
     * @brief Dereference operator.
     *
     * @return A reference to the current token.
     */
    reference operator*() NOEXCEPT
    {
        return m_value;
    }


    /**
     * @brief Dereference operator.
     *
     * @return A pointer to the current token.
     */
    pointer operator->() const NOEXCEPT
    {
        return &m_value;
    }


    /**
     * @brief Increment operator.
     *
     * @return *this.
     */
    IteratorVector& operator++() NOEXCEPT
    {
        if (m_value.x() == m_max.x())
        {
            ++m_value.y();
            m_value.x() = m_min.x();
        }
        else
        {
            ++m_value.x();
        }

        return *this;
    }


    /**
     * @brief Post-increment operator.
     *
     * @note Is deleted (because I'm to lazy to write it).
     */
    IteratorVector operator++(int) = delete;


// Private Data Members
private:

    /// Current value.
    Vector<T> m_value;

    /// Max value.
    Vector<T> m_max;

    /// Min value.
    Vector<T> m_min;

};

/* ************************************************************************ */

/**
 * @brief Make Vector range.
 *
 * @tparam T Vector element type.
 *
 * @param begin Begin vector.
 * @param end   End vector.
 *
 * @return
 */
template<typename T>
CONSTEXPR IteratorRange<IteratorVector<T>> range(Vector<T> begin, Vector<T> end) NOEXCEPT
{
    return makeRange(
        IteratorVector<T>{begin, end - T{1}, begin},
        IteratorVector<T>{end - T{1}, end - T{1}, begin}
    );
}

/* ************************************************************************ */

/**
 * @brief Make Vector range.
 *
 * @tparam T Vector element type.
 *
 * @param end End vector.
 *
 * @return
 */
template<typename T>
CONSTEXPR IteratorRange<IteratorVector<T>> range(Vector<T> end) NOEXCEPT
{
    return range(Vector<T>(Vector<T>::Zero), end);
}

/* ************************************************************************ */

#ifndef _MSC_VER
}
#endif

/* ************************************************************************ */