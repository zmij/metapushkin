/**
 * Copyright 2019 Sergei A. Fedorov
 * enumerators_test.cpp
 *
 *  Created on: Jan 19, 2019
 *      Author: ser-fedorov
 */

#include <gtest/gtest.h>
#include <psst/util/enum_range.hpp>

namespace test {

enum class rainbow { red, orange, yellow, green, cyan, blue, violet };

// inline constexpr auto enumerators(rainbow)
//{
//    using enum_helper = psst::util::enum_helper<rainbow>;
//    return enum_helper::enumerators{rainbow::red,  rainbow::orange, rainbow::yellow,
//    rainbow::green,
//                                    rainbow::cyan, rainbow::blue,   rainbow::violet};
//}
//
//// clang-format off
// inline constexpr auto literals(rainbow)
//{
//    using enum_helper = psst::util::enum_helper<rainbow>;
//    return enum_helper::literals{
//        {rainbow::red       , "red"     },
//        {rainbow::orange    , "orange"  },
//        {rainbow::yellow    , "yellow"  },
//        {rainbow::green     , "green"   },
//        {rainbow::cyan      , "cyan"    },
//        {rainbow::blue      , "blue"    },
//        {rainbow::violet    , "violet"  }
//    };
//}
//// clang-format on
//
// static_assert(psst::util::detail::has_enumerators_func_v<rainbow>, "");

TEST(Enumerators, HelperFuncs)
{
    ;
}

}    // namespace test
