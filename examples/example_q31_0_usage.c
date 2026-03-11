/**
 * @file example_q31_0_usage.c
 * @brief Example usage of 32-bit Q notation integer types (UQ32.0 and Q31.0)
 *
 * This file demonstrates practical applications of representing standard
 * 32-bit integers using Q notation for API consistency.
 *
 * @author Alex Giovannini
 * @date 2026-01-19
 */

#include <stdio.h>

#include "ag_fixedpoint/ag_fixedpoint.h"


/**
 * @brief Example: Large counter values (e.g., timestamps, packet counts)
 */
void example_large_counters(void) {
	printf("\n=== Large Counters (UQ32.0) ===\n");

	// System tick counter (milliseconds since boot)
	ag_fixedpoint_uq32_0_t tick_count = AG_FIXEDPOINT_UQ32_0_CONST(1000000, 0);
	printf("Tick count: %u ms (%.1f seconds)\n", tick_count, (float)tick_count / 1000.0f);

	// Network packet counter
	ag_fixedpoint_uq32_0_t packet_count = ag_fixedpoint_uq32_0_from_int(2500000);
	printf("Packets received: %u\n", packet_count);

	// File size in bytes
	ag_fixedpoint_uq32_0_t file_size = AG_FIXEDPOINT_UQ32_0_CONST(104857600, 0);  // 100 MB
	printf("File size: %u bytes (%.2f MB)\n", file_size, (float)file_size / (1024.0f * 1024.0f));

	// Maximum 32-bit unsigned value
	ag_fixedpoint_uq32_0_t max_value = AG_FIXEDPOINT_UQ32_0_CONST(4294967295U, 0);
	printf("Max UQ32.0 value: %u\n", max_value);
}

/**
 * @brief Example: High-resolution encoder positions
 */
void example_encoder_positions(void) {
	printf("\n=== Encoder Positions (Q31.0) ===\n");

	// Absolute encoder position (32-bit signed)
	ag_fixedpoint_q31_0_t encoder_pos = AG_FIXEDPOINT_Q31_0_CONST(1000000, 0);
	printf("Encoder position: %d counts\n", ag_fixedpoint_q31_0_to_int(encoder_pos));

	// Relative position change (negative values)
	ag_fixedpoint_q31_0_t delta = AG_FIXEDPOINT_Q31_0_CONST(-5000, 0);
	printf("Position delta: %d counts\n", ag_fixedpoint_q31_0_to_int(delta));

	// Large range encoder (e.g., multi-turn absolute encoder)
	ag_fixedpoint_q31_0_t multiturn_pos = AG_FIXEDPOINT_Q31_0_CONST(100000000, 0);
	printf("Multi-turn position: %d counts (%.2f rotations @ 1M counts/rev)\n",
		   ag_fixedpoint_q31_0_to_int(multiturn_pos), (float)multiturn_pos / 1000000.0f);
}

/**
 * @brief Example: Timestamp differences and intervals
 */
void example_timestamps(void) {
	printf("\n=== Timestamp Operations (UQ32.0, Q31.0) ===\n");

	// Current timestamp (microseconds)
	ag_fixedpoint_uq32_0_t timestamp1 = AG_FIXEDPOINT_UQ32_0_CONST(1000000, 0);
	ag_fixedpoint_uq32_0_t timestamp2 = AG_FIXEDPOINT_UQ32_0_CONST(1005000, 0);

	printf("Timestamp 1: %u μs\n", timestamp1);
	printf("Timestamp 2: %u μs\n", timestamp2);

	// Calculate time difference (can be negative if out of order)
	ag_fixedpoint_q31_0_t time_diff = (int32_t)timestamp2 - (int32_t)timestamp1;
	printf("Time difference: %d μs (%.3f ms)\n", ag_fixedpoint_q31_0_to_int(time_diff), (float)time_diff / 1000.0f);

	// Large time interval (hours in seconds)
	ag_fixedpoint_uq32_0_t uptime_seconds = AG_FIXEDPOINT_UQ32_0_CONST(86400, 0);  // 24 hours
	printf("System uptime: %u seconds (%.1f hours)\n", uptime_seconds, (float)uptime_seconds / 3600.0f);
}

/**
 * @brief Example: Memory addresses and sizes
 */
void example_memory_operations(void) {
	printf("\n=== Memory Operations (UQ32.0) ===\n");

	// Memory address offset
	ag_fixedpoint_uq32_0_t base_addr = AG_FIXEDPOINT_UQ32_0_CONST(0x20000000, 0);
	ag_fixedpoint_uq32_0_t offset = AG_FIXEDPOINT_UQ32_0_CONST(0x1000, 0);

	printf("Base address: 0x%08X\n", base_addr);
	printf("Offset: 0x%08X (%u bytes)\n", offset, offset);
	printf("Target address: 0x%08X\n", base_addr + offset);

	// Buffer sizes
	ag_fixedpoint_uq32_0_t buffer_size = AG_FIXEDPOINT_UQ32_0_CONST(65536, 0);	 // 64 KB
	ag_fixedpoint_uq32_0_t total_size = AG_FIXEDPOINT_UQ32_0_CONST(1048576, 0);	 // 1 MB

	printf("Buffer size: %u bytes (%.1f KB)\n", buffer_size, (float)buffer_size / 1024.0f);
	printf("Total size: %u bytes (%.1f MB)\n", total_size, (float)total_size / (1024.0f * 1024.0f));
}

