#pragma once

#include "../status/status.h"
#include <array>

namespace kwi::is_final {

/**
 * @brief Is target on destinatation: one destination
 *
 * @param s  Parking status
 * @param dest Destination coordinates
 * @return true
 * @return false
 */
bool isTargetOnDestination(const kwi::status::Status &s, std::array<unsigned int, 2> &dest);

/**
 * @brief Is target on destinatation: multiple destinations (overload)
 *
 * @param s  Parking status
 * @param dest Destination coordinates
 * @return true
 * @return false
 */
bool isTargetOnDestination(const kwi::status::Status &s, const std::vector<std::array<unsigned int, 2>> &dest);

} // namespace kwi::is_final