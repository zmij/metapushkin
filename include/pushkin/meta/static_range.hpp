/*
 * static_range.hpp
 *
 *  Created on: Dec 6, 2018
 *      Author: ser-fedorov
 */

#ifndef PUSHKIN_META_STATIC_RANGE_HPP_
#define PUSHKIN_META_STATIC_RANGE_HPP_

#include <pushkin/meta/algorithm.hpp>
#include <pushkin/meta/sequence_to_aray.hpp>

#include <limits>

namespace psst::meta::range {

enum class range_direction_type {
    empty,
    ascending,
    descending
};

template <typename T, T Begin, T End, bool IncludeEnd = false>
struct range {
    using type          = range<T, Begin, End, IncludeEnd>;
    using bounds_type   = T;

    inline static constexpr bounds_type begin   = Begin;
    inline static constexpr bounds_type end     = End;
    inline static constexpr range_direction_type direction =
            (begin < end ? range_direction_type::ascending :
                    (end < begin ? range_direction_type::descending
                            : range_direction_type::empty));
    inline static constexpr bool include_end = IncludeEnd;

    template <bounds_type B, bounds_type E, bool I = include_end>
    using range_type = range<bounds_type, B, E, I>;
};

template <typename T>
using range_bounds_type = typename T::bounds_type;

template <typename T>
using max_range =
    range<T, std::numeric_limits<T>::min(), std::numeric_limits<T>::max(), true>;

template <typename Key, typename Value, Key Begin, Key End, Value Val, bool IncludeEnd = false>
struct value_range : range<Key, Begin, End, IncludeEnd> {
    using type = value_range<Key, Value, Begin, End, Val, IncludeEnd>;
    using bounds_type = Key;
    using value_type = Value;
    static constexpr Value value = Val;

