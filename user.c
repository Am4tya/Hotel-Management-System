#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "utils.h"
#include "user.h"

// Function prototypes
void registerUser();
void saveUserToFile(User user);
int isUsernameTaken(const char *username, const char *filename);
int isValidPassword(const char *password);
int isValidDateFormat(const char *date);
int isValidPhoneNumber(char *phone);
int isValidEmail(char *email);

void registerUser()
{
    User newUser;

    printf("\n--- Register New User ---\n");
    // Role Input
    printf("Enter Role (1 for Staff, 2 for Customer): ");
    while (1)
    {
        scanf("%d", &newUser.role);
        getchar(); // Clear buffer
        if (newUser.role == 1 || newUser.role == 2)
        {
            break;
        }
        else
        {
            printf("Invalid role. Enter 1 for Staff or 2 for Customer: ");
        }
    }

    // Name Input
    printf("Enter Name: ");
    fgets(newUser.name, sizeof(newUser.name), stdin);
    newUser.name[strcspn(newUser.name, "\n")] = '\0'; // Remove trailing newline

    // Username input with duplicate check
    while (1)
    {
        printf("Enter Username: ");
        fgets(newUser.username, sizeof(newUser.username), stdin);
        newUser.username[strcspn(newUser.username, "\n")] = '\0'; // Remove trailing newline

        const char *fileToCheck = (newUser.role == 1) ? "staff.txt" : "customer.txt";

        if (!isUsernameTaken(newUser.username, fileToCheck))
        {
            break;
        }
        else
        {
            printf("Error: Username '%s' is already exists. Please try a different one.\n", newUser.username);
        }
    }

    // Password Input
    while (1)
    {
        printf("Enter Password: ");
        hidden_password(newUser.password); // Hidden password input
        if (!isValidPassword(newUser.password))
        {
            printf("Password must be 8 to 15 characters long. Try again.\n");
        }
        else
        {
            break;
        }
    }

    // Date of Birth Input
    while (1)
    {
        printf("Enter Date of Birth (DD/MM/YYYY): ");
        fgets(newUser.dob, sizeof(newUser.dob), stdin);
        newUser.dob[strcspn(newUser.dob, "\n")] = '\0'; // Remove trailing newline

        if (isValidDateFormat(newUser.dob))
        {
            break;
        }
        else
        {
            printf("Invalid date format. Try again.\n");
        }
    }

    // Gender Input
    while (1)
    {
        printf("Enter Gender (Male/Female): ");
        fgets(newUser.gender, sizeof(newUser.gender), stdin);
        newUser.gender[strcspn(newUser.gender, "\n")] = '\0'; // Remove trailing newline

        if (strcmp(newUser.gender, "Male") == 0 || strcmp(newUser.gender, "Female") == 0)
        {
            break;
        }
        else
        {
            printf("Invalid gender. Please enter 'Male' or 'Female'.\n");
        }
    }

    while (1)
    {
        // Email Input
        printf("Enter Email Address: ");
        fgets(newUser.email, sizeof(newUser.email), stdin);
        newUser.email[strcspn(newUser.email, "\n")] = '\0'; // Remove trailing newline

        if (isValidEmail(newUser.email))
        {
            break;
        }
        else
        {
            printf("Invalid email format. Try again.\n");
        }
    }

    // Phone Number Input
    while (1)
    {
        printf("Enter Phone Number: ");
        fgets(newUser.phone, sizeof(newUser.phone), stdin);
        newUser.phone[strcspn(newUser.phone, "\n")] = '\0'; // Remove trailing newline

        if (isValidPhoneNumber(newUser.phone))
        {
            break;
        }
        else
        {
            printf("Phone number must be 10 digits. Try again.\n");
        }
    }

    // Date of Registration Input
    while (1)
    {
        printf("Enter Date of Registration (DD/MM/YYYY): ");
        fgets(newUser.dateOfRegister, sizeof(newUser.dateOfRegister), stdin);
        newUser.dateOfRegister[strcspn(newUser.dateOfRegister, "\n")] = '\0'; // Remove trailing newline

        if (isValidDateFormat(newUser.dateOfRegister))
        {
            break;
        }
        else
        {
            printf("Invalid date format. Try again.\n");
        }
    }

    // Set default status as active
    newUser.status = 1;

    // Save the user to the respective file
    saveUserToFile(newUser);
}

// Save the user to the specified file
void saveUserToFile(User user)
{
    FILE *file;
    const char *filename;

    // Determine the file based on user role
    if (user.role == 1)
    {
        filename = "staff.txt";
    }
    else if (user.role == 2)
    {
        filename = "customer.txt";
    }
    else
    {
        printf("Invalid user role. Unable to save.\n");
        return;
    }

    // Open file in append mode
    file = fopen(filename, "a+");
    if (file == NULL)
    {
        printf("Error: Unable to open file '%s'.\n", filename);
        return;
    }

    // Check if file is empty to write the header
    fseek(file, 0, SEEK_END);
    if (ftell(file) == 0)
    {
        fprintf(file, "%-30s%-15s%-19s%-15s%-10s%-30s%-15s%-20s%-10s%-10s\n",
                "Name", "Username", "Password", "Date of Birth", "Gender",
                "Email", "Phone", "Date Registered", "Role", "Status");
    }

    // Append the user details
    fprintf(file, "%-30s%-15s%-19s%-15s%-10s%-30s%-15s%-20s%-10s%-10s\n",
            user.name,
            user.username,
            user.password,
            user.dob,
            user.gender,
            user.email,
            user.phone,
            user.dateOfRegister,
            (user.role == 1) ? "Staff" : "Customer",
            (user.status == 1) ? "Active" : "Inactive");

    fclose(file);
    printf("User successfully saved to '%s'.\n", filename);
}

