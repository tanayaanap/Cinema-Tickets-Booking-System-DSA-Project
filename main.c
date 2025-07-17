#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cinema.h"

// Declare movie timings globally
char movieTimings[5][30] = {"10:00 AM", "1:00 PM", "4:00 PM", "7:00 PM", "10:00 PM"};

// Movies for each theater
char pvrMovies[MAX_MOVIES][30] = {"Padmaavat", "Kabir Singh", "Baahubali", "RRR"};
char inoxMovies[MAX_MOVIES][30] = {"RRR", "Animal", "Pushpa", "Padmaavat"};
char cinepolisMovies[MAX_MOVIES][30] = {"RRR", "Animal", "Padmaavat"};
char mirajMovies[MAX_MOVIES][30] = {"RRR", "Animal", "Padmaavat"};

int main() {
    int theaterChoice, movieChoice, showtimeChoice;

    // Initialize the theater hash table
    initializeTheaterHash();
    for (int i = 0; i < MAX_THEATERS; i++) {
        createSeatsForTheater(i);
    }

    // User login menu
    userLoginMenu();

    do {
        printf("\n==============================================\n");
        printf("              Cinema Ticket Booking           \n");
        printf("==============================================\n");

        // Display available theaters
        displayTheatres();

        // Get user-selected theater
        theaterChoice = user_selected_theater();

        // Exit if the user selects the Exit option
        if (theaterChoice == 5) {
            printf("Exiting...\n");
            break;
        }

        if (theaterChoice < 1 || theaterChoice > MAX_THEATERS) {
            printf("Invalid theater choice. Please try again.\n");
            continue;
        }

        // Display movies for the selected theater
        display_movies(theaterChoice);

        // Get user-selected movie
        movieChoice = user_selected_movie(theaterChoice);  // Pass theaterChoice for proper movie array mapping

        if (movieChoice < 0 || movieChoice >= MAX_MOVIES) {
            printf("Invalid movie choice. Please try again.\n");
            continue;
        }

        // Prompt for showtime selection
        showtimeChoice = user_selected_showtime();

        // Correctly display the selected movie and showtime
        if (theaterChoice == 1) {
            // Adjust for 0-based indexing, user selects 1-4 but arrays are 0-indexed
            printf("Selected movie: %s, Showtime: %s\n", pvrMovies[movieChoice - 1], movieTimings[showtimeChoice]);
        } else if (theaterChoice == 2) {
            printf("Selected movie: %s, Showtime: %s\n", inoxMovies[movieChoice - 1], movieTimings[showtimeChoice]);
        } else if (theaterChoice == 3) {
            printf("Selected movie: %s, Showtime: %s\n", cinepolisMovies[movieChoice - 1], movieTimings[showtimeChoice]);
        } else if (theaterChoice == 4) {
            printf("Selected movie: %s, Showtime: %s\n", mirajMovies[movieChoice - 1], movieTimings[showtimeChoice]);
        }

        int ch; // For inner menu
        do {
            printf("\n1. DISPLAY SEATS\n2. BOOK SEATS\n3. CANCEL SEATS\n4. ORDER SNACKS\n5. UNDO ACTION\n6. EXIT\n");
            printf("Enter your choice: ");
            scanf("%d", &ch);

            switch (ch) {
                case 1:
                    displaySeatsForTheater(theaterChoice - 1); // Display seats for the selected theater
                    break;
                case 2:
                    if (theaterChoice == 1) {
                        bookSeatForTheater(theaterChoice - 1, pvrMovies[movieChoice - 1], movieTimings[showtimeChoice]);
                    } else if (theaterChoice == 2) {
                        bookSeatForTheater(theaterChoice - 1, inoxMovies[movieChoice - 1], movieTimings[showtimeChoice]);
                    } else if (theaterChoice == 3) {
                        bookSeatForTheater(theaterChoice - 1, cinepolisMovies[movieChoice - 1], movieTimings[showtimeChoice]);
                    } else if (theaterChoice == 4) {
                        bookSeatForTheater(theaterChoice - 1, mirajMovies[movieChoice - 1], movieTimings[showtimeChoice]);
                    }
                    break;
                case 3:
                    cancelSeatForTheater(theaterChoice - 1); // Cancel a booked seat
                    break;
                case 4:
                    bookSnacks(); // Order snacks
                    generateSnackBill(); // Generate and display the snack bill immediately after ordering
                    break;
                case 5:
                    undoLastAction(); // Undo the last action
                    break;
                case 6:
                    printf("Exiting...\n");
                    break;
                default:
                    printf("INVALID CHOICE\n");
            }
        } while (ch != 6); // Exit the inner menu when user selects Exit

    } while (1); // Continue until the user chooses to exit the program

    return 0;
}

