/*
 * integer_sequence.hpp
 *
 *  Created on: Mar 9, 2017
 *      Author: zmij
 */

#ifndef PUSHKIN_META_INTEGER_SEQUENCE_HPP_
#define PUSHKIN_META_INTEGER_SEQUENCE_HPP_

#include <type_traits>
#include <utility>

#include <pushkin/meta/algorithm.hpp>

namespace psst {
namespace meta {

/**
 * Metafunction to join sequences
 */
template < typename ... T >
struct join;

template < typename T >
struct join < T > {
    using type = T;
};

template < typename T, typename U, typename ... V >
struct join< T, U, V... >
    : join< typename join< T, U >::type, V... > {};

#if __cplusplus >= 201402L

template < typename T, T ... A, T... B >
struct join< ::std::integer_sequence<T, A...>, ::std::integer_sequence<T, B...> > {
    using type = ::std::integer_sequence<T, A..., B...>;
};

template <typename T>
struct is_empty<::std::integer_sequence<T>> : ::std::true_type {};
template <typename T, T ... Values>
struct is_empty<::std::integer_sequence<T, Values...>> : ::std::false_type {};

namespace detail {

template < typename T, T Min, T Max, T ... Values >
struct unwrap_range_impl {
    static_assert(Min < Max, "Start of range must be less than ending");
    using type = typename unwrap_range_impl<T, Min, Max - 1, Max, Values ... >::type;
};

template < typename T, T V, T ... Values >
struct unwrap_range_impl<T, V, V, Values...> {
    using type = ::std::integer_sequence< T, V, Values... >;
};

template < typename T, T Min, T Max, T ... Values >
struct reverse_unwrap_range_impl {
    static_assert(Min < Max, "Start of range must be less than ending");
    using type = typename reverse_unwrap_range_impl<T, Min + 1, Max, Min, Values ... >::type;
};

template < typename T, T V, T ... Values >
struct reverse_unwrap_range_impl<T, V, V, Values...> {
    using type = ::std::integer_sequence< T, V, Values... >;
};

const ::std::size_t template_depth = 128;

template < typename T, T Min, T Max >
struct unwrap_range;
template < typename T, T Min, T Max >
struct reverse_unwrap_range;

template < typename T, T Min, T Max >
struct split_range_helper {
    using type = typename join<
            typename unwrap_range_impl<T, Min, Min + template_depth - 1>::type,
            typename unwrap_range<T, Min + template_depth, Max>::type
    >::type;
};

template < typename T, T Min, T Max >
struct unwrap_range :
    ::std::conditional< (Max - Min < template_depth),
         unwrap_range_impl<T, Min, Max>,
         split_range_helper<T, Min, Max>
    >::type {};

template < typename T, T Min, T Max >
struct reverse_split_range_helper {
    using type = typename join<
            typename reverse_unwrap_range<T, Min + template_depth, Max>::type,
            typename reverse_unwrap_range_impl<T, Min, Min + template_depth - 1>::type
    >::type;
};
template < typename T, T Min, T Max >
struct reverse_unwrap_range :
    ::std::conditional< (Max - Min < template_depth),
         reverse_unwrap_range_impl<T, Min, Max>,
         reverse_split_range_helper<T, Min, Max>
    >::type {};

template < typename T, T Min, T Max, bool Reverse >
struct make_integer_sequence_impl : unwrap_range<T, Min, Max> {};

template < typename T, T Min, T Max >
struct make_integer_sequence_impl<T, Min, Max, true> : reverse_unwrap_range<T, Max, Min> {};

} /* namespace detail */


/**
 * Metafunction to make integer sequences in range [Min, Max]
 * If Min > Max, the sequence will be reversed.
 */
template < typename T, T Min, T Max >
struct make_integer_sequence :
    detail::make_integer_sequence_impl<T, Min, Max, (Min > Max )> {};

#endif /* __cplusplus >= 201402L */

} /* namespace meta */
} /* namespace psst */

#endif /* PUSHKIN_META_INTEGER_SEQUENCE_HPP_ */
