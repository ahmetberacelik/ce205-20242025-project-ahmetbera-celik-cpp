#include "gtest/gtest.h"
#include "../header/recipecosting.h"  
#include "../header/ingredientmanagement.h" 
#include "../header/userauthentication.h"
#include <cstdio>  
#include <cstring> 

class RecipeCostingTest : public ::testing::Test {
protected:
	const char* testPathFileIngredients = "test_ingredients.bin";
	const char* testPathFileRecipes = "test_recipes.bin";
	const char* inputTest = "inputTest.txt";
	const char* outputTest = "outputTest.txt";
	const char* ingredientBinFile = "ingredient.bin";

	void SetUp() override {
		// Create dummy ingredient file for testing
		remove(testPathFileIngredients);
		remove(testPathFileRecipes);
		createTestIngredientsFile();
		createTestRecipesFile();
	}

	void TearDown() override {
		remove(inputTest);
		remove(outputTest);
	}

	void simulateUserInput(const char* userInput) {
		FILE* fileinput = fopen(inputTest, "wb");
		fputs(userInput, fileinput);
		fclose(fileinput);
		freopen(inputTest, "rb", stdin);
		freopen(outputTest, "wb", stdout);
	}

	void resetStdinStdout() {
		fclose(stdin);
		fflush(stdout);
#ifdef _WIN32
		freopen("CON", "a", stdout);
		freopen("CON", "r", stdin);
#else
		freopen("/dev/tty", "a", stdout);
		freopen("/dev/tty", "r", stdin);
#endif // _WIN32
	}

	void createTestIngredientsFile() {
		Ingredient ingredients[] = {
			{1, "Ingredient1", 1.0},
			{2, "Ingredient2", 2.0},
			{3, "Ingredient3", 3.0},
			{4, "Ingredient4", 4.0},
			{5, "Ingredient5", 5.0}
		};

		Ingredient* head = NULL; // Baðlý liste baþlangýcý
		for (int i = 0; i < 5; ++i) {
			head = addIngredient(head, ingredients[i].name, ingredients[i].price, testPathFileIngredients);
		}
	}
	void createTestRecipesFile() {
		Recipe recipes[MAX_RECIPES];
		int recipeCount = 0; 

		// Creating test recipes
		const char* recipeNames[] = { "Recipe1", "Recipe2" };
		int categories[] = { 1, 2 }; // Category ýds
		int testIngredients[][MAX_INGREDIENTS] = { {1, 2}, {3, 4} };
		int ingredientCounts[] = { 2, 2 }; // Every recipe has 2 ingredients

		for (int i = 0; i < 2; ++i) {
			// Copy recipe data
			strcpy(recipes[recipeCount].name, recipeNames[i]);
			recipes[recipeCount].category = categories[i];
			recipes[recipeCount].ingredientCount = ingredientCounts[i];
			memcpy(recipes[recipeCount].ingredients, testIngredients[i], ingredientCounts[i] * sizeof(int));
			recipeCount++;
		}

		//Save recipes to file
		saveRecipesToFile(testPathFileRecipes, recipes, recipeCount);
	}
};

TEST_F(RecipeCostingTest, CreateRecipe_Success) {
	const char* inputSequence = "Test Recipe\n2\n1\ndone\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = createRecipe(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, CreateRecipe_InvalidCategory) {
	const char* inputSequence = "Test Recipe\n5\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = createRecipe(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, CreateRecipe_NoIngredientsSelected) {
	const char* inputSequence = "Test Recipe\n1\ndone\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = createRecipe(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, CreateRecipe_InvalidIngredientId) {
	const char* inputSequence = "Test Recipe\n1\n999\ndone\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = createRecipe(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, CreateRecipe_DuplicateIngredientIds) {
	const char* inputSequence = "Test Recipe\n1\n1\n1\ndone\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = createRecipe(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, EditRecipe_Success) {
	const char* inputSequence = "1\n1\nNew Recipe Name\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, EditRecipe_InvalidRecipeId) {
	const char* inputSequence = "10\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, -2);
}

TEST_F(RecipeCostingTest, EditRecipe_InvalidCategory) {
	const char* inputSequence = "1\n2\n5\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, -3);
}

