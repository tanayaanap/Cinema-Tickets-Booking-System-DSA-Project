#ifndef CINEMA_H
#define CINEMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define USERNAME_LENGTH 20
#define PASSWORD_LENGTH 20
#define MAX_THEATERS 5
#define MAX_HALLS 3
#define MAX_MOVIES 10
#define MAX_SEATS 10

extern char pvrMovies[MAX_MOVIES][30];
extern char inoxMovies[MAX_MOVIES][30];
extern char cinepolisMovies[MAX_MOVIES][30];
extern char mirajMovies[MAX_MOVIES][30];
extern char movieTimings[5][30];


// Node for seat booking
typedef struct node {
    char row_no;
    int seat_no, pin;
    char book;
    //int theater_id;
    int theater;
    int movie;
    struct node *next, *prev;
} node;

extern node *hn;

typedef struct Seat{
    char seatNumber[3];
    int booked; // 1 for booked, 0 for available
    int pin; // PIN for the booking
    struct Seat *next;
} Seat;

typedef struct SeatManagement{
    Seat **hashtable;
}SeatManagement;

typedef struct {
    char name[50];
    Seat seats[MAX_SEATS];
    int totalSeats;
} Movie;

typedef struct {
    char name[50];
    Movie movies[MAX_MOVIES];
    int totalMovies;
} Theater;




// User structure
typedef struct {
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} User;

extern User users[MAX_USERS];
extern int userCount;


typedef struct TheaterHashTable {
    node* head;  // Pointer to the doubly linked list for this theater
} TheaterHashTable;

TheaterHashTable theaterHash[MAX_THEATERS];

//node *seatLayout[MAX_THEATERS][MAX_HALLS]; // 2D array to represent seat layout for each theater and hall


// Snack structure
typedef struct {
    char name[30];
    float price;
    int quantity;
} Snack;

extern Snack snacks[];
extern int snackCount;

typedef struct {
    char actionType[10]; // "book" or "cancel"
    char row;
    int seat;
    int pin;
} Action;

// Stack structure for actions
typedef struct {
    Action actions[100];
    int top;
} ActionStack;


// Function declarations
void registerUser();
int loginUser();
void userLoginMenu();
void create();
//void display();
//void display1(node *tmp[20], int n);
//void book_seat(int current_theater, int current_movie);

int user_selected_theater();
void display_movies(int theater_id);
int user_selected_movie();

void displayTheatres();

void displaySnacksMenu();
void bookSnacks();
void generateSnackBill();
void cancel();
int user_selected_showtime();

//STACK
void undoLastAction();
void pushAction(Action action);
Action popAction();
int isStackEmpty();

//void bookSeatForTheater(int theaterId);
void bookSeatForTheater(int theaterId, char *selectedMovie, char *selectedShowtime);
void displaySeatsForTheater(int theaterId);
void createSeatsForTheater(int theaterId);
void initializeTheaterHash();
void cancelSeatForTheater(int theaterId);
void generateTicket(int theaterId, char *movieName, char *showtime, node *bookedSeats[], int seatCount);
void applyPromoCode(float *totalCost, int ticketCount);



#endif