/**
 * @brief Example: Accumulation and saturation
 */
void example_accumulation(void) {
	printf("\n=== Accumulation Example (UQ32.0) ===\n");

	// Accumulate sensor readings
	ag_fixedpoint_uq32_0_t accumulator = AG_FIXEDPOINT_UQ32_0_CONST(0, 0);

	printf("Initial accumulator: %u\n", accumulator);

	// Add multiple samples
	for (int i = 0; i < 5; i++) {
		ag_fixedpoint_uq32_0_t sample = ag_fixedpoint_uq32_0_from_int(1000 + i * 100);
		accumulator += sample;
		printf("After sample %d: accumulator = %u\n", i + 1, accumulator);
	}

	// Calculate average
	uint32_t average = accumulator / 5;
	printf("Average value: %u\n", average);
}

/**
 * @brief Example: Comparison with smaller integer types
 */
void example_memory_efficiency(void) {
	printf("\n=== Memory Efficiency Comparison ===\n");

	// Array sizes for comparison
	const int num_values = 1000;

	printf("Storage for %d values:\n", num_values);
	printf("  uint8_t  (UQ8.0):  %d bytes\n", num_values * 1);
	printf("  uint16_t (UQ16.0): %d bytes\n", num_values * 2);
	printf("  uint32_t (UQ32.0): %d bytes\n", num_values * 4);

	printf("\nUse cases by type size:\n");
	printf("  UQ8.0:  RGB values, small flags (saves 75%% vs UQ32.0)\n");
	printf("  UQ16.0: Counters, basic sensors (saves 50%% vs UQ32.0)\n");
	printf("  UQ32.0: Large ranges, timestamps, addresses\n");

	// Example: When to use Q31.0 vs UQ32.0
	printf("\nSigned vs Unsigned:\n");
	printf("  UQ32.0 range: [0, 4,294,967,295]\n");
	printf("  Q31.0 range:  [-2,147,483,648, 2,147,483,647]\n");
	printf("\nUse Q31.0 when you need:\n");
	printf("  - Negative values (deltas, offsets)\n");
	printf("  - Bidirectional measurements\n");
	printf("  - Compatibility with int32_t\n");
}

/**
 * @brief Example: Practical sensor application
 */
void example_sensor_application(void) {
	printf("\n=== Practical Application: Data Logger ===\n");

	// System uptime in seconds (UQ32.0)
	ag_fixedpoint_uq32_0_t system_time = AG_FIXEDPOINT_UQ32_0_CONST(3600, 0);  // 1 hour

	// Sample counter (UQ32.0)
	ag_fixedpoint_uq32_0_t sample_count = AG_FIXEDPOINT_UQ32_0_CONST(36000, 0);	 // 10 Hz for 1 hour

	// Temperature offset from calibration (Q31.0, signed)
	ag_fixedpoint_q31_0_t temp_offset = AG_FIXEDPOINT_Q31_0_CONST(-50, 0);	// -5.0°C in 0.1°C units

	printf("Data Logger Status:\n");
	printf("  System time: %u seconds (%.2f hours)\n", system_time, (float)system_time / 3600.0f);
	printf("  Samples collected: %u\n", sample_count);
	printf("  Temperature offset: %d (%.1f °C)\n", ag_fixedpoint_q31_0_to_int(temp_offset), (float)temp_offset / 10.0f);
	printf("  Sample rate: %.2f Hz\n", (float)sample_count / (float)system_time);
}

int main(void) {
	printf("========================================\n");
	printf("  AG Fixed-Point Library Examples\n");
	printf("  32-bit Integer Q Notation (UQ32.0, Q31.0)\n");
	printf("========================================\n");

	example_large_counters();
	example_encoder_positions();
	example_timestamps();
	example_memory_operations();
	example_accumulation();
	example_memory_efficiency();
	example_sensor_application();

	printf("\n========================================\n");
	printf("  Examples Complete\n");
	printf("========================================\n");

	return 0;
}

/**
 * @par Practical Applications for 32-bit Q Notation:
 *
 * UQ32.0 (uint32_t) Use Cases:
 * - System tick counters (millisecond/microsecond timestamps)
 * - Network statistics (packet counts, byte counters)
 * - File sizes and memory addresses
 * - High-speed event counters
 * - Cryptographic nonce values
 * - Large array indices
 * - Flash memory addresses
 *
 * Q31.0 (int32_t) Use Cases:
 * - High-resolution encoder positions (±2 billion counts)
 * - Time differences and intervals (can be negative)
 * - Position deltas in motion control
 * - Large-range sensor offsets
 * - Bidirectional accumulation
 * - Coordinate systems requiring negative values
 * - Financial calculations (cents, avoiding float errors)
 *
 * @par When to Choose 32-bit vs Smaller Types:
 *
 * Choose UQ32.0/Q31.0 when:
 * - Range exceeds 16-bit limits (>65535 unsigned, >32767 signed)
 * - Overflow protection critical (e.g., timestamps that run for days)
 * - Precision requirements exceed smaller types
 * - Interfacing with 32-bit hardware registers
 * - Memory is not constrained (desktop/server applications)
 *
 * Choose smaller types (UQ8.0, UQ16.0, Q15.0) when:
 * - Memory is constrained (embedded systems)
 * - Value range is limited
 * - Processing arrays of many values
 * - Cache efficiency matters
 * - Bandwidth limited (network/storage)
 */