// Trim leading/trailing whitespace
void trimWhitespace(char *str)
{
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1]))
    {
        str[--len] = '\0';
    }
    char *start = str;
    while (*start && isspace((unsigned char)*start))
    {
        start++;
    }
    memmove(str, start, strlen(start) + 1);
}

// Function to check if username is taken
int isUsernameTaken(const char *username, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return 0; // File does not exist yet, so username cannot be taken
    }

    char line[256];
    char fileUsername[30];

    // Skip the header row
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) != NULL)
    {

        // Extract the Username column (exactly 22 characters from the start)
        strncpy(fileUsername, line + 30, 15); // Username starts at position 30, is 15 chars wide
        fileUsername[15] = '\0';              // Null-terminate the extracted username

        // Trim any extra spaces from the extracted username
        trimWhitespace(fileUsername);

        // Compare with the input username
        if (strcmp(username, fileUsername) == 0)
        {
            fclose(file);
            return 1; // Username is taken
        }
    }

    fclose(file);
    return 0; // Username is not taken
}

// Password validation function (8 to 15 characters)
int isValidPassword(const char *password)
{
    int len = strlen(password);
    return len >= 8 && len <= 15;
}

// Date format(DD/MM/YYYY) validation function
int isValidDateFormat(const char *date)
{
    int day, month, year;
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) != 3)
    {
        return 0;
    }
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900)
    {
        return 0;
    }
    // Further checks for months with <31 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        return 0;
    if (month == 2)
    {
        // Check for leap year
        int isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (isLeap ? 29 : 28))
            return 0;
    }
    return 1;
}

// Phone number validation function
int isValidPhoneNumber(char *phone)
{
    int len = strlen(phone);

    // check if length is 10
    if (len != 10)
    {
        return 0;
    }

    // check if all digits are valid
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(phone[i]))
        {
            return 0;
        }
    }
    return 1;
}

// Email validation function
int isValidEmail(char *email)
{
    int atSymbol = -1;
    int dotSymbol = -1;
    int len = strlen(email);

    // Check for presence of '@'
    for (int i = 0; i < len; i++)
    {
        if (email[i] == '@')
        {
            atSymbol = i;
            break;
        }
    }
    // Check for presence of '.' after '@'
    if (atSymbol != -1)
    {
        for (int i = atSymbol + 1; i < len; i++)
        {
            if (email[i] == '.')
            {
                dotSymbol = i;
                break;
            }
        }
    }
    // Validate positions
    if (atSymbol != -1 && dotSymbol != -1 && dotSymbol > atSymbol + 1)
    {
        return 1;
    }
    return 0;
}

// Function to update user status
void updateUserStatus()
{
    char roleInput[10], username[30], newStatus[10];
    int role;
    FILE *file, *tempFile;
    char filename[20], tempFilename[] = "temp.txt";
    char line[256];
    int found = 0;

    printf("--Updating User Status--\n");
    // Ask for the role, username, and new status
    printf("Enter role (1 for Staff, 2 for Customer): ");
    fgets(roleInput, sizeof(roleInput), stdin);
    sscanf(roleInput, "%d", &role);

    if (role == 1)
    {
        strcpy(filename, "staff.txt");
    }
    else if (role == 2)
    {
        strcpy(filename, "customer.txt");
    }
    else
    {
        printf("Invalid role entered.\n");
        return;
    }

    printf("Enter username to update: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove newline character
    trimWhitespace(username);

    printf("Enter new status (1 for Active, 0 for Inactive): ");
    fgets(newStatus, sizeof(newStatus), stdin);
    int status = atoi(newStatus);

    // Open the file and a temporary file for rewriting
    file = fopen(filename, "r");
    tempFile = fopen(tempFilename, "w");

    if (file == NULL || tempFile == NULL)
    {
        printf("Error: Unable to open file.\n");
        return;
    }

    // Copy and modify data
    while (fgets(line, sizeof(line), file))
    {
        char fileUsername[30], fileStatus[10];

        // Extract the username from the line
        strncpy(fileUsername, line + 30, 15); // Username starts at position 30
        fileUsername[15] = '\0';
        trimWhitespace(fileUsername);

        // Check if the username matches
        if (strcmp(username, fileUsername) == 0)
        {
            found = 1;

            // Modify the status column
            if (status == 1)
            {
                snprintf(fileStatus, sizeof(fileStatus), "Active");
            }
            else if (status == 0)
            {
                snprintf(fileStatus, sizeof(fileStatus), "Inactive");
            }
            else
            {
                printf("Invalid status entered. No changes made.\n");
                fclose(file);
                fclose(tempFile);
                remove(tempFilename);
                return;
            }

            if (role == 1)
            {
                snprintf(line + 160, sizeof(line) - 160, "\t%-10s\n", fileStatus);
            }
            else if (role == 2)
            {
                snprintf(line + 160, sizeof(line) - 160, "er\t%-10s\n", fileStatus);
            }
        }

        // Write the line to the temp file
        fputs(line, tempFile);
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        // Replace the original file with the temp file
        remove(filename);
        rename(tempFilename, filename);
        printf("User status updated successfully.\n");
    }
    else
    {
        // Remove the temp file and notify the user
        remove(tempFilename);
        printf("Username '%s' not found.\n", username);
    }
}
