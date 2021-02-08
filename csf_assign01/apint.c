/*
 * CSF Assignment 1
 * Arbitrary-precision integer data type
 * Function implementations
 * Lucy Zhang, Ajay Ananthakrishnan
 * wzhang99@jhu.edu, ajayananth@jhu.edu
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
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
	ApInt *ap = malloc(sizeof(ApInt));
	if (strcmp(hex, "-0") == 0) hex = "0"; //-0 is positive

	int leadZeroes = 0; 
	if (hex[0] == '-') {
		ap->flags = 1;
		ap->len = ((strlen(hex) - 2) / 16) + 1; //number of elements needed
	} else {
		ap->flags = 0;
		while (hex[leadZeroes] == '0') { //count number of leading zeros
			leadZeroes++; 
		}
		ap->len = ((strlen(hex) - leadZeroes  - 1 )/ 16) + 1; 
	}

	if (ap->len == 0) { //if hex is "0" or ""
		ap->len = 1;
		ap->data = malloc(ap->len * sizeof(uint64_t));
		ap->data[0] = 0;
		return ap;
	}

	ap->data = malloc(ap->len * sizeof(uint64_t));
	uint64_t sum = 0; 
	uint64_t curDigitHex = 0; 
	uint64_t curDigitAP = 0; 

	//loop over each non-leadzero and non-sign char in hex
	for (int i = strlen(hex) - 1; i >= leadZeroes; i--) {
		sum += (hex_to_int(hex[i]) * (uint64_t)pow(16, curDigitHex)); 
		curDigitHex++; 
		if (curDigitHex == 16) { //current data array element is full
			ap->data[curDigitAP] = sum;
			curDigitAP++; 
			curDigitHex = 0; 
			sum = 0; 
		}
		if (ap->flags == 1 && i == 1) break; //one fewer iteration for negatives
	}
	//if the last sum is not stored afer the loop
	if (curDigitHex != 0) ap->data[curDigitAP] = sum;
	return ap;
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

	uint64_t dataVal= ap->data[ap->len - 1]; 
    
	while (dataVal > 0) {
		dataVal = dataVal >> 1; //move to the left bit
		highest_bit++; 
	}
	return highest_bit + 64 * (ap->len - 1);
}

char int_to_hex(const uint8_t num) {
	if (num < 10) {
		return (char)(num+48); //convert number to corresponding char
	}
	return (char)(num+87);
}

uint8_t hex_to_int(const char hex) {
	if (hex >= '0' && hex <= '9') {
		return hex - '0';
	}
    else if (hex >= 'a' && hex <='f') {
		return hex - 'a' + 10;
	}
    else if (hex >= 'A' && hex <= 'F') { 
		return hex - 'A' + 10;    
	}
	//error condition: hex char is not valid
	else {
		return 101; 
	}
}

char *apint_format_as_hex(const ApInt *ap) {
	assert(ap);
	int num_bin_bits = apint_highest_bit_set(ap);

	//number of elements in the hex char array
	uint64_t num_hex_bits = num_bin_bits / 4 + 1; 
	if (apint_is_negative(ap)) num_hex_bits++; //one more element for negative
	char* hex = malloc((num_hex_bits + 1) * sizeof(char));
	hex[num_hex_bits] = '\0';
	if (apint_is_negative(ap)) hex[0] = '-';

	//loop through every 4 bits in the apint data
	//sum the 4 bit number and convert to hex to store in char array
	uint64_t current = ap->data[0]; 
	for (uint64_t i = 0; i < num_hex_bits; i++) {
		if (i % 16 == 0) current = ap->data[i / 16]; //jump to the correct element
		hex[num_hex_bits - i - 1] = int_to_hex(current % 16);
		current = current / 16; //shift to the right by 4 bits
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
	if (a->flags == b->flags) { //signs are the same
		//sum->data[0] = add(a->data[0], b->data[0]);
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

//helper function for comparing the value of two ApInts
int unsigned_compare(const ApInt *left, const ApInt *right) {
	int left_highest = apint_highest_bit_set(left);
	int right_highest = apint_highest_bit_set(right);
	if (left_highest != right_highest) {
		if (left_highest > right_highest) return 1;
		return -1;
	}
	assert(left->len == right->len); //for testing
	for (int i = left->len - 1; i >= 0; i--) {
		if (left->data[i] != right->data[i]) {
			if (left->data[i] > right->data[i]) return 1;
			return -1;
		}
	}
	return 0;
}

//helper function of adding unsigned int
//returns uint64_t represeting the sum 
//of the two values
ApInt* add(const ApInt *left, const ApInt *right, ApInt *sum) {
	if (apint_highest_bit_set(left) >= apint_highest_bit_set(right)) {
		sum->len = left->len + 1; //guarantee enough space in sum's data array
	} else {
		sum->len = right->len + 1;
	}
	sum->data = malloc(sum->len * sizeof(uint64_t));
	return sum; 
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
