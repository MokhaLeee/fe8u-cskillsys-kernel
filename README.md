# FE8U C-SkillSystem

[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](https://makeapullrequest.com) 

[Contributing note](./docs/CONTRIBUTING.md)

[Community discussion on FEUniverse](https://feuniverse.us/t/fe8-modern-c-skillsystem-release/24614)

A modern buildfile for a C-SkillSystem based on the decompilation of FE8U.

## Main features

1. More diverse functionality
    - [600+ skills](https://docs.google.com/spreadsheets/d/1zd99uevX953OZQiA_oJU9R3hqFao5mrW78wMHKPco-U/edit?pli=1&gid=681794002#gid=681794002) have been introduced and the bit width of the skill index has been expanded to 16 bits. It can theoretically support up to 0xFFFF skills working at the same time.
    - Each unit can carry up to 23 skills, 7 of which can be configured through the prepscreen freely.
    - Introduced a lot of new combat mechanics:
        - The Three Houses style hit rate which decreases the greater the range between you and your enemy
        - An Engage style combo-attack
        - The combat calculation formula has been restructured to allow for more precise manipulation of damage increases and reductions. This has allowed for a large number of FE-Heroes skills to be ported based on that.
        - Reworked the ARM movement calculation function to enable more Heroes style mobility skills such as [Flier Formation](https://feheroes.fandom.com/wiki/Flier_Formation). This also avoids serious performance losses caused by the Pass skill.

2. Animation and UI effects
    - A reworked better interface on statscreen with a configurable FE6 style scrollable background.
    - A variety of cool map animation effects for skills.
    - Features can now be introduced via a custom tutorial configured in the debug map
    - Reworked efxskill and map skill animation effects.

3. Friendly for FEBuilder users
    - Proactively provide FEB patches. For non-developer users, you can just download the **Patches** zipped directory in the releases section of the repo and place it in ***\<path-to-FEBuilder\>\config\patch2\FE8U\\***. Then you can modify C-SkillSystem data in FEBuilder.

4. Better performance
    - Now the kernel runs within the range limit of the BL ARM instruction. The allows for faster calling of the SkillTester check instead of using the slower BLH.
    - Frequently called functions are placed in IWRAM and called as ARM functions, including the aformentioned `SkillTester`, map-task routine, some other judgements, etc.

5. Higher code quality and stability
    - Modifications on vanilla functions operate under the following principles: Unless specific circumstances are presented (such as bugs or glitches), hacked functions should work consistently with their vanilla counterparts.
    - Rigorous PR review based on decomp code.
    - Rigorous code style review.

6. Better development management
    - All components are generated at once via a `make` command.
    - Introduction of [debug kit](https://github.com/MokhaLeee/fe8u-cskillsys-kernel/blob/main/include/debug-kit.h) port from [FEHRR](https://github.com/laqieer/FEHRR) enables developers to use non-std IO functions (such as `Printf`) to debug their modifications in No$GBA.
    - More reasonable RAM space control. Now RAM space registration is designed very similarly to a `malloc` function. Developers can just tell the kernel the size. The kernel will automatically keep track of the physical address and overflow detection.

## Ported Hacks/Features

<details>
    <summary>Arms Scroll - By Jester</summary>
    <video src="https://github.com/user-attachments/assets/9fdaa295-4c35-4c39-b3d9-a356616a1ec2"></video>
</details>
<details>
    <summary>Skill Stealer - By Jester</summary>
    <video src="https://github.com/user-attachments/assets/0013e3ac-c320-4689-a404-7d6b39332373"></video>
</details>
<details>
    <summary>Lazberian Turn Flow - By Stan</summary>
    <video src="https://github.com/user-attachments/assets/c0ef4c4c-5f08-46eb-b75d-9cef135f2d89"></video>
</details>
<details>
    <summary>Support Post Battle - By Vesly</summary>
    <video src="https://github.com/user-attachments/assets/66f8d2e0-8cc0-4297-8857-6a1d75712fbf"></video>
</details>
<details>
    <summary>Heart Seals - By Vesly</summary>
    <video src="https://github.com/user-attachments/assets/141ce2ad-c37d-4db3-850c-8d83ab405472"></video>
</details>
<details>
    <summary>Allegiance Stat Screen Colors - By Contro</summary>
    <video src="https://github.com/user-attachments/assets/dd92fa8c-632c-409b-ae02-ea964376ab9c"></video>
</details>
<details>
    <summary>Show Terrain Bonuses in Stat Screen - By Jester</summary>
    <video src="https://github.com/user-attachments/assets/f6c42aaa-83e2-4f66-a8fd-20b3e74037aa"></video>
</details>
<details>
    <summary>In-game Debugger - By Vesly</summary>
    <video src="https://github.com/user-attachments/assets/ebf7718d-b832-4574-999a-94d6331fdbde"></video>
</details>
<details>
    <summary>Instant Level Up - By Tequila</summary>
    <video src=""https://github.com/user-attachments/assets/f8017d33-574a-4842-92b2-232df941c067></video>
</details>



## Custom build

> [!NOTE]
> You need a linux envirment!
>
> For Windows users, it is recommended to use a [Ubuntu Server](https://ubuntu.com/aws) or try [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

1. Install dependencies

```bash
sudo apt-get -y install binutils-arm-none-eabi ctags \
    gcc-arm-none-eabi build-essential cmake re2c ghc \
    cabal-install libghc-vector-dev libghc-juicypixels-dev \
    python3-pip pkg-config libpng* bsdiff moreutils

pip install pyelftools PyInstaller tmx six
```

2. Install DevkitPRO

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

3. Build EA

> [!WARNING]
> Please **Strictly** follow these steps to build linux EA for yourself.

Get into **Tools/EventAssembler** and then refer to [EA build note](https://github.com/StanHash/EventAssembler) to install [.NET](https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu).

```bash
# Install .NET
# refer to: https://learn.microsoft.com/en-us/dotnet/core/install/linux-scripted-manual#scripted-install
wget https://dot.net/v1/dotnet-install.sh -O dotnet-install.sh
chmod +x ./dotnet-install.sh
sudo ./dotnet-install.sh --channel 6.0

export DOTNET_ROOT=$HOME/.dotnet
export PATH=$PATH:$DOTNET_ROOT:$DOTNET_ROOT/tools
```

```bash
# Build EA
cd Tools/EventAssembler
./build.sh
```

4. Put a clean copy of the US version of **Fire Emblem: The Sacred Stones** in the root of the repo and call it **fe8.gba**

5. To build, run the following:

```bash
make
```

The following files are built and output to the root of the repo

| Name      | Desc 			|
| :--------	| :-----------	|
|fe8-kernel-dev.gba|ROM|
|fe8-kernel-dev.sym|debug on NO$GBA|
|fe8-kernel-dev.ref.s|lyn reference|
|fe8-kernel-dev.ref.event|EA reference|

**For more notes, see [docs dir](./docs/)**
