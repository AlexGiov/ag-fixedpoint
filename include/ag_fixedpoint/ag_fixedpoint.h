/**
 * @file ag_fixedpoint.h
 * @brief AG Fixed-Point Arithmetic Library - Main API
 *
 * This library provides efficient fixed-point arithmetic operations optimized
 * for embedded systems and microcontrollers without Floating-Point Unit (FPU).
 *
 * The library implements Q notation fixed-point types with various integer
 * and fractional bit configurations, supporting both signed and unsigned formats.
 *
 * @par Key Features:
 * - Zero runtime overhead for constant conversions (compile-time evaluation)
 * - Pure integer arithmetic - no FPU required
 * - Optional floating-point conversions (disabled by default)
 * - Inline functions for maximum performance
 * - Comprehensive Doxygen documentation
 *
 * @par Supported Fixed-Point Formats:
 * - UQ12.4  - Unsigned 12-bit integer, 4-bit fractional (16-bit total)
 * - UQ10.6  - Unsigned 10-bit integer, 6-bit fractional (16-bit total)
 * - UQ6.10  - Unsigned 6-bit integer, 10-bit fractional (16-bit total)
 * - UQ4.12  - Unsigned 4-bit integer, 12-bit fractional (16-bit total)
 * - Q4.11   - Signed 4-bit integer, 11-bit fractional (16-bit total)
 * - Q3.12   - Signed 3-bit integer, 12-bit fractional (16-bit total)
 * - Q3.28   - Signed 3-bit integer, 28-bit fractional (32-bit total)
 *
 * @par Usage Example:
 * @code
 * #include "ag_fixedpoint/ag_fixedpoint.h"
 *
 * // Create fixed-point values from integers
 * ag_fixedpoint_uq12_4_t temp = ag_fixedpoint_uq12_4_from_int(25);
 *
 * // Use compile-time constants
 * ag_fixedpoint_q4_11_t coefficient = AG_FIXEDPOINT_Q4_11_CONST(1, 5000);
 *
 * // Convert back to integer
 * int32_t result = ag_fixedpoint_uq12_4_to_int(temp);
 * @endcode
 *
 * @author Alex Giovannini
 * @date 2026-01-18
 * @version 1.0.0
 *
 * @see ag_fixedpoint_cfg.h for configuration options
 */

#ifndef AG_FIXEDPOINT_H
#define AG_FIXEDPOINT_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* INCLUDES                                                                   */
/*============================================================================*/

#include <stdbool.h>
#include <stdint.h>

#include "ag_fixedpoint_cfg.h"

#if AG_FIXEDPOINT_ENABLE_FLOAT
#include <float.h>

/*----------------------------------------------------------------------------*/
/* Automatic IEEE-754 Type Detection                                         */
/*----------------------------------------------------------------------------*/

/**
 * @brief Detect and define IEEE-754 compliant floating-point types
 *
 * The library automatically detects which native C floating-point types
 * (float, double, long double) correspond to IEEE-754 binary32 and binary64
 * formats at compile-time.
 *
 * This ensures portability across different platforms where the sizes of
 * float and double may vary.
 */

/* Detect IEEE-754 binary32 (32-bit, single precision) */
#if defined(FLT_MANT_DIG) && defined(FLT_MAX_EXP) && (FLT_RADIX == 2) && (FLT_MANT_DIG == 24) && (FLT_MAX_EXP == 128)
/* float is IEEE-754 binary32 */
_Static_assert(sizeof(float) == 4, "float must be 32-bit for IEEE-754 binary32");
typedef float float32_t;
#define AG_FIXEDPOINT_HAS_FLOAT32 1
#elif defined(DBL_MANT_DIG) && defined(DBL_MAX_EXP) && (FLT_RADIX == 2) && (DBL_MANT_DIG == 24) && (DBL_MAX_EXP == 128)
/* double is IEEE-754 binary32 (rare, but check) */
_Static_assert(sizeof(double) == 4, "double must be 32-bit for IEEE-754 binary32");
typedef double float32_t;
#define AG_FIXEDPOINT_HAS_FLOAT32 1
#else
/* No 32-bit IEEE-754 type available */
#define AG_FIXEDPOINT_HAS_FLOAT32 0
#endif

/* Detect IEEE-754 binary64 (64-bit, double precision) */
#if defined(DBL_MANT_DIG) && defined(DBL_MAX_EXP) && (FLT_RADIX == 2) && (DBL_MANT_DIG == 53) && (DBL_MAX_EXP == 1024)
/* double is IEEE-754 binary64 */
_Static_assert(sizeof(double) == 8, "double must be 64-bit for IEEE-754 binary64");
typedef double float64_t;
#define AG_FIXEDPOINT_HAS_FLOAT64 1
#elif defined(LDBL_MANT_DIG) && defined(LDBL_MAX_EXP) && (FLT_RADIX == 2) && (LDBL_MANT_DIG == 53) && \
	(LDBL_MAX_EXP == 1024)
/* long double is IEEE-754 binary64 (some platforms) */
_Static_assert(sizeof(long double) == 8, "long double must be 64-bit for IEEE-754 binary64");
typedef long double float64_t;
#define AG_FIXEDPOINT_HAS_FLOAT64 1
#else
/* No 64-bit IEEE-754 type available */
#define AG_FIXEDPOINT_HAS_FLOAT64 0
#endif

/*----------------------------------------------------------------------------*/
/* Floating-Point Type Selection                                             */
/*----------------------------------------------------------------------------*/

