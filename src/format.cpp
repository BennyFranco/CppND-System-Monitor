#include "format.h"

#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long hour = seconds / 3600;
  seconds %= 3600;
  long minutes = seconds / 60;
  seconds %= 60;

  string sHour = FmtString("%02ld", hour);
  string sMinutes = FmtString("%02ld", minutes);
  string sSeconds = FmtString("%02ld", seconds);

  return string(sHour + ":" + sMinutes + ":" + sSeconds);
}

template <typename T>
string Format::FmtString(const std::string& fmt, T value) {
  auto size = std::to_string(value).size() + 1;
  char buffer[size];
  sprintf(buffer, fmt.c_str(), value);
  return string(buffer);
}

std::string Format::FloatToString(const string& fmt, float value) {
  return FmtString(fmt, value);
}
