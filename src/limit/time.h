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

#include <array>
#include <atomic>
#include <optional>

#include "../util/multi_array.h"
#include "../util/range.h"
#include "../util/timer.h"
#include "limit.h"

namespace stormphrax::limit {
    constexpr i32 kDefaultMoveOverhead = 10;
    constexpr util::Range<i32> kMoveOverheadRange{0, 50000};

    class MoveTimeLimiter final : public ISearchLimiter {
    public:
        explicit MoveTimeLimiter(i64 time, i64 overhead = 0);
        ~MoveTimeLimiter() final = default;

        [[nodiscard]] bool stop(const search::SearchData& data, bool allowSoftTimeout) final;

        [[nodiscard]] bool stopped() const final;

    private:
        util::Instant m_endTime;
        std::atomic_bool m_stopped{false};
    };

    class TimeManager final : public ISearchLimiter {
    public:
        TimeManager(util::Instant start, f64 remaining, f64 increment, i32 toGo, f64 overhead);
        ~TimeManager() final = default;

        void update(const search::SearchData& data, Score score, Move bestMove, usize totalNodes) final;
        void updateMoveNodes(Move move, usize nodes) final;

        [[nodiscard]] bool stop(const search::SearchData& data, bool allowSoftTimeout) final;

        [[nodiscard]] bool stopped() const final;

    private:
        util::Instant m_startTime;

        f64 m_softTime{};
        f64 m_maxTime{};

        f64 m_scale{1.0};

        util::MultiArray<usize, 64, 64> m_moveNodeCounts{};

        Move m_prevBestMove{};
        u32 m_stability{};

        std::optional<Score> m_avgScore{};

        std::atomic_bool m_stopped{false};
    };
} // namespace stormphrax::limit
