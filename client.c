#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define SESSION_TIMEOUT 300

int sock = 0;

// Signal Handler timeout for 300 second
void handle_session_timeout(int sig) {
    printf("\nThis Session is expired due to inactivity. Logging out...\n");
    close(sock);
    exit(0);
}

//to reset the alarm
void reset_timer() {
    alarm(SESSION_TIMEOUT);
}

int main() {
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char account_id[20], pin[10];

    // --- STEP 1: Network Setup (Member 1) ---
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // --- STEP 2: Authentication (Logic from Login Activity Diagram) ---
    printf("--- Welcome to SixSeven ATM ---\n");
    printf("Enter Account ID: ");
    scanf("%s", account_id);
    printf("Enter PIN: ");
    scanf("%s", pin);

    // LGN:ID:PIN
    sprintf(buffer, "LGN:%s:%s", account_id, pin);
    send(sock, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));
    read(sock, buffer, 1024);

    if (strcmp(buffer, "OK") != 0) {
        printf("Authentication failed: %s\n", buffer);
        close(sock);
        return 0;
    }

    // --- STEP 3: Start Session Management (Member 4) ---
    signal(SIGALRM, handle_session_timeout);
    reset_timer();

    // --- STEP 4: Main Menu Loop ---
    int choice;
    while(1) {
        printf("\n1. Balance\n2. Withdraw\n3. Exit\nChoice: ");
        scanf("%d", &choice);
        
        reset_timer(); // Every interaction resets the 5-min clock!

        if (choice == 1) {
            send(sock, "BAL", 3, 0);
            memset(buffer, 0, sizeof(buffer));
            read(sock, buffer, 1024);
            printf("Your Balance: %s\n", buffer);
        } 
        else if (choice == 3) {
            send(sock, "BYE", 3, 0);
            break;
        }
        // ... add Withdraw/Deposit here ...
    }

    close(sock);
    return 0;
}
