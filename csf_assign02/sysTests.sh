#! /usr/bin/env bash

# System tests for postfix calculator (can test both C and asm versions)

# Check usage
if [ $# -ne 1 ]; then
	echo "Usage: ./sysTests.sh <exe name>"
	echo "  <exeName> should be './cPostfixCalc' or './asmPostfixCalc'"
	exit 1
fi

# Get the executable to test
exe="$1"

# Variables to keep track of test passed/attempted
numPassed=0
numAttempted=0

# Function testing that a postfix expression yields the expected result
expect() {
	local expected="$1"
	local expr="$2"

	if ./runTest.rb "$exe" "$expected" "$expr"; then
		numPassed=$(expr $numPassed + 1)
	fi
	numAttempted=$(expr $numAttempted + 1)
}

# Function testing that an invalid postfix expression yields an error
expect_error() {
	local expr="$1"

	if ./runTest.rb "$exe" "ERROR" "$expr"; then
		numPassed=$(expr $numPassed + 1)
	fi
	numAttempted=$(expr $numAttempted + 1)
}

#######################################################################
# Tests go here
#######################################################################

expect 5 '2 3 +'
expect 42 '6 7 *'
expect 42 '6 6 6 6 6 6 6 + + + + + +'
expect 6 '6'
expect 2 '   1    1    +    '
expect 4 '3 4-5+'
expect -305995602 '16 2 * 6 14 - - 2 * 3 + 1 16 / 15 20 + - 10 / 10 6 / 1 14 * + 14 6 * 19 7 * * * + * 14 14 / 18 + 2 13 12 * * + 20 16 5 * 19 - / * 14 6 / 4 12 / + 12 3 11 * / + 20 11 * 20 / 13 + - + *'
expect 60 '6 12 15 11 + + 2 3 * * - 2 9 4 + 16 + 9 11 / 15 13 * + + + + 2 2 - 14 9 3 - + + 18 - 8 20 + * +'
expect_error '2 2'
expect_error '1 *'
expect_error ''
expect_error ' '
# TODO: add more tests...

#######################################################################
# End of tests
#######################################################################

# Summarize results
echo "$numPassed/$numAttempted tests passed"
if [ $numPassed -eq $numAttempted ]; then
	exit 0
else
	exit 1
fi
