#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
template <typename T> std::string FmtString(const std::string& fmt, T value);
std::string FloatToString(const std::string& fmt, float value);
};                                    // namespace Format

#endif