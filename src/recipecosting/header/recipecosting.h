/**
 * @file recipecosting.h
 * @brief Header file for recipe costing, including struct definitions and function declarations for
 * creating, editing, and analyzing recipes, as well as managing sparse matrices and B+ trees.
 */
#ifndef RECIPECOSTING_H
#define RECIPECOSTING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
  * @brief Maximum number of ingredients allowed per recipe.
  */
#define MAX_INGREDIENTS 100

/**
   * @brief Maximum length of a recipe name.
   */
#define MAX_NAME_LENGTH 50

/**
	* @brief Maximum number of recipes that can be managed.
	*/
#define MAX_RECIPES 100

/**
	 * @brief Recipe struct definition.
	 */
typedef struct {
	char name[MAX_NAME_LENGTH]; /**< Recipe name. */
	int category; /**< Recipe category (e.g., soup, appetizer, main course, dessert). */
	int ingredients[MAX_INGREDIENTS]; /**< Array of ingredient IDs used in the recipe. */
	int ingredientCount; /**< Number of ingredients in the recipe. */
} Recipe;

/**
 * @brief Sparse Matrix Node struct definition for storing ingredient usage data.
 */
struct SparseMatrixNode {
	int row; /**< Row index in the sparse matrix. */
	int col; /**< Column index in the sparse matrix. */
	double value; /**< Value at the given row and column. */
	SparseMatrixNode* next; /**< Do next in the Spare Matrix Node. */
};

/**
 * @brief B+ Tree Node struct definition for managing recipes.
 */
struct BPlusTreeNode {
	int isLeaf; /**< Flag indicating if the node is a leaf. */
	int keyCount; /**< Number of keys in the node. */
	int keys[MAX_RECIPES]; /**< Array of keys in the node. */
	struct BPlusTreeNode* children[MAX_RECIPES + 1]; /**< Array of pointers to child nodes. */
	Recipe* recipes[MAX_RECIPES]; /**< Array of pointers to recipes. */
	struct BPlusTreeNode* next; /**< Pointer to the next node in the linked list of leaf nodes. */
};

// Function declarations
struct BPlusTreeNode* createNode(int isLeaf);
struct BPlusTreeNode* findParent(struct BPlusTreeNode* cursor, struct BPlusTreeNode* child);

SparseMatrixNode* createSparseMatrixNode(int row, int col, double value);
void insertSparseMatrixNode(SparseMatrixNode** head, int row, int col, double value);
void freeSparseMatrix(SparseMatrixNode* head);

int printRecipesToConsole(const char* pathFileRecipes);
int listRecipesName(const char* pathFileRecipes);
int loadRecipesFromFile(const char* pathFileRecipes, Recipe recipes[], int maxRecipes);
void saveRecipesToFile(const char* pathFileRecipes, Recipe recipes[], int recipeCount);

int recipeCostingMenu(const char* pathFileIngredients, const char* pathFileRecipes);
int createRecipe(const char* pathFileIngredients, const char* pathFileRecipes);
int editRecipe(const char* pathFileRecipes, const char* pathFileIngredients);
int calculateRecipeCost(const char* pathFileRecipes, const char* pathFileIngredients);

void insertInternal(int key, Recipe* recipe, struct BPlusTreeNode* cursor, struct BPlusTreeNode** root);
void insert(int key, Recipe* recipe, struct BPlusTreeNode** root);
void search(int key, struct BPlusTreeNode* root);
void traverseRecipesBFS(const char* pathFileRecipes, const char* pathFileIngredients);
void traverseRecipesDFS(const char* pathFileRecipes, const char* pathFileIngredients);
void analyzeIngredientUsage(const char* pathFileRecipes, const char* pathFileIngredients);

#endif // RECIPECOSTING_H
