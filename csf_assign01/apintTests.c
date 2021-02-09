/*
 * Unit tests for arbitrary-precision integer data type
 *
 * These tests are by no means comprehensive.  You will need to
 * add more tests of your own!  In particular, make sure that
 * you have tests for more challenging situations, such as
 *
 * - large values
 * - adding/subtracting/comparing values with different lengths
 * - special cases (carries when adding, borrows when subtracting, etc.)
 * - etc.
 * Lucy Zhang, Ajay Ananthakrishnan
 * wzhang99@jhu.edu, ajayananth@jhu.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apint.h"
#include "tctest.h"

typedef struct {
	ApInt *ap0;
	ApInt *ap1;
	ApInt *ap110660361;
	ApInt *max1;
	ApInt *minus1;
	ApInt *minusMax;
	ApInt *ap100000000;
	ApInt *ap0xbc848afUL;
	ApInt *ap2;
	ApInt *minus2;
	ApInt *ap7;
	ApInt *ap8;
	ApInt *ap15;
	ApInt *ap16;
	ApInt *apF;
	ApInt *apFFFFFFFFFFFFFFFF;
	ApInt *apFFFFFFFFFFFFFFFE;
	ApInt *ap10000000000000000;
	ApInt *minus10000000000000000; 
	ApInt *ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9;
	ApInt *ap7e5ff912c8ede6ccff0d56ae5a9b5459804f8; 
	ApInt *ap000962d7e839ed2d377;
	ApInt *minus962d7e839ed2d377;
	/* TODO: add additional fields of test fixture */
} TestObjs;

TestObjs *setup(void);
void cleanup(TestObjs *objs);

void testCreateFromU64(TestObjs *objs);
void testIsZero(TestObjs *objs);
void testIsNegative(TestObjs *objs);
void testHighestBitSet(TestObjs *objs);
void testCompare(TestObjs *objs);
void testFormatAsHex(TestObjs *objs);
void testAdd(TestObjs *objs);
void testSub(TestObjs *objs);
void testUnsignedCompare(TestObjs *objs);
/* TODO: add more test function prototypes */

int main(int argc, char **argv) {
	TEST_INIT();

	if (argc > 1) {
		/*
		 * name of specific test case to execute was provided
		 * as a command line argument
		 */
		tctest_testname_to_execute = argv[1];
	}

	TEST(testCreateFromU64);
	TEST(testIsZero);
	TEST(testIsNegative);
	TEST(testHighestBitSet);
	TEST(testCompare);
	TEST(testFormatAsHex);
	TEST(testAdd);
	TEST(testSub);
	TEST(testUnsignedCompare);
	/* TODO: use TEST macro to execute more test functions */

	TEST_FINI();
}

TestObjs *setup(void) {
	TestObjs *objs = malloc(sizeof(TestObjs));
	objs->ap0 = apint_create_from_hex("-000");
	objs->ap1 = apint_create_from_hex("0001");
	objs->ap110660361 = apint_create_from_u64(110660361UL);
	objs->ap100000000 = apint_create_from_u64(100000000UL);
	objs->ap0xbc848afUL = apint_create_from_u64(0xbc848afUL);
	objs->max1 = apint_create_from_u64(0xFFFFFFFFFFFFFFFFUL);
	objs->minus1 = apint_negate(objs->ap1);
	objs->minusMax = apint_negate(objs->max1);
	objs->ap2 = apint_create_from_hex("2");
	objs->minus2 = apint_negate(objs->ap2);
	objs->ap8 = apint_create_from_hex("8");
	objs->ap7 = apint_create_from_hex("7");
	objs->ap15 = apint_create_from_u64(15UL);
	objs->ap16 = apint_create_from_u64(16UL);
	objs->apF = apint_create_from_hex("F");
	objs->apFFFFFFFFFFFFFFFF = apint_create_from_hex("FFFFFFFFFFFFFFFF");
	objs->apFFFFFFFFFFFFFFFE = apint_create_from_hex("FFFFFFFFFFFFFFFE");
	objs->ap10000000000000000 = apint_create_from_hex("10000000000000000");
	objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9 = apint_create_from_hex("7e5ff912c8ede6ccff0d56ae5a9b5459804f9");
	objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f8 = apint_create_from_hex("7e5ff912c8ede6ccff0d56ae5a9b5459804f8");
	objs->ap000962d7e839ed2d377 = apint_create_from_hex("000962d7e839ed2d377");
	objs->minus962d7e839ed2d377 = apint_create_from_hex("-962d7e839ed2d377");
	objs->minus10000000000000000 = apint_create_from_hex("-10000000000000000");

	/* TODO: initialize additional members of test fixture */

	return objs;
}

