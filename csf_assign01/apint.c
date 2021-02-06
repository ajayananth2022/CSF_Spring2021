/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Lucy Zhang, Ajay Ananthakrishnan
 * wzhang99@jhu.edu, ajayananth@jhu.edu
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "apint.h"

ApInt *apint_create_from_u64(uint64_t val) {
	ApInt *ap = malloc(sizeof(ApInt));
	ap->flags = 0; //val is always positive
	ap->len = 1; //only one element needed for the array
	ap->data = malloc(ap->len * sizeof(uint64_t));
	ap->data[0] = val;
	return ap;
}

ApInt *apint_create_from_hex(const char *hex) {
	//for Milestone 2
	assert(0);
	(void)hex; //suppresses compile warning of unused variable
	return NULL;
}

void apint_destroy(ApInt *ap) {
	assert(ap); //make sure ap isn't pointing to NULL
	assert(ap->data);
	free(ap->data);
	free(ap);
}

int apint_is_zero(const ApInt *ap) {
	assert(ap); //make sure ap isn't pointing to NULL
	assert(ap->data);
	if (ap->data[0] == 0) return 1;
	return 0;
}

int apint_is_negative(const ApInt *ap) {
	assert(ap); //make sure ap isn't pointing to NULL
	if (ap->flags == 1) return 1;
	return 0;
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
	assert(ap); //make sure ap isn't pointing to NULL
	if (n >= ap->len) return 0;
	return ap->data[n];
}

int apint_highest_bit_set(const ApInt *ap) {
	assert(ap); //make sure ap isn't pointing to NULL
	int highest_bit = -1; 

	//for milestone 1
	uint64_t dataVal= ap->data[0]; 
    
	while (dataVal > 0) {
		dataVal = dataVal >> 1; //move to the left bit
		highest_bit++; 
	}
	return highest_bit;
}

char int_to_hex(const uint8_t num) {
	if (num < 10) {
		return (char)(num+48); //convert number to corresponding char
	}
	return (char)(num+87);
}

char *apint_format_as_hex(const ApInt *ap) {
	assert(ap);
	int num_bin_bits = apint_highest_bit_set(ap);

	//number of elements in the hex char array
	uint64_t num_hex_bits = num_bin_bits / 4 + 1; 
	if (apint_is_negative(ap)) num_hex_bits++; //one more element for negative
	char* hex = malloc((num_hex_bits + 1) * sizeof(char));
	if (apint_is_negative(ap)) hex[0] = '-';

	//loop through every 4 bits in the apint data
	//sum the 4 bit number and convert to hex to store in char array
	uint64_t current = ap->data[0]; 
	for (uint64_t i = 0; i < num_hex_bits; i++) {
		uint8_t hex_num = current % 2; //first bit
		current = current / 2; //second bit
		hex_num += current % 2 * 2; //2^1
		current = current / 2; //third bit
		hex_num += current % 2 * 4; //2^2
		current = current / 2; //fourth bit
		hex_num += current % 2 * 8; //2^3
		hex[num_hex_bits - i - 1] = int_to_hex(hex_num);
		current = current / 2; //first bit of next iteration
		if (apint_is_negative(ap) && i == num_hex_bits - 2) break;
	}
	return hex;
}

ApInt *apint_negate(const ApInt *ap) {
	assert(ap); //make sure ap isn't pointing to NULL
	ApInt *neg = malloc(sizeof(ApInt)); //declare a new instance of ApInt
	neg->len = ap->len;
	neg->data = malloc(neg->len * sizeof(uint64_t));
	for (uint32_t i = 0; i < ap->len; i++) { //deep copy of ap's data
		neg->data[i] = ap->data[i];
	}
	if (apint_is_zero(ap)) { //neg's sign is the same as ap if it's 0
		neg->flags = ap->flags;
	} else if (apint_is_negative(ap)) {
		neg->flags = 0;
	} else {
		neg->flags = 1;
	}
	return neg;
}

ApInt *apint_add(const ApInt *a, const ApInt *b) {
	assert(a); //make sure a isn't pointing to NULL
	assert(b); //make sure b isn't pointing to NULL
	ApInt *sum = malloc(sizeof(ApInt)); //new instance of ApInt representing sum
	sum->len = 1; //only for Milestone 1, data will have only 1 element
	sum->data = malloc(sizeof(uint64_t));

	if (a->flags == b->flags) { //signs are the same
		sum->data[0] = add(a->data[0], b->data[0]);
		sum->flags = a->flags;
	} else if (a->flags == 1 && b->flags == 0) { //a is negative and b is positive
		sum->data[0] = subtract(a->data[0], b->data[0]);
		if (a->data[0] <= b->data[0]) { 
			sum->flags = 0;  
		} else {
			sum->flags = 1; 
		}
	} else { //a is positive and b is negative
		sum->data[0] = subtract(a->data[0], b->data[0]); 
		if (a->data[0] < b->data[0]) {
			sum->flags = 1; 
		} else {
			sum->flags = 0; 
		}
	}
	return sum;
}

//helper function of adding unsigned int
//returns uint64_t represeting the sum 
//of the two values
uint64_t add(uint64_t val1, uint64_t val2) {
	return val1 + val2; 
}

//helper function of subtracting unsigned int
//returns uint64_t represeting the difference 
//between the two values
uint64_t subtract(uint64_t val1, uint64_t val2) {
	if (val1 > val2) { //make sure the result doesn't overflow
		return val1 - val2; 
	} else {
		return val2 - val1; 
	}
}

ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	assert(a); //make sure a isn't pointing to NULL
	assert(b); //make sure b isn't pointing to NULL
	ApInt *neg = apint_negate(b);
	ApInt *result = apint_add(a, neg);
	apint_destroy(neg);
	return result;
}

int apint_compare(const ApInt *left, const ApInt *right) {
	assert(left); //make sure left isn't pointing to NULL
	assert(right); //make sure right isn't pointing to NULL
	//both equal
	if (left->data[0] == right->data[0] && left->flags == right->flags) {
		return 0; 
	}

	//both negative
	if (left->flags == 1 && right->flags == 1) {
		if (left->data[0] > right->data[0]) return -1; 
		return 1; 
	}

	//both positive
	if (left->flags == 0 && right->flags == 0) {
		if (left->data[0] > right->data[0]) return 1; 
		return -1; 
	}
	//left is positive and right is negative
	if (left->flags == 0 && right->flags ==1) return 1; 
	//vice versa
	return -1; 
}
