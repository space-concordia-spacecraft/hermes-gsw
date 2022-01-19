# Hermes Ground Station Software (GSW)

## Installation & Setup

### Requirements

- [x] Visual Studio 2019 or 2022 ([link](https://visualstudio.microsoft.com/downloads/))
- [x] CLion ([link](https://www.jetbrains.com/clion/))
- [x] vcpkg ([link](https://github.com/microsoft/vcpkg))

### Setup

1. Install vcpkg by following the instructions [here](https://github.com/microsoft/vcpkg#quick-start-windows).
2. Install the required packages:
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
3. Clone the repo.
4. Open the repo as a CLion project.
5. Navigate to `File => Settings => Build, Execution, Deployment => Toolchains`.
6. Create a Visual Studio and let CLion autodetect all paths and settings.
7. Navigate to `File => Settings => Build, Execution, Deployment => CMake`.
8. Create two CMake profiles for Debug and Release respectively.
9. Set each of the two CMake profiles to use the Visual Studio toolchain you created above.
10. In the `CMake Options` field, add the following (and replace with your vcpkg path):
```
-DCMAKE_TOOLCHAIN_FILE="<path/to/vcpkg>/scripts/buildsystems/vcpkg.cmake"
```
10. Save your changes and close the settings dialog.
11. Right-click the root `CMakeLists.txt` file and load the CMake project.
12. You should now be able to build the GSW executable through the run configuration created by CMake.