
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

#include"cinema.h"

#define MAX_USERS 100
#define USERNAME_LENGTH 20
#define PASSWORD_LENGTH 20

#define MAX_THEATERS 5
#define MAX_HALLS 3
#define MAX_MOVIES 10
#define MAX_SEATS 10

// Global hash table for theaters
TheaterHashTable theaterHash[MAX_THEATERS];

// Stack implementation for undo functionality
#define MAX_ACTIONS 100

node *hn = NULL;

User users[MAX_USERS];
int userCount = 0;  // Counter for registered users

Snack snacks[] = {
    {"Popcorn", 150.0, 0},
    {"Cold Drink", 80.0, 0},
    {"Nachos", 120.0, 0},
    {"Samosa", 50.0, 0}
};



//MOVIES THEATRES HALLS BOOKING

// Global declarations
char *theaters[] = {
    "PVR Cinemas",
    "INOX Cinemas",
    "Cinepolis",
    "Miraj Cinemas"
};
//char theaters[MAX_THEATERS][30] = {"IMAX", "PVR Cinemas", "Cinepolis", "Miraj Cinemas"};
char halls[MAX_HALLS][30] = {"Hall 1", "Hall 2", "Hall 3"};
//char movies[MAX_MOVIES][30] = {"Padmaavat", "Kalki 2989 AD", "Kabir Singh", "RRR", "Animal"};
//char movieTimings[MAX_MOVIES][30] = {"10:00 AM", "1:00 PM", "4:00 PM", "7:00 PM", "10:00 PM"}; // Movie timings

// Array to track seat bookings (1 for booked, 0 for available)
int seats[MAX_HALLS][MAX_SEATS] = {0};  // Initialize all seats as available




// Function to register a new user


// Function to register a new user
void registerUser() {
    if (userCount >= MAX_USERS) {
        printf("==============================================\n");
        printf(" User limit reached. Cannot register more users.\n");
        printf("==============================================\n");
        return;
    }

    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];

    printf("==============================================\n");
    printf("          User Registration                   \n");
    printf("==============================================\n");
    printf("Enter a username: ");
    scanf("%s", username);
    printf("Enter a password: ");
    scanf("%s", password);

    // Check if username already exists
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0) {
            printf("==============================================\n");
            printf(" Username already exists. Try a different one.\n");
            printf("==============================================\n");
            return;
        }
    }

    // Save the user in the array
    strcpy(users[userCount].username, username);
    strcpy(users[userCount].password, password);
    userCount++;

    // Save the user data to the file
    FILE *file = fopen("user_data.txt", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "Username: %s\nPassword: %s\n\n", username, password);
    fclose(file);

    printf("==============================================\n");
    printf("       Registration successful!               \n");
    printf("==============================================\n");
}


// Function to login a user
// Function to login a user by reading user data from the file
int loginUser() {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char fileUsername[USERNAME_LENGTH];
    char filePassword[PASSWORD_LENGTH];

    printf("==============================================\n");
    printf("               User Login                     \n");
    printf("==============================================\n");
    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    // Open the file for reading
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;  // Return 0 for unsuccessful login if file can't be opened
    }

    // Read the user data from the file and check credentials
    while (fscanf(file, "Username: %s\nPassword: %s\n", fileUsername, filePassword) != EOF) {
        if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0) {
            printf("==============================================\n");
            printf(" Login successful! Welcome, %s.\n", username);
            printf("==============================================\n");
            fclose(file);
            return 1;  // Return 1 for successful login
        }
    }

    // Close the file if no match found
    fclose(file);
    printf("==============================================\n");
    printf(" Invalid username or password. Please try again.\n");
    printf("==============================================\n");
    return 0;  // Return 0 for unsuccessful login
}


// Function to display the login menu
void userLoginMenu() {
    int choice;
    do {
        printf("\n==============================================\n");
        printf("                USER LOGIN MENU               \n");
        printf("==============================================\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    return;  // Exit the menu after successful login
                }
                break;
            case 3:
                printf("Exiting the login menu.\n");
                exit(0);
            default:
                printf("==============================================\n");
                printf(" Invalid choice. Please try again.\n");
                printf("==============================================\n");
        }
    } while (choice != 3);
}