    template <bounds_type B, bounds_type E, bool I = IncludeEnd>
    using range_type = value_range<bounds_type, value_type, B, E, value, I>;
};

template <typename Key, typename Value, Key Begin, Key End, Value Val>
using value_range_exclusive = value_range<Key, Value, Begin, End, Val, false>;
template <typename Key, typename Value, Key Begin, Key End, Value Val>
using value_range_inclusive = value_range<Key, Value, Begin, End, Val, true>;

template <typename T>
using range_value_type = typename T::value_type;

template <typename Key, typename Value, Value V>
using max_value_range =
    value_range<Key, Value,
        std::numeric_limits<Key>::min(),
        std::numeric_limits<Key>::max(), V, true>;

//@{
/** @name is_range */
template < typename T >
struct is_range : std::false_type {};
template < typename T >
using is_range_t = typename is_range<T>::type;
template < typename T >
constexpr bool is_range_v = is_range_t<T>::value;

template <typename T, T Begin, T End, bool Inc>
struct is_range<range<T, Begin, End, Inc>> : std::true_type {};
template <typename Key, typename Value, Key Begin, Key End, Value Val, bool Inc>
struct is_range<value_range<Key, Value, Begin, End, Val, Inc>> : std::true_type{};
//@}

//@{
/** @name is_value_range */
template < typename T >
struct is_value_range : std::false_type {};
template < typename T >
using is_value_range_t = typename is_value_range<T>::type;
template < typename T >
constexpr bool is_value_range_v = is_value_range_t<T>::value;

template <typename Key, typename Value, Key Begin, Key End, Value Val, bool Inc>
struct is_value_range<value_range<Key, Value, Begin, End, Val, Inc>> : std::true_type{};
//@}

//@{
/** @name is_empty */
namespace detail {

template <typename T>
constexpr bool
is_empty()
{
    static_assert(is_range_v<T>, "Type must be a range");
    return !T::include_end && T::begin == T::end;
}

}  // namespace detail
template < typename T >
struct is_empty : std::integral_constant<bool, detail::is_empty<T>()> {};
template < typename T >
using is_empty_t = typename is_empty<T>::type;
template < typename T >
constexpr bool is_empty_v = is_empty_t<T>::value;

template <>
struct is_empty<void> : std::true_type {};
//@}

//@{
namespace detail {

template <typename T>
constexpr auto
range_begin()
{
    static_assert(is_range_v<T>, "Type must be a range");
    return std::integral_constant<typename T::bounds_type, T::begin>{};
}

}  // namespace detail
template <typename T>
struct range_begin : decltype(detail::range_begin<T>()) {};
template <typename T>
using range_begin_t = typename range_begin<T>::type;
template <typename T>
constexpr range_bounds_type<T> range_begin_v = range_begin_t<T>::value;
//@}

//@{
namespace detail {

template <typename T>
constexpr auto
range_end()
{
    static_assert(is_range_v<T>, "Type must be a range");
    return std::integral_constant<typename T::bounds_type, T::end>{};
}

}  // namespace detail
template <typename T>
struct range_end : decltype(detail::range_end<T>()){};
template <typename T>
using range_end_t = typename range_end<T>::type;
template <typename T>
constexpr range_bounds_type<T> range_end_v = range_end_t<T>::value;
//@}

//@{
namespace detail {

template <typename T>
constexpr auto
range_direction()
{
    static_assert(is_range_v<T>, "Type must be a range");
    return std::integral_constant<range_direction_type, T::direction>{};
}

}  // namespace detail
template <typename T>
struct range_direction : decltype(detail::range_direction<T>()){};
template <typename T>
using range_direction_t = typename range_direction<T>::type;
template <typename T>
constexpr range_direction_type range_direction_v = range_direction_t<T>::value;
//@}

//@{
/** @name range_value */
namespace detail {

template < typename T >
constexpr auto
range_value()
{
    static_assert(is_value_range_v<T>, "Type must be a value range");
    return std::integral_constant<range_value_type<T>, T::value>{};
}

}  // namespace detail
template < typename T >
struct range_value : decltype(detail::range_value<T>()) {};
template < typename T >
using range_value_t = typename range_value<T>::type;
template < typename T >
constexpr auto range_value_v = range_value_t<T>::value;
//@}

//@{
template <typename T>
struct reverse {
    static_assert(is_range_v<T>, "Type must be a range");
    using original_type = T;
    using type = typename original_type
        ::template range_type<original_type::end, original_type::begin>;
};
template <typename T>
using reverse_t = typename reverse<T>::type;
//@}

//@{
template <range_direction_type Direction, typename T>
struct align {
    static_assert(is_range_v<T>, "Type must be a range");
    using original_type = T;
    static constexpr range_direction_type direction = Direction;
    using type = std::conditional_t<
            direction == range_direction_type::empty ||
                original_type::direction == direction,
            original_type,
            reverse_t<original_type>
        >;
};
template <range_direction_type Direction, typename T>
using align_t = typename align<Direction, T>::type;
//@}

//@{
/** @name normalize */
namespace detail {

template <typename T>
constexpr auto
normalize()
{
    using aligned_type = align_t<range_direction_type::ascending, T>;
    using bounds_type  = range_bounds_type<T>;
    if constexpr (aligned_type::include_end) {
        constexpr bool can_overflow =
                range_end_v<T> == std::numeric_limits<bounds_type>::max();
        if constexpr (!can_overflow) {
            return typename aligned_type::template range_type<
                aligned_type::begin, aligned_type::end + 1, false>{};
        } else {
            return aligned_type{};
        }
    } else
        return aligned_type{};
}

}  // namespace detail
/// @brief Return ascending range with exclusive end if possible
template <typename T>
struct normalize {
    using type = decltype(detail::normalize<T>());
};
template <typename T>
using normalize_t = typename normalize<T>::type;
//@}

//@{
/** @name has_bounds_of_type */
namespace detail {

template <typename T, typename U>
constexpr auto
has_bounds_of_type()
{
    static_assert(is_range_v<T>, "Type must be a range");
    return typename std::is_same< range_bounds_type<T>, U >::type{};
}

}  // namespace detail
template < typename T, typename U >
struct has_bounds_of_type
    : decltype(detail::has_bounds_of_type<T,U>()) {};
template < typename T, typename U >
using has_bounds_of_type_t = typename has_bounds_of_type<T, U>::type;
template < typename T, typename U >
constexpr bool has_bounds_of_type_v = has_bounds_of_type_t<T, U>::value;
//@}

//@{
/** @name has_bounds_of_type */
namespace detail {

template <typename T, typename U>
constexpr auto
has_value_of_type()
{
    static_assert(is_value_range_v<T>, "Type must be a value_range");
    return typename std::is_same< range_value_type<T>, U >::type{};
}

}  // namespace detail
template < typename T, typename U >
struct has_value_of_type
    : decltype(detail::has_value_of_type<T,U>()) {};
template < typename T, typename U >
using has_value_of_type_t = typename has_value_of_type<T, U>::type;
template < typename T, typename U >
constexpr bool has_value_of_type_v = has_value_of_type_t<T, U>::value;
//@}

//@{
/** @name same_type */
template <typename LHS, typename RHS>
struct are_same_range_type : std::false_type {
    static_assert(is_range_v<LHS>, "Type must be a range");
    static_assert(is_range_v<RHS>, "Type must be a range");
};
template < typename LHS, typename RHS >
using are_same_range_type_t = typename are_same_range_type<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool are_same_range_type_v = are_same_range_type_t<LHS, RHS>::value;

template < typename T, T LBegin, T LEnd, bool LInc, T RBegin, T REnd, bool RInc >
struct are_same_range_type<range<T, LBegin, LEnd, LInc>,
                    range<T, RBegin, REnd, RInc>> : std::true_type {};
template < typename Key, typename Value,
    Key LBegin, Key LEnd, Value LVal, bool LInc,
    Key RBegin, Key REnd, Value RVal, bool RInc >
struct are_same_range_type<
    value_range<Key, Value, LBegin, LEnd, LVal, LInc>,
    value_range<Key, Value, RBegin, REnd, RVal, RInc>> : std::true_type {};
//@}

//@{
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
have_same_bounds_type()
{
    static_assert(is_range_v<LHS>, "Type must be a range");
    static_assert(is_range_v<RHS>, "Type must be a range");
    return std::is_same<range_bounds_type<LHS>,
            range_bounds_type<RHS>>::value;
}

}  // namespace detail
template < typename LHS, typename RHS >
struct have_same_bounds_type
    : std::integral_constant<bool, detail::have_same_bounds_type<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using have_same_bounds_type_t = typename have_same_bounds_type<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool have_same_bounds_type_v = have_same_bounds_type_t<LHS, RHS>::value;

