#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"
#include "user.h"

#define ROOM_FILE "rooms.txt"

struct Room {
    int roomID;
    char roomType[20];
    double roomRate;
    char status[15];  // "Available" or "Unavailable"
    int capacity;
};

// Function to check if the file already has a header
bool fileHasHeader() {
    FILE *file = fopen(ROOM_FILE, "r");
    if (file == NULL) {
        return false;  // File doesn't exist
    }

    char line[100];
    fgets(line, sizeof(line), file);
    fclose(file);

    return (strstr(line, "RoomID") != NULL);
}

// Function to check if a Room ID already exists
bool roomIDExists(int roomID) {
    FILE *file = fopen(ROOM_FILE, "r");
    if (file == NULL) {
        return false;  // File doesn't exist
    }

    char line[100];
    struct Room tempRoom;

    // Skip the header (read first two lines)
    fgets(line, sizeof(line), file);  // Header line
    fgets(line, sizeof(line), file);  // Blank line

    // Read each line and check for the room ID
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d", &tempRoom.roomID) == 1) {
            if (tempRoom.roomID == roomID) {
                fclose(file);
                return true;  // Room ID found
            }
        }
    }

    fclose(file);
    return false;  // Room ID not found
}

// Function to add a room
void addRoom() {
    FILE *file;
    if (!fileHasHeader()) {
        // Create a new file with a header and a blank line after it
        file = fopen(ROOM_FILE, "w");
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }
        fprintf(file, "%-6s  %-10s  %-8s   %-12s  %-8s\n\n",
                "RoomID", "RoomType", "RoomRate", "Status", "Capacity");
    } else {
        // Append to the existing file
        file = fopen(ROOM_FILE, "a");
        if (file == NULL) {
            printf("Error opening file.\n");
            return;
        }
    }

    struct Room newRoom;

    // Get room ID with validation and check for duplicates
    do {
        printf("Enter Room ID: ");
        scanf("%d", &newRoom.roomID);
        if (newRoom.roomID <= 0) {
            printf("Invalid Room ID. Please enter a positive integer.\n");
        } else if (roomIDExists(newRoom.roomID)) {
            printf("Error: Room ID %d already exists. Please enter a different Room ID.\n", newRoom.roomID);
        }
    } while (newRoom.roomID <= 0 || roomIDExists(newRoom.roomID));

    // Room type selection
    printf("Select Room Type:\n");
    printf("1. Deluxe\n");
    printf("2. Standard\n");
    printf("3. Suite\n");
    int roomTypeChoice;
    scanf("%d", &roomTypeChoice);

    switch (roomTypeChoice) {
        case 1:
            strcpy(newRoom.roomType, "Deluxe");
            break;
        case 2:
            strcpy(newRoom.roomType, "Standard");
            break;
        case 3:
            strcpy(newRoom.roomType, "Suite");
            break;
        default:
            printf("Invalid choice. Room not added.\n");
            fclose(file);
            return;
    }

    // Get room rate with validation
    do {
        printf("Enter Room Rate: ");
        scanf("%lf", &newRoom.roomRate);
        if (newRoom.roomRate <= 0) {
            printf("Invalid Room Rate. Please enter a positive value.\n");
        }
    } while (newRoom.roomRate <= 0);

    // Get capacity with validation
    do {
        printf("Enter Capacity: ");
        scanf("%d", &newRoom.capacity);
        if (newRoom.capacity <= 0) {
            printf("Invalid Capacity. Please enter a positive integer.\n");
        }
    } while (newRoom.capacity <= 0);

    strcpy(newRoom.status, "Available");  // New rooms are available by default

    // Write room details to the file in tabular format
    fprintf(file, "%-6d  %-10s  %-8.2f   %-12s  %-8d\n",
            newRoom.roomID, newRoom.roomType, newRoom.roomRate,
            newRoom.status, newRoom.capacity);

    fclose(file);
    printf("Room added successfully.\n");
}

