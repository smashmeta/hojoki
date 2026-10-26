# Hojoki

A small Qt 6 Widgets application for learning and experimenting with Qt Quick's Scene Graph. The first window is intentionally empty. Qt Quick is already available to the CMake project for future experiments.

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
