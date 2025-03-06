#include <stdio.h>
#include <string.h>
#include "rooms.h"
#include "utils.h"

void customerMenu(char *username)
{
    int choice;
    

    while (1)
    {
        // Get current date and time
        char todayDate[20], currentTime[20];
        getCurrentDate(todayDate);
        getCurrentTime(currentTime);

        printf("\nTime: %s\t\tDate: %s\n", currentTime, todayDate);
        printf("\nCustomer Menu:\n");
        printf("1. View Room Availability\n");
        printf("2. Make a Reservation\n");
        printf("3. Cancel Reservation\n");
        printf("4. View Reservation History\n");
        printf("5. View Booking History\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice)
        {
        case 1:
            showAvailableRooms();
            break;
        case 2:
            reserveRoom(username);
            break;
        case 3:
            cancelReservation(username);
            break;
        case 4:
            showBookingHistory(username);
            break;
        case 5:
            viewCurrentBooking(username);
            break;
        case 6:
            printf("Exiting customer menu...\n");
            return; // Exit to the main menu
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}


int customerLogin(const char *username, const char *password)
{
    FILE *file = fopen("customer.txt", "r");
    if (!file)
    {
        printf("Error: Unable to open staff file.\n");
        return 0;
    }

    char line[256];
    char uname[16], pwd[20], status[10];
    int loggedIn = 0;

    // Skip the header line
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file))
    {

        // Extract relevant fields based on fixed-width format
        char name[31], dob[16], gender[11], email[31], phone[16], dateReg[21], role[11];
        sscanf(line, "%30[^\n]%15s%19s%15s%10s%30[^\n]%15s%20s%10s%10s",
               name, uname, pwd, dob, gender, email, phone, dateReg, role, status);

        // Trim trailing whitespace or newline from status
        status[strcspn(status, "\n")] = '\0';

        // Check for match
        if (strcmp(username, uname) == 0 && strcmp(password, pwd) == 0 && strcmp(status, "Active") == 0)
        {
            loggedIn = 1;
            break;
        }
    }

    fclose(file);

    if (loggedIn)
    {
        printf("Login successful. Welcome, %s!\n", username);
        return 1;
    }
    else
    {
        printf("Invalid credentials or account is inactive.\n");
        return 0;
    }
}

int customer()
{
    char username[50], password[20];

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    hidden_password(password);

    if (customerLogin(username, password))
    {
        customerMenu(username);
    }

    return 0;
}