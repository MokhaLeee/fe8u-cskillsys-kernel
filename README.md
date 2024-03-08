# FE-C-Build Kernel

Engine-hacks for FE-Cbuild, including skillsys, battle-system hacks, etc.e.

Main purpose to split kernel form c-build is to make it independent form the wizardry to the real game.

## Custom build

1. Install sub-modules

```bash
cd Kernel/Tools
git clone https://github.com/MokhaLeee/FE-CLib-Mokha.git
git clone https://github.com/StanHash/EventAssembler.git --recursive
git clone https://github.com/StanHash/FE-PyTools.git --recursive
```

2. Install dependence

```bash
sudo apt-get -y install binutils-arm-none-eabi \
    gcc-arm-none-eabi build-essential cmake re2c ghc \
    cabal-install libghc-vector-dev libghc-juicypixels-dev \
    python3-pip pkg-config libpng*

pip install pyelftools PyInstaller tmx six
```

3. Install DevkitPRO

```bash
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
./install-devkitpro-pacman
sudo dkp-pacman -S gba-dev

# Export vars
export DEVKITPRO=/opt/devkitpro
export DEVKITARM=${DEVKITPRO}/devkitARM
export DEVKITPPC=${DEVKITPRO}/devkitPPC
export PATH=${DEVKITPRO}/tools/bin:$PATH
```

3. Build EA

    Get into **Tools/EventAssembler** and then refer to [EA build note](https://github.com/StanHash/EventAssembler).

4. Put **Fire Emblem: The Sacred Stones** clean rom named **fe8.gba** in the **./Kernel**.

4. build:

```bash
make
```

Then it build such outputs:
1. **fe8-kernel-\[VERSION].gba**
2. **fe8-kernel-\[VERSION].sym**: to debug on NO$GBA
3. **fe8-kernel-\[VERSION].ref.s**: to make lyn-reference
4. **fe8-kernel-\[VERSION].ref.event**: to make EA reference

## Build Chinese version

This repo support multi-language. For now, Chinese is supported, if you want to use Chinese version of kernel, follow these steps before `make`:

- Uncomment `include Fonts/fonts.mk` in **./makefile**:
- Uncomment `CONFIG_LANG_CHINESE` in **./Configs/configs.h**
- Uncomment `#include "Fonts/Fonts.event"` in **./main.event**
- Modify on **./Contants/Texts/Source/TextMain.txt**, use `texts-cn.txt` rather than `texts.txt`
