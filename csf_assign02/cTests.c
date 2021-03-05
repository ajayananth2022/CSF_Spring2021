/*
 * Unit tests for C version of postfix calculator
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tctest.h"
#include "cPostfixCalc.h"

typedef struct {
	/* TODO: add fields for test fixture */

	/* representation of operand stack */
	long values[MAX_STACK];
	long count;
} TestObjs;

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));

	/* TODO: initialize test fixture */

	/* stack is initially empty */
	objs->count = 0;

	return objs;
}

void cleanup(TestObjs *objs) {
	/* TODO: do any cleanup needed for test fixture */

	free(objs);
}

/*
 * Prototypes for test functions
 */
void testAddPositive(TestObjs *obj);         /* example test function */
void testAddPositiveInvalid(TestObjs *obj);  /* example test function */
void testIsSpace(TestObjs *objs);
void testIsDigit(TestObjs *objs);
void testSkipws(TestObjs *objs);
void testTokenType(TestObjs *objs);
void testConsumeInt(TestObjs *objs);
void testConsumeOp(TestObjs *objs);
void testPush(TestObjs *objs);
void testPushFull(TestObjs *objs);
void testPop(TestObjs *objs);
void testEvalOp(TestObjs *objs);
void testEval(TestObjs *objs);
void testEvalInvalid(TestObjs *objs);

/* set to nonzero if a call to exit is expected */
int expectedExit;
/* jump buffer that our version of exit can use to jump back to test function */
sigjmp_buf exitBuf;

/*
 * Custom version of exit: useful for testing functions where
 * the expected behavior is a call to exit (e.g., because
 * an argument value is invalid.)  If the expectedExit
 * variable is set to a nonzero value, uses siglongjmp (via
 * exitBuf) to return control to the test function.
 * If expectedExit is not set, immediately fails the
 * current test.
 */
void exit(int exitCode) {
	if (expectedExit) {
		/* jump back to test function */
		siglongjmp(exitBuf, 1);
	} else {
		/* exit called unexpectedly, fail the test */
		FAIL("Unexpected exit");
	}
}

void on_complete(int numPassed, int numExecuted) {
	printf("%d/%d tests passed\n", numPassed, numExecuted);
}

/*
 * Main function.
 * If no command line arguments are passed,
 * executes all tests.  If a single command line
 * argument is passed, only the test with that
 * name is executed.
 */
int main(int argc, char **argv) {
	if (argc > 2) {
		printf("Usage: %s [<test name>]\n", argv[0]);
		return 1;
	}

	if (argc == 2) {
		tctest_testname_to_execute = argv[1];
	}

	tctest_on_complete = on_complete;

	TEST_INIT();

	TEST(testAddPositive);
	TEST(testAddPositiveInvalid);
	TEST(testIsSpace);
	TEST(testIsDigit);
	TEST(testSkipws);
	TEST(testTokenType);
	TEST(testConsumeInt);
	TEST(testConsumeOp);
	TEST(testPush);
	TEST(testPushFull);
	TEST(testPop);
	TEST(testEvalOp);
	TEST(testEval);
	TEST(testEvalInvalid);

	TEST_FINI();
}

/*
 * Example test function.
 */
void testAddPositive(TestObjs *obj) {
	ASSERT(2L == addPositive(1L, 1L));
	ASSERT(23L == addPositive(15L, 8L));
}

/*
 * Example of a test function that verifies that an invalid
 * input results in a call to the exit function.
 */
void testAddPositiveInvalid(TestObjs *obj) {
	/*
	 * addPositive should call exit if either of its arguments is negative
	 */

	expectedExit = 1;

	if (sigsetjmp(exitBuf, 1) == 0) {
		addPositive(-1L, 1L);
		FAIL("addPositive with invalid first argument did not exit");
	} else {
		printf("addPositive exited, good...");
	}

	if (sigsetjmp(exitBuf, 1) == 0) {
		addPositive(1L, -1L);
		FAIL("addPositive with invalid second argument did not exit");
	} else {
		printf("addPositive exited, good...");
	}
}

void testIsSpace(TestObjs *objs) {
	ASSERT(isSpace(' '));
	ASSERT(isSpace('\t'));
	ASSERT(!isSpace('0'));
	ASSERT(!isSpace('3'));
	ASSERT(!isSpace('+'));
	ASSERT(!isSpace('a'));
	ASSERT(!isSpace('!'));
	ASSERT(!isSpace('"'));
	ASSERT(!isSpace(8));
	ASSERT(!isSpace(10));

}

void testIsDigit(TestObjs *objs) {
	ASSERT(isDigit('0'));
	ASSERT(isDigit('1'));
	ASSERT(isDigit('2'));
	ASSERT(isDigit('3'));
	ASSERT(isDigit('4'));
	ASSERT(isDigit('5'));
	ASSERT(isDigit('6'));
	ASSERT(isDigit('7'));
	ASSERT(isDigit('8'));
	ASSERT(isDigit('9'));
	ASSERT(!isDigit(':'));
	ASSERT(!isDigit(';'));
	ASSERT(!isDigit('/'));
	ASSERT(!isDigit(' '));
	ASSERT(!isDigit('\t'));
	ASSERT(!isDigit('a'));
}

