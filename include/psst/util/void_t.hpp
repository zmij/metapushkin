/**
 * Copyright 2019 Sergei A. Fedorov
 * void_t.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_UTIL_VOID_T_HPP_
#define PSST_UTIL_VOID_T_HPP_

#include <utility>

namespace psst {
namespace util {

#if __cpp_lib_void_t >= 201411 && !defined(__clang__)
template <typename... T>
using void_t = std::void_t<T...>;
#else

template <typename... T>
struct make_void_t {
    using type = void;
};
template <typename... T>
using void_t = typename make_void_t<T...>::type;

#endif

}    // namespace util
}    // namespace psst

#endif /* PSST_UTIL_VOID_T_HPP_ */
