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
#include <limits.h>
#include <unordered_map>

/**
  * @brief Initializes the graph with the given number of nodes.
  *
  * This function initializes the adjacency list, visited nodes, discovery times,
  * low links, and stack-related attributes of the graph.
  *
  * @param graph Pointer to the Graph structure to be initialized.
  * @param nodeCount The total number of nodes in the graph.
  */
void initializeGraph(Graph* graph, int nodeCount) {
	for (int i = 0; i < nodeCount; i++) {
		graph->adjList[i] = NULL;
		graph->visited[i] = 0;
		graph->discoveryTime[i] = -1;
		graph->lowLink[i] = -1;
		graph->inStack[i] = 0;
	}
	graph->time = 0;
	graph->stackTop = -1;
}

/**
 * @brief Adds a directed edge from the source node to the destination node.
 *
 * This function creates a new edge from the source node to the destination node,
 * linking the recipes in the graph structure.
 *
 * @param graph Pointer to the Graph structure.
 * @param src The source node index.
 * @param dest The destination node index.
 */
void addEdge(Graph* graph, int src, int dest) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->recipeIndex = dest;
	newNode->next = graph->adjList[src];
	graph->adjList[src] = newNode;
}

/**
 * @brief Pushes a node onto the stack.
 *
 * This function adds the given node to the stack and marks it as in-stack.
 *
 * @param graph Pointer to the Graph structure.
 * @param node The node to be pushed onto the stack.
 */
void push(Graph* graph, int node) {
	graph->stack[++graph->stackTop] = node;
	graph->inStack[node] = 1;
}

/**
 * @brief Pops a node from the stack.
 *
 * This function removes the top node from the stack and returns it.
 * It also marks the node as no longer in the stack.
 *
 * @param graph Pointer to the Graph structure.
 * @return The node that was popped from the stack.
 */
int pop(Graph* graph) {
	int node = graph->stack[graph->stackTop--];
	graph->inStack[node] = 0;
	return node;
}

/**
 * @brief Performs a detailed Tarjan's Strongly Connected Components (SCC) analysis.
 *
 * This function uses Tarjan's algorithm to find SCCs in a graph and prints the
 * recipes in each SCC. It recursively explores the graph to determine the
 * discovery times and low links of nodes.
 *
 * @param graph Pointer to the Graph structure.
 * @param node The starting node for the analysis.
 * @param recipes Array of Recipe structures containing recipe data.
 */
void tarjanSCCDetailed(Graph* graph, int node, Recipe recipes[]) {
	graph->discoveryTime[node] = graph->lowLink[node] = graph->time++;
	push(graph, node);

	Node* current = graph->adjList[node];
	while (current != NULL) {
		int neighbor = current->recipeIndex;
		if (graph->discoveryTime[neighbor] == -1) {
			tarjanSCCDetailed(graph, neighbor, recipes);
			graph->lowLink[node] = (graph->lowLink[node] < graph->lowLink[neighbor]) ? graph->lowLink[node] : graph->lowLink[neighbor];
		}
		else if (graph->inStack[neighbor]) {
			graph->lowLink[node] = (graph->lowLink[node] < graph->discoveryTime[neighbor]) ? graph->lowLink[node] : graph->discoveryTime[neighbor];
		}
		current = current->next;
	}

	// If node is the root of an SCC
	if (graph->lowLink[node] == graph->discoveryTime[node]) {
		printf("Recipes in this SCC:\n");
		int sccNode;
		do {
			sccNode = pop(graph);
			printf("- Recipe %d: %s (Category: %d, Ingredients: ",
				sccNode + 1, // Recipe ID (1-based indexing)
				recipes[sccNode].name,
				recipes[sccNode].category);
			for (int i = 0; i < recipes[sccNode].ingredientCount; i++) {
				printf("%d ", recipes[sccNode].ingredients[i]); // Ingredient IDs
			}
			printf(")\n");
		} while (sccNode != node);
		printf("\n");
	}
}

