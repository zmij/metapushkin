/**
 * Copyright 2019 Sergei A. Fedorov
 * nth_constant.hpp
 *
 *  Created on: Jan 11, 2019
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_META_NTH_CONSTANT_HPP_
#define PUSHKIN_META_NTH_CONSTANT_HPP_

#include <utility>

namespace psst {
namespace meta {

//@{
/** @name nth_constant */
template <std::size_t N, typename T>
struct nth_constant;
template <std::size_t N, typename T>
using nth_constant_t = typename nth_constant<N, T>::type;
template <std::size_t N, typename T>
constexpr std::size_t nth_constant_v = nth_constant_t<N, T>::value;

template <typename T, T V, T... Values>
struct nth_constant<0, std::integer_sequence<T, V, Values...>> : std::integral_constant<T, V> {};
template <std::size_t N, typename T, T V, T... Values>
struct nth_constant<N, std::integer_sequence<T, V, Values...>>
    : nth_constant<N - 1, std::integer_sequence<T, Values...>> {};
//@}

}    // namespace meta
}    // namespace psst

#endif /* PUSHKIN_META_NTH_CONSTANT_HPP_ */
