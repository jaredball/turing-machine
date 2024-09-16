#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cell on the Turing Machine tape
struct Cell {
    char value;             // The character value in the cell
    struct Cell* prev;       // Pointer to the previous cell
    struct Cell* next;       // Pointer to the next cell
};

// Turing Machine instruction
struct Instruction {
    char write_value;        // The value to write in the current cell
    char move_direction;     // The direction to move the tape head ('L' for left, 'R' for right)
    int new_state;           // The new state after applying the instruction
};

// Declare tape globally (doubly linked list)
struct Cell* tape = NULL;

// Function to create a new cell with a given value
struct Cell* createCell(char value) {
    // Allocate memory for a new cell
    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));

    // Check for memory allocation failure
    if (newCell == NULL) {
        printf("No space for new cell");
        exit(1);  // Exit program if allocation fails
    }

    // Initialize the cell with the provided value and set prev/next pointers to NULL
    newCell->value = value;
    newCell->prev = NULL;
    newCell->next = NULL;

    return newCell;
}

// Function to insert a cell with a given value at the end of the tape (linked list)
void insertEnd(struct Cell** head, char value) {
    struct Cell* newCell = createCell(value);  // Create a new cell

    // If the tape is empty, set the new cell as the head
    if (*head == NULL) {
        *head = newCell;
    } else {
        // Traverse to the end of the linked list
        struct Cell* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        // Insert the new cell at the end
        current->next = newCell;
        newCell->prev = current;
    }
}

// Function to print the tape contents (linked list values)
void print_tape() {
    struct Cell* current = tape;

    // Traverse the linked list and print each cell's value
    while (current != NULL) {
        printf("%c", current->value);
        current = current->next;
    }

    // Newline for print out
    printf("\n");
}

int main() {

    // Input file path
    char filename[] = "input.txt";

    // Open the input file for reading
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found");
        return 1;  // Exit if the file can't be opened
    }

    // Read the first line from the file (initial tape contents)
    char firstline;
    while ((firstline = fgetc(file)) != '\n' && firstline != -1) {
        // Insert the characters into the tape
        insertEnd(&tape, firstline);
    }

    // Display the initial tape contents
    printf("Initial Tape contents: ");
    print_tape();

    // Variables to store Turing Machine information (states, start/end states)
    int states, start_state;
    int end_state;
    char info_lines[100]; // Buffer for reading state information lines
    char instr_lines[100]; // Buffer for reading instructions

    // Read the number of states from the second line of the file
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &states);
        printf("Number of states: %d\n", states);
    }

    // Read the start state from the third line of the file
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &start_state);
        printf("Start state: %d\n", start_state);
    }

    // Read the end state from the fourth line of the file
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &end_state);
        printf("End state: %d\n", end_state);
    }

    // Initialize the instructions array for the Turing Machine
    struct Instruction instructionss[states][128];

    // Set default instruction values (blank write, no move, invalid state)
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < 128; j++) {
            instructionss[i][j].write_value = ' ';
            instructionss[i][j].move_direction = ' ';
            instructionss[i][j].new_state = -1;
        }
    }

    // Variables to store instruction information
    int current_state, new_st;
    char read_val, write_val, move_dir;

    // Read and parse each instruction line from the file
    while (fgets(instr_lines, sizeof(instr_lines), file)) {
        sscanf(instr_lines, "%d %c %c %c %d", &current_state, &read_val, &write_val, &move_dir, &new_st);

        // Populate the instructions array with new gained data
        instructionss[current_state][read_val].write_value = write_val;
        instructionss[current_state][read_val].move_direction = move_dir;
        instructionss[current_state][read_val].new_state = new_st;
    }

    // Close the file after reading all instructions
    fclose(file);

    // Initialize the tape head and start processing
    current_state = 0;  // Start at the initial state
    struct Cell* current_head = tape;  // Start at the head of the tape

    // Execute Turing Machine instructions until reaching the end state
    while (current_state != end_state) {
        // Read the current value under the tape head
        char read_values = current_head->value;

        // Fetch the needed instruction based on the current state and read value
        struct Instruction current_instruction = instructionss[current_state][read_values];

        // Update the tape with the new value
        current_head->value = current_instruction.write_value;

        // Move the tape head left or right based on the instruction
        if (current_instruction.move_direction == 'L') {
            if (current_head->prev == NULL) {
                // Create a new blank cell to the left if needed
                struct Cell* new_Cell = (struct Cell*)malloc(sizeof(struct Cell));
                if (new_Cell == NULL) {
                    printf("No space for new cell");
                    return 1;
                }
                new_Cell->value = 'B';  // Set value to blank
                new_Cell->prev = NULL;
                new_Cell->next = current_head;
                current_head->prev = new_Cell;
                tape = new_Cell;  // Update the tape head
            } else {
                // Move the tape head to the previous cell
                current_head = current_head->prev;
            }
        } else if (current_instruction.move_direction == 'R') {
            if (current_head->next == NULL) {
                // Create a new blank cell to the right if needed
                struct Cell* new_Cell = (struct Cell*)malloc(sizeof(struct Cell));
                if (new_Cell == NULL) {
                    printf("No space for new cell");
                    return 1;
                }
                new_Cell->value = 'B';  // Set value to blank
                new_Cell->prev = current_head;
                new_Cell->next = NULL;
                current_head->next = new_Cell;
            } else {
                // Move the tape head to the next cell
                current_head = current_head->next;
            }
        } else {
            // Handle any bad move direction
            printf("Invalid move direction: %c\n", current_instruction.move_direction);
            return 1;
        }

        // Update the current state based on the instruction
        current_state = current_instruction.new_state;
    }

//------------------------------------------------------------------------------------

    //Don't make changes in the following lines
    //To make the test code work, please comment out all the printf statements before pushing to GitHub.
    printf("Final tape contents: ");
    print_tape();

//------------------------------------------------------------------------------------

    // Free the memory allocated for the tape cells
    struct Cell* current = tape;
    while (current != NULL) {
        struct Cell* temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}
