#include "src/day_3/util.h"

#include <cctype>
#include <vector>

#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"

namespace day_3 {
namespace {

void AggregateNumber(const Pos &pos, const char new_digit,
                     const bool adj_to_symbol,
                     std::optional<GridNumber> &number) {
  int new_value = new_digit - '0';
  if (number.has_value()) {
    number->value = number->value * 10 + new_value;
    number->adjacent_to_symbol = number->adjacent_to_symbol || adj_to_symbol;
    number->tile_locations.push_back(pos);
  } else {
    number = GridNumber{new_value, adj_to_symbol, {pos}};
  }
}

bool IsSymbol(const char c) { return c != '.' && !std::isdigit(c); }

} // namespace

void Grid::VisitAdjacentTiles(
    const int x, const int y,
    std::function<void(char, const Pos &)> visit_fn) const {
  static constexpr std::array kOffsets = {Pos{-1, -1}, Pos{0, -1}, Pos{1, -1},
                                          Pos{-1, 0},  Pos{1, 0},  Pos{-1, 1},
                                          Pos{0, 1},   Pos{1, 1}};
  const Pos center{x, y};
  for (const auto &offset : kOffsets) {
    const Pos current = center + offset;
    if (!this->IsValid(current.x, current.y)) {
      continue;
    }
    visit_fn(this->at(current.x, current.y), current);
  }
}

bool Grid::IsAdjacentToSymbol(int x, int y) const {
  bool found_symbol = false;
  const auto visit_fn = [&](const char c, const Pos &) {
    found_symbol = found_symbol || IsSymbol(c);
  };
  this->VisitAdjacentTiles(x, y, visit_fn);
  return found_symbol;
}

void Grid::VisitNumbers(
    std::function<void(const GridNumber &)> visit_fn) const {
  for (int y = 0; y < this->height(); ++y) {
    std::optional<GridNumber> current_number;
    for (int x = 0; x < this->width(); ++x) {
      const char current_tile = this->at(x, y);
      if (std::isdigit(current_tile)) {
        AggregateNumber({x, y}, current_tile, this->IsAdjacentToSymbol(x, y),
                        current_number);
      } else if (current_number.has_value()) {
        visit_fn(*current_number);
        current_number = std::nullopt;
      }
    }
    if (current_number.has_value()) {
      visit_fn(*current_number);
      current_number = std::nullopt;
    }
  }
}

void Grid::VisitSymbol(const char symbol,
                       std::function<void(const Pos &)> visit_fn) const {
  for (int y = 0; y < this->height(); ++y) {
    for (int x = 0; x < this->width(); ++x) {
      if (this->at(x, y) == symbol) {
        visit_fn({x, y});
      }
    }
  }
}

} // namespace day_3
