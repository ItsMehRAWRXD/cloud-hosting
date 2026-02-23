param(
    [string]$Config = "Release",
    [string]$A = "x64"
)

Write-Host "=== Building RawrXD-AgenticIDE ===" -ForegroundColor Cyan
Write-Host "Configuration: $Config"
Write-Host "Architecture: $A"
Write-Host ""

# Configure CMake
Write-Host ">>> Configuring CMake..." -ForegroundColor Yellow
$BuildDir = "build_${Config}_${A}"
cmake -B $BuildDir -A $A -DCMAKE_BUILD_TYPE=$Config
if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed"
    exit 1
}

# Build
Write-Host ">>> Building..." -ForegroundColor Yellow
cmake --build $BuildDir --config $Config --target RawrXD-AgenticIDE
if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed"
    exit 1
}

# Check output
$ExePath = "$BuildDir\bin\$Config\RawrXD-AgenticIDE.exe"
if (Test-Path $ExePath) {
    $Size = (Get-Item $ExePath).Length / 1MB
    Write-Host ""
    Write-Host "=== Build Successful ===" -ForegroundColor Green
    Write-Host "Executable: $ExePath"
    Write-Host "Size: $([math]::Round($Size, 2)) MB"
} else {
    Write-Error "Build succeeded but executable not found at: $ExePath"
    exit 1
}