// Function to read all rooms into an array
int readRooms(struct Room rooms[], int maxRooms) {
    FILE *file = fopen(ROOM_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return 0;
    }

    char line[100];
    int count = 0;

    // Skip the header
    fgets(line, sizeof(line), file);
    fgets(line, sizeof(line), file);

    // Read rooms into the array
    while (fgets(line, sizeof(line), file) != NULL && count < maxRooms) {
        sscanf(line, "%d %s %lf %s %d",
               &rooms[count].roomID,
               rooms[count].roomType,
               &rooms[count].roomRate,
               rooms[count].status,
               &rooms[count].capacity);
        count++;
    }

    fclose(file);
    return count;
}

// Function to write all rooms back to the file
void writeRooms(struct Room rooms[], int count) {
    FILE *file = fopen(ROOM_FILE, "w");
    if (file == NULL) {
        printf("Error: Could not open file.\n");
        return;
    }

    // Write header
    fprintf(file, "%-6s  %-10s  %-8s   %-12s  %-8s\n\n",
            "RoomID", "RoomType", "RoomRate", "Status", "Capacity");

    // Write rooms
    for (int i = 0; i < count; i++) {
        fprintf(file, "%-6d  %-10s  %-8.2f   %-12s  %-8d\n",
                rooms[i].roomID,
                rooms[i].roomType,
                rooms[i].roomRate,
                rooms[i].status,
                rooms[i].capacity);
    }

    fclose(file);
}

// Function to find a room by ID
int findRoomIndex(struct Room rooms[], int count, int roomID) {
    for (int i = 0; i < count; i++) {
        if (rooms[i].roomID == roomID) {
            return i;
        }
    }
    return -1;
}

// Function to update room type
void updateRoomType() {
    struct Room rooms[100];
    int roomID, count = readRooms(rooms, 100);

    printf("Enter Room ID to update type: ");
    scanf("%d", &roomID);

    int index = findRoomIndex(rooms, count, roomID);
    if (index == -1) {
        printf("Room ID not found.\n");
        return;
    }

    printf("Select New Room Type:\n");
    printf("1. Deluxe\n2. Standard\n3. Suite\n");
    int choice;
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            strcpy(rooms[index].roomType, "Deluxe");
            break;
        case 2:
            strcpy(rooms[index].roomType, "Standard");
            break;
        case 3:
            strcpy(rooms[index].roomType, "Suite");
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    writeRooms(rooms, count);
    printf("Room type updated successfully.\n");
}

// Function to update room rate
void updateRoomRate() {
    struct Room rooms[100];
    int roomID, count = readRooms(rooms, 100);

    printf("Enter Room ID to update rate: ");
    scanf("%d", &roomID);

    int index = findRoomIndex(rooms, count, roomID);
    if (index == -1) {
        printf("Room ID not found.\n");
        return;
    }

    printf("Enter New Room Rate: ");
    scanf("%lf", &rooms[index].roomRate);

    writeRooms(rooms, count);
    printf("Room rate updated successfully.\n");
}

// Function to update room status
void updateRoomStatus() {
    struct Room rooms[100];
    int roomID, count = readRooms(rooms, 100);

    printf("Enter Room ID to update status: ");
    scanf("%d", &roomID);

    int index = findRoomIndex(rooms, count, roomID);
    if (index == -1) {
        printf("Room ID not found.\n");
        return;
    }

    printf("Select New Status:\n");
    printf("1. Available\n2. Unavailable\n");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        strcpy(rooms[index].status, "Available");
    } else if (choice == 2) {
        strcpy(rooms[index].status, "Unavailable");
    } else {
        printf("Invalid choice.\n");
        return;
    }

    writeRooms(rooms, count);
    printf("Room status updated successfully.\n");
}

// Function to update room capacity
void updateRoomCapacity() {
    struct Room rooms[100];
    int roomID, count = readRooms(rooms, 100);

    printf("Enter Room ID to update capacity: ");
    scanf("%d", &roomID);

    int index = findRoomIndex(rooms, count, roomID);
    if (index == -1) {
        printf("Room ID not found.\n");
        return;
    }

    printf("Enter New Room Capacity: ");
    scanf("%d", &rooms[index].capacity);

    writeRooms(rooms, count);
    printf("Room capacity updated successfully.\n");
}

