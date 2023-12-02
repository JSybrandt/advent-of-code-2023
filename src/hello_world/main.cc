#include "absl/status/status.h"
#include "absl/log/log.h"
#include "absl/log/check.h"
#include "absl/log/initialize.h"

#include "src/hello_world/util.h"

namespace hello_world {
namespace {

absl::Status Main(){
  LOG(INFO) << GetTheHelloWorldString();
  return absl::OkStatus();
}

} // namespace
} // namespace hello_world

int main() {
  absl::InitializeLog();
  QCHECK_OK(hello_world::Main());
  return 0;
}
