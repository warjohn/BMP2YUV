# BMP to YUV Converter and Video Processor

## Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Requirements](#requirements)
4. [Installation](#installation)
5. [Usage](#usage)
6. [Directory Structure](#directory-structure)
7. [Contributing](#contributing)
8. [License](#license)

---

## Overview

This project is designed to convert BMP images into YUV420P format and process video files by inserting overlays. The core functionality includes:
- Reading BMP files and converting them into YUV420P format.
- Inverting the YUV data vertically.
- Inserting overlays into YUV video streams.
- Saving the processed YUV data into a file.

The code is structured using Object-Oriented Programming (OOP) principles, ensuring modularity, reusability, and maintainability.

---

## Features

- **BMP to YUV Conversion**: Converts 24-bit BMP images into YUV420P format.
- **Parallel Processing**: Utilizes multi-threading for efficient conversion of Y, U, and V channels.
- **Video Overlay**: Inserts a YUV overlay into a video stream.
- **Modular Design**: Divided into reusable classes (`BMPToYUVConverter`, `BMPreader`, `SaveYUV`, `VideoYUV`) for better organization and abstract classes (`ConvertAnyFormat2YUV`, `ImageReader`, `SaveImage`, `VideoProcessor`)

---

## Requirements

To build and run this project, you need the following:

- **Compiler**: A modern C++ compiler supporting C++17 or later (e.g., GCC, Clang, MSVC).
- **Build System**: CMake (version 3.10 or later).
- **Standard Libraries**: STL (for vectors, threads, etc.).
- **Optional**: FFmpeg for testing and validating YUV files.

---

## Installation

### Step 1: Clone the Repository

```bash
git clone https://github.com/your-repo-url.git
cd your-repo-name
```

### Step 2: Create a Build Directory
```bash
mkdir build
cd build
```

### Step 3: Configure with CMake
```bash
cmake ..
```

### Step 4: Build the Project
```bash
make
```

For Windows users, you can use your preferred IDE (e.g., Visual Studio) or run: 
```bash
cmake --build . --config Release
```


## Usage

Convert BMP to YUV : 
- Place your BMP image in the project directory.
- Run the executable:
```bash
./bin/main input.bmp output.yuv
```

## Directory Structure 
```bash
project-root/
│
├── bin/               # Executable files (generated during build)
├── build/             # Intermediate build files (CMake output)
├── include/           # Header files (.h)
├── src/               # Source files (.cpp)
├── CMakeLists.txt     # CMake configuration file
├── README.md          # This file
└── .gitignore         # Git ignore rules
```

## Contributing
Contributions are welcome! If you find any issues or have suggestions for improvement, please open an issue or submit a pull request.


## License
This project is licensed under the MIT License. See the LICENSE  file for details. 
