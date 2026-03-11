#!/usr/bin/env pwsh
<#
.SYNOPSIS
    Create a release archive containing only library essentials

.DESCRIPTION
    This script exports only the necessary files for library distribution

.PARAMETER Version
    Version string (e.g., "1.0.0" or "v1.0.0")

.PARAMETER OutputDir
    Output directory for the archive (default: current directory)

.EXAMPLE
    .\create-release-archive.ps1 -Version "1.0.0"

.NOTES
    Author: Alex Giovannini
    Date: 2026-03-11
#>

param(
    [Parameter(Mandatory = $true)]
    [string]$Version,
    
    [Parameter(Mandatory = $false)]
    [string]$OutputDir = "."
)

# =============================================================================
# Configuration
# =============================================================================

$ErrorActionPreference = "Stop"

# Remove 'v' prefix if present
$CleanVersion = $Version -replace '^v', ''

# Project name and paths
$ProjectName = "ag_fixedpoint"
$ArchiveName = "$ProjectName-$CleanVersion"
$TempDir = Join-Path $env:TEMP $ArchiveName
$OutputPath = Join-Path $OutputDir "$ArchiveName.zip"

# Files and directories to include in release
$FilesToInclude = @(
    "include",
    "src",
    "config",
    "cmake", 
    "CMakeLists.txt",
    "README.md"
)

# Optional files (include if they exist)
$OptionalFiles = @(
    "LICENSE",
    "CHANGELOG.md"
)

# =============================================================================
# Functions
# =============================================================================

function Write-ColorMessage {
    param([string]$Message, [string]$Color = "White")
    Write-Host $Message -ForegroundColor $Color
}

function Test-GitRepository {
    try {
        git rev-parse --git-dir | Out-Null
        return $true
    }
    catch {
        return $false
    }
}

# =============================================================================
# Main Script
# =============================================================================

Write-ColorMessage "==========================================" Cyan
Write-ColorMessage " Creating Release Archive for $ProjectName" Cyan
Write-ColorMessage " Version: $CleanVersion" Cyan
Write-ColorMessage "==========================================" Cyan
Write-Host ""

# Check if we're in a Git repository
if (-not (Test-GitRepository)) {
    Write-ColorMessage "[ERROR] Not in a Git repository" Red
    exit 1
}

# Check if working directory is clean
$gitStatus = git status --porcelain
if ($gitStatus) {
    Write-ColorMessage "[WARNING] Working directory is not clean" Yellow
    Write-ColorMessage "Uncommitted changes will not be included in the archive" Yellow
    Write-Host ""
}

# Create temporary directory
Write-ColorMessage "[INFO] Creating temporary directory..." White
if (Test-Path $TempDir) {
    Remove-Item -Path $TempDir -Recurse -Force
}
New-Item -Path $TempDir -ItemType Directory | Out-Null

# Copy required files using git archive (preserves Git history cleanliness)
Write-ColorMessage "[INFO] Exporting files from Git..." White

foreach ($item in $FilesToInclude) {
    $itemPath = Join-Path $PWD $item
    if (Test-Path $itemPath) {
        $destPath = Join-Path $TempDir $item
        
        if (Test-Path $itemPath -PathType Container) {
            # Directory - copy recursively
            Copy-Item -Path $itemPath -Destination $destPath -Recurse -Force
            Write-ColorMessage "  [OK] Copied: $item/" Green
        }
        else {
            # File - copy directly
            Copy-Item -Path $itemPath -Destination $destPath -Force
            Write-ColorMessage "  [OK] Copied: $item" Green
        }
    }
    else {
        Write-ColorMessage "  [SKIP] Not found: $item" Yellow
    }
}

# Copy optional files
foreach ($item in $OptionalFiles) {
    $itemPath = Join-Path $PWD $item
    if (Test-Path $itemPath) {
        Copy-Item -Path $itemPath -Destination (Join-Path $TempDir $item) -Force
        Write-ColorMessage "  [OK] Copied: $item" Green
    }
}

# Create output directory if it doesn't exist
if (-not (Test-Path $OutputDir)) {
    New-Item -Path $OutputDir -ItemType Directory | Out-Null
}

# Remove existing archive if present
if (Test-Path $OutputPath) {
    Write-ColorMessage "[INFO] Removing existing archive..." White
    Remove-Item -Path $OutputPath -Force
}

# Create ZIP archive
Write-ColorMessage "[INFO] Creating archive..." White
Compress-Archive -Path "$TempDir\*" -DestinationPath $OutputPath -CompressionLevel Optimal

# Clean up temporary directory
Write-ColorMessage "[INFO] Cleaning up..." White
Remove-Item -Path $TempDir -Recurse -Force

# Display results
Write-Host ""
Write-ColorMessage "========================================" Cyan
Write-ColorMessage " Release archive created successfully!" Green
Write-ColorMessage "========================================" Cyan
Write-Host ""
Write-ColorMessage "Archive: $OutputPath" White
$archiveSize = (Get-Item $OutputPath).Length / 1KB
Write-ColorMessage "Size: $([math]::Round($archiveSize, 2)) KB" White
Write-Host ""
Write-ColorMessage "Next steps:" Yellow
Write-ColorMessage "  1. Test the archive:" White
Write-ColorMessage "     Expand-Archive -Path $OutputPath -DestinationPath .\test-extract" DarkGray
Write-ColorMessage "     cd test-extract\$ArchiveName" DarkGray
Write-ColorMessage "     cmake -B build -S ." DarkGray
Write-Host ""
Write-ColorMessage "  2. Create Git tag (if not already done):" White
Write-ColorMessage "     git tag -a v$CleanVersion -m `"Release $CleanVersion`"" DarkGray
Write-ColorMessage "     git push origin v$CleanVersion" DarkGray
Write-Host ""
Write-ColorMessage "  3. Upload to GitHub Releases:" White
Write-ColorMessage "     https://github.com/YOUR_USERNAME/$ProjectName/releases/new" DarkGray
Write-Host ""
