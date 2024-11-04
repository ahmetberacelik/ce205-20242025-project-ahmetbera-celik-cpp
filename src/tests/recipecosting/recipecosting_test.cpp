#include "gtest/gtest.h"
#include "../header/recipecosting.h"  /
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
        createTestIngredientsFile();
        createTestRecipesFile();
        createIngredientBinFile();
    }

    void TearDown() override {
        remove(testPathFileIngredients);
        remove(testPathFileRecipes);
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

        FILE* file = fopen(testPathFileIngredients, "wb");
        if (file) {
            fwrite(ingredients, sizeof(Ingredient), 5, file);
            fclose(file);
        }
    }

    void createTestRecipesFile() {
        Recipe recipes[] = {
            {"Recipe1", 1, {1, 2}, 2},
            {"Recipe2", 2, {3, 4}, 2}
        };

        FILE* file = fopen(testPathFileRecipes, "w");
        if (file) {
            for (int i = 0; i < 2; ++i) {
                fprintf(file, "%s\n", recipes[i].name);
                fprintf(file, "%d\n", recipes[i].category);
                for (int j = 0; j < recipes[i].ingredientCount; ++j) {
                    fprintf(file, "%d ", recipes[i].ingredients[j]);
                }
                fprintf(file, "\n");
            }
            fclose(file);
        }
    }

    void createIngredientBinFile() {
        Ingredient ingredients[] = {
            {1, "Ingredient1", 1.0},
            {2, "Ingredient2", 2.0},
            {3, "Ingredient3", 3.0},
            {4, "Ingredient4", 4.0},
            {5, "Ingredient5", 5.0}
        };

        FILE* file = fopen(ingredientBinFile, "wb");
        if (file) {
            fwrite(ingredients, sizeof(Ingredient), 5, file);
            fclose(file);
        }
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

    EXPECT_EQ(result, -2);
}


int main(int argc, char** argv) {
#ifdef ENABLE_RECIPECOSTING_TEST
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#else
    return 0;
#endif
}