//@}

//@{
/** @name have_same_value_type */
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
have_same_value_type()
{
    static_assert(is_value_range_v<LHS>, "Type must be a value range");
    static_assert(is_value_range_v<RHS>, "Type must be a value range");
    return std::is_same<range_value_type<LHS>,
            range_value_type<RHS>>::value;
}

}  // namespace detail
template < typename LHS, typename RHS >
struct have_same_value_type
    : std::integral_constant<bool, detail::have_same_value_type<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using have_same_value_type_t = typename have_same_value_type<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool have_same_value_type_v = have_same_value_type_t<LHS, RHS>::value;
//@}

//@{
/** @name have_same_value */
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
have_same_value()
{
    static_assert((have_same_value_type_v<LHS, RHS>),
            "Ranges must have the same value type");
    return range_value_v<LHS> == range_value_v<RHS>;
}

}  // namespace detail
template <typename LHS, typename RHS>
struct have_same_value
    : std::integral_constant<bool, detail::have_same_value<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using have_same_value_t = typename have_same_value<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool have_same_value_v = have_same_value_t<LHS, RHS>::value;
//@}


//@{
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
same_direction()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    return LHS::direction == RHS::direction;
}

}  // namespace detail
template <typename LHS, typename RHS>
struct are_same_direction
    : std::integral_constant<bool, detail::same_direction<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using are_same_direction_t = typename are_same_direction<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool are_same_direction_v = are_same_direction_t<LHS, RHS>::value;
