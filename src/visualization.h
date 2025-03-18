#pragma once

#include <string>

#include "status.h"

namespace kwi::visualization {

void play(const std::vector<kwi::status::Status> &history, 
        const uint x_size, const uint y_size, int delay_milliseconds=1000);

void play(const kwi::status::Status &start, const std::vector<uint> &moves, 
    const uint x_size, const uint y_size, int delay_milliseconds=1000);

void print(const std::vector<kwi::status::Status> &history, 
    const uint x_size, const uint y_size);

void print(const kwi::status::Status &start, const std::vector<uint> &moves, 
    const uint x_size, const uint y_size);

void save_to_file(const std::vector<kwi::status::Status> &history, 
    const uint x_size, const uint y_size, std::string& filename);

void save_to_file(const kwi::status::Status &start, const std::vector<uint> &moves, 
    const uint x_size, const uint y_size, std::string& filename);

}