/**
 * @brief Builds a graph from the provided recipes.
 *
 * This function constructs a graph where nodes represent recipes, and edges are
 * added between nodes that share ingredients or belong to the same category.
 *
 * @param graph Pointer to the Graph structure to be built.
 * @param recipes Array of Recipe structures containing recipe data.
 * @param recipeCount The number of recipes in the array.
 */
void buildGraphFromRecipes(Graph* graph, Recipe recipes[], int recipeCount) {
	for (int i = 0; i < recipeCount; i++) {
		for (int j = 0; j < recipeCount; j++) {
			if (i != j) {
				// Check if recipes i and j share ingredients or belong to the same category
				if (recipes[i].category == recipes[j].category) {
					addEdge(graph, i, j); // Add edge for same category
				}
				else {
					for (int k = 0; k < recipes[i].ingredientCount; k++) {
						for (int l = 0; l < recipes[j].ingredientCount; l++) {
							if (recipes[i].ingredients[k] == recipes[j].ingredients[l]) {
								addEdge(graph, i, j); // Add edge for shared ingredient
								break;
							}
						}
					}
				}
			}
		}
	}
}

/**
 * @brief Analyzes the Strongly Connected Components (SCCs) of recipes in a graph.
 *
 * This function loads recipes from a file, builds a graph, and then applies
 * Tarjan's SCC algorithm to identify and print SCCs of recipes that are
 * interconnected through shared ingredients or belong to the same category.
 *
 * @param pathFileRecipes Path to the file containing recipe data.
 */
void analyzeSCC(const char* pathFileRecipes) {
	Recipe recipes[MAX_RECIPES];
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	if (recipeCount == 0) {
		printf("No recipes available for analysis.\n");
		enterToContinue();
		return;
	}

	// Initialize the graph
	Graph graph;
	initializeGraph(&graph, recipeCount);

	// Build the graph from recipes
	buildGraphFromRecipes(&graph, recipes, recipeCount);

	// Run Tarjan's SCC algorithm
	printf("+--------------------------------------+\n");
	printf("|   STRONGLY CONNECTED COMPONENTS      |\n");
	printf("+--------------------------------------+\n");

	int sccCounter = 0; // Counter for SCC groups
	for (int i = 0; i < recipeCount; i++) {
		if (graph.discoveryTime[i] == -1) {
			printf("\nSCC Group %d:\n", ++sccCounter);
			tarjanSCCDetailed(&graph, i, recipes); // Call detailed SCC function
		}
	}

	enterToContinue();
}

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

/**
 * @brief Calculates the cost of a recipe by summing the prices of its ingredients.
 *
 * @param pathFileRecipes File path to load the recipes.
 * @param pathFileIngredients File path to load the ingredients data.
 */
int calculateRecipeCost(const char* pathFileRecipes, const char* pathFileIngredients) {
	Recipe recipes[MAX_RECIPES];
	Ingredient* ingredientList = loadIngredientsFromFile(pathFileIngredients);
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	if (recipeCount == 0) {
		printf("No recipes available to calculate cost.\n");
		enterToContinue();
		return -1; // Indicate no recipes available
	}

	// Step 1: List recipes
	clearScreen();
	printf("Available Recipes:\n");
	listRecipesName(pathFileRecipes);

	// Step 2: Select recipe by ID
	int recipeId;
	printf("Enter the ID of the recipe to calculate cost: ");
	recipeId = getInput();

	if (recipeId < 1 || recipeId > recipeCount) {
		printf("Invalid recipe ID.\n");
		enterToContinue();
		return -2; // Indicate invalid recipe ID
	}

	Recipe* selectedRecipe = &recipes[recipeId - 1]; // Adjusting for 0-based indexing

	// Step 3: Calculate total cost using Sparse Matrix
	double totalCost = 0.0;
	SparseMatrixNode* costMatrix = NULL;
	Ingredient* currentIngredient = ingredientList;
	for (int i = 0; i < selectedRecipe->ingredientCount; i++) {
		while (currentIngredient != NULL) {
			if (currentIngredient->id == selectedRecipe->ingredients[i]) {
				insertSparseMatrixNode(&costMatrix, recipeId, currentIngredient->id, currentIngredient->price);
				totalCost += currentIngredient->price;
				break;
			}
			currentIngredient = currentIngredient->next;
		}
		currentIngredient = ingredientList; // Reset pointer to beginning of ingredient list
	}

	printf("The total cost of the recipe '%s' is: $%.2f\n", selectedRecipe->name, totalCost);
	enterToContinue();

	// Free ingredient list memory
	while (ingredientList != NULL) {
		Ingredient* temp = ingredientList;
		ingredientList = ingredientList->next;
		free(temp);
	}

	// Free sparse matrix memory
	freeSparseMatrix(costMatrix);
	return 0; // Indicate success
}

