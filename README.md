# Custom Assembler (ANSI C)

An **assembler** written in pure **ANSI C**.  
Translates a made-up assembly language into binary machine code (3-byte word) for a hypothetical CPU.  
This project was built from scratch as a final course project for university.

---

## Features
- Written in pure ANSI C (portable and dependency-free).
- Converts custom assembly instructions into binary.
- Modular design (`.c` sources separated from `.h` headers).
- Build automation with `make`.

---

##  Getting Started

### 1. Clone the repo
bash
git clone https://github.com/TheIdoLevy/Assembler
cd your-assembler

### 2. Build with make
make

### 3. Run the assembler
For any number of input files, run:

./assembler inputFile-1-.as inputFile-2-.as ... inputFile-n-.as

where inputFile-n-.as is the final assembly file.

##  Assembler's output:
Important node: This assembler only assembles the code, without the linking and loading processes.

Given valid input files, the assembler will output 4 files per input file;
1) a .am file, containig the spread  macros.
2) a .obj file containing the assemble binary code of the file (formatted to binary).
3) a .ent file, containing all labels declared as .entry in the file
4) a .ext file, containig all labels declared as external in the file

Instructions and data are seperated in the .obj file, with insructions being listed on the left hand side of the file and data on the right hand side.

If a file is invalid, the assembler will output all errors in that file to the terminal.
