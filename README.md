# snake-rl
Snake learns to survive using reinforcement learning.

## Installation
You need a GCC (or alternative C99 compiler), make and git (to download raylib repo).
```bash
sudo apt install build-essential git
```
You need to install some required libraries; ALSA for audio, Mesa for OpenGL accelerated graphics and X11 for windowing system.

### Ubuntu
```bash
sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```
### Fedora
```bash
sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel libatomic
```
### Arch Linux
```bash
sudo pacman -S alsa-lib mesa libx11 libxrandr libxi libxcursor libxinerama
```