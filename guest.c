#include <stdio.h>
#include "rooms.h"

void guest() {
    int choice;

    while (1) {
        printf("\n--- Guest Menu ---\n");
        printf("1. View Room Availability\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline
        switch (choice) {
            case 1:
                showAvailableRooms();
                break;
            case 2:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}