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
		// Ingredients are double linked list. Create 5 ingredients and link them.
		Ingredient* head = NULL;
		head = addIngredient(head, "Ingredient1", 1.0, ingredientsFilePath);
		head = addIngredient(head, "Ingredient2", 2.0, ingredientsFilePath);
		head = addIngredient(head, "Ingredient3", 3.0, ingredientsFilePath);
		head = addIngredient(head, "Ingredient4", 4.0, ingredientsFilePath);
		head = addIngredient(head, "Ingredient5", 5.0, ingredientsFilePath);
		saveIngredientsToFile(head, ingredientsFilePath);
	}

	void createRecipes() {
		// Create 3 recipes and save them to the file.
		FILE* file = fopen(recipesFilePath, "wb");

		Recipe recipe1;
		strncpy(recipe1.name, "Recipe1", sizeof(recipe1.name) - 1);
		recipe1.name[sizeof(recipe1.name) - 1] = '\0';
		recipe1.category = 1;
		recipe1.ingredientCount = 2;
		recipe1.ingredients[0] = 1;
		recipe1.ingredients[1] = 2;
		fwrite(&recipe1, sizeof(Recipe), 1, file);

		Recipe recipe2;
		strncpy(recipe2.name, "Recipe2", sizeof(recipe2.name) - 1);
		recipe2.name[sizeof(recipe2.name) - 1] = '\0';
		recipe2.category = 2;
		recipe2.ingredientCount = 3;
		recipe2.ingredients[0] = 3;
		recipe2.ingredients[1] = 4;
		recipe2.ingredients[2] = 5;
		fwrite(&recipe2, sizeof(Recipe), 1, file);

		Recipe recipe3;
		strncpy(recipe3.name, "Recipe3", sizeof(recipe3.name) - 1);
		recipe3.name[sizeof(recipe3.name) - 1] = '\0';
		recipe3.category = 3;
		recipe3.ingredientCount = 3;
		recipe3.ingredients[0] = 1;
		recipe3.ingredients[1] = 3;
		recipe3.ingredients[2] = 5;
		fwrite(&recipe3, sizeof(Recipe), 1, file);

		fclose(file);
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