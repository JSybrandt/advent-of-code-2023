cc_library(
  name="util",
  hdrs=["util.h"],
  srcs=["util.cc"],
  deps=["@abseil-cpp//absl/strings"],
)

cc_test(
  name="util_test",
  srcs=["util_test.cc"],
  deps=[
    ":util",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
  ],
)

cc_binary(
  name="main",
  srcs=["main.cc"],
  deps=[
    ":util",
    "@abseil-cpp//absl/status:status",
    "@abseil-cpp//absl/log:log",
    "@abseil-cpp//absl/log:check",
    "@abseil-cpp//absl/log:initialize",
  ],
)