void cleanup(TestObjs *objs) {
	apint_destroy(objs->ap0);
	apint_destroy(objs->ap1);
	apint_destroy(objs->ap110660361);
	apint_destroy(objs->max1);
	apint_destroy(objs->minus1);
	apint_destroy(objs->ap100000000);
	apint_destroy(objs->ap0xbc848afUL);
	apint_destroy(objs->minusMax);
	apint_destroy(objs->ap2);
	apint_destroy(objs->minus2);
	apint_destroy(objs->ap8);
	apint_destroy(objs->ap7);
	apint_destroy(objs->ap15);
	apint_destroy(objs->ap16);
	apint_destroy(objs->apF);
	apint_destroy(objs->apFFFFFFFFFFFFFFFF);
	apint_destroy(objs->apFFFFFFFFFFFFFFFE);
	apint_destroy(objs->ap10000000000000000);
	apint_destroy(objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9);
	apint_destroy(objs->ap000962d7e839ed2d377);
	apint_destroy(objs->minus962d7e839ed2d377);
	/* TODO: destroy additional members of test fixture */

	free(objs);
}

void testCreateFromU64(TestObjs *objs) {
	ASSERT(0UL == apint_get_bits(objs->ap0, 0));
	ASSERT(1UL == apint_get_bits(objs->ap1, 0));
    ASSERT(110660361UL == apint_get_bits(objs->ap110660361, 0));
	ASSERT(0xFFFFFFFFFFFFFFFFUL == apint_get_bits(objs->max1, 0));
	ASSERT(0xbc848afUL == apint_get_bits(objs->ap0xbc848afUL, 0));
}

void testIsZero(TestObjs *objs) {
    ASSERT(apint_is_zero(objs->ap0) == 1);
	ASSERT(apint_is_zero(objs->ap1) == 0);
	ASSERT(apint_is_zero(objs->ap110660361) == 0);
	ASSERT(apint_is_zero(objs->max1) == 0);
}

void testIsNegative(TestObjs *objs) {
    ASSERT(apint_is_negative(objs->ap0) == 0);
	ASSERT(apint_is_negative(objs->ap1) == 0);
	ASSERT(apint_is_negative(objs->ap110660361) == 0);
	ASSERT(apint_is_negative(objs->max1) == 0);
    ASSERT(apint_is_negative(objs->minus1) == 1);
	ASSERT(apint_is_negative(objs->minus962d7e839ed2d377) == 1);
}

void testHighestBitSet(TestObjs *objs) {
	ASSERT(-1 == apint_highest_bit_set(objs->ap0));
	ASSERT(0 == apint_highest_bit_set(objs->ap1));
	ASSERT(26 == apint_highest_bit_set(objs->ap110660361));
	ASSERT(63 == apint_highest_bit_set(objs->max1));
	ASSERT(63 == apint_highest_bit_set(objs->apFFFFFFFFFFFFFFFF));
	ASSERT(64 == apint_highest_bit_set(objs->ap10000000000000000));
}

void testUnsignedCompare(TestObjs *objs) {
	ASSERT(unsigned_compare(objs->apFFFFFFFFFFFFFFFF, objs->apFFFFFFFFFFFFFFFE) > 0);
	ASSERT(unsigned_compare(objs->apFFFFFFFFFFFFFFFF, objs->apFFFFFFFFFFFFFFFF) == 0);
	ASSERT(unsigned_compare(objs->apFFFFFFFFFFFFFFFE, objs->apFFFFFFFFFFFFFFFF) < 0);
	ASSERT(unsigned_compare(objs->ap000962d7e839ed2d377, objs->minus962d7e839ed2d377) == 0);
	ASSERT(unsigned_compare(objs->ap10000000000000000, objs->apFFFFFFFFFFFFFFFF) > 0);
	ASSERT(unsigned_compare(objs->apFFFFFFFFFFFFFFFF, objs->ap10000000000000000) < 0);
}