void testSkipws(TestObjs *objs) {
	ASSERT(0 == strcmp("abc", skipws("abc")));
	ASSERT(0 == strcmp("abc", skipws(" abc")));
	ASSERT(0 == strcmp("abc", skipws("\tabc")));
	ASSERT(0 == strcmp("abc", skipws("     abc")));
	ASSERT(0 == strcmp("abc", skipws("\t\t\t\tabc")));
	ASSERT(0 == strcmp("abc", skipws(" \tabc")));
	ASSERT(0 == strcmp("abc  ", skipws(" \t abc  ")));
	ASSERT(NULL == skipws("   "));
	ASSERT(NULL == skipws("   \t\t \t"));
}

void testTokenType(TestObjs *objs) {
	ASSERT(TOK_INT == tokenType("2 3 +"));
	ASSERT(TOK_INT == tokenType("123 4 +"));
	ASSERT(TOK_INT == tokenType("0123 4 +"));
	ASSERT(TOK_OP == tokenType("+ 2 3"));
	ASSERT(TOK_OP == tokenType("- 2 3"));
	ASSERT(TOK_OP == tokenType("* 2 3"));
	ASSERT(TOK_OP == tokenType("/ 2 3"));
	ASSERT(TOK_UNKNOWN == tokenType("abc"));
	ASSERT(TOK_UNKNOWN == tokenType("ima mystery boooo"));
	ASSERT(TOK_UNKNOWN == tokenType("?"));
	ASSERT(TOK_UNKNOWN == tokenType(" "));
}

void testConsumeInt(TestObjs *objs) {
	long val;
	ASSERT(0 == strcmp(" 3 +", consumeInt("2 3 +", &val)));
	ASSERT(2L == val);
	ASSERT(0 == strcmp(" 456 -", consumeInt("123 456 -", &val)));
	ASSERT(123L == val);
	ASSERT(0 == strcmp(" 456 -", consumeInt("0123 456 -", &val)));
	ASSERT(123L == val);
	ASSERT(0 == strcmp(" 0456 -", consumeInt("0123456 0456 -", &val)));
	ASSERT(123456L == val);
	ASSERT(0 == strcmp(" 0456 -", consumeInt("012345678 0456 -", &val)));
	ASSERT(12345678L == val);
	ASSERT(0 == strcmp(" 0456 -", consumeInt("0 0456 -", &val)));
	ASSERT(0L == val);
}

void testConsumeOp(TestObjs *objs) {
	int op;

	ASSERT(0 == strcmp(" 3", consumeOp("+ 3", &op)));
	ASSERT('+' == op);
	ASSERT(0 == strcmp(" 3", consumeOp("- 3", &op)));
	ASSERT('-' == op);
	ASSERT(0 == strcmp(" 3", consumeOp("* 3", &op)));
	ASSERT('*' == op);
	ASSERT(0 == strcmp(" 3", consumeOp("/ 3", &op)));
	ASSERT('/' == op);
	ASSERT(0 == strcmp("3", consumeOp("/3", &op)));
	ASSERT('/' == op);
	ASSERT(0 == strcmp("33 4 12 /   ", consumeOp("*33 4 12 /   ", &op)));
	ASSERT('*' == op);
}

void testPush(TestObjs *objs) {
	ASSERT(0 == objs->count);
	stackPush(objs->values, &objs->count, 123L);
	ASSERT(1 == objs->count);
	ASSERT(123L == objs->values[0]);
	stackPush(objs->values, &objs->count, 456L);
	ASSERT(2 == objs->count);
	ASSERT(456L == objs->values[1]);
	stackPush(objs->values, &objs->count, 789L);
	ASSERT(3 == objs->count);
	ASSERT(789L == objs->values[2]);
	stackPush(objs->values, &objs->count, 1234567L);
	ASSERT(4 == objs->count);
	ASSERT(1234567L == objs->values[3]);
}

void testPushFull(TestObjs *objs) {
	/* push maximum number of values onto stack */
	for (long i = 1L; i <= MAX_STACK; i++) {
		stackPush(objs->values, &objs->count, i);
	}

	/* pushing another item should cause a fatal error */
	expectedExit = 1;
	if (sigsetjmp(exitBuf, 1) == 0) {
		stackPush(objs->values, &objs->count, 999L);
		FAIL("pushing onto full stack didn't fail");
	} else {
		/* good, push failed */
		printf("push onto full stack failed, good...");
	}
}

