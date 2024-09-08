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

# Overview
**ColdCPU** is a custom-made fictional processor architecture and assembly language written in C++ with a toolchain for compiling, cross-platform emulation, and disassembly. It features turing-completeness with support for arithmetic, floating-point, bitwise operations, conditional control flow, and memory load/store using a protected memory model.

# Usage
See the documentation for usage of each of the programs provided by the toolchain in the [wiki](https://github.com/cwielder/coldcpu/wiki).

# Building
This project is built upon the Premake5 metabuild system, meaning that it can compile using native build systems depending on the host. Currently, Windows and Linux are supported, and build systems for Visual Studio and GNU Make can be generated for each respectively by running the corresponding setup script found in the `scripts` directory at the root of the repository.