void testCompare(TestObjs *objs) {
	/* 1 > 0 */
	ASSERT(apint_compare(objs->ap1, objs->ap0) > 0);
	/* 0 < 1 */
	ASSERT(apint_compare(objs->ap0, objs->ap1) < 0);
	/* 110660361 > 0 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap0) > 0);
	/* 110660361 > 1 */
	ASSERT(apint_compare(objs->ap110660361, objs->ap1) > 0);
	/* 0 < 110660361 */
	ASSERT(apint_compare(objs->ap0, objs->ap110660361) < 0);
	/* 1 < 110660361 */
	ASSERT(apint_compare(objs->ap1, objs->ap110660361) < 0);

	//larger numbers below

	/* 1 < ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9 */
	ASSERT(apint_compare(objs->ap1, objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9) < 0);
	/* 10000000000000000 < ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9 */
	ASSERT(apint_compare(objs->ap10000000000000000, objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9) < 0);
	/* minus962d7e839ed2d377 < ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9 */
	ASSERT(apint_compare(objs->minus962d7e839ed2d377, objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9) < 0);
	/* ap7e5ff912c8ede6ccff0d56ae5a9b5459804f8 < ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9 */
	ASSERT(apint_compare(objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f8, objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9) < 0);
	/* -10000000000000000 < -962d7e839ed2d377 */
	ASSERT(apint_compare(objs->minus10000000000000000, objs->minus962d7e839ed2d377) < 0);
}

void testFormatAsHex(TestObjs *objs) {
	char *s;

	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(objs->ap0))));
	free(s);

	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(objs->ap1))));
	free(s);

	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(objs->ap2))));
	free(s);

	ASSERT(0 == strcmp("7", (s = apint_format_as_hex(objs->ap7))));
	free(s);

	ASSERT(0 == strcmp("8", (s = apint_format_as_hex(objs->ap8))));
	free(s);

	ASSERT(0 == strcmp("f", (s = apint_format_as_hex(objs->ap15))));
	free(s);

	ASSERT(0 == strcmp("10", (s = apint_format_as_hex(objs->ap16))));
	free(s);

	ASSERT(0 == strcmp("6988b09", (s = apint_format_as_hex(objs->ap110660361))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->max1))));
	free(s);

	ASSERT(0 == strcmp("-2", (s = apint_format_as_hex(objs->minus2))));
	free(s);

	ASSERT(0 == strcmp("f", (s = apint_format_as_hex(objs->apF))));
	free(s);

	ASSERT(0 == strcmp("ffffffffffffffff", (s = apint_format_as_hex(objs->apFFFFFFFFFFFFFFFF))));
	free(s);

	ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(objs->apFFFFFFFFFFFFFFFE))));
	free(s);

	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(objs->ap10000000000000000))));
	free(s);

	ASSERT(0 == strcmp("7e5ff912c8ede6ccff0d56ae5a9b5459804f9", (s = apint_format_as_hex(objs->ap7e5ff912c8ede6ccff0d56ae5a9b5459804f9))));
	free(s);

    ASSERT(0 == strcmp("962d7e839ed2d377", (s = apint_format_as_hex(objs->ap000962d7e839ed2d377))));
	free(s);

	ASSERT(0 == strcmp("-962d7e839ed2d377", (s = apint_format_as_hex(objs->minus962d7e839ed2d377))));
	free(s);
}


void testAdd(TestObjs *objs) {
	ApInt *sum;
	char *s;

	/* 0 + 0 = 0 */
	sum = apint_add(objs->ap0, objs->ap0);
	ASSERT(0UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->ap0));
	ASSERT(0 == strcmp("0", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);
    
	/* 1 + 0 = 1 */
	sum = apint_add(objs->ap1, objs->ap0);
	ASSERT(1UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->ap1));
	ASSERT(0 == strcmp("1", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 1 + 1 = 2 */
	sum = apint_add(objs->ap1, objs->ap1);
	ASSERT(2UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->ap2));
	ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* 110660361 + 1 = 110660362 */
	sum = apint_add(objs->ap110660361, objs->ap1);
	ASSERT(110660362UL == apint_get_bits(sum, 0));
	//ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* 197675183 + 1 = 197675184 */
	sum = apint_add(objs->ap0xbc848afUL, objs->ap1);
	ASSERT(197675184UL == apint_get_bits(sum, 0));
	//ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* 197675183 + 0 = 197675183 */
	sum = apint_add(objs->ap0xbc848afUL, objs->ap0);
	ASSERT(0xbc848afUL == apint_get_bits(sum, 0));
	//ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* 110660361 + 100000000 = 210660361 */
	sum = apint_add(objs->ap110660361, objs->ap100000000);
	ASSERT(210660361UL == apint_get_bits(sum, 0));
	//ASSERT(0 == strcmp("6988b0a", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	free(s);

	ApInt *a = apint_create_from_hex("7e5ff912c8ede6ccff0d56ae5a9b5459804f9");
	ApInt *b = apint_create_from_hex("6057bccd860546f03fd51bf5488d50cca96");
	sum = apint_add(a, b);
	s = apint_format_as_hex(sum);
	//ASSERT(0 == strcmp("7ec050cf9673ec13ef4d2bca4fe3e1aa4cf8f", (s = apint_format_as_hex(sum))));
	for (int i = 0; i < (int)strlen(s); i++) {
		printf("%c", s[i]);
	}
	apint_destroy(sum);
	apint_destroy(b);
	apint_destroy(a);
	free(s);

	/* 0 + -1 = -1 */
	sum = apint_add(objs->ap0, objs->minus1);
	ASSERT(1UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->minus1));
	//ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* -1 + 0 = -1 */
	sum = apint_add(objs->minus1, objs->ap0);
	ASSERT(1UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->minus1));
	//ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* 1 + -1 = 0 */
	sum = apint_add(objs->ap1, objs->minus1);
	ASSERT(0UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->ap0));
	//ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* -1 + 1 = 0 */
	sum = apint_add(objs->minus1, objs->ap1);
	ASSERT(0UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->ap0));
	//ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

    /* -1 + -1 = -2 */
	sum = apint_add(objs->minus1, objs->minus1);
	ASSERT(2UL == apint_get_bits(sum, 0));
	ASSERT(0 == apint_compare(sum, objs->minus2));
	//ASSERT(0 == strcmp("2", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);

	/* FFFFFFFFFFFFFFFF + 1 = 10000000000000000 */
	sum = apint_add(objs->max1, objs->ap1);
	//ASSERT(10000000000000000 == apint_get_bits(sum, 0));
	//ASSERT(0 == strcmp("10000000000000000", (s = apint_format_as_hex(sum))));
	apint_destroy(sum);
	//free(s);
}

