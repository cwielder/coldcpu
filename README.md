<p align="center">
    <img width="600px" src="https://github.com/user-attachments/assets/1cd0a6de-ea33-47f1-b8b4-9931e9c42260">
</p>
<div align="center">
    <a href="https://github.com/cwielder/coldcpu/wiki">
        <img src="https://img.shields.io/badge/docs-latest-blue.svg?style=flat">
    </a>
    <img src="https://sloc.xyz/github/cwielder/coldcpu">
    <a href="https://github.com/cwielder/coldcpu/blob/main/LICENSE.txt">
        <img src="https://img.shields.io/github/license/cwielder/coldcpu?style=flat">
    </a>
</div>

# ❄️ Overview
**ColdCPU** is a custom-made fictional processor architecture and assembly language written in C++. It offers a comprehensive toolchain for compiling, cross-platform emulation, and disassembly.

# ✔️ Features
* Turing Completeness: Supports arithmetic, floating-point and bitwise operations.
* Control Flow: Conditional branching for complex program logic.
* Memory Operations: Secure memory model with fast load/store instructions.
* Clean & Simple: ColdCPU provides an educational reference designed to explore low-level architecture concepts and enhance understanding of basic processor functionality.
* Examples: Several examples of programs written in the `cold` assembly language have been provided in the [workdir](https://github.com/cwielder/coldcpu/tree/main/workdir) folder of this repository.

# 📚 Usage
## Assembler
```
Usage: coldasm --input PATH --output PATH
```

## Emulator
```
Usage: coldemu --path PATH [--memory VAR]

Optional arguments:
  -m, --memory   memory size in bytes [default: 1024]
```

## Disassembler
```
Usage: colddsm --input PATH --output PATH
```

### See the documentation for more detailed information about the processor and toolchain in the [wiki](https://github.com/cwielder/coldcpu/wiki).

# 🔨 Building
This project is built upon the [Premake5](https://premake.github.io/) metabuild system, meaning that it can compile using native build systems depending on the host. Currently, Windows and Linux are supported, and build systems for Visual Studio and GNU Make can be generated for each respectively by running the corresponding setup script found in the `scripts` directory at the root of the repository.

## Steps
* Clone the repository:
```
git clone https://github.com/cwielder/coldcpu.git
cd coldcpu
```
* Generate build files:
  * Windows (Visual Studio):
    ```
    "./scripts/setupVS.bat"
    ```
    Open the resulting `coldcpu.sln` file in Visual Studio 2022+
  * Linux (GNU Make):
    ```
    "./scripts/setupMake"
    make
    ```
# 📃 License
All code in the `coldcpu` repository has been made available under the [MIT License](https://github.com/cwielder/coldcpu/blob/main/LICENSE.txt).
