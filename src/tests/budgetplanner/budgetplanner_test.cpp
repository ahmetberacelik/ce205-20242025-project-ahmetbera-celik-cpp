#include "gtest/gtest.h"
#include "../../budgetplanner/header/budgetplanner.h"  // Adjust this include path based on your project structure
#include <recipecosting.h>
#include <ingredientmanagement.h>

class BudgetplannerTest : public ::testing::Test {
protected:
	const char* inputTest = "inputTest.bin";
	const char* outputTest = "outputTest.bin";
	const char* recipesFilePath = "test_recipes.bin";
	const char* ingredientsFilePath = "test_ingredients.bin";

	void SetUp() override {
		// Setup test data
		remove(inputTest);
		remove(outputTest);
		remove(recipesFilePath);
		remove(ingredientsFilePath);
	}

	void TearDown() override {
		remove(inputTest);
		remove(outputTest);
		remove(recipesFilePath);
		remove(ingredientsFilePath);
	}

	void simulateUserInput(const char* userInput) {
		FILE* fileinput = fopen(inputTest, "wb");
		fputs(userInput, fileinput);
		fclose(fileinput);
		freopen(inputTest, "rb", stdin);
		freopen(outputTest, "wb", stdout);
	}

	void readOutput(char* buffer, size_t bufferSize) {
		FILE* fileoutput = fopen(outputTest, "rb");
		size_t charsRead = fread(buffer, sizeof(char), bufferSize - 1, fileoutput);
		fclose(fileoutput);
		buffer[charsRead] = '\0';
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
	void createIngredients() {
		Ingredient ingredients[] = {
			{1, "Ingredient1", 1.0},
			{2, "Ingredient2", 2.0},
			{3, "Ingredient3", 3.0},
			{4, "Ingredient4", 4.0},
			{5, "Ingredient5", 5.0}
		};

		Ingredient* head = NULL; // Baðlý liste baþlangýcý
		for (int i = 0; i < 5; ++i) {
			head = addIngredient(head, ingredients[i].name, ingredients[i].price, ingredientsFilePath);
		}
	}

	void createRecipes() {
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
		saveRecipesToFile(recipesFilePath, recipes, recipeCount);
	}
};


TEST_F(BudgetplannerTest, BudgetPlannerMenuTest) {
	// Simulate user input for budget and menu choices
	simulateUserInput("1500\n1\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuViewBudget) {
	// Simulate user input for budget and menu choices
	simulateUserInput("1500\n2\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuInvalidBudget) {
	// Simulate user input for budget and menu choices
	simulateUserInput("-1500\n1500\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuInvalidChoice) {
	// Simulate user input for budget and menu choices
	simulateUserInput("1500\n4\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuInvalidChoice2) {
	// Simulate user input for budget and menu choices
	simulateUserInput("1500\n-4\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuInvalidChoice3) {
	// Simulate user input for budget and menu choices
	simulateUserInput("1500\nqwe\n\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuSuccess) {
	// Simulate user input for budget and menu choices
	createIngredients();
	createRecipes();
	simulateUserInput("1500\n1\n1\n-1\nqwe\ndone\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
TEST_F(BudgetplannerTest, BudgetPlannerMenuNotValidReciepe) {
	// Simulate user input for budget and menu choices
	createIngredients();
	createRecipes();
	simulateUserInput("1500\n1\n1\n-1\nqwe\ndone\n\n3\n");

	// Call the function to test
	int result = budgetPlannerMenu(recipesFilePath, ingredientsFilePath);

	// Reset stdin and stdout
	resetStdinStdout();

	// Check the function result
	EXPECT_EQ(result, 1);
}
/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char** argv) {
#ifdef ENABLE_BUDGETPLANNER_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}