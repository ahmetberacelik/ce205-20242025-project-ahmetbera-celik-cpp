/**
 * @file costcalculatorapp.cpp
 * @brief A simple program to demonstrate the usage of the userauthentication model class.
 *
 * This program process infix notations and calculate operations
 *
 */

 // Standard Libraries
#include "stdio.h"
#include "../../userauthentication/header/userauthentication.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"// Adjust this include path based on your project structure

/**
 * @brief Main program function
 *
 * This function is the main entry point of the program. It defines the necessary file paths
 * for user authentication and ingredient management operations and starts the main menu.
 *
 * @return int Program exit status (0: success)
 */
int main() {
	const char* pathFileUsers = "users.bin";
	const char* pathFileIngredients = "ingredient.bin";
	const char* pathFileRecipes = "recipes.bin";

	mainMenu(pathFileUsers, pathFileIngredients, pathFileRecipes);
	return 0;
}