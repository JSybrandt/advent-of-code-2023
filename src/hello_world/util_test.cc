#include "gtest/gtest.h"

#include "src/hello_world/util.h"

TEST(HelloWorldUtilTest, ProducesExpectedString){
  EXPECT_EQ(hello_world::GetTheHelloWorldString(), "hello world!");
}