//HASH
void initializeTheaterHash() {
    for (int i = 0; i < MAX_THEATERS; i++) {
        theaterHash[i].head = NULL;
    }
}

void createSeatsForTheater(int theaterId) {
    node *hn = NULL, *nn, *cn;
    int j = 1;
    char c = 'A';

    do {
        int i = 1;
        do {
            nn = (node *)malloc(sizeof(node));
            nn->next = nn->prev = NULL;
            nn->row_no = c;
            nn->seat_no = i;
            nn->book = 'a'; // Available
            if (hn == NULL) {
                hn = nn;
                nn->next = nn->prev = hn;
            } else {
                cn = hn;
                while (cn->next != hn) {
                    cn = cn->next;
                }
                cn->next = nn;
                nn->prev = cn;
                nn->next = hn;
                hn->prev = nn;
            }
            i++;
        } while (i <= 7);
        j++;
        c++;
    } while (j <= 10);

    theaterHash[theaterId].head = hn;
}
// Function to display seats for a specific theater
void displaySeatsForTheater(int theaterId) {
    if (theaterId < 0 || theaterId >= MAX_THEATERS) {
        printf("Invalid theater ID.\n");
        return;
    }

    node *cn = theaterHash[theaterId].head;
    if (!cn) {
        printf("No seats available for this theater.\n");
        return;
    }

    printf("Displaying seats for Theater %d\n", theaterId + 1);
    printf("--------------------------------------------------------------------------------\n");
    printf("|                                  Platinum                                    |\n");
    do {
        if (cn->prev->row_no != cn->row_no)
            printf("| ");
        printf("%c%d", cn->row_no, cn->seat_no);
        if (cn->book == 'a')
            printf(" [AVL]   ");
        else
            printf(" [BKD]   ");
        if (cn->next->row_no != cn->row_no)
            printf("|\n");
        if (cn->row_no == 'C' && cn->seat_no == 7) {
            printf("--------------------------------------------------------------------------------\n");
            printf("|                                  Gold                                        |\n");
        }
        if (cn->row_no == 'H' && cn->seat_no == 7) {
            printf("--------------------------------------------------------------------------------\n");
            printf("|                                 Silver                                       |\n");
        }
        cn = cn->next;
    } while (cn != theaterHash[theaterId].head);

    printf("--------------------------------------------------------------------------------\n\n");
    printf("PLATINUM-> 750                 GOLD-> 600                     SILVER-> 450\n");
}

void bookSeatForTheater(int theaterId, char *selectedMovie, char *selectedShowtime) {
    if (theaterId < 0 || theaterId >= MAX_THEATERS) {
        printf("Invalid theater ID.\n");
        return;
    }

    node *cn = theaterHash[theaterId].head; // Start from the head of the linked list for the theater
    if (!cn) {
        printf("No seats available for this theater.\n");
        return;
    }

    int numTickets; // Number of tickets to book
    node *bookedSeats[MAX_SEATS]; // Array to track booked seats
    int seatCount = 0; // Counter for booked seats

    printf("Enter the number of tickets to book: ");
    scanf("%d", &numTickets);

    if (numTickets <= 0 || numTickets > MAX_SEATS) {
        printf("Invalid number of tickets.\n");
        return;
    }

    for (int i = 0; i < numTickets; i++) {
        char row; // Row identifier (e.g., 'A', 'B', etc.)
        int seat; // Seat number within the row

        printf("\nEnter Seat Number for Ticket %d (e.g., A1): ", i + 1);
        scanf(" %c%d", &row, &seat);

        node *current = cn; // Pointer to iterate over the linked list
        int seatFound = 0; // To track if a valid seat is found
        do {
            if (current->row_no == row && current->seat_no == seat) {
                seatFound = 1;
                if (current->book == 'a') { // Check if the seat is available
                    current->book = 'b'; // Mark the seat as booked
                    int pin = rand() % 9000 + 1000; // Generate a 4-digit PIN
                    current->pin = pin;
                    printf("Seat %c%d booked successfully! PIN: %d\n", row, seat, pin);

                    bookedSeats[seatCount++] = current; // Add the booked seat to the array
                    break;
                } else {
                    printf("Seat %c%d is already booked! Please select another seat.\n", row, seat);
                    i--; // Retry this ticket
                    break;
                }
            }
            current = current->next; // Move to the next seat
        } while (current != cn); // Continue until the circular linked list loops back

        if (!seatFound) {
            printf("Seat %c%d not found! Please enter a valid seat.\n", row, seat);
            i--; // Retry this ticket
        }
    }

    // Generate the ticket
    generateTicket(theaterId, selectedMovie, selectedShowtime, bookedSeats, seatCount);
}

