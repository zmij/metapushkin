/**
 * Copyright 2019 Sergei A. Fedorov
 * enum_range.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_UTIL_ENUM_RANGE_HPP_
#define PSST_UTIL_ENUM_RANGE_HPP_

#include <psst/util/char_utils.hpp>
#include <psst/util/is_declared.hpp>
#include <psst/util/void_t.hpp>
#include <array>
#include <string_view>
#include <type_traits>
#include <utility>

/**
 * Utility to use enumerations for range-based iterations.
 *
 * Usage synopsis:
 * @code
 * for (auto e : enum_range<my_enum>()) {
 *   // Use the enumeration value
 * }
 * @endcode
 */
namespace psst {
namespace util {

/**
 * Primary template to declare enumerator list and/or mapping from enumerators to corresponding
 * literals.
 *
 * Given an enumeration like this:
 * @code
 * enum class traffic_lights {
 *      green,
 *      yellow,
 *      red
 * };
 * @endcode
 *
 * there are two options to specialise the enumerators template. The first variant is to specialise
 * the template just for iteration over it:
 *
 * @code
 * namespace psst::util {
 *
 * template <>
 * struct enumerators <traffic_lights> : enum_helper<traffic_lights> {
 *     static constexpr enum_type values[]{
 *         enum_type::green,
 *         enum_type::yellow,
 *         enum_type::red
 *     };
 * };
 *
 * }
 * @endcode
 *
 * The second variant is
 */
template <typename Enum>
struct enumerators;

/**
 * Helper template for declaring enumerators.
 */
template <typename Enum>
struct enum_helper {
    static_assert((std::is_enum<Enum>::value),
                  "The helper can be used only with enumeration types");
    /**
     * Type alias for the enumeration type.
     */
    using enum_type = Enum;
    struct enumerator_literal {
        const enum_type        enumerator;
        const std::string_view literal;