void testPop(TestObjs *objs) {
	stackPush(objs->values, &objs->count, 1L);
	stackPush(objs->values, &objs->count, 2L);
	stackPush(objs->values, &objs->count, 3L);
	stackPush(objs->values, &objs->count, 4L);
	stackPush(objs->values, &objs->count, 5L);

	ASSERT(5L == stackPop(objs->values, &objs->count));
	ASSERT(4L == stackPop(objs->values, &objs->count));
	ASSERT(3L == stackPop(objs->values, &objs->count));
	ASSERT(2L == stackPop(objs->values, &objs->count));
	ASSERT(1L == stackPop(objs->values, &objs->count));
	ASSERT(0 == objs->count);

	/* pop another item should cause a fatal error */
	expectedExit = 1;
	if (sigsetjmp(exitBuf, 1) == 0) {
		stackPop(objs->values, &objs->count);
		FAIL("pop from empty stack didn't fail");
	} else {
		/* GREAT, pop failed */
		printf("pop from empty stack failed, good..");
	}
}

void testEvalOp(TestObjs *objs) {
	ASSERT(3L == evalOp('+', 1L, 2L));
	ASSERT(-10L == evalOp('-', 3L, 13L));
	ASSERT(77L == evalOp('*', 11L, 7L));
	ASSERT(3 == evalOp('/', 17L, 5L));
	ASSERT(10L == evalOp('-', 13L, 3L));
	ASSERT(0L == evalOp('*', 0L, 7L));
	ASSERT(0L == evalOp('*', 0L, 0L));
	ASSERT(0L == evalOp('+', 0L, 0L));
	ASSERT(0L == evalOp('-', 0L, 0L));

	//NOTE: division by 0 is tested in testEvalInvalid
}

void testEval(TestObjs *objs) {
	ASSERT(1L == eval("1"));
	ASSERT(2L == eval("1 1 +"));
	ASSERT(3L == eval("4 1 -"));
	ASSERT(33L == eval("11 3 *"));
	ASSERT(27L == eval("3 4 5 + *"));
	ASSERT(0 == eval("3 10 /"));
	ASSERT(-12 == eval("2 3 4 5 +-*"));

	//ruby generated tests
	ASSERT(489 == eval("14 2 16 - 12 + 9 11 / 7 13 + + + 7 18 12 + 16 3 + - * + 5 * +"));
	ASSERT(-15 == eval("19 4 10 + 7 10 + - 4 6 - 1 11 / - + - 6 13 9 6 + 17 5 * / + + - 20 -"));
	ASSERT(-168553 == eval("11 20 * 16 * 2 12 15 * - / 6 12 + 18 18 * * 13 17 5 - - + * 5 * 15 12 + 4 13 + + 9 13 - / 16 1 / 2 13 * + 18 15 17 * - + + 7 18 11 * * 13 14 7 + * * 19 14 1 + * 20 9 - 15 + * + + +"));
	ASSERT(958 == eval("19 6 * 8 1 * + 16 12 + 14 - + 4 20 / 3 6 / * 8 + * 1 2 - 15 10 * + 3 - 17 14 + 9 / 6 7 1 / - - - - 12 +"));
	ASSERT(0 == eval("14 10 1 * * 6 9 2 + 20 8 * * 1 9 * 16 20 + - 7 - + + /"));

	/* make sure eval can handle arbitrary whitespace */
	ASSERT(6L == eval("  1  \t 5\t\t + \t"));
}

void testEvalInvalid(TestObjs *objs) {
	expectedExit = 1;

	/* multiple items left on stack */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval("2 2");
		FAIL("multiple items on stack not handled");
	} else {
		/* good, expected failure */
		printf("good, multiple items on stack handled...");
	}

	/* multiple items left on stack */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval("* ");
		FAIL("expresison with beginning operator not resulting in fail");
	} else {
		/* good, expected failure */
		printf("good, expresison with beginning operator handled...");
	}

	/* operator with insufficient operands */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval("1 +");
		FAIL("stack underflow not handled");
	} else {
		/* good, expected failure */
		printf("good, stack underflow handled...");
	}

	/* division by 0 should cause a fatal error */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval("17 0/"); 
		FAIL("division by 0 didn't fail");
	} else {
		/* good, push failed */
		printf("division by 0 failed, good...");
	}

	/* division by 0 inside larger postfix should cause a fatal error */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval("2 3 + 0/"); 
		FAIL("division by 0 inside larger postfix didn't fail");
	} else {
		/* good, push failed */
		printf("division by 0 inside larger postfix failed, good...");
	}

	/* empty postfix string should cause a fatal error */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval(""); 
		FAIL("empty string by 0 didn't fail");
	} else {
		/* good, push failed */
		printf("empty string failed, good...");
	}

	/* postfix string with only space should cause a fatal error */
	if (sigsetjmp(exitBuf, 1) == 0) {
		eval(" "); 
		FAIL("postfix string with only space didn't fail");
	} else {
		/* good, push failed */
		printf("postfix string with only space failed, good...");
	}
}
