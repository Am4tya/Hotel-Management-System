#include <stdio.h>
#include <time.h>
#include <conio.h>

// Function to hide password input while typing
void hidden_password(char *password) {
    char ch;
    int i = 0;
    while((ch = getch()) != '\n' && ch != '\r') {  // '\n' is Enter key for Unix and '\r' is Enter key for Windows
        if (ch != 8 && ch != 127) {  // Backspace characters for Unix and Windows
            password[i++] = ch;
            printf("*");
        } else if (ch == 8 || ch == 127) {  // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b");  // Erase '*' character
            }
        }
    }
    password[i] = '\0';  // Null-terminate the string
    printf("\n");
}

// Function to get the current date in DD/MM/YY format
void getCurrentDate(char *date) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(date, 20, "%d/%m/%Y", t);
}

// Function to get the current time in HH:MM format
void getCurrentTime(char *time_now) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(time_now, 20, "%H:%M", t);
}