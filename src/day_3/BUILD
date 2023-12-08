cc_library(
  name="util",
  hdrs=["util.h"],
  srcs=["util.cc"],
  deps=[
    "@abseil-cpp//absl/strings",
    "@abseil-cpp//absl/status:statusor",
    "@abseil-cpp//absl/status:status",
    "@abseil-cpp//absl/container:flat_hash_map",
  ],
)

cc_test(
  name="util_test",
  srcs=["util_test.cc"],
  deps=[
    ":util",
    "@googletest//:gtest",
    "@googletest//:gtest_main",
    "@abseil-cpp//absl/container:flat_hash_set",
    "@abseil-cpp//absl/container:flat_hash_map",
  ],
)

cc_binary(
  name="main",
  srcs=["main.cc"],
  deps=[
    ":util",
    "@abseil-cpp//absl/flags:flag",
    "@abseil-cpp//absl/container:flat_hash_set",
    "@abseil-cpp//absl/flags:parse",
  ],
)

