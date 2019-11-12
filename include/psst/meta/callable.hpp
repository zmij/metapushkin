/*
 * callable.hpp
 *
 *  Created on: 29 мая 2016 г.
 *      Author: sergey.fedorov
 */

#ifndef PSST_META_CALLABLE_HPP_
#define PSST_META_CALLABLE_HPP_

#include <psst/meta/index_tuple.hpp>

#include <tuple>
#include <type_traits>

namespace psst {
namespace meta {

template <typename Functor, typename... Args>
struct is_callable {
private:
    using args_tuple = ::std::tuple<Args...>;
    using indexes    = typename index_builder<sizeof...(Args)>::type;
    static args_tuple& args;

    template <typename U, ::std::size_t... Indexes>
    static ::std::true_type
    test(indexes_tuple<Indexes...> const&,
         decltype(::std::declval<U>()(::std::forward<Args>(::std::get<Indexes>(args))...)) const*);
    template <typename U>
    static ::std::false_type
    test(...);

public:
    static constexpr bool value = decltype(test<Functor>(indexes{}, nullptr))::value;
};

template <typename Predicate>
struct not_ {
    template <typename... Args>
    bool
    operator()(Args&&... args) const
    {
        return !Predicate{}(::std::forward<Args>(args)...);
    }
};

template <typename... Predicate>
struct and_ {
    template <typename... Args>
    bool
    operator()(Args&&... args) const
    {
        return (Predicate{}(std::forward<Args>(args)...) && ...);
    }
};

template <>
struct and_<> {
    template <typename... Args>
    bool
    operator()(Args&&...) const
    {
        return false;
    }
};

template <typename... Predicate>
struct or_ {
    template <typename... Args>
    bool
    operator()(Args&&... args) const
    {
        return (Predicate{}(std::forward<Args>(args)...) || ...);
    }
};

template <>
struct or_<> {
    template <typename... Args>
    bool
    operator()(Args&&...) const
    {
        return true;
    }
};

} /* namespace meta */
}    // namespace psst

#endif /* PSST_META_CALLABLE_HPP_ */
