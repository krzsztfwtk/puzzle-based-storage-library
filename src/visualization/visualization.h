#pragma once

#include <string>
#include <vector>

#include "../status/status.h"
#include "../types.h"

namespace kwi::visualization {

/**
 * @brief Play path visualization in CLI
 *
 * @param history Parking states path
 * @param delay_milliseconds
 */
void play(const std::vector<kwi::status::Status> &history, int delay_milliseconds = 1000);

/**
 * @brief Play path visualization in CLI
 *
 * @param start Initial parking status
 * @param moves Sequence of moves
 * @param delay_milliseconds
 */
void play(const kwi::status::Status &start, const std::vector<Move> &moves, int delay_milliseconds = 1000);

/**
 * @brief Print path visualization in CLI
 *
 * @param history Parking states path
 */
void print(const std::vector<kwi::status::Status> &history);

/**
 * @brief Print path visualization in CLI
 *
 * @param start Initial parking status
 * @param moves Sequence of moves
 * @param delay_milliseconds
 */
void print(const kwi::status::Status &start, const std::vector<Move> &moves);

/**
 * @brief Save path visualization to a file
 *
 * @param history Parking states path
 * @param filename
 */
void save_to_file(const std::vector<kwi::status::Status> &history, const std::string &filename);

/**
 * @brief Save path visualization to a file
 *
 * @param start Initial parking status
 * @param moves Sequence of moves
 * @param filename
 */
void save_to_file(const kwi::status::Status &start, const std::vector<Move> &moves, const std::string &filename);

} // namespace kwi::visualization