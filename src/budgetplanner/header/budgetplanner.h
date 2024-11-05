/**
 * @file BUDGETPLANNER.h
 * @brief Header file containing budget planning functionality
 *
 * This header provides functions for meal planning, recipe listing,
 * budget management and menu operations
 */
#ifndef BUDGETPLANNER_H
#define BUDGETPLANNER_H

 /**
  * @brief Plans meals based on recipe and ingredient files
  * @param pathFileRecipes Path to the recipes file
  * @param pathFileIngredients Path to the ingredients file
  * @return Returns 0 on success, non-zero on failure
  */
int planMeals(const char* pathFileRecipes, const char* pathFileIngredients);

/**
 * @brief Lists all recipes with their calculated prices
 * @param pathFileRecipes Path to the recipes file
 * @param pathFileIngredients Path to the ingredients file
 * @return Returns 0 on success, non-zero on failure
 */
int listRecipesWithPrices(const char* pathFileRecipes, const char* pathFileIngredients);

/**
 * @brief Displays and handles the budget planner menu
 * @param pathFileRecipes Path to the recipes file
 * @param pathFileIngredients Path to the ingredients file
 * @return Returns 0 on success, non-zero on failure
 */
int budgetPlannerMenu(const char* pathFileRecipes, const char* pathFileIngredients);

/**
 * @brief Displays the current budget information
 * @param budget The current budget amount
 * @return Returns 0 on success, non-zero on failure
 */
int viewBudget(double budget);

#endif // BUDGETPLANNER_H