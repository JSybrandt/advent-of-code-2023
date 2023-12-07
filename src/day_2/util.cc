#include "src/day_2/util.h"

#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_2 {

absl::StatusOr<Game> ParseGame(absl::string_view serialized_game) {
  Game result;
  const auto invalid_error = [&]() {
    return absl::InvalidArgumentError(
        absl::StrCat("Invalid game: ", serialized_game));
  };

  std::vector<std::string> game_and_handfulls =
      absl::StrSplit(serialized_game, ':');
  if (game_and_handfulls.size() != 2) {
    return invalid_error();
  }
  std::string serialized_game_and_id = game_and_handfulls[0];
  std::string serialized_handfulls = game_and_handfulls[1];

  std::vector<std::string> game_and_id =
      absl::StrSplit(serialized_game_and_id, ' ');
  if (game_and_id.size() != 2) {
    return invalid_error();
  }
  if (!absl::SimpleAtoi(game_and_id[1], &result.id)) {
    return invalid_error();
  }

  for (const auto serialized_handful :
       absl::StrSplit(serialized_handfulls, ';')) {
    auto &handful = result.handfulls.emplace_back();
    for (const auto &serialized_cube_count :
         absl::StrSplit(serialized_handful, ',')) {
      std::vector<std::string> count_and_color = absl::StrSplit(
          absl::StripAsciiWhitespace(serialized_cube_count), ' ');
      if (count_and_color.size() != 2) {
        return invalid_error();
      }
      size_t count;
      if (!absl::SimpleAtoi(count_and_color[0], &count)) {
        return invalid_error();
      }
      handful[count_and_color[1]] = count;
    }
  }
  return result;
}

bool IsGamePossible(const Game &game, const CubeCounts &bag_contents) {
  for (const auto &handful : game.handfulls) {
    if (!IsHandfulPossible(handful, bag_contents)) {
      return false;
    }
  }
  return true;
}

bool IsHandfulPossible(const CubeCounts &handful,
                       const CubeCounts &bag_contents) {
  for (const auto &[color, num_cubes] : handful) {
    if (num_cubes == 0)
      continue;
    if (!bag_contents.contains(color) || bag_contents.at(color) < num_cubes)
      return false;
  }
  return true;
}

size_t ComputePower(const Game &game) {
  CubeCounts min_counts;
  for (const auto &handful : game.handfulls) {
    for (const auto &[color, num_cubes] : handful) {
      if (!min_counts.contains(color) || min_counts.at(color) < num_cubes) {
        min_counts[color] = num_cubes;
      }
    }
  }
  size_t power_total = 1;
  for (const auto &[_, num_cubes] : min_counts) {
    power_total *= num_cubes;
  }
  return power_total;
}

} // namespace day_2
