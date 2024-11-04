/**
 * @file try.c
 * @brief Source file containing budget planning and meal planning functions
 */
#include "../../recipecosting/header/recipecosting.h"
#include "../../userauthentication/header/userauthentication.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "../../budgetplanner/header/budgetplanner.h"
#include <stdio.h>
#include <stdlib.h>

 /**
  * @brief Plans meals and updates the budget
  * @param pathFileRecipes Path to the file containing recipes
  * @param pathFileIngredients Path to the file containing ingredients
  * @param budget Pointer to the current budget
  * @return Returns 1 if successful, 0 if failed
  */
int planMeals(const char* pathFileRecipes, const char* pathFileIngredients, double* budget) {
    Recipe recipes[MAX_RECIPES];
    Ingredient* ingredientList = loadIngredientsFromFile(pathFileIngredients);
    int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

    if (recipeCount == 0) {
        printf("\nNo recipes found for meal planning.\n");
        enterToContinue();
        return 0;
    }

    double totalCost = 0;
    int selectedRecipeIds[MAX_RECIPES];
    int selectedRecipeCount = 0;
    char input[10];
    int recipeId;

    // Step 1: List available recipes
    clearScreen();
    printf("\n=== Available Recipes ===\n\n");
    listRecipesWithPrices(pathFileRecipes, pathFileIngredients);

    // Step 2: Allow user to select recipes by ID
    printf("\n=== Recipe Selection ===\n");
    printf("Enter recipe ID to add to meal plan. Type 'done' to finish:\n\n");

    while (selectedRecipeCount < MAX_RECIPES) {
        printf("Recipe ID (or 'done'): ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "done") == 0) {
            break;
        }

        recipeId = atoi(input);
        if (recipeId < 1 || recipeId > recipeCount) {
            printf("\nInvalid recipe ID. Please enter a valid ID or type 'done' to finish.\n\n");
            continue;
        }

        Recipe* selectedRecipe = &recipes[recipeId - 1]; // Adjusting for 0-based indexing

        // Calculate the cost of the selected recipe
        double recipeCost = 0;
        Ingredient* currentIngredient = ingredientList;
        for (int i = 0; i < selectedRecipe->ingredientCount; i++) {
            while (currentIngredient != NULL) {
                if (currentIngredient->id == selectedRecipe->ingredients[i]) { recipeCost += currentIngredient->price; break; }
                currentIngredient = currentIngredient->next;
            }
            currentIngredient = ingredientList; // Reset pointer to beginning of ingredient list
        }

        // Check if the recipe can be added to the budget
        if (totalCost + recipeCost > *budget) { printf("\nCannot add '%s' due to budget constraints.\n\n", selectedRecipe->name); }
        else {
            selectedRecipeIds[selectedRecipeCount++] = recipeId;
            totalCost += recipeCost;
            printf("\n'%s' added to meal plan.\n", selectedRecipe->name);
            printf("Current total cost: %.2f TL\n", totalCost);
            double remainingBudget = *budget - totalCost;
            printf("Remaining budget after adding '%s': %.2f TL\n\n", selectedRecipe->name, remainingBudget);
        }
    }

    // Display the total cost and remaining budget
    double remainingBudget = *budget - totalCost;
    printf("\n=== Meal Plan Summary ===\n");
    printf("Total cost of selected recipes: %.2f TL\n", totalCost);
    printf("Remaining budget: %.2f TL\n\n", remainingBudget);

    // Update the budget
    *budget = remainingBudget;

    // Clean up ingredient list memory
    while (ingredientList != NULL) {
        Ingredient* temp = ingredientList;
        ingredientList = ingredientList->next;
        free(temp);
    }

    enterToContinue();
    return 1;
}
