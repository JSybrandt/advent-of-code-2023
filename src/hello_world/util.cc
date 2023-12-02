#include "src/hello_world/util.h"

#include "absl/strings/string_view.h"

namespace hello_world {

absl::string_view GetTheHelloWorldString(){
  return "hello world!";
}

} // hello_world
