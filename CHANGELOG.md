# Changelog

All notable changes to the AG Fixed-Point Arithmetic Library will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned
- Arithmetic operations (add, subtract, multiply, divide)
- Saturating arithmetic support
- Multiple rounding modes
- Additional Q formats (Q15.16, Q31.32)
- SIMD optimizations

## [1.0.0] - 2026-03-11

### Added
- Initial stable release
- 13 fixed-point types:
  - 8-bit: UQ8.0, Q7.0
  - 16-bit: UQ16.0, Q15.0, UQ12.4, UQ10.6, UQ6.10, UQ4.12, Q4.11, Q3.12
  - 32-bit: UQ32.0, Q31.0, Q3.28
- Integer conversion functions (to/from int32_t)
- Compile-time constant macros
- Optional floating-point conversion functions
- IEEE-754 automatic type detection
- Comprehensive unit test suite (22 tests)
- Example programs for all Q*.0 types
- CMake build system with MinGW support
- Code coverage reports
- Professional documentation (Doxygen-ready)
- **Configuration template system** (`config/` for template, `cfg/` for actual config)
- **CMake standalone detection** (auto-enable tests when not submodule)
- **Release automation scripts** for Windows (PowerShell) and Linux (Bash)
- **GitHub publish automation** (automated tag creation and release publishing)
- Implementation notes documentation (IMPLEMENTATION-NOTES.md)
- MIT License
- Professional CHANGELOG (Keep a Changelog format)

### Features
- Zero runtime overhead for constant conversions
- Pure integer arithmetic (no FPU required)
- Inline functions for maximum performance
- Configurable floating-point precision (32/64-bit)
- Header-only integer conversions
- C11 standard compliance
- **Smart config file search** (cfg/ → config/ fallback)
- **Automated release workflow** with GitHub CLI integration

### Documentation
- Complete README with quick start guide
- API reference documentation
- Build and installation instructions
- Example usage code
- Configuration guide (template pattern)
- Code coverage metrics
- Implementation notes and architecture docs
- Release automation guide

---

## Version History

### Pre-1.0 Development

**2026-01-19** - Q*.0 Types Extension
- Added Q31.0 and UQ32.0 (32-bit integer types)
- Unit tests for all Q*.0 types
- Example program for 32-bit types

**2026-01-18** - Extended Type Support
- Added Q15.0 and UQ16.0 (16-bit integer types)
- Added Q7.0 and UQ8.0 (8-bit integer types)
- Float conversion support for all types
- Example programs for 8-bit and 16-bit types

**2026-01-18** - Initial Development
- Base library structure
- First 7 types (UQ12.4, UQ10.6, UQ6.10, UQ4.12, Q4.11, Q3.12, Q3.28)
- CMake build system
- CMocka integration
- Initial unit tests (16 tests)
- README documentation
- Build scripts (Windows PowerShell)

---

## Links

- [Repository](https://github.com/AlexGiov/ag-fixedpoint)
- [Issue Tracker](https://github.com/AlexGiov/ag-fixedpoint/issues)
- [Documentation](https://github.com/AlexGiov/ag-fixedpoint#readme)
- [Releases](https://github.com/AlexGiov/ag-fixedpoint/releases)

---

## Notes

### Versioning Scheme

We use [Semantic Versioning](https://semver.org/):
- **MAJOR** version when you make incompatible API changes
- **MINOR** version when you add functionality in a backward compatible manner
- **PATCH** version when you make backward compatible bug fixes

### Categories

Changes are grouped into:
- **Added**: New features
- **Changed**: Changes in existing functionality
- **Deprecated**: Soon-to-be removed features
- **Removed**: Removed features
- **Fixed**: Bug fixes
- **Security**: Vulnerability fixes
