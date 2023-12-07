#include <iostream>
#include <string>

#include "absl/flags/flag.h"
#include "absl/flags/parse.h"

#include "src/day_2/util.h"

namespace day_2 {
absl::Status Main() {
  std::vector<Game> games;
  std::string serialized_game;
  while (std::getline(std::cin, serialized_game)) {
    absl::StatusOr<Game> game = ParseGame(serialized_game);
    if (!game.ok())
      return game.status();
    games.push_back(*game);
  }

  { // Part 1.
    CubeCounts bag_contents = {{"red", 12}, {"green", 13}, {"blue", 14}};
    size_t total = 0;
    for (const auto &game : games) {
      if (IsGamePossible(game, bag_contents)) {
        total += game.id;
      }
    }
    std::cout << "Total possible with example contents: " << total << std::endl;
  }

  { // Part 2.
    size_t total = 0;
    for (const auto &game : games) {
      total += ComputePower(game);
    }
    std::cout << "Sum of powers: " << total << std::endl;
  }

  return absl::OkStatus();
}
} // namespace day_2

int main(int argc, char **argv) {
  absl::ParseCommandLine(argc, argv);
  absl::Status result = day_2::Main();
  if (!result.ok()) {
    std::cerr << result << std::endl;
    return 1;
  }
  return 0;
}
