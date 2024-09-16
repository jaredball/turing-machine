## Overview:

This C program simulates a Turing Machine by reading input from a file, initializing a tape represented by a doubly linked list, and processing transitions based on defined rules. The program updates the tape and moves the head according to the Turing Machine's instructions until an end state is reached.

## Features:

- Simulates a basic Turing Machine.
- Reads tape contents, states, and instructions from an input file.
- Dynamically updates the tape based on machine rules and transitions.

## Usage:

1. Prepare an input file (`input.txt`) with the initial tape contents, number of states, start state, end state, and transition rules.
2. Run the program, and it will display both the initial and final tape contents along with the number of states, start state, and end state.
3. Follow the instructions in the code for memory management and state transitions.

## Included Files:

- `main.c`: Main program implementing the Turing Machine logic.
- `input.txt`: Example input file containing tape data and Turing Machine rules.

## Dependencies:

- Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`).

## Note:

Make sure the input file (`input.txt`) is placed in the same directory as the executable, with properly formatted contents, before running the program.
