# LibWDM framework

LibWDM is an attempt to recreate Windows 7 desktop environment inside
[raylib](https://www.raylib.com/). It supports windowed, windowed borderless and fullscreen windows.

![Screenshot from 2023-07-30 02-06-21](https://github.com/SergeyMC9730/aeroray/assets/57207009/504bc507-b074-48e5-8389-dde57b8da24d)

# Additional software

This repository also contains example applications that can be used with libwdm to get better experience with this framework. Aeroray includes taskbar and background implementations (with wallpaper switcher).

# Replacement resources

To have no issues with Microsoft, I made replacement textures instead. It contains 4 backgrounds and required Segoe UI font family (made by Microsoft (C)). libdwm uses only these textures and fonts -

- font Segoe UI;
- texture `resources/ui/exitbutton.png`;
- texture `resources/ui/exitbutton_selected.png`;
- Gaussian Blur fragment shader `resources/shaders/blur.fs`.

Everything else here is used by additional software.

# Usage

`CMakeLists.txt` generates two main targets -- `libdwm` as a main library and `aeroray` - program, that uses this framework. `raylib` library is already included.

# Sources

Backgrounds: [1](https://i.redd.it/q4oiqc4fo4bb1.jpg) | [2](https://static.wikia.nocookie.net/aesthetics/images/1/13/EjOHWqqVgAAg_VB.jpg/revision/latest?cb=20210413190412) | [3](https://wallpapers.com/wallpapers/neon-windows-7-logo-6fixm6qgz680lsi3.html) | [4](https://wallpapers.com/wallpapers/minimalistic-blue-and-black-windows-7-screen-vzsv5pakmaxq9apg.html)