// Function to remove a room
void removeRoom() {
    struct Room rooms[100];
    int count = readRooms(rooms, 100);

    if (count == 0) {
        printf("No rooms available to remove.\n");
        return;
    }

    int roomID;
    printf("Enter Room ID to remove: ");
    scanf("%d", &roomID);

    int index = findRoomIndex(rooms, count, roomID);
    if (index == -1) {
        printf("Room ID not found.\n");
        return;
    }

    // Remove the room by shifting subsequent rooms left
    for (int i = index; i < count - 1; i++) {
        rooms[i] = rooms[i + 1];
    }
    count--;  // Reduce room count

    writeRooms(rooms, count);
    printf("Room with ID %d removed successfully.\n", roomID);
}

// Function to display all available rooms in tabular format
void showAvailableRooms() {
    struct Room rooms[100];
    int count = readRooms(rooms, 100);

    if (count == 0) {
        printf("No rooms available.\n");
        return;
    }

    // Print table header
    printf("\n%-6s  %-10s  %-8s   %-12s  %-8s\n", 
           "RoomID", "RoomType", "RoomRate", "Status", "Capacity");
    printf("-------------------------------------------------------\n");

    bool hasAvailableRooms = false;

    // Iterate through rooms and display only available rooms
    for (int i = 0; i < count; i++) {
        if (strcmp(rooms[i].status, "Available") == 0) {
            printf("%-6d  %-10s  %-8.2f   %-12s  %-8d\n",
                   rooms[i].roomID,
                   rooms[i].roomType,
                   rooms[i].roomRate,
                   rooms[i].status,
                   rooms[i].capacity);
            hasAvailableRooms = true;
        }
    }

    if (!hasAvailableRooms) {
        printf("No rooms are currently available.\n");
    }
}

// Function to ensure `reservations.txt` has a header
void ensureReservationFileHeader() {
    FILE *file = fopen("reservations.txt", "r");
    if (file == NULL) {
        // Create the file with the header if it doesn't exist
        file = fopen("reservations.txt", "w");
        if (file != NULL) {
            fprintf(file, "%-15s%-10s%-20s%-12s%-12s\n\n",
                    "Username", "RoomID", "ReservationDate", "Check-In", "Check-Out");
            fclose(file);
        }
    } else {
        fclose(file);
    }
}

void reserveRoom(char *username) {
    int roomID;
    struct Room rooms[100];
    int roomCount = readRooms(rooms, 100);

    // Ensure reservations.txt has a header
    ensureReservationFileHeader();

    if (!isUsernameTaken(username, "customer.txt")) {
        printf("Error: Username does not exist.\n");
        return;
    }

    showAvailableRooms();

    // Get room ID and check availability
    printf("\nEnter Room ID to reserve: ");
    scanf("%d", &roomID);

    int roomIndex = findRoomIndex(rooms, roomCount, roomID);
    if (roomIndex == -1) {
        printf("Error: Room ID not found.\n");
        return;
    }

    if (strcmp(rooms[roomIndex].status, "Available") != 0) {
        printf("Error: Room is not available for reservation.\n");
        return;
    }

    // Reserve the room
    strcpy(rooms[roomIndex].status, "Reserved");

    // Save reservation data
    FILE *reservationFile = fopen("reservations.txt", "a");
    if (reservationFile == NULL) {
        printf("Error: Could not open reservations file.\n");
        return;
    }

    // Get current date as reservation date
    char reservationDate[20];
    getCurrentDate(reservationDate);

    fprintf(reservationFile, "%-15s%-10d%-20s%-12s%-12s\n",
            username, roomID, reservationDate, "N/A", "N/A");

    fclose(reservationFile);

    // Update rooms.txt to mark room as reserved
    writeRooms(rooms, roomCount);

    printf("Reservation successful for Room ID %d.\n", roomID);
}

#define RESERVATION_FILE "reservations.txt"
#define MAX_LINE_LENGTH 100

