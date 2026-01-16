#pragma once

#include "../status/status.h"

namespace kwi::heuristic {

/**
 * @brief Manahattan distance heuristic
 *
 * @param s Parking status
 * @param dest
 * @return unsigned int
 */
unsigned int manhattan(const status::Status &s, const std::array<unsigned int, 2> &dest);

/**
 * @brief Manahattan distance with multiple destinations (overload)
 *
 * @param s Parking status
 * @param dest Destination coordinates
 * @return unsigned int, distance to closest destination
 */
unsigned int manhattan(const status::Status &s, const std::vector<std::array<unsigned int, 2>> &dest);

/**
 * @brief Octile distance heuristic
 *
 * @param s Parking status
 * @param dest Destination coordinates
 * @return unsigned int
 */
unsigned int octile(const status::Status &s, const std::array<unsigned int, 2> &dest);

/**
 * @brief Octile distance heuristic with multiple destinations (overload)
 *
 * @param s Parking status
 * @param dest Destination coordinates
 * @return unsigned int, distance to closest destination
 */
unsigned int octile(const status::Status &s, const std::vector<std::array<unsigned int, 2>> &dest);

/**
 * @brief Chebyshev distance heuristic
 *
 * @param s Parking status
 * @param dest Destination coordinates
 * @return unsigned int
 */
unsigned int chebyshev(const status::Status &s, const std::array<unsigned int, 2> &dest);

/**
 * @brief Chebyshev distance heuristic with multiple destinations (overload)
 *
 * @param s Parking status
 * @param dest Destination coordinates
 * @return unsigned int, distance to closest destination
 */
unsigned int chebyshev(const status::Status &s, const std::vector<std::array<unsigned int, 2>> &dest);

/**
 * @brief Manahattan distance with extra heuristic cost consireding occupied positions between target and destination
 *
 * @param s Parking status
 * @param dest Destination coordinates
 * @return unsigned int
 */
unsigned int manhattanOccupancyVariant(const status::Status &s, const std::array<unsigned int, 2> &dest);

/**
 * @brief Manahattan distance with extra heuristic cost consireding occupied positions between target and destination:
 * multiple destinations (overload)
 *
 * @param s
 * @param dest
 * @return unsigned int, distance to closest destination
 */
unsigned int manhattanOccupancyVariant(const status::Status &s, const std::vector<std::array<unsigned int, 2>> &dest);

} // namespace kwi::heuristic