//@}

//@{
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
begin_compare()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    return LHS::begin < RHS::begin;
}

}  // namespace detail
template < typename LHS, typename RHS >
struct begin_compare
    : std::integral_constant<bool, detail::begin_compare<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using begin_compare_t = typename begin_compare<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool begin_compare_v = begin_compare_t<LHS, RHS>::value;
//@}

//@{
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
end_compare()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    if constexpr (LHS::include_end == RHS::include_end) {
        return LHS::end < RHS::end;
    } else if constexpr (LHS::end < RHS::end) {
        if constexpr (LHS::include_end) {
            return LHS::end + 1 < RHS::end;
        } else {
            return true;
        }
    } else if constexpr (LHS::end == RHS::end && RHS::include_end) {
        return true;
    }
    return false;
}

}  // namespace detail
template < typename LHS, typename RHS >
struct end_compare
    : std::integral_constant<bool, detail::end_compare<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using end_compare_t = typename end_compare<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool end_compare_v = end_compare_t<LHS, RHS>::value;
//@}

//@{
namespace detail {

template <typename LHS, typename RHS>
constexpr bool
is_before()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    using lhs_aligned = align_t<range_direction_type::ascending, LHS>;
    using rhs_aligned = align_t<range_direction_type::ascending, RHS>;
    if constexpr (lhs_aligned::include_end)
            return lhs_aligned::end < rhs_aligned::begin;
    return lhs_aligned::end <= rhs_aligned::begin;
}

}  // namespace detail
template <typename LHS, typename RHS>
struct before_compare
        : std::integral_constant<bool, detail::is_before<LHS, RHS>()>{};
template < typename LHS, typename RHS >
using before_compare_t = typename before_compare<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool before_compare_v = before_compare_t<LHS, RHS>::value;
//@}

//@{
/** @name ranges_overlap */
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
overlaps()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    if constexpr (is_before<LHS, RHS>())
            return false;
    if constexpr (is_before<RHS, LHS>())
            return false;
    return true;
}

}  // namespace detail

template <typename LHS, typename RHS>
struct overlaps
    : std::integral_constant<bool, detail::overlaps<LHS, RHS>()>{};
template < typename LHS, typename RHS >
using overlaps_t = typename overlaps<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool overlaps_v = overlaps_t<LHS, RHS>::value;
//@}

//@{
/** @name contains */
namespace detail {

template < typename LHS, typename RHS >
constexpr bool
contains()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    using lhs_aligned = align_t<range_direction_type::ascending, LHS>;
    using rhs_aligned = align_t<range_direction_type::ascending, RHS>;
    return lhs_aligned::begin <= rhs_aligned::begin
            && rhs_aligned::end <= lhs_aligned::end;
}

}  // namespace detail

template <typename LHS, typename RHS>
struct contains
        : std::integral_constant<bool, detail::contains<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using contains_t = typename contains<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool contains_v = contains_t<LHS, RHS>::value;
//@}

//@{
template <typename LHS, typename RHS>
struct contained {
    using type = std::conditional_t<
            overlaps_v<LHS, RHS>,
            std::conditional_t<
                contains_v<LHS, RHS>,
                RHS,
                std::conditional_t<
                    contains_v<RHS, LHS>,
                    LHS,
                    void
                >
            >,
            void
        >;
};
template <typename LHS, typename RHS>
using contained_t = typename contained<LHS, RHS>::type;
//@}

//@{
/** @name is_adjacent */
namespace detail {

template <typename LHS, typename RHS>
constexpr bool
is_adjacent()
{
    static_assert((have_same_bounds_type_v<LHS, RHS>),
            "Ranges must have the same bounds type");
    using lhs_aligned = align_t<range_direction_type::ascending, LHS>;
    using rhs_aligned = align_t<range_direction_type::ascending, RHS>;
    if constexpr (lhs_aligned::end <= rhs_aligned::begin) {
        if constexpr (lhs_aligned::include_end)
            return lhs_aligned::end + 1 == rhs_aligned::begin;
        else
            return lhs_aligned::end == rhs_aligned::begin;
    }
    return false;
}

}  // namespace detail

