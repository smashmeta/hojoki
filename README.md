# Hojoki

A Qt 6 task board for experimenting with interactive graphics. The current board uses Qt Graphics View (`QGraphicsScene` and reusable `QGraphicsObject` items); Qt Quick remains available for later Scene Graph experiments.

## Requirements

- Windows with Visual Studio 2026 C++ tools (x64)
- CMake 3.23 or newer
- vcpkg, with `VCPKG_ROOT` pointing at its directory
- VS Code with the recommended C/C++ and CMake Tools extensions (for launch/debug)

The first configure installs Qt Base, Qt Declarative, GoogleTest/GoogleMock, and spdlog through vcpkg. Qt may take a while to build on a fresh machine.

## Build and test

```powershell
cmake --preset windows-debug
cmake --build --preset debug
ctest --preset debug
```

Select the `windows-debug` configure preset and the `hojoki` launch target in CMake Tools. Press F5 and choose **Hojoki (CMake target)** to start the app. CMake Tools resolves the built executable path for `launch.json`.

The test runs Qt with its offscreen platform plugin so it can run without displaying a window.
The build copies Qt's Windows platform plugin into the executable's `platforms` directory for launching from VS Code or Explorer.

## Task board

Drag a task to another slot to change its priority. Tasks between the old and new slots animate into place. Priority follows the arrow: left to right on the first row, right to left on the second, then left to right on the third. The number on each task stays with that task when it moves.

Release a task outside the board's white border, or press **Escape** while dragging, to cancel that move and restore the previous order.

Right click a task to set its state to **busy** (red), **pending** (yellow), or **done** (green).
