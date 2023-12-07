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

struct GridNumber {
  int value = 0;
  bool adjacent_to_symbol = false;
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

  void VisitAdjacentTiles(int x, int y,
                          std::function<void(char)> visit_fn) const;

  bool IsAdjacentToSymbol(int x, int y) const;

private:
  // Row wise data.
  const std::vector<std::string> data_;
};

} // namespace day_3

#endif // SRC_DAY_3_UTIL_H_
