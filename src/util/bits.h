/*
 * Stormphrax, a UCI chess engine
 * Copyright (C) 2025 Ciekce
 *
 * Stormphrax is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Stormphrax is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Stormphrax. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "../types.h"

#include <bit>
#include <type_traits>

#include "../arch.h"

#if SP_HAS_BMI2
    #include <immintrin.h>
#endif

namespace stormphrax::util {
    namespace fallback {
        [[nodiscard]] constexpr u64 pext(u64 v, u64 mask) {
            u64 dst{};

            for (u64 bit = 1; mask != 0; bit <<= 1) {
                if ((v & mask & -mask) != 0) {
                    dst |= bit;
                }
                mask &= mask - 1;
            }

            return dst;
        }

        [[nodiscard]] constexpr u64 pdep(u64 v, u64 mask) {
            u64 dst{};

            for (u64 bit = 1; mask != 0; bit <<= 1) {
                if ((v & bit) != 0) {
                    dst |= mask & -mask;
                }
                mask &= mask - 1;
            }

            return dst;
        }
    } // namespace fallback

    [[nodiscard]] constexpr u64 isolateLsb(u64 v) {
        return v & -v;
    }

    [[nodiscard]] constexpr u64 resetLsb(u64 v) {
        return v & (v - 1);
    }

    [[nodiscard]] constexpr i32 ctz(u64 v) {
        if (std::is_constant_evaluated()) {
            return std::countr_zero(v);
        }

        return __builtin_ctzll(v);
    }

    [[nodiscard]] constexpr u64 pext(u64 v, u64 mask) {
#if SP_HAS_BMI2
        if (std::is_constant_evaluated()) {
            return fallback::pext(v, mask);
        }

        return _pext_u64(v, mask);
#else
        return fallback::pext(v, mask);
#endif
    }

    [[nodiscard]] constexpr u64 pdep(u64 v, u64 mask) {
#if SP_HAS_BMI2
        if (std::is_constant_evaluated()) {
            return fallback::pdep(v, mask);
        }

        return _pdep_u64(v, mask);
#else
        return fallback::pdep(v, mask);
#endif
    }
} // namespace stormphrax::util
