/*
 * min_max.hpp
 *
 *  Created on: Dec 9, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_META_MIN_MAX_HPP_
#define PUSHKIN_META_MIN_MAX_HPP_

#include <type_traits>

namespace psst::meta {

    template <typename T, T ... Values>
    struct min_value;
    template <typename T, T ... Values>
    using min_value_t = typename min_value<T, Values...>::type;
    template <typename T, T...Values>
    constexpr T min_value_v = min_value_t<T, Values...>::value;

    template <typename T, T A, T B>
    struct min_value<T, A, B> : std::integral_constant<T, (A < B ? A : B) > {};
    template <typename T, T V>
    struct min_value<T, V> : std::integral_constant<T, V> {};
    template <typename T, T A, T... V>
    struct min_value<T, A, V...> : min_value<T, A, min_value<T, V...>::value> {};

    template <typename T, T ... Values>
    struct max_value;
    template <typename T, T ... Values>
    using max_value_t = typename max_value<T, Values...>::type;
    template <typename T, T...Values>
    constexpr T max_value_v = max_value_t<T, Values...>::value;

    template <typename T, T A, T B>
    struct max_value<T, A, B> : std::integral_constant<T, (A > B ? A : B) > {};
    template <typename T, T V>
    struct max_value<T, V> : std::integral_constant<T, V> {};
    template <typename T, T A, T... V>
    struct max_value<T, A, V...> : max_value<T, A, max_value<T, V...>::value> {};

}  // namespace psst::meta


#endif /* PUSHKIN_META_MIN_MAX_HPP_ */