/**
 * @typedef ag_fixedpoint_float_t
 * @brief Configurable floating-point type for conversions
 *
 * This type is selected based on AG_FIXEDPOINT_FLOAT_TYPE configuration:
 * - If AG_FIXEDPOINT_FLOAT_TYPE == 32: uses float32_t (IEEE-754 binary32)
 * - If AG_FIXEDPOINT_FLOAT_TYPE == 64: uses float64_t (IEEE-754 binary64)
 *
 * The library automatically maps these to the appropriate native C type
 * (float, double, or long double) based on compile-time detection.
 */
#if AG_FIXEDPOINT_FLOAT_TYPE == 32
#if AG_FIXEDPOINT_HAS_FLOAT32
typedef float32_t ag_fixedpoint_float_t;
#define AG_FIXEDPOINT_FLOAT_BITS 32
#else
#error "AG_FIXEDPOINT_FLOAT_TYPE=32 requested, but no IEEE-754 binary32 type available on this platform"
#endif
#elif AG_FIXEDPOINT_FLOAT_TYPE == 64
#if AG_FIXEDPOINT_HAS_FLOAT64
typedef float64_t ag_fixedpoint_float_t;
#define AG_FIXEDPOINT_FLOAT_BITS 64
#else
#error "AG_FIXEDPOINT_FLOAT_TYPE=64 requested, but no IEEE-754 binary64 type available on this platform"
#endif
#else
#error "AG_FIXEDPOINT_FLOAT_TYPE must be either 32 or 64"
#endif

#endif /* AG_FIXEDPOINT_ENABLE_FLOAT */

