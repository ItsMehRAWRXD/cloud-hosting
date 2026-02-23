param(
    [ValidateSet("Debug", "Release")]
    [string]$Config = "Release",

    [ValidateSet("x64", "Win32", "ARM64")]
    [string]$A = "x64"
)

$ErrorActionPreference = "Stop"

$BuildDir = Join-Path $PSScriptRoot ".." "build"
$SourceDir = Join-Path $PSScriptRoot ".."

Write-Host "=== RawrXD-AgenticIDE Build ===" -ForegroundColor Cyan
Write-Host "  Configuration: $Config"
Write-Host "  Architecture:  $A"
Write-Host ""

# Create build directory
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
    Write-Host "Created build directory: $BuildDir"
}

# Configure
Write-Host "Configuring with CMake..." -ForegroundColor Yellow
cmake -S $SourceDir -B $BuildDir -G "Visual Studio 17 2022" -A $A
if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configure FAILED (exit code $LASTEXITCODE)" -ForegroundColor Red
    exit $LASTEXITCODE
}
Write-Host "Configure succeeded." -ForegroundColor Green

# Build
Write-Host "Building ($Config)..." -ForegroundColor Yellow
cmake --build $BuildDir --config $Config
if ($LASTEXITCODE -ne 0) {
    Write-Host "Build FAILED (exit code $LASTEXITCODE)" -ForegroundColor Red
    exit $LASTEXITCODE
}

Write-Host ""
Write-Host "Build succeeded: $Config | $A" -ForegroundColor Green
exit 0
