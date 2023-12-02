#include <iostream>
#include <string>

#include "absl/status/status.h"
#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/log/initialize.h"

#include "src/day_1/util.h"

int main() {
  int64_t total = 0;
  std::string line;
  while(std::getline(std::cin, line)){
    int number = *day_1::GetEncodedNumber(line);
    total += number;
  }
  std::cout << total << std::endl;
  return 0;
}
