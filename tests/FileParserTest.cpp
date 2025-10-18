#include <gtest/gtest.h>
#include "../src/FileParser.h"

static const char* test_file = "parser_test.txt";

TEST(FileParserTest, ParseFileSuccess){
    FileParser parser(test_file);
    EXPECT_NO_THROW(parser.ParseFile());

    EXPECT_EQ(parser.getTablesCount(), 3);
    EXPECT_EQ(parser.getStartTime(), Time(9, 0));
    EXPECT_EQ(parser.getEndTime(), Time(19,0));
    EXPECT_EQ(parser.getHourlyRate(), 10);
    EXPECT_EQ(parser.getEvents().size(), 14);
    
    auto example = parser.getEvents()[5];

    EXPECT_EQ(static_cast<int>(example.id), 2);
    EXPECT_EQ(example.time, Time(10, 25));
    EXPECT_EQ(example.body, "client2 2");
}