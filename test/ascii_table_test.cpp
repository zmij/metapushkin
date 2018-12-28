/*
 * ascii_table_test.cpp
 *
 *  Created on: Dec 9, 2018
 *      Author: ser-fedorov
 */

#include "ascii_table.hpp"

#include <gtest/gtest.h>

namespace psst::meta::test {

template <char Begin, char End, char_class Class>
using char_range_e = range::value_range_exclusive<char, char_class, Begin, End, Class>;

//@{
// for usage in metashell and static_asserts
using max_char_range = range::max_value_range<char, char_class, char_class::extended>;
// clang-format off
using ctl_chars      = char_range<  0   , 31    , char_class::control       >;
using wsp_chars      = char_range<  '\t', '\r'  , char_class::whitespace    >;
using spc_char       = single_char< ' '         , char_class::whitespace    >;
using pt1_chars      = char_range<  33  , 47    , char_class::punctuation   >;
using num_chars      = char_range<  '0' , '9'   , char_class::dec_digit     >;
using del_char       = single_char< 127         , char_class::control       >;
// clang-format on

using test_char_range = char_range_e<0, 127, char_class::extended>;
//@}

static_assert((!range::end_compare_v<max_char_range, del_char>), "Ends must be equal");
static_assert((!range::end_compare_v<del_char, max_char_range>), "Ends must be equal");
static_assert((range::end_compare_v<test_char_range, max_char_range>),
              "Exclusive end must me before the inclusive ones");
static_assert((!range::end_compare_v<max_char_range, test_char_range>),
              "Exclusive end must me before the inclusive ones");

static_assert(is_alpha('a'), "");
static_assert(is_alpha('Z'), "");
static_assert(!is_alpha('0'), "");

static_assert(is_digit('0'), "");
static_assert(is_digit('9'), "");
static_assert(!is_digit('a'), "");

static_assert(is_oct_digit('0'), "");
static_assert(is_oct_digit('7'), "");
static_assert(!is_oct_digit('8'), "");

static_assert(is_hex_digit('0'), "");
static_assert(is_hex_digit('a'), "");
static_assert(is_hex_digit('F'), "");
static_assert(!is_hex_digit('z'), "");

static_assert(is_punct('@'), "");
static_assert(is_punct('~'), "");

}    // namespace psst::meta::test
