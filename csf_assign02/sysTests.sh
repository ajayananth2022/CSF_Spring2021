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


#Ruby generated tests: ALL VALID
expect -16444845 '5 8 8 10 * 6 12 - + * 20 9 / 14 18 * - 14 1 * 14 10 - + * * 16 1 13 - * 15 7 * 14 17 + * * 9 - + *'
expect 0 '6 16 6 / + 6 12 - 17 19 + * * 12 18 7 18 * + * + 17 20 2 10 * 5 16 / - * - * 16 5 - 19 6 / + 13 10 - 8 13 + + * 12 4 + 10 3 + - 9 2 + 20 16 / / + / 9 + *'
expect 2 '19 11 * 7 7 * - 14 11 + 1 * + 15 18 + 19 3 / - 10 20 + * * 4 5 7 + - 3 3 / 4 11 + + + 13 * + 12 13 15 6 + 2 15 * * + * 9 * /'
expect -292632 '12 7 - 20 4 * * 7 * 11 1 - 12 / 16 + - 8 6 * 17 - / 18 1 * 17 13 * 4 - 12 15 * 3 3 - + + 19 15 * 13 * 10 7 / 1 3 * - + - + *'
expect 188317584 '13 16 17 * 1 7 - - 19 10 + 12 11 * * + 12 13 5 / 12 9 / * + * * 14 18 * *'

#testing of INVALID expressions
expect_error '2 2'
expect_error '1 *'
expect_error ''
expect_error ' '

#full stack error
expect_error '1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23'

#division by 0 error
expect_error '3 0 /'

#division by 0 error on longer operation
expect_error '2 3 + 0/'

#expression with only operator error
expect_error '* '

#invalid character error
expect_error '4 33 / attic '
expect_error '4 33 ? '


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
