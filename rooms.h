#ifndef ROOMS_H
#define ROOMS_H

// Function prototypes
void addRoom();
void updateRoomType();
void updateRoomStatus();
void updateRoomCapacity();
void removeRoom();
void showAvailableRooms();
void viewCurrentBooking(char *username);
void reserveRoom(char *username);
void checkIn(char *username);
void checkOut(char *username);
void cancelReservation(char *username);
void showBookingHistory(char *username);

#endif