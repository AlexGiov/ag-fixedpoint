/**
 * @file example_q15_0_usage.c
 * @brief Example usage of Q15.0 and UQ16.0 types
 *
 * Demonstrates how to use integer fixed-point types (zero fractional bits)
 * in mixed arithmetic with fractional fixed-point types.
 *
 * @author Alex Giovannini
 * @date 2026-01-19
 */

#include <stdio.h>

#include "ag_fixedpoint/ag_fixedpoint.h"


int main(void) {
	printf("==============================================\n");
	printf("  Q15.0 and UQ16.0 Usage Examples\n");
	printf("==============================================\n\n");

	/*========================================================================*/
	/* UQ16.0 - Unsigned Integer in Q Notation                               */
	/*========================================================================*/

	printf("1. UQ16.0 (Unsigned Integer)\n");
	printf("   Format: 16 integer bits, 0 fractional bits\n");
	printf("   Range: [0, 65535]\n\n");

	// Create from integer
	ag_fixedpoint_uq16_0_t counter = ag_fixedpoint_uq16_0_from_int(1000);
	printf("   Counter value: %d\n", ag_fixedpoint_uq16_0_to_int(counter));

	// Compile-time constant
	const ag_fixedpoint_uq16_0_t MAX_COUNT = AG_FIXEDPOINT_UQ16_0_CONST(65535, 0);
	printf("   Max count: %d\n", ag_fixedpoint_uq16_0_to_int(MAX_COUNT));

#if AG_FIXEDPOINT_ENABLE_FLOAT
	// Float conversion
	ag_fixedpoint_float_t counter_float = ag_fixedpoint_uq16_0_to_float(counter);
	printf("   As float: %.1f\n", (double)counter_float);

	ag_fixedpoint_uq16_0_t from_float = ag_fixedpoint_uq16_0_from_float(1234.56f);
	printf("   From float 1234.56: %d (fractional part truncated)\n\n", ag_fixedpoint_uq16_0_to_int(from_float));
#else
	printf("\n");
#endif

	/*========================================================================*/
	/* Q15.0 - Signed Integer in Q Notation                                  */
	/*========================================================================*/

	printf("2. Q15.0 (Signed Integer)\n");
	printf("   Format: 15 integer bits + sign, 0 fractional bits\n");
	printf("   Range: [-32768, 32767]\n\n");

	// Create from positive integer
	ag_fixedpoint_q15_0_t position = ag_fixedpoint_q15_0_from_int(500);
	printf("   Position: %d\n", ag_fixedpoint_q15_0_to_int(position));

	// Create from negative integer
	ag_fixedpoint_q15_0_t offset = ag_fixedpoint_q15_0_from_int(-250);
	printf("   Offset: %d\n", ag_fixedpoint_q15_0_to_int(offset));

	// Compile-time constants
	const ag_fixedpoint_q15_0_t MIN_VALUE = AG_FIXEDPOINT_Q15_0_CONST(-32768, 0);
	const ag_fixedpoint_q15_0_t MAX_VALUE = AG_FIXEDPOINT_Q15_0_CONST(32767, 0);
	printf("   Min value: %d\n", ag_fixedpoint_q15_0_to_int(MIN_VALUE));
	printf("   Max value: %d\n", ag_fixedpoint_q15_0_to_int(MAX_VALUE));

#if AG_FIXEDPOINT_ENABLE_FLOAT
	// Float conversion
	ag_fixedpoint_float_t position_float = ag_fixedpoint_q15_0_to_float(position);
	printf("   Position as float: %.1f\n", (double)position_float);

	ag_fixedpoint_q15_0_t from_negative_float = ag_fixedpoint_q15_0_from_float(-123.7f);
	printf("   From float -123.7: %d (rounded to nearest)\n\n", ag_fixedpoint_q15_0_to_int(from_negative_float));
#else
	printf("\n");
#endif

	/*========================================================================*/
	/* Mixed Arithmetic Example                                              */
	/*========================================================================*/

	printf("3. Mixed Arithmetic (Conceptual Example)\n");
	printf("   Combining integer and fractional fixed-point types\n\n");

	// Integer count
	ag_fixedpoint_uq16_0_t sample_count = ag_fixedpoint_uq16_0_from_int(100);

	// Fractional scaling factor
	ag_fixedpoint_uq4_12_t scale_factor = AG_FIXEDPOINT_UQ4_12_CONST(1, 5000);	// 1.5

	printf("   Sample count (UQ16.0): %d\n", ag_fixedpoint_uq16_0_to_int(sample_count));
	printf("   Scale factor (UQ4.12): 1.5\n");
	printf("   Note: Multiplication requires careful handling of Q formats\n");
	printf("         UQ16.0 * UQ4.12 = UQ20.12 (would need shift to normalize)\n\n");

	/*========================================================================*/
	/* Practical Use Case: Position Control                                  */
	/*========================================================================*/

	printf("4. Practical Use Case: Motor Position Control\n\n");

	// Current position (encoder counts)
	ag_fixedpoint_q15_0_t current_pos = ag_fixedpoint_q15_0_from_int(5000);

	// Target position
	ag_fixedpoint_q15_0_t target_pos = ag_fixedpoint_q15_0_from_int(8000);

	// Calculate error (in integer domain)
	int32_t error = ag_fixedpoint_q15_0_to_int(target_pos) - ag_fixedpoint_q15_0_to_int(current_pos);

	printf("   Current position: %d counts\n", ag_fixedpoint_q15_0_to_int(current_pos));
	printf("   Target position:  %d counts\n", ag_fixedpoint_q15_0_to_int(target_pos));
	printf("   Position error:   %d counts\n", error);

	// Proportional gain (fractional)
	ag_fixedpoint_q4_11_t kp = AG_FIXEDPOINT_Q4_11_CONST(0, 1500);	// 0.15

	printf("   Proportional gain: 0.15\n");
	printf("   Control output calculation: error * Kp\n");
	printf("   (Requires proper Q format multiplication)\n\n");

	/*========================================================================*/
	/* Summary                                                                */
	/*========================================================================*/

	printf("==============================================\n");
	printf("  Summary\n");
	printf("==============================================\n");
	printf("Q15.0 and UQ16.0 are useful for:\n");
	printf("  - API consistency in mixed fixed-point systems\n");
	printf("  - Explicit Q notation documentation\n");
	printf("  - Type safety in arithmetic operations\n");
	printf("  - Integer counters, positions, indices\n");
	printf("\n");
	printf("Note: These are identical to int16_t/uint16_t\n");
	printf("      but document intent clearly in Q notation.\n");
	printf("==============================================\n");

	return 0;
}
