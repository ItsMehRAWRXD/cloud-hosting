# RawrXD-QtShell Quick Reference

## 🏃 Quick Start

### Build CLI Only (No Dependencies)
```bash
mkdir build && cd build
cmake -DBUILD_GUI=OFF ..
make -j$(nproc)
./bin/rawrxd-cli help
```

### Build Everything (Requires Qt6)
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./bin/rawrxd-cli help
./bin/RawrXD-AgenticIDE
```

## 📁 Project Structure

| Directory | Purpose |
|-----------|---------|
| `src/core/` | Platform-independent core libraries |
| `src/cli/` | Command-line interface |
| `src/qtapp/` | Qt6 GUI application |
| `src/agent/` | Agentic systems (failure detection, correction) |
| `tests/` | Test suite |
| `build/` | Build output (gitignored) |

## 🔨 Common Build Commands

```bash
# Clean build
rm -rf build && mkdir build && cd build

# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..

# CLI only (no Qt)
cmake -DBUILD_GUI=OFF ..

# With Vulkan/ROCm stubs enabled
cmake -DENABLE_VULKAN=ON -DENABLE_ROCM=ON ..

# Build specific target
make rawrxd-cli
make RawrXD-AgenticIDE
make self_test_gate

# Run tests
make test
# or
ctest --output-on-failure
```

## 🧪 Testing Commands

```bash
# Run all tests
cd build && ctest

# Run specific test
./bin/self_test_gate

# Verbose test output
ctest --verbose
```

## 🖥️ CLI Commands Reference

| Command | Arguments | Description |
|---------|-----------|-------------|
| `help` | - | Show help message |
| `version` | - | Show version and build info |
| `devices` | - | List available GPU devices |
| `info` | - | Show system information |
| `load` | `<file>` | Load a GGUF model file |
| `patch` | `<file>` | Apply a hotpatch file |

### CLI Options

| Option | Description |
|--------|-------------|
| `--verbose` | Enable verbose output |
| `--device <type>` | Select device (cpu, vulkan, rocm) |

## 🎨 GUI Features

### Main Window
- **File Menu**: New, Open, Save, Exit
- **Edit Menu**: Undo, Redo, Cut, Copy, Paste
- **View Menu**: Toggle Minimap, Toggle Line Numbers
- **Help Menu**: About

### Keyboard Shortcuts
- `Ctrl+N` - New file
- `Ctrl+O` - Open file
- `Ctrl+S` - Save file
- `Ctrl+Q` - Exit
- `Ctrl+Z` - Undo
- `Ctrl+Y` - Redo
- `Ctrl+X` - Cut
- `Ctrl+C` - Copy
- `Ctrl+V` - Paste

## 📊 Key Files

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Build configuration |
| `src/core/patch_result.hpp` | Result structures |
| `src/core/platform_utils.*` | Platform abstractions |
| `src/core/gpu_stubs.*` | GPU stub implementations |
| `src/qtapp/model_memory_hotpatch.*` | Memory patching |
| `src/cli/main_cli.cpp` | CLI entry point |
| `src/qtapp/main_qt.cpp` | GUI entry point |
| `.gitignore` | Build artifacts exclusion |

## 🐛 Common Issues & Solutions

### Issue: CMake can't find Qt6
**Solution**: Specify Qt6 path explicitly:
```bash
cmake -DQt6_DIR="/path/to/Qt/6.7.3/gcc_64/lib/cmake/Qt6" ..
```

### Issue: Build fails with C++20 errors
**Solution**: Ensure compiler supports C++20:
- GCC 11+
- Clang 14+
- MSVC 2022+

### Issue: Qt MOC errors
**Solution**: Ensure `CMAKE_AUTOMOC=ON` in CMakeLists.txt (already set)

### Issue: Missing Qt libraries at runtime
**Linux**: Set `LD_LIBRARY_PATH`:
```bash
export LD_LIBRARY_PATH=/path/to/Qt/6.7.3/gcc_64/lib:$LD_LIBRARY_PATH
```

**Windows**: Use `windeployqt`:
```powershell
C:\Qt\6.7.3\msvc2022_64\bin\windeployqt.exe RawrXD-AgenticIDE.exe
```

## 🔧 Development Workflow

1. **Make Changes**: Edit source files in `src/`
2. **Build**: `cd build && make -j$(nproc)`
3. **Test**: `./bin/self_test_gate`
4. **Run**: `./bin/rawrxd-cli` or `./bin/RawrXD-AgenticIDE`
5. **Debug**: Use `CMAKE_BUILD_TYPE=Debug` and run with `gdb` or Visual Studio

## 🏗️ Adding New Features

### Add a New CLI Command
1. Edit `src/cli/cli_commands.hpp` - declare method
2. Edit `src/cli/cli_commands.cpp` - implement method
3. Edit `src/cli/main_cli.cpp` - add command handler
4. Rebuild and test

### Add a New Hotpatch Layer
1. Create header in `src/qtapp/` with class definition
2. Implement in corresponding `.cpp` file
3. Update `UnifiedHotpatchManager` to integrate
4. Add to `CMakeLists.txt` if new files
5. Rebuild and test

### Add a New Test
1. Edit `tests/self_test_gate.cpp`
2. Add test function following existing pattern
3. Call from `main()` function
4. Rebuild and run tests

## 📦 Build Artifacts

After successful build, find executables in:
- `build/bin/rawrxd-cli` - CLI tool
- `build/bin/RawrXD-AgenticIDE` - GUI application
- `build/bin/self_test_gate` - Test runner
- `build/lib/` - Static libraries

## 🚀 Performance Tips

- Use `CMAKE_BUILD_TYPE=Release` for production
- Enable LTO: `cmake -DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON ..`
- Use Ninja generator: `cmake -G Ninja ..` (faster than Make)
- Parallel builds: `make -j$(nproc)` or `ninja -j$(nproc)`

## 📝 Code Style

- **Indentation**: 4 spaces
- **Naming**:
  - Classes: `PascalCase`
  - Functions: `camelCase`
  - Variables: `camelCase` or `snake_case`
  - Members: `m_camelCase` (Qt style)
- **Headers**: `#pragma once` or include guards
- **Namespaces**: Always use `RawrXD::`
- **Error Handling**: Return `PatchResult`, never throw

## 🔗 Useful Links

- CMake Docs: https://cmake.org/documentation/
- Qt6 Docs: https://doc.qt.io/qt-6/
- C++20 Reference: https://en.cppreference.com/w/cpp/20
- Project README: `README-RAWRXD.md`
