# 🎨 FractaVista

[![CMake on Windows](https://github.com/Krish2882005/FractaVista/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/Krish2882005/FractaVista/actions/workflows/cmake-single-platform.yml)
[![CodeFactor](https://www.codefactor.io/repository/github/krish2882005/fractavista/badge)](https://www.codefactor.io/repository/github/krish2882005/fractavista)
![C++](https://img.shields.io/badge/C++-17-blue.svg?logo=c%2B%2B)
![OpenGL](https://img.shields.io/badge/OpenGL-Rendering-blue?logo=opengl)
![License](https://img.shields.io/github/license/krish2882005/fractavista)
![Status](https://img.shields.io/badge/Status-WIP-orange)

**A modern, GPU-accelerated fractal explorer built with C++ and OpenGL.**

> **Note: This is a work in progress. New features are being added and existing ones may be refined.**

FractaVista is a high-performance, interactive desktop application for exploring the mesmerizing beauty of fractals. It leverages modern C++ and the power of GPU compute shaders to deliver a smooth, real-time exploration experience with an intuitive user interface.

<p align="center">
  <img src="https://raw.githubusercontent.com/Krish2882005/FractaVista/main/assets/FractaVista.png" alt="FractaVista Interface"/>
</p>

---

## ✨ Features

- **GPU-Accelerated Rendering**: Utilizes OpenGL compute shaders for blazingly fast fractal generation, allowing for deep, real-time exploration.
- **Multiple Fractal Algorithms**: Comes with several built-in fractal types:
  - Mandelbrot
  - Julia
  - Burning Ship
  - Cubic Mandelbrot
  - Tricorn
  - Newton
- **Interactive Navigation**: Smoothly pan and zoom into the intricate details of each fractal with simple mouse controls.
- **Real-time Parameter Control**: Tweak parameters like max iterations, zoom, position, and Julia set constants on the fly.
- **High-Resolution Export**: Save stunning, high-quality screenshots of your discoveries with built-in supersampling options (up to 8x).
- **Modern & Modular Codebase**: Written in clean, modern C++ with a strong emphasis on modularity.

## 🛠️ Technology Stack

- **Language**: C++ 17
- **Graphics API**: OpenGL 4.3+
- **Dependency Management**: [vcpkg](https://github.com/microsoft/vcpkg)
- **Windowing & Input**: [SDL3](https://github.com/libsdl-org/SDL)
- **GUI**: [Dear ImGui](https://github.com/ocornut/imgui) (with Docking)
- **OpenGL Loading**: [GLAD](https://glad.dav1d.de/)
- **Image Loading/Saving**: [SDL3\_image](https://github.com/libsdl-org/SDL_image)
- **Math**: [GLM](https://github.com/g-truc/glm)
- **JSON Parsing**: [nlohmann/json](https://github.com/nlohmann/json)
- **File Dialogs**: [Native File Dialog Extended](https://github.com/btzy/nativefiledialog-extended)
- **Logging**: [spdlog](https://github.com/gabime/spdlog)

## 🚀 Getting Started

This project uses [CMake](https://cmake.org/) and [vcpkg](https://vcpkg.io/) for dependency management to ensure a smooth build process.

### Prerequisites

- A C++ compiler that supports C++ 17 or newer (GCC, Clang, MSVC)
- [CMake](https://cmake.org/) (version 3.27 or higher)
- [Git](https://git-scm.com/)
- [vcpkg](https://github.com/microsoft/vcpkg)

### 1. Install vcpkg

If you don't have vcpkg, you can install it with the following commands:

```bash
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
# For Windows
./bootstrap-vcpkg.bat
# For Linux/macOS
./bootstrap-vcpkg.sh
# Integrate with your build environment
./vcpkg integrate install
```

### 2. Install Dependencies

Use vcpkg to install all the required libraries. Be sure to specify the correct triplet for your target platform (e.g., `x64-windows` for 64-bit Windows).

```bash
vcpkg install sdl3 sdl3-image imgui[docking-experimental,opengl3-binding,sdl3-binding] glm nlohmann-json nativefiledialog-extended spdlog zlib libpng libjpeg-turbo --triplet x64-windows
```

### 3. Build FractaVista

1. **Clone the repository:**

   ```bash
   git clone https://github.com/Krish2882005/FractaVista.git
   cd FractaVista
   ```

2. **Create a build directory:**

   ```bash
   mkdir build
   cd build
   ```

3. **Configure the project with CMake:**
   Make sure to point CMake to the vcpkg toolchain file.

   ```bash
   # Replace [path-to-vcpkg] with the actual path to your vcpkg installation
   cmake .. -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
   ```

4. **Build the project:**

   ```bash
   cmake --build . --config Release
   ```

5. **Run the application:**
   The executable `FractaVista` will be located in the `build/Release` directory.

## 🕹️ How to Use

The user interface is fully dockable, allowing you to customize the layout to your preference.

- **Viewport**:

  - **Pan**: Click and drag with the left mouse button to move around the fractal.
  - **Zoom**: Use the mouse scroll wheel to zoom in and out. The zoom is centered on your mouse cursor.

- **Menu Bar**:

  - **File**: Save and load fractal parameter presets (`.fracta` files) or quit the application.
  - **View**: Toggle the visibility of the different control panels (Properties, Coloring, Export, Status Bar).
  - **Help**: View application information in the "About" modal.

- **Properties Panel**:

  - **Algorithm**: Switch between different fractal types (Mandelbrot, Julia, etc.).
  - **Controls**: Adjust core parameters like `Max Iterations`, `Zoom`, and `Offset` coordinates in real-time.
  - **Julia Parameters**: Appears when the Julia set is selected, allowing you to modify its unique constants.

- **Coloring Panel**:

  - **Shading**: Toggle smooth coloring and adjust the palette frequency.
  - **Palette Gradient**: A fully interactive gradient editor.

    - **Left-click** on the bar to add a new color stop.
    - **Drag** existing stops to change their position.
    - **Right-click** a stop to delete it.
    - Use the color pickers below to change the color of each stop.

- **Export Panel**:

  - Configure the `Filename`, `Format` (PNG, JPG, BMP), and `Supersample` factor for high-resolution screenshots.
  - Click **Save to File** to export the current view.

## 🔮 Future Roadmap

- [ ] Implement more fractal algorithms (e.g., Nova, Magnet).
- [ ] Implement keyframe animation to export videos of fractal zooms.
- [ ] Display performance metrics (render time, FPS) in the UI.

## 📄 License

This project is licensed under the MIT License.
