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

#include "../../types.h"

#include <array>

#include "../../bitboard.h"
#include "../../core.h"
#include "../../util/bits.h"
#include "../util.h"
#include "data.h"

namespace stormphrax::attacks {
    extern const std::array<u16, bmi2::kRookData.tableSize> g_rookAttacks;
    extern const std::array<Bitboard, bmi2::kBishopData.tableSize> g_bishopAttacks;

    inline Bitboard getRookAttacks(Square src, Bitboard occupancy) {
        const auto s = static_cast<i32>(src);

        const auto& data = bmi2::kRookData.data[s];

        const auto idx = util::pext(occupancy, data.srcMask);
        const auto attacks = util::pdep(g_rookAttacks[data.offset + idx], data.dstMask);

        return attacks;
    }

    inline Bitboard getBishopAttacks(Square src, Bitboard occupancy) {
        const auto s = static_cast<i32>(src);

        const auto& data = bmi2::kBishopData.data[s];
        const auto idx = util::pext(occupancy, data.mask);

        return g_bishopAttacks[data.offset + idx];
    }
} // namespace stormphrax::attacks