void cancelSeatForTheater(int theaterId) {
    if (theaterId < 0 || theaterId >= MAX_THEATERS) {
        printf("Invalid theater ID.\n");
        return;
    }

    node *cn = theaterHash[theaterId].head;
    char row;
    int seat, pin;

    if (!cn) {
        printf("No seats available for this theater.\n");
        return;
    }

    printf("Enter Seat Number (e.g., A1): ");
    scanf(" %c%d", &row, &seat);
    printf("Enter PIN: ");
    scanf("%d", &pin);

    do {
        if (cn->row_no == row && cn->seat_no == seat) {
            if (cn->book == 'b' && cn->pin == pin) {
                cn->book = 'a'; // Mark as available
                cn->pin = 0;    // Reset the PIN
                printf("Seat %c%d canceled successfully!\n", row, seat);

                // Push action to stack
                Action action = {"cancel", row, seat, pin};
                pushAction(action);
                return;
            } else if (cn->book == 'b') {
                printf("Invalid PIN for seat %c%d.\n", row, seat);
                return;
            } else {
                printf("Seat %c%d is not booked.\n", row, seat);
                return;
            }
        }
        cn = cn->next;
    } while (cn != theaterHash[theaterId].head);

    printf("Seat %c%d not found!\n", row, seat);
}


void create()
{
    node *nn, *cn;
    int j = 1;
    int k;
    char c = 'A';
    do
    {
        int i = 1;
        do
        {
            nn = (node *)malloc(sizeof(node));
            nn->next = nn->prev = NULL;
            nn->row_no = c;
            nn->seat_no = i;
            nn->pin = k;
            nn->book = 'a';
            if (hn == NULL)
            {
                hn = nn;
                nn->next = nn->prev = hn;
            }
            else
            {
                cn = hn;
                while (cn->next != hn)
                    cn = cn->next;
                cn->next = nn;
                nn->prev = cn;
                nn->next = hn;
                hn->prev = nn;
            }
            i++;
        } while (i <= 7);
        j++;
        c++;
    } while (j <= 10);
}


int user_selected_theater() {
    int choice;
    printf("Enter your choice (1-5): ");
    scanf("%d", &choice);
    return choice;
}


//DISPLAY THEATRE
void displayTheatres() {
    printf("==============================================\n");
    printf("                Available Theatres              \n");
    printf("==============================================\n");
    printf("1. PVR Cinemas\n");
    printf("2. INOX Cinemas\n");
    printf("3. Cinepolis\n");
    printf("4. Miraj Cinemas\n");
    printf("5. Exit\n");
}

//DISPLAY MOVIES OF THAT THEATRE
void display_movies(int theater_id) {
    printf("==============================================\n");
    printf("               Movies Available               \n");
    printf("==============================================\n");

    // Display movies based on the selected theater
    if (theater_id == 1) {
        printf("Movies available in PVR Cinemas:\n");
        printf("1. Padmaavat\n");
        printf("2. Kabir Singh\n");
        printf("3.Baahubali\n");
        printf("4. RRR\n");
    } else if (theater_id == 2) {
        printf("Movies available in INOX Cinemas:\n");
        printf("1. RRR\n");
        printf("2. Animal\n");
        printf("3.Pushpa\n");
        printf("4. Padmaavat\n");
    } else if (theater_id == 3) {
        printf("Movies available in Cinepolis:\n");
        printf("1. RRR\n");
        printf("2. Animal\n");
        printf("3. Padmaavat\n");
    } else if (theater_id == 4) {
        printf("Movies available in Miraj Cinemas:\n");
        printf("1. RRR\n");
        printf("2. Animal\n");
        printf("3. Padmaavat\n");
        
    } else if(theater_id == 5){
        printf("INVALID\n");

    }
}