void checkIn(char *username) {
    char line[MAX_LINE_LENGTH], updatedData[1000] = "";
    char currentDate[20];
    bool reservationFound = false;
    bool checkInUpdated = false;


    if (!isUsernameTaken(username, "customer.txt")) {
        printf("Error: Username does not exist.\n");
        return;
    }

    // Open reservation file
    FILE *file = fopen(RESERVATION_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open the reservation file.\n");
        return;
    }

    // Read the current date
    getCurrentDate(currentDate);

    // Load all rooms
    struct Room rooms[100];
    int roomCount = readRooms(rooms, 100);

    // Read reservations and update if a matching reservation is found
    fgets(line, sizeof(line), file);  // Read and store the header
    strcat(updatedData, line);        // Preserve the header in the updated data
    fgets(line, sizeof(line), file);  // Blank line
    strcat(updatedData, line);        // Preserve the blank line

    int roomIDToUpdate = -1;  // Track the room ID to update

    while (fgets(line, sizeof(line), file) != NULL) {
        char reservationUsername[50], reservationDate[20], checkIn[20], checkOut[20];
        int roomID;

        sscanf(line, "%s %d %s %s %s",
               reservationUsername, &roomID, reservationDate, checkIn, checkOut);

        if (strcmp(reservationUsername, username) == 0 && strcmp(checkIn, "N/A") == 0) {
            // Match found with no check-in
            reservationFound = true;
            strcpy(checkIn, currentDate);
            checkInUpdated = true;
            roomIDToUpdate = roomID;  // Mark room to be updated
        }

        // Append the (possibly updated) line to updatedData
        char updatedLine[MAX_LINE_LENGTH];
        snprintf(updatedLine, sizeof(updatedLine), "%-15s%-10d%-20s%-12s%-12s\n",
                 reservationUsername, roomID, reservationDate, checkIn, checkOut);
        strcat(updatedData, updatedLine);
    }

    fclose(file);

    if (reservationFound) {
        // Write the updated data back to the reservation file
        file = fopen(RESERVATION_FILE, "w");
        if (file == NULL) {
            printf("Error: Could not open the reservation file for writing.\n");
            return;
        }
        fputs(updatedData, file);
        fclose(file);

        // Update room status to "Occupied"
        if (roomIDToUpdate != -1) {
            int roomIndex = findRoomIndex(rooms, roomCount, roomIDToUpdate);
            if (roomIndex != -1) {
                strcpy(rooms[roomIndex].status, "Occupied");
                writeRooms(rooms, roomCount);  // Write back the updated room data
            }
        }

        printf("Check-in completed successfully for username '%s'.\n", username);
        return;
    }

    // Scenario 2: No reservation exists, check available rooms
    printf("No reservation found for '%s'. Checking available rooms...\n", username);

    // Display available rooms
    bool hasAvailableRooms = false;
    printf("\n%-6s  %-10s  %-8s   %-12s  %-8s\n", 
           "RoomID", "RoomType", "RoomRate", "Status", "Capacity");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < roomCount; i++) {
        if (strcmp(rooms[i].status, "Available") == 0) {
            printf("%-6d  %-10s  %-8.2f   %-12s  %-8d\n",
                   rooms[i].roomID,
                   rooms[i].roomType,
                   rooms[i].roomRate,
                   rooms[i].status,
                   rooms[i].capacity);
            hasAvailableRooms = true;
        }
    }

    if (!hasAvailableRooms) {
        printf("No rooms are currently available for check-in.\n");
        return;
    }

    // Get room ID from user
    int roomID;
    printf("\nEnter Room ID to check-in: ");
    scanf("%d", &roomID);

    // Validate the selected room
    int roomIndex = findRoomIndex(rooms, roomCount, roomID);
    if (roomIndex == -1 || strcmp(rooms[roomIndex].status, "Available") != 0) {
        printf("Error: Room ID is invalid or not available.\n");
        return;
    }

    // Update room status
    strcpy(rooms[roomIndex].status, "Occupied");
    writeRooms(rooms, roomCount);

    // Add check-in record to reservations file
    file = fopen(RESERVATION_FILE, "a");
    if (file == NULL) {
        printf("Error: Could not open the reservation file for appending.\n");
        return;
    }

    fprintf(file, "%-15s%-10d%-20s%-12s%-12s\n",
            username, roomID, "N/A", currentDate, "N/A");
    fclose(file);

    printf("Check-in completed successfully for Room ID %d.\n", roomID);
}

