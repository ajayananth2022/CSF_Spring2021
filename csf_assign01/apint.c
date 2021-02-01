/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
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
	/* TODO: implement */
	assert(0);
	return NULL;
}

void apint_destroy(ApInt *ap) {
	assert(ap); //make sure ap isn't pointing to NULL
	assert(ap->data);
	free(ap->data);
	free(ap);
}

int apint_is_zero(const ApInt *ap) {
	assert(ap);
	assert(ap->data);
	if (ap->data[0] == 0) return 1;
	return 0;
}

int apint_is_negative(const ApInt *ap) {
	assert(ap);
	if (ap->flags == 1) return 1;
	return 0;
}

uint64_t apint_get_bits(const ApInt *ap, unsigned n) {
	if (n >= ap->len) return 0;
	return ap->data[n];
}

int apint_highest_bit_set(const ApInt *ap) {
	int highest_bit = -1; 

	//for milestone 1
	uint64_t dataVal= ap->data[0]; 
    
	while (dataVal > 0) {
		dataVal = dataVal >> 1; //move to the left bit
		highest_bit++; 
	}
	return highest_bit;
}

char *apint_format_as_hex(const ApInt *ap) {
	/* TODO: implement */
	assert(0);
	return NULL;
}

ApInt *apint_negate(const ApInt *ap) {
	assert(ap);
	ApInt *neg = malloc(sizeof(ApInt)); //declare a new instance of ApInt
	neg->len = ap->len;
	neg->data = malloc(neg->len * sizeof(uint64_t));
	for (int i = 0; i < ap->len; i++) { //deep copy of ap's data
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
	ApInt *sum = malloc(sizeof(ApInt)); //new instance of ApInt representing sum
	sum->len = 1; //only for Milestone 1, data will have only 1 element
	sum->data = malloc(sizeof(uint64_t));

	if (a->flags == b->flags) { //signs are the same
		sum->data[0] = add(a->data[0], b->data[0]);
		sum->flags = a->flags;
	} else if (a->flags == 1 && b->flags == 0) { //a is negative and b is positive
		sum->data[0] = subtract(a->data[0], b->data[0]);
		if (a->data[0] < b->data[0]) { 
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
int add(uint64_t val1, uint64_t val2) {
	return val1 + val2; 
}

//val1 must be larger then val2
//uint64_t
int subtract(uint64_t val1, uint64_t val2) {
	if (val1 > val2) {
		return val1 - val2; 
	} else {
		return val2 - val1; 
	}
}

ApInt *apint_sub(const ApInt *a, const ApInt *b) {
	return apint_add(a, apint_negate(b));
}

int apint_compare(const ApInt *left, const ApInt *right) {
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
