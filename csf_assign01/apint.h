/*
 * Arbitrary-precision integer data type
 * Lucy Zhang, Ajay Ananthakrishnan
 * wzhang99@jhu.edu, ajayananth@jhu.edu
 */

#ifndef APINT_H
#define APINT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Representation: the data field is a little-endian bitstring ---
 * data[0] is bits 0..63, data[1] is bits 64..127, etc.
 */
typedef struct {
	uint32_t len; //number of elements the data array has
	uint32_t flags; //sign of the value (0 represents +; 1 represents -)
	uint64_t *data; //dynamically allocated array of uint64_t elements
} ApInt;

/*
 * Constructor for creating an ApInt from a 64-bit unsigned value.
 *
 * @param val a 64-bit unsigned value
 * @return a pointer to an ApInt instance whose value is specified 
 * by the val parameter
 */
ApInt *apint_create_from_u64(uint64_t val);

/*
 * Constructor for creating an ApInt from hexadecimal digits.
 *
 * @param hex pointer of an array of characters representing 
 * the hexadecimal digit
 * @return a pointer to an ApInt instance whose value is specified 
 * by the character array of hexadecimal digit
 */
ApInt *apint_create_from_hex(const char *hex);

/*
 * Deallocates memory used by the ApInt object.
 *
 * @param ap pointer of an ApInt instance to be destroyed
 */
void apint_destroy(ApInt *ap);

/*
 * Determines if the ApInt instance is zero.
 *
 * @param ap pointer of an ApInt instance
 * @return 1 if the ApInt instance's data is 0, otherwise returns 0
 */
int apint_is_zero(const ApInt *ap);

/*
 * Determines if the ApInt instance is negative.
 *
 * @param ap pointer of an ApInt instance
 * @return 1 if the ApInt instance's data is negative, otherwise returns 0
 */
int apint_is_negative(const ApInt *ap);

/*
 * Uses the specified index to retrieve the 64-bit unsigned integer stored 
 * in the corresponding index of the ApInt instance.
 *
 * @param ap pointer of an ApInt instance
 * @param n the index determining which 64 bits to return.
 * @return a 64-bit unsigned integer stored in the ApInt instance
 */
uint64_t apint_get_bits(const ApInt *ap, unsigned n);

/*
 * Finds the position of the most significant bit of 1 in the stored value.
 *
 * @param ap pointer of an ApInt instance
 * @return the integer position of the most significant bit of 1
 */
int apint_highest_bit_set(const ApInt *ap);

/*
 * Formats the data in an ApInt instance into a character array of 
 * hexadecimal digits.
 *
 * @param ap pointer of an ApInt instance
 * @return a pointer to a dynamically allocated char array of hexadecimal digits
 */
char *apint_format_as_hex(const ApInt *ap);

/*
 * Creates a new ApInt instance with same data value and opposite sign.
 *
 * @param ap pointer of an ApInt instance
 * @return an ApInt instance with the opposite sign (except 0)
 */
ApInt *apint_negate(const ApInt *ap);

/*
 * Adds two ApInt instances together.
 *
 * @param a pointer to the first ApInt instance
 * @param b pointer to the second ApInt instance
 * @return a new ApInt instance representing the sum
 */
ApInt *apint_add(const ApInt *a, const ApInt *b);

/*
 * Subtracts one ApInt instance from the other.
 *
 * @param a pointer to the ApInt instance to be subtracted from
 * @param b pointer to the ApInt instance to be subtracted
 * @return a new ApInt instance representing the difference
 */
ApInt *apint_sub(const ApInt *a, const ApInt *b);

/*
 * Compares the data value in two ApInt instances.
 *
 * @param left pointer to the first ApInt instance
 * @param right pointer to the second ApInt instance
 * @return -1 if left is less than right, 1 if left is greater, 0 if
 * left and right are equal
 */
int apint_compare(const ApInt *left, const ApInt *right);

/*
 * A helper function that adds two ApInts.
 *
 * @param left pointer to the first ApInt 
 * @param right pointer to the second ApInt
 * @param sum pointer to the ApInt to store the sum of the two ApInts
 * @return a pointer to the ApInt used to store the sum
 */
ApInt* add(const ApInt *left, const ApInt *right, ApInt *sum);

/*
 * A helper function that subtracts two ApInts.
 *
 * @param left pointer to the first ApInt
 * @param right pointer to the second ApInt
 * @param diff pointer to the ApInt to store the difference of the two ApInts
 * @return a pointer to the ApInt used to store the absolute difference 
 */
ApInt* subtract(const ApInt *left, const ApInt *right, ApInt *diff);

/*
 * A helper function that adds two elements at the same index from two ApInts.
 *
 * @param bigger pointer to the ApInt with bigger value
 * @param smaller pointer to the ApInt with smaller value
 * @param sum pointer to the ApInt to store the sum of the two ApInts
 * @temp_sum pointer to a 64-bit unsigned int to store the temporary sum and overflow
 */
void add_elements(const ApInt *bigger, const ApInt *smaller, ApInt *sum, uint64_t *temp_sum);

/*
 * A helper function that subtracts two elements at the same index from two ApInts.
 *
 * @param bigger pointer to the ApInt with bigger value
 * @param smaller pointer to the ApInt with smaller value
 * @param diff pointer to the ApInt to store the sum of the two ApInts
 * @temp_diff pointer to a 64-bit unsigned int to store the temporary diff and overflow
 */
void sub_elements(const ApInt *bigger, const ApInt *smaller, ApInt *diff, uint64_t *temp_diff);

/*
 * A helper function that converts an 8-bit unsigned int to a hex char.
 *
 * @param num an 8-bit unsigned integer to be converted into hexadecimal char
 * @return a char corresponding to the given 8-bit unsigned int
 */
char int_to_hex(const uint8_t num);

/*
 * A helper function that converts hex char to an 8-bit unsigned int.
 *
 * @param hex a hexadecimal char to be converted into 8-bit unsigned int
 * @return the corresponding 8-bit unsigned int, or 111 if the hex char is invalid
 */
uint8_t hex_to_int(const char hex);

/*
 * A helper function used by apint_create_from_hex to convert hex chars into 64-bit
 * unsigned int and place them into an ApInt data array.
 *
 * @param pointer to the ApInt to store the converted hex chars
 * @hex pointer to a hexadecimal array to be converted into 64-bit and stored
 * @leadZeroes the number of leading zeros or negative signs at the front of the hex
 */
void hexConvert(ApInt *ap, const char *hex, int leadZeroes);

/*
 * A helper function that compares the value ofo two ApInts.
 *
 * @param left pointer to the first ApInt
 * @param right pointer to the second ApInt
 * @return 1 if left > right, -1 if left < right, 0 if left == right
 */
int unsigned_compare(const ApInt *left, const ApInt *right);

#ifdef __cplusplus
}
#endif

#endif /* APINT_H */
