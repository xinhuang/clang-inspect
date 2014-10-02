#include "Util.h"

#include <iostream>
using namespace std;

namespace inspect {

std::vector<std::string> split(const std::string& s, char deli) {
  std::vector<std::string> result;
  auto prev = std::begin(s);
  auto pivot = prev;
  while (pivot != std::end(s)) {
    pivot = std::find(pivot, std::end(s), deli);
    if (pivot == std::end(s))
      break;
    result.emplace_back(prev, pivot);
    prev = ++pivot;
  }
  if (prev != std::end(s)) 
    result.emplace_back(prev, std::end(s));
  return result;
}

}