/**
 * @brief Lists the names of all recipes.
 *
 * @param pathFileRecipes File path to load the recipes from.
 */
int listRecipesName(const char* pathFileRecipes) {
	Recipe recipes[MAX_RECIPES];
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	if (recipeCount == 0) {
		printf("No recipes found.\n");
		return -1; // Indicate no recipes found
	}

	printf("+--------------------+\n");
	printf("| Available Recipes  |\n");
	printf("+--------------------+\n");
	for (int i = 0; i < recipeCount; i++) {
		printf("%d) %s\n", i + 1, recipes[i].name); // 1-based index for user
	}
	printf("+--------------------+\n");

	return recipeCount; // Return the number of recipes listed
}

 /**
  * @brief Prints all recipes to the console.
  *
  * @param pathFileRecipes File path to load the recipes from.
  */
int printRecipesToConsole(const char* pathFileRecipes) {
	FILE* file = fopen(pathFileRecipes, "rb");
	if (!file) {
		printf("Error opening recipe file.\n");
		return -1; // Indicate error opening file
	}

	Ingredient* ingredientList = loadIngredientsFromFile("ingredient.bin");
	if (!ingredientList) {
		printf("Error loading ingredients.\n");
		fclose(file);
		return -2; // Indicate error loading ingredients
	}

	int recipeCount;
	fread(&recipeCount, sizeof(int), 1, file); // Read the number of recipes

	printf("\nAvailable Recipes:\n");
	for (int i = 0; i < recipeCount; i++) {
		Recipe recipe;

		// Read the recipe name
		fread(recipe.name, sizeof(char), MAX_NAME_LENGTH, file);

		// Read the category
		fread(&recipe.category, sizeof(int), 1, file);

		// Read the ingredient count
		fread(&recipe.ingredientCount, sizeof(int), 1, file);

		// Read the ingredient IDs
		fread(recipe.ingredients, sizeof(int), recipe.ingredientCount, file);

		// Display the recipe name
		printf("%d) %s\n", i + 1, recipe.name);

		// Display the recipe category
		printf("   Category: ");
		switch (recipe.category) {
		case 1:
			printf("Soup\n");
			break;
		case 2:
			printf("Appetizer\n");
			break;
		case 3:
			printf("Main Course\n");
			break;
		case 4:
			printf("Dessert\n");
			break;
		default:
			printf("Unknown\n");
			break;
		}

		// Display the ingredients
		printf("   Ingredients: ");
		for (int j = 0; j < recipe.ingredientCount; j++) {
			int ingredientId = recipe.ingredients[j];
			Ingredient* current = ingredientList;
			while (current != NULL) {
				if (current->id == ingredientId) {
					printf("%s", current->name);
					if (j < recipe.ingredientCount - 1) {
						printf(", ");
					}
					break;
				}
				current = current->next;
			}
		}
		printf("\n");
	}

	fclose(file);

	// Free ingredient list memory
	while (ingredientList != NULL) {
		Ingredient* temp = ingredientList;
		ingredientList = ingredientList->next;
		free(temp);
	}

	return 0; // Indicate success
}

/**
 * @brief Loads recipes from a binary file.
 *
 * @param pathFileRecipes File path to load the recipes from.
 * @param recipes Array to store the loaded recipes.
 * @param maxRecipes Maximum number of recipes to load.
 * @return The number of recipes loaded.
 */
