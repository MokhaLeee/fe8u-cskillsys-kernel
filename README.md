# FE8U C-SkillSys

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com)

Modern CHAX for FE8U-SkillSystem.

- [**User manual**](./docs/Abstract.md)

- [Community discussion on FEUniverse](https://feuniverse.us/t/fe8-modern-c-skillsystem-release/24614)
- [Contributing note](./docs/CONTRIBUTING.md)

**For more notes, see [docs dir](./docs/)**

## Custom build

> [!NOTE]
> You need a linux envirment!
>
> For Windows users, it is recommended to use a [Ubuntu Server](https://ubuntu.com/aws) or try [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

1. Install sub-modules

```bash
cd Tools
git clone https://github.com/MokhaLeee/FE-CLib-Mokha.git
git clone https://github.com/MokhaLeee/EventAssembler.git -b mokha-fix
git clone https://github.com/StanHash/FE-PyTools.git --recursive
git clone https://github.com/MokhaLeee/check_patch.git
```

2. Install dependencies

```bash
sudo apt-get -y install binutils-arm-none-eabi ctags \
	gcc-arm-none-eabi build-essential cmake re2c ghc \
	cabal-install libghc-vector-dev libghc-juicypixels-dev \
	python3-pip pkg-config libpng* moreutils perl

pip install pyelftools PyInstaller tmx six Pillow

cabal update
```

3. Install DevkitPRO

```bash
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
sudo ./install-devkitpro-pacman
sudo dkp-pacman -S gba-dev

# Export vars
echo "export DEVKITPRO=/opt/devkitpro" >> ~/.bashrc
echo "export DEVKITARM=\${DEVKITPRO}/devkitARM" >> ~/.bashrc
echo "export DEVKITPPC=\${DEVKITPRO}/devkitPPC" >> ~/.bashrc
echo "export PATH=\${DEVKITPRO}/tools/bin:\$PATH" >> ~/.bashrc
source ~/.bashrc
```

4. Build EA

Get into **Tools/EventAssembler** and then refer to [EA build note](https://github.com/StanHash/EventAssembler) to install [.NET](https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu).

Recommended installation process
```bash
# Install .NET
# refer to: https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual#scripted-install
wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
chmod +x ./dotnet-install.sh
sudo ./dotnet-install.sh --channel 6.0

export DOTNET_ROOT=$HOME/.dotnet
export PATH=$PATH:$DOTNET_ROOT:$DOTNET_ROOT/tools

# Build EA
cd Tools/EventAssembler
./build.sh
```

5. Install code review tools

```bash
cp Tools/scripts/pre-commit .git/hooks/
```

6. Put **Fire Emblem: The Sacred Stones** clean rom named **fe8.gba** in the repo directory.

7. build:

```bash
make
```

> [!NOTE]
> If gcc report error, update C-Lib and retry, see [#155](https://github.com/MokhaLeee/fe8u-cskillsys-kernel/discussions/115)

It build such outputs

| Name      | Desc 			|
| :--------	| :-----------	|
|fe8-kernel-dev.gba|ROM|
|fe8-kernel-dev.sym|debug on NO$GBA|
|fe8-kernel-dev.ref.s|lyn reference|
|fe8-kernel-dev.ref.event|EA reference|
