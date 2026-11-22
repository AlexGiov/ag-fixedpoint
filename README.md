# Unit Test Template for C Libraries

Professional template for unit testing C libraries with CMocka framework, designed for embedded systems development with cross-compiler support.

## 📋 Features

- ✅ **CMocka Integration** - Professional C unit testing framework
- ✅ **Ninja Build System** - Fast, reliable, cross-platform builds (required)
- ✅ **CMake Build System** - Cross-platform, cross-compiler support
- ✅ **Code Coverage** - Integrated gcov support with HTML reports
- ✅ **VS Code Integration** - Debug configuration and build tasks
- ✅ **Embedded-Friendly** - Install sources for embedded integration
- ✅ **Modular Structure** - Professional directory organization
- ✅ **Cross-Platform Scripts** - CMake automation for Windows/Linux/macOS
- ✅ **Version Management** - Automated semantic versioning and Git tagging
- ✅ **Minimal Dependencies** - Only CMake + Ninja required

## 🚀 Quick Start

### Prerequisites

- CMake 3.24+ (required for conditional presets in `CMakePresets.json`)
- GCC or compatible C compiler
  - **Windows**: [MSYS2](https://www.msys2.org/) UCRT64 environment (recommended - MSYS2's own MINGW64 environment is deprecated as of 2026). Install MSYS2, then `pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-gdb` and make sure `C:\msys64\ucrt64\bin` is on your `PATH`.
  - **Linux/macOS**: system GCC (e.g. `sudo apt install gcc`)
- gcov (for coverage analysis)
- **Ninja build system** - **REQUIRED** for optimal performance and cross-platform consistency
- Git (for version management)
- (Optional) GitHub CLI `gh` for automated releases

**Installing Ninja:**
```bash
# Windows (Chocolatey)
choco install ninja

# Windows (Scoop)
scoop install ninja

# Linux (Ubuntu/Debian)
sudo apt install ninja-build

# macOS (Homebrew)
brew install ninja

# Or download standalone binary (~200KB):
# https://github.com/ninja-build/ninja/releases
```

**Why Ninja is required:**
- ✅ **2-3x faster builds** than Make
- ✅ **No Cygwin conflicts** on Windows
- ✅ **Consistent behavior** across all platforms
- ✅ **Industry standard** (used by LLVM, Chromium, Android NDK)
- ✅ **Tiny footprint** (~200KB standalone executable)

### Build and Test

**Using CMakePresets.json (recommended, IDE-friendly):**

```bash
# Windows (MSYS2 UCRT64)
cmake --preset ucrt64
cmake --build --preset ucrt64

# Linux
cmake --preset linux-gcc
cmake --build --preset linux-gcc
```

The preset to use depends on the platform, not on personal choice: `ucrt64` is only valid on Windows and `linux-gcc` only on Linux (enforced via the `condition` field in `CMakePresets.json`), so the commands above are not interchangeable between OSes.
To override the compiler location, create a local (untracked) `CMakeUserPresets.json` that inherits from one of these presets.

**Cross-Platform wrapper (works identically on Windows, Linux, macOS):**

```bash
# Build project (auto-selects the right preset for your OS)
cmake -P build.cmake

# Build with tests
cmake -DRUN_TESTS=ON -P build.cmake

# Clean build
cmake -DCLEAN=ON -P build.cmake

# Clean only (without rebuilding)
cmake -DCLEAN_ONLY=ON -P build.cmake

# Build with coverage
cmake -DCOVERAGE=ON -DRUN_TESTS=ON -P build.cmake

# Generate coverage report
cmake -P coverage.cmake
```

**Platform-Specific Configuration:**

Generator and compiler are defined declaratively in `CMakePresets.json`:

- **Build System**: Ninja (required), set by the preset
- **Compiler**: `ucrt64` preset -> MSYS2 UCRT64 GCC; `linux-gcc` preset -> system GCC
- **Cross-Platform**: `build.cmake` auto-selects the right preset for your OS

**Custom Configuration (Advanced):**

To point to a different compiler location, create a local `CMakeUserPresets.json` (not tracked by Git) that inherits from `ucrt64`/`linux-gcc`, or override via environment variable (still supported as an escape hatch by `build.cmake`):
```powershell
# Windows
$env:CMAKE_C_COMPILER = "C:\custom\path\gcc.exe"
$env:GCOV_EXECUTABLE = "C:\custom\path\gcov.exe"

# Linux/macOS
export CMAKE_C_COMPILER=/usr/bin/clang
export GCOV_EXECUTABLE=/usr/bin/gcov
```

### VS Code

- **Build**: `Ctrl+Shift+B`
- **Debug Tests**: `F5`
- **Run Tests**: Select "test" task

## 📁 Directory Structure

The template is organized into two main parts: the **`ag_fixedpoint/` library** (self-contained, deliverable) and the **development infrastructure** (testing & tooling) around it.

### Core Library: `ag_fixedpoint/` (Installable, exportable)

Everything the library needs lives under a single root, `ag_fixedpoint/`, on purpose: this is the folder that gets exported to other repositories once the library
is stable (see [Exporting `ag_fixedpoint/` with git subtree](#-exporting-ag_fixedpoint-with-git-subtree) below). It has its own `CMakeLists.txt` and configures/builds standalone.

```
unit_test_template/
└── ag_fixedpoint/
    ├── CMakeLists.txt          # Self-contained: project(), target, install rules
    ├── include/                # Public API headers
    │   └── ag_fixedpoint.h             # Public interface
    ├── src/                    # Library implementation
    │   ├── ag_fixedpoint.c             # Implementation files
    │   └── private/            # Private implementation (not installed)
    └── config/                 # Configuration templates
        └── ag_fixedpoint_config.h.template  # Configuration example
```

**Why this structure?**
- `ag_fixedpoint/` - Single root folder so the library can be split into its own branch/repo as-is
- `ag_fixedpoint/include/` - Public headers directly under `ag_fixedpoint/`, no extra namespace subfolder
- `ag_fixedpoint/src/` - Contains implementation; installable for embedded integration
- `ag_fixedpoint/src/private/` - Internal implementation details (excluded from install)
- `ag_fixedpoint/config/` - Template for application-specific configuration

### Development Infrastructure (Non-installable, stays outside `ag_fixedpoint/`)

Tools and tests for library development:

```
unit_test_template/
├── cfg/                    # Configuration for this build
│   └── ag_fixedpoint_config.h      # Actual config used during development/testing
├── test/                   # Testing framework
│   ├── unit/               # Unit test files
│   │   └── test_ag_fixedpoint.c
│   ├── mocks/              # Mock implementations
│   ├── fixtures/           # Test fixtures
│   └── data/               # Test data files
├── external/               # External dependencies
│   └── cmocka/             # CMocka test framework (embedded)
│       ├── include/
│       └── src/
├── cmake/                  # Build system configuration
│   └── README.md
├── build/                  # Build artifacts (gitignored)
├── bin/                    # Test executables (gitignored)
├── lib/                    # Compiled libraries (gitignored)
├── coverage/               # Coverage reports (gitignored)
├── .vscode/                # VS Code integration
├── CMakePresets.json       # Toolchain presets (ucrt64 / linux-gcc)
├── build.cmake             # Cross-platform build automation
├── coverage.cmake          # Cross-platform coverage report generator
├── release.cmake           # Cross-platform version management
├── rename-library.cmake    # Library renaming utility (cross-platform)
└── CMakeLists.txt          # Thin orchestrator (add_subdirectory(ag_fixedpoint), tests)
```

**Development vs Production:**

- Test infrastructure stays in this repository, outside `ag_fixedpoint/`
- `cfg/` contains the actual config used for testing (NOT installed, NOT part of `ag_fixedpoint/`)
- `ag_fixedpoint/config/` contains templates for applications to copy and customize
- Only `ag_fixedpoint/include/`, `ag_fixedpoint/src/`, and `ag_fixedpoint/config/` templates are installed
- Application gets clean library without test dependencies

## 🌿 Lib-export branch

This template has a branch used to share the library with consumer application. This branch was create with the following commands. Please see [GIT_WORKFLOW.md](GIT_WORKFLOW.md) doc to further information how to use the library in the application and how to syncronize work with the library repository.

```bash
# In this repository: create a branch containing only ag_fixedpoint/ (rewritten history)
git subtree split --prefix=ag_fixedpoint -b lib-export
git push origin lib-export

```

Because `ag_fixedpoint/CMakeLists.txt` is self-sufficient, the consuming app can either `add_subdirectory(external/ag_fixedpoint)` or configure/build it standalone. Point `MYLIB_CONFIG_DIR` at the app's own `cfg/` **before** `add_subdirectory(ag_fixedpoint)` to supply the real `ag_fixedpoint_config.h` (see `ag_fixedpoint/CMakeLists.txt` comments).

This mirrors what `git submodule add <repo-url> external/ag_fixedpoint` would look like once the library moves to its own repository - subtree is the lower-friction stepping stone: consumers don't need to learn submodule workflows, and history is squashed into a single commit per pull.

If `lib-export` ever receives hotfixes directly (bypassing `main`), see [GIT_WORKFLOW.md](GIT_WORKFLOW.md) for the bidirectional sync workflow.

## 🛠️ Renaming the Template Library

### Simple Automatic Rename ✨ (Recommended)

Thanks to CMake's `${PROJECT_NAME}` pattern, renaming is now **much simpler**:

```bash
# 1. Clone this template
git clone <repo-url> my-new-library
cd my-new-library

# 2. Run rename script (cross-platform)
cmake -DNEW_NAME=sensor_driver -P rename-library.cmake

# 3. Build and test (cross-platform)
cmake -DCLEAN=ON -DRUN_TESTS=ON -P build.cmake

# 4. Implement your library code
#    - Edit ag_fixedpoint/src/sensor_driver.c
#    - Edit ag_fixedpoint/include/sensor_driver.h
#    - Write tests in test/unit/test_sensor_driver.c
```

**What the script does (simplified):**
1. ✅ Changes `project(ag_fixedpoint)` → `project(sensor_driver)` in `CMakeLists.txt` and `ag_fixedpoint/CMakeLists.txt`
2. ✅ Renames `ag_fixedpoint/include/ag_fixedpoint.h` → `ag_fixedpoint/include/sensor_driver.h`
3. ✅ Renames source files: `ag_fixedpoint.*` → `sensor_driver.*`
4. ✅ Updates `#include` statements in C files
5. ✅ Updates README.md references

**Cross-platform:** Works identically on Windows, Linux, and macOS!

**What happens automatically (via `${PROJECT_NAME}`):**
- ✨ All CMake library targets
- ✨ All install paths and exports
- ✨ Package configuration files
- ✨ Test linkage

**Key Insight:** Because CMakeLists.txt now uses `${PROJECT_NAME}` everywhere, you only need to change the `project()` declaration and the rest updates automatically!

### Manual Rename (If You Prefer)

<details>
<summary>Click to expand manual steps</summary>

If you prefer to do it manually:

### 1. Update CMakeLists.txt (both files)

```cmake
# Change this line in CMakeLists.txt AND ag_fixedpoint/CMakeLists.txt:
project(ag_fixedpoint VERSION 1.0.0 LANGUAGES C)  
# to:
project(your_library_name VERSION 1.0.0 LANGUAGES C)

# Everything else updates automatically via ${PROJECT_NAME}!
```

### 2. Rename Directories and Files

```powershell
# Rename the header (directly under ag_fixedpoint/include/, no namespace subfolder)
mv ag_fixedpoint/include/ag_fixedpoint.h ag_fixedpoint/include/your_library_name.h

# Rename source files (if single-module library)
mv ag_fixedpoint/src/ag_fixedpoint.c ag_fixedpoint/src/your_library_name.c
mv test/unit/test_ag_fixedpoint.c test/unit/test_your_library_name.c
```

### 3. Update #include Statements

```c
// In ag_fixedpoint/src/your_library_name.c
#include "your_library_name.h"

// In test/unit/test_your_library_name.c
#include "your_library_name.h"
```

### 4. Update README.md

Replace all references to `ag_fixedpoint` with `your_library_name`.

</details>

## 💡 Template Design Philosophy

This template follows **CMake best practices** for relocatable packages:

- **Single Source of Truth**: Library name defined once in `project()` declaration
- **Variable-Based Configuration**: All targets/paths use `${PROJECT_NAME}` variable
- **Auto-Derived Values**: `${PROJECT_NAME_UPPER}` generated automatically
- **Minimal Renaming**: Only need to change project name + rename directories/files
- **Professional Structure**: Follows cmake-packages(7) documentation patterns

### File Naming Convention

For **single-module libraries** (most common):
- Use: `ag_fixedpoint/src/${PROJECT_NAME}.c` and `ag_fixedpoint/include/${PROJECT_NAME}.h`
- Example: `sensor_driver` → `ag_fixedpoint/src/sensor_driver.c`, `ag_fixedpoint/include/sensor_driver.h`

For **multi-module libraries**:
- Use descriptive module names
- Example: `uart_driver` → `ag_fixedpoint/src/uart_tx.c`, `ag_fixedpoint/src/uart_rx.c`, `ag_fixedpoint/src/uart_config.c`

### How It Works Under the Hood

The template leverages CMake variables for maximum flexibility:

```cmake
# In ag_fixedpoint/CMakeLists.txt - Single source of truth:
project(ag_fixedpoint VERSION 1.0.0 LANGUAGES C)

# Auto-derived variable:
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)

# All targets use variables:
add_library(${PROJECT_NAME} ...)
target_include_directories(${PROJECT_NAME} ...)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets ...)

# Install paths use variables:
install(TARGETS ${PROJECT_NAME} PUBLIC_HEADER DESTINATION include/${PROJECT_NAME} ...)
install(FILES ... DESTINATION cmake/${PROJECT_NAME})
```

This means changing `project(ag_fixedpoint)` to `project(your_lib)` automatically updates:
- Library target name
- All install paths
- Export configurations  
- Package configs
- Test linkage (via inherited `${LIB_NAME}`)

## 🔧 Usage

### 1. Clone and Customize

```bash
# Clone this template
git clone <repo-url> my-library-tests
cd my-library-tests

# Update library name
# Edit CMakeLists.txt AND ag_fixedpoint/CMakeLists.txt: project(ag_fixedpoint) -> project(your_lib_name)
```

### 2. Add Your Library Sources

```bash
# Add headers
ag_fixedpoint/include/your_module.h

# Add implementation
ag_fixedpoint/src/your_module.c

# Update ag_fixedpoint/CMakeLists.txt MYLIB_SOURCES variable
```

### 3. Write Tests

```bash
# Create test file
test/unit/test_your_module.c

# Update test/CMakeLists.txt to add new test executable
```

### 4. Build and Test

```bash
cmake -P build.cmake -DRUN_TESTS=ON
```

</details>

## 📊 Code Coverage

The template includes integrated code coverage support with HTML report generation:

```bash
# Build with coverage enabled (cross-platform)
cmake -DCLEAN=ON -DCOVERAGE=ON -DRUN_TESTS=ON -P build.cmake

# Generate coverage report (.gcov files only)
cmake -P coverage.cmake

# Generate coverage report with HTML visualization
cmake -DGENERATE_HTML=ON -P coverage.cmake

# View HTML report
# Windows: start coverage\index.html
# Linux: xdg-open coverage/index.html
# macOS: open coverage/index.html
```

Coverage reports show:
- **Line-by-line coverage** with gcov (.gcov files)
  - Format: `execution_count: line_number: source_code`
  - Example:
    ```
           11:   20:int add(int a, int b) { return a + b; }
            4:   22:int subtract(int a, int b) { return a - b; }
            -:   23:  // Lines starting with '-' are non-executable
    ```
- Overall coverage percentage
- Per-file coverage breakdown
- **HTML report** with interactive visualization (with `-DGENERATE_HTML=ON`):
  - Color-coded coverage levels (green ≥80%, yellow ≥50%, red <50%)
  - Visual progress bars
  - Clickable links to detailed .gcov files
  - Automatic browser launch

## 🎯 CMake Options

| Option              | Default | Description                         |
| ------------------- | ------- | ----------------------------------- |
| `BUILD_TESTING`     | `ON`    | Build unit tests                    |
| `ENABLE_COVERAGE`   | `OFF`   | Enable code coverage                |
| `INSTALL_SOURCES`   | `ON`    | Install source files (for embedded) |
| `BUILD_SHARED_LIBS` | `OFF`   | Build shared libraries              |

Example:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
```

## 🔧 Embedded Integration

This template is designed for embedded systems where you often need source files, not just compiled libraries.

### Installation Options

Control what gets installed with CMake options:

| Option            | Default | Description                            |
| ----------------- | ------- | -------------------------------------- |
| `INSTALL_SOURCES` | `ON`    | Install .c files for embedded projects |
| `BUILD_TESTING`   | `ON`    | Enable test building (dev only)        |

### Install Library Files

```bash
# Configure with install prefix
cmake -B build -DCMAKE_INSTALL_PREFIX=install

# Build and install
cmake --build build
cmake --install build

# Installed structure:
install/
├── include/                # Public headers (flat, not namespaced)
│   └── ag_fixedpoint.h
├── src/ag_fixedpoint/              # Source files (if INSTALL_SOURCES=ON)
│   └── ag_fixedpoint.c
├── config/ag_fixedpoint/           # Configuration template
│   └── ag_fixedpoint_config.h.template
└── lib/cmake/ag_fixedpoint/        # CMake package config
    ├── ag_fixedpointTargets.cmake
    └── ag_fixedpointConfig.cmake
```

### Configuration Template Usage

1. **Copy template to your application:**

```bash
cp install/config/ag_fixedpoint/ag_fixedpoint_config.h.template myapp/cfg/ag_fixedpoint_config.h
```

2. **Customize configuration:**

```c
// myapp/cfg/ag_fixedpoint_config.h
#ifndef MYLIB_CONFIG_H
#define MYLIB_CONFIG_H

#define MYLIB_DIV_BY_ZERO_RETURN -1  // Return error code
#define MYLIB_ENABLE_ASSERTIONS      // Enable debug checks

#endif
```

3. **Add config directory to your build:**

```cmake
# In your application CMakeLists.txt
target_include_directories(myapp PRIVATE
    ${CMAKE_SOURCE_DIR}/cfg  # Your config directory
)
```

The library will automatically detect and use `ag_fixedpoint_config.h` if available.

### Use in Embedded Project

**Option 1: Link installed library**

```cmake
find_package(ag_fixedpoint REQUIRED)
target_link_libraries(myapp PRIVATE ag_fixedpoint::ag_fixedpoint)
```

**Option 2: Include sources directly (embedded)**

```cmake
add_library(ag_fixedpoint
    ${VENDOR_DIR}/ag_fixedpoint/src/ag_fixedpoint.c
)

target_include_directories(ag_fixedpoint PUBLIC
    ${VENDOR_DIR}/ag_fixedpoint/include
    ${CMAKE_SOURCE_DIR}/cfg  # Your config
)
```

Both options resolve `#include "ag_fixedpoint.h"` the same way: the installed `PUBLIC_HEADER` is
placed flat in `include/` (not namespaced under `include/ag_fixedpoint/`), matching the layout used
when the library is embedded/subtree'd directly.

### Versioning and Releases

#### Automated Release (Recommended)

Use the included `release.cmake` script for cross-platform version management:

```bash
# Bump patch version (1.0.0 → 1.0.1)
cmake -DBUMP_VERSION=patch -P release.cmake

# Bump minor version (1.0.0 → 1.1.0)
cmake -DBUMP_VERSION=minor -P release.cmake

# Bump major version (1.0.0 → 2.0.0)
cmake -DBUMP_VERSION=major -P release.cmake

# Set specific version
cmake -DVERSION=2.0.0 -P release.cmake

# Create GitHub release (requires gh CLI)
cmake -DBUMP_VERSION=patch -DCREATE_GITHUB_RELEASE=ON -P release.cmake

# Preview changes without executing
cmake -DBUMP_VERSION=minor -DDRY_RUN=ON -P release.cmake
```

**What the script does:**
1. ✅ Reads current version from CMakeLists.txt
2. ✅ Bumps version following semantic versioning
3. ✅ Updates CMakeLists.txt with new version
4. ✅ Commits the version change
5. ✅ Creates annotated Git tag (e.g., v1.0.0)
6. ✅ Pushes tag to remote
7. ✅ Optionally creates GitHub release (with `-DCREATE_GITHUB_RELEASE=ON`)

**Requirements:**
- Git (always required)
- GitHub CLI (`gh`) - only if using `-DCREATE_GITHUB_RELEASE=ON`
  - Install from: https://cli.github.com/

**Cross-Platform:** Works identically on Windows, Linux, and macOS

#### Manual Git Tagging

If you prefer manual tagging:

```bash
# Update version in CMakeLists.txt manually
# Then create and push tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0
```

#### Using Specific Versions in Embedded Projects

```bash
# Clone specific version into vendor directory
git clone --branch v1.0.0 <repo-url> vendor/ag_fixedpoint
```

### Library Structure Best Practices

This template follows professional C library organization, validated against the
[Pitchfork Layout (PFL)](https://github.com/vector-of-bool/pitchfork) conventions for C/C++
projects (see `README_LIBRARY_STRUCTURE.md` for the full rationale and sources):

- **Flat public headers** - `ag_fixedpoint/include/` holds headers directly, no extra namespace subfolder in the source tree; the installed header is also kept flat (`include/ag_fixedpoint.h`), so `#include "ag_fixedpoint.h"` is identical whether the library is embedded/subtree'd or installed via `find_package`
- **Separate public/private** - Only `ag_fixedpoint/include/` is public API
- **Template config** - Library provides template, app provides actual config
- **Optional configuration** - Library works with or without config file
- **Source installation** - Enables embedded system integration

For detailed explanation of library structure patterns, see inline documentation in source files.

## 📝 Example: ag_fixedpoint Library

The template includes a simple `ag_fixedpoint` library as an example:

### Library Code

```c
// ag_fixedpoint/include/ag_fixedpoint.h
int add(int a, int b);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);

// ag_fixedpoint/src/ag_fixedpoint.c
int add(int a, int b) { return a + b; }
int divide(int a, int b) {
    if (b == 0) return 0;  // Safety check
    return a / b;
}
```

### Test Code

```c
// test/unit/test_ag_fixedpoint.c
static void test_add_positive(void **state) {
    assert_int_equal(add(2, 3), 5);
}

static void test_divide_by_zero(void **state) {
    assert_int_equal(divide(10, 0), 0);  // Handles edge case
}
```

### Results

- **8 tests** - All passing
- **100% coverage** - All functions and branches tested
- **Edge cases** - Division by zero handled

## 🐛 Debugging

### Debug Tests in VS Code

1. Set breakpoint in test file or source
2. Press `F5`
3. Select "Debug Test: ag_fixedpoint"
4. Step through code with GDB

### Debug Configuration

The template includes two debug configurations:

- **Debug Test (with build)** - Builds before debugging
- **Debug Test (no build)** - Debugs existing executable

## 📦 CMocka Framework

### Current Status

The template includes a **minimal CMocka stub** for demonstration. For production use:

1. Download full CMocka from https://cmocka.org/
2. Extract to `external/cmocka/`
3. Follow instructions in `external/cmocka/README.md`

### CMocka Features

- Assertions: `assert_int_equal()`, `assert_true()`, etc.
- Mock functions with `will_return()`
- Setup/teardown fixtures
- Test groups and organization

## 🔍 Testing Best Practices

### Test Organization

Only `test/unit/` is scaffolded by this template; the others below are optional and
created on demand as a project grows:

```
test/
├── unit/           # Unit tests (functions, modules)
├── integration/    # Integration tests (multiple modules) - create if needed
├── mocks/          # Mock implementations for dependencies - create if needed
├── fixtures/       # Test setup/teardown helpers - create if needed
└── data/           # Test data files - create if needed
```

### Test Naming

```c
// Format: test_<function>_<scenario>
static void test_add_positive(void **state) { ... }
static void test_add_negative(void **state) { ... }
static void test_divide_by_zero(void **state) { ... }
```

### Coverage Goals

- **Minimum**: 80% line coverage
- **Recommended**: 90%+ line coverage
- **Best**: 100% line coverage + branch coverage

## 🚀 Continuous Integration

### GitHub Actions Example

**Using new cross-platform scripts:**

```yaml
name: Tests

on: [push, pull_request]

jobs:
  test:
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        os: [ubuntu-latest, macos-latest, windows-latest]
    
    steps:
      - uses: actions/checkout@v3
      
      - name: Install CMake
        uses: jwlawson/actions-setup-cmake@v1
        with:
          cmake-version: '3.16.x'
      
      - name: Build and Test
        run: cmake -DRUN_TESTS=ON -DCOVERAGE=ON -P build.cmake
      
      - name: Generate Coverage Report
        run: cmake -P coverage.cmake
      
      - name: Upload Coverage
        uses: actions/upload-artifact@v3
        with:
          name: coverage-${{ matrix.os }}
          path: coverage/
```

**Key Benefits:**
- ✅ **Cross-platform**: Same commands work on Linux, macOS, and Windows
- ✅ **Matrix testing**: Test on multiple operating systems simultaneously  
- ✅ **No platform-specific scripts**: One workflow for all platforms
- ✅ **Zero additional dependencies**: Only uses CMake (already required)

## 📖 Additional Documentation

- [CMake Configuration](cmake/README.md) - Build system details
- [CMocka Integration](external/cmocka/README.md) - Test framework setup
- [Git Workflow](GIT_WORKFLOW.md) - Sharing the library via git subtree

## 🤝 Contributing

When using this as a template for your projects:

1. Replace `ag_fixedpoint` with your library name
2. Update version in `CMakeLists.txt`
3. Add your source files
4. Write comprehensive tests
5. Maintain coverage above 80%
6. Document public APIs

## 📄 License

This template is provided as-is for use in your projects. Customize as needed.

## ✅ Checklist for New Projects

- [ ] Rename project in CMakeLists.txt
- [ ] Update library name in all files
- [ ] Add library source files to `ag_fixedpoint/src/`
- [ ] Add public headers to `ag_fixedpoint/include/`
- [ ] Write unit tests in `test/unit/`
- [ ] Build and run tests: `cmake -DRUN_TESTS=ON -P build.cmake`
- [ ] Check coverage: `cmake -DCOVERAGE=ON -DRUN_TESTS=ON -P build.cmake && cmake -DGENERATE_HTML=ON -P coverage.cmake`
- [ ] Configure VS Code debugging
- [ ] Update README with library-specific details
- [ ] Tag first release: `cmake -DVERSION=0.1.0 -P release.cmake`

## 🎓 Resources

- [CMocka Documentation](https://api.cmocka.org/)
- [CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
- [GCC Coverage (gcov)](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)
- [Unit Testing Best Practices](https://github.com/testdouble/contributing-tests/wiki/Test-Driven-Development)
