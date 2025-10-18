#include <gtest/gtest.h>
#include "../src/Validator.h"

Validator validator(Time(10, 0), Time(19, 0), 15);

TEST(validatorTest, ValidateTime){
    Time time(9, 35);
    EXPECT_EQ(validator.validateTime(time), false);

    Time time2(10, 35);
    EXPECT_EQ(validator.validateTime(time2), true);
}

TEST(validatorTest, ValidateNick){
    std::string Nick1{"client_1"};
    EXPECT_EQ(validator.validateNick(Nick1), true);

    Nick1 = "clien_chert_%2";
    EXPECT_EQ(validator.validateNick(Nick1), false);
}

TEST(validatorTest, ValidateTable){
    int table = 4;
    EXPECT_EQ(validator.validateTable(table), true);

    EXPECT_EQ(validator.validateTable(18), false);
    EXPECT_EQ(validator.validateTable(-12), false);
}





