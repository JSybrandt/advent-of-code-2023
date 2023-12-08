#ifndef SRC_DAY_3_UTIL_H_
#define SRC_DAY_3_UTIL_H_

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"

namespace day_3 {

struct Pos {
  int x, y;
  Pos operator+(const Pos &other) const {
    return {this->x + other.x, this->y + other.y};
  }
  bool operator==(const Pos &other) const {
    return this->x == other.x && this->y == other.y;
  }
  // Make this class hashable.
  template <typename H> friend H AbslHashValue(H h, const Pos &pos) {
    return H::combine(std::move(h), pos.x, pos.y);
  }
};

struct GridNumber {
  int value = 0;
  bool adjacent_to_symbol = false;
  std::vector<Pos> tile_locations;
};

class Grid {
public:
  explicit Grid(const std::vector<std::string> &data) : data_(data) {}

  int width() const { return data_[0].size(); }
  int height() const { return data_.size(); }

  char at(const int x, const int y) const { return data_[y][x]; }

  bool IsValid(const int x, const int y) const {
    return x >= 0 && x < width() && y >= 0 && y < height();
  }

  void VisitNumbers(std::function<void(const GridNumber &)> visit_fn) const;

  void
  VisitAdjacentTiles(int x, int y,
                     std::function<void(char, const Pos &)> visit_fn) const;

  void
  VisitAdjacentTiles(const Pos &pos,
                     std::function<void(char, const Pos &)> visit_fn) const {
    return VisitAdjacentTiles(pos.x, pos.y, visit_fn);
  }

  bool IsAdjacentToSymbol(int x, int y) const;

  void VisitSymbol(char symbol,
                   std::function<void(const Pos &)> visit_fn) const;

private:
  // Row wise data.
  const std::vector<std::string> data_;
};

} // namespace day_3

#endif // SRC_DAY_3_UTIL_H_
