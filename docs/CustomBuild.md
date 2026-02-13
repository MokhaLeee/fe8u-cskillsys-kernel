# Custom Build in the Legacy Skill System

**Custom Build** was an FEB feature initially developed by **7743** for the legacy skill system. Its purpose was to allow users to directly modify the functionality of the **skillsys kernel** (e.g., altering how a skill works) without affecting their existing development (e.g., user-configured skill lists).

While the idea was sound, the legacy system's design imposed significant challenges:

1. **Kernel-User Data Coupling**: Core kernel functions (e.g., `skill-tester`, `status-getter`) were stored alongside user-configurable data, requiring decoupling.
2. **Unstable Data Pointers**: The system internally held data pointers, but these were not fixed—especially for player-expandable tables, which could change dynamically.
3. **External Patch Dependencies**: Third-party patches might also call kernel function pointers, complicating modifications.

### 7743's Solution

To address these issues, 7743 implemented the following:

1. **Dedicated FEB Skill System**: A modified version of the skill system was created, splitting functions (`.text` section) and player data (`.rodata` section) into separate components. These were merged into a single patch (`skillsystem_20220703`) for FEB installation.
2. **Customizable Kernel**: The `.text` section was isolated into a special `buildfile`, allowing users to modify and reinstall it via FEB's **Custom Build Form**.
3. **Modular Player Data**: Player data was modularized. FEB identified each module's address and replaced corresponding symbols in the `EA buildfile` during Custom Build.
4. **Pointer Updates for External Patches**: If external patches called kernel functions, their stored pointers were also updated during Custom Build.

This approach was **extremely complex and error-prone**.

### Improvements in Cskillsys

The **Cskillsys** redesign proactively resolved these issues with the following measures:

1. **Memory Partitioning**:
   - Allocates free space (`0x0B2A604–0xD59FC`) exclusively for user-modifiable data.
   - Reserves a dedicated **Pointer List** region (`start: 0xB2A614, size: 0x400`) for kernel-user data interaction.
2. **Pointer List Proxy**:
   - The kernel accesses data exclusively via the Pointer List and **never stores direct pointers** to user-space data.
3. **Two-Step Installation**:
   - **Step 1**: Install user data, saving addresses in the Pointer List.
   - **Step 2**: Install the kernel (`main-kernel.event`), which can be patched independently later.
4. **User Workflow Simplification**:
   - Users perform a **one-time full install** on a clean ROM.
   - Subsequent updates (e.g., skill assignments, chapter edits, animations) can be applied via FEB without affecting existing data.
   - Kernel updates only require re-patching `main-kernel.event`, preserving all imported assets.


# Custom build process

[Contributing note](./docs/CONTRIBUTING.md)

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

# install wine
sudo apt-get -y wine
wget https://mirrors.tuna.tsinghua.edu.cn/winehq/wine/wine-mono/9.4.0/wine-mono-9.4.0-x86.msi
wine msiexec /i wine-mono-9.4.0-x86.msi
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

4. Build EA tools

```bash
# Build EA tools
cp Tools/scripts/build_ea_wo_core.sh Tools/EventAssembler/
cd Tools/EventAssembler
./build_ea_wo_core.sh
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

# linux EA build

If you want not to use wine to call for ColorzCore.exe in linux, you could also directly build it in linux. Refer to [EA build note](https://github.com/StanHash/EventAssembler) to install [.NET](https://learn.microsoft.com/en-us/dotnet/core/install/linux-ubuntu). Here is a recommended installation method:


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

Then build kernel with config:

```bash
make CONFIG_EA_WIN=0
```
