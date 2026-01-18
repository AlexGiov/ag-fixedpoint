/**
 * @file test_ag_fixedpoint.c
 * @brief Unit tests for AG Fixed-Point Arithmetic Library
 *
 * Comprehensive test suite covering:
 * - Integer conversions
 * - Compile-time constants
 * - Floating-point conversions
 * - Boundary conditions
 * - Precision validation
 *
 * @author Alex Giovannini
 * @date 2026-01-18
 */

#include <cmocka.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "ag_fixedpoint/ag_fixedpoint.h"

/* Tolerance for floating-point comparisons */
#define FLOAT_TOLERANCE 0.001

/*============================================================================*/
/* TEST FIXTURES                                                              */
/*============================================================================*/

static int setup(void** state) {
	(void)state; /* unused */
	return 0;
}

static int teardown(void** state) {
	(void)state; /* unused */
	return 0;
}

/*============================================================================*/
/* UQ12.4 TESTS                                                               */
/*============================================================================*/

/**
 * @brief Test UQ12.4 integer conversions
 */
static void test_uq12_4_integer_conversion(void** state) {
	(void)state;

	/* Test zero */
	ag_fixedpoint_uq12_4_t zero = ag_fixedpoint_uq12_4_from_int(0);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(zero), 0);

	/* Test positive values */
	ag_fixedpoint_uq12_4_t val1 = ag_fixedpoint_uq12_4_from_int(25);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(val1), 25);

	ag_fixedpoint_uq12_4_t val2 = ag_fixedpoint_uq12_4_from_int(100);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(val2), 100);

	/* Test maximum value */
	ag_fixedpoint_uq12_4_t max_val = ag_fixedpoint_uq12_4_from_int(4095);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(max_val), 4095);
}

/**
 * @brief Test UQ12.4 compile-time constants
 */
static void test_uq12_4_constants(void** state) {
	(void)state;

	/* Test pi approximation: 3.1416 */
	ag_fixedpoint_uq12_4_t pi = AG_FIXEDPOINT_UQ12_4_CONST(3, 1416);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(pi), 3);

	/* Test exact quarter: 0.25 */
	ag_fixedpoint_uq12_4_t quarter = AG_FIXEDPOINT_UQ12_4_CONST(0, 2500);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(quarter), 0);

	/* Test 25.75 */
	ag_fixedpoint_uq12_4_t mixed = AG_FIXEDPOINT_UQ12_4_CONST(25, 7500);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(mixed), 25);
}

#if AG_FIXEDPOINT_ENABLE_FLOAT
/**
 * @brief Test UQ12.4 floating-point conversions
 */
static void test_uq12_4_float_conversion(void** state) {
	(void)state;

	/* Test zero */
	ag_fixedpoint_uq12_4_t zero = ag_fixedpoint_uq12_4_from_float(0.0);
	ag_fixedpoint_float_t zero_f = ag_fixedpoint_uq12_4_to_float(zero);
	assert_true(fabs(zero_f - 0.0) < FLOAT_TOLERANCE);

	/* Test simple value: 25.5 */
	ag_fixedpoint_uq12_4_t val1 = ag_fixedpoint_uq12_4_from_float(25.5);
	ag_fixedpoint_float_t val1_f = ag_fixedpoint_uq12_4_to_float(val1);
	assert_true(fabs(val1_f - 25.5) < FLOAT_TOLERANCE);

	/* Test fractional value: 3.1416 - UQ12.4 has only 4 fractional bits (1/16 resolution) */
	ag_fixedpoint_uq12_4_t pi = ag_fixedpoint_uq12_4_from_float(3.1416);
	ag_fixedpoint_float_t pi_f = ag_fixedpoint_uq12_4_to_float(pi);
	/* UQ12.4 resolution is 0.0625, so expect some quantization error */
	assert_true(fabs(pi_f - 3.1416) < 0.1);

	/* Test round-trip conversion */
	ag_fixedpoint_float_t original = 123.0625;
	ag_fixedpoint_uq12_4_t fp = ag_fixedpoint_uq12_4_from_float(original);
	ag_fixedpoint_float_t recovered = ag_fixedpoint_uq12_4_to_float(fp);
	assert_true(fabs(recovered - original) < FLOAT_TOLERANCE);
}
#endif

