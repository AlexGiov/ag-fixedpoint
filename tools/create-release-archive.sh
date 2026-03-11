#!/bin/bash
################################################################################
# create-release-archive.sh
# 
# Create a release archive containing only library essentials
#
# Usage: ./create-release-archive.sh <version> [output_dir]
# Example: ./create-release-archive.sh 1.0.0
#          ./create-release-archive.sh v1.1.0 ./releases
#
# Author: Alex Giovannini
# Date: 2026-03-11
################################################################################

set -e  # Exit on error

# =============================================================================
# Configuration
# =============================================================================

VERSION="$1"
OUTPUT_DIR="${2:-.}"

if [ -z "$VERSION" ]; then
    echo "Usage: $0 <version> [output_dir]"
    echo "Example: $0 1.0.0"
    echo "         $0 v1.1.0 ./releases"
    exit 1
fi

# Remove 'v' prefix if present
CLEAN_VERSION="${VERSION#v}"

# Project configuration
PROJECT_NAME="ag_fixedpoint"
ARCHIVE_NAME="$PROJECT_NAME-$CLEAN_VERSION"
OUTPUT_PATH="$OUTPUT_DIR/$ARCHIVE_NAME.tar.gz"

# Files to include
FILES_TO_INCLUDE=(
    "include"
    "src"
    "config"
    "cmake"
    "CMakeLists.txt"
    "README.md"
)

OPTIONAL_FILES=(
    "LICENSE"
    "CHANGELOG.md"
)

# =============================================================================
# Functions
# =============================================================================

print_header() {
    echo ""
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo " Creating Release Archive for $PROJECT_NAME"
    echo " Version: $CLEAN_VERSION"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo ""
}

check_git_repo() {
    if ! git rev-parse --git-dir > /dev/null 2>&1; then
        echo "❌ Error: Not in a Git repository"
        exit 1
    fi
}

check_git_status() {
    if [ -n "$(git status --porcelain)" ]; then
        echo "⚠️  Warning: Working directory is not clean"
        echo "Uncommitted changes will not be included in the archive"
        echo ""
    fi
}

# =============================================================================
# Main Script
# =============================================================================

print_header

# Validate environment
check_git_repo
check_git_status

# Create output directory if needed
mkdir -p "$OUTPUT_DIR"

# Remove existing archive
if [ -f "$OUTPUT_PATH" ]; then
    echo "🗑️  Removing existing archive..."
    rm -f "$OUTPUT_PATH"
fi

# Build file list for git archive
FILE_LIST=""
for item in "${FILES_TO_INCLUDE[@]}"; do
    if [ -e "$item" ]; then
        FILE_LIST="$FILE_LIST $item"
        echo "  ✓ Including: $item"
    else
        echo "  ⚠ Not found: $item"
    fi
done

# Add optional files if they exist
for item in "${OPTIONAL_FILES[@]}"; do
    if [ -f "$item" ]; then
        FILE_LIST="$FILE_LIST $item"
        echo "  ✓ Including: $item"
    fi
done

# Create archive using git archive (clean, excludes build artifacts)
echo ""
echo "📦 Creating archive..."

git archive --format=tar.gz \
    --prefix="$ARCHIVE_NAME/" \
    -o "$OUTPUT_PATH" \
    HEAD \
    $FILE_LIST

# Display results
echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "✅ Release archive created successfully!"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
echo "📄 Archive: $OUTPUT_PATH"
echo "📊 Size: $(du -h "$OUTPUT_PATH" | cut -f1)"
echo ""
echo "Next steps:"
echo "  1. Test the archive:"
echo "     tar -xzf $OUTPUT_PATH"
echo "     cd $ARCHIVE_NAME"
echo "     cmake -B build -S ."
echo ""
echo "  2. Create Git tag (if not already done):"
echo "     git tag -a v$CLEAN_VERSION -m 'Release $CLEAN_VERSION'"
echo "     git push origin v$CLEAN_VERSION"
echo ""
echo "  3. Upload to GitHub Releases:"
echo "     https://github.com/YOUR_USERNAME/$PROJECT_NAME/releases/new"
echo ""