int user_selected_movie() {
    int choice;
    printf("Enter your movie choice: ");
    scanf("%d", &choice);
    return choice;
}

int user_selected_showtime() {
    int choice;
    printf("\nSelect a showtime:\n");
    printf("1. 10:00 AM\n");
    printf("2. 1:00 PM\n");
    printf("3. 4:00 PM\n");
    printf("4. 7:00 PM\n");
    printf("5. 10:00 PM\n");
    printf("Enter your showtime choice: ");
    scanf("%d", &choice);

    if (choice >= 1 && choice <= 5) {
        return choice - 1; // Convert to array index
    }
    printf("Invalid choice. Defaulting to 10:00 AM.\n");
    return 0; // Default to the first timing
}



int snackCount = sizeof(snacks) / sizeof(snacks[0]);

void displaySnacksMenu() {
    printf("\n-------------------- SNACKS MENU --------------------\n");
    for (int i = 0; i < snackCount; i++) {
        printf("%d. %s - Rs. %.2f\n", i + 1, snacks[i].name, snacks[i].price);
    }
    printf("------------------------------------------------------\n");
}

void bookSnacks() {
    int choice, qty;
    char more;

    do {
        displaySnacksMenu();
        printf("Select a snack by entering the corresponding number: ");
        scanf("%d", &choice);

        if (choice >= 1 && choice <= snackCount) {
            printf("Enter quantity for %s: ", snacks[choice - 1].name);
            scanf("%d", &qty);

            if (qty <= 0) {
                printf("Invalid quantity. Try again.\n");
                continue;
            }

            snacks[choice - 1].quantity += qty; // Increment snack quantity
            printf("Snack added successfully!\n");

            // Push snack order action to the stack
            Action action = {"snack", '\0', choice - 1, qty}; // Use `seat` for snack index
            pushAction(action);
        } else {
            printf("Invalid selection. Please try again.\n");
        }

        printf("Do you want to add more snacks? (y/n): ");
        scanf(" %c", &more);
    } while (more == 'y');
}


void generateSnackBill() {
    float totalCost = 0.0;
    printf("\n-------------------- SNACKS BILL --------------------\n");
    printf("| Snack Name       | Quantity | Price per Item | Total Cost |\n");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < snackCount; i++) {
        if (snacks[i].quantity > 0) {
            float cost = snacks[i].quantity * snacks[i].price;
            totalCost += cost;
            printf("| %-16s | %-8d | Rs. %-11.2f | Rs. %-8.2f |\n", 
                   snacks[i].name, snacks[i].quantity, snacks[i].price, cost);
        }
    }
    printf("------------------------------------------------------------\n");
    printf("Total Snack Cost: Rs. %.2f\n", totalCost);
    printf("------------------------------------------------------------\n");
}

// Cancellation function modified to push actions
void cancel() {
    char row;
    int seat, pin;
    node *cn;

    printf("\nSEAT CANCELLATION\n");
    printf("Enter Seat Number: ");
    scanf(" %c%d", &row, &seat);
    printf("Enter PIN: ");
    scanf("%d", &pin);

    cn = hn;
    do {
        if (cn->row_no == row && cn->seat_no == seat) {
            if (cn->pin == pin) {
                printf("Are you sure you want to cancel the seat (y/n)? ");
                char confirm;
                scanf(" %c", &confirm);
                if (confirm == 'y' || confirm == 'Y') {
                    cn->book = 'a'; // Mark the seat as available
                    printf("Seat %c%d cancelled successfully!\n", cn->row_no, cn->seat_no);

                    // Push cancellation action to the stack
                    Action action = {"cancel", row, seat, pin};
                    pushAction(action);
                }
                return;
            } else {
                printf("Invalid PIN for seat %c%d.\n", row, seat);
                return;
            }
        }
        cn = cn->next;
    } while (cn != hn);

    printf("Seat %c%d not found.\n", row, seat);
}

// Global stack and its top index
ActionStack actionStack = {.top = -1};

// Push an action onto the stack
void pushAction(Action action) {
    if (actionStack.top < MAX_ACTIONS - 1) {
        actionStack.actions[++actionStack.top] = action;
    } else {
        printf("Action stack is full. Cannot push action.\n");
    }
}