/*============================================================================*/
/* UQ10.6 TESTS                                                               */
/*============================================================================*/

/**
 * @brief Test UQ10.6 integer conversions
 */
static void test_uq10_6_integer_conversion(void** state) {
	(void)state;

	ag_fixedpoint_uq10_6_t zero = ag_fixedpoint_uq10_6_from_int(0);
	assert_int_equal(ag_fixedpoint_uq10_6_to_int(zero), 0);

	ag_fixedpoint_uq10_6_t val = ag_fixedpoint_uq10_6_from_int(512);
	assert_int_equal(ag_fixedpoint_uq10_6_to_int(val), 512);

	ag_fixedpoint_uq10_6_t max_val = ag_fixedpoint_uq10_6_from_int(1023);
	assert_int_equal(ag_fixedpoint_uq10_6_to_int(max_val), 1023);
}

/**
 * @brief Test UQ10.6 compile-time constants
 */
static void test_uq10_6_constants(void** state) {
	(void)state;

	ag_fixedpoint_uq10_6_t half = AG_FIXEDPOINT_UQ10_6_CONST(0, 5000);
	assert_int_equal(ag_fixedpoint_uq10_6_to_int(half), 0);

	ag_fixedpoint_uq10_6_t percent = AG_FIXEDPOINT_UQ10_6_CONST(100, 0);
	assert_int_equal(ag_fixedpoint_uq10_6_to_int(percent), 100);
}

/*============================================================================*/
/* Q4.11 TESTS (SIGNED)                                                       */
/*============================================================================*/

/**
 * @brief Test Q4.11 integer conversions with signed values
 */
static void test_q4_11_integer_conversion(void** state) {
	(void)state;

	/* Test zero */
	ag_fixedpoint_q4_11_t zero = ag_fixedpoint_q4_11_from_int(0);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(zero), 0);

	/* Test positive value */
	ag_fixedpoint_q4_11_t pos = ag_fixedpoint_q4_11_from_int(10);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(pos), 10);

	/* Test negative value */
	ag_fixedpoint_q4_11_t neg = ag_fixedpoint_q4_11_from_int(-5);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(neg), -5);

	/* Test maximum positive */
	ag_fixedpoint_q4_11_t max_pos = ag_fixedpoint_q4_11_from_int(15);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(max_pos), 15);

	/* Test maximum negative */
	ag_fixedpoint_q4_11_t max_neg = ag_fixedpoint_q4_11_from_int(-16);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(max_neg), -16);
}

/**
 * @brief Test Q4.11 compile-time constants with signed values
 */
static void test_q4_11_constants(void** state) {
	(void)state;

	/* Test positive fractional */
	ag_fixedpoint_q4_11_t pos = AG_FIXEDPOINT_Q4_11_CONST(1, 5000);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(pos), 1);

	/* Test negative fractional - note: -3.25 truncated via right shift gives -4 (rounds toward negative infinity) */
	ag_fixedpoint_q4_11_t neg = AG_FIXEDPOINT_Q4_11_CONST(-3, 2500);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(neg), -4); /* Arithmetic right shift behavior */

	/* Test zero with fraction */
	ag_fixedpoint_q4_11_t zero_frac = AG_FIXEDPOINT_Q4_11_CONST(0, 7500);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(zero_frac), 0);
}

#if AG_FIXEDPOINT_ENABLE_FLOAT
/**
 * @brief Test Q4.11 floating-point conversions with signed values
 */
