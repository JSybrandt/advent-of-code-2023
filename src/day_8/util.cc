#include "src/day_8/util.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <math.h>
#include <optional>
#include <sstream>
#include <unistd.h>
#include <utility>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_replace.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_8 {

absl::StatusOr<Map> ParseMap(std::vector<std::string> edges) {
  Map result;
  for (const auto &edge : edges) {
    std::vector<std::string> tokens = absl::StrSplit(
        absl::StrReplaceAll(absl::StripAsciiWhitespace(edge),
                            {{" = ", " "}, {"(", ""}, {")", ""}, {",", ""}}),
        ' ');
    if (tokens.size() != 3) {
      return absl::InvalidArgumentError("Invalid edge: " + edge);
    }
    result[tokens[0]] = {.left = tokens[1], .right = tokens[2]};
  }
  return result;
}

Instructions ParseInstructions(absl::string_view serialized_directions) {
  Instructions result;
  for (const char d : serialized_directions) {
    switch (d) {
    case 'R':
      result.directions.push_back(Direction::RIGHT);
      break;
    case 'L':
      result.directions.push_back(Direction::LEFT);
      break;
    default:
      CHECK(false);
      break;
    }
  }
  return result;
}

std::string Traverse(const Map &map, absl::string_view current_position,
                     Direction direction) {
  switch (direction) {
  case Direction::LEFT:
    return map.at(current_position).left;
  case Direction::RIGHT:
    return map.at(current_position).right;
  }
  return "N/A";
}

} // namespace day_8
