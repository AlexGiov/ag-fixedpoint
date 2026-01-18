/**
 * @file ag_fixedpoint.c
 * @brief Implementation of simple math utility functions
 */

#include "ag_fixedpoint/ag_fixedpoint.h"

int add(int a, int b) { return a + b; }

int subtract(int a, int b) { return a - b; }

int multiply(int a, int b) { return a * b; }

int divide(int a, int b) {
	if (b == 0) {
		return 0;  // Avoid division by zero
	}
	return a / b;
}
