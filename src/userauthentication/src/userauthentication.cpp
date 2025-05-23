/**
 * @file userauthentication.cpp
 * @brief Implementation of user authentication functions, including login, registration, guest operations, and menu management.
 */
#include "../header/userauthentication.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "../../recipecosting/header/recipecosting.h"
#include "../../budgetplanner/header/budgetplanner.h"
#include "../../priceadjustment/header/priceadjustment.h"
#include <stdio.h>      
#include <stdlib.h>     
#include <string.h>
#include <ctype.h>     
#include <stdbool.h> 

#ifdef _WIN32
#include <conio.h> // For Windows
#else
#include <termios.h> // For Linux
#include <unistd.h>
#endif

/**
  * @brief Defines the logged-in user for the current session.
  */
User loggedUser;

/**
 * @brief Creates a new XOR node with the given user data.
 *
 * @param user User data to store in the node.
 * @return Pointer to the newly created XOR node.
 */
XORNode* createXORNode(User user) {
	XORNode* newNode = (XORNode*)malloc(sizeof(XORNode));
	if (!newNode) {
		printf("Memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}
	newNode->user = user;
	newNode->xorPtr = NULL;
	return newNode;
}

/**
 * @brief Inserts a user into the XOR doubly linked list.
 *
 * @param head Pointer to the head of the XOR list.
 * @param user User data to insert.
 * @return Pointer to the updated head of the XOR list.
 */
XORNode* insertXORNode(XORNode* head, User user) {
	XORNode* newNode = createXORNode(user);
	if (!head) {
		return newNode; 
	}

	XORNode* current = head;
	XORNode* prev = NULL;
	XORNode* next;

	while (1) {
		next = (XORNode*)((uintptr_t)prev ^ (uintptr_t)current->xorPtr);
		if (!next) {
			break; 
		}
		prev = current;
		current = next;
	}

	current->xorPtr = (XORNode*)((uintptr_t)prev ^ (uintptr_t)newNode);
	newNode->xorPtr = current;

	return head; 
}

/**
 * @brief Displays all users stored in the XOR doubly linked list, allowing navigation.
 *
 * @param head Pointer to the head of the XOR list.
 */
void viewUsers(const XORNode* head) {
	if (!head) {
		printf("No users available to display.\n");
		return;
	}

	const XORNode* current = head;
	const XORNode* prev = NULL;
	const XORNode* next;
	int choice;

	while (1) {
		clearScreen();
		printf("+---------------------------------------+\n");
		printf("|               USERS                   |\n");
		printf("+---------------------------------------+\n");
		printf("ID: %d\nName: %s %s\nEmail: %s\n", current->user.id, current->user.name, current->user.surname, current->user.email);
		printf("+---------------------------------------+\n");
		printf("1. Next\n2. Previous\n3. Exit\n");
		printf("Enter your choice: ");

		choice = getInput();
		if (choice == -2) {
			handleInputError();
			enterToContinue();
			continue;
		}

		switch (choice) {
		case 1: // Move to the next node
			next = (XORNode*)((uintptr_t)prev ^ (uintptr_t)current->xorPtr);
			if (next) {
				prev = current;
				current = next;
			}
			else {
				printf("You are at the last user.\n");
				enterToContinue();
			}
			break;

		case 2: // Move to the previous node
			if (prev) {
				next = (XORNode*)((uintptr_t)current ^ (uintptr_t)prev->xorPtr);
				current = prev;
				prev = next;
			}
			else {
				printf("You are at the first user.\n");
				enterToContinue();
			}
			break;

		case 3: // Exit
			printf("Exiting user view...\n");
			return;

		default:
			printf("Invalid choice. Please try again.\n");
			enterToContinue();
			break;
		}
	}
}

/**
 * @brief Loads users from a file and constructs an XOR doubly linked list.
 *
 * @param pathFileUsers Path to the file containing user data.
 * @return Pointer to the head of the XOR list.
 */
XORNode* loadUsersIntoXORList(const char* pathFileUsers) {
	FILE* file = fopen(pathFileUsers, "rb");
	if (!file) {
		printf("Failed to open user file.\n");
		return NULL;
	}

	int userCount = 0;
	fread(&userCount, sizeof(int), 1, file);

	XORNode* head = NULL;
	User tempUser;

	for (int i = 0; i < userCount; ++i) {
		fread(&tempUser, sizeof(User), 1, file);
		head = insertXORNode(head, tempUser);
	}

	fclose(file);
	return head;
}

/**
 * @brief Frees the memory allocated for the XOR doubly linked list.
 *
 * @param head Pointer to the head of the XOR list.
 */
void freeXORList(XORNode* head) {
	XORNode* current = head;
	XORNode* prev = NULL;
	XORNode* next;

	while (current) {
		next = (XORNode*)((uintptr_t)prev ^ (uintptr_t)current->xorPtr);
		free(current);
		prev = current;
		current = next;
	}
}

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
	printf("| 5. View Users (ForAdmins)             |\n");
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
	int maxId = 0;

	for (int i = 0; i < userCount; ++i) {
		if (users[i].id > maxId) {
			maxId = users[i].id;
		}
	}

	return maxId + 1; 
}

/**
 * @brief Registers a new user and saves to file.
 *
 * @param user The user to register.
 * @param pathFileUser The file path to save the user data.
 * @return 1 if registration is successful, 0 otherwise.
 */
int registerUser(User user, const char* pathFileUser) {
	FILE* file = fopen(pathFileUser, "rb+");
	int userCount = 0;
	User* users = NULL;

	if (file) {
		fread(&userCount, sizeof(int), 1, file);
		if (userCount > 0) {
			users = (User*)malloc(sizeof(User) * userCount);
			fread(users, sizeof(User), userCount, file);

			for (int i = 0; i < userCount; ++i) {
				if (strcmp(users[i].email, user.email) == 0) {
					printf("User already exists.\n");
					fclose(file);
					free(users);
					enterToContinue();
					return 0;
				}
			}
		}rewind(file);
	}
	else { file = fopen(pathFileUser, "wb"); }

	user.id = getNewUserId(users, userCount);
	userCount++;
	users = (User*)realloc(users, sizeof(User) * userCount);
	users[userCount - 1] = user;

	fwrite(&userCount, sizeof(int), 1, file);
	fwrite(users, sizeof(User), userCount, file);

	printf("User registered successfully\n");

	if (users != NULL) {
		free(users);
	}
	fclose(file);
	enterToContinue();
	return 1;
}

/**
 * @brief Displays the registration menu and prompts for user information.
 *
 * @param pathFileUsers The file path to save the registered user data.
 * @return 1 if registration menu is successful.
 */
int registerUserMenu(const char* pathFileUsers) {
	clearScreen();
	User newUser;

	printf("Enter Name: ");
	fgets(newUser.name, sizeof(newUser.name), stdin);
	newUser.name[strcspn(newUser.name, "\n")] = 0;

	printf("Enter Surname: ");
	fgets(newUser.surname, sizeof(newUser.surname), stdin);
	newUser.surname[strcspn(newUser.surname, "\n")] = 0;

	printf("Enter email: ");
	fgets(newUser.email, sizeof(newUser.email), stdin);
	newUser.email[strcspn(newUser.email, "\n")] = 0;

	printf("Enter password: ");
	fgets(newUser.password, sizeof(newUser.password), stdin);
	newUser.password[strcspn(newUser.password, "\n")] = 0; printf("\n"); if (registerUser(newUser, pathFileUsers)) { return 1; }
	else { printf("Registration failed.\n"); }enterToContinue(); return 1;
}

/**
 * @brief Attempts to log in a user with the provided credentials.
 *
 * @param loginUser The user attempting to log in.
 * @param pathFileUsers The file path to read the user data from.
 * @return 1 if login is successful, 0 otherwise.
 */
int loginUser(User loginUser, const char* pathFileUsers) {
	FILE* file = fopen(pathFileUsers, "rb");
	if (!file) {
		printf("Failed to open user file.\n");
		return 0;
	}

	int userCount = 0;
	fread(&userCount, sizeof(int), 1, file);
	if (userCount == 0) {
		printf("No users registered.\n");
		fclose(file);
		enterToContinue();
		return 0;
	}

	User userFromFile;
	for (int i = 0; i < userCount; i++) {
		fread(&userFromFile, sizeof(User), 1, file);
		if (strcmp(userFromFile.email, loginUser.email) == 0 && strcmp(userFromFile.password, loginUser.password) == 0) {
			printf("Login successful.\n");
			fclose(file);
			enterToContinue();
			loggedUser = userFromFile;
			return 1;
		}
	}

	printf("Incorrect email or password.\n");
	fclose(file);
	enterToContinue();
	return 0;
}

/**
 * @brief Displays the login menu and prompts for user credentials.
 *
 * @param pathFileUsers The file path to read the user data from.
 * @return 1 if login is successful, 0 otherwise.
 */
int loginUserMenu(const char* pathFileUsers) {
	clearScreen();
	User userLogin;

	printf("Enter email: ");
	fgets(userLogin.email, sizeof(userLogin.email), stdin);
	userLogin.email[strcspn(userLogin.email, "\n")] = 0;

	printf("Enter password: ");
	fgets(userLogin.password, sizeof(userLogin.password), stdin);
	userLogin.password[strcspn(userLogin.password, "\n")] = 0;
	printf("\n");

	return loginUser(userLogin, pathFileUsers);
}

/**
 * @brief Handles guest operations menu and actions.
 *
 * @param pathFileIngredients The file path to the ingredients data.
 * @param pathFileRecipes The file path to the recipes data.
 * @return Always returns 0 when exiting guest operations.
 */
int guestOperations(const char* pathFileIngredients, const char* pathFileRecipes) {
	int choice;

	while (1) {
		clearScreen();
		printGuestMenu();

		choice = getInput();

		if (choice == -2) {
			handleInputError();
			enterToContinue();
			continue;
		}

		switch (choice) {
		case 1:
			clearScreen();
			PrintIngredientsToConsole(pathFileIngredients);
			enterToContinue();
			break;

		case 2:
			clearScreen();
			printRecipesToConsole(pathFileRecipes);
			enterToContinue();
			break;

		case 3:
			printf("Exiting Guest Operations...\n");
			return 0;

		default:
			printf("Invalid choice. Please try again.\n");
			enterToContinue();
			break;
		}
	}
}

/**
 * @brief Handles user operations menu and actions.
 *
 * @param pathFileIngredients The file path to the ingredients data.
 * @param pathFileRecipes The file path to the recipes data.
 * @return Always returns 0 when exiting user operations.
 */
int userOperations(const char* pathFileIngredients, const char* pathFileRecipes) {
	int choice;

	while (1) {
		printUserMenu();
		choice = getInput();

		if (choice == -2) {
			handleInputError();
			enterToContinue();
			continue;
		}

		switch (choice) {
		case 1:
			ingredientManagementMenu("ingredient.bin");
			break;

		case 2:
			recipeCostingMenu(pathFileIngredients, pathFileRecipes);
			break;

		case 3:
			AdjustIngredientPriceMenu(pathFileIngredients);
			break;

		case 4:
			budgetPlannerMenu(pathFileRecipes, pathFileIngredients);
			break;

		case 5:
			printf("exiting");
			return 0;
		default:
			clearScreen();
			printf("Invalid choice. Please try again.\n");
			enterToContinue();
			break;
		}
	}
}

/**
 * @brief Handles the main menu and overall program navigation.
 *
 * @param pathFileUsers The file path to the user data.
 * @param pathFileIngredients The file path to the ingredients data.
 * @param pathFileRecipes The file path to the recipes data.
 * @return Always returns 0 when exiting the program.
 */
int mainMenu(const char* pathFileUsers, const char* pathFileIngredients, const char* pathFileRecipes) {
	int choice;
	XORNode* userList = NULL; 

	while (1) {
		clearScreen();
		printMainMenu();

		choice = getInput();

		if (choice == -2) {
			handleInputError();
			enterToContinue();
			continue;
		}

		switch (choice) {
		case 1:
			clearScreen();
			if (loginUserMenu(pathFileUsers)) userOperations(pathFileIngredients, pathFileRecipes);
			break;

		case 2:
			clearScreen();
			registerUserMenu(pathFileUsers);
			break;

		case 3:
			clearScreen();
			guestOperations(pathFileIngredients, pathFileRecipes);
			break;

		case 4:
			printf("Exit Program\n");
			return 0;

		case 5:
			clearScreen();
			userList = loadUsersIntoXORList(pathFileUsers);
			if (!userList) {
				printf("No users found to display.\n");
				enterToContinue();
			}
			else {
				viewUsers(userList);
				freeXORList(userList);
				userList = NULL;
			}
			break;

		default:
			printf("Invalid choice. Please try again.\n");
			enterToContinue();
			break;
		}
	}
}
