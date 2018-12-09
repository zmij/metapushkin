/*
 * ascii_table.hpp
 *
 *  Created on: Dec 9, 2018
 *      Author: ser-fedorov
 */

#ifndef TEST_ASCII_TABLE_HPP_
#define TEST_ASCII_TABLE_HPP_

#include <pushkin/meta/static_range.hpp>

namespace psst::meta::test {

enum class char_class {
    none        =   0x00,
    extended    =   0x01,
    control     =   0x02,
    whitespace  =   0x04,
    punctuation =   0x08,
    oct_digit   =   0x10,
    dec_digit   =   0x20,
    hex_digit   =   0x40,
    letter      =   0x80,
    alnum       =   dec_digit | letter,
    printable   =   whitespace | punctuation | alnum
};

constexpr char_class
operator | (char_class lhs, char_class rhs)
{
    return static_cast<char_class>( static_cast<int>(lhs) | static_cast<int>(rhs) );
}

constexpr char_class
operator & (char_class lhs, char_class rhs)
{
    return static_cast<char_class>( static_cast<int>(lhs) & static_cast<int>(rhs) );
}


constexpr bool
any(char_class lhs)
{
    return lhs != char_class::none;
}

template <char Begin, char End, char_class Class>
using char_range = range::value_range_inclusive<char, char_class, Begin, End, Class>;
template <char Begin, char_class Class>
using single_char = char_range<Begin, Begin, Class>;

/**
 * This is just to test the static range_map class
 */
using ascii_table = range::range_map<
    char, char_class, char_class::extended,
    char_range< 0   , 31    , char_class::control                       >,
    char_range< '\t', '\r'  , char_class::whitespace                    >,
    single_char<' '         , char_class::whitespace                    >,
    char_range< 33  , 126   , char_class::punctuation                   >,
    char_range< '0' , '7'   , char_class::oct_digit
                        | char_class::dec_digit | char_class::hex_digit >,
    char_range< '8' , '9'   , char_class::dec_digit                     >,
    char_range< 'A' , 'F'   , char_class::letter | char_class::hex_digit>,
    char_range< 'G' , 'Z'   , char_class::letter                        >,
    char_range< 'a' , 'f'   , char_class::letter | char_class::hex_digit>,
    char_range< 'g' , 'z'   , char_class::letter                        >,
    single_char<127         , char_class::control                       >
>;

static constexpr bool
is_alpha(char c)
{
    return any(ascii_table::get_value(c) & char_class::letter);
}

static constexpr bool
is_digit(char c)
{
    return any(ascii_table::get_value(c) & char_class::dec_digit);
}

static constexpr bool
is_oct_digit(char c)
{
    return any(ascii_table::get_value(c) & char_class::oct_digit);
}

static constexpr bool
is_hex_digit(char c)
{
    return any(ascii_table::get_value(c) & char_class::hex_digit);
}

static constexpr bool
is_alnum(char c)
{
    return any(ascii_table::get_value(c) & char_class::alnum);
}

static constexpr bool
is_punct(char c)
{
    return ascii_table::get_value(c) == char_class::punctuation;
}

}  // namespace psst::meta::test



#endif /* TEST_ASCII_TABLE_HPP_ */