int loadRecipesFromFile(const char* pathFileRecipes, Recipe recipes[], int maxRecipes) {
	FILE* file = fopen(pathFileRecipes, "rb");
	if (!file) {
		printf("Error: Could not open file %s.\n", pathFileRecipes);
		return 0; // Indicate failure to open file
	}

	int recipeCount = 0;
	fread(&recipeCount, sizeof(int), 1, file); // Read the total number of recipes

	if (recipeCount > maxRecipes) {
		printf("Warning: File contains more recipes than allowed (%d > %d). Truncating.\n", recipeCount, maxRecipes);
		recipeCount = maxRecipes; // Prevent overflow
	}

	for (int i = 0; i < recipeCount; i++) {
		// Read the recipe name
		fread(recipes[i].name, sizeof(char), MAX_NAME_LENGTH, file);

		// Read the category
		fread(&recipes[i].category, sizeof(int), 1, file);

		// Read the ingredient count
		fread(&recipes[i].ingredientCount, sizeof(int), 1, file);

		// Read the ingredient IDs
		fread(recipes[i].ingredients, sizeof(int), recipes[i].ingredientCount, file);
	}

	fclose(file);
	return recipeCount;
}

/**
 * @brief Saves all recipes to a binary file.
 *
 * @param pathFileRecipes File path to save the recipes.
 * @param recipes Array of recipes to be saved.
 * @param recipeCount Number of recipes to save.
 */
void saveRecipesToFile(const char* pathFileRecipes, Recipe recipes[], int recipeCount) {
	FILE* file = fopen(pathFileRecipes, "wb");
	if (!file) {
		printf("Error opening recipe file for writing.\n");
		return;
	}

	// Write the total number of recipes first
	fwrite(&recipeCount, sizeof(recipeCount), 1, file);

	// Write each recipe
	for (int i = 0; i < recipeCount; i++) {
		// Write the recipe name
		fwrite(recipes[i].name, sizeof(char), MAX_NAME_LENGTH, file);

		// Write the category
		fwrite(&recipes[i].category, sizeof(int), 1, file);

		// Write the ingredient count
		fwrite(&recipes[i].ingredientCount, sizeof(int), 1, file);

		// Write the ingredient IDs
		fwrite(recipes[i].ingredients, sizeof(int), recipes[i].ingredientCount, file);
	}

	fclose(file);
}

/**
 * @brief Inserts a key and recipe into the B+ tree.
 *
 * @param key The key to insert.
 * @param recipe Pointer to the recipe to insert.
 * @param root Pointer to the root of the B+ tree.
 */
void insert(int key, Recipe* recipe, struct BPlusTreeNode** root) {
	struct BPlusTreeNode* cursor = *root;
	if (cursor->keyCount == MAX_RECIPES) {
		struct BPlusTreeNode* newRoot = createNode(0); newRoot->children[0] = *root; insertInternal(key, recipe, *root, root); *root = newRoot;
	}
	else {
		insertInternal(key, recipe, cursor, root);
	}
}

/**
 * @brief Inserts a key and recipe into the internal nodes of the B+ tree.
 *
 * @param key The key to insert.
 * @param recipe Pointer to the recipe to insert.
 * @param cursor Pointer to the current B+ tree node.
 * @param root Pointer to the root of the B+ tree.
 */
