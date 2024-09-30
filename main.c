#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------- Turing Machine Tape Cell ----------
struct Cell {
    char value;             // The character value in the cell
    struct Cell* prev;       // Pointer to the previous cell
    struct Cell* next;       // Pointer to the next cell
};

// ---------- Instruction Struct ----------
struct Instruction {
    char write_value;        // Write
    char move_direction;     // L for Left, R for Right
    int new_state;           // New State
};

// ---------- Declare Tape ----------
struct Cell* tape = NULL;

// ---------- Create Cell ----------
struct Cell* createCell(char value) {
    // Allocate Memory
    struct Cell* newCell = (struct Cell*)malloc(sizeof(struct Cell));
    // Error check
    if (newCell == NULL) {
        printf("No space for new cell");
        exit(1);
    }

    // Initialize Cell with the provided value and set prev/next pointers to NULL
    newCell->value = value;
    newCell->prev = NULL;
    newCell->next = NULL;

    return newCell;
}

// ---------- Insert End ----------
void insertEnd(struct Cell** head, char value) {

    // Create a new cell
    struct Cell* newCell = createCell(value);

    // If head is null (empty tape), new cell becomes first cell (head points to newCell)
    if (*head == NULL) {
        *head = newCell;

    } else {
        // If tape is not empty, traverse to end until next is null
        struct Cell* current = *head;
        while (current->next != NULL) {
            // Advances current pointer to next
            current = current->next;
        }
        // Insert the new cell at the end
        current->next = newCell;    // next pointer now points to new cell
        newCell->prev = current;    // prev pointer now points to last cell
    }
}

// ---------- Print Function ----------
void print_tape() {
    // Current now points to start of tape
    struct Cell* current = tape;

    // While current exists
    while (current != NULL) {
        // Access value field of current cell (character)
        printf("%c", current->value);
        // Advances current pointer to next cell
        current = current->next;
    }

    printf("\n");
}

// ---------- Main Function ----------
int main() {

    // Required variable for input file
    char filename[] = "input.txt";

    // Open the input file in read mode
    FILE* file = fopen(filename, "r");

    // Print error
    if (file == NULL) {
        printf("File not found");
        return 1;
    }

    // Reading file
    char firstline;
    // Read line by character until new line or end of file
    while ((firstline = fgetc(file)) != '\n' && firstline != -1) {
        // Call to insert, using tape (pointer to head of Linked List) and firstline (character read)
        insertEnd(&tape, firstline);
    }

    // Display the initial tape contents
    printf("Initial Tape contents: ");
    print_tape();

    // Required variables
    int states, start_state;
    int end_state;
    char info_lines[100]; // String for reading state information lines
    char instr_lines[100]; // String for reading instructions

    // Reads number of states and stores in info_lines
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &states);
        printf("Number of states: %d\n", states);
    }

    // Reads start state and stores in info_lines
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &start_state);
        printf("Start state: %d\n", start_state);
    }

    // Reads end state and stores in info_lines
    if (fgets(info_lines, sizeof(info_lines), file) != NULL) {
        sscanf(info_lines, "%d", &end_state);
        printf("End state: %d\n", end_state);
    }

    // ---------- Instruction Array ----------

    // Rows are machine states, columns are ASCII characters
    struct Instruction instructions[states][128];

    // Set default instruction values (blank write, no move, invalid state)
    for (int i = 0; i < states; i++) {
        for (int j = 0; j < 128; j++) {
            instructions[i][j].write_value = ' ';
            instructions[i][j].move_direction = ' ';
            instructions[i][j].new_state = -1;
        }
    }

    // Required temporary variables to store instruction information
    int current_state, new_st;
    char read_val, write_val, move_dir;

    // Read and parse each instruction line from the file (one at a time)
    while (fgets(instr_lines, sizeof(instr_lines), file)) {
        sscanf(instr_lines, "%d %c %c %c %d", &current_state, &read_val, &write_val, &move_dir, &new_st);

        // Populate the instructions with the value read
        // When state A, Read B: Write C. Move L or R. Go to State D.
        instructions[current_state][read_val].write_value = write_val;
        instructions[current_state][read_val].move_direction = move_dir;
        instructions[current_state][read_val].new_state = new_st;
    }

    // Close file after reading
    fclose(file);

    // ---------- Execute Turing Machine ----------

    // Start at state 0
    current_state = 0;
    // Set current head to point to head of tape (Start on left)
    struct Cell* current_head = tape;

    // Run until end state reached
    while (current_state != end_state) {
        // Read the current value (A or 1 or B) and store in read_values
        char read_values = current_head->value;

        // Fetch the proper instruction based on the current state and read value
        struct Instruction current_instruction = instructions[current_state][read_values];

        // Updates tape with new value from instructions fetched
        current_head->value = current_instruction.write_value;

        // ---------- Traversal Left & Right ----------

        // Move tape head left or right based on instructions
        if (current_instruction.move_direction == 'L') {
            if (current_head->prev == NULL) {
                // Create a new blank cell to the left if needed
                struct Cell* new_Cell = (struct Cell*)malloc(sizeof(struct Cell));
                if (new_Cell == NULL) {
                    printf("No space for new cell on the left");
                    return 1;
                }
                // Initialize new cell
                new_Cell->value = 'B';
                new_Cell->prev = NULL;
                new_Cell->next = current_head;
                // Update the prev to the new cell
                current_head->prev = new_Cell;
                // Update the tape head to the new cell
                tape = new_Cell;
            } else {
                // Move the tape head to the previous cell
                current_head = current_head->prev;
            }
        } else if (current_instruction.move_direction == 'R') {
            if (current_head->next == NULL) {
                // Create a new blank cell to the right if needed
                struct Cell* new_Cell = (struct Cell*)malloc(sizeof(struct Cell));
                if (new_Cell == NULL) {
                    printf("No space for new cell on the right");
                    return 1;
                }
                // Initialize new cell
                new_Cell->value = 'B';
                // Update the prev to the current (last) cell
                new_Cell->prev = current_head;
                new_Cell->next = NULL;
                current_head->next = new_Cell;
            } else {
                // If there was a next cell, move the tape head there
                current_head = current_head->next;
            }
        } else {
            // If move isn't L or R, exit program and print error
            printf("Invalid move direction: %c", current_instruction.move_direction);
            return 1;
        }

        // Retrieves state associated with instruction and updates current state
        current_state = current_instruction.new_state;
    }

    printf("Final tape contents:");
    print_tape();

    // ---------- Free Memory ----------

    // Initialize pointer current to head
    struct Cell* current = tape;
    while (current != NULL) {
        // Temp pointer stores address of current cell
        struct Cell* temp = current;
        // Advances current pointer to next cell
        current = current->next;
        // Deallocate memory occupied by cell pointed to
        free(temp);
    }

    return 0;
}
