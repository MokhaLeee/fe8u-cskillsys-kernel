# FE-C-Build Kernel

Modern CHAX for FE8U-SkillSystem, including skillsys, battle-system hacks, etc.e.

Main purpose to split kernel form c-build is to make it independent form the wizardry to the real game.

## Custom build

> <!> You need a linux envirment!
>
> For Windows user, it is recommended to use a ubuntu server or try [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

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

    Get into **Tools/EventAssembler** and then refer to [EA build note](https://github.com/StanHash/EventAssembler) to install [.NET](https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu).

```bash
# Install .NET
# refer to: https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual#scripted-install
wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
chmod +x ./dotnet-install.sh
./dotnet-install.sh --channel 6.0

export DOTNET_ROOT=$HOME/.dotnet
export PATH=$PATH:$DOTNET_ROOT:$DOTNET_ROOT/tools
```

```bash
# Build EA
cd Tools/EventAssembler
./build.sh
```

5. Put **Fire Emblem: The Sacred Stones** clean rom named **fe8.gba** in the repo directory.

4. build:

```bash
make
```

It build such outputs

| Name      | Desc 			|
| :--------	| :-----------	|
|fe8-kernel-beta.gba|ROM|
|fe8-kernel-beta.sym|debug on NO$GBA|
|fe8-kernel-beta.ref.s|lyn reference|
|fe8-kernel-beta.ref.event|EA reference|

**For more notes, see [Doc dir](./Doc/)**
