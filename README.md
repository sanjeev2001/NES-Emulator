# NES-Emulator

  An NES Emulator that can run NROM (Mapper 0) titles such as Super Mario Bros and the original Donkey Kong.

  ## Dependencies
  - **GLFW** - Window management and input handling
  - **OpenGL** - Graphics rendering

  ### macOS

  Install GLFW via Homebrew:
  ```bash
  brew install glfw
  ```

  ### Linux
  
  ```bash
  sudo apt-get install libglfw3-dev libgl1-mesa-dev
  ```

  ### Windows

  Using MSYS2/MinGW:
  ```bash
  pacman -S mingw-w64-x86_64-glfw
  ```

  ## Compilation

  The project uses a Makefile that automatically detects your operating system.

  ### Build

  ```bash
  make
  ```

  This compiles the emulator and outputs the executable to the build/ directory.

  ### Clean

  ```bash
  make clean
  ```

  Removes all compiled object files and the executable.

  ### Run

  On MacOS/Linux:
  ```bash
  ./build/nes_emulator <path-to-rom.nes>
  ```

  On Windows:
  ```bash
  ./build/nes_emulator.exe <path-to-rom.nes>
  ```

## Gameplay
Super Mario Bros                                                                                            |  Donkey Kong
:----------------------------------------------------------------------------------------------------------:|:-----------------------------------------------------------------------------------------------------------:
![image](https://i.imgur.com/3AixaGD.png)  |  ![image](https://i.imgur.com/hc81sH2.png)
  
## Roadmap

- Add support for mappers 1-5
- Port to web application using WebAssembly
