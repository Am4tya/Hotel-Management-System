#include <stdio.h>
#include "utils.h"
#include "admin.h"
#include "staff.h"
#include "customer.h"
#include "guest.h"

// Function prototypes
void mainMenu();


int main() {
    mainMenu();
    return 0;
}

// Main menu function
void mainMenu() {
    int choice;

    while (1) {
        // Get current date and time
        char todayDate[20], currentTime[20];
        getCurrentDate(todayDate);
        getCurrentTime(currentTime);

        printf("\nTime: %s\t\t\tDate: %s\n", currentTime, todayDate);
        printf("\n--- Welcome to the Hotel Management System ---\n");
        printf("1. Administrator\n");
        printf("2. Hotel Staff\n");
        printf("3. Customer\n");
        printf("4. Guest\n");
        printf("5. Exit\n");
        printf("Enter your role (1-5): ");
        scanf("%d", &choice);
        getchar();  // Consume the newline character left by scanf()

        switch (choice) {
            case 1:
                adminLogin();
                break;
            case 2:
                staff();
                break;
            case 3:
                customer();
                break;
            case 4:
                guest();
                break;
            case 5:
                printf("Exiting the program...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}