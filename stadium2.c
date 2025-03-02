#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define number_of_rows 10
#define seats_per_row 10
struct Seat {
    int ROW;
    int seatNUM;
    struct Seat* next;
};

// Function to initialize an empty seat queue
//CREATE_SEAT_QUEUE():Initializes an empty seat queue and returns a pointer to the rear of the queue.

struct Seat* CREATE_SEAT_QUEUE() {
    return NULL;
}

// Function to enqueue a seat (book a seat)
//enqueue:Adds a new seat to the queue (books a seat).Uses a circular queue implementation.

struct Seat* enqueue(struct Seat* REAR, int ROW, int seat_num) {
    struct Seat* NEW_seat = (struct Seat*)malloc(sizeof(struct Seat));
    if (NEW_seat == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    NEW_seat->ROW = ROW;
    NEW_seat->seatNUM = seat_num;
    NEW_seat->next = NULL;

    if (REAR == NULL) {
        NEW_seat->next = NEW_seat; // First seat, circular queue
    } else {
        NEW_seat->next = REAR->next;
        REAR->next = NEW_seat;
    }

    return NEW_seat;
}

// Function to dequeue a seat (release a seat)
//dequeue:Removes a seat from the queue (cancels a booked seat).Uses a circular queue implementation.

struct Seat* dequeue(struct Seat* REAR, int* ROW, int* seatNUM) {
    if (REAR == NULL || REAR->next == NULL) {
        fprintf(stderr, "Seat queue is empty.\n");
        exit(1);
    }

    struct Seat* front = REAR->next;
    *ROW = front->ROW;
    *seatNUM = front->seatNUM;

    if (REAR->next == REAR) {
        free(front);
        REAR = NULL; // The last seat is being dequeued
    } else {
        REAR->next = front->next;
        free(front);
    }

    return REAR;
}


// Function to display the seating layout
//displayseating():Displays the current seating layout using 'X' for booked seats and 'O' for unbooked seats.

void displaySeating(bool seats[number_of_rows][seats_per_row]) {
    printf("Seating Layout:\n");
    for (int i = 0; i < number_of_rows; i++) {
        for (int j = 0; j < seats_per_row; j++) {
            printf("%c ", seats[i][j] ? 'X' : 'O');
        }
        printf("\n");
    }
}
//main():Manages the main menu and user interactions.Allows users to:Display the seating layout.Book a seat (if available).
//Cancel a booked seat.Exit the program.


int main() {
    bool seats[number_of_rows][seats_per_row] = {false}; // Initialize all seats as unbooked
    struct Seat* REAR = CREATE_SEAT_QUEUE();

    FILE* inputFile = fopen("input.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Error opening the input file.\n");
        exit(1);
    }

    while (1) {
        int choice, ROW, seatNUM;

        printf("1. Display Seating\n");
        printf("2. Book a Seat\n");
        printf("3. Cancel a Ticket\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        fscanf(inputFile, "%d", &choice);

        switch (choice) {
            case 1:
                displaySeating(seats);
                break;
            case 2:
                printf("Enter the ROW and seat number (e.g., 3 5): ");
                fscanf(inputFile, "%d %d", &ROW, &seatNUM);
                if (ROW >= 1 && ROW <= number_of_rows && seatNUM >= 1 && seatNUM <= seats_per_row && !seats[ROW - 1][seatNUM - 1]) {
                    seats[ROW - 1][seatNUM - 1] = true;
                    REAR = enqueue(REAR, ROW, seatNUM);
                    printf("Seat booked successfully!\n");
                } else {
                    printf("Invalid seat selection or seat is already booked.\n");
                }
                break;
            case 3:
                if (REAR != NULL) {
                    printf("Enter the ROW and seat number of the ticket to cancel (e.g., 3 5): ");
                    fscanf(inputFile, "%d %d", &ROW, &seatNUM);
                    struct Seat* current = REAR->next;
                    bool found = false;
                    do {
                        if (current->ROW == ROW && current->seatNUM == seatNUM) {
                            found = true;
                            break;
                        }
                        current = current->next;
                    } while (current != REAR->next);

                    if (found) {
                        REAR = dequeue(REAR, &ROW, &seatNUM);
                        seats[ROW - 1][seatNUM - 1] = false;
                        printf("Ticket canceled successfully!\n");
                    } else {
                        printf("Ticket not found. Please check the ROW and seat number.\n");
                    }
                } else {
                    printf("No booked seats to cancel.\n");
                }
                break;
            case 4:
                fclose(inputFile);
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}