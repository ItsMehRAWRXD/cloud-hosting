# RawrXD IDE - Qt Application

## Overview
RawrXD IDE is a comprehensive Qt6-based development environment with 23 specialized UI widgets for code editing, collaboration, monitoring, and productivity.

## Features

### 🎨 Collaboration Tools
- **WhiteboardWidget** - Interactive drawing canvas with save/load
- **AudioCallWidget** - Audio call simulation with timer
- **ScreenShareWidget** - Real-time screen capture
- **InlineChatWidget** - AI-powered chat interface

### 📊 Monitoring & Analytics
- **TelemetryWidget** - Real-time system metrics visualization
- **ProgressManager** - Multi-task progress tracking
- **AICompletionCache** - Cache statistics display
- **LanguageClientHost** - LSP status monitoring

### 💻 Code Tools
- **CodeStreamWidget** - Live code streaming with syntax highlighting
- **AIReviewWidget** - AI code review with severity levels
- **CodeMinimap** - Code overview minimap
- **SearchResultWidget** - Search results browser

### ⏱️ Productivity
- **TimeTrackerWidget** - Precision timer with lap tracking
- **TaskManagerWidget** - Task management with priorities
- **PomodoroWidget** - Pomodoro technique timer
- **TodoWidget** - TODO list with persistence
- **BookmarkWidget** - Code bookmarks

### 🛠️ Utilities
- **MacroRecorderWidget** - Macro recording and playback
- **PluginManagerWidget** - Plugin management
- **AccessibilityWidget** - Accessibility settings
- **UpdateCheckerWidget** - Software update checker
- **WelcomeScreenWidget** - Welcome screen with quick links
- **BreadcrumbBar** - Navigation breadcrumbs

## Building

### Prerequisites
- CMake 3.20+
- Qt6 (Core, Widgets, Gui, Charts)
- C++20 compatible compiler (GCC 13+, Clang 14+, MSVC 2022+)

### Ubuntu/Debian
```bash
sudo apt-get install qt6-base-dev qt6-charts-dev cmake build-essential
```

### Build Commands
```bash
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
```

### Run
```bash
./build/bin/RawrXD-QtShell
```

## Architecture

### Widget System
- All widgets inherit from `QWidget`
- Integrated as dock widgets in `MainWindow`
- Accessible via Tools menu
- Persistent settings via `QSettings`

### Project Structure
```
src/qtapp/
├── main_qt.cpp           # Application entry point
├── MainWindow.{h,cpp}    # Main window with menu system
├── Subsystems.h          # Widget includes
└── widgets/              # 23 widget implementations
    ├── whiteboard_widget/
    ├── audio_call_widget/
    ├── screen_share_widget/
    └── ... (20 more)
```

### Build System
- CMake with automatic MOC/UIC
- Static libraries per widget
- Qt6 module linking

## Usage

1. Launch RawrXD-QtShell
2. Access widgets via **Tools** menu
3. Widgets appear as dock panels
4. Drag widgets to customize layout
5. Settings persist between sessions

## Widget Highlights

### WhiteboardWidget
- Drawing with customizable brush
- Color picker
- Undo/redo support
- Save/load images (PNG, JPEG)

### TelemetryWidget
- Real-time metrics (CPU, Memory, Network, Disk)
- Qt Charts line graphs
- 60-second rolling window
- Multiple metric types

### TimeTrackerWidget
- Millisecond precision (HH:MM:SS.mmm)
- Lap recording
- Start/Stop/Pause controls
- Persistent lap history

### AIReviewWidget
- Code review with 5 severity levels
- Color-coded issue display
- Filter by severity
- Detailed issue information

## Development

### Adding New Widgets
1. Create widget directory: `src/qtapp/widgets/my_widget/`
2. Create header: `my_widget.h`
3. Create implementation: `my_widget.cpp`
4. Create CMakeLists.txt
5. Add to root CMakeLists.txt
6. Include in Subsystems.h
7. Add to MainWindow menu

### Widget Template
```cpp
class MyWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);
private slots:
    void onAction();
private:
    void setupUI();
};
```

## Technical Details

- **Language**: C++20
- **Framework**: Qt 6.4.2+
- **Lines of Code**: ~3600
- **Widgets**: 23
- **Binary Size**: ~761 KB
- **Platform**: Cross-platform (Linux, Windows, macOS)

## License
MIT License - See LICENSE file for details

## Credits
RawrXD Team - Advanced GGUF Model Loader with Live Hotpatching & Agentic Correction

## Related Documentation
- [WIDGETS_IMPLEMENTATION_SUMMARY.md](WIDGETS_IMPLEMENTATION_SUMMARY.md) - Detailed implementation notes
- [Cloud Hosting Guide](README.md) - DigitalOcean deployment instructions