// Pop an action from the stack
Action popAction() {
    if (actionStack.top >= 0) {
        return actionStack.actions[actionStack.top--];
    }
    Action emptyAction = {"", '\0', 0, 0}; // Return an empty action if stack is empty
    return emptyAction;
}

// Check if the stack is empty
int isStackEmpty() {
    return actionStack.top == -1;
}

// Undo last action
void undoLastAction() {
    if (isStackEmpty()) {
        printf("No actions to undo.\n");
        return;
    }

    Action lastAction = popAction();
    printf("Undoing action: %s, Row: %c, Seat/Snack: %d, Quantity/PIN: %d\n", 
           lastAction.actionType, lastAction.row, lastAction.seat, lastAction.pin);

    node *cn;

    // Undo booking
    if (strcmp(lastAction.actionType, "book") == 0) {
        for (int i = 0; i < MAX_THEATERS; i++) {
            cn = theaterHash[i].head;
            if (!cn) continue;

            do {
                if (cn->row_no == lastAction.row && cn->seat_no == lastAction.seat) {
                    cn->book = 'a'; // Mark as available
                    cn->pin = 0;    // Reset the PIN
                    printf("Booking for seat %c%d undone successfully!\n", lastAction.row, lastAction.seat);
                    return;
                }
                cn = cn->next;
            } while (cn != theaterHash[i].head);
        }
    }

    // Undo cancellation
    if (strcmp(lastAction.actionType, "cancel") == 0) {
        for (int i = 0; i < MAX_THEATERS; i++) {
            cn = theaterHash[i].head;
            if (!cn) continue;

            do {
                if (cn->row_no == lastAction.row && cn->seat_no == lastAction.seat) {
                    cn->book = 'b'; // Mark as booked
                    cn->pin = lastAction.pin;
                    printf("Cancellation for seat %c%d undone successfully!\n", lastAction.row, lastAction.seat);
                    return;
                }
                cn = cn->next;
            } while (cn != theaterHash[i].head);
        }
    }

    // Undo snack order
    if (strcmp(lastAction.actionType, "snack") == 0) {
        int snackIndex = lastAction.seat; // Use `seat` as the snack index
        if (snackIndex >= 0 && snackIndex < snackCount) {
            snacks[snackIndex].quantity -= lastAction.pin; // Decrement the ordered quantity
            printf("Undo snack order: %d %s removed.\n", lastAction.pin, snacks[snackIndex].name);
            return;
        }
    }

    printf("Unable to undo the last action.\n");
}

//generate ticket
void generateTicket(int theaterId, char *movieName, char *showtime, node *bookedSeats[], int seatCount) {
    float totalCost = 0.0;
    printf("\n-------------------- MOVIE TICKET --------------------\n");
    printf("Theater: %s\n", theaters[theaterId]);
    printf("Movie: %s\n", movieName);
    printf("Showtime: %s\n", showtime);
    printf("------------------------------------------------------\n");
    printf("Seats: ");
    for (int i = 0; i < seatCount; i++) {
        printf("%c%d ", bookedSeats[i]->row_no, bookedSeats[i]->seat_no);
        // Calculate cost based on seat type
        if (bookedSeats[i]->row_no <= 'C') totalCost += 750; // Platinum
        else if (bookedSeats[i]->row_no <= 'H') totalCost += 600; // Gold
        else totalCost += 450; // Silver
    }
    printf("\n------------------------------------------------------\n");
    
    // Apply promo code for discounts
    applyPromoCode(&totalCost, seatCount);

    printf("Total Cost After Discount (if applicable): Rs. %.2f\n", totalCost);
    printf("------------------------------------------------------\n");
}


//apply for discount
void applyPromoCode(float *totalCost, int ticketCount) {
    if (ticketCount >= 3) {
        float discount = *totalCost * 0.10; // 10% discount
        *totalCost -= discount;
        printf("\nPromo Code Applied! You saved Rs. %.2f\n", discount);
        
    } else {
        printf("\nPromo Code is not applicable. Book at least 3 tickets to get a 10%% discount.\n");
    }
}