static void test_q4_11_float_conversion(void** state) {
	(void)state;

	/* Test positive value */
	ag_fixedpoint_q4_11_t pos = ag_fixedpoint_q4_11_from_float(5.75);
	ag_fixedpoint_float_t pos_f = ag_fixedpoint_q4_11_to_float(pos);
	assert_true(fabs(pos_f - 5.75) < FLOAT_TOLERANCE);

	/* Test negative value */
	ag_fixedpoint_q4_11_t neg = ag_fixedpoint_q4_11_from_float(-3.25);
	ag_fixedpoint_float_t neg_f = ag_fixedpoint_q4_11_to_float(neg);
	assert_true(fabs(neg_f - (-3.25)) < FLOAT_TOLERANCE);

	/* Test zero */
	ag_fixedpoint_q4_11_t zero = ag_fixedpoint_q4_11_from_float(0.0);
	ag_fixedpoint_float_t zero_f = ag_fixedpoint_q4_11_to_float(zero);
	assert_true(fabs(zero_f - 0.0) < FLOAT_TOLERANCE);

	/* Test small negative */
	ag_fixedpoint_q4_11_t small_neg = ag_fixedpoint_q4_11_from_float(-0.5);
	ag_fixedpoint_float_t small_neg_f = ag_fixedpoint_q4_11_to_float(small_neg);
	assert_true(fabs(small_neg_f - (-0.5)) < FLOAT_TOLERANCE);
}
#endif

/*============================================================================*/
/* Q3.28 TESTS (HIGH PRECISION SIGNED)                                       */
/*============================================================================*/

/**
 * @brief Test Q3.28 integer conversions
 */
static void test_q3_28_integer_conversion(void** state) {
	(void)state;

	/* Test zero */
	ag_fixedpoint_q3_28_t zero = ag_fixedpoint_q3_28_from_int(0);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(zero), 0);

	/* Test positive */
	ag_fixedpoint_q3_28_t pos = ag_fixedpoint_q3_28_from_int(5);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(pos), 5);

	/* Test negative */
	ag_fixedpoint_q3_28_t neg = ag_fixedpoint_q3_28_from_int(-7);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(neg), -7);
}

/**
 * @brief Test Q3.28 compile-time constants
 */
static void test_q3_28_constants(void** state) {
	(void)state;

	ag_fixedpoint_q3_28_t pi = AG_FIXEDPOINT_Q3_28_CONST(3, 1416);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(pi), 3);

	/* -1.5 truncated via arithmetic right shift gives -2 */
	ag_fixedpoint_q3_28_t neg_half = AG_FIXEDPOINT_Q3_28_CONST(-1, 5000);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(neg_half), -2); /* Arithmetic right shift behavior */
}

#if AG_FIXEDPOINT_ENABLE_FLOAT
/**
 * @brief Test Q3.28 high-precision floating-point conversions
 */
static void test_q3_28_float_conversion(void** state) {
	(void)state;

	/* Test high precision value */
	ag_fixedpoint_float_t precise_val = 1.23456789;
	ag_fixedpoint_q3_28_t fp = ag_fixedpoint_q3_28_from_float(precise_val);
	ag_fixedpoint_float_t recovered = ag_fixedpoint_q3_28_to_float(fp);

/* Q3.28 has 28 fractional bits, should preserve good precision */
#if AG_FIXEDPOINT_FLOAT_TYPE == 64
	/* With 64-bit float, expect very high precision */
	assert_true(fabs(recovered - precise_val) < 0.000001);
#else
	/* With 32-bit float, expect some precision loss due to mantissa size */
	assert_true(fabs(recovered - precise_val) < 0.0001);
#endif

	/* Test negative high precision */
	ag_fixedpoint_float_t neg_precise = -2.987654321;
	ag_fixedpoint_q3_28_t neg_fp = ag_fixedpoint_q3_28_from_float(neg_precise);
	ag_fixedpoint_float_t neg_recovered = ag_fixedpoint_q3_28_to_float(neg_fp);
	assert_true(fabs(neg_recovered - neg_precise) < 0.001);
}
#endif

