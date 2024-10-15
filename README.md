# FE8U C-SkillSys

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com) 

[Contributing note](./docs/CONTRIBUTING.md)

[Community discussion on FEUniverse](https://feuniverse.us/t/fe8-modern-c-skillsystem-release/24614)

Modern CHAX for FE8U-SkillSystem.

## Main features

1. More diverse functionality features
    - 450+ skills have been introduced and with the bit width of skill index has been expanded to 16 bits, it can support up to 0xFFFF skills working at the same time in theory.
    -  Each unit can carry up to 23 skills, 7 of which can be configured through the prepscreen freely.
    - Introduced a lot of new combat mechanics, such as ThreeHouses style hit rate decreases with battle range increses, Engage style combo-attack, etc. Combat calculation formula has been restructured so that we can get more reasonable calculation on damage increase and reduction, and thus a large number of FE-Heros skills were introduced based on that.
    - Reworked movement calculation ARM function enables more Heros style mobility skills introduced such as [Flier Formation](https://feheroes.fandom.com/wiki/Flier_Formation), etc. This also avoids serious performance losses caused by the introduction of Pass skill.

2. Anim and UI effects
    - Reworked better interface on statscreen with FE6 style scrollable statscreenfx.
    - A variety of cool map anim effects for skills.
    - Introduce new features via kernel tutorial info during game-playing.
    - Reworked efxskill and map skill anim effects.

3. Friendly for FEBuilder users
    - Proactively provide FEB patches. For non-developer users, you may just download **Patches** zipped directory in release and put it to ***\<path-to-FEBuilder\>\config\patch2\FE8U\\***. Then you can modify C-Skillsys data easily.

4. Better performace
    - Now the kernel run on in-BL range space. For the function call, we can just use `BL` instruction ranther than slowly `BLH`.
    - Frequently called functions are placed in IWRAM and called as ARM functions, including `SkillTester`, map-task routine, some other judgements, etc.

5. Higher code quality and stability
    - Modifications on vanilla functions followed the principles: Unless specific circumstances, the hacked functions should worked consistent with original.
    - Rigorous PR review based on decomp code.
    - Rigorous code style review.

6. Better development management
    - All components are all generated at once via a `make` command.
    - Introduction of [debug kit](https://github.com/MokhaLeee/fe8u-cskillsys-kernel/blob/main/include/debug-kit.h) port from [FEHRR](https://github.com/laqieer/FEHRR) enables developers use non-std IO functions (such as `Printf`) to debug on their modification.
    - More reasonable RAM space control. Now RAM space registration is designed very similar to a `malloc` function. Developers may just tell the kernel the size. And kernel may automatically maintain the physical address and overflow detection.

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
    python3-pip pkg-config libpng* bsdiff moreutils perl

pip install pyelftools PyInstaller tmx six
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

> [!WARNING]
> Please **Strictly** follow the following steps to build linux EA by yourself.

Get into **Tools/EventAssembler** and then refer to [EA build note](https://github.com/StanHash/EventAssembler) to install [.NET](https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu).

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

**For more notes, see [docs dir](./docs/)**
