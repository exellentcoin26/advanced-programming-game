#pragma once

#ifndef GAME_SRC_UTILS_TYPES_H
#define GAME_SRC_UTILS_TYPES_H

#include <cstdint>
#include <type_traits>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using isize = std::make_signed_t<std::size_t>;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = std::size_t;

using f32 = float;
using f64 = double;

#endif // GAME_SRC_UTILS_TYPES_H