void insertInternal(int key, Recipe* recipe, struct BPlusTreeNode* cursor, struct BPlusTreeNode** root) {
	if (cursor->keyCount < MAX_RECIPES) {
		// Insert into non-full node
		int i = cursor->keyCount - 1;
		while (i >= 0 && cursor->keys[i] > key) {
			cursor->keys[i + 1] = cursor->keys[i];
			cursor->recipes[i + 1] = cursor->recipes[i];
			i--;
		}
		cursor->keys[i + 1] = key;
		cursor->recipes[i + 1] = recipe;
		cursor->keyCount++;
	}
	else {
		// Split full node
		int tempKeys[MAX_RECIPES + 1];
		Recipe* tempRecipes[MAX_RECIPES + 1];
		for (int i = 0; i < MAX_RECIPES; ++i) {
			tempKeys[i] = cursor->keys[i];
			tempRecipes[i] = cursor->recipes[i];
		}

		int i = MAX_RECIPES - 1;
		while (i >= 0 && tempKeys[i] > key) {
			tempKeys[i + 1] = tempKeys[i]; tempRecipes[i + 1] = tempRecipes[i]; i--;
		}
		tempKeys[i + 1] = key;
		tempRecipes[i + 1] = recipe;

		struct BPlusTreeNode* newLeaf = createNode(1);
		cursor->keyCount = (MAX_RECIPES + 1) / 2;
		newLeaf->keyCount = MAX_RECIPES + 1 - cursor->keyCount;

		for (int i = 0; i < cursor->keyCount; ++i) {
			cursor->keys[i] = tempKeys[i];
			cursor->recipes[i] = tempRecipes[i];
		}
		for (int i = 0; i < newLeaf->keyCount; ++i) {
			newLeaf->keys[i] = tempKeys[cursor->keyCount + i];
			newLeaf->recipes[i] = tempRecipes[cursor->keyCount + i];
		}

		newLeaf->next = cursor->next;
		cursor->next = newLeaf;

		// Insert new key to parent
		if (cursor == *root) {
			struct BPlusTreeNode* newRoot = createNode(0);
			newRoot->keys[0] = newLeaf->keys[0];
			newRoot->children[0] = cursor;
			newRoot->children[1] = newLeaf;
			newRoot->keyCount = 1;
			*root = newRoot;
		}
		else {
			insertInternal(newLeaf->keys[0], NULL, findParent(*root, cursor), root);
		}
	}
}

/**
 * @brief Finds the parent of a given B+ tree node.
 *
 * @param cursor Pointer to the current B+ tree node.
 * @param child Pointer to the child node whose parent is to be found.
 * @return Pointer to the parent node, or NULL if not found.
 */
struct BPlusTreeNode* findParent(struct BPlusTreeNode* cursor, struct BPlusTreeNode* child) {
	if (cursor->isLeaf || cursor->children[0]->isLeaf) return NULL;
	for (int i = 0; i < cursor->keyCount + 1; i++) {
		if (cursor->children[i] == child) return cursor;
		struct BPlusTreeNode* parent = findParent(cursor->children[i], child);
		if (parent != NULL) return parent;
	}
	return NULL;
}

/**
 * @brief Searches for a recipe by key in the B+ tree.
 *
 * @param key The key to search for.
 * @param root Pointer to the root of the B+ tree.
 */
void search(int key, struct BPlusTreeNode* root) {
	struct BPlusTreeNode* cursor = root;
	while (!cursor->isLeaf) {
		for (int i = 0; i < cursor->keyCount; ++i) {
			if (key < cursor->keys[i]) {
				cursor = cursor->children[i];
				break;
			}
			if (i == cursor->keyCount - 1) cursor = cursor->children[i + 1];
		}
	}
	int found = 0;
	while (cursor != NULL) {
		for (int i = 0; i < cursor->keyCount; ++i) {
			if (cursor->keys[i] == key) {
				printf("Recipe found: %s\n", cursor->recipes[i]->name);
				found = 1;
			}
		}
		cursor = cursor->next;
	}
	if (!found) {
		printf("Recipe not found\n");
	}
}

/**
 * @brief Creates a new sparse matrix node.
 *
 * @param row Row index in the sparse matrix.
 * @param col Column index in the sparse matrix.
 * @param value Value to be stored in the sparse matrix.
 * @return Pointer to the newly created sparse matrix node.
 */
SparseMatrixNode* createSparseMatrixNode(int row, int col, double value) {
	SparseMatrixNode* newNode = (SparseMatrixNode*)malloc(sizeof(SparseMatrixNode));
	newNode->row = row;
	newNode->col = col;
	newNode->value = value;
	newNode->next = NULL;
	return newNode;
}

