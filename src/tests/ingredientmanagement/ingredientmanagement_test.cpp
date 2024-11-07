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
	simulateUserInput("6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientmanagementInvalidInputTest) {
	simulateUserInput("sdafaf\n\n\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientmanagementInvalidNumberTest) {
	simulateUserInput("45454545\n\n6\n\n");

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
	simulateUserInput("1\n1\n1\n2\n3\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);


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
	simulateUserInput("1\n3\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);
}

TEST_F(IngredientmanagementTest, IngredientmanagementMenuAddIngredientTest) {
	simulateUserInput("2\nTomato\n10\n\n6\n\n");

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
	simulateUserInput("-2\n\n\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

}


// Test for invalid ingredient ID input when viewing ingredients
TEST_F(IngredientmanagementTest, IngredientmanagementMenuViewInvalidInputTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	simulateUserInput("1\n-2\n\n\n3\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for invalid name input during ingredient addition
TEST_F(IngredientmanagementTest, AddIngredientInvalidNameTest) {
	simulateUserInput("2\nTomato123\n\nValidName\n2.5\n\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

	char buffer[1024];
	readOutput(outputTest, buffer, sizeof(buffer));
	EXPECT_NE(strstr(buffer, "Invalid ingredient name. Please enter a valid name without numbers."), nullptr);
}

// Test for invalid price input during ingredient addition
TEST_F(IngredientmanagementTest, AddIngredientInvalidPriceTest) {
	simulateUserInput("2\nTomato\ninvalidprice\n\n2.5\n\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

	char buffer[1024];
	readOutput(outputTest, buffer, sizeof(buffer));
	EXPECT_NE(strstr(buffer, "Invalid price. Please enter a valid positive number."), nullptr);
}


// Test for invalid input in ingredient viewing navigation
TEST_F(IngredientmanagementTest, IngredientViewingInvalidNavigationTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	simulateUserInput("1\n10\n\n3\n6\n\n");

	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	EXPECT_EQ(result, 0);

	char buffer[1024];
	readOutput(outputTest, buffer, sizeof(buffer));
	EXPECT_NE(strstr(buffer, "Invalid choice or no more ingredients in that direction."), nullptr);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, AddIngredientTest) {
	Ingredient* head = nullptr;
	const char* name = "Tomato";
	float price = 2.5;

	head = addIngredient(head, name, price, testFilePath);

	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Tomato");
	EXPECT_FLOAT_EQ(head->price, 2.5);
	EXPECT_EQ(head->next, nullptr);
	EXPECT_EQ(head->prev, nullptr);

	// Clean up
	free(head);
}

// Test for saving ingredients to file
TEST_F(IngredientmanagementTest, SaveIngredientsToFileTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	bool result = saveIngredientsToFile(head, testFilePath);

	EXPECT_TRUE(result);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for listing ingredients using DLL
TEST_F(IngredientmanagementTest, ListIngredientsDLLTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	simulateUserInput("");

	bool result = listIngredientsDLL(head);

	resetStdinStdout();

	EXPECT_TRUE(result);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for listing ingredients using DLL when no ingredients are available
TEST_F(IngredientmanagementTest, ListIngredientsDLLNoIngredientsTest) {
	Ingredient* head = nullptr;

	simulateUserInput("");

	bool result = listIngredientsDLL(head);

	resetStdinStdout();

	EXPECT_FALSE(result);

	char buffer[1024];
	readOutput(outputTest, buffer, sizeof(buffer));
	EXPECT_NE(strstr(buffer, "No ingredients available."), nullptr);
}

TEST_F(IngredientmanagementTest, ListIngredientsXLLTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	simulateUserInput("");

	bool result = listIngredientsXLL(head);

	resetStdinStdout();

	EXPECT_TRUE(result);


	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for listing ingredients using XLL when no ingredients are available
TEST_F(IngredientmanagementTest, ListIngredientsXLLNoIngredientsTest) {
	Ingredient* head = nullptr;

	simulateUserInput("");

	bool result = listIngredientsXLL(head);

	resetStdinStdout();

	EXPECT_FALSE(result);

	char buffer[1024];
	readOutput(outputTest, buffer, sizeof(buffer));
	EXPECT_NE(strstr(buffer, "No ingredients available."), nullptr);
}


// Test for listing ingredients
TEST_F(IngredientmanagementTest, ListIngredientsTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	// Test for DLL choice
	simulateUserInput("1\n");

	bool result = listIngredients(head);

	resetStdinStdout();

	EXPECT_TRUE(result);

	// Test for XLL choice
	simulateUserInput("2\n");

	result = listIngredients(head);

	resetStdinStdout();

	EXPECT_TRUE(result);


	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for invalid input handling in listIngredients function
TEST_F(IngredientmanagementTest, ListIngredientsInvalidInputTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);

	// Test for invalid input
	simulateUserInput("-2\n\n\n");

	bool result = listIngredients(head);

	resetStdinStdout();

	EXPECT_FALSE(result);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for input error handling in listIngredients function
TEST_F(IngredientmanagementTest, ListIngredientsInputErrorHandlingTest) {
	Ingredient* head = nullptr;

	// Test for input error
	simulateUserInput("4545\n\n\n");

	bool result = listIngredients(head);

	resetStdinStdout();

	EXPECT_FALSE(result);

}

// Test for loading ingredients from file
TEST_F(IngredientmanagementTest, LoadIngredientsFromFileTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	saveIngredientsToFile(head, testFilePath);

	// Load ingredients from file
	Ingredient* loadedHead = loadIngredientsFromFile(testFilePath);

	ASSERT_NE(loadedHead, nullptr);
	EXPECT_EQ(loadedHead->id, 1);
	EXPECT_STREQ(loadedHead->name, "Tomato");
	EXPECT_FLOAT_EQ(loadedHead->price, 2.5);

	Ingredient* second = loadedHead->next;
	ASSERT_NE(second, nullptr);
	EXPECT_EQ(second->id, 2);
	EXPECT_STREQ(second->name, "Cucumber");
	EXPECT_FLOAT_EQ(second->price, 1.8);

	EXPECT_EQ(second->next, nullptr);

	// Clean up
	Ingredient* temp;
	while (loadedHead != nullptr) {
		temp = loadedHead;
		loadedHead = loadedHead->next;
		free(temp);
	}
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for removing an ingredient
TEST_F(IngredientmanagementTest, RemoveIngredientTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	simulateUserInput("\n");
	// Remove the first ingredient
	head = removeIngredient(head, 1, testFilePath);

	resetStdinStdout();

	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 2);
	EXPECT_STREQ(head->name, "Cucumber");
	EXPECT_FLOAT_EQ(head->price, 1.8);
	EXPECT_EQ(head->next, nullptr);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for removing an ingredient when the list is empty
TEST_F(IngredientmanagementTest, RemoveIngredientEmptyListTest) {
	Ingredient* head = nullptr;

	simulateUserInput("\n");

	head = removeIngredient(head, 1, testFilePath);

	resetStdinStdout();

	EXPECT_EQ(head, nullptr);

}

// Test for removing a non-existent ingredient
TEST_F(IngredientmanagementTest, RemoveNonExistentIngredientTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	// Attempt to remove an ingredient with an ID that does not exist
	simulateUserInput("\n");

	head = removeIngredient(head, 3, testFilePath);

	resetStdinStdout();

	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Tomato");
	EXPECT_FLOAT_EQ(head->price, 2.5);
	EXPECT_NE(head->next, nullptr);
	EXPECT_EQ(head->next->id, 2);
	EXPECT_STREQ(head->next->name, "Cucumber");


	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for removing a middle ingredient
TEST_F(IngredientmanagementTest, RemoveMiddleIngredientTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	head = addIngredient(head, "Onion", 1.2, testFilePath);

	simulateUserInput("\n");

	// Remove the middle ingredient (Cucumber)
	head = removeIngredient(head, 2, testFilePath);

	resetStdinStdout();

	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Tomato");
	EXPECT_FLOAT_EQ(head->price, 2.5);
	EXPECT_NE(head->next, nullptr);
	EXPECT_EQ(head->next->id, 3);
	EXPECT_STREQ(head->next->name, "Onion");
	EXPECT_FLOAT_EQ(head->next->price, 1.2);
	EXPECT_EQ(head->next->prev, head);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for viewing ingredients in the list after removing an element
TEST_F(IngredientmanagementTest, RemoveIngredientPrevPointerUpdateTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	head = addIngredient(head, "Onion", 1.2, testFilePath);

	simulateUserInput("\n");

	// Remove the first ingredient (Tomato)
	head = removeIngredient(head, 1, testFilePath);

	resetStdinStdout();

	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 2);
	EXPECT_STREQ(head->name, "Cucumber");
	EXPECT_FLOAT_EQ(head->price, 1.8);
	EXPECT_NE(head->next, nullptr);
	EXPECT_EQ(head->next->id, 3);
	EXPECT_STREQ(head->next->name, "Onion");
	EXPECT_EQ(head->next->prev, head);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for editing an ingredient
TEST_F(IngredientmanagementTest, EditIngredientTest) {
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);

	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to edit the ingredient with ID 1 (Tomato) to "Lettuce"
	simulateUserInput("1\nLettuce\n\n\n");

	head = editIngredient(head, testFilePath);

	resetStdinStdout();

	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_FLOAT_EQ(head->price, 2.5);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

// Test for editing an ingredient when no ingredients are available
TEST_F(IngredientmanagementTest, EditIngredientNoIngredientsTest) {
	Ingredient* head = nullptr;

	simulateUserInput("\n");

	head = editIngredient(head, testFilePath);

	resetStdinStdout();

	EXPECT_EQ(head, nullptr);

}


TEST_F(IngredientmanagementTest, EditIngredientValidTest) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to edit the ingredient with ID 1 (Tomato) to "Lettuce"
	simulateUserInput("1\n1\nLettuce\n\n");

	// Call the editIngredient function
	head = editIngredient(head, testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient was updated correctly
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Lettuce");
	EXPECT_FLOAT_EQ(head->price, 2.5);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}


TEST_F(IngredientmanagementTest, EditNonExistentIngredientTest) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to try editing a non-existent ingredient with ID 3
	simulateUserInput("1\n3\n\n");

	// Call the editIngredient function
	head = editIngredient(head, testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient list has not changed
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Tomato");
	EXPECT_FLOAT_EQ(head->price, 2.5);
	EXPECT_EQ(head->next, nullptr);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, EditIngredientInvalidNameTest) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input with an invalid name (containing digits) and then a valid name
	simulateUserInput("1\n1\nTomato123\n\nValidName\n\n");

	// Call the editIngredient function
	head = editIngredient(head, testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient name was updated correctly
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "ValidName");
	EXPECT_FLOAT_EQ(head->price, 2.5);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, RemoveIngredientValidTest) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to remove the ingredient with ID 1 (Tomato)
	simulateUserInput("3\n1\n1\n\n6\n\n");

	// Call the ingredientManagementMenu function
	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient was removed correctly
	ASSERT_EQ(result, 0);
	head = loadIngredientsFromFile(testFilePath);
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 2);
	EXPECT_STREQ(head->name, "Cucumber");
	EXPECT_FLOAT_EQ(head->price, 1.8);
	EXPECT_EQ(head->next, nullptr);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, IngredientmanagementMenuEditIngredientTest) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to edit the ingredient with ID 1 (Tomato) to "Lettuce"
	simulateUserInput("4\n1\n1\nLettuce\n\n6\n\n");

	// Call the ingredientManagementMenu function
	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient was updated correctly
	ASSERT_EQ(result, 0);
	head = loadIngredientsFromFile(testFilePath);
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Lettuce");
	EXPECT_FLOAT_EQ(head->price, 2.5);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, IngredientmanagementMenuRemoveInvalidIdTest) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to try removing an ingredient with an invalid ID (-2)
	simulateUserInput("3\n1\n-2\n\n\n6\n\n");

	// Call the ingredientManagementMenu function
	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient list has not changed
	ASSERT_EQ(result, 0);
	head = loadIngredientsFromFile(testFilePath);
	ASSERT_NE(head, nullptr);
	EXPECT_EQ(head->id, 1);
	EXPECT_STREQ(head->name, "Tomato");
	EXPECT_FLOAT_EQ(head->price, 2.5);
	EXPECT_EQ(head->next, nullptr);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, IngredientmanagementMenuRemoveInvalidIdTest2) {
	// Setup: Add initial ingredients to the linked list and save to file
	Ingredient* head = nullptr;
	head = addIngredient(head, "Tomato", 2.5, testFilePath);
	saveIngredientsToFile(head, testFilePath);

	// Simulate user input to try removing an ingredient with an invalid ID (-2)
	simulateUserInput("3\n35\n\n6\n\n");

	// Call the ingredientManagementMenu function
	int result = ingredientManagementMenu(testFilePath);

	resetStdinStdout();

	// Verify: Check that the ingredient list has not changed
	ASSERT_EQ(result, 0);
	head = loadIngredientsFromFile(testFilePath);

	// Clean up
	Ingredient* temp;
	while (head != nullptr) {
		temp = head;
		head = head->next;
		free(temp);
	}
}

TEST_F(IngredientmanagementTest, ComputeLPSArrayTest) {
	// Test the computeLPSArray function with different patterns
	const char* pattern1 = "AABAACAABAA";
	int lps1[11];
	computeLPSArray(pattern1, lps1, 11);
	int expectedLps1[] = { 0, 1, 0, 1, 2, 0, 1, 2, 3, 4, 5 };
	for (int i = 0; i < 11; ++i) {
		EXPECT_EQ(lps1[i], expectedLps1[i]) << "LPS mismatch at index " << i;
	}
}

TEST_F(IngredientmanagementTest, KMPSearchTest) {
	// Test the KMPSearch function with different text and pattern combinations
	const char* text1 = "ABABDABACDABABCABAB";
	const char* pattern1 = "ABABCABAB";
	EXPECT_TRUE(KMPSearch(text1, pattern1)) << "Pattern should be found in the text.";

	const char* text2 = "THIS IS A TEST TEXT";
	const char* pattern2 = "TEST";
	EXPECT_TRUE(KMPSearch(text2, pattern2)) << "Pattern should be found in the text.";

	const char* text3 = "AABAACAADAABAABA";
	const char* pattern3 = "AABA";
	EXPECT_TRUE(KMPSearch(text3, pattern3)) << "Pattern should be found in the text.";

	const char* text4 = "ABCDEF";
	const char* pattern4 = "GHI";
	EXPECT_FALSE(KMPSearch(text4, pattern4)) << "Pattern should not be found in the text.";
}

//TEST_F(IngredientmanagementTest, SearchIngredientByKMPTest) {
//	// Setup: Add initial ingredients to the linked list and save to file
//	Ingredient* head = nullptr;
//	head = addIngredient(head, "Tomato", 2.5, testFilePath);
//	head = addIngredient(head, "Cucumber", 1.8, testFilePath);
//	head = addIngredient(head, "Onion", 1.2, testFilePath);
//	saveIngredientsToFile(head, testFilePath);
//
//	// Simulate user input to search for an existing ingredient (Cucumber)
//	simulateUserInput("Cucumber\n\n\n");
//
//	// Call the searchIngredientByKMP function
//	searchIngredientByKMP(head);
//
//	resetStdinStdout();
//
//	// Verify: Check that the ingredient was found
//	char buffer[1024];
//	readOutput(outputTest, buffer, sizeof(buffer));
//	EXPECT_NE(strstr(buffer, "Ingredient found:"), nullptr);
//	EXPECT_NE(strstr(buffer, "Name: Cucumber"), nullptr);
//
//	// Simulate user input to search for a non-existent ingredient (Potato)
//	simulateUserInput("Potato\n");
//
//	// Call the searchIngredientByKMP function
//	searchIngredientByKMP(head);
//
//	resetStdinStdout();
//
//	// Verify: Check that the ingredient was not found
//	readOutput(outputTest, buffer, sizeof(buffer));
//	EXPECT_NE(strstr(buffer, "Ingredient 'Potato' not found in the list."), nullptr);
//
//	// Clean up
//	Ingredient* temp;
//	while (head != nullptr) {
//		temp = head;
//		head = head->next;
//		free(temp);
//	}
//}

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