template <typename LHS, typename RHS>
struct is_adjacent : std::integral_constant<bool, detail::is_adjacent<LHS, RHS>()> {};
template < typename LHS, typename RHS >
using is_adjacent_t = typename is_adjacent<LHS, RHS>::type;
template < typename LHS, typename RHS >
constexpr bool is_adjacent_v = is_adjacent_t<LHS, RHS>::value;

//@}

//@{
namespace detail {

template <typename LHS, typename RHS>
constexpr auto
intersect()
{
    static_assert((are_same_range_type_v<LHS, RHS>),
            "Ranges must have the same type");
    using original_direction    = range_direction_t<LHS>;
    using lhs_aligned           = align_t<range_direction_type::ascending, LHS>;
    using rhs_aligned           = align_t<range_direction_type::ascending, RHS>;
    if constexpr (overlaps_v<LHS, RHS>) {
        using begin_order = stable_order<meta::range::begin_compare, lhs_aligned, rhs_aligned>;
        using end_order = stable_order<meta::range::end_compare, lhs_aligned, rhs_aligned>;
        using right_begin = typename begin_order::second;
        using left_end = normalize_t<typename end_order::first>;
        using intersection = typename right_begin::template range_type<
            right_begin::begin, left_end::end, left_end::include_end>;
        return align_t<original_direction::value, intersection>{};
    } else {
        return;
    }
}

}  // namespace detail
template <typename LHS, typename RHS>
struct intersect {
    using type = decltype(detail::intersect<LHS, RHS>());
};
template <typename LHS, typename RHS>
using intersect_t = typename intersect<LHS, RHS>::type;
//@}

