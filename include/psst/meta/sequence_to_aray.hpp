/*
 * sequence_to_aray.hpp
 *
 *  Created on: Dec 9, 2018
 *      Author: ser-fedorov
 */

#ifndef PSST_META_SEQUENCE_TO_ARAY_HPP_
#define PSST_META_SEQUENCE_TO_ARAY_HPP_

#include <array>
#include <utility>

namespace psst::meta {

template <typename T, T... Values>
constexpr auto
sequence_to_array(std::integer_sequence<T, Values...>)
{
    return std::array<T, sizeof...(Values)>{Values...};
}

}    // namespace psst::meta

#endif /* PSST_META_SEQUENCE_TO_ARAY_HPP_ */