void testSub(TestObjs *objs) {
	//ApInt *a, *b;
	ApInt *diff;
	//char *s;

	/* subtracting 1 from ffffffffffffffff is fffffffffffffffe */
	diff = apint_sub(objs->max1, objs->ap1);
	ASSERT(18446744073709551614UL == apint_get_bits(diff, 0));
	//ASSERT(0 == strcmp("fffffffffffffffe", (s = apint_format_as_hex(diff))));
	apint_destroy(diff);
	//free(s);

	/* subtracting 0 from 1 is 1 */
	diff = apint_sub(objs->ap1, objs->ap0);
	ASSERT(1UL == apint_get_bits(diff, 0));
	//ASSERT(0 == strcmp("1", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap1));
	apint_destroy(diff);
	//free(s);

	/* subtracting 1 from 1 is 0 */
	diff = apint_sub(objs->ap1, objs->ap1);
	ASSERT(0UL == apint_get_bits(diff, 0));
	//ASSERT(0 == strcmp("0", (s = apint_format_as_hex(diff))));
	ASSERT(0 == apint_compare(diff, objs->ap0));
	apint_destroy(diff);
	//free(s);

	/* subtracting 1 from 0 is -1 */
	diff = apint_sub(objs->ap0, objs->ap1);
	ASSERT(1UL == apint_get_bits(diff, 0));
	//ASSERT(0 == (strcmp("-1", (s = apint_format_as_hex(diff)))));
	ASSERT(0 == apint_compare(diff, objs->minus1));
	apint_destroy(diff);
	//free(s);

	/* subtracting 1 from -1 is -2 */
	diff = apint_sub(objs->minus1, objs->ap1);
	ASSERT(2UL == apint_get_bits(diff, 0));
	//ASSERT(0 == (strcmp("-1", (s = apint_format_as_hex(diff)))));
	//ASSERT(0 == apint_compare(diff, objs->minus1));
	apint_destroy(diff);
	//free(s);

	/* test involving larger values */
	//a = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	//b = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	//diff = apint_sub(a, b);
	//ASSERT(0 == strcmp("7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
	//	(s = apint_format_as_hex(diff))));
	//apint_destroy(diff);
	//apint_destroy(b);
	//apint_destroy(a);
	//free(s);

	/* test involving larger values (with a negative difference) */
	//a = apint_create_from_hex("9fa0fb165441ade7cb8b17c3ab3653465e09e8078e09631ec8f6fe3a5b301dc");
	//b = apint_create_from_hex("7e35207519b6b06429378631ca460905c19537644f31dc50114e9dc90bb4e4ebc43cfebe6b86d");
	//diff = apint_sub(a, b);
	//ASSERT(0 == strcmp("-7e35207519b6afc4883c6fdd8898213a367d73b918de95f20766963b0251c622cd3ec4633b691",
	//	(s = apint_format_as_hex(diff))));
	//apint_destroy(diff);
	//apint_destroy(b);
	//apint_destroy(a);
	//free(s);
}

/* TODO: add more test functions */
