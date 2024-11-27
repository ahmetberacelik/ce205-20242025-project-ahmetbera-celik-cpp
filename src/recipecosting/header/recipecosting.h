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
typedef struct SparseMatrixNode {
	int row; /**< Row index in the sparse matrix. */
	int col; /**< Column index in the sparse matrix. */
	double value; /**< Value at the given row and column. */
	struct SparseMatrixNode* next; /**< Pointer to the next node in the Sparse Matrix. */
} SparseMatrixNode;

/**
 * @brief B+ Tree Node struct definition for managing recipes.
 */
typedef struct BPlusTreeNode {
	int isLeaf; /**< Flag indicating if the node is a leaf. */
	int keyCount; /**< Number of keys in the node. */
	int keys[MAX_RECIPES]; /**< Array of keys in the node. */
	struct BPlusTreeNode* children[MAX_RECIPES + 1]; /**< Array of pointers to child nodes. */
	Recipe* recipes[MAX_RECIPES]; /**< Array of pointers to recipes. */
	struct BPlusTreeNode* next; /**< Pointer to the next node in the linked list of leaf nodes. */
} BPlusTreeNode;

/**
 * @brief Node struct definition for graph representation.
 */
typedef struct Node {
	int recipeIndex; /**< Recipe index in the graph. */
	struct Node* next; /**< Pointer to the next node. */
} Node;

/**
 * @brief Graph struct definition for representing relationships between recipes.
 */
typedef struct Graph {
	Node* adjList[MAX_RECIPES]; /**< Adjacency list for each recipe. */
	int visited[MAX_RECIPES]; /**< Array to track visited nodes. */
	int discoveryTime[MAX_RECIPES]; /**< Discovery time of each node. */
	int lowLink[MAX_RECIPES]; /**< Low link values for Tarjan's SCC. */
	int inStack[MAX_RECIPES]; /**< Stack membership flag for each node. */
	int time; /**< Timer for Tarjan's SCC algorithm. */
	int stack[MAX_RECIPES]; /**< Stack to store nodes. */
	int stackTop; /**< Top index of the stack. */
} Graph;

// Function declarations
BPlusTreeNode* createNode(int isLeaf);
BPlusTreeNode* findParent(BPlusTreeNode* cursor, BPlusTreeNode* child);

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

void insertInternal(int key, Recipe* recipe, BPlusTreeNode* cursor, BPlusTreeNode** root);
void insert(int key, Recipe* recipe, BPlusTreeNode** root);
void search(int key, BPlusTreeNode* root);
void traverseRecipesBFS(const char* pathFileRecipes, const char* pathFileIngredients);
void traverseRecipesDFS(const char* pathFileRecipes, const char* pathFileIngredients);
void analyzeIngredientUsage(const char* pathFileRecipes, const char* pathFileIngredients);

void initializeGraph(Graph* graph, int nodeCount);
void addEdge(Graph* graph, int src, int dest);
void push(Graph* graph, int node);
int pop(Graph* graph);
void tarjanSCCDetailed(Graph* graph, int node, Recipe recipes[]);
void buildGraphFromRecipes(Graph* graph, Recipe recipes[], int recipeCount);
void analyzeSCC(const char* pathFileRecipes);

#endif // RECIPECOSTING_H
