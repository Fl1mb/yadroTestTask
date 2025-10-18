#include <gtest/gtest.h>
#include "../src/MyTime.h"

TEST(TimeTests, timeTest){
    std::string t = "15:03";

    auto time = Time::fromString(t);

    EXPECT_EQ(time.hours, 15);
    EXPECT_EQ(time.minutes, 3);

    t = "08:05";

    time = Time::fromString(t);

    EXPECT_EQ(time.hours, 8);
    EXPECT_EQ(time.minutes, 5);
}
