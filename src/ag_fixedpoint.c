/**
 * @file ag_fixedpoint.c
 * @brief AG Fixed-Point Arithmetic Library - Implementation
 *
 * This file contains the implementation of floating-point conversion functions
 * and other non-inline utility functions for the fixed-point library.
 *
 * The floating-point conversions use the configured type (ag_fixedpoint_float_t)
 * which can be either 32-bit or 64-bit IEEE-754 format, automatically mapped
 * to the appropriate native C type based on compile-time detection.
 *
 * @note The core fixed-point operations are implemented as inline functions
 *       in the header file for maximum performance.
 *
 * @author Alex Giovannini
 * @date 2026-01-18
 * @version 1.0.0
 */

#include "ag_fixedpoint/ag_fixedpoint.h"

/*============================================================================*/
/* FLOATING-POINT CONVERSION IMPLEMENTATIONS                                 */
/*============================================================================*/

#if AG_FIXEDPOINT_ENABLE_FLOAT

/*
 * Note: All conversion functions use ag_fixedpoint_float_t, which resolves to
 * either float32_t or float64_t based on AG_FIXEDPOINT_FLOAT_TYPE configuration.
 *
 * The scaling factors are chosen appropriately for each fixed-point format:
 * - UQ12.4: 2^4 = 16
 * - UQ10.6: 2^6 = 64
 * - UQ6.10: 2^10 = 1024
 * - UQ4.12: 2^12 = 4096
 * - Q4.11: 2^11 = 2048
 * - Q3.12: 2^12 = 4096
 * - Q3.28: 2^28 = 268435456
 */

/*----------------------------------------------------------------------------*/
/* UQ12.4 <-> float conversions                                              */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert UQ12.4 fixed-point to floating-point
 *
 * @param[in] value UQ12.4 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU - uses floating-point division
 */
ag_fixedpoint_float_t ag_fixedpoint_uq12_4_to_float(ag_fixedpoint_uq12_4_t value) {
	return (ag_fixedpoint_float_t)value / 16.0;
}

/**
 * @brief Convert floating-point to UQ12.4 fixed-point
 *
 * @param[in] value Floating-point value
 * @return UQ12.4 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU - uses floating-point multiplication
 * @note Values are rounded using round-half-up method
 */
ag_fixedpoint_uq12_4_t ag_fixedpoint_uq12_4_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_uq12_4_t)(value * 16.0 + 0.5);
}

/*----------------------------------------------------------------------------*/
/* UQ10.6 <-> float conversions                                              */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert UQ10.6 fixed-point to floating-point
 *
 * @param[in] value UQ10.6 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_float_t ag_fixedpoint_uq10_6_to_float(ag_fixedpoint_uq10_6_t value) {
	return (ag_fixedpoint_float_t)value / 64.0;
}

/**
 * @brief Convert floating-point to UQ10.6 fixed-point
 *
 * @param[in] value Floating-point value
 * @return UQ10.6 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_uq10_6_t ag_fixedpoint_uq10_6_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_uq10_6_t)(value * 64.0 + 0.5);
}

/*----------------------------------------------------------------------------*/
/* UQ6.10 <-> float conversions                                              */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert UQ6.10 fixed-point to floating-point
 *
 * @param[in] value UQ6.10 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_float_t ag_fixedpoint_uq6_10_to_float(ag_fixedpoint_uq6_10_t value) {
	return (ag_fixedpoint_float_t)value / 1024.0;
}

/**
 * @brief Convert floating-point to UQ6.10 fixed-point
 *
 * @param[in] value Floating-point value
 * @return UQ6.10 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_uq6_10_t ag_fixedpoint_uq6_10_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_uq6_10_t)(value * 1024.0 + 0.5);
}

/*----------------------------------------------------------------------------*/
/* UQ4.12 <-> float conversions                                              */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert UQ4.12 fixed-point to floating-point
 *
 * @param[in] value UQ4.12 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_float_t ag_fixedpoint_uq4_12_to_float(ag_fixedpoint_uq4_12_t value) {
	return (ag_fixedpoint_float_t)value / 4096.0;
}

/**
 * @brief Convert floating-point to UQ4.12 fixed-point
 *
 * @param[in] value Floating-point value
 * @return UQ4.12 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_uq4_12_t ag_fixedpoint_uq4_12_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_uq4_12_t)(value * 4096.0 + 0.5);
}

/*----------------------------------------------------------------------------*/
/* Q4.11 <-> float conversions                                               */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert Q4.11 fixed-point to floating-point
 *
 * @param[in] value Q4.11 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_float_t ag_fixedpoint_q4_11_to_float(ag_fixedpoint_q4_11_t value) {
	return (ag_fixedpoint_float_t)value / 2048.0;
}

/**
 * @brief Convert floating-point to Q4.11 fixed-point
 *
 * @param[in] value Floating-point value
 * @return Q4.11 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU
 * @note Uses round-half-up for positive, round-half-down for negative
 */
ag_fixedpoint_q4_11_t ag_fixedpoint_q4_11_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_q4_11_t)(value * 2048.0 + (value >= 0.0 ? 0.5 : -0.5));
}

/*----------------------------------------------------------------------------*/
/* Q3.12 <-> float conversions                                               */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert Q3.12 fixed-point to floating-point
 *
 * @param[in] value Q3.12 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_float_t ag_fixedpoint_q3_12_to_float(ag_fixedpoint_q3_12_t value) {
	return (ag_fixedpoint_float_t)value / 4096.0;
}

/**
 * @brief Convert floating-point to Q3.12 fixed-point
 *
 * @param[in] value Floating-point value
 * @return Q3.12 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU
 */
ag_fixedpoint_q3_12_t ag_fixedpoint_q3_12_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_q3_12_t)(value * 4096.0 + (value >= 0.0 ? 0.5 : -0.5));
}

/*----------------------------------------------------------------------------*/
/* Q3.28 <-> float conversions                                               */
/*----------------------------------------------------------------------------*/

/**
 * @brief Convert Q3.28 fixed-point to floating-point
 *
 * @param[in] value Q3.28 fixed-point value
 * @return Floating-point representation
 *
 * @warning SLOW on MCUs without FPU
 * @warning When using AG_FIXEDPOINT_FLOAT_TYPE=32, precision loss will occur
 *          since float32 has only 23 mantissa bits but Q3.28 has 28 fractional
 *          bits. Use AG_FIXEDPOINT_FLOAT_TYPE=64 for full precision.
 */
ag_fixedpoint_float_t ag_fixedpoint_q3_28_to_float(ag_fixedpoint_q3_28_t value) {
	return (ag_fixedpoint_float_t)value / 268435456.0;
}

/**
 * @brief Convert floating-point to Q3.28 fixed-point
 *
 * @param[in] value Floating-point value
 * @return Q3.28 fixed-point representation
 *
 * @warning SLOW on MCUs without FPU
 * @warning When using AG_FIXEDPOINT_FLOAT_TYPE=32, precision loss may occur
 *          in the conversion. Use AG_FIXEDPOINT_FLOAT_TYPE=64 for best results.
 */
ag_fixedpoint_q3_28_t ag_fixedpoint_q3_28_from_float(ag_fixedpoint_float_t value) {
	return (ag_fixedpoint_q3_28_t)(value * 268435456.0 + (value >= 0.0 ? 0.5 : -0.5));
}

#endif /* AG_FIXEDPOINT_ENABLE_FLOAT */