/*============================================================================*/
/* BOUNDARY AND EDGE CASE TESTS                                              */
/*============================================================================*/

/**
 * @brief Test boundary values for unsigned types
 */
static void test_unsigned_boundaries(void** state) {
	(void)state;

	/* UQ12.4 max value */
	ag_fixedpoint_uq12_4_t uq12_4_max = ag_fixedpoint_uq12_4_from_int(4095);
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(uq12_4_max), 4095);

	/* UQ10.6 max value */
	ag_fixedpoint_uq10_6_t uq10_6_max = ag_fixedpoint_uq10_6_from_int(1023);
	assert_int_equal(ag_fixedpoint_uq10_6_to_int(uq10_6_max), 1023);

	/* UQ6.10 max value */
	ag_fixedpoint_uq6_10_t uq6_10_max = ag_fixedpoint_uq6_10_from_int(63);
	assert_int_equal(ag_fixedpoint_uq6_10_to_int(uq6_10_max), 63);

	/* UQ4.12 max value */
	ag_fixedpoint_uq4_12_t uq4_12_max = ag_fixedpoint_uq4_12_from_int(15);
	assert_int_equal(ag_fixedpoint_uq4_12_to_int(uq4_12_max), 15);
}

/**
 * @brief Test boundary values for signed types
 */
static void test_signed_boundaries(void** state) {
	(void)state;

	/* Q4.11 positive max */
	ag_fixedpoint_q4_11_t q4_11_max_pos = ag_fixedpoint_q4_11_from_int(15);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(q4_11_max_pos), 15);

	/* Q4.11 negative max */
	ag_fixedpoint_q4_11_t q4_11_max_neg = ag_fixedpoint_q4_11_from_int(-16);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(q4_11_max_neg), -16);

	/* Q3.12 positive max */
	ag_fixedpoint_q3_12_t q3_12_max_pos = ag_fixedpoint_q3_12_from_int(7);
	assert_int_equal(ag_fixedpoint_q3_12_to_int(q3_12_max_pos), 7);

	/* Q3.12 negative max */
	ag_fixedpoint_q3_12_t q3_12_max_neg = ag_fixedpoint_q3_12_from_int(-8);
	assert_int_equal(ag_fixedpoint_q3_12_to_int(q3_12_max_neg), -8);

	/* Q3.28 positive max */
	ag_fixedpoint_q3_28_t q3_28_max_pos = ag_fixedpoint_q3_28_from_int(7);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(q3_28_max_pos), 7);

	/* Q3.28 negative max */
	ag_fixedpoint_q3_28_t q3_28_max_neg = ag_fixedpoint_q3_28_from_int(-8);
	assert_int_equal(ag_fixedpoint_q3_28_to_int(q3_28_max_neg), -8);
}

/**
 * @brief Test fractional precision for different formats
 */
static void test_fractional_precision(void** state) {
	(void)state;

	/* UQ12.4 has 4 fractional bits = 1/16 resolution */
	ag_fixedpoint_uq12_4_t uq12_4_frac = AG_FIXEDPOINT_UQ12_4_CONST(0, 625); /* 0.0625 = 1/16 */
	assert_true(uq12_4_frac != 0);											 /* Should be able to represent 1/16 */

	/* UQ4.12 has 12 fractional bits = 1/4096 resolution */
	ag_fixedpoint_uq4_12_t uq4_12_frac = AG_FIXEDPOINT_UQ4_12_CONST(0, 244); /* ~1/4096 */
	assert_true(uq4_12_frac != 0);											 /* Should represent very small fractions */
}

/*============================================================================*/
/* COMPILE-TIME CONSTANT VALIDATION                                          */
/*============================================================================*/

/**
 * @brief Verify that constants are compile-time evaluated
 */