TEST_F(RecipeCostingTest, EditRecipe_AddIngredients) {
	const char* inputSequence = "1\n3\n1\n1\ndone\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, EditRecipe_RemoveIngredients) {
	const char* inputSequence = "1\n3\n2\n1\ndone\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, EditRecipe_NoRecipesAvailable) {
	remove(testPathFileRecipes); // Remove recipes file to simulate no recipes available
	const char* inputSequence = "\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, -1);
}

TEST_F(RecipeCostingTest, CalculateRecipeCost_Success) {
	const char* inputSequence = "1\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = calculateRecipeCost(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, CalculateRecipeCost_NoRecipesAvailable) {
	std::remove(testPathFileRecipes); // Remove recipes file to simulate no recipes available
	const char* inputSequence = "\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = calculateRecipeCost(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, -1);
}

TEST_F(RecipeCostingTest, CalculateRecipeCost_InvalidRecipeId) {
	const char* inputSequence = "10\n\n\n\n";
	simulateUserInput(inputSequence);

	int result = calculateRecipeCost(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, -2);
}

TEST_F(RecipeCostingTest, PrintRecipesToConsole_Success) {
	simulateUserInput("\n\n\n");

	int result = printRecipesToConsole(testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, PrintRecipesToConsole_ErrorOpeningFile) {
	std::remove(testPathFileRecipes); // Remove recipes file to simulate error opening file
	simulateUserInput("\n\n\n");

	int result = printRecipesToConsole(testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, -1);
}

TEST_F(RecipeCostingTest, PrintRecipesToConsole_ErrorLoadingIngredients) {
	std::remove(testPathFileIngredients); // Remove ingredients file to simulate error loading ingredients
	simulateUserInput("\n\n\n");

	int result = printRecipesToConsole(testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, PrintRecipesToConsole_NoRecipes) {
	std::remove(testPathFileRecipes); // Remove recipes file to simulate no recipes available
	simulateUserInput("");

	int result = printRecipesToConsole(testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, -1);
}

TEST_F(RecipeCostingTest, ListRecipesName_Success) {
	simulateUserInput("\n\n\n");

	int result = listRecipesName(testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, ListRecipesName_ErrorOpeningFile) {
	std::remove(testPathFileRecipes); // Remove recipes file to simulate error opening file
	simulateUserInput("\n\n\n");

	int result = listRecipesName(testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, -1);
}

TEST_F(RecipeCostingTest, LoadRecipesFromFile_Success) {
	Recipe recipes[MAX_RECIPES];
	int result = loadRecipesFromFile(testPathFileRecipes, recipes, MAX_RECIPES);

	EXPECT_EQ(result, 2);
}

TEST_F(RecipeCostingTest, LoadRecipesFromFile_ErrorOpeningFile) {
	remove(testPathFileRecipes); // Remove recipes file to simulate error opening file
	Recipe recipes[MAX_RECIPES];

	int result = loadRecipesFromFile(testPathFileRecipes, recipes, MAX_RECIPES);

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, recipeCostingMenu_ALL) {
	const char* inputSequence = "10\n\n7\n5\n4\n\n";
	simulateUserInput(inputSequence);

	int result = recipeCostingMenu(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, recipeCostingMenu_ALL1) {
	const char* inputSequence = "1\nqwe\n1\n2\n55\n1\ndone\n\n2\n1\n1\nqwer\n\n2\n1\n2\n3\n\n3\n1\n\n4\n1\n\n4\n2\n\n4\n3\n\n4\n4\n\n5\n1\n\n5\n2\n\n7\n5\n4\n\n";
	simulateUserInput(inputSequence);

	int result = recipeCostingMenu(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, recipeCostingMenu_ALL2) {
	const char* inputSequence = "1\ndfg\n1\n1\n1\ndone\n\n2\n1\n3\n1\n1\n2\n55\ndone\n\n2\n1\n3\n2\n1\n2\n55\ndone\n\n7\n5\n4\n\n";
	simulateUserInput(inputSequence);

	int result = recipeCostingMenu(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, InsertInternal_SplitNode) {
	// Set up B+ tree root and nodes
	BPlusTreeNode* root = createNode(1);
	Recipe recipe1 = { "Recipe1", 1, {1, 2}, 2 };
	Recipe recipe2 = { "Recipe2", 2, {3, 4}, 2 };
	Recipe recipe3 = { "Recipe3", 3, {5}, 1 };

	// Insert initial recipes to fill the node
	insertInternal(10, &recipe1, root, &root);
	insertInternal(20, &recipe2, root, &root);
	insertInternal(30, &recipe3, root, &root);

	// Insert additional key to force a split
	Recipe recipe4 = { "Recipe4", 4, {1, 3}, 2 };
	insertInternal(25, &recipe4, root, &root);

	// Verify root and child node keys after split
	EXPECT_EQ(root->keyCount, 4);
}

TEST_F(RecipeCostingTest, InsertInternal_ForceFullNodeSplit) {
	// Set up B+ tree root and nodes
	BPlusTreeNode* root = createNode(1);
	Recipe recipes[MAX_RECIPES + 1];
	for (int i = 0; i <= MAX_RECIPES; ++i) {
		recipes[i] = { "Recipe", i + 1, {1, 2}, 2 };
	}

	// Insert keys to fill the node to its maximum capacity
	for (int i = 0; i < MAX_RECIPES; ++i) {
		insertInternal(i * 10, &recipes[i], root, &root);
	}

	// Insert one more key to force the split
	insertInternal(MAX_RECIPES * 10, &recipes[MAX_RECIPES], root, &root);

	// Verify root and child node keys after split
	EXPECT_EQ(root->keyCount, 1);
}

TEST_F(RecipeCostingTest, PrintRecipesToConsole_Successsss) {
	// Redirect stdout to capture the output
	std::ostringstream capturedOutput;
	std::streambuf* originalCoutBuffer = std::cout.rdbuf();
	std::cout.rdbuf(capturedOutput.rdbuf());

	// Call the function to print recipes
	int result = printRecipesToConsole(testPathFileRecipes);

	// Restore the original stdout
	std::cout.rdbuf(originalCoutBuffer);

	// Verify the output
	std::string output = capturedOutput.str();
	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, FindParent_Node) {
	// Set up B+ tree root and nodes
	BPlusTreeNode* root = createNode(0);
	BPlusTreeNode* child1 = createNode(1);
	BPlusTreeNode* child2 = createNode(1);

	root->children[0] = child1;
	root->children[1] = child2;
	root->keys[0] = 10;
	root->keyCount = 1;

	// Test finding the parent of child1
	BPlusTreeNode* parent = findParent(root, child1);

	// Test finding the parent of child2
	parent = findParent(root, child2);

	// Test finding parent of a node not in the tree
	BPlusTreeNode* nonExistentChild = createNode(1);
	parent = findParent(root, nonExistentChild);
	EXPECT_EQ(parent, nullptr);
}

TEST_F(RecipeCostingTest, EditRecipe_InvalidIngredientChoice) {
	simulateUserInput("1\ninvalid\n\n");

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, -5);
}

TEST_F(RecipeCostingTest, EditRecipe_InvalidEditChoice) {
	simulateUserInput("1\n3\n2\nasdasd\nasdasd\n.efasd\ndone\n\n");

	int result = editRecipe(testPathFileRecipes, testPathFileIngredients);
	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(RecipeCostingTest, recipeCostingMenu_SCC) {
	const char* inputSequence = "6\n\n7\n5\n4\n\n";
	simulateUserInput(inputSequence);

	int result = recipeCostingMenu(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}

TEST_F(RecipeCostingTest, recipeCostingMenu_SCC2) {
	const char* inputSequence = "1\nasd\n1\n1\n1\n2\ndone\n\n1\nqwe\n2\n1\n1\n2\ndone\n\n1\nrty\n3\n1\n1\n2\ndone\n\n1\nfgh\n4\n1\n1\n2\ndone\n\n6\n\n7\n5\n4\n\n";
	simulateUserInput(inputSequence);

	int result = recipeCostingMenu(testPathFileIngredients, testPathFileRecipes);
	resetStdinStdout();

	EXPECT_EQ(result, 1);
}



int main(int argc, char** argv) {
#ifdef ENABLE_RECIPECOSTING_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}
