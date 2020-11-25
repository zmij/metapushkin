/**
 * Copyright 2019 Sergei A. Fedorov
 * metashell_stuff.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: ser-fedorov
 */

#ifndef TEST_METASHELL_ENUMERATORS_HPP_
#define TEST_METASHELL_ENUMERATORS_HPP_

#include <psst/util/enum_range.hpp>

namespace enums {

enum class rainbow { red, orange, yellow, green, cyan, blue, violet };

}    // namespace enums
namespace lits {

enum class rainbow { red, orange, yellow, green, cyan, blue, violet };

}    // namespace lits

namespace undef {

enum class rainbow { red, orange, yellow, green, cyan, blue, violet };

}    // namespace undef

namespace psst::util {

template <>
struct enumerators<enums::rainbow> : enum_helper<enums::rainbow> {
    static constexpr enum_type values[]{enum_type::red,   enum_type::orange, enum_type::yellow,
                                        enum_type::green, enum_type::cyan,   enum_type::blue,
                                        enum_type::violet};
};

template <>
struct enumerators<lits::rainbow> : enum_helper<lits::rainbow> {
    static constexpr enumerator_literal values[]{
        {enum_type::red, "red"},      {enum_type::orange, "orange"}, {enum_type::yellow, "yellow"},
        {enum_type::green, "green"},  {enum_type::cyan, "cyan"},     {enum_type::blue, "blue"},
        {enum_type::violet, "violet"}};
    ;
};

}    // namespace psst::util

#ifdef __METASHELL
using namespace psst::util;
#endif

#endif /* TEST_METASHELL_ENUMERATORS_HPP_ */