/**
 * @brief Inserts a new node into the sparse matrix.
 *
 * @param head Pointer to the head of the sparse matrix linked list.
 * @param row Row index for the new node.
 * @param col Column index for the new node.
 * @param value Value to be stored in the new node.
 */
void insertSparseMatrixNode(SparseMatrixNode** head, int row, int col, double value) {
	SparseMatrixNode* newNode = createSparseMatrixNode(row, col, value);
	newNode->next = *head;
	*head = newNode;
}

/**
 * @brief Frees the memory used by the sparse matrix.
 *
 * @param head Pointer to the head of the sparse matrix linked list.
 */
void freeSparseMatrix(SparseMatrixNode* head) {
	while (head != NULL) {
		SparseMatrixNode* temp = head;
		head = head->next;
		free(temp);
	}
}

/**
 * @brief Traverses recipes using Breadth-First Search (BFS) and prints ingredient usage.
 *
 * @param pathFileRecipes File path to load the recipes.
 * @param pathFileIngredients File path to load the ingredients data.
 */
void traverseRecipesBFS(const char* pathFileRecipes, const char* pathFileIngredients) {
	Recipe recipes[MAX_RECIPES];
	Ingredient* ingredientList = loadIngredientsFromFile(pathFileIngredients);
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	if (recipeCount == 0) {
		printf("No recipes available."); enterToContinue(); return;
	}

	std::queue<int> q;
	std::unordered_map<int, std::unordered_map<int, double>> ingredientUsage;

	// Enqueue all recipes by their ID
	for (int i = 0; i < recipeCount; ++i) {
		q.push(i);
	}

	// Traverse using BFS
	while (!q.empty()) {
		int recipeId = q.front();
		q.pop();
		Recipe currentRecipe = recipes[recipeId];

		// Count the ingredients used in this recipe
		for (int i = 0; i < currentRecipe.ingredientCount; i++) {
			int ingredientId = currentRecipe.ingredients[i];
			ingredientUsage[recipeId][ingredientId]++;
		}
	}

	// Display the results
	for (const auto& recipeEntry : ingredientUsage) {
		int recipeId = recipeEntry.first;
		for (const auto& ingredientEntry : recipeEntry.second) {
			int ingredientId = ingredientEntry.first;
			double count = ingredientEntry.second;
			printf("-------------------------------------------\n");
			printf("Recipe %d uses Ingredient %d - %0.2f times\n", recipeId, ingredientId, count);
			printf("-------------------------------------------\n");
		}
	}
}

/**
 * @brief Traverses recipes using Depth-First Search (DFS) and prints ingredient usage.
 *
 * @param pathFileRecipes File path to load the recipes.
 * @param pathFileIngredients File path to load the ingredients data.
 */
void traverseRecipesDFS(const char* pathFileRecipes, const char* pathFileIngredients) {
	Recipe recipes[MAX_RECIPES];
	Ingredient* ingredientList = loadIngredientsFromFile(pathFileIngredients);
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);

	if (recipeCount == 0) {
		printf("No recipes available."); enterToContinue(); return;
	}

	std::stack<int> s;
	std::unordered_map<int, std::unordered_map<int, double>> ingredientUsage;

	// Push all recipes by their ID
	for (int i = 0; i < recipeCount; ++i) {
		s.push(i);
	}

	// Traverse using DFS
	while (!s.empty()) {
		int recipeId = s.top();
		s.pop();
		Recipe currentRecipe = recipes[recipeId];

		// Count the ingredients used in this recipe
		for (int i = 0; i < currentRecipe.ingredientCount; i++) {
			int ingredientId = currentRecipe.ingredients[i];
			ingredientUsage[recipeId][ingredientId]++;
		}
	}

	// Display the results
	for (const auto& recipeEntry : ingredientUsage) {
		int recipeId = recipeEntry.first;
		for (const auto& ingredientEntry : recipeEntry.second) {
			int ingredientId = ingredientEntry.first;
			double count = ingredientEntry.second;
			printf("-------------------------------------------\n");
			printf("Recipe %d uses Ingredient %d - %0.2f times\n", recipeId, ingredientId, count);
			printf("-------------------------------------------\n");
		}
	}
}

