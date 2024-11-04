/**
 * @file userauthentication.cpp
 * @brief Implementation of user authentication functions, including login, registration, guest operations, and menu management.
 */
#include "../header/userauthentication.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "../../recipecosting/header/recipecosting.h"
#include "../../budgetplanner/header/budgetplanner.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>
#ifdef _WIN32
#include <conio.h> // For Windows
#else
#include <termios.h> // For Linux
#include <unistd.h>
#endif
#include <ctype.h>     
#include <stdbool.h> 
#include "../../priceadjustment/header/priceadjustment.h"

 /**
  * @brief Defines the logged-in user for the current session.
  */
User loggedUser;

/**
 * @brief Clears the console screen.
 */
void clearScreen() {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

/**
 * @brief Waits for the user to press enter to continue.
 *
 * @return Always returns 1.
 */
int enterToContinue() {
	printf("\n\033[1;33mPress enter to continue...\033[0m");
	while (getchar() != '\n');
	return 1;
}

/**
 * @brief Handles input error by clearing the input buffer and displaying an error message.
 *
 * @return Always returns 0.
 */
int handleInputError() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
	printf("Invalid input. Please enter a number.\n");
	return 0;
}

/**
 * @brief Gets input from the user and returns it as an integer.
 *
 * @return The integer input by the user, or -2 if the input is invalid.
 */
int getInput() {
	char line[256];
	int choice, result;

	if (fgets(line, sizeof(line), stdin) == NULL) {
		return -1;
	}

	result = sscanf(line, "%d", &choice);
	if (result != 1) {
		return -2;
	}

	return choice;
}

/**
 * @brief Prints the main menu options.
 *
 * @return Always returns 1.
 */
int printMainMenu() {
	printf("\n+---------------------------------------+\n");
	printf("|           MAIN MENU                   |\n");
	printf("+---------------------------------------+\n");
	printf("| 1. Login                              |\n");
	printf("| 2. Register                           |\n");
	printf("| 3. Guest Operations                   |\n");
	printf("| 4. Exit Program                       |\n");
	printf("+---------------------------------------+\n");
	printf("\nPlease enter a number to select: ");
	return 1;
}

/**
 * @brief Prints the user operations menu options.
 *
 * @return Always returns 1.
 */
int printUserMenu() {
	clearScreen();
	printf("\n+---------------------------------------+\n");
	printf("|         USER OPERATIONS MENU          |\n");
	printf("+---------------------------------------+\n");
	printf("| 1. Ingredient Management              |\n");
	printf("| 2. Recipe Costing                     |\n");
	printf("| 3. Price Adjustment                   |\n");
	printf("| 4. Budget Planner                     |\n");
	printf("| 5. Exit                               |\n");
	printf("+---------------------------------------+\n");
	printf("\nPlease enter a number to select: ");
	return 1;
}

/**
 * @brief Prints the guest operations menu options.
 *
 * @return Always returns 1.
 */
int printGuestMenu() {
	printf("\n+---------------------------------------+\n");
	printf("|         GUEST OPERATIONS MENU         |\n");
	printf("+---------------------------------------+\n");
	printf("| 1. View Ingredients                   |\n");
	printf("| 2. View Recipes                       |\n");
	printf("| 3. Exit                               |\n");
	printf("+---------------------------------------+\n");
	printf("\nPlease enter a number to select: ");
	return 1;
}

/**
 * @brief Generates a new user ID.
 *
 * @param users Array of existing users.
 * @param userCount Number of users in the array.
 * @return A new unique user ID.
 */
int getNewUserId(User users[], int userCount) {
	if (userCount == 0)
		return 1;

	int maxId = users[0].id;

	return maxId + 1;
}
