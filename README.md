<p align="center">
  <img src="https://media.discordapp.net/attachments/1263187470737080322/1320509379187048458/Untitled525_20241214210738.png?ex=67751071&is=6773bef1&hm=edb7a01e04fa5b874debda17e73a034d2cf6df76cd5a0fa45120cde0e6621bad&=&format=webp&quality=lossless&width=550&height=275" alt="GrowBase Logo">
</p>

<div align="center">

[![GitHub Stars](https://img.shields.io/github/stars/Debugtopia/GrowBase?style=flat-square)](https://github.com/Debugtopia/GrowBase/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/Debugtopia/GrowBase?style=flat-square)](https://github.com/Debugtopia/GrowBase/network/members)
[![Discord](https://img.shields.io/discord/1123020686852636735?style=flat-square&label=Discord&logo=discord)](https://discord.gg/2ynZ4cT6G6)

</div>

# GrowBase - Open Source Growtopia Server

A barebones Growtopia server implementation written in C++.

## Table of Contents
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Windows Setup](#windows-setup)
  - [Linux Setup](#linux-setup)
- [Configuration](#configuration)
- [Support](#support)
- [Credits](#credits)

## Features
- Minimal and efficient implementation
- Cross-platform support (Windows & Linux)
- Easy to modify and extend
- Built with modern C++

## Prerequisites
- C++ compiler with C++20 support
- CMake 3.10 or higher
- Build essentials

## Installation

### Windows Setup

#### Option 1: Using Visual Studio
1. Download [Visual Studio 2022](https://visualstudio.microsoft.com/vs/community/)
2. Open the solution file `GrowBase.sln` from `/src/` directory
3. Build the solution in desired configuration (Debug/Release)

#### Option 2: Using CMake
1. Download [CMake](https://cmake.org/download/)
2. Run appropriate build script:
   - `build_release.bat` for release build
   - `build_debug.bat` for debug build

> Note: Use debug build for development and crash investigation, release build for production deployment.

### Linux Setup

1. Install required packages:
```bash
sudo apt-get update
sudo apt-get install build-essential cmake
```

2. Build the project:
```bash
# For release build
./build_release.sh

# For debug build
./build_debug.sh
```

3. Move binary to bin directory:
```bash
mv build/src/GrowBase bin/
```

## Configuration
1. Navigate to `/bin/config.txt`
2. Configure the following settings:
   - `address` (default: 0.0.0.0)
   - `basePort` (your desired port number)

## Support
- Join our [Discord Community](https://discord.gg/2ynZ4cT6G6) for support and discussions
- Create an issue on GitHub for bug reports and feature requests

## Credits
Created by [@xhexago](https://discord.gg/) (Discord & Telegram)

> **Important**: Please maintain attribution by keeping credits in the BaseApp.h file!
