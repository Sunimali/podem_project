# PODEM Project

## Overview
This project implements the **Path-Oriented Decision Making (PODEM)** algorithm for Automatic Test Pattern Generation (ATPG). The algorithm generates test patterns for detecting faults in combinational circuits, particularly stuck-at faults. This repository includes source code for the PODEM algorithm, benchmark circuits, and test results.

## What is PODEM?
PODEM is an efficient ATPG algorithm that focuses on determining input patterns to propagate a fault to a circuit's output. It works by systematically exploring possible input assignments using decision trees and backtracking. PODEM is more effective than older methods like the D-Algorithm because it avoids unnecessary computations by focusing only on specific paths within the circuit.

### Key Features of PODEM:
- **Fault Detection**: Detects single stuck-at faults in digital circuits.
- **Systematic Search**: Uses a decision-making approach with backtracking to explore input patterns.
- **Reduced Complexity**: Optimizes test pattern generation by focusing on paths that can propagate faults to observable outputs.

### Application:
PODEM is widely used in the field of **VLSI testing** and **digital design verification** to ensure the correctness of hardware designs by identifying and isolating faults in logic circuits.

## Repository Contents
- **Source Files**: C-based implementation of the PODEM algorithm (`main.c`, `input.c`).
- **Benchmark Circuits**: Includes `.isc`, `.pattern`, `.res`, and `.podem` files for circuits such as `c17`, `c880`, and `c7552`.
- **Makefile**: Automates the building process for the project.

## Usage
1. Clone this repository:
   ```bash
   git clone https://github.com/Sunimali/podem_project.git
   ```
2. Build the project using the provided Makefile:
   ```bash
   make
   ```
3. Run the program by passing the benchmark circuits:
   ```bash
   ./podem <circuit_file> <pattern_file> <result_file>
   ```
