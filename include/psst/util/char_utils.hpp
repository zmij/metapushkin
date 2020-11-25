/**
 * Copyright 2019 Sergei A. Fedorov
 * char_utils.hpp
 *
 *  Created on: Jan 19, 2019
 *      Author: ser-fedorov
 */

#ifndef PSST_UTIL_CHAR_UTILS_HPP_
#define PSST_UTIL_CHAR_UTILS_HPP_

#include <cstddef>
#include <string_view>

namespace psst {
namespace util {

/**
 * constexpr function to calculate the length of a null-terminated string
 * @param s
 * @return
 */
inline constexpr std::size_t
string_length(char const* s)
{
    if (s == nullptr)
        return 0;
    std::size_t sz = 0;
    for (; *s != '\0'; ++s, ++sz)
        ;
    return sz;
}

inline constexpr std::size_t
rotate_hash(std::size_t x, std::size_t r)
{
    return (x << r) | (x >> (64 - r));
}

/**
 * constexpr hash function for null-terminated strings.
 *
 * A very naïve implementation, but will do for the purpose of hashing a small number of strings.
 */
inline constexpr std::size_t
string_hash(char const* s, std::size_t length, std::size_t seed = 0xc005)
{
    std::size_t hash = seed;
    if (s == nullptr)
        return hash;

    for (auto c = s; *c != '\0' && static_cast<std::size_t>(c - s) < length; ++c) {
        hash = rotate_hash(hash, 63) ^ *c;
    }
    return hash;
}

inline constexpr std::size_t
string_hash(std::string_view s)
{
    return string_hash(s.data(), s.size());
}

}    // namespace util
}    // namespace psst

#endif /* PSST_UTIL_CHAR_UTILS_HPP_ */
