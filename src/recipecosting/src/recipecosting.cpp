/**
 * @file recipecosting.cpp
 * @brief Implementation of recipe costing features including recipe creation, editing, costing, and ingredient analysis.
 */
#include "../header/recipecosting.h"
#include "../../userauthentication/header/userauthentication.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "../../priceadjustment/header/priceadjustment.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

 /**
  * @brief Creates a new B+ tree node.
  *
  * @param isLeaf Flag indicating whether the new node is a leaf.
  * @return Pointer to the newly created B+ tree node.
  */
struct BPlusTreeNode* createNode(int isLeaf) {
	struct BPlusTreeNode* newNode = (struct BPlusTreeNode*)malloc(sizeof(struct BPlusTreeNode));
	newNode->isLeaf = isLeaf;
	newNode->keyCount = 0;
	newNode->next = NULL;
	for (int i = 0; i < MAX_RECIPES + 1; ++i) {
		newNode->children[i] = NULL;
		if (i < MAX_RECIPES) newNode->recipes[i] = NULL;
	}
	return newNode;
}

/**
 * @brief Creates a new recipe by prompting the user for recipe details.
 *
 * @param pathFileIngredients File path to load the ingredients.
 * @param pathFileRecipes File path to save the created recipe.
 */
int createRecipe(const char* pathFileIngredients, const char* pathFileRecipes) {
	Recipe recipes[MAX_RECIPES];
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	char recipeName[MAX_NAME_LENGTH];
	int categoryChoice;
	int selectedIngredients[MAX_INGREDIENTS];
	int ingredientCount = 0;
	char input[10];
	int ingredientId;

	// Step 1: Prompt for recipe name
	clearScreen();
	printf("Enter the name of the new recipe: ");
	fgets(recipeName, MAX_NAME_LENGTH, stdin);
	recipeName[strcspn(recipeName, "\n")] = '\0';

	// Step 2: Prompt for recipe category
	clearScreen();
	printf("Select the category of the recipe:\n");
	printf("1) Soup\n");
	printf("2) Appetizer\n");
	printf("3) Main Course\n");
	printf("4) Dessert\n");
	printf("Enter your choice (1-4): ");
	categoryChoice = getInput();

	if (categoryChoice < 1 || categoryChoice > 4) {
		printf("Invalid category choice. Please try again.\n");
		enterToContinue();
		return 0;
	}

	// Step 3: List available ingredients
	clearScreen();
	printf("Available Ingredients:\n");
	PrintIngredientsToConsole(pathFileIngredients);

	// Step 4: Select ingredients by ID
	printf("Enter the ingredient ID to add to the recipe. Type 'done' when finished:\n");
	while (ingredientCount < MAX_INGREDIENTS) {
		printf("Ingredient ID (or 'done'): ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = '\0';

		if (strcmp(input, "done") == 0) {
			break;
		}

		ingredientId = atoi(input);
		if (ingredientId > 0) {
			selectedIngredients[ingredientCount++] = ingredientId;
		}
		else { printf("Invalid input or ingredient limit reached. Please enter a valid ingredient ID or 'done' to finish.\n"); }
	}

	// Step 5: Add the new recipe to the array
	strcpy(recipes[recipeCount].name, recipeName);
	recipes[recipeCount].category = categoryChoice;
	recipes[recipeCount].ingredientCount = ingredientCount;
	memcpy(recipes[recipeCount].ingredients, selectedIngredients, ingredientCount * sizeof(int));
	recipeCount++;

	// Step 6: Save all recipes to file
	saveRecipesToFile(pathFileRecipes, recipes, recipeCount);
	printf("Recipe created successfully!\n");
	enterToContinue();
	return 1;
}

/**
 * @brief Edits an existing recipe by allowing the user to modify its details.
 *
 * @param pathFileRecipes File path to load and save the updated recipe.
 * @param pathFileIngredients File path to load the ingredients data.
 */
int editRecipe(const char* pathFileRecipes, const char* pathFileIngredients) {
	Recipe recipes[MAX_RECIPES];
	Ingredient* ingredientList = loadIngredientsFromFile(pathFileIngredients);
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	if (recipeCount == 0) {
		printf("No recipes available to edit.\n");
		enterToContinue();
		return -1; // Indicate no recipes available
	}

	// Step 1: List recipes
	clearScreen();
	printf("Available Recipes:\n");
	listRecipesName(pathFileRecipes);

	// Step 2: Select recipe by ID
	int recipeId;
	printf("Enter the ID of the recipe you want to edit: ");
	recipeId = getInput();

	if (recipeId < 1 || recipeId > recipeCount) {
		printf("Invalid recipe ID.\n");
		enterToContinue();
		return -2; // Indicate invalid recipe ID
	}

	Recipe* selectedRecipe = &recipes[recipeId - 1]; // Adjusting for 0-based indexing

	// Step 3: Prompt for edit option
	clearScreen();
	printf("Editing Recipe: %s\n", selectedRecipe->name);
	printf("What would you like to edit?\n");
	printf("1) Name\n");
	printf("2) Category\n");
	printf("3) Ingredients\n");
	printf("Enter your choice: ");
	int editChoice = getInput();

	int ingredientChoice; // Declare this variable outside the switch block

	switch (editChoice) {
	case 1:
		// Edit recipe name
		printf("Enter new name: ");
		fgets(selectedRecipe->name, MAX_NAME_LENGTH, stdin);
		selectedRecipe->name[strcspn(selectedRecipe->name, "\n")] = '\0'; // Remove newline character
		break;
	case 2:
		// Edit category
		printf("Select new category:\n");
		printf("1) Soup\n");
		printf("2) Appetizer\n");
		printf("3) Main Course\n");
		printf("4) Dessert\n");
		printf("Enter your choice (1-4): ");
		selectedRecipe->category = getInput();
		if (selectedRecipe->category < 1 || selectedRecipe->category > 4) {
			printf("Invalid category choice.\n");
			enterToContinue();
			return -3; // Indicate invalid category choice
		}
		break;
	case 3:
		// Edit ingredients
		clearScreen();
		printf("Current Ingredients in Recipe:\n");
		for (int i = 0; i < selectedRecipe->ingredientCount; i++) {
			// Find ingredient name from ingredient list
			Ingredient* current = ingredientList;
			while (current != NULL) {
				if (current->id == selectedRecipe->ingredients[i]) {
					printf("ID: %d - %s\n", current->id, current->name);
					break;
				}
				current = current->next;
			}
		}
		printf("\n");

		printf("Would you like to:\n1) Add Ingredients\n2) Remove Ingredients\n");
		printf("Enter your choice: ");
		ingredientChoice = getInput(); // Use the variable here

		if (ingredientChoice == 1) {
			// Add ingredients
			printf("Available Ingredients:\n");
			PrintIngredientsToConsole(pathFileIngredients);
			printf("Enter the ingredient ID to add to the recipe. Type 'done' when finished:\n");

			char input[10];
			while (selectedRecipe->ingredientCount < MAX_INGREDIENTS) {
				printf("Ingredient ID (or 'done'): ");
				fgets(input, sizeof(input), stdin);
				input[strcspn(input, "\n")] = '\0'; // Remove newline character

				if (strcmp(input, "done") == 0) {
					break;
				}

				int ingredientId = atoi(input);
				if (ingredientId > 0) {
					selectedRecipe->ingredients[selectedRecipe->ingredientCount++] = ingredientId;
				}
				else { printf("Invalid input. Please enter a valid ingredient ID or 'done' to finish.\n"); }
			}
		}
		else if (ingredientChoice == 2) {
			// Remove ingredients
			clearScreen();
			printf("Current ingredients in recipe:\n");
			for (int i = 0; i < selectedRecipe->ingredientCount; i++) {
				// Find and display ingredient name
				Ingredient* current = ingredientList;
				while (current != NULL) {
					if (current->id == selectedRecipe->ingredients[i]) {
						printf("ID: %d - %s\n", current->id, current->name);
						break;
					}
					current = current->next;
				}
			}
			printf("\nEnter the ingredient ID to remove from the recipe. Type 'done' when finished:\n");

			char input[10];
			while (1) {
				printf("Ingredient ID (or 'done'): ");
				fgets(input, sizeof(input), stdin);
				input[strcspn(input, "\n")] = '\0'; // Remove newline character

				if (strcmp(input, "done") == 0) {
					break;
				}

				int ingredientId = atoi(input);
				int found = 0;
				for (int i = 0; i < selectedRecipe->ingredientCount; i++) {
					if (selectedRecipe->ingredients[i] == ingredientId) {
						// Shift ingredients to remove the selected one
						for (int j = i; j < selectedRecipe->ingredientCount - 1; j++) {
							selectedRecipe->ingredients[j] = selectedRecipe->ingredients[j + 1];
						}
						selectedRecipe->ingredientCount--;
						found = 1;
						printf("Ingredient %d removed.\n", ingredientId);
						break;
					}
				}
				if (!found) {
					printf("Ingredient %d not found in the recipe.\n", ingredientId);
				}
			}
		}
		else {
			printf("Invalid choice.\n");
			enterToContinue();
			return -4; // Indicate invalid ingredient choice
		}
		break;
	default:
		printf("Invalid choice. Returning to menu.\n");
		enterToContinue();
		return -5; // Indicate invalid edit choice
	}

	// Free ingredient list memory
	while (ingredientList != NULL) {
		Ingredient* temp = ingredientList;
		ingredientList = ingredientList->next;
		free(temp);
	}

	// Step 4: Save updated recipes back to file
	saveRecipesToFile(pathFileRecipes, recipes, recipeCount);
	printf("Recipe updated successfully!\n");
	enterToContinue();
	return 0; // Indicate success
}