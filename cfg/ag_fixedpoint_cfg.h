/**
 * @file ag_fixedpoint_cfg.h
 * @brief Development configuration for AG Fixed-Point Arithmetic Library
 *
 * This is the configuration file used during development and testing.
 *
 * For production use, copy config/ag_fixedpoint_cfg.h.template to your
 * application's cfg/ directory and customize the settings.
 *
 * @author Alex Giovannini
 * @date 2026-03-11
 * @version 1.0.0
 */

#ifndef AG_FIXEDPOINT_CFG_H
#define AG_FIXEDPOINT_CFG_H

/*============================================================================*/
/* DEVELOPMENT CONFIGURATION                                                 */
/*============================================================================*/

/**
 * @brief Enable floating-point conversions for testing
 *
 * Enabled in development to allow comprehensive testing of all features.
 * For production embedded systems, consider setting to 0.
 */
#ifndef AG_FIXEDPOINT_ENABLE_FLOAT
#define AG_FIXEDPOINT_ENABLE_FLOAT 1
#endif

/**
 * @brief Floating-point type: 32-bit for testing
 */
#ifndef AG_FIXEDPOINT_FLOAT_TYPE
#define AG_FIXEDPOINT_FLOAT_TYPE 32
#endif

/**
 * @brief Saturation arithmetic (future feature - not yet implemented)
 */
#ifndef AG_FIXEDPOINT_ENABLE_SATURATION
#define AG_FIXEDPOINT_ENABLE_SATURATION 0
#endif

/**
 * @brief Rounding mode (future feature - not yet implemented)
 */
#ifndef AG_FIXEDPOINT_ROUNDING_MODE
#define AG_FIXEDPOINT_ROUNDING_MODE 0
#endif

#endif /* AG_FIXEDPOINT_CFG_H */
