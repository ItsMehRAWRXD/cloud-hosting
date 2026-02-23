# RawrXD-AgenticIDE Build Guide

## Overview

This repository integrates the RawrXD-AgenticIDE from the [ItsMehRAWRXD/RawrXD](https://github.com/ItsMehRAWRXD/RawrXD) repository as a submodule for cloud hosting and CI/CD purposes.

## Prerequisites

### Windows
- Visual Studio 2022 (MSVC compiler)
- CMake 3.20 or higher
- Qt 6.7.3 (MSVC 2022 64-bit)
- NASM (for assembly kernels)
- Git (with submodule support)

### Linux
- GCC or Clang
- CMake 3.20 or higher
- Qt6 development packages
- NASM

## Quick Build

### Windows (PowerShell)

```powershell
# Clone with submodules
git clone --recursive https://github.com/ItsMehRAWRXD/cloud-hosting.git
cd cloud-hosting

# Build Release x64
.\scripts\build.ps1 -Config Release -A x64

# Build Debug x64
.\scripts\build.ps1 -Config Debug -A x64

# Build Win32
.\scripts\build.ps1 -Config Release -A Win32
```

### Linux (Bash)

```bash
# Clone with submodules
git clone --recursive https://github.com/ItsMehRAWRXD/cloud-hosting.git
cd cloud-hosting

# Configure
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --target RawrXD-AgenticIDE -j$(nproc)
```

## Build Output

The compiled executable will be located at:
- Windows: `build_<Config>_<Arch>/bin/<Config>/RawrXD-AgenticIDE.exe`
- Linux: `build/bin/RawrXD-AgenticIDE`

## Architecture

```
cloud-hosting/                    # This repository
├── CMakeLists.txt                # Top-level build configuration
├── RawrXD/                       # Git submodule
│   └── RawrXD-ModelLoader/       # Main IDE source
│       ├── CMakeLists.txt        # IDE build config (modified)
│       ├── src/
│       │   ├── win32app/         # Windows IDE implementation
│       │   ├── gguf_loader.cpp   # Model loading
│       │   └── ...
│       └── include/              # Headers
├── scripts/
│   └── build.ps1                 # Build automation script
└── .github/workflows/            # CI/CD configurations
    ├── build.yml                 # Multi-platform build
    └── ci.yml                    # Continuous integration
```

## Build Target: RawrXD-AgenticIDE

The `RawrXD-AgenticIDE` target is defined in `RawrXD/RawrXD-ModelLoader/CMakeLists.txt` and includes:

### Source Files
- Win32 IDE framework (Win32IDE.cpp, Win32IDE.h)
- Agentic bridge and agent commands
- Autonomous operation support  
- File operations and debugger integration
- PowerShell panel and terminal manager
- Transparent rendering engine
- GGUF model loader
- Sidebar and VSCode-style UI

### Dependencies
- comctl32 (Windows common controls)
- d3d11, dxgi, dcomp (DirectX graphics)
- d2d1, dwrite (Direct2D/DirectWrite)
- winhttp (HTTP client)
- shlwapi, shell32 (Shell utilities)

### Build Options
- `ENABLE_VULKAN=ON/OFF` - Enable Vulkan renderer (default: OFF)
- `ENABLE_COPILOT=ON/OFF` - Enable Copilot integration (default: OFF)

## Modifications from Original

The following changes were made to enable cloud-hosting integration:

1. **Target Renamed**: `RawrXD-Win32IDE` → `RawrXD-AgenticIDE`
   - Matches CI/CD workflow expectations
   - Located in `RawrXD/RawrXD-ModelLoader/CMakeLists.txt:113`

2. **Target Enabled**: Removed `if(FALSE AND MSVC)` guard
   - Original was disabled with comment "Disabled until headers exist"
   - All required source files are present and functional

3. **Output Directory**: Changed from `bin-msvc` to `bin/${CMAKE_BUILD_TYPE}`
   - Supports multiple configurations (Debug/Release)
   - Compatible with CI/CD artifact collection

4. **Submodule Integration**: Added RawrXD as git submodule
   - Commit: `b6ef06ef` in submodule
   - Allows tracking specific IDE version

## Troubleshooting

### Submodule Not Initialized
```bash
git submodule update --init --recursive
```

### Qt Not Found
Set Qt6_DIR environment variable:
```powershell
# Windows
$env:Qt6_DIR = "C:\Qt\6.7.3\msvc2022_64\lib\cmake\Qt6"

# Linux
export Qt6_DIR="/usr/lib/x86_64-linux-gnu/cmake/Qt6"
```

### Build Fails with Missing Headers
Ensure all submodules are up to date:
```bash
git submodule update --remote
```

### NASM Not Found
Windows (using Chocolatey):
```powershell
choco install nasm -y
```

Linux:
```bash
sudo apt-get install nasm
```

## CI/CD Integration

The repository includes GitHub Actions workflows that:
1. Check out code with submodules (`submodules: recursive`)
2. Install dependencies (Qt, NASM, MSVC/GCC)
3. Configure CMake
4. Build RawrXD-AgenticIDE
5. Verify executable exists
6. Upload artifacts

See `.github/workflows/build.yml` and `.github/workflows/ci.yml` for details.

## Contributing

When making changes:
1. **Core IDE changes**: Submit to [ItsMehRAWRXD/RawrXD](https://github.com/ItsMehRAWRXD/RawrXD)
2. **Build integration changes**: Submit to this repository
3. **CMakeLists.txt modifications**: Document and coordinate with upstream

## License

Inherits license from RawrXD repository (see RawrXD/LICENSE).
