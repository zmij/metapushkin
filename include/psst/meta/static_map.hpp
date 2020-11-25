/*
 * static_map.hpp
 *
 *  Created on: Dec 6, 2018
 *      Author: ser-fedorov
 */

#ifndef PSST_META_STATIC_MAP_HPP_
#define PSST_META_STATIC_MAP_HPP_

#include <psst/meta/algorithm.hpp>

namespace psst::meta {

template < typename Key, typename Value, Key K, Value V >
struct static_key_value_pair {
    using key_type = Key;
    using value_type = Value;
    static constexpr key_type key = K;
    static constexpr value_type value = V;
};

namespace detail {

template < typename LHS, typename RHS >
struct static_key_value_pair_compare : std::integral_constant<bool, LHS::key < RHS::key> {};

template < typename T >
struct is_key_value_pair : std::false_type {};

template < typename Key, typename Value, Key K, Value V >
struct is_key_value_pair<static_key_value_pair<Key, Value, K, V>> : std::true_type {};

template < typename Key, typename Value, typename T>
struct types_match : std::false_type {};

template < typename Key, typename Value, Key K, Value V >
struct types_match<Key, Value, static_key_value_pair<Key, Value, K, V>> : std::true_type{};

template < typename Key, Key K, typename T >
struct key_match : std::false_type {};

template < typename Key, typename Value, Key K, Value V >
struct key_match< Key, K, static_key_value_pair<Key, Value, K, V> > : std::true_type {};

template < typename Key, typename Value >
struct pair_storage {

};

//template < typename K >

}  // namespace detail

template < typename ... T >
struct static_map {
    static constexpr std::size_t size = sizeof...(T);
    static_assert(size > 0, "Cannot create an empty static map");
    static_assert((all_match_v<detail::is_key_value_pair, T...>), "All types must be static_key_value_pairs");
    using pair_type = front_t<T...>;
    using key_type = typename pair_type::key_type;
    using value_type = typename pair_type::value_type;
    template< typename Y >
    using match_type = detail::types_match<key_type, value_type, Y>;
    static_assert((all_match_v<match_type, T...>), "All pairs must have the same types");
};

}  // namespace psst::meta


#endif /* PSST_META_STATIC_MAP_HPP_ */
