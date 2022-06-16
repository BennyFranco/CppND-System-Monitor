#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
template <typename T>
std::string FmtString(const std::string& fmt, T value);
std::string FormatFloat(const std::string& fmt, float value);
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
std::string Truncate(const std::string& str, unsigned long maxWidth,
                     bool showEllipsis = true);
};  // namespace Format

#endif