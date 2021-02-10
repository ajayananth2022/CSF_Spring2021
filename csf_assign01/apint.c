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

	uint8_t makeZero = 0; 

	if (strcmp(hex, "") == 0) makeZero = 1; //account for empty string

	int leadZeroes = 0; 

	if (hex[0] == '-') {
		leadZeroes++; 
		ap->flags = 1;
	} else {
		ap->flags = 0;
	}

	while (hex[leadZeroes] == '0') { //count number of leading zeros
		leadZeroes++; 
		if (leadZeroes == (int)strlen(hex)) {
			//string of ONLY 0 or 0s (preceeded or not preceeded by - sign)
			makeZero = 1; 
			break; 
		}
	}

	if (makeZero) {
		ap->len = 1;
		ap->flags = 0; 
		ap->data = malloc(sizeof(uint64_t));
		ap->data[0] = (uint64_t)0;
		return ap;
	}

	//2^64 is 17 hex chars long, and represented as 0 1 as 2 elements in an apInt
	ap->len = ((strlen(hex) - leadZeroes - 1)/ 16) + 1; 
	ap->data = malloc(ap->len * sizeof(uint64_t));

	hexConvert(ap, hex, leadZeroes); 
	return ap; 
}

//helper function that converts hex chars and places then into ap object
void hexConvert(ApInt *ap, const char *hex, int leadZeroes) {
	uint64_t sum = 0; 
	uint64_t curDigitHex = 0; 
	uint64_t curDigitAP = 0; 

	//loop over each non-leadzero and non-sign char in hex
	for (int i = strlen(hex) - 1; i >= leadZeroes; i--) {
		uint8_t convertedInt = hex_to_int(hex[i]); 

		//111 is invalid code
		if (convertedInt == 111) {
			ap = NULL; 
			break; 
		}

		sum += (convertedInt * (uint64_t)pow(16, curDigitHex)); 
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
		//111 will only be returned in case of invalid hex char
		return 111; 
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
	for (uint64_t i = 0; i < num_hex_bits - ap->flags; i++) {
		if (i % 16 == 0) current = ap->data[i / 16]; //jump to the correct element
		hex[num_hex_bits - i - 1] = int_to_hex(current % 16);
		current = current / 16; //shift to the right by 4 bits
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
		sum->flags = a->flags;
		sum = add(a, b, sum);
	} else if (a->flags == 1 && b->flags == 0) { //a is negative and b is positive
		sum = subtract(a, b, sum);
		if (unsigned_compare(a, b) <= 0) { 
			sum->flags = 0;  
		} else {
			sum->flags = 1; 
		}
	} else { //a is positive and b is negative
		sum = subtract(a, b, sum); 
		if (unsigned_compare(a, b) < 0) {
			sum->flags = 1; 
		} else {
			sum->flags = 0; 
		}
	}
	return sum;
}

//helper function for comparing the value of two ApInts
//return 1 if left > right, -1 if left < right, 0 if left == right
int unsigned_compare(const ApInt *left, const ApInt *right) {
	int left_highest = apint_highest_bit_set(left);
	int right_highest = apint_highest_bit_set(right);
	if (left_highest != right_highest) {
		if (left_highest > right_highest) return 1;
		return -1;
	}
	//assert(left->len == right->len); //for testing
	for (int i = left->len - 1; i >= 0; i--) {
		if (left->data[i] != right->data[i]) {
			if (left->data[i] > right->data[i]) return 1;
			return -1;
		}
	}
	return 0;
}

//helper function of adding two positive ApInts
//returns pointer to ApInt representing the sum 
//of the two ApInts
ApInt* add(const ApInt *left, const ApInt *right, ApInt *sum) {
	const ApInt *bigger;
	const ApInt *smaller;
	if (unsigned_compare(left, right) >= 0) {
		bigger = left;
		smaller = right;
	} else {
		bigger = right;
		smaller = left;
	}
	int bigger_num_elements = apint_highest_bit_set(bigger) / 64 + 1;
	int smaller_num_elements = apint_highest_bit_set(smaller) / 64 + 1;
	//guarantee's enough memory for the sum value
	sum->data = malloc((bigger_num_elements + 1) * sizeof(uint64_t));
	sum->len = 0;
	uint64_t temp_sum = 0;
	for (int i = 0; i < smaller_num_elements; i++) {
		temp_sum += left->data[i] + right->data[i];
		sum->data[i] = temp_sum;
		if (temp_sum < left->data[i]) { //addition causes overflow
			temp_sum = 1;
		} else { //no overflow
			temp_sum = 0;
		}
		sum->len++;
	}  
	int remaining_elements = bigger_num_elements - smaller_num_elements;
	for (int i = 0; i < remaining_elements; i++) {
		temp_sum += bigger->data[smaller_num_elements + i];
		sum->data[sum->len] = temp_sum;
		if (temp_sum < bigger->data[smaller_num_elements + i]) { //overflow
			temp_sum = 1;
		} else {
			temp_sum = 0; 
		}
		sum->len++;
	}
	if (temp_sum != 0) {
		sum->data[sum->len] = temp_sum;
		sum->len++;
	}
	//take care of unassigned empty memory
	if (sum->len < (unsigned)(bigger_num_elements + 1)) {
		sum->data = realloc(sum->data, sum->len * sizeof(uint64_t));
	}
	return sum; 
}

//helper function of subtracting two positive ApInts
//returns a pointer to an ApInt represeting the difference 
//between the two ApInts
ApInt* subtract(const ApInt *left, const ApInt *right, ApInt *diff) {
	const ApInt *bigger;
	const ApInt *smaller;
	if (unsigned_compare(left, right) >= 0) {
		bigger = left;
		smaller = right;
	} else {
		bigger = right;
		smaller = left;
	}
	int bigger_num_elements = apint_highest_bit_set(bigger) / 64 + 1;
	int smaller_num_elements = apint_highest_bit_set(smaller) / 64 + 1;
	//guarantee's enough memory for the sum value
	diff->data = malloc(bigger_num_elements * sizeof(uint64_t));
	diff->len = 0;
	uint64_t temp_diff = 0;
	for (int i = 0; i < smaller_num_elements; i++) {
		temp_diff += bigger->data[i] - smaller->data[i];
		if (temp_diff > bigger->data[i]) { //subtraction causes overflow
			diff->data[i] = temp_diff + 1 + UINT64_MAX;
			temp_diff = -1;
		} else { //no overflow
			diff->data[i] = temp_diff;
			temp_diff = 0;
		}
		diff->len++;
	}
	int remaining_elements = bigger_num_elements - smaller_num_elements;
	for (int i = 0; i < remaining_elements; i++) {
		temp_diff += bigger->data[smaller_num_elements + i];
		diff->data[diff->len] = temp_diff;
		if (temp_diff > bigger->data[smaller_num_elements + i]) { //overflow
			temp_diff = -1;
		} else {
			if (i != remaining_elements -1) temp_diff = 0; 
		}
		diff->len++;
	}
	if (diff->len != 1 && temp_diff == 0) {
		diff->len--;
	}
	//take care of unassigned empty memory
	assert(!(diff->len > (unsigned)bigger_num_elements));
	if (diff->len < (unsigned)bigger_num_elements) {
		diff->data = realloc(diff->data, diff->len * sizeof(uint64_t));
	}
	return diff; 
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

	if (left-> flags > right->flags) {
		return -1; 
	} else if (left-> flags < right->flags) {
		return 1; 
	} else if (left-> flags == 0 && right->flags == 0) {
		if (unsigned_compare(left, right) > 0) {
			return 1; 
		} else if (unsigned_compare(left, right) < 0) {
			return -1; 
		}
	} else {
		if (unsigned_compare(left, right) > 0) {
			return -1; 
		} else if (unsigned_compare(left, right) < 0) {
			return 1; 
		}
	}
	return 0; 
}
