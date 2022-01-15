# Hermes Ground Station Software (GSW)

## Installation & Setup

### Requirements

- [x] Visual Studio 2019 or 2022 ([link](https://visualstudio.microsoft.com/downloads/))
- [x] CLion ([link](https://www.jetbrains.com/clion/))
- [x] vcpkg ([link](https://github.com/microsoft/vcpkg))

### Setup

1. Install vcpkg by following the instructions [here](https://github.com/microsoft/vcpkg#quick-start-windows).
2. Clone the repo.
3. Open the repo as a CLion project.
4. Navigate to `File => Settings => Build, Execution, Deployment => Toolchains`.
5. Create a Visual Studio and let CLion autodetect all paths and settings.
6. Navigate to `File => Settings => Build, Execution, Deployment => CMake`.
7. Create two CMake profiles for Debug and Release respectively.
8. Set each of the two CMake profiles to use the Visual Studio toolchain you created above.
9. In the `CMake Options` field, add the following (and replace with your vcpkg path):
```
-DCMAKE_TOOLCHAIN_FILE="<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake"
```
10. Save your changes and close the settings dialog.
11. Right-click the root `CMakeLists.txt` file and load the CMake project.
12. You should now be able to build the GSW executable through the run configuration created by CMake.