#ifndef USER_H
#define USER_H

// Define the User structure for staff and customers
typedef struct {
    int role;                     // 1 for Staff, 2 for Customer
    char name[50];                // Full name
    char username[30];            // Unique username
    char password[30];            // Password
    char dob[15];                 // Date of Birth (DD/MM/YYYY)
    char gender[10];              // Gender (Male/Female)
    char email[50];               // Email address
    char phone[15];               // Phone number (10 digits)
    char dateOfRegister[15];      // Date of registration (DD/MM/YYYY)
    int status;                   // 1 for Active, 0 for Inactive
} User;

void registerUser();
void updateUserStatus();
int isUsernameTaken(const char *username, const char *filename);

#endif