/*============================================================================*/
/* VERSION INFORMATION                                                        */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_VERSION Version Information
 * @brief Library version macros
 * @{
 */

/** Major version number */
#define AG_FIXEDPOINT_VERSION_MAJOR 1

/** Minor version number */
#define AG_FIXEDPOINT_VERSION_MINOR 0

/** Patch version number */
#define AG_FIXEDPOINT_VERSION_PATCH 0

/** Version string */
#define AG_FIXEDPOINT_VERSION_STRING "1.0.0"

/** @} */ /* end of AG_FIXEDPOINT_VERSION */

/*============================================================================*/
/* TYPE DEFINITIONS                                                           */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_TYPES Fixed-Point Type Definitions
 * @brief Type definitions for various Q notation formats
 * @{
 */

/**
 * @typedef ag_fixedpoint_uq12_4_t
 * @brief Unsigned fixed-point type with 12 integer bits and 4 fractional bits
 *
 * - Format: UQ12.4
 * - Storage: 16-bit unsigned integer
 * - Range: [0, 4095.9375]
 * - Resolution: 0.0625 (1/16)
 *
 * @par Typical Applications:
 * - Temperature sensors (0-4095°C with 0.0625°C resolution)
 * - ADC values with scaling
 * - Large range measurements
 */
typedef uint16_t ag_fixedpoint_uq12_4_t;

/**
 * @typedef ag_fixedpoint_uq10_6_t
 * @brief Unsigned fixed-point type with 10 integer bits and 6 fractional bits
 *
 * - Format: UQ10.6
 * - Storage: 16-bit unsigned integer
 * - Range: [0, 1023.984375]
 * - Resolution: 0.015625 (1/64)
 *
 * @par Typical Applications:
 * - Sensor readings with moderate range and precision
 * - Percentage values with extended precision
 */
typedef uint16_t ag_fixedpoint_uq10_6_t;

/**
 * @typedef ag_fixedpoint_uq6_10_t
 * @brief Unsigned fixed-point type with 6 integer bits and 10 fractional bits
 *
 * - Format: UQ6.10
 * - Storage: 16-bit unsigned integer
 * - Range: [0, 63.9990234375]
 * - Resolution: 0.0009765625 (1/1024)
 *
 * @par Typical Applications:
 * - Small range measurements with high precision
 * - Normalized values [0, 1] scaled to [0, 63]
 */
typedef uint16_t ag_fixedpoint_uq6_10_t;

/**
 * @typedef ag_fixedpoint_uq4_12_t
 * @brief Unsigned fixed-point type with 4 integer bits and 12 fractional bits
 *
 * - Format: UQ4.12
 * - Storage: 16-bit unsigned integer
 * - Range: [0, 15.999755859375]
 * - Resolution: 0.000244140625 (1/4096)
 *
 * @par Typical Applications:
 * - High-precision fractional values in small range
 * - Coefficients and scaling factors
 * - Normalized values [0, 1] with high precision
 */
typedef uint16_t ag_fixedpoint_uq4_12_t;

/**
 * @typedef ag_fixedpoint_q4_11_t
 * @brief Signed fixed-point type with 4 integer bits and 11 fractional bits
 *
 * - Format: Q4.11
 * - Storage: 16-bit signed integer (two's complement)
 * - Range: [-16, 15.9995117188]
 * - Resolution: 0.00048828125 (1/2048)
 *
 * @par Typical Applications:
 * - Bipolar sensor readings
 * - Control system coefficients
 * - Signal processing values
 */
typedef int16_t ag_fixedpoint_q4_11_t;

/**
 * @typedef ag_fixedpoint_q3_12_t
 * @brief Signed fixed-point type with 3 integer bits and 12 fractional bits
 *
 * - Format: Q3.12
 * - Storage: 16-bit signed integer (two's complement)
 * - Range: [-8, 7.999755859375]
 * - Resolution: 0.000244140625 (1/4096)
 *
 * @par Typical Applications:
 * - Normalized bipolar signals [-1, +1] scaled
 * - High-precision small-range measurements
 * - Audio processing coefficients
 */
typedef int16_t ag_fixedpoint_q3_12_t;

/**
 * @typedef ag_fixedpoint_q3_28_t
 * @brief Signed fixed-point type with 3 integer bits and 28 fractional bits
 *
 * - Format: Q3.28
 * - Storage: 32-bit signed integer (two's complement)
 * - Range: [-8, 7.9999999962747097015380859375]
 * - Resolution: 0.0000000037252902984619140625 (1/268435456)
 *
 * @par Typical Applications:
 * - Very high-precision calculations
 * - Trigonometric function results
 * - DSP filter coefficients
 * - Intermediate calculation results
 */
typedef int32_t ag_fixedpoint_q3_28_t;

/**
 * @typedef ag_fixedpoint_uq16_0_t
 * @brief Unsigned fixed-point type with 16 integer bits and 0 fractional bits
 *
 * - Format: UQ16.0
 * - Storage: 16-bit unsigned integer
 * - Range: [0, 65535]
 * - Resolution: 1
 *
 * @par Typical Applications:
 * - Standard unsigned integers in Q notation
 * - Mixed arithmetic with other Q formats
 * - Counter values, absolute positions
 *
 * @note This is identical to uint16_t but uses Q notation for consistency
 */
typedef uint16_t ag_fixedpoint_uq16_0_t;

/**
 * @typedef ag_fixedpoint_q15_0_t
 * @brief Signed fixed-point type with 15 integer bits and 0 fractional bits
 *
 * - Format: Q15.0
 * - Storage: 16-bit signed integer (two's complement)
 * - Range: [-32768, 32767]
 * - Resolution: 1
 *
 * @par Typical Applications:
 * - Standard signed integers in Q notation
 * - Mixed arithmetic with other Q formats
 * - Relative positions, offsets
 *
 * @note This is identical to int16_t but uses Q notation for consistency
 */
typedef int16_t ag_fixedpoint_q15_0_t;

/**
 * @typedef ag_fixedpoint_uq8_0_t
 * @brief Unsigned fixed-point type with 8 integer bits and 0 fractional bits
 *
 * - Format: UQ8.0
 * - Storage: 8-bit unsigned integer
 * - Range: [0, 255]
 * - Resolution: 1
 *
 * @par Typical Applications:
 * - Byte values in Q notation
 * - Small counters, flags
 * - 8-bit sensor data
 * - Color channel values
 *
 * @note This is identical to uint8_t but uses Q notation for consistency
 */
typedef uint8_t ag_fixedpoint_uq8_0_t;

/**
 * @typedef ag_fixedpoint_q7_0_t
 * @brief Signed fixed-point type with 7 integer bits and 0 fractional bits
 *
 * - Format: Q7.0
 * - Storage: 8-bit signed integer (two's complement)
 * - Range: [-128, 127]
 * - Resolution: 1
 *
 * @par Typical Applications:
 * - Byte-sized signed values in Q notation
 * - Small offsets, deltas
 * - Temperature sensors (°C)
 * - 8-bit signed sensor data
 *
 * @note This is identical to int8_t but uses Q notation for consistency
 */
typedef int8_t ag_fixedpoint_q7_0_t;

/**
 * @typedef ag_fixedpoint_uq32_0_t
 * @brief Unsigned fixed-point type with 32 integer bits and 0 fractional bits
 *
 * - Format: UQ32.0
 * - Storage: 32-bit unsigned integer
 * - Range: [0, 4294967295]
 * - Resolution: 1
 *
 * @par Typical Applications:
 * - Standard unsigned 32-bit integers in Q notation
 * - Large counters, timestamps
 * - File sizes, memory addresses
 * - High-precision accumulators
 *
 * @note This is identical to uint32_t but uses Q notation for consistency
 */
typedef uint32_t ag_fixedpoint_uq32_0_t;

/**
 * @typedef ag_fixedpoint_q31_0_t
 * @brief Signed fixed-point type with 31 integer bits and 0 fractional bits
 *
 * - Format: Q31.0
 * - Storage: 32-bit signed integer (two's complement)
 * - Range: [-2147483648, 2147483647]
 * - Resolution: 1
 *
 * @par Typical Applications:
 * - Standard signed 32-bit integers in Q notation
 * - Large range measurements
 * - Position encoders (high resolution)
 * - Timestamp differences
 *
 * @note This is identical to int32_t but uses Q notation for consistency
 */
typedef int32_t ag_fixedpoint_q31_0_t;

/** @} */ /* end of AG_FIXEDPOINT_TYPES */

/*============================================================================*/
/* COMPILE-TIME CONSTANT MACROS                                              */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_CONST_MACROS Compile-Time Constant Macros
 * @brief Macros for creating fixed-point constants at compile time
 *
 * These macros allow creating fixed-point constants that are fully evaluated
 * by the compiler/preprocessor, resulting in zero runtime overhead.
 *
 * @{
 */

/**
 * @def AG_FIXEDPOINT_UQ12_4_CONST
 * @brief Create a UQ12.4 constant at compile time
 *
 * @param[in] int_part Integer part (0-4095)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return UQ12.4 fixed-point constant
 *
 * @par Example:
 * @code
 * ag_fixedpoint_uq12_4_t pi = AG_FIXEDPOINT_UQ12_4_CONST(3, 1416); // 3.1416
 * @endcode
 *
 * @note The frac_numerator is interpreted as a 4-digit decimal fraction
 *       (i.e., divided by 10000)
 */
#define AG_FIXEDPOINT_UQ12_4_CONST(int_part, frac_numerator) \
	((ag_fixedpoint_uq12_4_t)(((int_part) << 4) + (((frac_numerator) * 16 + 5000) / 10000)))

/**
 * @def AG_FIXEDPOINT_UQ10_6_CONST
 * @brief Create a UQ10.6 constant at compile time
 *
 * @param[in] int_part Integer part (0-1023)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return UQ10.6 fixed-point constant
 */
#define AG_FIXEDPOINT_UQ10_6_CONST(int_part, frac_numerator) \
	((ag_fixedpoint_uq10_6_t)(((int_part) << 6) + (((frac_numerator) * 64 + 5000) / 10000)))

/**
 * @def AG_FIXEDPOINT_UQ6_10_CONST
 * @brief Create a UQ6.10 constant at compile time
 *
 * @param[in] int_part Integer part (0-63)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return UQ6.10 fixed-point constant
 */
#define AG_FIXEDPOINT_UQ6_10_CONST(int_part, frac_numerator) \
	((ag_fixedpoint_uq6_10_t)(((int_part) << 10) + (((frac_numerator) * 1024 + 5000) / 10000)))

/**
 * @def AG_FIXEDPOINT_UQ4_12_CONST
 * @brief Create a UQ4.12 constant at compile time
 *
 * @param[in] int_part Integer part (0-15)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return UQ4.12 fixed-point constant
 */
#define AG_FIXEDPOINT_UQ4_12_CONST(int_part, frac_numerator) \
	((ag_fixedpoint_uq4_12_t)(((int_part) << 12) + (((frac_numerator) * 4096 + 5000) / 10000)))

/**
 * @def AG_FIXEDPOINT_Q4_11_CONST
 * @brief Create a Q4.11 constant at compile time
 *
 * @param[in] int_part Integer part (-16 to 15)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return Q4.11 fixed-point constant
 *
 * @note For negative numbers, use negative int_part
 */
#define AG_FIXEDPOINT_Q4_11_CONST(int_part, frac_numerator)                                                       \
	((ag_fixedpoint_q4_11_t)(((int_part) < 0) ? (((int_part) << 11) - (((frac_numerator) * 2048 + 5000) / 10000)) \
											  : (((int_part) << 11) + (((frac_numerator) * 2048 + 5000) / 10000))))
/**
 * @def AG_FIXEDPOINT_Q3_12_CONST
 * @brief Create a Q3.12 constant at compile time
 *
 * @param[in] int_part Integer part (-8 to 7)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return Q3.12 fixed-point constant
 */
#define AG_FIXEDPOINT_Q3_12_CONST(int_part, frac_numerator)                                                       \
	((ag_fixedpoint_q3_12_t)(((int_part) < 0) ? (((int_part) << 12) - (((frac_numerator) * 4096 + 5000) / 10000)) \
											  : (((int_part) << 12) + (((frac_numerator) * 4096 + 5000) / 10000))))
/**
 * @def AG_FIXEDPOINT_Q3_28_CONST
 * @brief Create a Q3.28 constant at compile time
 *
 * @param[in] int_part Integer part (-8 to 7)
 * @param[in] frac_numerator Fractional numerator (0-9999)
 *
 * @return Q3.28 fixed-point constant
 */
#define AG_FIXEDPOINT_Q3_28_CONST(int_part, frac_numerator)                                                          \
	((ag_fixedpoint_q3_28_t)(((int_part) < 0)                                                                        \
								 ? (((int_part) << 28) - (((int64_t)(frac_numerator) * 268435456LL + 5000) / 10000)) \
								 : (((int_part) << 28) + (((int64_t)(frac_numerator) * 268435456LL + 5000) / 10000))))

/**
 * @def AG_FIXEDPOINT_UQ16_0_CONST
 * @brief Create a UQ16.0 constant at compile time
 *
 * @param[in] int_part Integer value (0-65535)
 * @param[in] frac_numerator Fractional numerator (ignored, must be 0)
 *
 * @return UQ16.0 fixed-point constant
 *
 * @note Since UQ16.0 has no fractional bits, frac_numerator should always be 0
 *
 * @par Example:
 * @code
 * ag_fixedpoint_uq16_0_t count = AG_FIXEDPOINT_UQ16_0_CONST(1000, 0); // 1000
 * @endcode
 */
#define AG_FIXEDPOINT_UQ16_0_CONST(int_part, frac_numerator) ((ag_fixedpoint_uq16_0_t)(int_part))

/**
 * @def AG_FIXEDPOINT_Q15_0_CONST
 * @brief Create a Q15.0 constant at compile time
 *
 * @param[in] int_part Integer value (-32768 to 32767)
 * @param[in] frac_numerator Fractional numerator (ignored, must be 0)
 *
 * @return Q15.0 fixed-point constant
 *
 * @note Since Q15.0 has no fractional bits, frac_numerator should always be 0
 *
 * @par Example:
 * @code
 * ag_fixedpoint_q15_0_t offset = AG_FIXEDPOINT_Q15_0_CONST(-100, 0); // -100
 * @endcode
 */
#define AG_FIXEDPOINT_Q15_0_CONST(int_part, frac_numerator) ((ag_fixedpoint_q15_0_t)(int_part))

/**
 * @def AG_FIXEDPOINT_UQ8_0_CONST
 * @brief Create a UQ8.0 constant at compile time
 *
 * @param[in] int_part Integer value (0-255)
 * @param[in] frac_numerator Fractional numerator (ignored, must be 0)
 *
 * @return UQ8.0 fixed-point constant
 *
 * @note Since UQ8.0 has no fractional bits, frac_numerator should always be 0
 *
 * @par Example:
 * @code
 * ag_fixedpoint_uq8_0_t brightness = AG_FIXEDPOINT_UQ8_0_CONST(128, 0); // 128
 * @endcode
 */
#define AG_FIXEDPOINT_UQ8_0_CONST(int_part, frac_numerator) ((ag_fixedpoint_uq8_0_t)(int_part))

/**
 * @def AG_FIXEDPOINT_Q7_0_CONST
 * @brief Create a Q7.0 constant at compile time
 *
 * @param[in] int_part Integer value (-128 to 127)
 * @param[in] frac_numerator Fractional numerator (ignored, must be 0)
 *
 * @return Q7.0 fixed-point constant
 *
 * @note Since Q7.0 has no fractional bits, frac_numerator should always be 0
 *
 * @par Example:
 * @code
 * ag_fixedpoint_q7_0_t temperature = AG_FIXEDPOINT_Q7_0_CONST(25, 0); // 25°C
 * @endcode
 */
#define AG_FIXEDPOINT_Q7_0_CONST(int_part, frac_numerator) ((ag_fixedpoint_q7_0_t)(int_part))

/**
 * @def AG_FIXEDPOINT_UQ32_0_CONST
 * @brief Create a UQ32.0 constant at compile time
 *
 * @param[in] int_part Integer value (0-4294967295)
 * @param[in] frac_numerator Fractional numerator (ignored, must be 0)
 *
 * @return UQ32.0 fixed-point constant
 *
 * @note Since UQ32.0 has no fractional bits, frac_numerator should always be 0
 *
 * @par Example:
 * @code
 * ag_fixedpoint_uq32_0_t counter = AG_FIXEDPOINT_UQ32_0_CONST(1000000, 0); // 1000000
 * @endcode
 */
#define AG_FIXEDPOINT_UQ32_0_CONST(int_part, frac_numerator) ((ag_fixedpoint_uq32_0_t)(int_part))

/**
 * @def AG_FIXEDPOINT_Q31_0_CONST
 * @brief Create a Q31.0 constant at compile time
 *
 * @param[in] int_part Integer value (-2147483648 to 2147483647)
 * @param[in] frac_numerator Fractional numerator (ignored, must be 0)
 *
 * @return Q31.0 fixed-point constant
 *
 * @note Since Q31.0 has no fractional bits, frac_numerator should always be 0
 *
 * @par Example:
 * @code
 * ag_fixedpoint_q31_0_t position = AG_FIXEDPOINT_Q31_0_CONST(1000000, 0); // 1000000
 * @endcode
 */
#define AG_FIXEDPOINT_Q31_0_CONST(int_part, frac_numerator) ((ag_fixedpoint_q31_0_t)(int_part))

/** @} */ /* end of AG_FIXEDPOINT_CONST_MACROS */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ12.4                                     */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ12_4 UQ12.4 Conversion Functions
 * @brief Conversion functions for UQ12.4 format
 * @{
 */

/**
 * @brief Convert integer to UQ12.4 fixed-point
 *
 * @param[in] value Integer value to convert (0-4095)
 *
 * @return UQ12.4 fixed-point representation
 *
 * @warning Values outside [0, 4095] will overflow (wrapping behavior)
 *
 * @par Example:
 * @code
 * ag_fixedpoint_uq12_4_t temp = ag_fixedpoint_uq12_4_from_int(25); // 25.0
 * @endcode
 */
static inline ag_fixedpoint_uq12_4_t ag_fixedpoint_uq12_4_from_int(int32_t value) {
	return (ag_fixedpoint_uq12_4_t)(value << 4);
}

/**
 * @brief Convert UQ12.4 fixed-point to integer (truncate)
 *
 * @param[in] value UQ12.4 fixed-point value
 *
 * @return Integer part (fractional part discarded)
 *
 * @par Example:
 * @code
 * ag_fixedpoint_uq12_4_t val = AG_FIXEDPOINT_UQ12_4_CONST(25, 7500); // 25.75
 * int32_t result = ag_fixedpoint_uq12_4_to_int(val); // result = 25
 * @endcode
 */
static inline int32_t ag_fixedpoint_uq12_4_to_int(ag_fixedpoint_uq12_4_t value) { return (int32_t)(value >> 4); }

/** @} */ /* end of AG_FIXEDPOINT_UQ12_4 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ10.6                                     */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ10_6 UQ10.6 Conversion Functions
 * @brief Conversion functions for UQ10.6 format
 * @{
 */

/**
 * @brief Convert integer to UQ10.6 fixed-point
 *
 * @param[in] value Integer value to convert (0-1023)
 * @return UQ10.6 fixed-point representation
 * @warning Values outside [0, 1023] will overflow
 */
static inline ag_fixedpoint_uq10_6_t ag_fixedpoint_uq10_6_from_int(int32_t value) {
	return (ag_fixedpoint_uq10_6_t)(value << 6);
}

/**
 * @brief Convert UQ10.6 fixed-point to integer (truncate)
 *
 * @param[in] value UQ10.6 fixed-point value
 * @return Integer part (fractional part discarded)
 */
static inline int32_t ag_fixedpoint_uq10_6_to_int(ag_fixedpoint_uq10_6_t value) { return (int32_t)(value >> 6); }

/** @} */ /* end of AG_FIXEDPOINT_UQ10_6 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ6.10                                     */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ6_10 UQ6.10 Conversion Functions
 * @brief Conversion functions for UQ6.10 format
 * @{
 */

/**
 * @brief Convert integer to UQ6.10 fixed-point
 *
 * @param[in] value Integer value to convert (0-63)
 * @return UQ6.10 fixed-point representation
 * @warning Values outside [0, 63] will overflow
 */
static inline ag_fixedpoint_uq6_10_t ag_fixedpoint_uq6_10_from_int(int32_t value) {
	return (ag_fixedpoint_uq6_10_t)(value << 10);
}

/**
 * @brief Convert UQ6.10 fixed-point to integer (truncate)
 *
 * @param[in] value UQ6.10 fixed-point value
 * @return Integer part (fractional part discarded)
 */
static inline int32_t ag_fixedpoint_uq6_10_to_int(ag_fixedpoint_uq6_10_t value) { return (int32_t)(value >> 10); }

/** @} */ /* end of AG_FIXEDPOINT_UQ6_10 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ4.12                                     */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ4_12 UQ4.12 Conversion Functions
 * @brief Conversion functions for UQ4.12 format
 * @{
 */

/**
 * @brief Convert integer to UQ4.12 fixed-point
 *
 * @param[in] value Integer value to convert (0-15)
 * @return UQ4.12 fixed-point representation
 * @warning Values outside [0, 15] will overflow
 */
static inline ag_fixedpoint_uq4_12_t ag_fixedpoint_uq4_12_from_int(int32_t value) {
	return (ag_fixedpoint_uq4_12_t)(value << 12);
}

/**
 * @brief Convert UQ4.12 fixed-point to integer (truncate)
 *
 * @param[in] value UQ4.12 fixed-point value
 * @return Integer part (fractional part discarded)
 */
static inline int32_t ag_fixedpoint_uq4_12_to_int(ag_fixedpoint_uq4_12_t value) { return (int32_t)(value >> 12); }

/** @} */ /* end of AG_FIXEDPOINT_UQ4_12 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - Q4.11                                      */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_Q4_11 Q4.11 Conversion Functions
 * @brief Conversion functions for Q4.11 format
 * @{
 */

/**
 * @brief Convert integer to Q4.11 fixed-point
 *
 * @param[in] value Integer value to convert (-16 to 15)
 * @return Q4.11 fixed-point representation
 * @warning Values outside [-16, 15] will overflow
 */
static inline ag_fixedpoint_q4_11_t ag_fixedpoint_q4_11_from_int(int32_t value) {
	return (ag_fixedpoint_q4_11_t)(value << 11);
}

/**
 * @brief Convert Q4.11 fixed-point to integer (truncate)
 *
 * @param[in] value Q4.11 fixed-point value
 * @return Integer part (fractional part discarded)
 */
static inline int32_t ag_fixedpoint_q4_11_to_int(ag_fixedpoint_q4_11_t value) { return (int32_t)(value >> 11); }

/** @} */ /* end of AG_FIXEDPOINT_Q4_11 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - Q3.12                                      */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_Q3_12 Q3.12 Conversion Functions
 * @brief Conversion functions for Q3.12 format
 * @{
 */

/**
 * @brief Convert integer to Q3.12 fixed-point
 *
 * @param[in] value Integer value to convert (-8 to 7)
 * @return Q3.12 fixed-point representation
 * @warning Values outside [-8, 7] will overflow
 */
static inline ag_fixedpoint_q3_12_t ag_fixedpoint_q3_12_from_int(int32_t value) {
	return (ag_fixedpoint_q3_12_t)(value << 12);
}

/**
 * @brief Convert Q3.12 fixed-point to integer (truncate)
 *
 * @param[in] value Q3.12 fixed-point value
 * @return Integer part (fractional part discarded)
 */
static inline int32_t ag_fixedpoint_q3_12_to_int(ag_fixedpoint_q3_12_t value) { return (int32_t)(value >> 12); }

/** @} */ /* end of AG_FIXEDPOINT_Q3_12 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - Q3.28                                      */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_Q3_28 Q3.28 Conversion Functions
 * @brief Conversion functions for Q3.28 format
 * @{
 */

/**
 * @brief Convert integer to Q3.28 fixed-point
 *
 * @param[in] value Integer value to convert (-8 to 7)
 * @return Q3.28 fixed-point representation
 * @warning Values outside [-8, 7] will overflow
 */
static inline ag_fixedpoint_q3_28_t ag_fixedpoint_q3_28_from_int(int32_t value) {
	return (ag_fixedpoint_q3_28_t)(value << 28);
}

/**
 * @brief Convert Q3.28 fixed-point to integer (truncate)
 *
 * @param[in] value Q3.28 fixed-point value
 * @return Integer part (fractional part discarded)
 */
static inline int32_t ag_fixedpoint_q3_28_to_int(ag_fixedpoint_q3_28_t value) { return (int32_t)(value >> 28); }

/** @} */ /* end of AG_FIXEDPOINT_Q3_28 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ16.0                                     */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ16_0 UQ16.0 Conversion Functions
 * @brief Conversion functions for UQ16.0 format (standard unsigned integer)
 * @{
 */

/**
 * @brief Convert integer to UQ16.0 fixed-point
 *
 * @param[in] value Integer value to convert (0-65535)
 * @return UQ16.0 fixed-point representation (identical to input)
 * @warning Values outside [0, 65535] will overflow
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline ag_fixedpoint_uq16_0_t ag_fixedpoint_uq16_0_from_int(int32_t value) {
	return (ag_fixedpoint_uq16_0_t)value;
}

/**
 * @brief Convert UQ16.0 fixed-point to integer
 *
 * @param[in] value UQ16.0 fixed-point value
 * @return Integer value (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline int32_t ag_fixedpoint_uq16_0_to_int(ag_fixedpoint_uq16_0_t value) { return (int32_t)value; }

/** @} */ /* end of AG_FIXEDPOINT_UQ16_0 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - Q15.0                                      */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_Q15_0 Q15.0 Conversion Functions
 * @brief Conversion functions for Q15.0 format (standard signed integer)
 * @{
 */

/**
 * @brief Convert integer to Q15.0 fixed-point
 *
 * @param[in] value Integer value to convert (-32768 to 32767)
 * @return Q15.0 fixed-point representation (identical to input)
 * @warning Values outside [-32768, 32767] will overflow
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline ag_fixedpoint_q15_0_t ag_fixedpoint_q15_0_from_int(int32_t value) { return (ag_fixedpoint_q15_0_t)value; }

/**
 * @brief Convert Q15.0 fixed-point to integer
 *
 * @param[in] value Q15.0 fixed-point value
 * @return Integer value (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline int32_t ag_fixedpoint_q15_0_to_int(ag_fixedpoint_q15_0_t value) { return (int32_t)value; }

/** @} */ /* end of AG_FIXEDPOINT_Q15_0 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ8.0                                      */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ8_0 UQ8.0 Conversion Functions
 * @brief Conversion functions for UQ8.0 format (standard unsigned byte)
 * @{
 */

/**
 * @brief Convert integer to UQ8.0 fixed-point
 *
 * @param[in] value Integer value to convert (0-255)
 * @return UQ8.0 fixed-point representation (identical to input)
 * @warning Values outside [0, 255] will overflow
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline ag_fixedpoint_uq8_0_t ag_fixedpoint_uq8_0_from_int(int32_t value) { return (ag_fixedpoint_uq8_0_t)value; }

/**
 * @brief Convert UQ8.0 fixed-point to integer
 *
 * @param[in] value UQ8.0 fixed-point value
 * @return Integer value (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline int32_t ag_fixedpoint_uq8_0_to_int(ag_fixedpoint_uq8_0_t value) { return (int32_t)value; }

/** @} */ /* end of AG_FIXEDPOINT_UQ8_0 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - Q7.0                                       */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_Q7_0 Q7.0 Conversion Functions
 * @brief Conversion functions for Q7.0 format (standard signed byte)
 * @{
 */

/**
 * @brief Convert integer to Q7.0 fixed-point
 *
 * @param[in] value Integer value to convert (-128 to 127)
 * @return Q7.0 fixed-point representation (identical to input)
 * @warning Values outside [-128, 127] will overflow
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline ag_fixedpoint_q7_0_t ag_fixedpoint_q7_0_from_int(int32_t value) { return (ag_fixedpoint_q7_0_t)value; }

/**
 * @brief Convert Q7.0 fixed-point to integer
 *
 * @param[in] value Q7.0 fixed-point value
 * @return Integer value (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline int32_t ag_fixedpoint_q7_0_to_int(ag_fixedpoint_q7_0_t value) { return (int32_t)value; }

/** @} */ /* end of AG_FIXEDPOINT_Q7_0 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - UQ32.0                                     */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_UQ32_0 UQ32.0 Conversion Functions
 * @brief Conversion functions for UQ32.0 format (standard unsigned 32-bit integer)
 * @{
 */

/**
 * @brief Convert integer to UQ32.0 fixed-point
 *
 * @param[in] value Integer value to convert (0-4294967295)
 * @return UQ32.0 fixed-point representation (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 * @note For values larger than int32_t range, use direct assignment or cast
 */
static inline ag_fixedpoint_uq32_0_t ag_fixedpoint_uq32_0_from_int(int32_t value) {
	return (ag_fixedpoint_uq32_0_t)value;
}

/**
 * @brief Convert UQ32.0 fixed-point to integer
 *
 * @param[in] value UQ32.0 fixed-point value
 * @return Integer value (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 * @warning For values > INT32_MAX, will overflow when cast to int32_t
 */
static inline int32_t ag_fixedpoint_uq32_0_to_int(ag_fixedpoint_uq32_0_t value) { return (int32_t)value; }

/** @} */ /* end of AG_FIXEDPOINT_UQ32_0 */

/*============================================================================*/
/* INTEGER CONVERSION FUNCTIONS - Q31.0                                      */
/*============================================================================*/

/**
 * @defgroup AG_FIXEDPOINT_Q31_0 Q31.0 Conversion Functions
 * @brief Conversion functions for Q31.0 format (standard signed 32-bit integer)
 * @{
 */

/**
 * @brief Convert integer to Q31.0 fixed-point
 *
 * @param[in] value Integer value to convert (-2147483648 to 2147483647)
 * @return Q31.0 fixed-point representation (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline ag_fixedpoint_q31_0_t ag_fixedpoint_q31_0_from_int(int32_t value) { return (ag_fixedpoint_q31_0_t)value; }

/**
 * @brief Convert Q31.0 fixed-point to integer
 *
 * @param[in] value Q31.0 fixed-point value
 * @return Integer value (identical to input)
 *
 * @note This is an identity operation (no shift) but provided for API consistency
 */
static inline int32_t ag_fixedpoint_q31_0_to_int(ag_fixedpoint_q31_0_t value) { return (int32_t)value; }

/** @} */ /* end of AG_FIXEDPOINT_Q31_0 */

/*============================================================================*/
/* FLOATING-POINT CONVERSION FUNCTIONS (OPTIONAL)                            */
/*============================================================================*/

#if AG_FIXEDPOINT_ENABLE_FLOAT

/**
 * @defgroup AG_FIXEDPOINT_FLOAT_CONV Floating-Point Conversions
 * @brief Floating-point conversion functions (optional, slow without FPU)
 *
 * These functions convert between fixed-point and floating-point formats
 * using the configured precision (ag_fixedpoint_float_t), which can be
 * either 32-bit or 64-bit IEEE-754 format depending on the
 * AG_FIXEDPOINT_FLOAT_TYPE configuration.
 *
 * @warning These functions use floating-point arithmetic and are VERY SLOW
 *          on microcontrollers without an FPU. Use only for debugging,
 *          testing, or when interfacing with floating-point code.
 *          For production embedded code, use integer operations instead.
 *
 * @note These functions are only available when AG_FIXEDPOINT_ENABLE_FLOAT
 *       is set to 1 in ag_fixedpoint_cfg.h
 *
 * @note The actual floating-point type used (ag_fixedpoint_float_t) is:
 *       - float32_t (IEEE-754 binary32) if AG_FIXEDPOINT_FLOAT_TYPE == 32
 *       - float64_t (IEEE-754 binary64) if AG_FIXEDPOINT_FLOAT_TYPE == 64
 *
 * @warning When using AG_FIXEDPOINT_FLOAT_TYPE=32 (float32_t) with Q3.28,
 *          precision loss will occur since float32 has only 23 mantissa bits
 *          but Q3.28 has 28 fractional bits. Use 64-bit for Q3.28 if full
 *          precision is required.
 *
 * @{
 */

/* UQ12.4 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq12_4_to_float(ag_fixedpoint_uq12_4_t value);
ag_fixedpoint_uq12_4_t ag_fixedpoint_uq12_4_from_float(ag_fixedpoint_float_t value);

/* UQ10.6 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq10_6_to_float(ag_fixedpoint_uq10_6_t value);
ag_fixedpoint_uq10_6_t ag_fixedpoint_uq10_6_from_float(ag_fixedpoint_float_t value);

/* UQ6.10 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq6_10_to_float(ag_fixedpoint_uq6_10_t value);
ag_fixedpoint_uq6_10_t ag_fixedpoint_uq6_10_from_float(ag_fixedpoint_float_t value);

/* UQ4.12 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq4_12_to_float(ag_fixedpoint_uq4_12_t value);
ag_fixedpoint_uq4_12_t ag_fixedpoint_uq4_12_from_float(ag_fixedpoint_float_t value);

/* Q4.11 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_q4_11_to_float(ag_fixedpoint_q4_11_t value);
ag_fixedpoint_q4_11_t ag_fixedpoint_q4_11_from_float(ag_fixedpoint_float_t value);

/* Q3.12 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_q3_12_to_float(ag_fixedpoint_q3_12_t value);
ag_fixedpoint_q3_12_t ag_fixedpoint_q3_12_from_float(ag_fixedpoint_float_t value);

/* Q3.28 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_q3_28_to_float(ag_fixedpoint_q3_28_t value);
ag_fixedpoint_q3_28_t ag_fixedpoint_q3_28_from_float(ag_fixedpoint_float_t value);

/* UQ16.0 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq16_0_to_float(ag_fixedpoint_uq16_0_t value);
ag_fixedpoint_uq16_0_t ag_fixedpoint_uq16_0_from_float(ag_fixedpoint_float_t value);

/* Q15.0 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_q15_0_to_float(ag_fixedpoint_q15_0_t value);
ag_fixedpoint_q15_0_t ag_fixedpoint_q15_0_from_float(ag_fixedpoint_float_t value);

/* UQ8.0 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq8_0_to_float(ag_fixedpoint_uq8_0_t value);
ag_fixedpoint_uq8_0_t ag_fixedpoint_uq8_0_from_float(ag_fixedpoint_float_t value);

/* Q7.0 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_q7_0_to_float(ag_fixedpoint_q7_0_t value);
ag_fixedpoint_q7_0_t ag_fixedpoint_q7_0_from_float(ag_fixedpoint_float_t value);

/* UQ32.0 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_uq32_0_to_float(ag_fixedpoint_uq32_0_t value);
ag_fixedpoint_uq32_0_t ag_fixedpoint_uq32_0_from_float(ag_fixedpoint_float_t value);

/* Q31.0 <-> float */
ag_fixedpoint_float_t ag_fixedpoint_q31_0_to_float(ag_fixedpoint_q31_0_t value);
ag_fixedpoint_q31_0_t ag_fixedpoint_q31_0_from_float(ag_fixedpoint_float_t value);

/** @} */ /* end of AG_FIXEDPOINT_FLOAT_CONV */

#endif /* AG_FIXEDPOINT_ENABLE_FLOAT */

/*============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* AG_FIXEDPOINT_H */