#include <stdio.h>
#include <string.h>
#include "rooms.h"
#include "utils.h"

void manageRoomReservation();

void staffMenu()
{
    int choice;
    char username[50];

    while (1)
    {
        // Get current date and time
        char todayDate[20], currentTime[20];
        getCurrentDate(todayDate);
        getCurrentTime(currentTime);

        printf("\nTime: %s\t\tDate: %s\n", currentTime, todayDate);
        printf("\nStaff Menu:\n");
        printf("1. Manage Room Reservation\n");
        printf("2. Check-In\n");
        printf("3. Check-Out\n");
        printf("4. View Room Availability\n");
        printf("5. View Booking History\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice)
        {
        case 1:
            manageRoomReservation();
            break;
        case 2:
            printf("\nEnter the username for the check-in: ");
            scanf("%s", username);
            checkIn(username);
            break;
        case 3:
            printf("\nEnter the username for the check-out: ");
            scanf("%s", username);
            checkOut(username);
            break;
        case 4:
            showAvailableRooms();
            break;
        case 5:
            printf("\nEnter the username to show booking history: ");
            scanf("%s", username);
            viewCurrentBooking(username);
            break;
        case 6:
            printf("Logging out...\n");
            return;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void manageRoomReservation()
{
    // Implementation of manageRoomReservation
    int choice;
    char username[50];

    printf("Manage Room Reservation Menu:\n");
    printf("1. Reserve Room\n");
    printf("2. Cancel Reservation\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume newline character

    switch (choice)
    {
    case 1:
        printf("\nEnter the username for the reservation: ");
        scanf("%s", username);
        reserveRoom(username);
        break;
    case 2:
        printf("\nEnter the username for the reservation to cancel: ");
        scanf("%s", username);
        cancelReservation(username);
        break;
    case 3:
        printf("Exiting manage room reservation menu...\n");
        return; // Exit to the main menu
    default:
        printf("Invalid choice. Please try again.\n");
    }
}

int staffLogin(const char *username, const char *password)
{
    FILE *file = fopen("staff.txt", "r");
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

int staff()
{
    char username[50], password[20];

    printf("\n--- Staff Login ---\n");

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    hidden_password(password);

    if (staffLogin(username, password))
    {
        staffMenu();
    }

    return 0;
}