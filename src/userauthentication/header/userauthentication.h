/**
 * @file userauthentication.h
 * @brief Header file for user authentication, including function declarations for login, registration,
 * guest operations, and menu management.
 */
#ifndef USERAUTHENTICATION_H
#define USERAUTHENTICATION_H

#include <stdbool.h>
#include <stdio.h>

 /**
  * @brief User struct definition.
  */
typedef struct {
    int id;  /**< Unique user ID. */
    char name[100]; /**< User's first name. */
    char surname[100]; /**< User's surname. */
    char email[100]; /**< User's email address. */
    char password[100]; /**< User's password. */
} User;

typedef struct XORNode {
	User user;               /**< User data stored in the node. */
	struct XORNode* xorPtr; /**< XOR of the previous and next node pointers. */
} XORNode;

// Xor
XORNode* createXORNode(User user);
XORNode* insertXORNode(XORNode* head, User user);
void viewUsers(const XORNode* head);
XORNode* loadUsersIntoXORList(const char* pathFileUsers);
void freeXORList(XORNode* head);

// Tools
void clearScreen();
int enterToContinue();
int handleInputError();
int getInput();

// Print Menus
int printMainMenu();
int printUserMenu();
int printGuestMenu();

// Register-Login
int getNewUserId(User users[], int userCount);
int registerUser(User user, const char* pathFileUser);
int registerUserMenu(const char* pathFileUsers);
int loginUser(User loginUser, const char* pathFileUsers);
int loginUserMenu(const char* pathFileUsers);

// Menus
int guestOperations(const char* pathFileIngredients, const char* pathFileRecipes);
int userOperations(const char* pathFileIngredients, const char* pathFileRecipes);
int mainMenu(const char* pathFileUsers, const char* pathFileIngredients, const char* pathFileRecipes);

#endif // USERAUTHENTICATION_H