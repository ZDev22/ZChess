# ZCHESS - ZDEV'S CHESS
If you find an issue, please report it! I'm sure there are plenty of bugs lol
was a hobby project a few weeks before release, but i just recently ported it with GUI in ZEngine

# Clone the repo:
```bash
git clone --depth=1 https://github.com/ZDev22/ZEngine
cd ZEngine
```
# Dependencies:
## LINUX:
#### Arch:
```bash
sudo pacman -Syu --noconfirm
sudo pacman -S gcc cmake gdb --noconfirm #C/C++/Cmake/Cdbg
sudo pacman -S libx11 libxrandr libxcursor xkbcommon mesa --noconfirm #X11
sudo pacman -S vulkan-headers vulkan-tools vulkan-icd-loader --noconfirm #Vulkan
# Intel GPU: sudo pacman -S vulkan-intel --noconfirm
# AMD GPU: sudo pacman -S vulkan-radeon --noconfirm
```
#### Debian:
```bash
sudo apt update
sudo apt install build-essential cmake gdb -y #C/C++/Cmake/Cdbg
sudo apt install libx11-dev libxrandr-dev libxkbcommon-dev libegl1-mesa-dev libxcursor-dev libxi-dev -y #X11
sudo apt install libvulkan-dev libvulkan1 mesa-vulkan-drivers vulkan-tools -y #Vulkan
```
#### Fedora:
```bash
sudo dnf update -y
sudo dnf install gcc gcc-c++ cmake gdb -y #C/C++/CMake/Cdbg
sudo dnf install libX11-devel libXrandr-devel libxkbcommon-devel mesa-libEGL-devel libXi-devel libXcursor-devel -y #X11
sudo dnf install vulkan-headers vulkan-tools vulkan-loader-devel -y #Vulkan
# Intel GPU: sudo dnf install vulkan-intel -y
# AMD GPU: sudo dnf install vulkan-radeon -y
```
#### Gentoo:
```bash
root # emerge --sync
sudo emerge --ask sys-devel/gcc sys-devel/cmake sys-devel/gdb -y #C/C++/CMake/Cdbg
sudo emerge --ask x11-libs/libX11 x11-libs/libXrandr x11-libs/libXi x11-libs/libXcursor x11-libs/libxkbcommon mesa-libs/mesa -y #X11
sudo emerge --ask media-libs/vulkan-loader dev-util/vulkan-headers media-libs/vulkan-tools -y #Vulkan
# Intel GPU: sudo emerge --ask media-libs/vulkan-intel
# AMD GPU: sudo emerge --ask media-libs/vulkan-radeon
```
#### Leap:
```bash
sudo zypper refresh
sudo zypper update -y
sudo zypper install -t pattern devel_C_C++ cmake gdb -y #C/C++/CMake/Cdbg
sudo zypper install libX11-devel libXrandr-devel libxkbcommon-devel Mesa-libEGL-devel libXi-devel libXcursor-devel -y #X11
sudo zypper install vulkan-headers vulkan-loader-devel vulkan-tools -y #Vulkan
# Intel GPU: sudo zypper install vulkan-intel -y
# AMD GPU: sudo zypper install vulkan-radeon -y
```
#### Alpine:
```bash
sudo apk update
sudo apk add build-base cmake gdb #C/C++/CMake/Cdbg
sudo apk add libx11-dev libxrandr-dev libxkbcommon-dev libxi-dev libxcursor-dev mesa-egl-dev #X11
sudo apk add vulkan-headers vulkan-tools vulkan-loader-dev #Vulkan
# Intel GPU: sudo apk add vulkan-intel
# AMD GPU: sudo apk add vulkan-radeon
```
## MAC (beta):
- Install XCode from the app store
- Install the lldb extension
```bash
brew install gcc cmake lld #C/C++/Cmake/Cppdbg
brew install vulkan-tools vulkan-headers molten-vk #Vulkan
```
## WINDOWS:
- Download the **Windows Installer** from [https://cmake.org/download/](https://cmake.org/download/)
- Download vulkan from: [https://vulkan.lunarg.com/sdk/home](https://vulkan.lunarg.com/sdk/home)
- Download msys2 from [https://www.msys2.org/](https://www.msys2.org/) **CLOSE THE TERMINAL**
- Launch MSYS2 MSYS (the purple one)
- Run the following commands to download a C++ compiler:
```bash
pacman -Syu
pacman -S base-devel mingw-w64-x86_64-toolchain
```
- In the windows search bar, look up "Edit environment variables"
- Add C:\msys64\ucrt64\bin to your user (or system) PATH
- Add C:\msys64\mingw64\bin to your user (or system) PATH
- Restart your IDE (or terminal)

## Extensions (Linux/Windows):
- Install **@category:debuggers cppdbg** or **ms-vscode.cpptools** to run the program in VSCode
- Install the **cppdbg** extension to run the program in VSCodium

# How do make your own games:
- Duplicate flappybird.hpp as a good base for your game
- Delete the tick() implementation and replace it with your own
- In main.cpp make sure to include your game instead of the example
- ZDeps documentation is inside their respective files
- sprites[0] is the default sprite, you dont need to create it

# Included dependencies:
[VULKAN (Graphics)](https://vulkan.lunarg.com/sdk/home)</br>
[RGFW (Window)](https://github.com/ColleagueRiley/RGFW)</br>
[STB-IMAGE (Image loader) {Modified}](https://github.com/nothings/stb)</br>

# Build commands:
## Linux:
```bash
cmake --preset linuxrelease
cmake --build --preset linuxrelease -j4
```
## Mac:
```bash
cmake --preset macrelease
cmake --build --preset macrelease -j4
```
## Windows:
```bash
cmake --preset windowsrelease
cmake --build --preset windowsrelease -j4
```
### Availiable presets:
```
linuxdebug    linuxrelease    linuxsmol
macdebug      macrelease      macsmol
windowsdebug  windowsrelease  windowssmol
```

# Run your games:
## Linux:
```bash
cd build/linux/Release
./main
```
## Mac:
```bash
cd build/mac/Release
./main
```
## Windows:
```bash
cd build\windows\Release
.\main
```
