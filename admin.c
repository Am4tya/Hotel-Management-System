#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "user.h"
#include "rooms.h"

void updateRoomDetails();

// Administrator Menu
void adminMenu()
{
    int choice;

    while (1)
    {
        // Get current date and time
        char todayDate[20], currentTime[20];
        getCurrentDate(todayDate);
        getCurrentTime(currentTime);

        printf("\nTime: %s\t\tDate: %s\n", currentTime, todayDate);
        printf("\n--- Administrator Menu ---\n");
        printf("1. Register new user\n");
        printf("2. Update user status\n");
        printf("3. Manage hotel rooms\n");
        printf("4. Logout\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice)
        {
        case 1:
            registerUser(); // Call function to register a new user
            break;
        case 2:
            updateUserStatus(); // Call function to manage existing users
            break;
        case 3:
            printf("\n--- Manage Hotel Rooms ---\n");
            int roomChoice;
            printf("1. Add New Room\n");
            printf("2. Update Room Details\n");
            printf("3. Show All Available Rooms\n");
            printf("4. Remove Room\n");
            printf("Enter your choice (1-4): ");
            scanf("%d", &roomChoice);
            getchar(); // Consume newline

            switch (roomChoice)
            {
            case 1:
                addRoom(); // Function to add a new room
                break;
            case 2:
                updateRoomDetails(); // Function to update room details
                break;
            case 3:
                showAvailableRooms(); // Function to show available rooms
                break;
            case 4:
                removeRoom(); // Function to remove a room
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
            }
            break;
        case 4:
            printf("Logging out of the Administrator menu...\n");
            return; // Logout and return to the main menu
        default:
            printf("Invalid choice! Please enter a valid option.\n");
        }
    }
}

// Admin login function
void adminLogin()
{
    printf("\n--- Admin Login ---\n");
    char username[50], password[50];

    printf("Enter admin username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0; // Remove trailing newline

    printf("Enter admin password: ");
    hidden_password(password); // Password input with hidden characters

    // For simplicity, assume fixed admin credentials
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0)
    {
        printf("\nLogin successful!\n");
        adminMenu(); // Only show the admin menu if login is successful
    }
    else
    {
        printf("\nInvalid username or password. Access denied.\n");
    }
}

void updateRoomDetails()
{
    int choice;
    while (1)
    {
        printf("\n--- Update Room Details ---\n");
        printf("1. Add Room\n");
        printf("2. Update Room Type\n");
        printf("3. Update Room Status\n");
        printf("4. Update Room Capacity\n");
        printf("5. Back\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);
        getchar(); // Consume newline character
        switch (choice)
        {
        case 1:
            addRoom();
            break;
        case 2:
            updateRoomType();
            break;
        case 3:
            updateRoomStatus();
            break;
        case 4:
            updateRoomCapacity();
            break;
        case 5:
            printf("Returning to the main menu...\n");
            return;
        default:
            printf("Invalid choice! Please enter a valid option.\n");
        }
    }
}