        constexpr enumerator_literal(enum_type e, char const* l) : enumerator{e}, literal{l} {}
    };
};

namespace detail {

//@{
/** @name enumerators_declared */
template <typename Enum>
struct enumerators_declared : is_declared<enumerators<Enum>> {};
template <typename Enum>
using enumerators_declared_t = typename enumerators_declared<Enum>::type;
template <typename Enum>
constexpr bool enumerators_declared_v = enumerators_declared_t<Enum>::value;

template <typename Enum>
using enable_if_enumerators_declared = std::enable_if_t<enumerators_declared_v<Enum>>;
//@}

//@{
/** @name has_enumerators_member */
template <typename Enum, typename = void_t<>>
struct has_enumerators_member : std::false_type {};
template <typename Enum>
using has_enumerators_member_t = typename has_enumerators_member<Enum>::type;
template <typename Enum>
constexpr bool has_enumerators_member_v = has_enumerators_member_t<Enum>::value;
template <typename Enum>
struct has_enumerators_member<Enum, void_t<decltype(enumerators<Enum>::values)>> : std::true_type {
};

template <typename Enum, typename = enable_if_enumerators_declared<Enum>>
using enable_if_has_enumerators_member = std::enable_if_t<has_enumerators_member_v<Enum>>;
//@}

//@{
template <typename Enum, typename = enable_if_has_enumerators_member<Enum>>
using enumerator_values_type = decltype(enumerators<Enum>::values);
//@}

//@{
/**
 * Selector for implementation where the enumerators function returns just a list of enumerators or
 * a mapping of enumerators to literals
 */
enum class enumerators_type { undefined, enumerators, literals };
template <enumerators_type V>
using enumerators_type_constant = std::integral_constant<enumerators_type, V>;
using enumerators_only_type     = enumerators_type_constant<enumerators_type::enumerators>;
using literals_type             = enumerators_type_constant<enumerators_type::literals>;
//@}

template <typename Enum>
struct enumerator_values_traits {
    using helper_type = enum_helper<Enum>;
    using values_type = enumerator_values_type<Enum>;
    static_assert(std::is_array<values_type>{}, "The `values` member must be an array");
    static constexpr auto size = std::extent<values_type>::value;
    using value_type           = std::remove_const_t<std::remove_extent_t<values_type>>;
    static constexpr enumerators_type classification
        = std::is_same<value_type, Enum>::value
              ? enumerators_type::enumerators
              : std::is_same<value_type, typename helper_type::enumerator_literal>::value
                    ? enumerators_type::literals
                    : enumerators_type::undefined;
};

template <typename Enum, typename = void_t<>>
struct enumerators_class : enumerators_type_constant<enumerators_type::undefined> {};
template <typename Enum>
struct enumerators_class<Enum, enable_if_has_enumerators_member<Enum>>
    : enumerators_type_constant<enumerator_values_traits<Enum>::classification> {};
template <typename Enum>
using enumerators_class_t = typename enumerators_class<Enum>::type;

template <typename T>
constexpr T const*
binary_search(T const* begin, T const* end, T v)
{
    T const* left  = begin;
    T const* right = end;

    while (left != right) {
        auto mid = left + (right - left) / 2;
        if (mid == end)
            return end;
        if (v == *mid)
            return mid;
        if (v < *mid) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    return end;
}

template <typename T>
constexpr T const*
linear_search(T const* begin, T const* end, T v)
{
    auto cur = begin;
    for (; cur != end && v != *cur; ++cur) {}
    return cur;
}

// Calculate the number of elements less than the value
template <typename T>
constexpr std::size_t
index_of_linear(T const* begin, T const* end, T v)
{
    std::size_t n{0};
    for (auto cur = begin; cur != end; ++cur) {
        if (*cur < v)
            ++n;
    }

    return n;
}

template <typename T>
constexpr std::size_t
position_of_linear(T const* begin, T const* end, T v)
{
    std::size_t n = 0;
    for (auto cur = begin; cur != end; ++cur, ++n) {
        if (*cur == v)
            break;
    }
    return n;
}

template <typename Enum, typename FuncType>
struct enum_traits_impl;

template <typename Enum>
struct enum_traits_impl<Enum, enumerators_only_type> {
    using helper_type                      = enum_helper<Enum>;
    using traits_type                      = enumerator_values_traits<Enum>;
    using value_type                       = Enum;
    using const_iterator                   = Enum const*;
    static constexpr std::size_t    npos   = std::numeric_limits<std::size_t>::max();
    static constexpr std::size_t    size   = traits_type::size;
    static constexpr auto           values = enumerators<Enum>::values;
    static constexpr const_iterator begin  = values;
    static constexpr const_iterator end    = values + size;

    static constexpr const_iterator
    position(value_type v)
    {
        return binary_search(begin, end, v);
    }

    static constexpr std::size_t
    index(value_type v)
    {
        auto pos = position(v);
        if (pos != end) {
            return pos - begin;
        }
        return npos;
    }

    static constexpr bool
    valid(value_type v)
    {
        return position(v) != end;
    }
};

template <typename Enum, typename IndexTuple>
struct extract_enumerators;

template <typename Enum, std::size_t... Indexes>
struct extract_enumerators<Enum, std::index_sequence<Indexes...>> {
    using helper_type = enum_helper<Enum>;
    using traits_type = enumerator_values_traits<Enum>;
    struct hash_to_enumerator {
        std::size_t hash;
        Enum        enumerator;
    };
    using hash_iterator                           = hash_to_enumerator const*;
    static constexpr auto             size        = traits_type::size;
    static constexpr auto             literal_map = enumerators<Enum>::values;
    static constexpr Enum             values[]{literal_map[Indexes].enumerator...};
    static constexpr std::string_view literals[]{literal_map[Indexes].literal...};
    static constexpr std::size_t      hashes[size]{string_hash(literal_map[Indexes].literal)...};
    static constexpr std::size_t      hash_indexes[size]{
        index_of_linear(hashes, hashes + size, hashes[Indexes])...};
    static constexpr std::size_t literal_indexes[size]{
        position_of_linear(hash_indexes, hash_indexes + size, Indexes)...};
    static constexpr hash_to_enumerator enumerator_map[size]{
        {hashes[literal_indexes[Indexes]], values[literal_indexes[Indexes]]}...};
};

template <typename Enum>
struct enum_traits_impl<Enum, literals_type> {
    using helper_type                          = enum_helper<Enum>;
    using traits_type                          = enumerator_values_traits<Enum>;
    using value_type                           = Enum;
    using const_iterator                       = Enum const*;
    static constexpr std::size_t npos          = std::numeric_limits<std::size_t>::max();
    static constexpr std::size_t size          = traits_type::size;
    using index_sequence                       = std::make_index_sequence<traits_type::size>;
    using extractor_type                       = extract_enumerators<Enum, index_sequence>;
    using hash_iterator                        = typename extractor_type::hash_iterator;
    static constexpr auto           values     = extractor_type::values;
    static constexpr auto           literals   = extractor_type::literals;
    static constexpr const_iterator begin      = values;
    static constexpr const_iterator end        = values + size;
    static constexpr auto           hash_map   = extractor_type::enumerator_map;
    static constexpr hash_iterator  hash_begin = hash_map;
    static constexpr hash_iterator  hash_end   = hash_begin + size;

    template <std::size_t N>
    struct get {
        static constexpr value_type       value   = values[N];
        static constexpr std::string_view literal = literals[N];
    };
    // enumerator -> literal - find index in values, return literal from literals by index
    // literal -> enumerator - build a container with literal hashes
    static constexpr const_iterator
    position(value_type v)
    {
        return binary_search(begin, end, v);
    }

    static constexpr std::size_t
    index(value_type v)
    {
        auto pos = position(v);
        if (pos != end) {
            return pos - begin;
        }
        return npos;
    }

    static constexpr bool
    valid(value_type v)
    {
        return position(v) != end;
    }

    static constexpr std::string_view
    literal(value_type v)
    {
        auto idx = index(v);
        if (idx != npos) {
            return literals[idx];
        }
        throw std::runtime_error("Invalid enumerator value");
    }

    static constexpr value_type
    enumerator(std::string_view s)
    {
        auto hash  = string_hash(s);
        auto left  = hash_begin;
        auto right = hash_end;
        while (left != right) {
            auto mid = left + (right - left) / 2;
            if (mid == hash_end)
                break;
            if (hash == mid->hash) {
                return mid->enumerator;
            }
            if (hash < mid->hash) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        throw std::runtime_error("Invalid literal");
    }
};

struct enum_begin {};
struct enum_end {};

}    // namespace detail

constexpr detail::enum_begin enum_begin;
constexpr detail::enum_end   enum_end;

template <typename Enum>
using enable_if_has_enumerators = detail::enable_if_has_enumerators_member<Enum>;

template <typename Enum, typename = enable_if_has_enumerators<Enum>>
struct enum_traits : detail::enum_traits_impl<Enum, detail::enumerators_class_t<Enum>> {
    using type          = Enum;
    using integral_type = typename std::underlying_type<Enum>::type;
};

template <typename Enum>
struct enum_value_range {
    static_assert(detail::enumerators_declared_v<Enum>,
                  "Specialisation of psst::util::enumerators is not declared");
    static_assert(detail::has_enumerators_member_v<Enum>,
                  "The specialisation of psst::util::enumerators doesn't have the `values` member");
    using traits_type    = enum_traits<Enum>;
    using value_type     = typename traits_type::value_type;
    using const_iterator = typename traits_type::const_iterator;

    constexpr enum_value_range() : enum_value_range{traits_type::begin, traits_type::end} {}
    /**
     * Enumeration value range with inclusive ends
     * @param from
     * @param to
     */
    constexpr enum_value_range(value_type from, value_type to)
        : enum_value_range{traits_type::position(from), traits_type::position(to) + 1}
    {}

    constexpr explicit enum_value_range(value_type from, detail::enum_end = enum_end)
        : enum_value_range(traits_type::position(from), traits_type::end)
    {}

    constexpr enum_value_range(detail::enum_begin, value_type to)
        : enum_value_range(traits_type::begin, traits_type::position(to) + 1)
    {}

    constexpr std::size_t
    size() const
    {
        return end_ - begin_;
    }

    constexpr bool
    empty() const
    {
        return begin_ == end_;
    }

    constexpr const_iterator
    begin() const
    {
        return begin_;
    }
    constexpr const_iterator
    end() const
    {
        return end_;
    }

private:
    constexpr enum_value_range(const_iterator first, const_iterator last)
        : begin_{first}, end_{last}
    {
        static_assert(begin_ < end_, "The range ends must be in ascending order");
    }

private:
    const_iterator const begin_;
    const_iterator const end_;
};

/**
 * Enumeration range-based loop helper
 *
 * All elements of the enumeration.
 */
template <typename Enum>
constexpr auto
enum_range()
{
    return enum_value_range<Enum>();
}

/**
 * Enumeration range-based loop helper
 *
 * All elements of the enumeration.
 */
template <typename Enum>
constexpr auto
enum_range(Enum first, Enum last)
{
    return enum_value_range<Enum>(first, last);
}

}    // namespace util
}    // namespace psst

#endif /* PSST_UTIL_ENUM_RANGE_HPP_ */
