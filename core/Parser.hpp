/* ************************************************************************ */
/* Georgiev Lab (c) 2015                                                    */
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
#include "core/Log.hpp"
#include "core/Range.hpp"
#include "core/Exception.hpp"

/* ************************************************************************ */

inline namespace core {

/* ************************************************************************ */

/**
 * @brief Define specialized parser exception with custom base type.
 *
 * @param className Name of exception class.
 * @param message   Error message.
 */
#define DEFINE_PARSER_EXCEPTION_BASE(className, baseType, message) \
    class className : public baseType { \
    const char* what() const noexcept override { \
        return message; \
    } \
} \

/* ************************************************************************ */

/**
 * @brief Define specialized parser exception.
 *
 * @param className Name of exception class.
 * @param message   Error message.
 */
#define DEFINE_PARSER_EXCEPTION(className, message) \
    DEFINE_PARSER_EXCEPTION(className, ParserException, message)

/* ************************************************************************ */

/**
 * @brief Base parser exception.
 */
class ParserException: public Exception {};

/* ************************************************************************ */

/**
 * @brief Basic parser class.
 *
 * @tparam Derived    Type of derived parser.
 * @tparam Tokenizer  Type used tokenizer.
 */
template<
    typename Derived,
    typename Tokenizer
>
class BasicParser
{
    friend Derived;

// Public Types
public:


    /// The token type.
    using TokenType = typename Tokenizer::TokenType;

    /// The input iterator type.
    using InputIteratorType = typename Tokenizer::InputIteratorType;


// Public Ctors & Dtors
public:


    /**
     * @brief Constructor.
     *
     * @param range Source range.
     */
    explicit BasicParser(IteratorRange<InputIteratorType> range) noexcept
        : m_tokenizer{std::move(range)}
        , m_range{makeRange(m_tokenizer)}
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param beg Begin iterator.
     * @param end End iterator.
     */
    BasicParser(InputIteratorType beg, InputIteratorType end) noexcept
        : m_tokenizer{beg, end}
        , m_range{makeRange(m_tokenizer)}
    {
        // Nothing to do
    }


    /**
     * @brief Constructor.
     *
     * @param source Source object.
     */
    template<typename Source>
    explicit BasicParser(Source&& source) noexcept
        : m_tokenizer(std::forward<Source>(source))
        , m_range{makeRange(m_tokenizer)}
    {
        // Nothing to do
    }


// Public Accessors
public:


    /**
     * @brief Returns used tokenizer.
     *
     * @return
     */
    Tokenizer& getTokenizer() noexcept
    {
        return m_tokenizer;
    }


    /**
     * @brief Returns used tokenizer.
     *
     * @return
     */
    const Tokenizer& getTokenizer() const noexcept
    {
        return m_tokenizer;
    }


// Protected Accessors
protected:


    /**
     * @brief Check if tokenizer is done.
     *
     * @return Is source empty?
     */
    bool isEof() const noexcept
    {
        return m_range.isEmpty();
    }


    /**
     * @brief Returns current token.
     *
     * @return Current token.
     */
    const TokenType& token() const noexcept
    {
        return m_range.front();
    }


    /**
     * @brief Test if current source value match the given value.
     *
     * This operation requires Value to be comparable with source value_type.
     *
     * @tparam Value Type of tested value.
     *
     * @param val Tested value.
     *
     * @return Result of the test.
     */
    template<typename Value>
    bool is(Value&& val) const noexcept
    {
        return (token() == val);
    }


    /**
     * @brief Test if current token match one of the given values.
     *
     * Its recommended to pass arguments sorted by propability of each value.
     *
     * @tparam Value Type of tested value.
     * @tparam Values Types of tested values.
     *
     * @param value First tested value.
     * @param values Tested values.
     *
     * @return Result of the test.
     */
    template<typename Value, typename... Values>
    bool is(Value&& value, Values&&... values) const noexcept
    {
        // Recursive testing.
        return
            is(std::forward<Value>(value)) ||
            is(std::forward<Values>(values)...)
        ;
    }


    /**
     * @brief Test if current token match to the given value.
     *
     * Difference between `match` function and `is` function is the `match`
     * function consumes input character if the character match requirements.
     * The `is` function doesn't change tokenizer state but the `match`
     * function does.
     *
     * @tparam Values Types of tested values.
     *
     * @param values Tested values.
     *
     * @return Result of the test.
     */
    template<typename... Values>
    bool match(Values&&... values)
    {
        if (is(std::forward<Values>(values)...))
        {
            // Move to the next character
            next();
            return true;
        }

        return false;
    }


    /**
     * @brief Skip given values.
     *
     * @tparam Values Types of tested values.
     *
     * @param values Tested values.
     *
     * @return !isEof().
     */
    template<typename... Values>
    bool skip(Values&&... values) noexcept
    {
        while (!isEof() && is(values...))
            next();

        return !isEof();
    }


    /**
     * @brief Try to find one of the given values.
     *
     * @tparam Values Types of tested values.
     *
     * @param values Tested values.
     *
     * @return !isEof().
     */
    template<typename... Values>
    bool find(Values&&... values) noexcept
    {
        while (!isEof() && !is(values...))
            next();

        return !isEof();
    }


// Protected Operations
protected:


    /**
     * @brief Read next token from input range.
     */
    void next() noexcept
    {
        m_range.advanceBegin();
    }


    /**
     * @brief Check if current value match given value.
     *
     * @tparam Value   Test value type.
     * @tparam Message Error message type.
     *
     * @param val Value tested agains current token.
     *
     * @throw ParserException If value doesn't match current token.
     */
    template<typename Value, typename Message = const char*>
    void require(Value&& val, Message&& msg = "Required value doesn't match")
    {
        if (!is(val))
            fatalError(msg);

        // Next token
        next();
    }


// Protected Operations
protected:


    /**
     * @brief Write note message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     */
    template<typename Message>
    void note(Message&& msg) const noexcept
    {
        Log::info(msg);
    }


    /**
     * @brief Write warning message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     */
    template<typename Message>
    void warning(Message&& msg) const noexcept
    {
        Log::warning(msg);
    }


    /**
     * @brief Write error message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     */
    template<typename Message>
    void error(Message&& msg) const
    {
        Log::error(msg);
    }


    /**
     * @brief Throw fatal error with given message.
     *
     * @tparam Message Written message type.
     *
     * @param msg Message.
     *
     * @throw ParserException
     */
    template<typename Message>
    [[noreturn]] void fatalError(Message&& msg) const
    {
        throw ParserException(msg);
    }


    /**
     * @brief Throw fatal error with given message.
     *
     * @tparam ExceptionType Exception type.
     * @tparam Message       Written message type.
     *
     * @param msg Message.
     *
     * @throw ExceptionType
     */
    template<typename ExceptionType, typename Message>
    [[noreturn]] void fatalError(Message&& msg) const
    {
        throw ExceptionType(msg);
    }


    /**
     * @brief Throw fatal error.
     *
     * @tparam ExceptionType Exception type.
     *
     * @throw ExceptionType
     */
    template<typename ExceptionType>
    [[noreturn]] void fatalError() const
    {
        throw ExceptionType();
    }


// Private Data Members
private:

    /// Source tokenizer
    Tokenizer m_tokenizer;

    /// Tokenizer source range.
    IteratorRange<typename Tokenizer::IteratorType> m_range;

};

/* ************************************************************************ */

}

/* ************************************************************************ */
