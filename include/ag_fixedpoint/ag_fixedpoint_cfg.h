/**
 * @file ag_fixedpoint_cfg.h
 * @brief Configuration options for the AG Fixed-Point Arithmetic Library
 *
 * This file contains compile-time configuration options that control
 * the features and behavior of the fixed-point library.
 *
 * @author Alex Giovannini
 * @date 2026-01-18
 * @version 1.0.0
 */

#ifndef AG_FIXEDPOINT_CFG_H
#define AG_FIXEDPOINT_CFG_H

/**
 * @defgroup AG_FIXEDPOINT_CONFIG Configuration Options
 * @brief Compile-time configuration macros
 * @{
 */

/**
 * @def AG_FIXEDPOINT_ENABLE_FLOAT
 * @brief Enable floating-point conversion functions
 *
 * When set to 1, enables conversion functions between fixed-point
 * and floating-point (float/double) types.
 *
 * @warning Enabling this feature significantly increases code size and
 *          drastically reduces performance on microcontrollers without
 *          a Floating-Point Unit (FPU). Use only for debugging or when
 *          interfacing with floating-point code.
 *
 * @note Default: 0 (disabled) for embedded systems optimization
 */
#ifndef AG_FIXEDPOINT_ENABLE_FLOAT
#define AG_FIXEDPOINT_ENABLE_FLOAT 1
#endif

/**
 * @def AG_FIXEDPOINT_ENABLE_SATURATION
 * @brief Enable saturation arithmetic variants
 *
 * When set to 1, enables saturating versions of arithmetic operations
 * that clamp results to the valid range instead of wrapping on overflow.
 *
 * @note Default: 0 (disabled) - standard wrapping behavior
 * @note Saturation adds overhead - enable only if needed
 */
#ifndef AG_FIXEDPOINT_ENABLE_SATURATION
#define AG_FIXEDPOINT_ENABLE_SATURATION 0
#endif

/**
 * @def AG_FIXEDPOINT_ROUNDING_MODE
 * @brief Default rounding mode for fixed-point operations
 *
 * Possible values:
 * - 0: Truncate (fastest, default)
 * - 1: Round half-up (add 0.5 before truncate)
 * - 2: Round half-even (banker's rounding)
 *
 * @note Default: 0 (truncate) for maximum performance
 */
#ifndef AG_FIXEDPOINT_ROUNDING_MODE
#define AG_FIXEDPOINT_ROUNDING_MODE 0
#endif

/**
 * @def AG_FIXEDPOINT_FLOAT_TYPE
 * @brief Select floating-point precision for conversions
 *
 * This option controls the precision of floating-point conversion functions
 * when AG_FIXEDPOINT_ENABLE_FLOAT is enabled.
 *
 * Possible values:
 * - 32: Use 32-bit IEEE-754 binary32 (float32_t)
 *       - Sufficient for all 16-bit fixed-point formats
 *       - Faster on 32-bit FPUs
 *       - Uses less memory
 *
 * - 64: Use 64-bit IEEE-754 binary64 (float64_t)
 *       - Required for full precision with 32-bit fixed-point (Q3.28)
 *       - Higher precision in intermediate calculations
 *       - Required on systems where float is not 32-bit
 *
 * @note The library will automatically detect which native C type
 *       (float, double, long double) corresponds to the requested
 *       IEEE-754 format at compile-time.
 *
 * @note If the requested format is not available on the target platform,
 *       a compile-time error will be generated.
 *
 * @note Default: 32 (binary32) for embedded systems optimization
 *
 * @warning Choosing 32-bit for Q3.28 conversions will result in precision
 *          loss, as float32 has only 23 mantissa bits but Q3.28 has 28
 *          fractional bits. Use 64-bit for Q3.28 if precision is critical.
 */
#ifndef AG_FIXEDPOINT_FLOAT_TYPE
#define AG_FIXEDPOINT_FLOAT_TYPE 32
#endif

/** @} */ /* end of AG_FIXEDPOINT_CONFIG */

#endif /* AG_FIXEDPOINT_CFG_H */
