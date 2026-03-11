/**
 * @file example_q7_0_usage.c
 * @brief Example usage of Q7.0 and UQ8.0 types (8-bit integers)
 *
 * Demonstrates how to use 8-bit integer fixed-point types in Q notation.
 * These are particularly useful for embedded systems with limited memory.
 *
 * @author Alex Giovannini
 * @date 2026-01-19
 */

#include <stdio.h>

#include "ag_fixedpoint/ag_fixedpoint.h"


int main(void) {
	printf("==============================================\n");
	printf("  Q7.0 and UQ8.0 Usage Examples (8-bit)\n");
	printf("==============================================\n\n");

	/*========================================================================*/
	/* UQ8.0 - Unsigned 8-bit Integer in Q Notation                          */
	/*========================================================================*/

	printf("1. UQ8.0 (Unsigned 8-bit Integer)\n");
	printf("   Format: 8 integer bits, 0 fractional bits\n");
	printf("   Range: [0, 255]\n");
	printf("   Storage: 1 byte\n\n");

	// Create from integer
	ag_fixedpoint_uq8_0_t brightness = ag_fixedpoint_uq8_0_from_int(128);
	printf("   LED brightness: %d/255\n", ag_fixedpoint_uq8_0_to_int(brightness));

	// Compile-time constants
	const ag_fixedpoint_uq8_0_t MIN_BRIGHTNESS = AG_FIXEDPOINT_UQ8_0_CONST(0, 0);
	const ag_fixedpoint_uq8_0_t MAX_BRIGHTNESS = AG_FIXEDPOINT_UQ8_0_CONST(255, 0);
	const ag_fixedpoint_uq8_0_t HALF_BRIGHTNESS = AG_FIXEDPOINT_UQ8_0_CONST(127, 0);

	printf("   Min: %d, Half: %d, Max: %d\n", ag_fixedpoint_uq8_0_to_int(MIN_BRIGHTNESS),
		   ag_fixedpoint_uq8_0_to_int(HALF_BRIGHTNESS), ag_fixedpoint_uq8_0_to_int(MAX_BRIGHTNESS));

#if AG_FIXEDPOINT_ENABLE_FLOAT
	// Float conversion
	ag_fixedpoint_float_t brightness_percent = ag_fixedpoint_uq8_0_to_float(brightness) / 255.0f * 100.0f;
	printf("   Brightness: %.1f%%\n", (double)brightness_percent);

	// RGB color channels
	ag_fixedpoint_uq8_0_t red = ag_fixedpoint_uq8_0_from_float(255.0f);
	ag_fixedpoint_uq8_0_t green = ag_fixedpoint_uq8_0_from_float(128.5f);  // Rounds to 129
	ag_fixedpoint_uq8_0_t blue = ag_fixedpoint_uq8_0_from_float(0.0f);

	printf("   RGB Color: (%d, %d, %d)\n\n", ag_fixedpoint_uq8_0_to_int(red), ag_fixedpoint_uq8_0_to_int(green),
		   ag_fixedpoint_uq8_0_to_int(blue));
#else
	printf("\n");
#endif

	/*========================================================================*/
	/* Q7.0 - Signed 8-bit Integer in Q Notation                             */
	/*========================================================================*/

	printf("2. Q7.0 (Signed 8-bit Integer)\n");
	printf("   Format: 7 integer bits + sign, 0 fractional bits\n");
	printf("   Range: [-128, 127]\n");
	printf("   Storage: 1 byte\n\n");

	// Temperature sensor example
	ag_fixedpoint_q7_0_t room_temp = ag_fixedpoint_q7_0_from_int(22);
	ag_fixedpoint_q7_0_t freezer_temp = ag_fixedpoint_q7_0_from_int(-18);
	ag_fixedpoint_q7_0_t oven_temp = ag_fixedpoint_q7_0_from_int(100);

	printf("   Room temperature:    %d°C\n", ag_fixedpoint_q7_0_to_int(room_temp));
	printf("   Freezer temperature: %d°C\n", ag_fixedpoint_q7_0_to_int(freezer_temp));
	printf("   Oven temperature:    %d°C\n", ag_fixedpoint_q7_0_to_int(oven_temp));

	// Compile-time constants
	const ag_fixedpoint_q7_0_t MIN_TEMP = AG_FIXEDPOINT_Q7_0_CONST(-128, 0);
	const ag_fixedpoint_q7_0_t MAX_TEMP = AG_FIXEDPOINT_Q7_0_CONST(127, 0);

	printf("   Sensor range: [%d°C, %d°C]\n", ag_fixedpoint_q7_0_to_int(MIN_TEMP), ag_fixedpoint_q7_0_to_int(MAX_TEMP));

#if AG_FIXEDPOINT_ENABLE_FLOAT
	// Float conversion
	ag_fixedpoint_q7_0_t temp_from_float = ag_fixedpoint_q7_0_from_float(25.7f);
	printf("   From float 25.7°C: %d°C (rounded)\n\n", ag_fixedpoint_q7_0_to_int(temp_from_float));
#else
	printf("\n");
#endif

	/*========================================================================*/
	/* Practical Use Case: ADC Processing                                    */
	/*========================================================================*/

	printf("3. Practical Use Case: 8-bit ADC Processing\n\n");

	// Simulate 8-bit ADC readings (0-255)
	ag_fixedpoint_uq8_0_t adc_samples[5] = {ag_fixedpoint_uq8_0_from_int(120), ag_fixedpoint_uq8_0_from_int(125),
											ag_fixedpoint_uq8_0_from_int(122), ag_fixedpoint_uq8_0_from_int(128),
											ag_fixedpoint_uq8_0_from_int(123)};

	// Calculate average (integer domain)
	int32_t sum = 0;
	for (int i = 0; i < 5; i++) {
		sum += ag_fixedpoint_uq8_0_to_int(adc_samples[i]);
	}
	ag_fixedpoint_uq8_0_t average = ag_fixedpoint_uq8_0_from_int(sum / 5);

	printf("   ADC samples: ");
	for (int i = 0; i < 5; i++) {
		printf("%d ", ag_fixedpoint_uq8_0_to_int(adc_samples[i]));
	}
	printf("\n   Average: %d\n\n", ag_fixedpoint_uq8_0_to_int(average));

	/*========================================================================*/
	/* Practical Use Case: Delta/Offset Calculation                          */
	/*========================================================================*/

	printf("4. Practical Use Case: Temperature Delta\n\n");

	ag_fixedpoint_q7_0_t current_temp = ag_fixedpoint_q7_0_from_int(25);
	ag_fixedpoint_q7_0_t setpoint = ag_fixedpoint_q7_0_from_int(22);

	// Calculate temperature error
	int32_t temp_error = ag_fixedpoint_q7_0_to_int(current_temp) - ag_fixedpoint_q7_0_to_int(setpoint);

	printf("   Current temperature: %d°C\n", ag_fixedpoint_q7_0_to_int(current_temp));
	printf("   Setpoint:            %d°C\n", ag_fixedpoint_q7_0_to_int(setpoint));
	printf("   Error:               %+d°C\n", temp_error);

	if (temp_error > 0) {
		printf("   Action: Turn on cooling\n\n");
	} else if (temp_error < 0) {
		printf("   Action: Turn on heating\n\n");
	} else {
		printf("   Action: Maintain\n\n");
	}

	/*========================================================================*/
	/* Memory Efficiency Comparison                                          */
	/*========================================================================*/

	printf("5. Memory Efficiency\n\n");

	printf("   Size comparison:\n");
	printf("   - UQ8.0/Q7.0:   %zu byte(s)\n", sizeof(ag_fixedpoint_uq8_0_t));
	printf("   - UQ16.0/Q15.0: %zu byte(s)\n", sizeof(ag_fixedpoint_uq16_0_t));
	printf("   - int32_t:      %zu bytes\n\n", sizeof(int32_t));

	printf("   Example: Array of 100 sensor values\n");
	printf("   - Using UQ8.0:  %zu bytes\n", 100 * sizeof(ag_fixedpoint_uq8_0_t));
	printf("   - Using UQ16.0: %zu bytes\n", 100 * sizeof(ag_fixedpoint_uq16_0_t));
	printf("   - Using int32_t: %zu bytes\n", 100 * sizeof(int32_t));
	printf("   Memory saved with UQ8.0: %zu bytes (vs int32_t)\n\n",
		   100 * (sizeof(int32_t) - sizeof(ag_fixedpoint_uq8_0_t)));

	/*========================================================================*/
	/* Typical Applications                                                  */
	/*========================================================================*/

	printf("6. Typical Applications for 8-bit Types\n\n");

	printf("   UQ8.0 (0-255):\n");
	printf("   - PWM duty cycle (0-100%%)\n");
	printf("   - RGB color channels\n");
	printf("   - LED brightness levels\n");
	printf("   - Volume/gain settings\n");
	printf("   - Percentage values\n");
	printf("   - 8-bit ADC readings\n\n");

	printf("   Q7.0 (-128 to 127):\n");
	printf("   - Temperature sensors (°C)\n");
	printf("   - Small offsets/deltas\n");
	printf("   - Joystick positions\n");
	printf("   - Audio levels (dB)\n");
	printf("   - Accelerometer data\n");
	printf("   - Error signals\n\n");

	/*========================================================================*/
	/* Summary                                                                */
	/*========================================================================*/

	printf("==============================================\n");
	printf("  Summary\n");
	printf("==============================================\n");
	printf("8-bit Q notation types are ideal for:\n");
	printf("  ✓ Memory-constrained embedded systems\n");
	printf("  ✓ Sensor data within byte range\n");
	printf("  ✓ Communication protocols (single byte)\n");
	printf("  ✓ Color/brightness values\n");
	printf("  ✓ Small temperature ranges\n");
	printf("  ✓ Array storage optimization\n");
	printf("\n");
	printf("Benefits:\n");
	printf("  ✓ 75%% memory savings vs int32_t\n");
	printf("  ✓ Cache-friendly (4x more values per cache line)\n");
	printf("  ✓ Type-safe Q notation API\n");
	printf("  ✓ Perfect for 8-bit MCUs and protocols\n");
	printf("==============================================\n");

	return 0;
}
