#include "gtest/gtest.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"
#include "gtest/gtest.h"
#include "../../ingredientmanagement/header/ingredientmanagement.h"

class IngredientmanagementTest : public ::testing::Test {
protected:
	const char* inputTest = "inputTest.bin";
	const char* outputTest = "outputTest.bin";
	const char* testFilePath = "testdeneme.bin";
	void SetUp() override {
		// Setup test data
	}

	void TearDown() override {
		remove(inputTest);
		remove(outputTest);
		remove(testFilePath);
	}

	void simulateUserInput(const char* userInput) {
		FILE* fileinput = fopen(inputTest, "wb");
		fputs(userInput, fileinput);
		fclose(fileinput);
		freopen(inputTest, "rb", stdin);
		freopen(outputTest, "wb", stdout);
	}

	void readOutput(const char* outputFilePath, char* buffer, size_t bufferSize) {
		FILE* fileoutput = fopen(outputFilePath, "rb");
		size_t charsRead = fread(buffer, sizeof(char), bufferSize - 1, fileoutput);
		fclose(fileoutput);
		buffer[charsRead] = '\0';
		removeClearScreenCharsFromOutputFile(buffer);
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

	void removeClearScreenCharsFromOutputFile(char* str) {
		char* src = str;
		char* dst = str;
		while (*src) {
			if (*src != '\f') {
				*dst++ = *src;
			}
			src++;
		}
		*dst = '\0';
	}
};

TEST_F(IngredientmanagementTest, IngredientmanagementExitMenuTest) {
	simulateUserInput("5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientmanagementInvalidInputTest) {
	simulateUserInput("sdafaf\n\n\n5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientmanagementInvalidNumberTest) {
	simulateUserInput("45454545\n\n5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientViewingNextPreviousTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	head = addIngredient(head, "Onion", 1.2, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Test navigation from the first ingredient to the next and then back to the previous
	simulateUserInput("1\n1\n1\n2\n3\n5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

	char buffer[1024];
	readOutput(outputTest, buffer, sizeof(buffer));
	EXPECT_NE(strstr(buffer, "ID: 2"), nullptr); // Should navigate to the second ingredient (Cucumber)
	EXPECT_NE(strstr(buffer, "ID: 3"), nullptr); // Should navigate to the third ingredient (Onion)
	EXPECT_NE(strstr(buffer, "ID: 2"), nullptr); // Should navigate back to the second ingredient

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for viewing ingredients in the menu
TEST_F(IngredientmanagementTest, IngredientmanagementMenuViewIngredientsTest) {
	simulateUserInput("1\n3\n5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientmanagementMenuAddIngredientTest) {
	simulateUserInput("2\nTomato\n10\n\n5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

	// Verify the ingredient was added
	Ingredient* head = loadIngredientsFromFile(testFilePath);
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Tomato");
	EXPECT_FLOAT_EQ(head->price, 10);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for ingredient management menu case 4 (Edit Ingredient)


TEST_F(IngredientmanagementTest, IngredientmanagementMenuInvalidChoiceTest) {
	simulateUserInput("-2\n\n\n5\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

}



/**
 * @brief The main function of the test program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return int The exit status of the program.
 */
int main(int argc, char** argv) {
#ifdef ENABLE_INGREDIENTMANAGEMENT_TEST
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else
	return 0;
#endif
}