/**
 * @brief Analyzes ingredient usage across all recipes using BFS or DFS.
 *
 * @param pathFileRecipes File path to load the recipes.
 * @param pathFileIngredients File path to load the ingredients data.
 */
void analyzeIngredientUsage(const char* pathFileRecipes, const char* pathFileIngredients) {
	int choice;
	clearScreen();
	printf("How would you like to analyze the ingredients used in all recipes?\n");
	printf("1) BFS (Breadth-First Search)\n");
	printf("2) DFS (Depth-First Search)\n");
	printf("Enter your choice (1-2): ");
	choice = getInput();

	if (choice == 1) {
		clearScreen();
		traverseRecipesBFS(pathFileRecipes, pathFileIngredients);
	}
	else if (choice == 2) {
		clearScreen();
		traverseRecipesDFS(pathFileRecipes, pathFileIngredients);
	}
	else {
		printf("Invalid choice. Returning to menu.\n");
	}
	enterToContinue();
}

/**
 * @brief Displays the recipe costing menu and manages user interactions for creating, editing, and analyzing recipes.
 *
 * @param pathFileIngredients File path to the ingredients data.
 * @param pathFileRecipes File path to the recipes data.
 * @return Always returns 1 when exiting the menu.
 */
int recipeCostingMenu(const char* pathFileIngredients, const char* pathFileRecipes) {
	struct BPlusTreeNode* bptRoot = createNode(1);
	Recipe recipes[MAX_RECIPES];
	int recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);
	for (int i = 0; i < recipeCount; ++i) {
		insert(recipes[i].category, &recipes[i], &bptRoot);
	}

	int choice;
	while (1) {
		clearScreen();
		printf("+--------------------------------------+\n");
		printf("|           RECIPE COSTING MENU        |\n");
		printf("+--------------------------------------+\n");
		printf("| 1. Create Recipe                     |\n");
		printf("| 2. Edit Recipe                       |\n");
		printf("| 3. Calculate Recipe Cost             |\n");
		printf("| 4. Search Recipe by Category         |\n");
		printf("| 5. Analyze Ingredient Usage          |\n");
		printf("| 6. Analyze SCC in Recipe Graph       |\n"); 
		printf("| 7. Exit                              |\n");
		printf("+--------------------------------------+\n");
		printf("Enter your choice: ");

		choice = getInput();
		if (choice == -2) {
			handleInputError();
			enterToContinue();
			continue;
		}

		switch (choice) {
		case 1:
			createRecipe(pathFileIngredients, pathFileRecipes);
			recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);
			bptRoot = createNode(1); // Reset the tree to avoid duplicate entries
			for (int i = 0; i < recipeCount; ++i) {
				insert(recipes[i].category, &recipes[i], &bptRoot);
			}
			break;
		case 2:
			editRecipe(pathFileRecipes, pathFileIngredients);
			recipeCount = loadRecipesFromFile(pathFileRecipes, recipes, MAX_RECIPES);
			bptRoot = createNode(1); // Reset the tree to avoid duplicate entries
			for (int i = 0; i < recipeCount; ++i) {
				insert(recipes[i].category, &recipes[i], &bptRoot);
			}
			break;
		case 3:
			calculateRecipeCost(pathFileRecipes, pathFileIngredients);
			break;
		case 4: {
			int category;
			printf("Enter category to search (1: Soup, 2: Appetizer, 3: Main Course, 4: Dessert): ");
			category = getInput();
			if (category < 1 || category > 4) {
				printf("Invalid category choice.\n");
				enterToContinue();
			}
			else {
				search(category, bptRoot);
				enterToContinue();
			}
			break;
		}
		case 5:
			analyzeIngredientUsage(pathFileRecipes, pathFileIngredients);
			break;
		case 6:
			analyzeSCC(pathFileRecipes); // Call the new SCC function
			break;
		case 7:
			return 1;
		default:
			clearScreen();
			printf("Invalid choice. Please try again.\n");
			enterToContinue();
			break;
		}
	}
}