# Hermes Ground Station Software (GSW)

## Installation & Setup

### Requirements
- [x] Visual Studio 2022 ([link](https://visualstudio.microsoft.com/downloads/)) or Visual Studio 2019 ([link](https://visualstudio.microsoft.com/vs/older-downloads/https://visualstudio.microsoft.com/vs/older-downloads/))
- [x] CLion ([link](https://www.jetbrains.com/clion/))
- [x] vcpkg ([link](https://github.com/microsoft/vcpkg))

### Setup

1. Install vcpkg by following the instructions [here](https://github.com/microsoft/vcpkg#quick-start-windows).
2. Make sure to run the following command:
```
vcpkg integrate install
```
3. Install the required packages:
- [x] glad
- [x] glfw3
- [x] imgui (docking branch, opengl and glfw bindings)
- [x] imguizmo
- [x] implot
- [x] glm
- [x] spdlog
```
vcpkg install glfw3 glad imgui[core,docking-experimental,opengl3-binding,glfw-binding] imguizmo implot glm spdlog --triplet x64-windows
```
4. Clone the repo.
5. Open the repo as a CLion project.
6. Navigate to `File => Settings => Build, Execution, Deployment => Toolchains`.
7. Create a Visual Studio and let CLion autodetect all paths and settings.
8. Set the architecture to amd64 for the toolchain.
9. Navigate to `File => Settings => Build, Execution, Deployment => CMake`.
10. Create two CMake profiles for Debug and Release respectively.
11. Set each of the two CMake profiles to use the Visual Studio toolchain you created above.
12. In the `CMake Options` field, add the following (and replace with your vcpkg path):
```
-DCMAKE_TOOLCHAIN_FILE="<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake"
```
12. Save your changes and close the settings dialog.
13. Right-click the root `CMakeLists.txt` file and load the CMake project.
14. You should now be able to build the GSW executable through the run configuration created by CMake.

### Documentation

