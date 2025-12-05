# ARM Emulator

A simple ARMv8 emulator written in C++

## Features

- Emulates a subset of ARMv8/AArch64 instruction set
- Interactive REPL for debugging
- Memory and register inspection
- Support for breakpoints
- Step-by-step execution

## Requirements

- C++17 compatible compiler (clang++ or g++)
- CMake 3.10+ (optional, for alternative build)
- Make

## Building

```bash
make
```
This will create an executable named `arm_emulator` in the current directory.

## Usage

Running the emulator:

```bash
./arm_emulator
./arm_emulator program.bin [load_address]
```

### REPL Commands

- `step` or `s` - Execute one instruction
- `run` or `r` - Run until breakpoint or end
- `break <addr>` or `b <addr>` - Set breakpoint at address
- `reg` - Show all registers
- `reg <reg> [= <value>]` - Get/set register value
- `mem <addr> [count]` - Show memory contents
- `help` - Show available commands
- `quit` or `q` - Exit the emulator

## Project Structure

- `include/` - Header files
- `src/` - Source files
- `tests/` - Test programs
