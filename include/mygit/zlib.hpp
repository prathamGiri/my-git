#pragma once

#include <string>

std::string zcompress(std::string &in, int level);
std::string zdecompress(const std::string& in);