void checkOut(char *username) {
    char line[MAX_LINE_LENGTH], updatedData[1000] = "";
    char currentDate[20];
    bool checkOutUpdated = false;
    int roomIDToUpdate = -1;

    if (!isUsernameTaken(username, "customer.txt")) {
        printf("Error: Username does not exist.\n");
        return;
    }

    // Open reservation file
    FILE *file = fopen(RESERVATION_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open the reservation file.\n");
        return;
    }

    // Read the current date
    getCurrentDate(currentDate);

    // Read the file header and preserve it
    fgets(line, sizeof(line), file);  // Read header
    strcat(updatedData, line);        // Store the header
    fgets(line, sizeof(line), file);  // Blank line
    strcat(updatedData, line);        // Store the blank line

    while (fgets(line, sizeof(line), file) != NULL) {
        char reservationUsername[50], reservationDate[20], checkIn[20], checkOut[20];
        int roomID;

        // Parse the reservation data
        sscanf(line, "%s %d %s %s %s", reservationUsername, &roomID, reservationDate, checkIn, checkOut);

        // Check if the username matches, and the user has checked in but not checked out
        if (strcmp(reservationUsername, username) == 0 && strcmp(checkIn, "N/A") != 0 && strcmp(checkOut, "N/A") == 0) {
            // Match found with check-in but no check-out
            strcpy(checkOut, currentDate);  // Update check-out date
            roomIDToUpdate = roomID;        // Store the room ID to update later
            checkOutUpdated = true;
        }

        // Append the (possibly updated) line to updatedData
        char updatedLine[MAX_LINE_LENGTH];
        snprintf(updatedLine, sizeof(updatedLine), "%-15s%-10d%-20s%-12s%-12s\n",
                 reservationUsername, roomID, reservationDate, checkIn, checkOut);
        strcat(updatedData, updatedLine);
    }

    fclose(file);

    // If a check-out was performed
    if (checkOutUpdated) {
        // Write the updated data back to the file
        file = fopen(RESERVATION_FILE, "w");
        if (file == NULL) {
            printf("Error: Could not open the reservation file for writing.\n");
            return;
        }
        fputs(updatedData, file);
        fclose(file);

        // Update the room status to "Available"
        struct Room rooms[100];
        int roomCount = readRooms(rooms, 100);

        // Find the room and update its status to "Available"
        int roomIndex = findRoomIndex(rooms, roomCount, roomIDToUpdate);
        if (roomIndex != -1) {
            strcpy(rooms[roomIndex].status, "Available");
            writeRooms(rooms, roomCount);  // Write back the updated room data
        }

        printf("Check-out completed successfully for username '%s'.\n", username);
        return;
    }

    // If no check-in was found
    printf("No check-in found for username '%s' with no check-out.\n", username);
}

// Function to show booking history of a user
void showBookingHistory(char *username) {
    char line[MAX_LINE_LENGTH];
    bool bookingFound = false;

    if (!isUsernameTaken(username, "customer.txt")) {
        printf("Error: Username does not exist.\n");
        return;
    }

    // Open reservation file
    FILE *file = fopen(RESERVATION_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open the reservation file.\n");
        return;
    }

    // Read the file header and skip it
    fgets(line, sizeof(line), file);  // Read header (no need to store it)
    fgets(line, sizeof(line), file);  // Blank line (skip)

    // Print header for booking history
    printf("\n%-15s %-10s %-20s %-12s %-12s\n", "Username", "RoomID", "ReservationDate", "Check-In", "Check-Out");
    printf("---------------------------------------------------------------\n");

    // Read through all reservations and print those matching the username
    while (fgets(line, sizeof(line), file) != NULL) {
        char reservationUsername[50], reservationDate[20], checkIn[20], checkOut[20];
        int roomID;

        // Parse each reservation line
        sscanf(line, "%s %d %s %s %s", reservationUsername, &roomID, reservationDate, checkIn, checkOut);

        // If the username matches, print the booking details
        if (strcmp(reservationUsername, username) == 0) {
            printf("%-15s %-10d %-20s %-12s %-12s\n",
                   reservationUsername, roomID, reservationDate, checkIn, checkOut);
            bookingFound = true;
        }
    }

    fclose(file);

    // If no bookings were found for the username
    if (!bookingFound) {
        printf("No bookings found for username '%s'.\n", username);
    }
}