//@{
namespace detail {

// Primary template
template <typename Value, Value Default, typename ... Ranges>
struct make_adjacent_ranges;
// Type alias for result
template <typename Value, Value Default, typename ... Ranges>
using make_adjacent_ranges_t = typename make_adjacent_ranges<Value, Default, Ranges...>::type;

// Unpack type_tuple
template <typename Value, Value Default, typename ... Ranges>
struct make_adjacent_ranges<Value, Default, type_tuple<Ranges...>>
    : make_adjacent_ranges<Value, Default, Ranges...> {};

// Empty pack
template <typename Value, Value Default>
struct make_adjacent_ranges<Value, Default> {
    using type = type_tuple<>;
};

// Single range
template <typename Value, Value Default, typename Range>
struct make_adjacent_ranges<Value, Default, Range> {
    using type = type_tuple<Range>;
};

template <typename Value, Value Default, typename LHS, typename RHS>
constexpr auto
make_range_sequence()
{
    static_assert(is_value_range_v<LHS>, "Type must be a value range");
    static_assert(is_value_range_v<RHS>, "Type must be a value range");
    static_assert((are_same_range_type_v<LHS, RHS>),
            "Types must be of same type");

    using bounds_type = range_bounds_type<LHS>;
    using value_type = Value;
    using lhs_norm = normalize_t<LHS>;
    using rhs_norm = normalize_t<RHS>;
    using begin_order = stable_order<
            meta::range::begin_compare, lhs_norm, rhs_norm>;
    using end_order = stable_order<
            meta::range::end_compare, lhs_norm, rhs_norm>;
    if constexpr (lhs_norm::value == rhs_norm::value) {
        return type_tuple<
            value_range<
                bounds_type, value_type,
                begin_order::first::begin, end_order::second::end,
                begin_order::first::value, end_order::second::include_end
            >
        >{};
    } else if constexpr (overlaps<lhs_norm, rhs_norm>()) {
        constexpr bool begins_equal = lhs_norm::begin == rhs_norm::begin;
        using end_rev_order = stable_order<
                meta::range::end_compare, rhs_norm, lhs_norm>;
        constexpr bool ends_equal =
                (end_order::original_order && end_rev_order::original_order);

        using first = std::conditional_t<
                begins_equal,
                void,
                value_range<
                    bounds_type, value_type,
                    begin_order::first::begin, begin_order::second::begin,
                    begin_order::first::value, false
                >
            >;
        using middle = value_range<
                    bounds_type, value_type,
                    begin_order::second::begin, end_order::first::end,
                    begin_order::second::value, end_order::first::include_end
                >;
        using last = std::conditional_t<
                ends_equal,
                void,
                value_range<
                    bounds_type, value_type,
                    end_order::first::end, end_order::second::end,
                    end_order::second::value, end_order::second::include_end
                >
            >;

        return remove_if_t< meta::range::is_empty, first, middle, last>{};
    } else if constexpr (!is_adjacent<LHS, RHS>()) {
        // Build a type tuple of ranges:
        // left side, gap fill, right side
        return type_tuple<
                lhs_norm,
                value_range<
                    bounds_type, value_type,
                    lhs_norm::end, rhs_norm::begin, Default, false>,
                rhs_norm
            >{};
    } else {
        // Non-intersecting adjacent ranges
        return type_tuple<lhs_norm, rhs_norm>{};
    }
}

// The workhorse
template <typename Value, Value Default, typename LHS, typename RHS>
struct make_adjacent_ranges<Value, Default, LHS, RHS> {
    using type = decltype(make_range_sequence<Value, Default, LHS, RHS>());
};

template <typename Value, Value Default, typename RangeTuple, typename Range>
struct add_range_to_end {
    using range_tuple   = RangeTuple;
    using next          = Range;
    using tail          = back_t<range_tuple>;
    using head          = pop_back_t<range_tuple>;
    using type          = combine_t<
            head,
            make_adjacent_ranges_t<Value, Default, tail, next>
        >;
};

template <typename Value, Value Default, typename RangeTuple, typename Range,
    typename Overlap, std::size_t OverlapSize>
struct replace_overlap;

template <typename Value, Value Default, typename RangeTuple, typename Range,
    typename Overlap>
struct replace_overlap<Value, Default, RangeTuple, Range, Overlap, 0>
    : add_range_to_end<Value, Default, RangeTuple, Range> {};

template <typename Value, Value Default, typename RangeTuple, typename Range,
    typename Overlap>
struct replace_overlap<Value, Default, RangeTuple, Range, Overlap, 1> {
    using range_tuple   = RangeTuple;
    using next          = Range;
    using overlap       = Overlap;

    template < typename U >
    using in_overlap_list = meta::contains<U, overlap>;
    using overlap_removed = remove_if_t<in_overlap_list, range_tuple>;

    using type = stable_sort_t<
            meta::range::begin_compare,
            combine_t<
                overlap_removed,
                make_adjacent_ranges_t<
                    Value, Default, combine_t<overlap, next> >
            >
        >;
};

// TODO Replace overlap of 2

template <typename Value, Value Default, typename RangeTuple, typename ... Rest>
struct merge_ranges;

template <typename Value, Value Default, typename RangeTuple>
struct merge_ranges<Value, Default, RangeTuple> {
    using type = RangeTuple;
};

template <typename Value, Value Default, typename RangeTuple, typename Range, typename ... Rest>
struct merge_ranges<Value, Default, RangeTuple, Range, Rest...> {
    using range_tuple   = RangeTuple;
    using next          = Range;

    template < typename U >
    using overlaps_next = meta::range::overlaps<next, U>;
    using overlap       = find_if_t<overlaps_next, range_tuple>;

