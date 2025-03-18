#pragma once

#include <vector>
#include <tuple>
#include <array>
#include "status.h"

namespace kwi::neighbors {

    namespace status_cost {
        // Functions returning status and cost
        std::vector<std::pair<kwi::status::Status, uint>> axisTargetOnly(const kwi::status::Status &s);
        std::vector<std::pair<kwi::status::Status, uint>> planarTargetOnly(const kwi::status::Status &s);
        std::vector<std::pair<kwi::status::Status, uint>> axisFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
        std::vector<std::pair<kwi::status::Status, uint>> planarFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
        std::vector<std::pair<kwi::status::Status, uint>> axis(const kwi::status::Status &s);
        std::vector<std::pair<kwi::status::Status, uint>> planar(const kwi::status::Status &s);
        std::vector<std::pair<kwi::status::Status, uint>> axisCoverage(const kwi::status::Status &s);
        std::vector<std::pair<kwi::status::Status, uint>> planarCoverage(const kwi::status::Status &s);
        std::vector<std::pair<kwi::status::Status, uint>> axisSimultaneous(const kwi::status::Status &s, int max_moves);
    }

    namespace status_move_cost {
        // Functions returning status, encoded move, and cost
        std::vector<std::tuple<kwi::status::Status, uint, uint>> axisTargetOnly(const kwi::status::Status &s);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> planarTargetOnly(const kwi::status::Status &s);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> axisFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> planarFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> axis(const kwi::status::Status &s);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> planar(const kwi::status::Status &s);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> axisCoverage(const kwi::status::Status &s);
        std::vector<std::tuple<kwi::status::Status, uint, uint>> planarCoverage(const kwi::status::Status &s);

        std::vector<std::tuple<kwi::status::Status, uint, uint>> planarFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
    }
}