// Function to cancel a reservation
void cancelReservation(char *username) {
    int roomID = -1;
    char line[MAX_LINE_LENGTH], updatedData[1000] = "";
    bool reservationFound = false;

    if (!isUsernameTaken(username, "customer.txt")) {
        printf("Error: Username does not exist.\n");
        return;
    }

    // Open reservation file
    FILE *file = fopen(RESERVATION_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open the reservation file.\n");
        return;
    }

    // Read the header and blank line, keeping them in the updated data
    fgets(line, sizeof(line), file);  // Read and store the header
    strcat(updatedData, line);        // Preserve the header in the updated data
    fgets(line, sizeof(line), file);  // Blank line
    strcat(updatedData, line);        // Preserve the blank line

    // Read reservations and check if cancellation is needed
    while (fgets(line, sizeof(line), file) != NULL) {
        char reservationUsername[50], reservationDate[20], checkIn[20], checkOut[20];
        int tempRoomID;

        sscanf(line, "%s %d %s %s %s",
               reservationUsername, &tempRoomID, reservationDate, checkIn, checkOut);

        if (strcmp(reservationUsername, username) == 0 && strcmp(checkIn, "N/A") == 0) {
            // Reservation found that can be canceled
            reservationFound = true;
            roomID = tempRoomID; // Store roomID for status update
            printf("Reservation found for username '%s' with Room ID %d. Cancelling...\n", username, roomID);
            continue;  // Skip this reservation as it is being canceled
        }

        // If reservation is not canceled, append it to updatedData
        char updatedLine[MAX_LINE_LENGTH];
        snprintf(updatedLine, sizeof(updatedLine), "%-15s%-10d%-20s%-12s%-12s\n",
                 reservationUsername, tempRoomID, reservationDate, checkIn, checkOut);
        strcat(updatedData, updatedLine);
    }

    fclose(file);

    if (reservationFound) {
        // Write the updated data back to the reservation file
        file = fopen(RESERVATION_FILE, "w");
        if (file == NULL) {
            printf("Error: Could not open the reservation file for writing.\n");
            return;
        }
        fputs(updatedData, file);
        fclose(file);

        // Update the room status to "Available"
        struct Room rooms[100];
        int roomCount = readRooms(rooms, 100);
        int roomIndex = findRoomIndex(rooms, roomCount, roomID);
        if (roomIndex != -1) {
            strcpy(rooms[roomIndex].status, "Available");
            writeRooms(rooms, roomCount);
            printf("Room ID %d status updated to 'Available'.\n", roomID);
        } else {
            printf("Error: Could not find Room ID %d in the room file.\n", roomID);
        }

        printf("Reservation canceled successfully for username '%s'.\n", username);
    } else {
        printf("No reservation found for username '%s' to cancel.\n", username);
    }
}

// Function to view current booking for a username
void viewCurrentBooking(char *username) {
    char line[MAX_LINE_LENGTH];
    bool bookingFound = false;

    if (!isUsernameTaken(username, "customer.txt")) {
        printf("Error: Username does not exist.\n");
        return;
    }

    // Open the reservation file
    FILE *file = fopen(RESERVATION_FILE, "r");
    if (file == NULL) {
        printf("Error: Could not open the reservation file.\n");
        return;
    }

    // Read the header and blank line
    fgets(line, sizeof(line), file);  // Read and store the header
    fgets(line, sizeof(line), file);  // Blank line (to skip)

    // Read reservations and display the booking if found
    while (fgets(line, sizeof(line), file) != NULL) {
        char reservationUsername[50], reservationDate[20], checkIn[20], checkOut[20];
        int roomID;

        sscanf(line, "%s %d %s %s %s",
               reservationUsername, &roomID, reservationDate, checkIn, checkOut);

        if (strcmp(reservationUsername, username) == 0 && strcmp(checkIn, "N/A") == 0) {
            // Booking found with no check-in yet
            bookingFound = true;
            printf("\nCurrent booking for username '%s':\n", username);
            printf("Room ID: %-6d\n", roomID);
            printf("Reservation Date: %-12s\n", reservationDate);
            printf("Check-In Date: %-12s\n", checkIn);
            printf("Check-Out Date: %-12s\n", checkOut);
            break;  // Stop after finding the first matching reservation
        }
    }

    fclose(file);

    if (!bookingFound) {
        printf("No current booking found for username '%s' or you have already checked in.\n", username);
    }
}