static void test_compile_time_constants(void** state) {
	(void)state;

	/* These should all be compile-time constants (preprocessor/compiler evaluated) */
	static const ag_fixedpoint_uq12_4_t const_val1 = AG_FIXEDPOINT_UQ12_4_CONST(100, 5000);
	static const ag_fixedpoint_q4_11_t const_val2 = AG_FIXEDPOINT_Q4_11_CONST(-5, 7500); /* -5.75 */
	static const ag_fixedpoint_q3_28_t const_val3 = AG_FIXEDPOINT_Q3_28_CONST(3, 1416);

	/* Verify they work correctly */
	assert_int_equal(ag_fixedpoint_uq12_4_to_int(const_val1), 100);
	assert_int_equal(ag_fixedpoint_q4_11_to_int(const_val2), -6); /* -5.75 truncates to -6 */
	assert_int_equal(ag_fixedpoint_q3_28_to_int(const_val3), 3);
}

/*============================================================================*/
/* CONFIGURATION VALIDATION                                                  */
/*============================================================================*/

/**
 * @brief Verify library configuration and version
 */
static void test_library_configuration(void** state) {
	(void)state;

	/* Version check */
	assert_int_equal(AG_FIXEDPOINT_VERSION_MAJOR, 1);
	assert_int_equal(AG_FIXEDPOINT_VERSION_MINOR, 0);
	assert_int_equal(AG_FIXEDPOINT_VERSION_PATCH, 0);

/* Verify float type configuration if enabled */
#if AG_FIXEDPOINT_ENABLE_FLOAT
#if AG_FIXEDPOINT_FLOAT_TYPE == 32
	assert_int_equal(AG_FIXEDPOINT_FLOAT_BITS, 32);
#elif AG_FIXEDPOINT_FLOAT_TYPE == 64
	assert_int_equal(AG_FIXEDPOINT_FLOAT_BITS, 64);
#endif
#endif
}

/*============================================================================*/
/* MAIN TEST RUNNER                                                           */
/*============================================================================*/

/**
 * @brief Main test runner
 */
int main(void) {
	const struct CMUnitTest tests[] = {
		/* UQ12.4 tests */
		cmocka_unit_test(test_uq12_4_integer_conversion),
		cmocka_unit_test(test_uq12_4_constants),
#if AG_FIXEDPOINT_ENABLE_FLOAT
		cmocka_unit_test(test_uq12_4_float_conversion),
#endif

		/* UQ10.6 tests */
		cmocka_unit_test(test_uq10_6_integer_conversion),
		cmocka_unit_test(test_uq10_6_constants),

		/* Q4.11 tests (signed) */
		cmocka_unit_test(test_q4_11_integer_conversion),
		cmocka_unit_test(test_q4_11_constants),
#if AG_FIXEDPOINT_ENABLE_FLOAT
		cmocka_unit_test(test_q4_11_float_conversion),
#endif

		/* Q3.28 tests (high precision signed) */
		cmocka_unit_test(test_q3_28_integer_conversion),
		cmocka_unit_test(test_q3_28_constants),
#if AG_FIXEDPOINT_ENABLE_FLOAT
		cmocka_unit_test(test_q3_28_float_conversion),
#endif

		/* Boundary tests */
		cmocka_unit_test(test_unsigned_boundaries),
		cmocka_unit_test(test_signed_boundaries),
		cmocka_unit_test(test_fractional_precision),

		/* Compile-time validation */
		cmocka_unit_test(test_compile_time_constants),

		/* Configuration */
		cmocka_unit_test(test_library_configuration),
	};

	printf("\n");
	printf("========================================\n");
	printf("  AG Fixed-Point Library - Unit Tests  \n");
	printf("========================================\n");
	printf("Library Version: %s\n", AG_FIXEDPOINT_VERSION_STRING);
#if AG_FIXEDPOINT_ENABLE_FLOAT
	printf("Float Support:   ENABLED (%d-bit)\n", AG_FIXEDPOINT_FLOAT_BITS);
#else
	printf("Float Support:   DISABLED\n");
#endif
	printf("========================================\n\n");

	return cmocka_run_group_tests(tests, setup, teardown);
}
