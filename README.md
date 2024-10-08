
# Logic Circuits Simulator

## Introduction
A simulator allows users to create a virtual representation of a digital circuit and observe its behavior under different conditions without physically building the circuit. This project focuses on an event-driven logic circuit simulator, which models the behavior of digital circuits based on events when signals change their logical states.

## Project Description
The purpose of this project is to build an event-driven logic circuit simulator that satisfies the following requirements:

1. The simulator accepts two input files:
   - A **Verilog file (.v)** that describes the circuit using Verilog primitives (and, or, xor, nand, nor, xnor, buf, not).
   - A **stimuli file (.stim)** that provides the events happening on the circuit’s inputs.

2. The stimuli file format:
   ```
   #<delay in ps> <input>=<logic_value>;
   ```
   Example:
   ```
   #0 A=0;
   #0 B=0;
   #0 C=1;
   #500 A=1;
   #800 B=1;
   #1300 C=1;
   ```

3. The output simulation file (.sim) records events with a time stamp and the new value. Example:
   ```
   500, A, 1
   700, w2, 1
   800, B, 1
   900, Y, 1
   ```
