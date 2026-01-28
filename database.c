// database.c
#include <string.h>

// Format for a User
struct User {
    char username[50];
    char password[50];
};

// Authorized users
struct User user_list[] = {
    {"haqimi", "1234"},
    {"ali", "5678"},
    {"sara", "9999"}
};

// Calculate how many users are in the list
int total_users = sizeof(user_list) / sizeof(user_list[0]);

int check_credentials(char *input_user, char *input_pass) {
    for (int i = 0; i < total_users; i++) {
        // Check username and password match 
        if (strcmp(input_user, user_list[i].username) == 0 && 
            strcmp(input_pass, user_list[i].password) == 0) {
            return 1; // user found
        }
    }
    return 0; // if not in database it exit
}