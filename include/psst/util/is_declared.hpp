/**
 * Copyright 2019 Sergei A. Fedorov
 * is_declared.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_UTIL_IS_DECLARED_HPP_
#define PSST_UTIL_IS_DECLARED_HPP_

#include <cstddef>
#include <type_traits>

namespace psst {
namespace util {

//@{
/** @name is_declared */
namespace detail {

template <typename T, std::size_t = sizeof(T)>
std::true_type
is_declared_impl(T*);

std::false_type
is_declared_impl(...);

}    // namespace detail

/**
 * Trait class to detect if a certain specialisation of a template is available.
 */
template <typename T>
struct is_declared : decltype(detail::is_declared_impl(std::declval<T*>())) {};
template <typename T>
using is_declared_t = typename is_declared<T>::type;
template <typename T>
constexpr bool is_declared_v = is_declared_t<T>::value;
//@}

}    // namespace util
}    // namespace psst

#endif /* PSST_UTIL_IS_DECLARED_HPP_ */
