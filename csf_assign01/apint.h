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
 * @return a pointer to a char array of hexadecimal digits
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
 * A helper function that adds two 64-bit unsigned integers.
 *
 * @param val1 first 64-bit unsigned integer
 * @param val2 second 64-bit unsigned integer
 * @return a 64-bit unsigned integer that is the sum
 */
uint64_t add(uint64_t val1, uint64_t val2);

/*
 * A helper function that subtracts two 64-bit unsigned integers.
 *
 * @param val1 64-bit unsigned integer to be subtracted from
 * @param val2 64-bit unsigned integer to be subtracted
 * @return the absolute value of the difference between the two values
 */
uint64_t subtract(uint64_t val1, uint64_t val2);
#ifdef __cplusplus
}
#endif

#endif /* APINT_H */
