#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' 

EXECUTABLE="./out/bin/computer_club"
TEST_FILE="./big_test.txt"
EXPECTED_FILE="./expected_output.txt"
ACTUAL_FILE="./actual_output.txt"

echo -e "${YELLOW}=== Computer Club Test Runner ===${NC}"
echo ""

if [ ! -f "$EXECUTABLE" ]; then
    echo -e "${RED}Error: Executable '$EXECUTABLE' not found${NC}"
    echo "Please build the project first:"
    echo "  mkdir build && cd build && cmake .. && make"
    exit 1
fi

if [ ! -f "$TEST_FILE" ]; then
    echo -e "${RED}Error: Test file '$TEST_FILE' not found${NC}"
    exit 1
fi

if [ ! -f "$EXPECTED_FILE" ]; then
    echo -e "${RED}Error: Expected output file '$EXPECTED_FILE' not found${NC}"
    exit 1
fi

echo -e "${YELLOW}Running test: $TEST_FILE${NC}"
echo ""

echo "Command: $EXECUTABLE $TEST_FILE"
echo ""

$EXECUTABLE "$TEST_FILE" > "$ACTUAL_FILE"

if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Program execution failed${NC}"
    exit 1
fi

echo -e "${YELLOW}Comparing results...${NC}"
echo ""

if diff -q "$ACTUAL_FILE" "$EXPECTED_FILE" > /dev/null; then
    echo -e "${GREEN}✅ TEST PASSED! Output matches expected result.${NC}"
    echo ""
    
    # Show line count for verification
    actual_lines=$(wc -l < "$ACTUAL_FILE")
    expected_lines=$(wc -l < "$EXPECTED_FILE")
    echo "Output lines: $actual_lines"
    echo "Expected lines: $expected_lines"
    
    # Clean up
    rm -f "$ACTUAL_FILE"
    
else
    echo -e "${RED}❌ TEST FAILED! Output differs from expected result.${NC}"
    echo ""
    
    # Show diff details
    echo -e "${YELLOW}Differences found:${NC}"
    diff --color=always -u "$EXPECTED_FILE" "$ACTUAL_FILE"
    
    echo ""
    echo -e "${YELLOW}Line counts:${NC}"
    echo "Expected: $(wc -l < "$EXPECTED_FILE") lines"
    echo "Actual:   $(wc -l < "$ACTUAL_FILE") lines"
    
    echo ""
    echo -e "${YELLOW}Files saved for inspection:${NC}"
    echo "Actual output: $ACTUAL_FILE"
    echo "Expected output: $EXPECTED_FILE"
    
    exit 1
fi