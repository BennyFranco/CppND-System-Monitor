#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
template <typename T> std::string Format(const std::string& fmt, T value);
};                                    // namespace Format

#endif