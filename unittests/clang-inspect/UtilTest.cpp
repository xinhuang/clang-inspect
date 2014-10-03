#include "Util.h"

using namespace inspect;

#include "gtest/gtest.h"

TEST(test_split, given_empty_string_should_return_empty_vector) {
  const auto& actual = split("", ':');
  EXPECT_TRUE(0 == actual.size());
}

TEST(test_split,
     given_string_when_no_delimeter_should_return_vector_of_the_string) {
  const auto& actual = split("abc", ':');
  EXPECT_TRUE(1 == actual.size());
  EXPECT_TRUE("abc" == actual[0]);
}

TEST(test_split,
     given_string_when_one_delimeter_should_return_each_substring) {
  const auto& actual = split("a:bc:def", ':');
  EXPECT_TRUE(3 == actual.size());
  EXPECT_TRUE("a" == actual[0]);
  EXPECT_TRUE("bc" == actual[1]);
  EXPECT_TRUE("def" == actual[2]);
}

TEST(test_split,
     given_string_when_delimeter_at_both_ends_should_return_empty_substring) {
  const auto& actual = split(":a:", ':');
  EXPECT_EQ(2UL, actual.size());
  EXPECT_EQ("", actual[0]);
  EXPECT_EQ("a", actual[1]);
}

