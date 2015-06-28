/* ************************************************************************ */
/* Department of Cybernetics                                                */
/* Faculty of Applied Sciences                                              */
/* University of West Bohemia in Pilsen                                     */
/* ************************************************************************ */
/* Author: Jiří Fatka <fatkaj@ntis.zcu.cz>                                  */
/* ************************************************************************ */

// C++
#include <iostream>

// GTest
#include <gtest/gtest.h>

// Module
#include "../ReactionParser.hpp"


/* ************************************************************************ */

/**
 * @brief Compare source code with expected reaction values.
 *
 * @param line  Source test line.
 * @param code  Code to parse.
 * @param names Expected names in code.
 * @param rates Expected rates in code.
 * @param rules Expected rules in code.
 */
static void test_impl(int line, const String& code, std::initializer_list<String> names,
    std::initializer_list<float> rates, std::initializer_list<std::initializer_list<int>> rules)
{
    std::cout << "@" << line << ": `" << code << "`\n";

    // Parse code
    auto reaction = ReactionParser().parseReactionCode(code);

    // Reaction IDs
    ASSERT_EQ(names.size(), reaction.m_ids.size());

    for (auto it = names.begin(); it != names.end(); ++it)
        EXPECT_EQ(*it, reaction.m_ids[it - names.begin()]);

    // Reaction rates
    ASSERT_EQ(rates.size(), reaction.m_rates.size());

    for (auto it = rates.begin(); it != rates.end(); ++it)
        EXPECT_EQ(*it, reaction.m_rates[it - rates.begin()]);

    // Reaction table
    ASSERT_EQ(rules.size(), reaction.m_rules.size());

    for (auto it = rules.begin(); it != rules.end(); ++it)
    {
        const auto& row = reaction.m_rules[it - rules.begin()];
        ASSERT_EQ(it->size(), row.size());

        for (auto it2 = it->begin(); it2 != it->end(); ++it2)
            EXPECT_EQ(*it2, row[it2 - it->begin()]);

    }
}

/* ************************************************************************ */

/**
 * @brief Compare source code with expectation of invalid code.
 *
 * @param line Source test line.
 * @param code Code to parse.
 */
static void test_invalid_impl(int line, const String& code)
{
    std::cout << "@" << line << ": `" << code << "`\n";

    // Parse code
    auto reaction = ReactionParser().parseReactionCode(code);

    // Reaction IDs
    EXPECT_EQ(0, reaction.m_ids.size());

    // Reaction rates
    ASSERT_EQ(0, reaction.m_rates.size());

    // Reaction table
    ASSERT_EQ(0, reaction.m_rules.size());
}

/* ************************************************************************ */

/**
 * @brief Macros that hides passing line number.
 */
#define test(...) test_impl(__LINE__, __VA_ARGS__)
#define test_invalid(...) test_invalid_impl(__LINE__, __VA_ARGS__)

/* ************************************************************************ */

TEST(Parser, empty)
{
    test("", {}, {}, {});
    test(";", {}, {}, {});
}

/* ************************************************************************ */

TEST(Parser, expression)
{
    // Single expression
    test(
        "null > 0.453 > A;",
        {"A"},
        {0.453},
        {{1}}
    );

    // Multiple expressions
    test(
        "null > 0.1 > A;"
        "null > 0.2 > B;",
        {"A", "B"},
        {0.1, 0.2},
        {{+1,  0},
         { 0, +1}}
    );
}

/* ************************************************************************ */

TEST(Parser, extract) // Can't remember the right name
{
    test(
        "A > 0.1 > null;"
        "B0 > 0.3 > null;",
        {"A", "B0"},
        {0.1, 0.3},
        {{-1,  0},
         { 0, -1}}
    );
}

/* ************************************************************************ */

TEST(Parser, assembly)
{
    test(
        "A +B\n> 0.5\t> C;\n"
        "D + A + N32 > 1 > A61",
        {"A", "B", "C", "D", "N32", "A61"},
        {0.5, 1},
        {{-1, -1, +1,  0,  0,  0},
         {-1,  0,  0, -1, -1, +1}}
    );
}

/* ************************************************************************ */

TEST(Parser, disassembly)
{
    test(
        "A > 0.5 > B + XNa0;",
        {"A", "B", "XNa0"},
        {0.5},
        {{-1, +1, +1}}
    );
}

/* ************************************************************************ */

TEST(Parser, invalid)
{
    // Missing semicolon
    // TODO: reaction not stored?
    test_invalid("A > 0.5 > B");

    // Molecule name as reaction rate
    // TODO: variable name?
    test_invalid("A > A > B;");

    // Uncomplete definition
    test_invalid("A");
    test_invalid("A;");
    test_invalid("A\t>;");
    test_invalid("A > B;");
    test_invalid("A > 0.3");
    test_invalid("A > 0.3 >;");
    test_invalid("A > B;");
}

/* ************************************************************************ */

TEST(Parser, code1)
{
    test("A>0.3>B;", {"A", "B"}, {0.3}, {{-1, +1}});
}

/* ************************************************************************ */

TEST(Parser, code2)
{
    test("A > 10 > B;", {"A", "B"}, {10}, {{-1, +1}});
}

/* ************************************************************************ */