    using merged = typename replace_overlap< Value, Default,
            range_tuple, next, overlap, overlap::size >::type;
    using type = typename merge_ranges<Value, Default, merged, Rest...>::type;
};

// Handle first two
template <typename Value, Value Default, typename LHS, typename RHS, typename ... T>
struct make_adjacent_ranges<Value, Default, LHS, RHS, T...> {
    using range_tuple = make_adjacent_ranges_t<Value, Default, LHS, RHS>;
    using type = typename merge_ranges<Value, Default, range_tuple, T...>::type;
};

template <typename ... T>
struct build_range_starts {
    using bounds_type = range_bounds_type<front_t<T...>>;
    using type = std::integer_sequence<bounds_type, range_begin_v<T>...>;
    static constexpr auto value = sequence_to_array(type{});
};
template <typename ... T>
using build_range_starts_t = typename build_range_starts<T...>::type;

template <typename ... T>
struct build_range_starts<type_tuple<T...>> : build_range_starts<T...> {};
template <>
struct build_range_starts<> {
    using type = std::integer_sequence<std::size_t>;
};

template <typename ... T>
struct build_range_values {
    using value_type = range_value_type<front_t<T...>>;
    using type = std::array<value_type, sizeof...(T)>;
    static constexpr type value{
        range_value_v<T>...
    };
};
template <typename ... T>
using build_range_values_t = typename build_range_values<T...>::type;

template <typename ... T>
struct build_range_values<type_tuple<T...>> : build_range_values<T...> {};
template <>
struct build_range_values<> {
    using type = std::integer_sequence<std::size_t>;
};

template <typename Value, Value Default, typename ... T>
struct build_adjacent_ranges;

// Accepts a sorted type tuple of non-empty normalized ranges
template <typename Value, Value Default, typename ... T>
struct build_adjacent_ranges<Value, Default, type_tuple<T...>> {
    using type = make_adjacent_ranges_t<Value, Default, T...>;
    using range_starts = build_range_starts<type>;
    using range_values = build_range_values<type>;
};

}  // namespace detail
template <typename Key, typename Value, Value Default,
        typename ... Ranges>
struct range_map {
    using key_type      = Key;
    using value_type    = Value;
    static constexpr value_type default_value = Default;

    static_assert((all_match_v<is_value_range, Ranges...>),
            "All ranges must be value ranges");

    template <typename T>
    using bounds_type_check = has_bounds_of_type<T, key_type>;
    static_assert((all_match_v<bounds_type_check, Ranges...>),
            "All ranges must have the same bounds type");

    template <typename T>
    using value_type_check = has_value_of_type<T, value_type>;
    static_assert((all_match_v<value_type_check, Ranges...>),
            "All ranges must have the same value type");
    using default_value_range =
            max_value_range<key_type, value_type, default_value>;
    using ranges = detail::build_adjacent_ranges<
        value_type, default_value,
        combine_t<
            default_value_range,
            stable_sort_t<
                begin_compare,
                transform_t<
                    normalize_t,
                    remove_if_t<is_empty, Ranges...>
                >
            >
        >
    >;
    static constexpr std::size_t size = ranges::type::size;

    static constexpr auto range_starts = ranges::range_starts::value;
    using range_start_container = decltype(range_starts);
    using range_start_iterator  = typename range_start_container::const_iterator;
    static constexpr auto range_values = ranges::range_values::value;

    static constexpr range_start_iterator
    lower_bound(key_type const& k)
    {
        auto left = range_starts.begin();
        auto right = range_starts.end();

        while (left != right) {
            auto mid = left + (right - left) / 2;
            if (k <= *mid) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        if (k == *right) {
            return right;
        }
        if (right > range_starts.begin()
                && k >= *(right - 1)) {
            return (right - 1);
        }
        return range_starts.end();
    }
    static constexpr value_type const&
    get_value(key_type const& k)
    {
        auto lb = lower_bound(k);
        auto idx = lb - range_starts.begin();
        if (0 <= idx && idx < static_cast<decltype(idx)>(size)) {
            return range_values[idx];
        }
        return default_value;
    }
};
//@}

}  // namespace psst::meta::range

#endif /* PUSHKIN_META_STATIC_RANGE_HPP_ */
