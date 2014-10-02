#pragma once

#include <string>
#include <algorithm>

namespace inspect {

template <typename T, typename U>
bool contains(const T &container, const U &value) {
  return std::find(std::begin(container), std::end(container), value) !=
         std::end(container);
}

std::vector<std::string> split(const std::string& s, char deli);

}


