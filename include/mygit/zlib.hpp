#pragma once

#include <string>

std::string zcompress(const std::string &in, int level = 6);
std::string zdecompress(const std::string& in);