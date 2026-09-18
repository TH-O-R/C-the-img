# Simple image viewer:

C-the-img (see the image) is a simple image viewer that only supports `binary format .ppm` files for now.
![C-the-img demo](assets/c-the-img_demo.png)

## Repo structure:

```sh
C-the-img/
├── assets
│   ├── abstarct.ppm
│   ├── android.ppm
│   ├── Clang.ppm
│   └── c-the-img_demo.png
├── LICENSE
├── Makefile
├── README.md
└── src
    ├── main.c
    ├── utils.c
    └── utils.h
```

## Dependencies:

- SDL2 development libraries (`-lSDL2`)
  - Debian/Ubuntu: `sudo apt install libsdl2-dev`
  - Fedora: `sudo dnf install SDL2-devel`
  - Arch: `sudo pacman -S sdl2`
  - macOS: `brew install sdl2`

## Building/Running:

- `make` compile the program
- `make run` run the example
- `make clean` delete the program
